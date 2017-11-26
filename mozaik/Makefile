all: mozaik
mozaik: main.c
	gcc -O4 -fexpensive-optimizations -funroll-all-loops -DUSE_JPEG -I/usr/local/include -L/usr/local/lib -Wall -ansi -pedantic -o mozaik main.c -ljpeg -lc -lm
	strip -s mozaik
