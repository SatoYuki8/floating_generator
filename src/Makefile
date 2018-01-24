all:	comp

comp:	*.c
	gcc *c


do:	a.out
	./a.out 5 10
	ls *.sfl *.h

clean:
	rm -rf *~ *.out *.o *.sfl *.v
	ls *.h | grep -v -E 'generate.h' | xargs rm -r
