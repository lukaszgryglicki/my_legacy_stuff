#!/bin/sh
find /data/Jpeg -name "*.[jJ][pP][gGeE]*" > ./data_jpeg.lst
./mozaik --generate-db data_jpeg.lst --db-outfile all.dat --mode-lf --snap-dir all --jqual 90 --snap-x 256 --snap-y 256 --skip-bmpin --skip-bmpout
echo done
