#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strbuf.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int sb_init(strbuf_t *L, size_t length)
{
    L->data = malloc(sizeof(char) * length);
    if (!L->data) return 1;
    L->data[0] = '\0';
    L->length = length;
    L->used   = 1;

    return 0;
}

void sb_destroy(strbuf_t *L)
{
    free(L->data);
}


int sb_append(strbuf_t *L, char item)
{
    if (sb_extend(L,1) == 1){
        return 1; // if there is one space or no space list is double
     } // checks if there is enough room to add char at the end of list
	if (DEBUG) printf("Increased size to %lu\n", L->length);
     L->data[L->used-1] = item;
     L->data[L->used] = '\0';
     L->used++;


    return 0;
}


int sb_remove(strbuf_t *L, char *item)
{
    if (L->used == 0) return 1;
    --L->used;
    if (item) *item = L->data[L->used];
    L->data[L->used-1] = '\0';
    return 1;
}


int sb_insert(strbuf_t *list, int index, char item){
    if (index < list->length){ // within the length array
	    if(list->used == list->length){ // we need more room first
            if (sb_extend(list,1) == 1) return 1; 
        }        
	    if(index > list->used){ // within the length but no shifting needed
            list->used = index+1;
	        list->data[list->used-1] = item;
     	    list->data[list->used] = '\0';
     	    list->used++;
           return 0;
        }
        char temp1 = list->data[index]; 
	    list->data[index]= item;
        list->used++; // used increments by one
        for(int m = (index + 1); m < list->length; m++){ // used values are contiguous
            if(m == ( (list->used) - 1) ) {
		        list->data[m] = temp1;
             break;   
            }
            else{
                char temp2 = list->data[m];
		        list->data[m] = temp1;
                temp1 = temp2; 
            } 
        }   
    }
    else{
        if (sb_extend(list,index) == 1) return 1; // if needed will extend
        list->used = index + 1;
	    list->data[index]= item;
	    list->data[index+1] = '\0';
	}
    return 0;
}


int sb_concat(strbuf_t *sb, char *str){
    sb_extend(sb,strlen(str)); // if needed will extend arraylist to have enough space
	int m;
	for(m = 0; m < strlen(str); m++){
	   sb_append(sb, str[m]);
	}
    return 0;
}


int sb_extend(strbuf_t *sb, int len){
    int space = sb->length - (sb->used - 1); 
    if(len >= space){ // there is not enough space in list to add string AND end with null terminator
    size_t size = max((sb->used+len), sb->length*2);
	char *p = realloc(sb->data, sizeof(char) * size);
        if (!p) return 1;
    sb->length = size;
	sb->data = p;
    }  
    return 0;  
}
int max(int val1, int val2){ // helper method
    if (val1 > val2) 
        return val1;
    else 
        return val2;
}

