all: inv_fns fill_gaps
inv_fns: inv_fns.c
	gcc -Wall -ansi -pedantic -O3 -o inv_fns inv_fns.c -lc
	strip -s inv_fns

fill_gaps: fill_gaps.c
	gcc -Wall -ansi -pedantic -O3 -o fill_gaps fill_gaps.c -lc
	strip -s fill_gaps
