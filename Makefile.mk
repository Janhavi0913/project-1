projectwrap: projectwrap.o strbuf.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o p1 p1.o strbuf.o

projectwrap.o: projectwrap.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined p1.c

strbuf.o: strbuf.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined strbuf.c


# separate version of strbuf.o with debugging text enabled
dstrbuf.o: strbuf.c strbuf.h
	gcc -c -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -DDEBUG -o dstrbuf.o strbuf.c

dp1: projectwrap.o dstrbuf.o
	gcc -g -std=c99 -Wvla -Wall -fsanitize=address,undefined -o projectwrap projectwrap.o dstrbuf.o

clean:
	rm -f *.o projectwrap dprojectwrap