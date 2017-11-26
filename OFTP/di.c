#include <stdio.h>
#include <stdlib.h>

int main(int lb, char** par)
{
 FILE *f1, *f2;
 int z1,z2,i,di;
 if (lb<3) return 1;
 f1 = fopen(par[1],"r");
 f2 = fopen(par[2],"r");
 if (!f1 || !f2) return 2;
 i=0;
 di=0;
 while ((z1=fgetc(f1))!=EOF)
 {
  z2=fgetc(f2);
  if (z2==EOF) break;
  i++;
  if (z1!=z2) { printf("%d: (%d,%d)\n", i,z1,z2); di++; }
 }
 printf("di=%d,i=%d, %%bad=%f\n",di,i,((float)di*100.0)/(float)i);
 return 0;
}

