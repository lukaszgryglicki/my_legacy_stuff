#!/bin/sh
rm 00*.jpeg audio.mp2 audio2.mp2 flist.txt
ffmpeg  -i "$1" %08d.jpeg -ab 256k audio.mp2
find . -name "0*.jpeg" > flist.txt
inv_fns flist.txt
rm flist.txt
sox audio.mp2 audio2.mp2 reverse
rm audio.mp2
ffmpeg  -r 30 -b 6000k -ab 256k -i %08d.jpeg -i audio2.mp2 "hdinv_$1"
rm 00*.jpeg audio2.mp2
for f in 00*.jpeg; do rm -f $f; done

