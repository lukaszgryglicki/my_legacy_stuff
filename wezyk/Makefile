all: wezyk wezyk.dbg
wezyk: wezyk.c 
	g++ -I/usr/X11R6/include -L/usr/X11R6/lib -O3 -o wezyk wezyk.c -lX11
	strip -s wezyk
wezyk.dbg: wezyk.c
	g++ -I/usr/X11R6/include -L/usr/X11R6/lib -g3 -o wezyk.dbg wezyk.c -lX11
