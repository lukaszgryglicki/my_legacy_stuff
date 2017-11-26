all: gcvoice
gcvoice: gcvoice.cc
	g++ -Wall -ansi -pedantic -O -o gcvoice gcvoice.cc -lm `pkg-config --cflags --libs gtk+-2.0`
	strip -s gcvoice
clean:
	-rm -f gcvoice

