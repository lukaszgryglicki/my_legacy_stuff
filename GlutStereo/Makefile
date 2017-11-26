# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: program
CFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -O2
DCFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -g3 -DDEBUG
LINK= -lglut -lXmu -lXext -lX11 -lm
CC= gcc
STRIP= strip -s 
program: program.c 
	$(CC) $(CFLAGS) -o program program.c $(LINK)
	$(STRIP) program
program.dbg: program.c 
	$(CC) $(DCFLAGS) -o program.dbg program.c l /usr/lib/libefence.a $(LINK) 
clean:
	-rm -f program *.dbg
