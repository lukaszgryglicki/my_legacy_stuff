#!/bin/sh
rm 0*.jpeg audio.mp2 audio2.mp2 flist.txt
ffmpeg  -i "$1" %08d.png -ab 128k audio.mp2
for f in 0*.png
do
    convert -quality 100 $f $f.jpeg
    rm $f
    echo "Converted $f"
done
find . -name "0*.jpeg" > flist.txt
inv_fns flist.txt
rm flist.txt
sox audio.mp2 audio2.mp2 reverse
rm audio.mp2
ffmpeg  -r $2 -sameq -qmax 4 -ab 128k -i %08d.png.jpeg -i audio2.mp2 "inv_$1"
rm 0*.jpeg audio2.mp2
for f in 0*.jpeg; do rm -f $f; done
mencoder -oac mp3lame -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=$3:vhq -lameopts q=1:vbr=2 "inv_$1" -o "inv_$1.avi"

