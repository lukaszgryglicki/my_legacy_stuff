#!/bin/sh
make install; rm effect.jpeg; fjpeg effect.jpeg "$1" 2 $2 $3 $4 $5 $6 $7 $8 $9; xv -root -quit effect.jpeg

