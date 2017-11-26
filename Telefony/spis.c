#include "tele.h"
#include "exception.h"

int main()
{
 TELE* spis = new TELE;
 Wykonaj(spis);
 delete spis;
 mgetch();
 return OK;
}
