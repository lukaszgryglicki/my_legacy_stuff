#include "tele.h"
#include "exceptbsd.h"

int main()
{
 TELE* spis = new TELE;
 Wykonaj(spis);
 delete spis;
 mgetch();
 return OK;
}