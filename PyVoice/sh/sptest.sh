#!/bin/sh
#speexenc --rate 8000 --le --8bit --quality 0 --vbr --dtx -n - - < rec.raw > rec.spx
#speexdec --enh --rate 8000 --force-nb --mono - - < rec.spx > rec2.raw
#speexenc --rate 8000 --le --8bit --quality 0 --vbr --dtx -n - - < rec2.raw > rec2.spx
speexenc --rate 8000 --le --8bit --quality 0 - - < rec.raw > rec.spx
speexdec --rate 8000 --mono - - < rec.spx > rec2.raw
speexenc --rate 8000 --le --8bit --quality 0 - - < rec2.raw > rec2.spx
mplayer rec2.spx
