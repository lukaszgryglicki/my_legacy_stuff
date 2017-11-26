all: nettest readdb mergedb neton
nettest: nettest.c
	#gcc -D_POSIX_SOURCE -D_BSD_SOURCE -Wall -ansi -pedantic -O3 -o nettest nettest.c -lc
	gcc -Wall -ansi -pedantic -O3 -o nettest nettest.c -lc
	strip -s nettest
readdb: readdb.c
	#gcc -DNOJPEG -Wall -ansi -pedantic -I/usr/local/include -L/usr/local/lib -O3 -o readdb readdb.c -lc
	gcc -Wall -ansi -pedantic -I/usr/local/include -L/usr/local/lib -g3 -o readdb readdb.c -ljpeg -lc
	#strip -s readdb
mergedb: mergedb.c
	gcc -Wall -ansi -pedantic -O3 -o mergedb mergedb.c -lc
	strip -s mergedb
neton: neton.c
	gcc -Wall -ansi -pedantic -O3 -o neton neton.c -lc
	strip -s neton
clean:
	-rm -f nettest neton readdb mergedb *.csv data.txt data.sql stats.txt readdb.jpeg
install: nettest readdb mergedb
	-cp -iv nettest neton readdb mergedb /usr/local/bin

