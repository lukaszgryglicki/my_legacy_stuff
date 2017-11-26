#include <mh.h>

static int ints=0;

void info_intr(unsigned char n, int at)
{
 if (n==0x80) printf("\tUNIX system interrupt 'call kernel' sys_call on stack <below>\n\tsearch stack at address 0x%08x-0x%08x\n", n-32, n);
 if (n==0) printf("\tNON_SYSTEM call, probably exit or return statement, arch independent\n");
}

void intr(FILE* f, int c)
{
 unsigned char intx;
 fscanf(f,"%c",&intx);
 printf("INTR at: 0x%08x, byte:%08d, kbyte:%05d, mbyte:%03d ==> (INT 0X%02x, DEC: %03d)\n", c, c, (c/1024), (c/(1024*1024)), intx, intx);
 info_intr(intx,c);
 ints++;
}

void search_ints(char* n)
{
 FILE* f= fopen(n,"rb");
 if (!f) { printf("file not found: %s\n", n); return;}
 int c=0;
 int zn;
 while ((zn=fgetc(f))!=EOF)
  {
   c++;
   if (zn==0xcd) { intr(f,c); c++; }      
  }
 fclose(f);
 printf("total interrupts found: %d, percent: %9.5f%%\n",ints, double((double)ints/(double)c));
}


int main(int lb, char** par)
{
 if (lb<2) { printf("%s exe-file\ncannot execute on: (null)\n", par[0]); return -1;}
 search_ints(par[1]);
 return 0;
}

 
