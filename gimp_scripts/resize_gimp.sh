#!/bin/sh
#(let* ((x(car(gimp-file-load 1 "bla.jpeg" "bla.jpeg")))) (gimp-image-scale-full x (/ (car(gimp-image-width x)) 4) (/ (car(gimp-image-height x)) 4) 2) (file-jpeg-save 1 x (car(gimp-image-get-active-drawable x)) "bla2.jpeg" "bla2.jpeg" 0.6 0.8 1 1 "gimp" 1 1 0 1))
for f in $*
do
	echo "(let* ((x(car(gimp-file-load 1 \"$f\" \"$f\")))) (gimp-image-scale-full x (/ (car(gimp-image-width x)) 4) (/ (car(gimp-image-height x)) 4) 2) (file-jpeg-save 1 x (car(gimp-image-get-active-drawable x)) \"scaled_$f\" \"scaled_$f\" 0.6 0.8 1 1 \"MorgothV8 Gimped\" 1 1 0 1) (gimp-quit 0))"
	gimp -i -d -f -s -b  "(let* ((x(car(gimp-file-load 1 \"$f\" \"$f\")))) (gimp-image-scale-full x (/ (car(gimp-image-width x)) 4) (/ (car(gimp-image-height x)) 4) 2) (file-jpeg-save 1 x (car(gimp-image-get-active-drawable x)) \"scaled_$f\" \"scaled_$f\" 0.6 0.8 1 1 \"MorgothV8 Gimped\" 1 1 0 1) (gimp-quit 0))"
done

