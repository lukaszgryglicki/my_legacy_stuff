all: game fast
game: game.c VesaWnd.h VesaGraph.h EmuXlib.h dbase.h game.c mran.h mh.h
	g++ -O3 -I/usr/local/include -L/usr/local/lib -o vesagame game.c -lvga
fast: game.c VesaWnd.h VesaGraph.h EmuXlib.h dbase.h game.c mran.h mh.h
	g++ -g3 -I/usr/local/include -L/usr/local/lib -o vesagame game.c -lvga
