# For different Operating Systems, check Server.cc:95 and README
all: wwwserver terminal
DEBUGCXXFLAGS   = -DDEBUG -Wall -g
DEBUGCXXLINK    = -DDEBUG -Wall -g -lc
RELEASECXXFLAGS = -Wall -O2
RELEASECXXLINK  = -Wall -O2 -lc
CXXFLAGS 	= $(RELEASECXXFLAGS)	# use release or debug
CXXLINK 	= $(RELEASECXXLINK)	# version
wwwserver: Server.o XServerMain.o XGetOptions.o XSemaphore.o XShM.o Logger.o XHttpParser.o
	g++ $(CXXFLAGS) -o wwwserver Server.o XServerMain.o XGetOptions.o XSemaphore.o XShM.o Logger.o XHttpParser.o $(CXXLINK)
Server.o: Server.cc XServerMain.hh Common.hh
	g++ $(CXXFLAGS) -c Server.cc -o Server.o
XServerMain.o: XServerMain.cc XServerMain.hh Common.hh Definitions.hh
	g++ $(CXXFLAGS) -c XServerMain.cc -o XServerMain.o
XGetOptions.o: XGetOptions.cc XGetOptions.hh Common.hh Definitions.hh
	g++ $(CXXFLAGS) -c XGetOptions.cc -o XGetOptions.o
XSemaphore.o: XSemaphore.cc XSemaphore.hh Common.hh Definitions.hh
	g++ $(CXXFLAGS) -c XSemaphore.cc -o XSemaphore.o
XShM.o: XShM.cc XShM.hh Common.hh Definitions.hh
	g++ $(CXXFLAGS) -c XShM.cc -o XShM.o
XHttpParser.o: XHttpParser.cc XHttpParser.hh Common.hh Definitions.hh
	g++ $(CXXFLAGS) -c XHttpParser.cc -o XHttpParser.o
Logger.o: Logger.cc Logger.hh Definitions.hh
	g++ $(CXXFLAGS) -c Logger.cc -o Logger.o
terminal: terminal.c
	gcc $(CXXFLAGS) -o terminal terminal.c -lc
clean:
	-rm -f *.o wwwserver
package:
	-tar jcvf WWWServer-1.0.tar.bz2 Makefile* *.cc *.hh *.c *.png *.gif *.jp*g *.htm* README
