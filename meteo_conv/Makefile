all: meteo_conv meteo_convd
meteo_conv: meteo_conv.cc
	g++ -Wall -ansi -pedantic -O3 -o meteo_conv meteo_conv.cc -lc -lm
	strip -s meteo_conv
meteo_convd: meteo_conv.cc
	g++ -Wall -ansi -pedantic -g3 -o meteo_convd meteo_conv.cc /usr/local/lib/libefence.a

