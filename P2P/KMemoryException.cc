#include "Common.hh"
#include "KMemoryException.hh"

//destructor of KException handles all, so nothing should be done there
KMemoryException :: ~KMemoryException() {}

//print msg inherited from KException
//but with other description
void KMemoryException :: PrintType() const
{
 printf("Memory exception");
 if (msg) printf(": %s\n", msg);
 else printf("\n");
}

