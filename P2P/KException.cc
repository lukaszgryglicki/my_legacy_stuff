#include "Common.hh"
#include "KException.hh"

//constructs exception with default description
KException :: KException()
{
 msg = NULL;
 printf("Exception occured: ");
}

//construct exception with 'm' description
KException :: KException(char* m)
{
 assert(m);
 msg = new char[strlen(m)+1];
 assert(msg);
 strcpy(msg, m);
 printf("Exception occured: ");
}

//deletes exception
KException :: ~KException()
{
 if (msg) delete msg;
 msg = NULL;
}

//prints msg of type 'General Exception'
void KException :: PrintType() const
{
 printf("General exception");
 if (msg) printf(": %s\n", msg);
 else printf("\n");
}

