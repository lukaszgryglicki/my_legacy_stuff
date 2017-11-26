#!/bin/sh
echo 'Usage: fogg.sh 'equation' [length=10] [rate=48000] [bits=16]'

eq=$1
if [ "$eq" = "" ]
then
    eq='sin(1000*x*x)'
fi

n=$2
if [ "$n" = "" ]
then
    n=10
fi

rate=$3
if [ "$rate" = "" ]
then
    rate=48000
fi

bits=$4
if [ "$bits" = "" ]
then
    bits=16
fi

echo "Equation: $eq"
echo "Seconds: $n"
echo "Hz: $rate"
echo "Bits: $bits"

echo "Digitizing..."
fdigitize -b $bits -r $rate  -1 "$eq" -n $n > out.raw
echo "Making WAV header..."
if [ "$bits" = "8" ]
then
    	echo "Sox 8bits, rate $rate..."
	sox -r $rate -e unsigned -b 8 -c 1 out.raw out.wav
elif [ "$bits" = "16" ]
then
    	echo "Sox 16bits, rate $rate..."
	sox -r $rate -e signed -b 16 -c 1 out.raw out.wav
else
    	echo "Bad bits value: $bits, allowed 8 or 16"
	exit 1
fi
echo "Encoding to Ogg..."
oggenc -q 10 out.wav
rm out.wav
echo "Playing OGG..."
mplayer out.ogg
echo "Playing RAW..."
cdsp -b $bits -r $rate -w < out.raw

