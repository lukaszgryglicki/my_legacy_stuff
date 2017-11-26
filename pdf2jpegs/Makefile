all: pdf2jpegs
pdf2jpegs: pdf2jpegs.c
	cc -Wall -ansi -pedantic -O3 -o pdf2jpegs pdf2jpegs.c -lc
	strip -s pdf2jpegs
clean:
	-rm pdf2jpegs
install: pdf2jpegs
	-cp pdf2jpegs /usr/local/bin/ && echo done

