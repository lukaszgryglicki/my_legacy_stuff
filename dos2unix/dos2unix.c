#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHAR_CR 0xd

int main(int lb, char** par)
{
 if (lb-2) { printf("%s: No filename given.\n", par[0]); return -1;}
 FILE* in = fopen(par[1],"r");
 if (!in)  { printf("%s: failed to open file RDONLY:%s\n",par[0],par[1]); return -2;}
 FILE* out= fopen("fiipi", "w");
 if (!out)  { printf("%s: failed to open file RDWR:fiipi\n",par[0]); fclose(in); return -4;}
 int zn;
 while ((zn=fgetc(in))!=EOF) if (zn!=CHAR_CR) fputc(zn, out);
 fclose(in);
 fclose(out);
 char syscmd[0x140];
 sprintf(syscmd, "mv -f ./fiipi %s 2>/dev/null", par[1]);
 system(syscmd);
 return 0;
}