#
# Generic makefile for xmountins. You will have to add the locations
# of your X include files and libraries to the appropriate macros
#
CC=cc
CFLAGS=-I/usr/include/X11
LDFLAGS=-L/usr/lib/X11

LOCAL_LIBRARIES = -lX11 
  SYS_LIBRARIES = -lm
           SRCS = calcalt.c random.c  artist.c xmountains.c X_graphics.c print_alg.c
           OBJS = calcalt.o random.o  artist.o xmountains.o X_graphics.o print_alg.o
           HDRS = crinkle.h  paint.h patchlevel.h copyright.h vroot.h

xmountains: $(OBJS)
	$(CC) $(LDFLAGS) -o xmountains $(OBJS) -lX11 -lm


calcalt.o: crinkle.h
artist.o: paint.h crinkle.h
xmountains.o: paint.h patchlevel.h copyright.h crinkle.h
X_graphics.o: paint.h crinkle.h

