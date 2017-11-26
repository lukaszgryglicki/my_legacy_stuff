#include "MathException.hh"
#include <iostream>

using namespace std;

MathException :: ~MathException() {}

void MathException :: Print() const
{
#ifdef DEBUG
 cout<<"MathException::Print()"<<endl;
#endif
 cout<<"Math exception";
 if (msg) cout<<": "<<msg;
 cout<<endl;
}

MathException :: MathException(char* m, char* f, int l)
{
#ifdef DEBUG
 cout<<"MathException constructor"<<endl;
#endif
 char buff[MSGBUF];
 sprintf(buff, "%s location: %s line: %d\n", m,f,l);
 ConstructMsg(buff);
}

