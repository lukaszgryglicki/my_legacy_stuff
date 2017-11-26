all: cppMatrix cppMatrix.dbg
cppMatrix: cppMatrix.cc TemplateMatrix.cc List.cc TList.cc Exception.cc MathException.cc MemException.cc cppMatrix.hh TemplateMatrix.hh List.hh TList.hh Exception.hh MathException.hh MemException.hh Exceptions.hh
	c++ -Wall -ansi -pedantic -O3 -o cppMatrix cppMatrix.cc List.cc TList.cc Exception.cc MemException.cc MathException.cc
cppMatrix.dbg: cppMatrix.cc TemplateMatrix.cc List.cc TList.cc Exception.cc MathException.cc MemException.cc cppMatrix.hh TemplateMatrix.hh List.hh TList.hh Exception.hh MathException.hh MemException.hh Exceptions.hh
	c++ -Wall -ansi -pedantic -DDEBUG -g3 -o cppMatrix.dbg cppMatrix.cc List.cc TList.cc Exception.cc MemException.cc MathException.cc
clean:
	-rm -f cppMatrix cppMatrix.dbg
