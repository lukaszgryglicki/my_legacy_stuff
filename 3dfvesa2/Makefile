all: palette 3df f
3df: 3do.c function.h
	g++ -I/usr/local/include -L/usr/local/lib -o 3dfvesa 3do.c -lvga
f: f.c
	g++ -o f f.c
palette: palette.c
	g++ -I/usr/local/include -L/usr/local/lib -o palette palette.c -lvga
