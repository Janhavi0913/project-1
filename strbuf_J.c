#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strbuf.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int sb_init(strbuf_t *L, size_t length)
{
    L->data = malloc(sizeof(int) * length);
    if (!L->data) return 1;

    L->length = length;
    L->used = 1;
    L->data[L->length] = '\0';

    return 0;
}

void sb_destroy(strbuf_t *L)
{
    free(L->data);
}


int sb_append(strbuf_t *L, char item)
{
    if (L->used == L->length) 
    {
	   size_t size = L->length * 2;
	   char *p = realloc(L->data, sizeof(int) * size);
	   if (!p) return 1;

	   L->data = p;
	   L->length = size;
	   if (DEBUG) printf("Increased size to %lu\n", size);
    }

    L->data[L->used-1] = item;
    L->data[L->used] = '\0';
    ++L->used;

    return 0;
}


int sb_remove(strbuf_t *L, char *item)
{
    if (L->used == 0) return 1;

    L->data[L->used] = '\0';
    --L->used;

    if (item) *item = L->data[L->used];

    return 0;
}

int sb_insert(strbuf_t *L, int index, char item)
{
    int oldlength = L->length;
    //making the arraylist bigger
    if(L->length <= index + 1 || L->used == L->length)
    {
        size_t size = L->length * 2;
        if(size < index + 2)
        {
            size = index + 2;
        }
        else
        {
            size = L->length * 2;
        }
        char *p = realloc(L->data, sizeof(int) * size);
        if (!p) 
            return 1;
        L->data = p;
        L->length = size;
    }

    if(index < L->used)
        ++L->used;
    else
        L->used = index + 2;

    //put in the item and move everything down one
    int prev = L->data[index];
    int curr = item;
    int i;
    for(i = index; i < L->length; i ++)
    {
        L->data[i] = curr;
        curr = prev;
        if(i + 1 < L->used)
            prev = L->data[i + 1];
        else
            break;
    }
    L->data[L->used] = '\0';

    return 0;    
}

int sb_concat(strbuf_t *L, char *str)
{
    //how long the string is, change the length of the data set to include the string
    if (str == NULL) return 1;
    int len = strlen(str);
    if(L->length <= L->used + len)
    {
        size_t size = L->length * 2;
        if(size < L->length + len)
        {
            size = L->length + len;
        }
        else
        {
            size = L->length * 2;
        }
        char *p = realloc(L->data, sizeof(int) * size);
        if (!p) 
            return 1;
        L->data = p;
        L->length = size;
    }

    //put in the new string and a string terminator at the end
    int pos = L->used - 1;
    int i;
    for(i = 0; i < len; i ++)
    {
        L->data[pos] = str[i];
        pos ++;
    }
    L->data[L->length] = '\0';

    L->used = L->used + len;
    return 0;
}

