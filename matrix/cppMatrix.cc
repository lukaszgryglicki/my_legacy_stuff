#include "cppMatrix.hh"
#include "TemplateMatrix.cc"

int main()		//Tests all engines
{
#ifdef DEBUG
 cout<<"main"<<endl;	//tests:
#endif
 Matrix<List> A(3,2);	//M(a,b)
 Matrix<List> B(2,4);	//Other matrix M(a,b)
 Matrix<List> C;	//M (M(4,4))
 Matrix<List> D=C;	//copy constructor for Matrix
 List list[6][6];	//List default constructor
 
 for (int i=0;i<6;i++)
 for (int j=0;j<6;j++) 
 for (int k=0;k<4;k++) list[i][j].AddOnHead(i+j+k+10);	
 				//adding elements to list
 list[0][0] = list[5][5];	//List's copy constructor
 
 for (int i=0;i<3;i++)		//filling lists
 for (int j=0;j<2;j++)
  {
   A.SetElem(i,j, list[i][j]);	//Lists's operator=
   B.SetElem(j,i, list[j][i]);	//Setting elements in Matrix
  }
 
 for (int i=0;i<2;i++) B.SetElem(i, 3, list[i+4][i+3]);
 for (int i=0;i<4;i++)
 for (int j=0;j<4;j++) D.SetElem(i,j, list[i+2][j+2]);
 
 C = D; 			//operator= for Matrix
 Matrix<List> E = C + D;	//operator= and operator+ for Matrix
 				//uses operator= and operator+
				//of List internally
 Matrix<List> F = A * B;	//operator * for Matrix
 				//uses operator+= * operator*
				//for List internally
 cout<<list[2][2]<<endl;	//operator<< for List	
 
 cout<<A<<endl;			//operator<< for Matrix
 cout<<B<<endl;
 cout<<C<<endl;
 cout<<D<<endl;
 cout<<E<<endl;
 cout<<F<<endl;
 
 Matrix<int> G;			//Matrix of int
 G.SetElem(0,0,1);
 G.SetElem(1,1,1);		//setElem for ints, int dont uses
 G.SetElem(2,2,1);		//copy constructor, it is copied
 G.SetElem(3,3,1);		//using buildin operator+ for integers
 cout<<G<<endl;			//print out int Matrix

 			//4x4 wit 4-elements list will produce
			//a very big: 4x4 8-elements list matrix
			//which elements are big ~200-->~400
			//so it will thrash the screen
 //cout<<E*E<<endl;	//would print a LOT of stuff
 			//but will work OK, uncomment if U want

 //cout<<C*G<<endl;	//matrices of different types: int and List
 			//there are no proper opeerators defined
			//A and C has bad dimnesions to be mulpiplied
 cout<<A*C<<endl;	//should throw an exception
 
 cout<<A+B<<endl;	//it would throw an exception (Math)
 			//because A and B are not of the same dimnesion
			//but it won't because exception will be thrown
			//above in multiply and program will halt there
 return 0;		//never goes there
}



//that's all folks
