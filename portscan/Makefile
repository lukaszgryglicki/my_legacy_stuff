all: portscan portscand
portscan: portscan.c
	gcc -Wall -ansi -pedantic -O3 -o portscan portscan.c -lc
	#strip -s portscan
portscand: portscan.c
	gcc -Wall -ansi -pedantic -g3 -o portscand portscan.c
install: portscan
	-cp portscan /usr/local/bin
clean:
	-rm -f portscan portscand

