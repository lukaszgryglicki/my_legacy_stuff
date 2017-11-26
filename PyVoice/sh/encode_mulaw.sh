#!/bin/sh
sox -b 8 -c 1 -r 8000 -e mu-law $1 $1.ogg

