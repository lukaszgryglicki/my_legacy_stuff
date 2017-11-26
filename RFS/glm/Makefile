# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: glm ldpix
CFLAGS= -Wall -ansi -pedantic -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -O4
LINK= -lglut -lXmu -lXext -lX11 -lm
CC= gcc
STRIP= strip -s
glm: glm.c
	$(CC) $(CFLAGS) -o glm glm.c $(LINK)
	$(STRIP) glm
ldpix: ldpix.c
	$(CC) $(CFLAGS) -o ldpix ldpix.c $(LINK)
	$(STRIP) ldpix
clean:
	-rm -f glm
	-rm -f ldpix
