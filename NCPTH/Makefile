all: game
game: game.cc CursesWins.h
	c++ -D_THREAD_SAFE -pthread -Wall -O2 -o game game.cc -lcurses
package:
	tar zcf projekt.tgz *.cc Makefile README
