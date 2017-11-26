# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: scene4d lib4d.so
CFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -g3 # -O3
SFLAGS= -Wall -ansi -pedantic -pthread -shared -fPIC -O
LINK= -lglut -lXmu -lXext -lX11 -lm
SLINK= -lm -lc
CC= gcc
STRIP= ls -l #strip -s
scene4d: MG11.c lib4d.c lib4d.h
	$(CC) $(CFLAGS) -o scene4d MG11.c lib4d.c $(LINK)
	$(STRIP) scene4d
lib4d.so: lib4d.c lib4d.c
	$(CC) $(SFLAGS) -o lib4d.so lib4d.c $(SLINK)
	$(STRIP) lib4d.so
clean:
	-rm -f scene4d lib4d.so
