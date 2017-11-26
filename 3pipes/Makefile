all: x3pipe x2pipe
x3pipe: 3pipe.c
	cc -Wall -ansi -pedantic -O3 -I/usr/X11R6/include -L/usr/X11R6/lib -o x3pipe 3pipe.c -lX11 -lm
	strip -s ./x3pipe
x2pipe: 2pipe.c
	cc -Wall -ansi -pedantic -O3 -I/usr/X11R6/include -L/usr/X11R6/lib -o x2pipe 2pipe.c -lX11 -lm
	strip -s ./x2pipe
