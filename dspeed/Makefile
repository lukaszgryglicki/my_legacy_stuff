all: dspeed_bsd install
dspeed_bsd: dspeed.c
	c++ -o dspeed_bsd dspeed.c -lcompat
install: dspeed_bsd
	mins dspeed_bsd

