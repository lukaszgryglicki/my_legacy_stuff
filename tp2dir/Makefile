all: tp2
tp2: tp2.o
	ld -e _start -o tp2 tp2.o
	strip -s tp2
	rm -f tp2.o
tp2.o: tp2.s
	as -o tp2.o tp2.s
clean:
	rm -f tp2.o tp2

