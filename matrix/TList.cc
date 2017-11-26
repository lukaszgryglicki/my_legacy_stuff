#include "TList.hh"
#include <stdlib.h>
#include <iostream>

using namespace std;

TList :: TList()
{
#ifdef DEBUG
 cout<<"constructor TList()"<<endl;
#endif
 this->elem = 0;
 this->next = this->prev = NULL;
}

TList :: TList(int el)
{
#ifdef DEBUG
 cout<<"constructor TList()"<<endl;
#endif
 this->elem = el;
 this->next = this->prev = NULL;
}

TList :: ~TList()
{
#ifdef DEBUG
 cout<<"destructor ~TList()"<<endl;
#endif
 this->elem = 0;
 this->next = this->prev = NULL;
}

void TList :: SetPrev(TList* prv)
{
#ifdef DEBUG
 cout<<"TList:::SetPrev("<<prv<<")"<<endl;
#endif
 this->prev = prv;
}

void TList :: SetNext(TList* nxt)
{
#ifdef DEBUG
 cout<<"TList:::SetNext("<<nxt<<")"<<endl;
#endif
 this->next = nxt;
}

void TList :: SetElem(int ele)
{
#ifdef DEBUG
 cout<<"TList:::SetElem("<<ele<<")"<<endl;
#endif
 this->elem = ele;
}

TList* TList :: GetNext() const
{
#ifdef DEBUG
 cout<<"TList:::GetNext()"<<endl;
#endif
 return this->next;
}

TList* TList :: GetPrev() const
{
#ifdef DEBUG
 cout<<"TList:::GetPrev()"<<endl;
#endif
 return this->prev;
}

int TList :: GetElem() const
{
#ifdef DEBUG
 cout<<"TList:::GetElem()"<<endl;
#endif
 return this->elem;
}

