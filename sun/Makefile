all: sun
sun: sun.c
	gcc -I/usr/local/include -L/usr/local/lib -DUSEJPEG -Wall -ansi -pedantic -O4 -o sun sun.c -lm -lc -ljpeg
	strip -s sun
