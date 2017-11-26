all: Matrix
Matrix: main.cpp BinTree.cpp CList.cpp TList.cpp TBinTree.cpp Exception.cpp main.h BinTree.h CList.h TList.h TBinTree.h Exception.h
	c++ -Wall -ansi -pedantic -O3 -o Matrix main.cpp CList.cpp TList.cpp Exception.cpp
	strip -s Matrix
clean:
	-rm -f Matrix
