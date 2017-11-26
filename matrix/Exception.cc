#include "Exception.hh"
#include <iostream>

using namespace std;

Exception :: Exception()
{
#ifdef DEBUG
 cout<<"Exception constructor"<<endl;
#endif
 msg = NULL;
 cout<<"Exception occured: ";
}

void Exception :: ConstructMsg(char* m)
{
#ifdef DEBUG
 cout<<"exception::ConstructMsg()"<<endl;
#endif
 if (!m)
   {
    cout<<"Fatal error, memory exception while processing general exception!"<<endl;
    return;
   }
 msg = new char[strlen(m)+1];
 if (!msg)
   {
    cout<<"Fatal error, memory exception while processing general exception!"<<endl;
    return;
   }
 strcpy(msg, m);
}

Exception :: ~Exception()
{
#ifdef DEBUG
 cout<<"Exception destructor"<<endl;
#endif
 if (msg) delete msg;
 msg = NULL;
}

void Exception :: Print() const
{
#ifdef DEBUG
 cout<<"Exception Print()"<<endl;
#endif
 cout<<"General exception";
 if (msg) cout<<": "<<msg;
 cout<<endl;
}

