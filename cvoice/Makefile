#!/bin/sh
CPP=g++
CC=gcc
STRIP=strip -s
MT=-DMMT -pthread
SPEEX=-DMSPX -lspeex
CPATH=-I/usr/local/include -I./ -L/usr/local/lib -L./
CFLAGS=-Wall -ansi -pedantic
OPT=-O3
DBG=-DMDBG -g3
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
STATIC=-DMIPC -static /usr/local/lib/libspeex.a
#ADD=-DMSLOW=3000
ADD=
#EFENCE=/usr/local/lib/libefence.a
EFENCE=-lefence
all: cvoice cvoices cvoicest cvoiceraw cvoicedbg echosrv echocli mspxplay wizard libcvoice.so libcvoicest.so libcvoiceraw.so libcvoicedbg.so libmipc.so libmipcdbg.so gcvoice inf
win: cvoice.exe gcvoice.exe
inf: inf.cc libmipc.so libcvoice.so
	$(CC) $(ADD) $(LIBCV) $(IPC) $(CPATH) $(CFLAGS) -o inf inf.cc
cvoices: cvoice.cc libcvoice.cc libcvoice.h libmipc.cc libmipc.h
	$(CPP) $(ADD) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) $(STATIC) -o cvoices cvoice.cc libcvoice.cc libmipc.cc $(STATIC)
	$(STRIP) cvoices
libmipc.so: libmipc.cc libmipc.h
	$(CPP) $(ADD) $(SHARED) $(CFLAGS) $(OPT) -o libmipc.so libmipc.cc
	$(STRIP) libmipc.so
libmipcdbg.so: libmipc.cc libmipc.h
	$(CPP) $(ADD) $(SHARED) $(CFLAGS) $(DBG) -o libmipcdbg.so libmipc.cc
gcvoice: libcvoice.so libmipc.so gcvoice.cc
	$(CPP) $(ADD) $(LIBCV) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) $(IPC) $(GTK) -o gcvoice gcvoice.cc
	$(STRIP) gcvoice
libcvoice.so: libcvoice.cc libcvoice.h libmipc.so
	$(CPP) $(ADD) $(SHARED) $(IPC) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o libcvoice.so libcvoice.cc
	$(STRIP) libcvoice.so
cvoice: cvoice.cc libcvoice.so libmipc.so
	$(CPP) $(ADD) $(LIBCV) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(IPC) $(OPT) -o cvoice cvoice.cc 
	$(STRIP) cvoice
libcvoicest.so: libcvoice.cc libcvoice.h libmipc.so
	$(CPP) $(ADD) $(SHARED) $(IPC) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o libcvoicest.so libcvoice.cc
	$(STRIP) libcvoicest.so
cvoicest: cvoice.cc libcvoicest.so libmipc.so
	$(CPP) $(ADD) $(LIBCVST) $(IPC) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o cvoicest cvoice.cc
	$(STRIP) cvoicest
libcvoicedbg.so: libcvoice.cc libcvoice.h libmipcdbg.so
	$(CPP) $(ADD) $(SHARED) $(MT) $(IPCDBG) $(SPEEX) $(DBG) $(CPATH) $(CFLAGS) -o libcvoicedbg.so libcvoice.cc
cvoicedbg: cvoice.cc libcvoicedbg.so libmipcdbg.so
	$(CPP) $(ADD) $(LIBCVDBG) $(MT) $(IPCDBG) $(SPEEX) $(DBG) $(CPATH) $(CFLAGS) -o cvoicedbg cvoice.cc
libcvoiceraw.so: libcvoice.cc libcvoice.h libmipc.so
	$(CPP) $(ADD) $(SHARED) $(MT) $(IPC) $(CPATH) $(CFLAGS) $(OPT) -o libcvoiceraw.so libcvoice.cc
	$(STRIP) libcvoiceraw.so
cvoiceraw: cvoice.cc libcvoiceraw.so libmipc.so
	$(CPP) $(ADD) $(LIBCVRAW) $(MT) $(IPC) $(CPATH) $(CFLAGS) $(OPT) -o cvoiceraw cvoice.cc
	$(STRIP) cvoiceraw
echosrv: echosrv.cc
	$(CPP) $(CFLAGS) $(OPT) $(MT) -o echosrv echosrv.cc
	$(STRIP) echosrv
echocli: echocli.cc
	$(CPP) $(CFLAGS) $(OPT) -o echocli echocli.cc
	$(STRIP) echocli
mspxplay: mspxplay.cc libcvoice.so libmipc.so
	$(CPP) $(CPATH) $(CFLAGS) $(SPEEX) $(OPT) $(LIBCV) $(ADD) $(IPC) -o mspxplay mspxplay.cc
	$(STRIP) mspxplay
wizard: wizard.cc
	$(CPP) $(CFLAGS) $(OPT) -o wizard wizard.cc
	$(STRIP) wizard
cvoice.exe: cvoice.cc windsp.cc libcvoice.cc libcvoice.h
	$(CCWIN) $(ADD) $(CPATH) $(OPT) $(CFLAGS) $(WINMT) $(SIMPLEX) -o cvoice.exe cvoice.cc windsp.cc libcvoice.cc libmipc.cc $(WIN) $(SPEEX)
gcvoice.exe: gcvoice.cc windsp.cc libcvoice.cc libcvoice.h
	$(CCWIN) $(ADD) $(CPATH) $(OPT) $(CFLAGS) $(WINMT) $(SIMPLEX) $(GTK) -o gcvoice.exe gcvoice.cc windsp.cc libcvoice.cc libmipc.cc $(WIN) $(SPEEX)
clean:
	-rm -f inf gcvoice cvoice cvoicedbg cvoiceraw cvoices cvoicest echosrv echocli mspxplay wizard *.core *.o *.exe *.so *.stackdump /tmp/cvoice*.log
install: all
	-cp *.so /usr/local/lib
	-cp inf gcvoice cvoice cvoices cvoicest cvoicedbg cvoiceraw mspxplay echosrv echocli wizard /usr/local/bin
	-ldconfig -m /usr/local/lib

