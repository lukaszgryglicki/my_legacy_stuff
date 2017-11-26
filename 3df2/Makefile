all: 3df f
3df: 3do.c function.h
	g++ -I/usr/X11R6/include -L/usr/X11R6/lib -o 3df 3do.c -lX11
f: f.c
	g++ -o f f.c

