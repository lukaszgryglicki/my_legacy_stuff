
#ifndef __M_T_MEEXC__
#define __M_T_MEEXC__
#include "Exception.hh"
class MemException : public Exception
{		//read MathException.hh comments
 public:	//the same but "Mem" is used as exception type
	 MemException() : Exception() { } 
	 MemException(char* m) : Exception(m) { } 
	 MemException(char* m, char* f, int l);
	 ~MemException();
	 void Print() const;
};
#endif
