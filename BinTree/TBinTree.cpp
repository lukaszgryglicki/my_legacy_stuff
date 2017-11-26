#include "Exception.h"
#include "TBinTree.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

template <class T>
TBinTree<T> :: TBinTree()
{
 // this->elem = 0; wywolany bedzie domyslny konstruktor typu T
 this->l = this->r = NULL;
}

template <class T>
TBinTree<T> :: TBinTree(const T& el)
{
 this->elem = el;
 this->l = this->r = NULL;
}

template <class T>
TBinTree<T> :: ~TBinTree()
{
 //this->elem = 0;
 this->l = this->r = NULL;
}

template <class T>
void TBinTree<T> :: SetL(TBinTree* ll)
{
 this->l = ll;
}

template <class T>
void TBinTree<T> :: SetR(TBinTree* rr)
{
 this->r = rr;
}

template <class T>
void TBinTree<T> :: SetElem(const T& ele)
{
 this->elem = ele;
}

template <class T>
TBinTree<T>* TBinTree<T> :: GetL() const
{
 return this->l;
}

template <class T>
TBinTree<T>* TBinTree<T> :: GetR() const
{
 return this->r;
}

template <class T>
const T& TBinTree<T> :: GetElem() const
{
return this->elem;
}

template <class T>
void TBinTree<T> :: MulLRV(TBinTree*& dst, const TBinTree* const& src1, const TBinTree* const& src2)
{
 TBinTree<T> *l1, *r1, *l2, *r2;
 if (!src1 && !src2) return;
 try
   {
    if ((src1 && !src2) || (src2 && !src1)) throw new Exception("rozne topologie drzew");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 l1 = src1->GetL();
 r1 = src1->GetR();
 l2 = src2->GetL();
 r2 = src2->GetR();
 const T& elm1 = src1->GetElem();
 const T& elm2 = src2->GetElem();
 if (l1 || l2) dst->MulLRV(dst, l1, l2);
 if (r1 || r2) dst->MulLRV(dst, r1, r2);
 dst->Mul2(dst, elm1, elm2);
}

template <class T>
void TBinTree<T> :: AddLRV(TBinTree*& dst, const TBinTree* const& src1, const TBinTree* const& src2)
{
 TBinTree<T> *l1, *r1, *l2, *r2;
 if (!src1 && !src2) return;
 try
   {
    if ((src1 && !src2) || (src2 && !src1)) throw new Exception("rozne topologie drzew");
   }
 catch (Exception* ex)
   {
    ex->Print();
    exit(1);
   }
 l1 = src1->GetL();
 r1 = src1->GetR();
 l2 = src2->GetL();
 r2 = src2->GetR();
 const T& elm1 = src1->GetElem();
 const T& elm2 = src2->GetElem();
 if (l1 || l2) dst->AddLRV(dst, l1, l2);
 if (r1 || r2) dst->AddLRV(dst, r1, r2);
 dst->Add2(dst, elm1, elm2);
}

template <class T>
void TBinTree<T> :: CopyLVR(TBinTree*& dst, const TBinTree* const& src)
{
 TBinTree<T> *l, *r;
 if (!src) return;
 l = src->GetL();
 r = src->GetR();
 const T& elm = src->GetElem();
 if (l) dst->CopyLVR(dst, l);
 dst->Add(dst, elm);
 if (r) dst->CopyLVR(dst, r);
}

template <class T>
void TBinTree<T> :: CopyVLR(TBinTree*& dst,const TBinTree* const& src)
{
 TBinTree<T> *l, *r;
 if (!src) return;
 l = src->GetL();
 r = src->GetR();
 const T& elm = src->GetElem();
 dst->Add(dst, elm);
 if (l) dst->CopyVLR(dst, l); 
 if (r) dst->CopyVLR(dst, r); 
}

template <class T>
void TBinTree<T> :: CopyLRV(TBinTree*& dst, const TBinTree* const& src)
{
 TBinTree<T> *l, *r;
 if (!src) return;
 l = src->GetL();
 r = src->GetR();
 const T& elm = src->GetElem();
 if (l) dst->CopyLRV(dst, l);
 if (r) dst->CopyLRV(dst, r);
 dst->Add(dst, elm);
}

template <class T>
void TBinTree<T> :: Mul2(TBinTree*& root, const T& ref1, const T& ref2)
{
 TBinTree<T> *l, *r;
 l = root->GetL();
 r = root->GetR();
 const T& ref = ref1 * ref2;
 root->Add(root, ref);
}

template <class T>
void TBinTree<T> :: Add2(TBinTree*& root, const T& ref1, const T& ref2)
{
 TBinTree<T> *l, *r;
 l = root->GetL();
 r = root->GetR();
 const T& ref = ref1 + ref2;
 root->Add(root, ref);
}

template <class T>
void TBinTree<T> :: Add(TBinTree*& root, const T& ref)
{
 TBinTree<T> *l, *r;
 l = root->GetL();
 r = root->GetR();
 const T& elm = root->GetElem();
 if (root) 
  {
   if (ref < elm && l)       
     {
      root->Add(l, ref); 
      return; 
     }
   else if (ref >= elm && r) 
     { 
      root->Add(r, ref); 
      return; 
     }
  }
 TBinTree<T>* ptr;
 try
   {
    ptr = new TBinTree<T>;
    if (!ptr) throw new Exception("blad alokacji: ptr");
   }
 catch (Exception* ex)
  {
   ex->Print();
   exit(1);
  }
 ptr->SetElem(ref);
 ptr->SetL(NULL);
 ptr->SetR(NULL);
 if (ref < elm) root->SetL(ptr);
 else root->SetR(ptr);
}

template <class T>
ostream& operator<<(ostream& out, const TBinTree<T>& ref)
{
 if (ref.GetL()) out<<"/"<<(*ref.GetL())<<"/";
 out<<" ["<<ref.GetElem()<<"] ";
 if (ref.GetR()) out<<"\\"<<(*ref.GetR())<<"\\";
 return out;
}
