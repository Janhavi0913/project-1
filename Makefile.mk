projectwrap: projectwrap.o strbuf.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o projectwrap projectwrap.o strbuf.o

projectwrap.o: projectwrap.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined projectwrap.c

strbuf.o: strbuf.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined strbuf.c


# separate version of strbuf.o with debugging text enabled
dstrbuf.o: strbuf.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -DDEBUG -o dstrbuf.o strbuf.c

dprojectwrap: projectwrap.o dstrbuf.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o projectwrap projectwrap.o dstrbuf.o

clean:
	rm -f *.o projectwrap dprojectwrap