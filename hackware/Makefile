CC = gcc46
STRIP = strip
all: hackware busy hwc
hackware: hackware.c
#	$(CC) -pthread -O3 -funroll-all-loops -o hackware hackware.c
	$(CC) -O3 -funroll-all-loops -o hackware hackware.c -lc
	$(STRIP) hackware
busy: busy.c
	$(CC) -o busy busy.c
	$(STRIP) busy
hwc: hwc.c
	$(CC) -save-temps -funroll-all-loops -fomit-frame-pointer -o hwc hwc.c
	$(STRIP) hwc
install: hackware hwc
	-cp -v hackware hwc /usr/local/bin

