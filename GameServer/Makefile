all: server client rnd levedit grfx
server: server.c
	#gcc -DDEBUG -D_REENTRANT -Wall -std=c99 -pthread -pedantic -march=pentium2 -g3 -o server server.c
	gcc -D_REENTRANT -Wall -std=c99 -pthread -pedantic -march=pentium2 -O2 -o server server.c
	-strip -s server
client: client.c
	gcc -D_REENTRANT -Wall -std=c99 -pthread -pedantic -march=pentium2 -O2 -o client client.c -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
	-strip -s client
	#gcc -DDEBUG -D_REENTRANT -Wall -std=c99 -pthread -pedantic -march=pentium2 -g3 -o client client.c -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
rnd: rnd.c
	gcc -Wall -std=c99 -pedantic -march=pentium2 -O2 -o rnd rnd.c -lc
grfx: grfx.c
	gcc -Wall -std=c99 -pedantic -march=pentium2 -O2 -o grfx grfx.c -lc
levedit: levedit.c
	gcc -Wall -std=c99 -pedantic -march=pentium2 -O2 -o levedit levedit.c -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -lc
clean:
	-rm -f server client rnd levedit grfx *.o
