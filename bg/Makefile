all: bg
bg: bg.c
	gcc -Wall -ansi -pedantic -o bg bg.c -lc
	strip -s bg
clean:
	-rm -f bg
install: bg
	-cp -v bg /usr/local/bin

