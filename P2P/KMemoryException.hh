
#ifndef __HAVE_KMEMEXC_H__
#define __HAVE_KMEMEXC_H__
#include "KException.hh"
//inherits from KException
//its msg and uses its constructor
class KMemoryException : public KException
{
 public:
	 KMemoryException() : KException() { } 
	 KMemoryException(char* m) : KException(m) { } 
	 ~KMemoryException();
	 void PrintType() const;
};
#endif
