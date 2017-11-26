#!/bin/sh
echo "sa.sh outfile infile"
fjpeg $1 "sr(255*(1-((gr(y1_8')/255)^(gr(y1_1')/255))))+sg(255*(1-((gg(y1_6')/255)^(gg(y1_3')/255))))+sb(255*(1-((gb(y1_2')/255)^(gb(y1_7')/255))))" 2 $2 && xv -root $1 -quit

