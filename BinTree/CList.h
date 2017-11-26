#ifndef CLIST_
#define CLIST_
#include <iostream>
using namespace std;
class CList		//klasa opakowuje elementy list bedace klasami TList
{			//przechowuje wskaznik, ilosc elementow
 public:		//zarzadza dodawaniem/usuwaniem itd
   CList(const CList&);	//konstruktor kopiujacy
   CList(int);		//tworzy liste z jednym elementem
   CList();		//tworzy pusta liste, CList nie ma zadnego elementu TList wtedy
   ~CList();		//zwalnia pamiec listy
   void Add(int);
   int GetNElems() const;
   TList* GetRoot() const;
   void operator=(const CList&);   //kopiuje (glebokie kopiowanie) cala liste
   void operator+=(const CList&);  //dodaje elementy lub wyjatek
   void operator*=(const CList&);  //dodaje elementy lub wyjatek
   CList& operator+(const CList&) const; //mnozy elementy lub wyjatek
   CList& operator*(const CList&) const; //mnozy elementy lub wyjatek
   bool operator>(const CList&)   const;
   bool operator>=(const CList&)  const;
   bool operator<(const CList&)   const;
   bool operator<=(const CList&)  const;
   friend ostream& operator<<(ostream&, const CList&);
 private:
   TList* root;			//wskaznik  do root'a listy
   int nelems;			//ilosc elementow na liscie
};

#endif
