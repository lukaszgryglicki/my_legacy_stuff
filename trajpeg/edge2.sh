#!/bin/sh
./fjpeg edge.jpeg "if(abs(ggs(y1)-ggs(y1_1'))-8,(sr(255*pow(gr(y1)/255,2))+sg(255*pow(gg(y1)/255,2))+sb(255*pow(gb(y1)/255,2))),sg(255*pow(ggs(y2)/255,2)))" 2 in/img_1239.jpg in/img_1237.jpg

