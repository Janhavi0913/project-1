#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "strbuf.c"
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>


void print(strbuf_t w, int l, unsigned int placeto){
char *word = malloc(sizeof(char) * l);
    for(int i = 0; i < l; i++){
        word[i] = w.data[i];
    }
    write(placeto, word, l);
    free(word);
}
strbuf_t newWord(strbuf_t w, int width){
    sb_destroy(&w);
    strbuf_t word2;
    sb_init(&word2, width);
    return word2;
}
int wrapout(unsigned int width, unsigned int placefrom, unsigned int placeto){ 
int exceed = 0, space = width, newLine = 0;
char *a = malloc(sizeof(char));
int rval = read(placefrom, a, sizeof(char));
strbuf_t word; sb_init(&word, width);
    while(isspace(a[0]) != 0 && rval == 1){ // if there is space character before the first word
        rval = read(placefrom, a, sizeof(char));
    }
    while(rval == 1){       
       int wordlength = 0;
       while(isspace(a[0]) == 0 && rval == 1){
            sb_append(&word, a[0]);
            wordlength++;
            rval = read(placefrom, a, sizeof(char));
        }
        if(space == width){
            if(wordlength >= width){
                if(wordlength > width){
                    exceed++;
                }
                print(word,wordlength,placeto);
                word = newWord(word, width);
                write(placeto, "\n", 1);
                space = width;
            } 
            else{
                print(word,wordlength,placeto);
                word = newWord(word, width);
                space = width - wordlength;
            }
        } 
        else if(space != width){
            if(wordlength > width){
                exceed++;
                write(placeto, "\n", 1);
                print(word,wordlength,placeto);
                word = newWord(word, width);
                write(placeto, "\n", 1);
                space = width;
            }else if(wordlength+1 > space){
                write(placeto, "\n", 1);
                print(word,wordlength,placeto);
                word = newWord(word, width);
                space = width - wordlength;
            }else{
                write(placeto, " ", 1);
                print(word,wordlength,placeto);
                word = newWord(word, width);
                space = space - (wordlength + 1);
            }
        }
        while(isspace(a[0]) != 0 && rval == 1){ // dealing with space characters now
            if (a[0] == '\n'){
                newLine++;
            }
            rval = read(placefrom, a, sizeof(char));
        }
        if(newLine >= 2){ // paragraph break
            write(placeto, "\n", 1);
            write(placeto, "\n", 1);
            space = width;
        }
        newLine = 0;
   }
   free(a);
   sb_destroy(&word);
   close(placefrom);
   return exceed;
}

int printfile(unsigned int width, char* filename, int fd, char* dirname){
    strbuf_t newfile;
    sb_init(&newfile, 6);
    sb_concat(&newfile, dirname);
    sb_concat(&newfile, "/wrap.");
    sb_concat(&newfile, filename);
    int newfd = open(newfile.data, O_WRONLY|O_CREAT, 0600);
    int error = wrapout(width, fd, newfd);
    write(newfd, "\n", 1);
    sb_destroy(&newfile);
    close(newfd);
    return error;
}
int startsWith(const char *a, const char *b){
    if(strncmp(a, b, strlen(b)) == 0) 
        return 1;
    return 0;
}
int isdir(char *name) {
    DIR* dir1 = opendir(name);
    if(dir1 == NULL)
        return 0;
    closedir(dir1);
    return 1;
}
int isFileExistsStats(const char *path){
    struct stat stats;
    stat(path, &stats);
    if (S_ISDIR(stats.st_mode)) // Check for file existence
        return 0;
    return 1;
}
int wrapdir (unsigned int width, DIR* dir, char* dirname){
    struct dirent *cd;
    int willerror = 0;
    cd = readdir(dir);
    strbuf_t file;
    while(cd != NULL){
        char* f = cd->d_name;
        sb_init(&file, width);
        sb_concat(&file, dirname);
        sb_append(&file, '/');
        sb_concat(&file, f);
        if(isFileExistsStats(file.data) == 1){
            int wr = startsWith(f, "wrap");
            int pd = startsWith(f, ".");
            if(wr == 0 && pd == 0){
                int fd = open(file.data, O_RDONLY);
                if(fd != -1){
                    int error = printfile(width, f, fd, dirname);
                    if(error != 0)
                        willerror = 1;
                }
                file = newWord(file, width);
            }
        }
        cd = readdir(dir);
    }
    sb_destroy(&file);
    closedir(dir);
    return willerror;
}

int main(int argc, char **argv){
    if(argc < 2){ // too many arguments or too little arguments
        printf("Number of argument error\n");
        return EXIT_FAILURE;
    }
    unsigned int width = atoi(argv[1]); 
    if(width <= 0){
        printf("Inappropriate size of width\n");
        return EXIT_FAILURE;  
    }
    if(argc == 2){ // no file or directory provided  
        int error = wrapout(width, 0, 1);
        write(1, "\n", 1); // made it out anyways
        if(error != 0) 
            return EXIT_FAILURE; // a word had exceeded page length   
    }
    else{
        char* f = argv[2];
        if(isdir(f) == 0){
            int fd = open(f, O_RDONLY);
            if(fd == -1)
                perror(argv[2]);
            int error = wrapout(width, fd, 1);
            if(error != 0)
                return EXIT_FAILURE;
        }
        else{
            DIR* dir1 = opendir(f);
            int error = wrapdir(width, dir1, f);
            if(error != 0)
                return EXIT_FAILURE;
        }
    }
return EXIT_SUCCESS;
}
