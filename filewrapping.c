#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "strbuf.c"

void printline(strbuf_t whole)
{
	char line[whole.length];
	int i, bytes_written;
	for(i = 0; i < whole.length; i ++)
	{
		line[i] = whole.data[i];
	}
	bytes_written = write(1, line, whole.length);
	write(1, "\n", 1);
}

void newline(strbuf_t whole)
{
	char c;
	int i;
	for(i = 0; i < whole.length; i ++)
		sb_remove(&whole, &c);
}

char[] tochararray(strbuf_t whole)
{
	char array[whole.length];
	for(i = wordlength - 1; i >= 0; -- i)
			{
				//printf("Problem in here?\n");
				sb_remove(&word, &c);
				warray[i] = c;
			}
}

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Error\n");
		return EXIT_FAILURE;
	}
	int wrapval = atoi(argv[1]);
	char* f = argv[2];
	int fd = open(f, O_RDONLY);

	int nl = 0, wordlength = 0, linelength = 0, exitfailure = 0, i;
	strbuf_t word;
	strbuf_t whole;
	char c;
	char* a = malloc(sizeof(char));
	int final = read(fd, a, sizeof(char));
	printf("%c\n", a[0]);
	sb_init(&whole, wrapval);

	while(final != 0)
	{
		//printf("Line length = %d  word length = %d  wrapval = %d  \n",linelength, wordlength, wrapval);
		//printf("%c\n", a[0]);
		sb_init(&word, 5);
		while(isspace(a[0]) == 0)
		{
			sb_append(&word, a[0]);
			wordlength ++;
			final = read(fd, a, sizeof(char));
		}
		if(a[0] == '\n') //need to fix this
		{
			//if there are 2 \n, write the 2. If more, 
			//printf("Is it the end of the line?\n");
			if(linelength + wordlength > wrapval)
			{	
				nl ++;
				if(nl == 2)
				{
					int bytes_written;
					bytes_written = write(1, "\n", 1);
				}
				else if(nl > 2)
				{
				}
				else
				{

				}
				linelength = 0;
			}
		}
		else if((linelength + wordlength  + 1) <= wrapval)
		{
			//printf("Word is over and can be added to the line\n");
			nl = 0;
			char warray[wordlength];
			for(i = wordlength - 1; i >= 0; -- i)
			{
				//printf("Problem in here?\n");
				sb_remove(&word, &c);
				warray[i] = c;
			}
			printf("Current word: ");
			for(i = 0; i < wordlength; i ++)
				printf("%c", warray[i]);
			printf("\n");
			if(linelength == 0)
				sb_concat(&whole, warray);
			else
			{
				sb_append(&whole, ' ');
				sb_concat(&whole, warray);
				linelength ++;
			}
			linelength += wordlength;
			wordlength = 0;
		}
		else if((linelength + wordlength + 1) > wrapval)
		{
			//printf("Word is over and can be added to the next line\n");
			nl = 0;
			if(wordlength + 1 > wrapval)
			{
				exitfailure = 1;
				printline(whole);
				newline(whole);
				printline(word);
				newline(word);
				linelength = 0;
				wordlength = 0;
			}
			else
			{
				//printf("Here\n");
				nl = 0;
				printline(whole);
				newline(whole);
				char warray[wordlength];
				for(i = wordlength - 1; i >= 0; -- i)
				{
					sb_remove(&word, &c);
					warray[i] = c;
				}
				sb_concat(&whole,warray);
				linelength = wordlength;
				wordlength = 0;
			}
		}
		else
		{
			//printf("Here?\n");
		}
		final = read(fd, a, sizeof(char));
	}
	if(wordlength > 0)
	{
		printline(word);
	}
	close(fd);
	free(a);
	sb_destroy(&whole);
	sb_destroy(&word);
	if(exitfailure == 1)
	{
		printf("Width is too small for the size of the words in the file\n");
		return EXIT_FAILURE;
	}
	else
		return EXIT_SUCCESS;
}