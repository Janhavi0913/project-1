#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
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
int wrapout (strbuf_t input, unsigned int width){ // stdout wrap text
int m = 0, space = width, exceed = 0, nl = 0, tab = 0, bl = 0;
strbuf_t word;
sb_init(&word, 5);  
while(m < input.used){       
       int wordlength = 0, wptr = 0;
       while(isspace(input.data[m]) == 0){
            sb_append(&word, input.data[m]);
            wordlength++;
            m++;
        }
        if (wordlength <= space){
            space = space - wordlength;
            print(word, wordlength);
        }
        else{ //if (wordlength > space)
            write(1, "\n", 1);// put a newline
            if(wordlength > width){
                exceed++;
                print(word, wordlength);//write the word one the newline then start a newline 
                write(1, "\n", 1);
                break;
            }
            space = width - wordlength;
            print(word, wordlength);
        }
        while(isspace(input.data[m]) != 0){// dealing with space characters now
            if (input.data[m] == '\n'){
                nl++;
            }
            else if (input.data[m] == '\t'){
                tab++;
            }
            else if (input.data[m] == ' '){ 
                bl++;
            }
            m++;
        }
        if (nl > 1){
            for (int p = 0; p < (nl-1); p++){
                write(1, "\n", 1);
            }
        }
        if (tab > 0 || bl > 0){
            if(space >= 1)
                write(1, " ", 1);
        }  
        word = newword(word); // done with the word
   }
return exceed;
}

int main(int argc, char **argv){
    if(argc < 2 || argc > 3){ // too many arguments or too little arguments
     printf("Number of argument error\n");
        return EXIT_FAILURE;
    }
    unsigned int width = atoi(argv[1]); 
    if(argc == 2){ // no file or directory provided  
        strbuf_t S;
        sb_init(&S,5);
        char *input = malloc(sizeof(char)*INT_MAX); 
        int rval = read(0, input, INT_MAX);
        sb_concat(&S, input);
        free(input);
        int error = wrapout(S, width);
        sb_destroy(&S);
        if(error != 0) return EXIT_FAILURE; // a word had exceeded page length   
    }
    
return EXIT_SUCCESS;
}
/*
printf("the length of input %ld\n", strlen(input));
        for(int k = 0; k < strlen(input); k++){
            printf("%c", input[k]);
        }
        printf("\n");

        printf("placed in the buffer\n");
        for(int i = 0; i < S.used; ++i){
            printf("%c", S.data[i]);
        }
        printf("\n");
*/