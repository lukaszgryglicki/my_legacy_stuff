all: libmipc.so testlib dropipc prod cons bufview mutcopy
libmipc.so: libmipc.c libmipc.h
	cc -D_XOPEN_SOURCE -fPIC -shared -Wall -ansi -pedantic -O3 -o libmipc.so libmipc.c -lc
testlib: libmipc.so testlib.c libmipc.h
	cc -D_XOPEN_SOURCE -L. -Wall -ansi -pedantic -O3 -o testlib testlib.c -lmipc -lc
dropipc: libmipc.so dropipc.c libmipc.h
	cc -D_XOPEN_SOURCE -L. -Wall -ansi -pedantic -O3 -o dropipc dropipc.c -lmipc -lc
prod: libmipc.so prod.c libmipc.h common.o
	cc -D_XOPEN_SOURCE -L. -Wall -ansi -pedantic -O3 -o prod prod.c common.o -lmipc -lc
cons: libmipc.so cons.c libmipc.h common.o
	cc -D_XOPEN_SOURCE -L. -Wall -ansi -pedantic -O3 -o cons cons.c common.o -lmipc -lc
bufview: libmipc.so bufview.c libmipc.h common.o
	cc -D_XOPEN_SOURCE -L. -Wall -ansi -pedantic -O3 -o bufview bufview.c common.o -lmipc -lc
mutcopy: libmipc.so mutcopy.c libmipc.h
	cc -D_XOPEN_SOURCE -L. -Wall -ansi -pedantic -O3 -o mutcopy mutcopy.c -lmipc -lc
common.o: common.h common.c
	cc -D_XOPEN_SOURCE -Wall -ansi -pedantic -O3 -c common.c
clean:
	-rm *.o *~ cons prod dropipc testlib bufview mutcopy libmipc.so
package:
	tar jcf MorgothIPC.tar.bz2 *.c *.h Makefile
