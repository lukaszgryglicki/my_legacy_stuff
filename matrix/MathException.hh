#ifndef __M_T_MEXC__
#define __M_T_MEXC__
#include "Exception.hh"
class MathException : public Exception
{			//extends exception, adds type "Math" as a
 public:		//addictional information, uses mostly base methods
	 MathException() : Exception() { } 
	 MathException(char* m) : Exception(m) { } 
	 MathException(char* m, char* f, int l);
	 		//prints message with filename and line
	 ~MathException();
	 void Print() const;	//prints prepared by thow statement
};
#endif
