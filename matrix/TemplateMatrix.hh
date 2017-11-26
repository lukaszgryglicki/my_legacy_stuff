#ifndef __M_T_TMATRIX__
#define __M_T_TMATRIX__
template <class T>		//parametrized class abstraction
class Matrix			//Matrix(M,N) elements of type T
{				//T can be: int,char,double,... class List
 public:			//and this is main usage of this template
   Matrix();			// M(4,4)
   Matrix(int);			// M(a,a)
   Matrix(int, int);		// M(a,b)
   Matrix(const Matrix<T>&);	//copies entire matrix to another
   				//it uses DEEP copyy so each element is
				//moved to other memory location
				//NOTE: elements are T type, so there must be
				//operator "=" defined  for type used as T
				//List operator= IS defined; see List.cc
				//so List caan be used as T: Matrix<List>
   ~Matrix();			//destroys matrix and elements using
   				//their destructor: ex: ~List
   T& GetElem(int, int) const;	//Get M[i][j]
   int GetM() const;		
   int GetN() const;
   void SetElem(int, int, const T&);	//Set M[i][j] uses operator= on T
   void operator=(const Matrix<T>&);	
   					//copies entire matrix if N and M
					//are equal, works like copy 
					//constructor, DEEP copy, 
					//operator= on T is used
   Matrix<T>& operator+(const Matrix<T>&);
   				//uses operator+ on T to add each elements
				//of matrix, in List operator T works:
				//1,2,3 + 4,3,2 = 1,2,3,4,3,2
   Matrix<T>& operator*(const Matrix<T>&);
   				//uses T defined operators + and *
				//read List.hh comments to see how
				//operator* on T=List works
 private:
   int n,m;			//dimnesions
   T** a;			//abstract 2-D array of T-elements
};
#endif
