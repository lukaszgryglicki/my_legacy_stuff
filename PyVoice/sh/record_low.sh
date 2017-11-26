#!/bin/sh
rec -b 8 -r 6000 -c 1 -e unsigned-integer -t raw - | oggenc -r -B 8 -C 1 -R 6000 -q -1 - -o "$1.ogg"
#rec -b 8 -r 6000 -c 1 -e unsigned-integer -t raw "%1.raw" 

