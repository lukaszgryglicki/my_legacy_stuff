#!/bin/sh
mcompressf -i 12802177 -o 12802177.mz -n -s 6m -b 24 1>alg1.out 2>alg2.out
cat alg2.out | sort -g -k 2 -r > rep
vim rep

