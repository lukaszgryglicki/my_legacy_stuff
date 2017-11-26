all: jvcrypt mcrypt mcryptd
jvcrypt: jvcrypt.c
	gcc -Werror -Wall -ansi -pedantic -O2 -o jvcrypt jvcrypt.c -lc
	strip -s ./jvcrypt
mcrypt: mcrypt.c
	gcc -Werror -Wall -ansi -pedantic -O2 -o mcrypt mcrypt.c -lc
	strip -s ./mcrypt
mcryptd: mcryptd.c
	gcc -Werror -Wall -ansi -pedantic -O2 -o mcryptd mcryptd.c -lc
	strip -s ./mcryptd
