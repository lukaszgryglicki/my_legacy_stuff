#include "Exceptions.hh"
#include "TList.hh"
#include "List.hh"
#include <stdlib.h>


List :: List()
{
#ifdef DEBUG
 cout<<"constructor List()"<<endl;
#endif
 tail = head = NULL;
 nelems = 0;
}

List :: List(int val)
{
#ifdef DEBUG
 cout<<"constructor List("<<val<<")"<<endl;
#endif
 try
   {
    head = new TList;
    if (!head) throw new MemException("cannot allocate head", HERE);
   }
 catch (MemException* mex)
   {
    mex->Print();
    exit(1);
   }
 head->SetElem(val);
 head->SetNext(NULL);
 head->SetPrev(NULL);
 tail = head;
 nelems = 1;
}

List :: List(const List& ref)	//copy constructor
{			//there is VERY small probability that somethiong is 
#ifdef DEBUG		//on the list (must be memorry-error)
    			//but lets check that
 cout<<"copy constructor List(const List&)"<<endl;
#endif		
 if (head)			//delete if exists
 {
  TList* tmp = head->GetNext();	//get next to head
  if (!tmp)			//if no then only head exists
    {
     delete head;
     head = tail = NULL;	//so kill and zero it
     nelems = 0;
    }
  else				//more then one element
   {
    while (tmp)
      {
       delete tmp->GetPrev();	//delete behind U
       tmp = tmp->GetNext();
      }
    delete tail;		//doing that we must manually kill tail
    head = tail = NULL;
    nelems = 0;
   }
 }				//ok if there was a list, now is dead
 TList* tmp2=ref.head;		//so copy new to this location
 this->nelems = ref.nelems;
 while (tmp2)
   {
    AddOnTail(tmp2->GetElem());	//get from head, put on tail
    tmp2 = tmp2->GetNext();
   }
}


void List::operator=(const List& ref)
{
#ifdef DEBUG
 cout<<"operator=List(const List&)"<<endl;
#endif
 if (nelems != 0)	//also if there was something
   {			//read comments above
    TList* tmp = head->GetNext();
    if (!tmp)
       {
        delete head;
        head = tail = NULL;
        nelems = 0;
        return;
       }
    while (tmp)
       {
        delete tmp->GetPrev();
        tmp = tmp->GetNext();
       }
    delete tail;
    head = tail = NULL;
    nelems = 0;
   }
 TList* tmp=ref.head;
 this->nelems = ref.nelems;
 while (tmp)
 {
  AddOnTail(tmp->GetElem());
  tmp = tmp->GetNext();
 }
}

List :: ~List()		//free list, described in copy constructor
{
#ifdef DEBUG
 cout<<"destructor ~List()"<<endl;
#endif
 if (!head) return;
 TList* tmp = head->GetNext();
 if (!tmp)
   {
    delete head;
    head = tail = NULL;
    nelems = 0;
    return;
   }
 while (tmp)
   {
    delete tmp->GetPrev();
    tmp = tmp->GetNext();
   }
 delete tail;
 head = tail = NULL;
 nelems = 0;
}

void List :: AddOnHead(int elem)
{
#ifdef DEBUG
 cout<<"List::AddOnHead("<<elem<<")"<<endl;
#endif
 TList* tmp;
 try 
   {
    tmp = new TList(elem);
    if (!tmp) throw new MemException("cannot allocate tmp", HERE);
   }
 catch (MemException* mex)
   {
    mex->Print();
    exit(1); 
   }
 if (!head) 
   {
    head = tail = tmp;
    nelems = 1;
    return;
   }
 tmp->SetNext(head);
 head->SetPrev(tmp);
 head = tmp;
 nelems++;
}

void List :: AddOnTail(int elem)
{
#ifdef DEBUG
 cout<<"List::AddOnTail("<<elem<<")"<<endl;
#endif
 TList* tmp;
 try 
   {
    tmp = new TList(elem);
    if (!tmp) throw new MemException("cannot allocate tmp", HERE);
   }
 catch (MemException* mex)
   {
    mex->Print();
    exit(1); 
   }
 if (!tail) 
   {
    head = tail = tmp;
    nelems = 1;
    return;
   }
 tmp->SetPrev(tail);
 tail->SetNext(tmp);
 tail = tmp;
 nelems++;
}

		//use out given and ffinally return it
ostream& operator<<(ostream& out, const List& ref)
{
#ifdef DEBUG
 cout<<"operator<< List(ostream&, List&)"<<endl;
#endif
 if (ref.nelems==0)
   {
    out<<"list is empty ";
    return out;
   }
 TList* tmp = ref.head;
 while (tmp)
   {
    out<<tmp->GetElem()<<",";
    tmp = tmp->GetNext();
   }
 return out;
}

void List::operator+=(const List& ref)	//adds elements to the list's tail
{
#ifdef DEBUG
 cout<<"operator+=List(const List&)"<<endl;
#endif
 TList* tmp=ref.head;
 this->nelems += ref.nelems;		//sum of numbers
 while (tmp)	
 {
  AddOnTail(tmp->GetElem());
  tmp = tmp->GetNext();
 }
}

		//multiplies lists, description: see List.hh
List& List :: operator*(const List& ref)
{
#ifdef DEBUG
 cout<<"operator* List(const List&)"<<endl;
#endif
 List* ptr;
 try 
   {
    ptr = new List();
    if (!ptr) throw new MemException("cannot allocate ptr", HERE);
   }
 catch (MemException* mex)
   {
    mex->Print();
    exit(1); 
   }
 TList* tmp1 = head;
 TList* tmp2 = ref.head;
 while (tmp1 && tmp2)
   {
    ptr->AddOnHead(tmp1->GetElem()*tmp2->GetElem());
    tmp1 = tmp1->GetNext();
    tmp2 = tmp2->GetNext();
   }
 return *ptr;
}

