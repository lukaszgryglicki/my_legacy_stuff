CC = gcc48
CFLAGS = -Wall -ansi -pedantic -O3
SHARED = -fPIC -shared
LINK = -lc -lmipc
STRIP = strip -s
all: mat
mat: mat.c libmipc.so
	#gcc -D_POSIX_SOURCE -D_BSD_SOURCE -Wall -ansi -pedantic -O3 -o mat mat.c -lc
	$(CC) $(CFLAGS) -o mat mat.c $(LINK)
	$(STRIP) mat
libmipc.so: libmipc.c libmipc.h
	$(CC) $(SHARED) $(CFLAGS) -o libmipc.so libmipc.c
	$(STRIP) libmipc.so
install: mat
	-cp -v mat /usr/local/bin
	-cp -v libmipc.so /usr/local/lib
	echo 'Instalation done'
clean:
	-rm -f mat libmipc.so
	echo 'Cleaned'
