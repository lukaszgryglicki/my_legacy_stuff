
#ifndef __HAVE_KIOEXC_H__
#define __HAVE_KIOEXC_H__
#include "KException.hh"
//inherits from KException its msg uses its constructor
//thrown on open/write/read faileres
class KIOException : public KException
{
 public:
	 KIOException() : KException() { } 
	 KIOException(char* m) : KException(m) { } 
	 ~KIOException();
	 void PrintType() const;
};
#endif
