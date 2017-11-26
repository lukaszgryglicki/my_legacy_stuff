CC=gcc
STRIP=strip
all: mplayereq
mplayereq: mplayereq.c
	$(CC) -O -o mplayereq mplayereq.c -lc
	$(STRIP) -s mplayereq
install: mplayereq
	-cp -vi mplayereq /usr/local/bin
	-chmod +s /usr/local/bin/mplayereq
test: mplayereq
	./mplayereq rock -ao oss -vo null /data/mus/DarkFuneral/VobiscumSatanas/*

