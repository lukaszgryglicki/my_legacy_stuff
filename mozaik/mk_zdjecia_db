#!/bin/sh
find /data/Jpeg/Zdjecia -name "*.[jJ][pP][gGeE]*" > ./data_zdjecia.lst
./mozaik --generate-db data_zdjecia.lst --db-outfile zdjecia.dat --mode-lf --snap-dir zdjecia --jqual 90 --snap-x 512 --snap-y 512 --skip-bmpin --skip-bmpout
echo done
