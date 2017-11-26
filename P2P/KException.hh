#ifndef __HAVE_KEXC_H__
#define __HAVE_KEXC_H__
class KException	//general exception, base for KMemoryException and KIOException
{
 public:
	 KException();
	 KException(char*);
	 ~KException();
	 void PrintType() const;
 protected:
	 char* msg;	//Exception message
};
#endif
