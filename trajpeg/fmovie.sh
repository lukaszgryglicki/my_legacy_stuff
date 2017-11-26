#!/bin/sh
echo 'usage: par1=filename[mandatory], par2=rank[1] par3=func_rank[asmf$rank] par4=vbitrate[3000k], par5=abitrate[128k], par6=fps[30], par7=nthreads[2], par8=do_rm[no], par9=add_opts[]'


fn=$1
if [ "$fn" = "" ]
then
    echo 'Filename is mandatory'
    exit 1
fi

rank=$2
if [ "$rank" = "" ]
then
    rank=1
fi

ffunc=$3
if [ "$ffunc" = "" ]
then
    ffunc=asmf$rank
fi

vbr=$4
if [ "$vbr" = "" ]
then
    vbr=3000k
fi

abr=$5
if [ "$abr" = "" ]
then
    abr=128k
fi

fps=$6
if [ "$fps" = "" ]
then
    fps=30.0
fi

thr=$7
if [ "$thr" = "" ]
then
    thr=2
fi

dorm=$8
if [ "$dorm" = "" ]
then
    dorm="no"
fi

aopts=$9

echo "Input filename:  $fn"
echo "Output filename: udf/$fn"
echo "Rank:            $rank"
echo "Function:        $ffunc"
echo "Video bitrate:   $vbr"
echo "Audio bitrate:   $abr"
echo "FPS:             $fps"
echo "Threads:         $thr"
echo "Add options:     $aopts"

echo "Making udf directory"
mkdir udf

if [ "$dorm" = "yes" ]
then
    echo 'Removing PNGs, JPGs and audio.mp2'
    for f in 00*.png; do rm -f $f; done
    for f in 00*.jpeg; do rm -f $f; done
    rm audio.mp2
fi

echo "ffmpeg -i $fn %08d.png -ab $abr audio.mp2"
ffmpeg -i "$fn" %08d.png -ab $abr audio.mp2

for f in 00*.png
do
    fname=`echo udf/$f | cut -f 1 -d .`.jpeg
#    echo "calln $rank fjpeg $fname $ffunc $thr $f -q95 $aopts"
    calln $rank fjpeg "$fname" "$ffunc" $thr "$f" -q95 $aopts
done

echo 'Moving JPGs'
for f in udf/00*.jpeg; do mv -v $f ./; done

echo "Removing udf/$fn"
rm -vf "udf/$fn"

echo "ffmpeg -r $fps -i %08d.jpeg -i audio.mp2 -ab $abr -b:v $vbr udf/$fn"
ffmpeg -r $fps -i %08d.jpeg -i audio.mp2 -ab $abr -b:v $vbr "udf/$fn"

if [ "$dorm" = "yes" ]
then
    echo 'Removing PNGs, JPGs and audio.mp2'
    for f in 00*.jpeg; do rm -f $f; done
    for f in 00*.png; do rm -f $f; done
    rm audio.mp2
fi

echo "File udf/$fn created"

