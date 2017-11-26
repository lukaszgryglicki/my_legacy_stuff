#!/bin/sh
sox -b 8 -c 1 -r 6000 -e mu-law $1 $1.ogg

