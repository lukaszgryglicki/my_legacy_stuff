#include "Exception.h"
#include "TList.h"
#include "CList.h"
#include <stdlib.h>


CList :: CList()
{
 root = NULL;
 nelems = 0;
}

CList :: CList(int val)
{
 try
   {
    root = new TList;
    if (!root) throw new Exception("blad alokacji: head");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 root->SetElem(val);
 root->SetNext(root);
 root->SetPrev(root);
 nelems = 1;
}

CList :: CList(const CList& ref)
{
 if (root)			//jesli istnieje to zwolnij
 {
  TList* tmp = root->GetNext();
  if (tmp == root)
    {
     delete root;
     root = NULL;
     nelems = 0;
    }
  else
   {
    while (tmp != root)
      {
       delete tmp->GetPrev();
       tmp = tmp->GetNext();
      }
    root = NULL;
    nelems = 0;
   }
 }
 TList* tmp2=ref.root;
 this->nelems = ref.nelems;
 if (tmp2)	//FIXME
   {
    Add(tmp2->GetElem());
    tmp2 = tmp2->GetNext();
   }
 while (tmp2 != ref.root)
   {
    Add(tmp2->GetElem()); //kladz na ogon kolejne pobrane
    tmp2 = tmp2->GetNext();
   }
}


void CList::operator=(const CList& ref)
{
 if (nelems != 0)	//zwolnij jak cos na liscie
   {
    TList* tmp = root->GetNext();
    if (tmp == root)
       {
        delete root;
        root = NULL;
        nelems = 0;
        return;
       }
    while (tmp != root)
       {
        delete tmp->GetPrev();
        tmp = tmp->GetNext();
       }
/*    delete root;*/
    root = NULL;
    nelems = 0;
   }
 TList* tmp=ref.root;
 this->nelems = ref.nelems;
 if (tmp)	//FIXME
   {
    Add(tmp->GetElem());
    tmp = tmp->GetNext();
   }
 while (tmp != ref.root)
   {
    Add(tmp->GetElem()); //kladz na ogon kolejne pobrane
    tmp = tmp->GetNext();
   }
}

CList :: ~CList()
{
 if (!root) return;
 TList* tmp = root->GetNext();
 if (tmp == root)
   {
    delete root;
    root = NULL;
    nelems = 0;
    return;
   }
 while (tmp != root)
   {
    delete tmp->GetPrev();
    tmp = tmp->GetNext();
   }
/* delete root;*/
 root = NULL;
 nelems = 0;
}

void CList :: Add(int elem)
{
 TList* tmp;
 try 
   {
    tmp = new TList(elem);
    if (!tmp) throw new Exception("blad alokacji: tmp");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 if (!root) 
   {
    root = tmp;
    tmp->SetNext(tmp);
    tmp->SetPrev(tmp);
    nelems = 1;
    return;
   }
 tmp->SetPrev(root->GetPrev());
 (root->GetPrev())->SetNext(tmp);
 tmp->SetNext(root);
 root->SetPrev(tmp);
 root = tmp;
 nelems++;
}

ostream& operator<<(ostream& out, const CList& ref)
{
 if (ref.GetNElems() == 0)
   {
    out<<"pusta lista ";
    return out;
   }
 TList* tmp = ref.GetRoot();
 if (tmp)
   {
    out<<tmp->GetElem()<<",";
    tmp = tmp->GetNext();
   }
 while (tmp != ref.GetRoot())
   {
    out<<tmp->GetElem()<<",";
    tmp = tmp->GetNext();
   }
 return out;
}

bool CList :: operator>(const CList& ref) const
{
 try 
   {
    if (nelems != ref.nelems) throw new Exception("rozne dlugosci list");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 int s1,s2;
 s1 = s2 = 0;
 TList* tmp1 = root;
 TList* tmp2 = ref.root;
 if (tmp1 && tmp2)
   {
    s1 += tmp1->GetElem();
    s2 += tmp2->GetElem();
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 while (tmp1 != root && tmp2 != ref.root)
   {
    s1 += tmp1->GetElem();
    s2 += tmp2->GetElem();
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 return (s1 > s2);
}

bool CList :: operator>=(const CList& ref) const
{
 try 
   {
    if (nelems != ref.nelems) throw new Exception("rozne dlugosci list");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 int s1,s2;
 s1 = s2 = 0;
 TList* tmp1 = root;
 TList* tmp2 = ref.root;
 if (tmp1 && tmp2)
   {
    s1 += tmp1->GetElem();
    s2 += tmp2->GetElem();
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 while (tmp1 != root && tmp2 != ref.root)
   {
    s1 += tmp1->GetElem();
    s2 += tmp2->GetElem();
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 return (s1 >= s2);
}

bool CList :: operator<(const CList& ref) const
{
 try 
   {
    if (nelems != ref.nelems) throw new Exception("rozne dlugosci list");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 int s1,s2;
 s1 = s2 = 0;
 TList* tmp1 = root;
 TList* tmp2 = ref.root;
 if (tmp1 && tmp2)
   {
    s1 += tmp1->GetElem();
    s2 += tmp2->GetElem();
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 while (tmp1 != root && tmp2 != ref.root)
   {
    s1 += tmp1->GetElem();
    s2 += tmp2->GetElem();
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 return (s1 < s2);
}

bool CList :: operator<=(const CList& ref) const
{
 try 
   {
    if (nelems != ref.nelems) throw new Exception("rozne dlugosci list");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 int s1,s2;
 s1 = s2 = 0;
 TList* tmp1 = root;
 TList* tmp2 = ref.root;
 if (tmp1 && tmp2)
   {
    s1 += tmp1->GetElem();
    s2 += tmp2->GetElem();
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 while (tmp1 != root && tmp2 != ref.root)
   {
    s1 += tmp1->GetElem();
    s2 += tmp2->GetElem();
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 return (s1 <= s2);
}

void CList::operator+=(const CList& ref)
{
 try 
   {
    if (nelems != ref.nelems) throw new Exception("rozne dlugosci list");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 TList* tmp1 = root;
 TList* tmp2 = ref.root;
 if (tmp1 && tmp2)
   {
    tmp1->SetElem(tmp1->GetElem()+tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 while (tmp1 != root && tmp2 != ref.root)
   {
/*    ptr->AddOnHead(tmp1->GetElem()+tmp2->GetElem());*/
    tmp1->SetElem(tmp1->GetElem()+tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
}

void CList::operator*=(const CList& ref)
{
 try 
   {
    if (nelems != ref.nelems) throw new Exception("rozne dlugosci list");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 TList* tmp1 = root;
 TList* tmp2 = ref.root;
 if (tmp1 && tmp2)
   {
    tmp1->SetElem(tmp1->GetElem()*tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 while (tmp1 != root && tmp2 != ref.root)
   {
/*    ptr->AddOnHead(tmp1->GetElem()+tmp2->GetElem());*/
    tmp1->SetElem(tmp1->GetElem()*tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
}

CList& CList :: operator*(const CList& ref) const
{
 CList* ptr;
 try 
   {
    ptr = new CList();
    if (!ptr) throw new Exception("blad alokacji: ptr");
    if (nelems != ref.nelems) throw new Exception("rozne dlugosci list");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 TList* tmp1 = root;
 TList* tmp2 = ref.root;
 if (tmp1 && tmp2)
   {
    ptr->Add(tmp1->GetElem()*tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 while (tmp1 != root && tmp2 != ref.root)
   {
    ptr->Add(tmp1->GetElem()*tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 return *ptr;
}

CList& CList :: operator+(const CList& ref) const
{
 CList* ptr;
 try 
   {
    ptr = new CList();
    if (!ptr) throw new Exception("blad alokacji: ptr");
    if (nelems != ref.nelems) throw new Exception("rozne dlugosci list");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1); 
   }
 TList* tmp1 = root;
 TList* tmp2 = ref.root;
 if (tmp1 && tmp2)
   {
    ptr->Add(tmp1->GetElem()+tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 while (tmp1 != root && tmp2 != ref.root)
   {
    ptr->Add(tmp1->GetElem()+tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 return *ptr;
}


int CList :: GetNElems() const
{
 return this->nelems;
}

TList* CList :: GetRoot() const
{
 return this->root;
}

