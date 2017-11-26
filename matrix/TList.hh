#ifndef __M_T_TLIST__
#define __M_T_TLIST__
class TList			//one list element (not entire list)
{				//entire list is "List" class
 public:			//which contains "TList" elements
   TList();			//elem = 0, neighboards NULLs
   TList(int);			//same but elem = some int
   ~TList();
   void SetElem(int);
   void SetNext(TList*);	//no comment
   void SetPrev(TList*);
   TList* GetNext() const;
   TList* GetPrev() const;
   int GetElem()    const;
 private:
   TList* next;			//pointer to next element
   TList* prev;			//and to previous
   int elem;			//elem value
};				//not that there is only ONE instance
#endif				//of class "List" which contains many
				//instances of class "TList"
