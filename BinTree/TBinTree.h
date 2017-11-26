#ifndef TBINTREE_
#define TBINTREE_
template <class T>
class TBinTree			//pojedynczy element listy
{				//cala lista to klasa BinTree
 public:			//zawierajaca elementy TBinTree
   TBinTree();
   TBinTree(const T&);
   ~TBinTree();
   void SetElem(const T&);
   void SetL(TBinTree*);
   void SetR(TBinTree*);
   TBinTree* GetR() const;
   TBinTree* GetL() const;
   const T& GetElem()    const;
   void Add(TBinTree*&, const T&);
   void Add2(TBinTree*&, const T&, const T&);
   void Mul2(TBinTree*&, const T&, const T&);
   void CopyLVR(TBinTree*&, const TBinTree* const&);
   void CopyVLR(TBinTree*&, const TBinTree* const&);
   void CopyLRV(TBinTree*&, const TBinTree* const&);
   void AddLRV(TBinTree*&, const TBinTree* const&, const TBinTree* const&);
   void MulLRV(TBinTree*&, const TBinTree* const&, const TBinTree* const&);
 private:
   TBinTree* l;			//wskaznik na lewy
   TBinTree* r;			//i prawy
   T elem;			//wartosc typu T
};				//wazne: jest tylko jedna instancja klasy BinTree
#endif				//zawierajaca wiele elementow klasy TBinTree
