# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: cpu4 msi4
CFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -O3 -fexpensive-optimizations
LINK= -lglut -lXmu -lXext -lX11 -l4d -lm
CC= gcc
STRIP= strip -s
cpu4: cpu4.c 
	$(CC) $(CFLAGS) -o cpu4 cpu4.c $(LINK)
	$(STRIP) cpu4
msi4: msi4.c 
	$(CC) $(CFLAGS) -o msi4 msi4.c $(LINK)
	$(STRIP) msi4
clean:
	-rm -f cpu4
	-rm -f msi4
