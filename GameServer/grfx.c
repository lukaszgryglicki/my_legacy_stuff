#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void write_gfx(FILE* dat)
{
 int i,j,k;
 if (!dat) return;
 for (i=0;i<0x100;i++)
  {
   fprintf(dat,"FIELD %02x\n", i);
   for (j=0;j<8;j++)
     {
      for (k=0;k<8;k++) fprintf(dat,"(%02x,%02x,%02x) ", (rand()%0x40)+0x40, (rand()%0x40)+0x40, (rand()%0x40)+0x40);
      fprintf(dat,"\n");
     }
  }
 fclose(dat);
}


int main()
{
 FILE* f;
 f = fopen("graphics.dat","w");
 if (!f) return 1;
 srand(getpid());
 write_gfx(f);
 fclose(f);
}

