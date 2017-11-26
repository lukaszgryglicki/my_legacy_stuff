#!/bin/sh
./portscan -s c0,f2000,t-1,w10 -n c1,w40 -i c2,w100 -d c-1 -f c3,w4000 | sort -g -k 1 -r > report.txt
cat report.txt | grep 'games/' > games.txt

