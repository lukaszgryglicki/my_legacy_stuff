#!/bin/sh
rm $1_$2; fjpeg $1_$2 "$1" 2 $2 $3 $4 $5 $6 $7 $8 $9; xv -root -quit $1_$2

