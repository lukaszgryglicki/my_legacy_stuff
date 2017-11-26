# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: pusn
CFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -g2
DCFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -g3 -DDEBUG
LINK= -lglut -lXmu -lXext -lX11 -lm
CC= gcc
STRIP= ls -l #strip -s 
pusn: pusn.c
	$(CC) $(CFLAGS) -o pusn pusn.c $(LINK)
	$(STRIP) pusn
pusn.dbg: pusn.c
	$(CC) $(DCFLAGS) -o pusn.dbg pusn.c /usr/lib/libefence.a $(LINK) 
clean:
	-rm -f pusn *.dbg
