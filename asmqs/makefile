all: wypas debug strsort
wypas: wypas.o
	nm wypas.o
	ld -e _start -o wypas wypas.o
	strip -s wypas
	strip -S wypas
	strip -x wypas
	strip -X wypas
	strip --strip-unneeded wypas
	strip -R .sbss wypas
	strip -R .comment wypas
	strip -R .ident wypas
debug: wypas_dbg.o strs_dbg.o
	ld -e _start -o strs_dbg strs_dbg.o
	ld -e _start -o wypas_dbg wypas_dbg.o
wypas.o: wypas.s
	as -o wypas.o wypas.s
strs_dbg.o: asmqsort.s
	as --gstabs -o strs_dbg.o asmqsort.s
asmqsort.o: asmqsort.s
	as -o asmqsort.o asmqsort.s
strsort: asmqsort.o
	ld -e _start -o strsort asmqsort.o
	strip -s strsort
	strip -S strsort
	strip -x strsort
	strip -X strsort
	strip --strip-unneeded strsort
	strip -R .sbss strsort
	strip -R .comment strsort
	strip -R .ident strsort
wypas_dbg.o: wypas.s
	as --gstabs --gdwarf2 -o wypas_dbg.o wypas.s
