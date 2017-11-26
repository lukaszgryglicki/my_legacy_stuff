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
WINIPC=-DMIPC
TEMPS=
all: cvoice cvoicest cvoiceraw cvoicedbg echosrv echocli mspxplay wizard libcvoice.so libcvoicest.so libcvoiceraw.so libcvoicedbg.so libmipc.so gcvoice
win: cvoice.exe
libmipc.so: libmipc.cc libmipc.h
	$(CPP) $(SHARED) $(CFLAGS) $(OPT) -o libmipc.so libmipc.cc
gcvoice: libcvoice.so libmipc.so gcvoice.cc
	$(CPP) $(LIBCV) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) $(IPC) $(GTK) -o gcvoice gcvoice.cc
	$(STRIP) gcvoice
libcvoice.so: libcvoice.cc libcvoice.h libmipc.so
	$(CPP) $(SHARED) $(IPC) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o libcvoice.so libcvoice.cc
	$(STRIP) libcvoice.so
cvoice: cvoice.cc libcvoice.so libmipc.so
	$(CPP) $(LIBCV) $(MT) $(SPEEX) $(CPATH) $(CFLAGS) $(IPC) $(OPT) -o cvoice cvoice.cc
	$(STRIP) cvoice
libcvoicest.so: libcvoice.cc libcvoice.h
	$(CPP) $(SHARED) $(IPC) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o libcvoicest.so libcvoice.cc
	$(STRIP) libcvoicest.so
cvoicest: cvoice.cc libcvoicest.so
	$(CPP) $(LIBCVST) $(IPC) $(SPEEX) $(CPATH) $(CFLAGS) $(OPT) -o cvoicest cvoice.cc
	$(STRIP) cvoicest
libcvoicedbg.so: libcvoice.cc libcvoice.h
	$(CPP) $(SHARED) $(MT) $(IPC) $(SPEEX) $(DBG) $(CPATH) $(CFLAGS) -o libcvoicedbg.so libcvoice.cc
cvoicedbg: cvoice.cc libcvoicedbg.so
	$(CPP) $(LIBCVDBG) $(MT) $(IPC) $(SPEEX) $(DBG) $(CPATH) $(CFLAGS) -o cvoicedbg cvoice.cc
libcvoiceraw.so: libcvoice.cc libcvoice.h
	$(CPP) $(SHARED) $(MT) $(IPC) $(CPATH) $(CFLAGS) $(OPT) -o libcvoiceraw.so libcvoice.cc
	$(STRIP) libcvoiceraw.so
cvoiceraw: cvoice.cc libcvoiceraw.so
	$(CPP) $(LIBCVRAW) $(MT) $(IPC) $(CPATH) $(CFLAGS) $(OPT) -o cvoiceraw cvoice.cc
	$(STRIP) cvoiceraw
echosrv: echosrv.cc
	$(CPP) $(CFLAGS) $(OPT) $(MT) -o echosrv echosrv.cc
	$(STRIP) echosrv
echocli: echocli.cc
	$(CPP) $(CFLAGS) $(OPT) -o echocli echocli.cc
	$(STRIP) echocli
mspxplay: mspxplay.cc
	$(CPP) $(CPATH) $(CFLAGS) $(SPEEX) $(OPT) -o mspxplay mspxplay.cc
	$(STRIP) mspxplay
wizard: wizard.cc
	$(CPP) $(CFLAGS) $(OPT) -o wizard wizard.cc
	$(STRIP) wizard
cvoice.exe: cvoice.cc windsp.cc libcvoice.cc libcvoice.h
	$(CCWIN) $(CPATH) $(OPT) $(CFLAGS) $(WINMT) $(SIMPLEX) -o cvoice.exe cvoice.cc windsp.cc libcvoice.cc libmipc.cc $(WIN) $(SPEEX)
clean:
	-rm -f gcvoice cvoice cvoicedbg cvoiceraw cvoicest echosrv echocli mspxplay wizard *.core *.o *.exe *.so *.stackdump /tmp/cvoice*.log
install: all
	-cp *.so /usr/local/lib
	-cp gcvoice cvoice cvoicest cvoicedbg cvoiceraw mspxplay echosrv echocli wizard /usr/local/bin
	-ldconfig -m /usr/local/lib

