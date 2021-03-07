#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "strbuf.c"

void print(strbuf_t w, int l){
    char line[l];
    for(int i = 0; i <l; i++){
        line[i] = w.data[i];
    }
     write(1, line, l);
}
strbuf_t newword(strbuf_t w){
    sb_destroy(&w);
    strbuf_t word2;
    sb_init(&word2, 5);
    return word2;
}
int wrapout (unsigned int width){ // stdout wrap text width fd, 
int space = width, exceed = 0, nl = 0, tab = 0, bl = 0;
char *a = malloc(sizeof(char));
int rval = read(0, a, sizeof(char));
strbuf_t word;
sb_init(&word, 5);  
    while(rval == 1){       
       int wordlength = 0;
       while(isspace(a[0]) == 0 && rval== 1){
            sb_append(&word, a[0]);
            wordlength++;
            rval = read(0, a, sizeof(char));
        }
        if (wordlength <= space){
            space = space - wordlength;
            print(word, wordlength);
            word = newword(word); // done with the word
        }
        else if (wordlength > space){ //if 
            write(1, "\n", 1);// put a newline
            if(wordlength > width){
                exceed++;
                print(word, wordlength);//write the word one the newline then start a newline 
                write(1, "\n", 1);
                break;
            }
            space = width - wordlength;
            print(word, wordlength);
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
        rval = read(0, a, sizeof(char));

        }
        if(nl > 1){
            for (int p = 0; p < (nl-1); p++){
                write(1, "\n", 1);
            }
        }
        if(tab > 0 || bl > 0){
            if(space >= 1){
                write(1, " ", 1);
            }
        }     
   }
   free(a);
   sb_destroy(&word);
   close(0);
   return exceed;
}

int main(int argc, char **argv){
    if(argc < 2 || argc > 3){ // too many arguments or too little arguments
     printf("Number of argument error\n");
        return EXIT_FAILURE;
    }
    unsigned int width = atoi(argv[1]); 
    if(argc == 2){ // no file or directory provided  
        int error = wrapout(width);
        write(1, "\n", 1); // made it out anyways
        if(error != 0) 
            return EXIT_FAILURE; // a word had exceeded page length   
    }
return EXIT_SUCCESS;
}