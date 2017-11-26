all: test
test: test.o asmqsort.o
	g++ -o test test.o asmqsort.o
test.o: test.c
	g++ -c test.c
asmqsort.o: asmqsort.s
	as -o asmqsort.o asmqsort.s

