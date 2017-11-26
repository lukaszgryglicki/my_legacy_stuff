# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: glstruna
CFLAGS= -Wall -ansi -pedantic -O3
LINK= -lc -lm #-lefence -lm
XFLAGS= -I/usr/X11R6/include
XLINK= -L/usr/X11R6/lib -lX11 -L/usr/local/lib -l4d -lX11 -lGL -lGLU -lglut -lXext -lXmu
CC= gcc
STRIP= strip -s 
glstruna: glstruna.c
	$(CC) $(XFLAGS) $(CFLAGS) -o glstruna glstruna.c $(LINK) $(XLINK)
	$(STRIP) glstruna
clean:
	-rm -f glstruna
