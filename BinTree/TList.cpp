#include "TList.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

TList :: TList()
{
 this->elem = 0;
 this->next = this->prev = NULL;
}

TList :: TList(int el)
{
 this->elem = el;
 this->next = this->prev = NULL;
}

TList :: ~TList()
{
 this->elem = 0;
 this->next = this->prev = NULL;
}

void TList :: SetPrev(TList* prv)
{
 this->prev = prv;
}

void TList :: SetNext(TList* nxt)
{
 this->next = nxt;
}

void TList :: SetElem(int ele)
{
 this->elem = ele;
}

TList* TList :: GetNext() const
{
 return this->next;
}

TList* TList :: GetPrev() const
{
 return this->prev;
}

int TList :: GetElem() const
{
 return this->elem;
}

