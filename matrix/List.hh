#ifndef __M_T_LIST__
#define __M_T_LIST__
#include <iostream>
using namespace std;	//to use cout instead of std::cout
class List		//used as a list abstraction
{			//stores int elements, head, tail
 public:		//manages adding/removing elements
   List(const List&);	//copy constructor (copies entire list
   			//element after element to new memory location)
   List(int);		//one element list
   List();		//empty list
   ~List();		//destroys entire list elem-by-elem
   void AddOnHead(int);
   void AddOnTail(int);
   void operator=(const List&);	 //copies list like copyy constructor
   void operator+=(const List&); //adds list to current's list tail
   List& operator*(const List&); //multiplies lists
   				 //if list1 has N elements, and list2 has M
				 //elements then result has K=min(N,M) elems
				 //each is defined as for i=0 to K
				 //elem[i] = elem_list1[i * elem_list2[i]
   friend ostream& operator<<(ostream&, const List&);
   				//prints list just like: 3,2,3,
				//friend is used to have access to class
				//members and first argument MUST be ostream
 private:
   TList* head;			//head and tail encloses the same list
   TList* tail;
   int nelems;			//counter
};

#endif
