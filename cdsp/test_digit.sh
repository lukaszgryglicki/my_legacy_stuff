#!/bin/sh
./fdigitize -c 2 -r 44100 -b 16 -1 '0.3*sin(x*2000)+0.2*cos(x*500)+0.4*sin(x*10000)+0.3*sin(x*2000)*cos(x*7000)' -2 '0.3*sin(x*200)+0.2*cos(x*5000)+0.4*sin(x*1000)+0.3*sin(x*20000)*cos(x*700)' -n 5 > out.pcm
./cdsp -w -b 16 -c 2 -r 44100 < out.pcm
rm -f ./out.pcm
echo 'Stereo S16 44,1 kHz sineline tests done'

