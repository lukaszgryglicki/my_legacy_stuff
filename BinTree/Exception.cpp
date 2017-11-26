#include "Exception.h"
#include <iostream>

using namespace std;

Exception :: Exception()
{
 msg = NULL;
 cout<<"Przechwycono wyjatek: ";
}

void Exception :: ConstructMsg(char* m)
{
 if (!m)
   {
    cout<<"Blad alokacji pamieci w obsludze wyjatku!"<<endl;
    return;
   }
 msg = new char[strlen(m)+1];
 if (!msg)
   {
    cout<<"Blad alokacji pamieci w obsludze wyjatku!"<<endl;
    return;
   }
 strcpy(msg, m);
}

Exception :: ~Exception()
{
 if (msg) delete msg;
 msg = NULL;
}

void Exception :: Print() const
{
 cout<<"Wyjatek";
 if (msg) cout<<": "<<msg;
 cout<<endl;
}

