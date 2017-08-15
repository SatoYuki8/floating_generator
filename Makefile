all:	comp

comp:	src/*.c bin
	gcc -o bin/gen src/*c

bin:	bin
	mkdir bin

clean:
	rm -rf *~ *.out *.o *.sfl bin/
