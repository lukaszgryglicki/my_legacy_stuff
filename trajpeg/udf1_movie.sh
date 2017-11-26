#!/bin/sh
rm audio.mp2 00*.jpeg udf/00*.jpeg
ffmpeg  -i "$1" %08d.jpeg -ab 128k audio.mp2
for f in 00*.jpeg
do
    calln 1 fjpeg udf/$f 'udf1' 2 $f
done
rm 00*.jpeg
mv udf/00*.jpeg .
ffmpeg  -r $2 -b 4400k -ab 128k -i %08d.jpeg -i audio.mp2 "udf/$1"
rm 00*.jpeg audio.mp2
mv "$1" ./processed/
#mplayer "udf/$1"

