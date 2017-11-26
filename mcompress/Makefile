all: mcompress mcompressf mcompressfd
mcompress: mcompress.c
	gcc -g3 -Wall -ansi -pedantic -o mcompress mcompress.c 
mcompressf: mcompress.c
	gcc -DSPEED -O4 -fexpensive-optimizations -fomit-frame-pointer -finline-functions -Wall -ansi -pedantic -o mcompressf mcompress.c 
	strip -s mcompressf
mcompressfd: mcompress.c
	gcc -DSPEED -g3 -Wall -ansi -pedantic -o mcompressfd mcompress.c 
mcompress.exe: mcompress.c
	gcc -g3 -Wall -ansi -pedantic -o mcompress.exe mcompress.c -lc
mcompressf.exe: mcompress.c
	gcc -DSPEED -O4 -fexpensive-optimizations -fomit-frame-pointer -finline-functions -Wall -ansi -pedantic -o mcompressf.exe mcompress.c -lc
	strip -s mcompressf.exe

