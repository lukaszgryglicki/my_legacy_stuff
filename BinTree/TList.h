#ifndef TLIST_
#define TLIST_
class TList			//pojedynczy element listy
{				//cala lista to klasa List
 public:			//zawierajaca elementy TList
   TList();
   TList(int);
   ~TList();
   void SetElem(int);
   void SetNext(TList*);
   void SetPrev(TList*);
   TList* GetNext() const;
   TList* GetPrev() const;
   int GetElem()    const;
 private:
   TList* next;			//wskaznik na nastepny
   TList* prev;			//i poprzedni
   int elem;			//wartosc, lista NIE JEST szablonem, ma elementy int
};				//wazne: jest tylko jedna instancja klasy List
#endif				//zawierajaca wiele elementow klasy TList
