#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "strbuf.c"
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>

void print(strbuf_t w, int l, unsigned int placeto){ //can't we just do write(placeto, w.data, l)?
    char line[l];
    for(int i = 0; i <l; i++){
        line[i] = w.data[i];
    }
     write(placeto, line, l);
}
strbuf_t newword(strbuf_t w){
    sb_destroy(&w);
    strbuf_t word2;
    sb_init(&word2, 5);
    return word2;
}
int wrapout (unsigned int width, unsigned int placefrom, unsigned int placeto){ // stdout wrap text width fd, 
int space = width, exceed = 0;
char *a = malloc(sizeof(char));
int rval = read(placefrom, a, sizeof(char));
strbuf_t word;
sb_init(&word, 5);  
    while(rval == 1){       
       int wordlength = 0, nl = 0, tab = 0, bl = 0;
       while(isspace(a[0]) == 0 && rval == 1){
            sb_append(&word, a[0]);
            wordlength++;
            rval = read(placefrom, a, sizeof(char));
        }
        if (wordlength <= space){
            space = space - wordlength;
            print(word, wordlength, placeto);
            word = newword(word); // done with the word
        }
        else if (wordlength > space){ //if 
            write(placeto, "\n", 1);// put a newline
            if(wordlength > width){
                exceed++;
                print(word, wordlength, placeto);//write the word one the newline then start a newline 
                write(placeto, "\n", 1);
                break;
            }
            space = width - wordlength;
            print(word, wordlength, placeto);
            word = newword(word); // done with the word
        }
        while(isspace(a[0]) != 0 && rval == 1){// dealing with space characters now
            if (a[0] == '\n'){
                nl++;
            }
            else if (a[0] == '\t'){
                tab++;
            }
            else if (a[0] == ' '){ 
                bl++;
            }
        rval = read(placefrom, a, sizeof(char));

        }
        if(nl > 1){
            for (int p = 0; p < (nl-1); p++){
                write(placeto, "\n", 1);
            }
        }
        if(tab > 0 || bl > 0){
            if(space >= 1){
                write(placeto, " ", 1);
            }
        }     
   }
   free(a);
   sb_destroy(&word);
   close(placefrom);
   return exceed;
}
int printfile(unsigned int width, char* filename, int fd, char* dirname)
{
    strbuf_t newfile;
    sb_init(&newfile, 6);
    sb_concat(&newfile, dirname);
    sb_concat(&newfile, "/wrap.");
    sb_concat(&newfile, filename);
    int newfd = open(newfile.data, O_WRONLY|O_CREAT, 600);
    int error = wrapout(width, fd, newfd);
    write(newfd, "\n", 1);
    sb_destroy(&newfile);
    close(newfd);
    return error;
}
int StartsWith(const char *a, const char *b)
{
    if(strncmp(a, b, strlen(b)) == 0) 
        return 1;
    return 0;
}
int wrapdir (unsigned int width, DIR* dir, char* dirname)
{
    struct dirent *cd;
    int willerror = 0;
    cd = readdir(dir);
    strbuf_t file;
    while(cd != NULL)
    {
        char* f = cd->d_name;
        int wr = startsWith(f, "wrap");
        int pd = startsWith(f, ".");
        if(wr == 0 && pd == 0)
        {
            sb_init(&file, 5);
            sb_concat(&file, dirname);
            sb_append(&file, '/');
            sb_concat(&file, f);
            int fd = open(file.data, O_RDONLY);
            if(fd != -1){
                int error = printfile(width, f, fd, dirname);
                if(error != 0)
                    willerror = 1;
            }
            file = newword(file);
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
    if(argc == 2){ // no file or directory provided  
        int error = wrapout(width, 0, 1);
        write(1, "\n", 1); // made it out anyways
        if(error != 0) 
            return EXIT_FAILURE; // a word had exceeded page length   
    }
    else{
        char* f = argv[2];
        DIR* dir1 = opendir(f);
        if(dir1 == NULL)
        {
            int fd = open(f, O_RDONLY);
            int error = wrapout(width, fd, 1);
            write(1, "\n", 1);
            if(error != 0)
                return EXIT_FAILURE;
        }
        else
        {
            int error = wrapdir(width, dir1, f);
            if(error != 0)
                return EXIT_FAILURE;
        }
    }
return EXIT_SUCCESS;
}
