#!/bin/sh
mixer vol 100 pcm 10 mic 100 rec 100
rm /tmp/PyVoice.* ./*.raw
./PyVoice.py -g -1 --low -a
sleep 2
./PyVoice.py -g -1 --low

