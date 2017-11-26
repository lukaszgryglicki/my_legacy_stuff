#!/bin/sh
echo "./fjpeg mm.jpg 'sr(maxa(maxb(gr(y1), gr(y2)), mina(gr(y3), gr(y4)))) + sg(minb(min(gg(y1), gg(y2)), max(gg(y3), gg(y4)))) + sb(maxa(maxb(gb(y1), gb(y2)), mina(gb(y3), gb(y4))))' 2 in/img_1230.jpg in/img_1231.jpg in/img_1237.jpg in/img_1239.jpg"
./fjpeg mm.jpg 'sr(maxa(maxb(gr(y1), gr(y2)), mina(gr(y3), gr(y4)))) + sg(minb(min(gg(y1), gg(y2)), max(gg(y3), gg(y4)))) + sb(maxa(maxb(gb(y1), gb(y2)), mina(gb(y3), gb(y4))))' 2 in/img_1230.jpg in/img_1231.jpg in/img_1237.jpg in/img_1239.jpg
