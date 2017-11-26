STRIP=strip
all: ggmerge
ggmerge: ggmerge.c
	$(CC) -Wall -ansi -pedantic -O3 -o ggmerge ggmerge.c 
	$(STRIP) -s ggmerge
clean:
	-rm -f ggmerge
install: ggmerge
	-cp -iv ggmerge /usr/local/bin

