#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "strbuf.h"

int wrapout (char *str, int width, int strlen){ // stdout wrap text
    char *base,*right_margin;
    base = str;
    while(*base){
        if(strlen <= width){
            puts(base);     /* display string */
            return(0);      /* and leave */
        }
        right_margin = base+width;
        while(!isspace(*right_margin)){
            right_margin--;
            if( right_margin == base){
                right_margin += width;
                while(!isspace(*right_margin)){
                    if( *right_margin == '\0')
                        break;
                    right_margin++;
                }
            }
        }
        *right_margin = '\0';
        puts(base);
        strlen -= right_margin-base+1;      /* +1 for the space */
        base = right_margin+1;
    }
    return 0;
}


int main(int argc, char **argv){
    if(argc > 2 || argc < 1){ // too many arguments or too little arguments
        return EXIT_FAILURE;
    }
    int width = argv[0]; 
    assert(width > 0);  
    if(argc == 1){ // no file or directory provided  
        char *input = NULL; 
        size_t length = 0;
        ssize_t read;
        read = getline(&input, &length, stdin);
        int error = wrapout(input, width, length);
        if(error != 0) return EXIT_FAILURE; // a word had exceeded page length
    }
    
return EXIT_SUCCESS;
}