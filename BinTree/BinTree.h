#ifndef BINTREE_
#define BINTREE_
#include <iostream>

using namespace std;

template <class T>		//szablonowa klasa drzewa binarnego
class BinTree			//elementy typu T
{				//T moze byc np: int,char,double,... class List
 public:
   BinTree();			//puste drzewo
   BinTree(const T&);		//drzewo z jednym elementem
   BinTree(const BinTree<T>&);	//konstruktor kopiujacy
   				//glebokie kopiowanie kazdego elementu
				//do drzewa docelowego
				//ZAUWAZ elementy sa typu T, wiec dla tego typu
				//musi byc zdefiniowany operator =
				//operator = dla listy jest zdefiniowany;
				//zobacz: CList.cpp, mozna wiec uzywac BinTree<List>
   ~BinTree();			//niszczy drzewo, dla kazdego elementu uzywa destruktora T
   				//np ~CList() gdy BinTree<CList>
   void operator=(const BinTree<T>&);	
   					//kopiuje cale drzewo uzywajac
					//operatora =, jak w konstruktorze kopiujacym
   BinTree<T>& operator+(const BinTree<T>&);
   BinTree<T>& operator*(const BinTree<T>&);
   TBinTree<T>* GetRoot() const;
   int GetNElems() const;
   void Add(const T&);
   void Free(TBinTree<T>*&);
 private:
   int nelems;			//ilosc elementow
   TBinTree<T>* root;		//korzen drzewa
};
#endif
