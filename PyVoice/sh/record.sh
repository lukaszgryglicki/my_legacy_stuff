#!/bin/sh
rec -b 16 -r 44100 -c 1 -t wav - | oggenc -q -1 - -o "$1.ogg" 1>/dev/null 2>/dev/null

