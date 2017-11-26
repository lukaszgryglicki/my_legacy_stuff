CC = gcc46
STRIP = strip
all: distance
distance: distance.c
	$(CC) -Wall -ansi -pedantic -O4 -o distance distance.c -I/usr/local/include -L/usr/local/lib -lc -lm -ljpeg
	$(STRIP) -s distance
install: distance
	cp -iv distance /usr/local/bin

