all: gas
gas: gas.o link.o
	g++ -g3 -o gas gas.o link.o
	#strip -S gas
	#strip -s gas
	#strip -x gas
	#strip -X gas
	#strip -R .sbss gas
	#strip -R .ident gas
	#strip -R .comment gas
	#strip --strip-unneeded gas
gas.o: gas.s
	as --gstabs -o gas.o gas.s
link.o: link.c
	g++ -g3 -c link.c
clean:
	-rm -rf *.o qsortasm

