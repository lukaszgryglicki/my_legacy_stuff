#!/bin/sh
echo 'usage: par1=filename, par2=vbitrate[4000k]'
mkdir udf

rm audio.mp2
for f in 00*.jpeg; do rm -f $f; done
for f in 00*.png; do rm -f $f; done

ffmpeg  -i "$1" %08d.png -ab 128k audio.mp2

for f in 00*.png
do
    FNAME=`echo $f | cut -f 1 -d .`.jpeg
    convert $f -quality 100 $FNAME  
    rm $f
    echo "$FNAME"
done

for f in 00*.jpeg
do
    calln 2 fjpeg udf/$f 'asmf2' 2 $f
done

for f in udf/00*.jpeg; do mv -v $f ./; done

ffmpeg -r 30 -i %08d.jpeg -i audio.mp2 -ab 128k -b:v $2 "udf/$1"

for f in 00*.jpeg; do rm -f $f; done
rm audio.mp2

