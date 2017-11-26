#include <stdio.h>
#include <string.h>
#include "mran.h"

int main(int lb, char** par)
{
 if (lb-2) return -1;
 FILE* out = fopen(par[1],"r");
 out = fopen(par[1], "w");
 randomize();
 int size = random(10)+7;
 int etab;
 int last;
 int kk;
 fprintf(out,"%d\n", size);
 for (int i=0;i<size;i++)
   {
    etab = random(size/2)+1;
    fprintf(out,"[%d] ", etab);
    kk = random(6);
    if (kk) fprintf(out,"(%d, %lf) ", i, (double)((double)(random(1000)-500.0)/100.0));
    else fprintf(out,"(%d, %lf) ", 0, (double)(random(1000)-500.0)/100.0);
    last=0;
    for (int j=0;j<etab-1;j++)
      {
       last = last + random((size/etab)+1);
       if (last >= size) last = size-1;
       fprintf(out,"(%d, %lf) ", last, (double)((double)(random(1000)-500.0)/100.0));
      }
    fprintf(out," | %lf\n",(double)((double)(random(1000)-500.0)/100.0));
   }
 kill_random();
 return 0;
 fclose(out);
}

