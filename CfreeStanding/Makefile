all: prog.COM pasm.COM
prog.COM: prog.o
	ld -o prog.COM --oformat binary -Ttext 0x0100 prog.o
prog.o: prog.c
	cc -c -ffreestanding -o prog.o prog.c
clean: 
	-rm *.o *.COM
pasm.COM: pasm.o
	ld -o pasm.COM --oformat binary -Ttext 0x0100 pasm.o
pasm.o: pasm.s
	as -o pasm.o pasm.s
run: pasm.COM
	-rm /dist/pasm.COM
	-sync
	-cp pasm.COM /dist
	-sync
	-msdos
	
