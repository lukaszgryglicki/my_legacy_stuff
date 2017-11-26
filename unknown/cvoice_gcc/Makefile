#!/bin/sh
CPP=g++45
CC=gcc45
STRIP=strip -s
MT=-DMMT -pthread
SPEEX=-DMSPX -lspeex
CPATH=-I/usr/local/include -I./ -L/usr/local/lib -L./
CFLAGS=-Wall -ansi -pedantic
OPT=-O3
DBG=-DMDBG -g3
#DBG=-g3
GDB=-g3
SHARED=-shared -fPIC
LIBCV=-L./ -I./ -lcvoice
LIBCVST=-L./ -I./ -lcvoicest
LIBCVRAW=-L./ -I./ -lcvoiceraw
LIBCVDBG=-L./ -I./ -lcvoicedbg
GTK=`pkg-config --cflags --libs gtk+-2.0`
CCWIN=g++
WIN=-DMWIN -lwinmm
WINMT=-DMMT
SIMPLEX=-DMNDPLX
IPC=-DMIPC -lmipc
IPCDBG=-DMIPC -lmipcdbg
WINIPC=-DMIPC
TEMPS=-save-temps
STATIC=-DMIPC -static /usr/local/lib/libspeex.a -lm
#ADD=-DMSLOW=400
ADD=
#EFENCE=/usr/local/lib/libefence.a
EFENCE=-lefence
all: cvoice cvoices cvoicest cvoiceraw cvoicedbg echosrv echocli mspxplay wizard libcvoice.so libcvoicest.so libcvoiceraw.so libcvoicedbg.so libmipc.so libmipcdbg.so gcvoice inf
win: cvoice.exe gcvoice.exe
inf: inf.c libmipc.so libcvoice.so
	$(CC) $(ADD) $(LIBCV) $(IPC) $(CPATH) $(CFLAGS) $(MT) -o inf inf.c
cvoices: cvoice.c libcvoice.c libcvoice.h libmipc.c libmipc.h
	$(CC) $(ADD) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) $(STATIC) -o cvoices cvoice.c libcvoice.c libmipc.c $(STATIC)
	$(STRIP) cvoices
libmipc.so: libmipc.c libmipc.h
	$(CC) $(ADD) $(SHARED) $(CFLAGS) $(OPT) -o libmipc.so libmipc.c
	$(STRIP) libmipc.so
libmipcdbg.so: libmipc.c libmipc.h
	$(CC) $(ADD) $(SHARED) $(CFLAGS) $(DBG) -o libmipcdbg.so libmipc.c
gcvoice: libcvoice.so libmipc.so gcvoice.c
	$(CC) $(ADD) $(LIBCV) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) $(IPC) $(GTK) -o gcvoice gcvoice.c
	$(STRIP) gcvoice
libcvoice.so: libcvoice.c libcvoice.h libmipc.so
	$(CC) $(ADD) $(SHARED) $(IPC) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o libcvoice.so libcvoice.c
	$(STRIP) libcvoice.so
cvoice: cvoice.c libcvoice.so libmipc.so
	$(CC) $(ADD) $(LIBCV) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(IPC) $(OPT) -o cvoice cvoice.c 
	$(STRIP) cvoice
libcvoicest.so: libcvoice.c libcvoice.h libmipc.so
	$(CC) $(ADD) $(SHARED) $(IPC) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o libcvoicest.so libcvoice.c
	$(STRIP) libcvoicest.so
cvoicest: cvoice.c libcvoicest.so libmipc.so
	$(CC) $(ADD) $(LIBCVST) $(IPC) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o cvoicest cvoice.c
	$(STRIP) cvoicest
libcvoicedbg.so: libcvoice.c libcvoice.h libmipcdbg.so
	$(CC) $(ADD) $(SHARED) $(MT) $(IPCDBG) $(SPEEX) $(DBG) $(CPATH) $(CFLAGS) -o libcvoicedbg.so libcvoice.c
cvoicedbg: cvoice.c libcvoicedbg.so libmipcdbg.so
	$(CC) $(ADD) $(LIBCVDBG) $(MT) $(IPCDBG) $(SPEEX) $(DBG) $(CPATH) $(CFLAGS) -o cvoicedbg cvoice.c
libcvoiceraw.so: libcvoice.c libcvoice.h libmipc.so
	$(CC) $(ADD) $(SHARED) $(MT) $(IPC) $(CPATH) $(CFLAGS) $(OPT) -o libcvoiceraw.so libcvoice.c
	$(STRIP) libcvoiceraw.so
cvoiceraw: cvoice.c libcvoiceraw.so libmipc.so
	$(CC) $(ADD) $(LIBCVRAW) $(MT) $(IPC) $(CPATH) $(CFLAGS) $(OPT) -o cvoiceraw cvoice.c
	$(STRIP) cvoiceraw
echosrv: echosrv.cc
	$(CPP) $(CFLAGS) $(OPT) $(MT) -o echosrv echosrv.cc
	$(STRIP) echosrv
echocli: echocli.c
	$(CC) $(CFLAGS) $(OPT) -o echocli echocli.c
	$(STRIP) echocli
mspxplay: mspxplay.c libcvoice.so libmipc.so
	$(CC) $(CPATH) $(CFLAGS) $(SPEEX) $(OPT) $(LIBCV) $(ADD) $(IPC) $(MT) -o mspxplay mspxplay.c
	$(STRIP) mspxplay
wizard: wizard.c
	$(CC) $(CFLAGS) $(OPT) -o wizard wizard.c
	$(STRIP) wizard
cvoice.exe: cvoice.c windsp.c libcvoice.c libcvoice.h
	$(CCWIN) $(ADD) $(CPATH) $(OPT) $(CFLAGS) $(WINMT) $(SIMPLEX) -o cvoice.exe cvoice.c windsp.c libcvoice.c libmipc.c $(WIN) $(SPEEX)
gcvoice.exe: gcvoice.c windsp.c libcvoice.c libcvoice.h
	$(CCWIN) $(ADD) $(CPATH) $(OPT) $(CFLAGS) $(WINMT) $(SIMPLEX) $(GTK) -o gcvoice.exe gcvoice.c windsp.c libcvoice.c libmipc.c $(WIN) $(SPEEX)
clean:
	-rm -f inf gcvoice cvoice cvoicedbg cvoiceraw cvoices cvoicest echosrv echocli mspxplay wizard *.core *.o *.exe *.so *.stackdump /tmp/cvoice*.log
install: all
	-cp *.so /usr/local/lib
	-cp inf gcvoice cvoice cvoices cvoicest cvoicedbg cvoiceraw mspxplay echosrv echocli wizard /usr/local/bin
	-ldconfig -m /usr/local/lib

