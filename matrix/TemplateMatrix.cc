#include "Exceptions.hh"
#include "TemplateMatrix.hh"
template <class T>
Matrix<T> :: Matrix(int m, int n)	//Matrix M x N
{
#ifdef DEBUG
 cout<<"template constructor Matrix<T>("<<m<<", "<<n<<")"<<endl;
#endif
 this->m = m;
 this->n = n;
 try
  {
   a = new T*[m];
   if (!a) throw new MemException("cannot allocate matrix: a", HERE);
   for (int i=0;i<m;i++) 
     {
      a[i] = new T[n];
      if (!a[i]) throw new MemException("cannot allocate matrix: a[i]", HERE);
     }
  }
 catch (MemException* mex)
  {
   mex->Print();
   exit(1);
  }
}

template <class T>
Matrix<T> :: Matrix(int x)	//SQUARE matrix
{
#ifdef DEBUG
 cout<<"template constructor Matrix<T>("<<x<<")"<<endl;
#endif
 this->m = x;
 this->n = x;
 try
  {
   a = new T*[m];
   if (!a) throw new MemException("cannot allocate matrix: a", HERE);
   for (int i=0;i<m;i++) 
     {
      a[i] = new T[n];
      if (!a[i]) throw new MemException("cannot allocate matrix: a[i]", HERE);
     }
  }
 catch (MemException* mex)
  {
   mex->Print();
   exit(1);
  }
}

template <class T>
Matrix<T> :: Matrix()	//Matrix 4x4, as in OpenGL world transformations
{
#ifdef DEBUG
 cout<<"template constructor Matrix<T>()"<<endl;
#endif
 this->m = 4;
 this->n = 4;
 try
  {
   a = new T*[m];
   if (!a) throw new MemException("cannot allocate matrix: a", HERE);
   for (int i=0;i<m;i++) 
     {
      a[i] = new T[n];
      if (!a[i]) throw new MemException("cannot allocate matrix: a[i]", HERE);
     }
  }
 catch (MemException* mex)
  {
   mex->Print();
   exit(1);
  }
}

template <class T>
Matrix<T> :: Matrix(const Matrix<T>& M)	//copy constructor
{					//read comments in TemplateMatrix.hh
#ifdef DEBUG
 cout<<"copy template constructor Matrix<T>(Matrix<T>&)"<<endl;
#endif
 this->m = M.m;
 this->n = M.n;
 try
  {
   a = new T*[m];
   if (!a) throw new MemException("cannot allocate matrix: a", HERE);
   for (int i=0;i<m;i++) 
     {
      a[i] = new T[n];
      if (!a[i]) throw new MemException("cannot allocate matrix: a[i]", HERE);
     }
  }
 catch (MemException* mex)
  {
   mex->Print();
   exit(1);
  }
 for (int i=0;i<m;i++)
 for (int j=0;j<n;j++) a[i][j] = M.a[i][j];	//this uses operator= of type T
}

template <class T>
void Matrix<T> :: operator=(const Matrix<T>& ref)
{						//must check sizes, delete 
#ifdef DEBUG					//if exists and finally DEEP copy
 cout<<"template operator=(Matrix<T>&)"<<endl;	//comments: TemplateMatrix.hh
#endif
 try 
  {
   if (this->m != ref.m || this->n != ref.n)	//sizes, can throw MathExc
     {
      cout<<"Cannot copy matrices. Different MxN parameters!"<<endl;
      cout<<"<"<<m<<","<<n<<"> + <"<<ref.m<<","<<ref.n<<">"<<endl;
      throw new MathException("cannot copy matrices", HERE);
     }
  }
 catch (MathException* mex)
  {
   mex->Print();
   exit(1);
  }
 if (m && n)		//delete existing
   {
    for (int i=0;i<m;i++) delete [] a[i];
    delete [] a;
   }
 try			//create new
  {
   a = new T*[m];
   if (!a) throw new MemException("cannot allocate matrix: a", HERE);
   for (int i=0;i<m;i++) 
     {
      a[i] = new T[n];
      if (!a[i]) throw new MemException("cannot allocate matrix: a[i]", HERE);
     }
  }
 catch (MemException* mex)
  {
   mex->Print();
   exit(1);
  }
 for (int i=0;i<m;i++)
 for (int j=0;j<n;j++) a[i][j] = ref.a[i][j];	//copy values 
 						//using operator= of T
}						//MUST be defined for type T

template <class T>
Matrix<T>& Matrix<T> :: operator+(const Matrix<T>& ref)
{						//Adds matrix, using
#ifdef DEBUG					//T's operator+
 cout<<"template operator+(Matrix<T>&)"<<endl;
#endif
 try						//sizes check
   {
    if (this->m != ref.m || this->n != ref.n)
      {
       cout<<"Cannot add matrices. Different MxN parameters!"<<endl;
       cout<<"<"<<m<<","<<n<<"> + <"<<ref.m<<","<<ref.n<<">"<<endl;
       throw new MathException("cannot add matrices", HERE);
      }
   }
 catch (MathException* mex)
  {
   mex->Print();
   exit(1);
  }
 Matrix<T>* res = new Matrix<T>(m, n);	//Result construct
 for (int i=0;i<m;i++)
 for (int j=0;j<n;j++) 
   {
    res->a[i][j] = a[i][j];		//uses operator= of T
    res->a[i][j] += ref.a[i][j];	//uses operator+ of T
   }					//READ how operator+ works
 return *res;				//for T=List: comments in 
}					//TemplateMatrix.hh

template <class T>
Matrix<T>& Matrix<T> :: operator*(const Matrix<T>& ref)
{					//multiply matrices
#ifdef DEBUG
 cout<<"template operator*(Matrix<T>&)"<<endl;
#endif
 try 
   {
    if (this->n != ref.m)		//sizes
      {					//C = A*B
	  				//then A(M,N) and B(N,K)
					//so C will  be C(M,K)
       cout<<"Cannot multiply matrices. Different MxN parameters!"<<endl;
       cout<<"<"<<m<<","<<n<<"> * <"<<ref.m<<","<<ref.n<<">"<<endl;
       throw new MathException("cannot multiply matrices", HERE);
      }
   }
 catch (MathException* mex)
  {
   mex->Print();
   exit(1);
  }
 int M,N,K;		//sizes evaluations
 M = this->m;
 N = ref.n;
 K = this->n;
 Matrix<T>* res = new Matrix<T>(M, N);	//target creating
 for (int i=0;i<M;i++)			//N^3 loop
 for (int j=0;j<N;j++)
 for (int k=0;k<K;k++) res->a[i][j] += this->a[i][k]*ref.a[k][j];
 			//this one line uses 2 operators:
			//operator+= on T, and operator* on T
			//this can be done not introducing operator+=
			//instead of a += b, we can write a = a + b
			//read how operator* works on T=List
			//comments in: List.hh and TemplateMatrix.hh
 return *res;
}

template <class T>
Matrix<T> :: ~Matrix()
{			//destructor frees arrays (2-D)
#ifdef DEBUG		//and uses T's destructor to delete elements
 cout<<"template destructor ~Matrix<T>()"<<endl;
#endif
 for (int i=0;i<m;i++) delete [] a[i];
 this->m = 0;
 this->n = 0;
 delete [] a;
 a = NULL;
}

template <class T>	//gets A[i][j] element
			//or throws MathExc if indexes OOR
T& Matrix<T> :: GetElem(int i, int j) const
{
#ifdef DEBUG
 cout<<"Matrix<T>::GetElem("<<i<<", "<<j<<")"<<endl;
#endif
 try
   {
    if (i>=0 && i<m && j>=0 && j<n) return a[i][j];
    else throw new MathException("index out of range!", HERE);
   }
 catch (MathException* mex)
   {
    mex->Print();
    exit(1);
   }
 return a[0][0];
}

template <class T>
int Matrix<T> :: GetM() const
{
#ifdef DEBUG
 cout<<"Matrix<T>::GetM()"<<endl;
#endif
 return this->m;
}

template <class T>
int Matrix<T> :: GetN() const
{
#ifdef DEBUG
 cout<<"Matrix<T>::GetN()"<<endl;
#endif
 return this->n;
}

template <class T>	//sets A[i][j] elem or throws MathExc if OOR
void Matrix<T> :: SetElem(int i, int j, const T& elem)
{
#ifdef DEBUG
 cout<<"Matrix<T>::SetElem("<<i<<", "<<j<<", "<<elem<<")"<<endl;
#endif
 try
   {
    if (i>=0 && i<m && j>=0 && j<n) a[i][j] = elem;
    else throw new MathException("index out of range!", HERE);
   }
 catch (MathException* mex)
   {
    mex->Print();
    exit(1);
   }
}

template <class T>
ostream& operator<<(ostream& out, const Matrix<T>& ref)
{				//prints entire matrix using
#ifdef DEBUG			//T's operator<<
    				//(which MUST be defined)
 cout<<"operator<< Matrix<T>(ostream&, Matrix<T>&)"<<endl;
#endif
 for (int i=0;i<ref.GetM();i++)
   {
    for (int j=0;j<ref.GetN();j++) cout<<ref.GetElem(i,j)<<"| ";
    cout<<endl;
   }
 return out;
}

/*template <class T>
T* Matrix<T>:: operator[](int idx)
{
 if (idx>0 && idx<m) return a[idx];
 else return NULL;
}*/
