all: ggnot ggnotd gge gged ggssh ggsshd
win: ggssh.exe ggsshd.exe gge.exe gged.exe ggnot.exe ggnotd.exe
ggnot: ggnot.cc
	g++ -D_THREAD_SAFE -pthread -I/usr/local/include -L/usr/local/lib -Wall -ansi -O3 -o ggnot ggnot.cc -lgadu -lssl -lcrypto
	strip -s ggnot
ggnotd: ggnot.cc
	g++ -D_THREAD_SAFE -pthread -DMDBG -I/usr/local/include -L/usr/local/lib -Wall -ansi -g3 -o ggnotd ggnot.cc -lgadu -lssl -lcrypto
gge: ggnot.cc
	g++ -D_THREAD_SAFE -pthread -DGGEXEC -I/usr/local/include -L/usr/local/lib -Wall -ansi -O3 -o gge ggnot.cc -lgadu -lssl -lcrypto
	strip -s gge
gged: ggnot.cc
	g++ -D_THREAD_SAFE -pthread -DMDBG -DGGEXEC -I/usr/local/include -L/usr/local/lib -Wall -ansi -g3 -o gged ggnot.cc -lgadu -lssl -lcrypto
ggssh: ggssh.cc
	g++ -D_THREAD_SAFE -pthread -I/usr/local/include -L/usr/local/lib -Wall -ansi -O3 -o ggssh ggssh.cc -lgadu -lssl -lcrypto
	strip -s ggssh
ggsshd: ggssh.cc
	g++ -D_THREAD_SAFE -pthread -DMDBG -I/usr/local/include -L/usr/local/lib -Wall -ansi -g3 -o ggsshd ggssh.cc -lgadu -lssl -lcrypto
ggssh.exe: ggssh.cc
	g++ -DMWIN -L/usr/local/lib -Wall -ansi -O3 -o ggssh.exe ggssh.cc -lgadu -lz
	strip -s ggssh.exe
ggsshd.exe: ggssh.cc
	g++ -DMDBG -DMWIN -L/usr/local/lib -Wall -ansi -g3 -o ggsshd.exe ggssh.cc -lgadu -lz
gge.exe: ggnot.cc
	g++ -DGGEXEC -L/usr/local/lib -Wall -ansi -O3 -o gge.exe ggnot.cc -lgadu -lz
	strip -s gge.exe
gged.exe: ggnot.cc
	g++ -DGGEXEC -DMDBG -L/usr/local/lib -Wall -ansi -g3 -o gged.exe ggnot.cc -lgadu -lz
ggnot.exe: ggnot.cc
	g++ -L/usr/local/lib -Wall -ansi -O3 -o ggnot.exe ggnot.cc -lgadu -lz
	strip -s ggnot.exe
ggnotd.exe: ggnot.cc
	g++ -DMDBG -L/usr/local/lib -Wall -ansi -g3 -o ggnotd.exe ggnot.cc -lgadu -lz
clean:
	-rm -f ggnot ggnotd gge gged ggssh ggsshd *.exe *.stackdump *.core /tmp/ggssh*
clog:
	-rm -f /tmp/ggssh.*

