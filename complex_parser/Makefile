# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: glparser
CFLAGS= -Wall -ansi -pedantic -O4 -pthread
LINK= -lc -lm
XFLAGS= -I/usr/X11R6/include
XLINK= -L/usr/X11R6/lib -lX11 -L/usr/local/lib -lX11 -lGL -lGLU -lglut -lXext -lXmu
CC= g++
STRIP= strip -s 
glparser: glparser.cc parser.cc parser.h
	$(CC) $(XFLAGS) $(CFLAGS) -o glparser glparser.cc parser.cc $(LINK) $(XLINK)
	$(STRIP) glparser
clean:
	-rm -f glparser
