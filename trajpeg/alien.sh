#!/bin/sh
echo "alien.sh outfile infile1 infile2"
fjpeg $1 "sr(255*(1-((gr(y1)/255)^(gr(y2)/255))))+sg(255*(1-((gg(y1)/255)^(gg(y2)/255))))+sb(255*(1-((gb(y1)/255)^(gb(y2)/255))))" 2 $2 $3
xv -root $1 -quit

