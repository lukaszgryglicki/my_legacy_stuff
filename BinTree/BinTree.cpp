#include "Exception.h"
#include "TBinTree.h"
#include "BinTree.h"
#include <iostream>

using namespace std;

template <class T>
BinTree<T> :: BinTree(const T& ref)
{
 try
  {
   root = new TBinTree<T>;
   root->SetElem(ref);
   if (!root) throw new Exception("blad alokacji: root");
  }
 catch (Exception* ex)
  {
   ex->Print();
   exit(1);
  }
 this->nelems = 1;
}

template <class T>
BinTree<T> :: BinTree()
{
 this->nelems = 0;
 this->root = NULL;
}

template <class T>
BinTree<T> :: BinTree(const BinTree<T>& M)
{
 this->nelems = M.nelems;
 if (nelems == 0) { root = NULL; return; }
 try
   {
    root = new TBinTree<T>;
    if (!root) throw new Exception("blad alokacji: root");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 TBinTree<T> *l, *r;
 l = M.root->GetL();
 r = M.root->GetR();
 root->SetElem(M.root->GetElem());
 root->SetL(NULL);
 root->SetR(NULL);
 root->CopyVLR(root, l);
 root->CopyVLR(root, r);
 nelems = M.nelems;
}

template <class T>
void BinTree<T> :: operator=(const BinTree<T>& ref)
{
 if (root) Free(root);
 this->nelems = ref.nelems;
 if (nelems == 0) { root = NULL; return; }
 try
   {
    root = new TBinTree<T>;
    if (!root) throw new Exception("blad alokacji: root");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 TBinTree<T> *l, *r;
 l = ref.root->GetL();
 r = ref.root->GetR();
 root->SetElem(ref.root->GetElem());
 root->SetL(NULL);
 root->SetR(NULL);
 root->CopyVLR(root, l);
 root->CopyVLR(root, r);
 nelems = ref.nelems;
}

template <class T>
BinTree<T>& BinTree<T> :: operator+(const BinTree<T>& ref)
{
 BinTree<T>* res;
 try
   {
    res = new BinTree<T>;
    if (!res) throw new Exception("blad alokacji: res");
    if (nelems != ref.nelems) throw new Exception("drzewa maja rozna ilosc elementow");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 res->nelems = nelems;
 if (nelems == 0) { res->root = NULL; return *res; }
 try
   {
    res->root = new TBinTree<T>;
    if (!res->root) throw new Exception("blad alokacji: root");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 TBinTree<T> *l1, *r1, *l2, *r2;
 const T& elm1 = root->GetElem();
 const T& elm2 = ref.root->GetElem();
 l1 = root->GetL();
 r1 = root->GetR();
 l2 = ref.root->GetL();
 r2 = ref.root->GetR();
 res->root->SetElem(elm1 + elm2);
 res->root->SetL(NULL);
 res->root->SetR(NULL);
 res->root->AddLRV(res->root, l1, l2);
 res->root->AddLRV(res->root, r1, r2);
 return *res;
}

template <class T>
BinTree<T>& BinTree<T> :: operator*(const BinTree<T>& ref)
{
 BinTree<T>* res;
 try
   {
    res = new BinTree<T>;
    if (!res) throw new Exception("blad alokacji: res");
    if (nelems != ref.nelems) throw new Exception("drzewa maja rozna ilosc elementow");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 res->nelems = nelems;
 if (nelems == 0) { res->root = NULL; return *res; }
 try
   {
    res->root = new TBinTree<T>;
    if (!res->root) throw new Exception("blad alokacji: root");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 TBinTree<T> *l1, *r1, *l2, *r2;
 const T& elm1 = root->GetElem();
 const T& elm2 = ref.root->GetElem();
 l1 = root->GetL();
 r1 = root->GetR();
 l2 = ref.root->GetL();
 r2 = ref.root->GetR();
 res->root->SetElem(elm1 * elm2);
 res->root->SetL(NULL);
 res->root->SetR(NULL);
 res->root->MulLRV(res->root, l1, l2);
 res->root->MulLRV(res->root, r1, r2);
 return *res;
}

template <class T>
void BinTree<T> :: Free(TBinTree<T>*& rt)
{
 TBinTree<T> *l, *r;
 l = rt->GetL();
 r = rt->GetR();
 if (l) Free(l);
 if (r) Free(r);
 if (rt) delete rt;
}

template <class T>
BinTree<T> :: ~BinTree()
{
 if (!root) return;
 Free(root);
}
template <class T>
TBinTree<T>* BinTree<T> :: GetRoot() const
{
 return this->root;
}
template <class T>
int BinTree<T> :: GetNElems() const
{
 return this->nelems;
}

template <class T>
ostream& operator<<(ostream& out, const BinTree<T>& ref)
{
 if (ref.GetRoot()) out<<"{ "<<ref.GetNElems()<<" "<<(*ref.GetRoot())<<"} ";
 else out<<"{ puste drzewo 0 }";
 return out;
}

template <class T>
void BinTree<T> :: Add(const T& ref)
{
 TBinTree<T>* tmp;
 try 
   {
    tmp = new TBinTree<T>;
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
    root->SetElem(ref);
    tmp->SetL(NULL);
    tmp->SetR(NULL);
    nelems = 1;
    return;
   }
 root->Add(root, ref);
 nelems++;
}
