#include "main.h"
#include "TBinTree.cpp"	
#include "BinTree.cpp"	
			//szablony musza byc dolaczone z implementacja 
			//gdyz kompilator musi wiedziec dla jakich typow
			//wygenerowac kod

int main()		//testuje wszystkie operacje
{
 BinTree<int> tr;
 BinTree<int> tr2;
 BinTree<int> tr3;
 BinTree<int> tr4;	//konstruktor
 tr.Add(4);
 tr.Add(3);
 tr.Add(2);		//dodawanie
 tr.Add(5);
 tr.Add(1);
 tr.Add(3);
 
 cout<<tr<<endl;	//wypisywanie

 tr2 = tr;		//operator= (uzywa VLR)
 
 cout<<tr2<<endl;

 tr3 = tr + tr2;	//operator+ i operator*
 tr4 = tr * tr2;	//uzywa LRV (czyli zmienia topologie drzeewa)
 
 cout<<tr3<<endl;
 cout<<tr4<<endl;

 CList list[6][6]; 	//lista konstruktor
 for (int i=0;i<6;i++)
 for (int j=0;j<6;j++) 
 for (int k=0;k<4;k++) list[i][j].Add(i+j+k+10); //dodawanie do listy
 
 list[0][0] = list[5][5];	//operator = na liscie
 
 cout<<list[2][2]<<endl;	//wypisanie listy
 
 BinTree<CList> A;		//drzewo na liscie cyklicznej
 BinTree<CList> B;
 BinTree<CList> C;
 BinTree<CList> D;
 for (int i=0;i<3;i++)
 for (int j=0;j<2;j++)
  {
      A.Add(list[i][j]);	//wstawianie elementow (uzywa operator=(CList)
      B.Add(list[j][i]);
  }
 for (int i=0;i<4;i++)
 for (int j=0;j<4;j++) D.Add(list[i][j]);

 cout<<A<<endl;			//wypisanie drzewa, uzywa wypisania listy wewnetrznie
 cout<<B<<endl;
 cout<<D<<endl;

 C=D;				//operator= na drzewie list

 cout<<C<<endl;
 
 BinTree<CList> E=C;		//konstruktor kopiujacy
 
 cout<<E<<endl;


 BinTree<CList> F = C * D;	//operator* na drzewie, moze zmienic topologie drzewa
 BinTree<CList> G = A * B;	//operator+ na drzewie, -||-
 cout<<F<<endl;
 cout<<G<<endl;

 cout<<C+F<<endl;	//C (LVR) ma inna topologie niz F (LRV)
 			//co wynika z definicji operatorow + i *
			//bo sa one LRV 'rozne topologie drzew'
			//powinno rzucic wyjatkiem
 
 cout<<A*C<<endl;	//program tu nie dojdzie ale AA i C maja rozna ilosc
 			//elementow tez rzuci wyjatkiem jezeli powyzszee zakomentujemy
 
 return 0;
}

