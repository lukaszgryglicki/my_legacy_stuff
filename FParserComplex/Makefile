# Makefile written by Morgoth DBMA, morgothdbma@o2.pl, tel: +48693582014
# Lukasz Gryglicki MiNI M1 CC, License BSD
all: FParserGL
CFLAGS= -Wall -ansi -pedantic -pthread -I/usr/X11R6/include -L/usr/X11R6/lib -L/usr/local/lib -O3
LINK= -lglut -lXmu -lXext -lX11 -lm
SCFLAGS= -Wall -ansi -pedantic -pthread -fPIC -shared -O3
SLINK= -lm
CC= g++
STRIP= strip -s
FParserGL: FParserGL.cxx
	$(CC) $(CFLAGS) -o FParserGL FParserGL.cxx $(LINK)
	$(STRIP) FParserGL
clean:
	-rm -f FParserGL
