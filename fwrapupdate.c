#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include "strbuf.c"

strbuf_t newline(strbuf_t whole)
{
	int wrapval = whole.length;
	sb_destroy(&whole);
	strbuf_t new;
	sb_init(&new, wrapval);
	return new;
}

int main(int argc, char* argv[])
{
	//opening the file and getting the width for wrapping
	if(argc < 3)
	{
		printf("Error\n");
		return EXIT_FAILURE;
	}
	int wrapval = atoi(argv[1]);
	char* f = argv[2];
	int fd = open(f, O_RDONLY);

	//getting all the values set up
	int nl = 0, linelength = 0, wordlength = 0, exitfailure = 0, i;
	strbuf_t word;
	strbuf_t whole;
	char c;
	char* a = malloc(sizeof(char));
	int final = read(fd, a, sizeof(char));

	while(final != 0)
	{
		sb_init(&word, 5);
		//getting the word into word
		while(isspace(a[0]) == 0)
		{
			sb_append(&word, a[0]);
			wordlength ++;
			final = read(fd, a, sizeof(char));
		}
		//just adding the word to the line
		if((linelength + wordlength + 1) <= wrapval)
		{
			sb_append(&whole, ' ');
			sb_concat(&whole, word.data);
			if(linelength != 0)
				linelength ++;
			linelength += wordlength;
			word = newline(word);
			wordlength = 0;
		}
		//printing out the line and seeing what to do about the next word
		else if((linelength + wordlength) > wrapval || (linelength + wordlength + 1) > wrapval)
		{
			//printf("Word in line: %s\n", whole.data);
			write(1, whole.data, linelength + 1);
			write(1, "\n", 1);
			whole = newline(whole);
			if(wordlength + 1 > wrapval)
			{
				exitfailure = 1;
				write(1, word.data, wordlength);
				linelength = 0;
			}
			else
			{
				//sb_append(&whole, ' ');
				sb_concat(&whole, word.data);
				linelength = wordlength;
			}
			word = newline(word);
			wordlength = 0;
		}
		else
		{
			//getting to the word
			while(isspace(a[0]) == 1)
			{
				if(a[0] == '\n')
					nl ++;
				final = read(fd, a, sizeof(char));
			}
		}
		final = read(fd, a, sizeof(char));
	}
	if(linelength > 0)
	{
		write(1, whole.data, linelength);
	}
	write(1, "\n", 1);
	close(fd);
	free(a);
	sb_destroy(&whole);
	sb_destroy(&word);
	if(exitfailure == 1)
	{
		printf("Error: Width is too small for the size of the words in the file\n");
		return EXIT_FAILURE;
	}
	else
		return EXIT_SUCCESS;
}
