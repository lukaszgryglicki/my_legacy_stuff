all: cpuid
cpuid: cpuid.s
	as -o cpuid.o cpuid.s
	ld -o cpuid cpuid.o
	-rm -ff cpuid.o
	-strip -s cpuid
