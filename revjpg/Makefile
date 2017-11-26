all: xjpeg
xjpeg.o: xjpeg.c
	gcc -Wall -ansi -pedantic -g -O -I/usr/X11R6/include -c xjpeg.c -o xjpeg.o
xjpeg: xjpeg.o rgb.o mjpeg.o
	gcc -Wall -ansi -pedantic -g -O -L/usr/X11R6/lib -L/usr/local/lib -o xjpeg xjpeg.o mjpeg.o rgb.o -lX11 -ljpeg
rgb.o: rgb.s
	as -o rgb.o rgb.s
mjpeg.o: mjpeg.c
	gcc -Wall -ansi -pedantic -g -O -I/usr/local/include -c mjpeg.c -o mjpeg.o
clean:
	-rm -f *.o

