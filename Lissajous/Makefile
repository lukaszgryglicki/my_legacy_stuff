all: xlissajous x3dliss
xlissajous: lissajous.c
	cc -Wall -ansi -pedantic -O3 -I/usr/X11R6/include -L/usr/X11R6/lib -o xlissajous lissajous.c -lX11 -lm
	strip -s ./xlissajous
x3dliss: lissajous3d.c
	cc -Wall -ansi -pedantic -O3 -I/usr/X11R6/include -L/usr/X11R6/lib -o x3dliss lissajous3d.c -lX11 -lm
	strip -s ./x3dliss
