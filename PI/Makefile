CC=c++
CFLAGS=-Wall -ansi
DCFLAGS=-Wall -ansi -g3 
UDCFLAGS=-Wall -ansi -g3 -DDEBUG -DDBG 
DAFLAGS=--gstabs
LIBS=/usr/X11R6/lib
INCL=/usr/X11R6/include
LINK=-lX11 
DLINK=-lX11 -lefence
ASM=as
STR=strip -s
SVFLAGS=--save-temps
#all: project 
all: project 
project: project.o asmqsort.o
	$(CC) $(CFLAGS) -L$(LIBS) -I$(INCL) -o project project.o asmqsort.o $(LINK)
	$(STR) project
asmqsort.o: asmqsort.s
	$(ASM) -o asmqsort.o asmqsort.s
project.o: project.c mh.h xmwnd.h mgraph.h mran.h
	$(CC) $(CFLAGS) -I$(INCL) -L$(LIBS)  -c project.c 
clean:
	rm *.o
debug: projectd.o asmqsortd.o
	$(CC) $(DCFLAGS) -L$(LIBS) -I$(INCL) -o projectd projectd.o asmqsortd.o $(DLINK)
asmqsortd.o: asmqsort.s
	$(ASM) $(DAFLAGS) -o asmqsortd.o asmqsort.s
projectd.o: project.c mh.h xmwnd.h mgraph.h mran.h
	$(CC) $(DCFLAGS) -I$(INCL) -L$(LIBS)  -o projectd.o -c project.c 
projectud.o: project.c mh.h xmwnd.h mgraph.h mran.h
	$(CC) $(UDCFLAGS) -I$(INCL) -L$(LIBS)  -o projectud.o -c project.c 
asmsrc: project.c mh.h mran.h mgraph.h xmwnd.h
	$(CC) $(SVFLAGS) $(DCFLAGS) -I$(INCL) -L$(LIBS) -c project.c
	-rm -f *.i
	-rm -f *.ii
pkg: 
	-tar jcvf PI.tbz project.c asmqsort.s mh.h mran.h xmwnd.h mgraph.h Makefile data/*.[dD][aA][tT] data/*.R 
testsort: testsort.c
	g++ -g3 -o testsort testsort.c
ultra_debug: asmqsortd.o projectud.o
	$(CC) $(UDCFLAGS) -I$(INCL) -L$(LIBS) -o projectud projectud.o asmqsortd.o $(DLINK)

