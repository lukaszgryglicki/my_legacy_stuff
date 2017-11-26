all: mpctet render interpol newton
mpctet: mpctet.cc
	#c++ -g3 -pthread -o mpctet mpctet.cc -I/usr/local/include -L/usr/local/lib -lmpc -lm
	c++ -O4 -pthread -o mpctet mpctet.cc -I/usr/local/include -L/usr/local/lib -lmpc -lm
	strip -s mpctet
render: render.cc
	c++ -O4 -o render render.cc -I/usr/local/include -L/usr/local/lib -lglut -lGLU
	strip -s render
interpol: interpol.cc
	c++ -O4 -o interpol interpol.cc -I/usr/local/include -L/usr/local/lib -lm
	strip -s interpol
newton: newton.cc
	#c++ -O4 -pthread -o newton newton.cc -I/usr/local/include -L/usr/local/lib -lm -lmpc
	#strip -s newton
	c++ -g3 -pthread -o newton newton.cc -I/usr/local/include -L/usr/local/lib -lm -lmpc
clean: 
	-rm -f mpctet render interpol newton

