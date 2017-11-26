# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: program curve draw drawC nurbs sign
CFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -O4
DCFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -g3 -DDEBUG
LINK= -lglut -lXmu -lXext -lX11 -lm
CC= gcc
STRIP= strip -s 
program: program.c libfparser.h libfparser.c
	$(CC) $(CFLAGS) -o program program.c libfparser.c $(LINK)
	$(STRIP) program
program.dbg: program.c libfparser.h libfparser.c
	$(CC) $(DCFLAGS) -o program.dbg program.c libfparser.c /usr/local/lib/libefence.a $(LINK) 
curve: curve.c
	$(CC) $(CFLAGS) -o curve curve.c $(LINK) 
	$(STRIP) curve
curve.dbg: curve.c
	$(CC) $(DCFLAGS) -o curve.dbg curve.c /usr/local/lib/libefence.a $(LINK) 
draw: draw.c
	$(CC) $(CFLAGS) -o draw draw.c $(LINK) 
	$(STRIP) draw
draw.dbg: draw.c
	$(CC) $(DCFLAGS) -o draw.dbg draw.c /usr/local/lib/libefence.a $(LINK) 
sign: sign.c
	$(CC) $(CFLAGS) -o sign sign.c $(LINK) 
	$(STRIP) sign
sign.dbg: sign.c
	$(CC) $(DCFLAGS) -o sign.dbg sign.c /usr/local/lib/libefence.a $(LINK) 
drawC: drawC.c
	$(CC) $(CFLAGS) -o drawC drawC.c $(LINK) 
	$(STRIP) drawC
drawC.dbg: drawC.c
	$(CC) $(DCFLAGS) -o drawC.dbg drawC.c /usr/local/lib/libefence.a $(LINK) 
nurbs: nurbs.c
	$(CC) $(CFLAGS) -o nurbs nurbs.c $(LINK)
	$(STRIP) nurbs
nurbs.dbg: nurbs.c
	$(CC) $(DCFLAGS) -o nurbs.dbg nurbs.c /usr/local/lib/libefence.a $(LINK) 
clean:
	-rm -f nurbs program curve draw drawC *.dbg
