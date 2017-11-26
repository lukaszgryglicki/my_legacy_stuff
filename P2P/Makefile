all: Kp2p testUDP
DEBUGCXXFLAGS   = -Wall -g3 -DDEBUG -ansi -D_REENTRANT -D_THREAD_SAFE
DEBUGCXXLINK    = -Wall -g3 -pthread  -DDEBUG -ansi -D_REENTRANT -D_THREAD_SAFE #-lefence
RELEASECXXFLAGS = -Wall -O2 -ansi -D_REENTRANT -D_THREAD_SAFE
RELEASECXXLINK  = -Wall -O2 -pthread -ansi -D_REENTRANT -D_THREAD_SAFE
CXXFLAGS 	= $(RELEASECXXFLAGS)	# use release or debug
CXXLINK 	= $(RELEASECXXLINK)	# version
Kp2p: KApp.o Main.o KGetOptions.o KException.o KMemoryException.o KIOException.o KList.o KFile.o KSysLog.o KMutex.o
	g++ $(CXXFLAGS) -o Kp2p KList.o KApp.o Main.o KGetOptions.o KException.o KMemoryException.o KIOException.o KFile.o KSysLog.o KMutex.o $(CXXLINK)
KSysLog.o: KSysLog.cc KSysLog.hh Common.hh
	g++ $(CXXFLAGS) -c KSysLog.cc
KMutex.o: KMutex.cc KMutex.hh Common.hh
	g++ $(CXXFLAGS) -c KMutex.cc
KException.o: KException.cc KException.hh Common.hh
	g++ $(CXXFLAGS) -c KException.cc
KMemoryException.o: KMemoryException.cc KMemoryException.hh KException.o Common.hh
	g++ $(CXXFLAGS) -c KMemoryException.cc
KIOException.o: KIOException.cc KIOException.hh KException.o Common.hh
	g++ $(CXXFLAGS) -c KIOException.cc
KApp.o: KApp.cc KApp.hh Common.hh KMemoryException.o KList.o KFile.o KSysLog.o KMutex.o
	g++ $(CXXFLAGS) -c KApp.cc
KGetOptions.o: KGetOptions.cc KGetOptions.hh Common.hh KMemoryException.o
	g++ $(CXXFLAGS) -c KGetOptions.cc
KList.o: Common.hh KList.hh KList.cc KMemoryException.hh
	g++ $(CXXFLAGS) -c KList.cc
KFile.o: Common.hh KFile.hh KFile.cc KMemoryException.hh KIOException.hh
	g++ $(CXXFLAGS) -c KFile.cc
Main.o: Main.cc Common.hh KApp.hh KGetOptions.hh KException.hh KMemoryException.hh
testUDP: testUDP.cc Common.hh
	g++ $(CXXFLAGS) -o testUDP testUDP.cc $(CXXLINK)
	g++ $(CXXFLAGS) -c Main.cc
clean:
	-rm -f *.o Kp2p testUDP
package:
	-tar jcvf Kp2p-0.21.tar.bz2 Makefile* *.cc *.hh README *host* *example*
