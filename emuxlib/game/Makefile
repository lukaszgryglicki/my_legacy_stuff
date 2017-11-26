all: game 
game: game.c VesaWnd.h VesaGraph.h EmuXlib.h dbase.h game.c mran.h mh.h
	g++ -O3 -I/usr/local/include -L/usr/local/lib -o vesagame game.c -lvga
fast: game.c VesaWnd.h VesaGraph.h EmuXlib.h dbase.h game.c mran.h mh.h
	g++ -g3 -I/usr/local/include -L/usr/local/lib -o vesagame game.c -lvga
bz:
	tar cvf dist.tar *.c *.h Makefile font.dat compile_vga
	bzip2 -9 dist.tar
	mv dist.tar.bz2 dist.tbz
	echo OK

