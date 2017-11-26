# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: program
CFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -O3
LINK= -lglut -lXmu -lXext -lX11 -l4d -lm
CC= gcc
STRIP= strip -s
program: program.c
	$(CC) $(CFLAGS) -o program program.c $(LINK)
	$(STRIP) program
clean:
	-rm -f program
