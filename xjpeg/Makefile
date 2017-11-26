all: xjpeg
xjpeg.o: xjpeg.c
	g++ -O -I/usr/X11R6/include -c xjpeg.c -o xjpeg.o
xjpeg: xjpeg.o rgb.o mjpeg.o
	g++ -O -L/usr/X11R6/lib -L/usr/local/lib -o xjpeg xjpeg.o mjpeg.o rgb.o -lX11 -ljpeg
rgb.o: rgb.s
	as -o rgb.o rgb.s
mjpeg.o: mjpeg.c
	gcc -O -I/usr/local/include -c mjpeg.c -o mjpeg.o
clean:
	-rm -f *.o

