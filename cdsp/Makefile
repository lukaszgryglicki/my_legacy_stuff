all: cdsp fdigitize
cdsp: cdsp.c
	gcc -Wall -ansi -pedantic -O3 -o cdsp cdsp.c -lc
	strip -s cdsp
fdigitize.o: fdigitize.c
	gcc -Wall -ansi -pedantic -O3 -c fdigitize.c
fdigitize: fdigitize.o fpar.o
	gcc -Wall -ansi -pedantic -o fdigitize fdigitize.o fpar.o -lc -lm
	strip -s fdigitize
fpar.o: fpar.c fpar.h
	gcc -ffast-math -O3 -fexpensive-optimizations -funroll-all-loops -Wall -ansi -pedantic -c fpar.c
clean:
	-rm -f ./cdsp
install: all
	-cp -v ./cdsp /usr/local/bin && echo Done

