//user defined classes shoul have operators:
//initializer ClassName() -constructor
//friend operator ostream& <<(ostream&,const ClassName&)
#ifndef __M_STACK_t
#define __M_STACK_t
#include <iostream.h>

template <class T>
class TListStack
{
 private:
 T elem;
 TListStack* next;
 TListStack* prev;
 public:
 TListStack(const T& ref) : elem(ref), next(0), prev(0) {}
 TListStack() { elem=0; next=0;  prev=0; }
 ~TListStack();
 TListStack* AddOnBegin(const T&);
 TListStack* DelFromBegin(T&);
 TListStack* AddOnEnd(const T&);
 TListStack* DelFromEnd(T&);
 void print() ;
 void printe() ;
 void free_memory_up();	  //zwalnia od ptr w gore
 void free_memory_down(); //zwalnia od ptr w dol
 T& retelem(void) { return elem; }
 void AddSorted(const T&, TListStack<T>*&, TListStack<T>*&);
 void DelMin(T&, TListStack<T>*&, TListStack<T>*&);
 void DelMax(T&, TListStack<T>*&, TListStack<T>*&);
 void FindMin(T&, TListStack<T>*, TListStack<T>*);
 void FindMax(T&, TListStack<T>*, TListStack<T>*);
 int SearchAbout(T&, const T&);
 int SearchEx(T&, const T&);
 int SearchDelAbout(T&, const T&, TListStack<T>*&, TListStack<T>*&);
 int SearchDelEx(T&, const T&, TListStack<T>*&, TListStack<T>*&);
};


template <class T>
void TListStack<T>::AddSorted(const T& ref, TListStack<T>*& h, TListStack<T>*& t)
{
 if (!h || !t) return;
 TListStack* tmp = h;
 while (tmp->next && ref<tmp->elem) tmp = tmp->next;
 TListStack* sort = new TListStack(ref);
 if (tmp==h && h==t)
   {
    if (ref>tmp->elem) {sort->next=h;sort->prev=0;h->prev=sort;h=sort;return;}
    else                {sort->prev=h;sort->next=0;t->next=sort;t=sort;return;}
   }
 if (tmp==h)
  {
   sort->next = h;
   sort->prev = NULL;
   h->prev = sort;
   h = sort;
   return ;
  }
 else if (tmp==t)
  {
   if (sort->elem<t->elem)
     {
      sort->prev = t;
      sort->next = NULL;
      t->next = sort;
      t = sort;
      return ;
     }
   else
     {
      sort->next = t;
      sort->prev = t->prev;
      t->prev->next = sort;
      t->prev = sort;
     }
  }
 else
  {
   sort->next = tmp;
   sort->prev = tmp->prev;
   tmp->prev->next = sort;
   tmp->prev = sort;
  }
}

template <class T>
int TListStack<T>::SearchAbout(T& ref, const T& what)
{
 if (!next && !(elem>=what)) { ref=elem; return 2; }
 TListStack* tmp = this;
 while (tmp)    //dziala przy przeciazonym operatorze >=
    {		//ktory znajduje przyblizenie elementu
     if (tmp->elem >= what) { ref=tmp->elem; return 0; }
     tmp=tmp->next;
    }
 ref=this->elem;
 return 1;
}


template <class T>
int TListStack<T>::SearchEx(T& ref, const T& what)
{
 if (!next && !(elem==what)) { ref=elem; return 2; }
 TListStack* tmp = this;
 while (tmp)    //dziala przy przeciazonym operatorze >=
    {		//ktory znajduje przyblizenie elementu
     if (tmp->elem == what) { ref=tmp->elem; return 0; }
     tmp=tmp->next;
    }
 ref=this->elem;
 return 1;
}


template <class T>
int TListStack<T>::SearchDelAbout(T& ref, const T& what, TListStack<T>*& h, TListStack<T>*& t)
{
 if (!h || !t) return 2;
 TListStack* tmp = h;
 ref = tmp->elem;
 while (tmp)
    {
     if (tmp->elem >= what) 
        { 
	 ref=tmp->elem; 
 	 if (tmp==t && t==h) { h=0; t=0; delete this; return 0; }
 	 if (tmp==t) t = tmp->prev;
	 if (tmp==h) h = tmp->next;
	 if (tmp->prev) tmp->prev->next = tmp->next;
	 if (tmp->next) tmp->next->prev = tmp->prev;
	 tmp->next=0;
	 tmp->prev=0;
 	 delete tmp;
	 return 0;
        }
     tmp=tmp->next;
    }
 ref=this->elem;
 return 1;
}

template <class T>
int TListStack<T>::SearchDelEx(T& ref, const T& what, TListStack<T>*& h, TListStack<T>*& t)
{
 if (!h || !t) return 2;
 TListStack* tmp = h;
 ref = tmp->elem;
 while (tmp)
    {
     if (tmp->elem == what) 
        { 
	 ref=tmp->elem; 
 	 if (tmp==t && t==h) { h=0; t=0; delete this; return 0; }
 	 if (tmp==t) t = tmp->prev;
	 if (tmp==h) h = tmp->next;
	 if (tmp->prev) tmp->prev->next = tmp->next;
	 if (tmp->next) tmp->next->prev = tmp->prev;
	 tmp->next=0;
	 tmp->prev=0;
 	 delete tmp;
	 return 0;
        }
     tmp=tmp->next;
    }
 ref=this->elem;
 return 1;
}

template <class T>
void TListStack<T>::FindMin(T& ref, TListStack<T>* h, TListStack<T>* t)
{
 if (!h || !t) return;
 TListStack* tmp = t;
 TListStack* min = tmp;
 ref = min->elem;
 while (tmp->prev)
    {
     if (tmp->elem < min->elem) { ref = tmp->elem; min = tmp; }
     tmp=tmp->prev;
    }
 if (tmp->elem < min->elem) { ref = tmp->elem; min = tmp; }   
}


template <class T>
void TListStack<T>::FindMax(T& ref, TListStack<T>* h, TListStack<T>* t)
{
 if (!h || !t) return;
 TListStack* tmp = h;
 TListStack* max = tmp;
 ref = max->elem;
 while (tmp->next)
    {
     if (tmp->elem > max->elem) { ref=tmp->elem; max=tmp; }
     tmp=tmp->next;
    }
 if (tmp->elem > max->elem) { ref=tmp->elem; max=tmp; }
}

template <class T>
void TListStack<T>::DelMin(T& ref, TListStack<T>*& h, TListStack<T>*& t)
{
 if (!h || !t) return;
 TListStack* tmp = t;
 TListStack* min = tmp;
 ref = min->elem;
// cout<<"tmp="<<(void*)tmp<<endl;
 while (tmp->prev)
    {
     if (tmp->elem < min->elem) { ref = tmp->elem; min = tmp; }
     tmp=tmp->prev;
    }
 if (tmp->elem < min->elem) { ref = tmp->elem; min = tmp; }   
 if (min==h && h==t) { h=0; t=0; delete this; return ; }
 if (min==h) h = min->next;
 if (min==t) t = min->prev;
 if (min->prev) min->prev->next = min->next;
 if (min->next) min->next->prev = min->prev;
 min->next=0;
 min->prev=0;
 delete min;
}


template <class T>
void TListStack<T>::DelMax(T& ref, TListStack<T>*& h, TListStack<T>*& t)
{
 if (!h || !t) return;
 TListStack* tmp = h;
 TListStack* max = tmp;
 ref = max->elem;
// cout<<"tmp="<<(void*)tmp<<endl;
 while (tmp->next)
    {
     if (tmp->elem > max->elem) { ref=tmp->elem; max=tmp; }
     tmp=tmp->next;
    }
 if (tmp->elem > max->elem) { ref=tmp->elem; max=tmp; }
 if (max==t && t==h) { h=0; t=0; delete this; return ; }
 if (max==t) t = max->prev;
 if (max==h) h = max->next;
 if (max->prev) max->prev->next = max->next;
 if (max->next) max->next->prev = max->prev;
 max->next=0;
 max->prev=0;
 delete max;

}

template <class T>
void TListStack<T>::print()
{
 TListStack* tmp = this;
 while (tmp)
   {
    cout<<tmp->elem;
    //cout<<"THIS= "<<(void*)tmp<<", ELEM="<<tmp->elem<<", NEXT="<<(void*)tmp->next<<", PREV="<<(void*)tmp->prev<<endl;
    tmp = tmp->next;
   }
 cout<<endl;
}


template <class T>
void TListStack<T>::printe()
{
 TListStack* tmp = this;
 while (tmp)
   {
    cout<<tmp->elem;
    tmp = tmp->prev;
   }
 cout<<endl;
}


template <class T>
TListStack<T>::~TListStack()
{
 //zwalniania dokonuje funkcja free_memory();
}

template <class T>
TListStack<T>* TListStack<T>::AddOnEnd(const T& ref)
{
 TListStack* tmp = new TListStack(ref);
 tmp->prev = this;
 tmp->next = 0;
 this->next= tmp;
 return tmp;
}

template <class T>
TListStack<T>* TListStack<T>::DelFromEnd(T& ref)
{
 TListStack* tmp = this->prev;
 ref = this->elem;
 if (tmp) tmp->next=0;
 delete this;
 return tmp;
}


template <class T>
TListStack<T>* TListStack<T>::AddOnBegin(const T& ref)
{
 TListStack* tmp = new TListStack(ref);
 tmp->next = this;
 tmp->prev = 0;
 this->prev= tmp;
 return tmp;
}

template <class T>
TListStack<T>* TListStack<T>::DelFromBegin(T& ref)
{
 TListStack* tmp = this->next;
 ref = this->elem;
 if (tmp) tmp->prev = 0;
 delete this;
 return tmp;
}


template <class T>
void TListStack<T>::free_memory_up()
{
 if (!next) return; 
 TListStack<T>* head=next;
 while (head->next)
   {
    delete head->prev;
    head = head->next;
   }
}


template <class T>
void TListStack<T>::free_memory_down()
{
 if (!prev) return;
 TListStack<T>* head=prev;
 while (head->prev)
   {

    delete head->next;
    head = head->prev;
   }
}

template <class T>
class TFifoP
{
 private:
 T e;
 T et;
 int esp;
 TListStack<T>* list;
 TListStack<T>* tail;
 public:
 TFifoP() : esp(0), list(0), tail(0) {}   //ok
 ~TFifoP() { if (tail) tail->free_memory_down(); } //mozna tez list->free_memory_up() ok
 int emp() const { return !list; }    //ok
 void put(const T&);         //poloz na pocz ok
 void pute(const T&);        //poloz na koniec ok
 void putsort(const T&);     //wstaw posortowany ok
 void get(T&);               //wez z konca FIFO
 T& get(void);               //wez z konca
 void getb(T&);		     //wez z pocz
 T& getb(void);		     //wez z pocz
 void getmax(T&);	     //wez max
 T& getmax(void);	     //wez max
 void getmin(T&);	     //wez min [ok]
 T& getmin(void);	     //wez min [ok]
 void max(T&);	     //wez max
 T& max(void);	     //wez max
 void min(T&);	     //wez min [ok]
 T& min(void);	     //wez min [ok]
 int count() const { return esp; }//ok
 void print() ;                   //ok
 void printe() ;                  //ok
 void top(T& ref)   { ref=e;    } //ok
 T& top(void)       { return e; } //ok
 void floor(T& ref) { ref=et;   } //ok
 T& floor(void)     { return et;} //ok
 int searcha(T&, const T&);
 int search(T&, const T&);
 int searchdel(T&, const T&);
 int searchadel(T&, const T&);
};


template <class T>
int TFifoP<T>::searcha(T& ref, const T& what) //dziala
{
 if (emp()) { cout<<"empty\n"; return 4; }   //znaczy blad
 return list->SearchAbout(ref, what);
}


template <class T>
int TFifoP<T>::search(T& ref, const T& what) 
{
 if (emp()) { cout<<"empty\n"; return 4; }
 return list->SearchEx(ref, what);
}

template <class T>
int TFifoP<T>::searchdel(T& ref, const T& what) 
{
 if (emp()) { cout<<"empty\n"; return 4; }
 int rcode = list->SearchDelEx(ref, what, list, tail);
 if (emp()) return rcode;
 if (!rcode) 
   {
    e = list->retelem();
    et= tail->retelem();
    esp--;
   }
 return rcode;
}


template <class T>
int TFifoP<T>::searchadel(T& ref, const T& what) 
{
 if (emp()) { cout<<"empty\n"; return 4; }
 int rcode = list->SearchDelAbout(ref, what, list, tail);
 if (emp()) return rcode;
 if (!rcode) 
   {
    e = list->retelem();
    et= tail->retelem();
    esp--;
   }
 return rcode;
}
template <class T>
void TFifoP<T>::put(const T& ref) //dziala
{
 e = ref;
 if (emp()) { tail = list = new TListStack<T>(ref); et=ref; esp++; return ;}
 list = list->AddOnBegin(ref);
 esp++;
}


template <class T>
void TFifoP<T>::pute(const T& ref) //dziala
{
 et = ref;
 if (emp()) { tail = list = new TListStack<T>(ref); e=ref; esp++; return ;}
 tail = tail->AddOnEnd(ref);
 esp++;
}


template <class T>
void TFifoP<T>::putsort(const T& ref)//ok
{
 if (emp()) { tail = list = new TListStack<T>(ref); e=ref; et=ref; esp++; return ;}
 list->AddSorted(ref,list,tail);
 e  = list->retelem();
 et = tail->retelem();
 esp++;
}

template <class T>
void TFifoP<T>::get(T& ref)
{
 if (emp()) { cout<<"empty\n"; return; }
 if (list==tail)  list=list->DelFromBegin(ref);
 else tail = tail->DelFromEnd(ref);
 et = ref;
 esp--;
}

template <class T>
T& TFifoP<T>::get()
{
 if (emp()) { cout<<"empty\n"; return et; }
 if (list==tail)  list=list->DelFromBegin(et);
 else tail = tail->DelFromEnd(et);
 esp--;
 return et;
}


template <class T>
void TFifoP<T>::getb(T& ref)
{
 if (emp()) { cout<<"empty\n"; return; }
 list = list->DelFromBegin(ref);
 e = ref;
 esp--;
}

template <class T>
T& TFifoP<T>::getb()
{
 if (emp()) { cout<<"empty\n"; return e; }
 list = list->DelFromBegin(e);
 esp--;
 return e;
}


template <class T>
void TFifoP<T>::getmax(T& ref)
{
 if (emp()) { cout<<"empty\n"; return; }
 list->DelMax(ref, list, tail);
 if (emp()) return ;
 e = list->retelem();
 et= tail->retelem();
 esp--;
}

template <class T>
T& TFifoP<T>::getmax()
{
 if (emp()) { cout<<"empty\n"; return et; }
 T* ref = new T;
 list->DelMax(*ref, list, tail);
 if (emp()) return *ref;
 e = list->retelem();
 et= list->retelem();
 esp--;
 return *ref;
}


template <class T>
void TFifoP<T>::getmin(T& ref)
{
 if (emp()) { cout<<"empty\n"; return; }
 tail->DelMin(ref, list, tail);
 if (emp()) return ;
 e = list->retelem();
 et= tail->retelem();
 esp--;
}

template <class T>
T& TFifoP<T>::getmin()
{
 if (emp()) { cout<<"empty\n"; return e; }
 T* ref = new T;
 tail->DelMin(*ref, list, tail);
 //cout<<"min="<<ref<<endl;
 if (emp()) return *ref ;
 e = list->retelem();
 et= tail->retelem();
 esp--;
 return *ref;
}

template <class T>
void TFifoP<T>::print()
{
 if (emp()) { cout<<"empty\n"; return; }
 list->print();
}


template <class T>
void TFifoP<T>::printe()
{
 if (emp()) { cout<<"empty\n"; return; }
 tail->printe();
}



template <class T>
void TFifoP<T>::max(T& ref)
{
 if (emp()) { cout<<"empty\n"; return; }
 list->FindMax(ref, list, tail);
}

template <class T>
T& TFifoP<T>::max()
{
 if (emp()) { cout<<"empty\n"; return et; }
 T* ref = new T;
 list->FindMax(*ref, list, tail);
 return *ref;
}


template <class T>
void TFifoP<T>::min(T& ref)
{
 if (emp()) { cout<<"empty\n"; return; }
 tail->FindMin(ref, list, tail);
}

template <class T>
T& TFifoP<T>::min()
{
 if (emp()) { cout<<"empty\n"; return e; }
 T* ref = new T;
 tail->FindMin(*ref, list, tail);
 return *ref;
}

#endif
