all: oftps oftpc
oftps: oftps.c
	cc -Werror -Wall -ansi -pedantic -O3 -o oftps oftps.c -lc
	-strip -s oftps
oftpc: oftpc.c
	cc -Werror -Wall -ansi -pedantic -O3 -o oftpc oftpc.c -lc
	-strip -s oftpc
clean:
	-rm -f oftps oftpc *.o
