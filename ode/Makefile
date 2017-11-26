all: ode
ode: ode.c odeparser.h odeparser.c
	gcc -pthread -Wall -ansi -pedantic -O3 -o ode ode.c odeparser.c -I/usr/X11R6/include -L/usr/X11R6/lib -lglut -lc -lm
	strip -s ode

