#include "MemException.hh"
#include <iostream>

using namespace std;

MemException :: ~MemException() {}

void MemException :: Print() const
{
#ifdef DEBUG
 cout<<"Memexception::Print()"<<endl;
#endif
 cout<<"Mem exceptionon";
 if (msg) cout<<": "<<msg;
 cout<<endl;
}

MemException :: MemException(char* m, char* f, int l)
{
#ifdef DEBUG
 cout<<"MemException constructor"<<endl;
#endif
 char buff[MSGBUF];
 sprintf(buff, "%s location: %s line: %d\n", m,f,l);
 ConstructMsg(buff);
}
