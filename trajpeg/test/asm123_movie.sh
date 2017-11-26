#!/bin/sh
echo 'usage: par1=filename, par2=vbitrate[4000k]'
mkdir udf1 udf2 udf3

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
    fjpeg udf1/$f 'asmf1' 2 $f
    calln 2 fjpeg udf2/$f 'asmf2' 2 $f
    calln 3 fjpeg udf3/$f 'asmf3' 2 $f
done

for f in 00*.jpeg; do rm -f $f; done

ffmpeg -r 30 -i udf1/%08d.jpeg -i audio.mp2 -ab 128k -b:v $2 "udf1/$1"
ffmpeg -r 30 -i udf2/%08d.jpeg -i audio.mp2 -ab 128k -b:v $2 "udf2/$1"
ffmpeg -r 30 -i udf3/%08d.jpeg -i audio.mp2 -ab 128k -b:v $2 "udf3/$1"

#for f in udf1/00*.jpeg; do rm -f $f; done
#for f in udf2/00*.jpeg; do rm -f $f; done
#for f in udf3/00*.jpeg; do rm -f $f; done

#rm audio.mp2
