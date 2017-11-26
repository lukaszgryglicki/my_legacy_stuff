#!/bin/sh

echo 'usage par1=filename[out.speex] par2=[85] par3=quality[7] par4=Hz[8000] par5=bits[8] par6=channels[1] par7=log_prefix[]'
echo 'due to 4k buffering while recording, to kill recording'
echo 'and not loose last few minutes you must kill proper sox process'
echo 'notthis running shell, then speexenc stays alive, recognizes EOF'
echo 'and properly flushes buffer out to speex file'

fn=$1
if [ "$fn" = "" ]
then
    fn=out.speex
    echo "Give at leas output file name"
    exit 1
fi

mx=$2
if [ "$mx" = "" ]
then
    mx=85
fi

q=$3
if [ "$q" = "" ]
then
    q=7
fi

hz=$4
if [ "$hz" = "" ]
then
    hz=8000
fi

bi=$5
if [ "$bi" = "" ]
then
    bi=8
fi

ch=$6
if [ "$ch" = "" ]
then
    ch=1
fi

ch2=""
if [ "$ch" = "2" ]
then
    ch2="--stereo"
fi

ll=$7
l1=/dev/null
l2=/dev/null
l3=/dev/null
if [ "$ll" = "" ]
then
    echo "All logs skipped"
else
    l1="$ll.sox"
    l2="$ll.enc1"
    l3="$ll.enc2"
fi

rm -f "$fn"

echo "output:           $fn"
echo "mixer volume:     $mx"
echo "quality:          $q"
echo "sample rate:      $hz Hz"
echo "sample bits:      $bi"
echo "channels:         $ch"
echo "log sox err:      $l1"
echo "log enc out:      $l2"
echo "log enc err:      $l3"

mixer mic $mx
mixer rec $mx
echo "cdsp -B $bi -C $ch -R $hz 2>$l1 | speexenc --le --$bi bit --rate $hz $ch2 --nframes 10 --quality $q --vbr --dtx --comp 10 - $fn 1>$l2 2>$l3"
cdsp -B "$bi" -C "$ch" -R "$hz" 2>$l1 | speexenc --le --"$bi"bit --rate "$hz" $ch2 --nframes 10 --quality "$q" --vbr --dtx --comp 10 - "$fn" 1>$l2 2>$l3
