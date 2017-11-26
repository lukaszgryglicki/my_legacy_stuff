#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define FIELD_NONE 0x21

int main()
{
 FILE* f;
 int i;
 f = fopen("game.dat","w");
 if (!f) return 1;
 srand(getpid());
 /*for (i=0;i<4096;i++) fputc(0x40+(rand()%0x40), f);*/
 for (i=0;i<4096;i++) fputc(FIELD_NONE, f);
 fclose(f);
}

