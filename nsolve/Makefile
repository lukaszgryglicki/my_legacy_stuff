all: nsolve nsolves ode
nsolve: nsolve.c parser.h parser.c
	gcc -pthread -Wall -ansi -pedantic -O3 -o nsolve nsolve.c parser.c -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lc -lm
	strip -s nsolve
nsolves: nsolves.c parser.h parser.c
	gcc -pthread -Wall -ansi -pedantic -O3 -o nsolves nsolves.c parser.c -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lc -lm
	strip -s nsolves
ode: ode.c odeparser.h odeparser.c
	gcc -pthread -Wall -ansi -pedantic -O3 -o ode ode.c odeparser.c -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lc -lm
	strip -s ode

