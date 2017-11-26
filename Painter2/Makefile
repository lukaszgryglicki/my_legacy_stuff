# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: program painter texconv
CFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -O2
DCFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -g3 -DDEBUG
LINK= -lglut -lXmu -lXext -lX11 -lm
CC= gcc
STRIP= strip -s 
program: program.c libfparser.h libfparser.c
	$(CC) $(CFLAGS) -o program program.c libfparser.c $(LINK)
	$(STRIP) program
program.dbg: program.c libfparser.h libfparser.c
	$(CC) $(DCFLAGS) -o program.dbg program.c libfparser.c /usr/lib/libefence.a $(LINK) 
painter: painter.c
	$(CC) $(CFLAGS) -o painter painter.c $(LINK)
	$(STRIP) painter
painter.dbg: painter.c
	$(CC) $(DCFLAGS) -o painter.dbg painter.c /usr/lib/libefence.a $(LINK) 
texconv: texconv.c
	$(CC) $(CFLAGS) -o texconv texconv.c $(LINK)
	$(STRIP) texconv
texconv.dbg: texconv.c
	$(CC) $(DCFLAGS) -o texconv.dbg texconv.c /usr/lib/libefence.a $(LINK) 
clean:
	-rm -f program painter texconv *.dbg
