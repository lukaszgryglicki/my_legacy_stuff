all: xmsi xmsi4 xcpus xcpus4 msi xfast msi.exe msi4
xmsi: xmsi.c
	cc -Wall -ansi -pedantic  -O3 -fexpensive-optimizations -ffast-math -fomit-frame-pointer -I/usr/X11R6/include -L/usr/X11R6/lib -o xmsi xmsi.c -lX11 -lm
	-strip -s xmsi
xmsi4: xmsi4.c
	cc  -Wall -ansi -pedantic  -g3 -fexpensive-optimizations -ffast-math -fomit-frame-pointer -I/usr/X11R6/include -L/usr/X11R6/lib -o xmsi4 xmsi4.c -lX11 -lm
	#-strip -s xmsi4
xfast: fast.c
	cc -Wall -ansi -pedantic  -O3 -fexpensive-optimizations -ffast-math -fomit-frame-pointer -I/usr/X11R6/include -L/usr/X11R6/lib -o xfast fast.c -lX11 -lm
	-strip -s xcpus
xcpus4: cpus4.c
	cc  -Wall -ansi -pedantic  -O3 -fexpensive-optimizations -ffast-math -fomit-frame-pointer -I/usr/X11R6/include -L/usr/X11R6/lib -o xcpus4 cpus4.c -lX11 -lm
	-strip -s xcpus4
xcpus: cpus.c
	cc -Wall -ansi -pedantic  -O3 -fexpensive-optimizations -ffast-math -fomit-frame-pointer -I/usr/X11R6/include -L/usr/X11R6/lib -o xcpus cpus.c -lX11 -lm
	-strip -s xcpus
msi: msi.c
	cc -Wall -ansi -pedantic  -O3 -fexpensive-optimizations -ffast-math -fomit-frame-pointer -o msi msi.c -lm
	-strip -s msi
msi4: msi4.c
	cc -Werror -Wall -ansi -pedantic -O2 -o msi4 msi4.c -lm
	-strip -s msi4
msi.exe: msi.c
	cc -Wall -ansi -pedantic  -O3 -fexpensive-optimizations -ffast-math -fomit-frame-pointer -o msi.exe msi.c -lm -lc
package:
	tar jcf msi.tbz *.c Makefile
clean:
	-rm *.o *.exe *.tbz msi xmsi xcpus xfast xcpus4 xmsi4
# -DDEBUG -DDEBBUGALG
