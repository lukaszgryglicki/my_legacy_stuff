#include "Common.hh"
#include "KIOException.hh"

//destructor of KException handles all
KIOException :: ~KIOException() {}

//print msg inherited from KException
//but with other description
void KIOException :: PrintType() const
{
 printf("I/O exception");
 if (msg) printf(": %s\n", msg);
 else printf("\n");
 perror("I/O status");
}

