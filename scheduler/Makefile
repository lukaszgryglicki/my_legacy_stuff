all: scheduler
scheduler: scheduler.cc
	c++ -Wall -ansi -pedantic -g -O -o scheduler scheduler.cc -lc
	#strip -s scheduler
clean:
	rm *.core scheduler /tmp/scheduler*
