all: trajpeg fjpeg fjpegd libfjpegudf.so libfjpegudfd.so calln testasm
testasm: testasm.o asmudf64.o
	gcc -g3 -Wall -ansi -pedantic -o testasm testasm.o asmudf64.o -lc
testasm.o: testasm.c
	gcc -g3 -Wall -ansi -pedantic -c testasm.c -o testasm.o
trajpeg: trajpeg.c
	gcc -Wall -ansi -pedantic -O3 -I/usr/local/include -L/usr/local/lib -o trajpeg trajpeg.c -ljpeg
	strip -s trajpeg
fjpeg: fjpeg.o fpar.o asmudf64.o libfjpegudf.so
	gcc -DSILENT -ffast-math -pthread -O4 -fexpensive-optimizations -funroll-all-loops -L. -L/usr/local/lib -Wall -ansi -pedantic -o fjpeg fjpeg.o fpar.o asmudf64.o -lfjpegudf -ljpeg -lpng -lc -lm
	strip -s fjpeg
fjpegd: fjpegd.o fpard.o asmudf64.o libfjpegudfd.so
	gcc -pthread -g3 -L. -L/usr/local/lib -Wall -ansi -pedantic -o fjpegd fjpegd.o fpard.o asmudf64.o -lfjpegudfd -ljpeg -lpng -lc -lm
fjpeg.o: fjpeg.c
	gcc --save-temps -DSILENT -ffast-math -pthread -O4 -fexpensive-optimizations -funroll-all-loops -I/usr/local/include -Wall -ansi -pedantic -c fjpeg.c
fjpegd.o: fjpeg.c
	gcc -pthread -g3 -I/usr/local/include -Wall -ansi -pedantic -c fjpeg.c -o fjpegd.o
fpar.o: fpar.c fpar.h
	gcc --save-temps -DSILENT -ffast-math -pthread -O4 -fexpensive-optimizations -funroll-all-loops -I/usr/local/include -Wall -ansi -pedantic -c fpar.c
fpard.o: fpar.c fpar.h
	gcc -g3 -I/usr/local/include -Wall -ansi -pedantic -c fpar.c -o fpard.o
libfjpegudf.so: udf.c udf.h
	gcc --save-temps -DSILENT -Wall -ansi -pedantic -O4 -fPIC -pthread -shared -o libfjpegudf.so udf.c 
	strip -s libfjpegudf.so
libfjpegudfd.so: udf.c udf.h
	gcc -Wall -ansi -pedantic -g3 -fPIC -pthread -shared -o libfjpegudfd.so udf.c 
asmudf.o: asmudf.s
	as --gstabs -o asmudf.o asmudf.s
	#as -o asmudf.o asmudf.s
asmudf64.o: asmudf64.s
	as --gstabs -o asmudf64.o asmudf64.s
	#as -o asmudf64.o asmudf64.s
calln: calln.c
	gcc --save-temps -Wall -ansi -pedantic -O3 -o calln calln.c
	strip -s calln
install: all
	cp *.so /usr/lib
	cp fjpeg fjpegd calln /usr/bin
clean:
	-rm -f trajpeg fjpeg fjpegd calln *.o *.i *.so

