all: xrw
xrw: xrw.c
	g++ -L/usr/X11R6/lib -L/usr/local/lib -I/usr/X11R6/include -I/usr/local/include -Wall -ansi -pedantic -o xrw xrw.c -lX11
	strip -s xrw
clean:
	-rm -f xrw

