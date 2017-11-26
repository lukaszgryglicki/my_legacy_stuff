#!/bin/sh
rm 00*.jpeg audio.mp2 audio2.mp2 flist.txt
ffmpeg  -i "$1" %08d.jpeg -ab 128k audio.mp2
for f in 00*.jpeg
do
    fjpeg udf/$f 'udf1' 2 $f
done
rm 00*.jpeg
mv udf/00*.jpeg .
find . -name "00*.jpeg" > flist.txt
inv_fns flist.txt
rm flist.txt
sox audio.mp2 audio2.mp2 reverse
rm audio.mp2
ffmpeg  -r $2 -b 1500k -ab 128k -i %08d.jpeg -i audio2.mp2 "udfinv_$1"
rm 00*.jpeg audio2.mp2
for f in 00*.jpeg; do rm -f $f; done

