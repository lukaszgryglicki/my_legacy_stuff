#include <stdlib.h>
#include <stdio.h>

unsigned long get_rand(unsigned long range, unsigned long avg_degree)
{
 unsigned long *itab, i, ir, degree;
 degree = (rand() % avg_degree) + 1;
 itab = (unsigned long*)malloc(degree * sizeof(unsigned long));
 for (i=0;i<degree;i++)
 {
     itab[ i ] = rand() % range;
 }
 ir = itab[ 0 ];
 for (i=1;i<degree;i++)
 {
  ir  = (ir * itab[ i ]) % range;
 }
 free((void*)itab);
 return ir;
}

void rgen(unsigned long bstart, unsigned long brange, unsigned long fsize, unsigned long degree, unsigned long bytes)
{
 unsigned long i, zn;
 sranddev();
 if (bytes == 1)
 {
  	for (i=0;i<fsize;i++)
  	{
   	 zn = (bstart + get_rand( brange, degree )) % 0x100;
   	 fputc(zn, stdout);
  	}
 }
 else if (bytes == 2)
 {
  	for (i=0;i<(fsize >> 1);i++)
  	{
   	 zn = (bstart + get_rand( brange, degree )) % 0x10000;
   	 fputc(zn & 0xFF, stdout);
	 fputc(zn >> 0x8, stdout);
  	}
 }
 else if (bytes == 3)
 {
  	for (i=0;i<fsize/3;i++)
  	{
   	 zn = (bstart + get_rand( brange, degree )) % 0x1000000;
   	 fputc(zn & 0xFF, stdout);
	 fputc((zn >> 0x8) & 0xFF, stdout);
	 fputc(zn >> 0x10, stdout);
  	}
 }
 else
 {
  	for (i=0;i<(fsize >> 2);i++)
  	{
   	 zn = bstart + get_rand( brange, degree );
   	 fputc(zn & 0xFF, stdout);
	 fputc((zn >> 0x8) & 0xFF, stdout);
	 fputc((zn >> 0x10) & 0xFF, stdout);
	 fputc(zn >> 0x18, stdout);
  	}
 }
}

int main(int lb, char** par)
{
 unsigned long bstart, brange, fsize, degree, nbytes;
 if (lb < 5) 
 {
     printf("usage: %s byte_start[0-0xff[ffffff]] byte_range[1-0x100[000000]] avg_degree[1+] fsize[1+] nbytes {1,2,3,4}\n", par[0]);
     return 1;
 }
 nbytes = atoi(par[5]);
 if (nbytes < 1) nbytes = 1;
 if (nbytes > 4) nbytes = 4;
 if (sscanf(par[1], "0x%x", &bstart) != 1)
 {
     printf("error bstart\n");
     return 2;
 }
 /*bstart = atoi(par[1]);*/
 if (nbytes == 1) bstart %= 0x100;
 if (nbytes == 2) bstart %= 0x10000;
 if (nbytes == 3) bstart %= 0x1000000;
 if (sscanf(par[2], "0x%x", &brange) != 1)
 {
     printf("error brange\n");
     return 3;
 }
 /*brange = atoi(par[2]);*/
 if (brange < 1) brange = 1;
 if (nbytes == 1 && brange > 0x100) brange = 0x100;
 if (nbytes == 2 && brange > 0x10000) brange = 0x10000;
 if (nbytes == 3 && brange > 0x1000000) brange = 0x1000000;
 degree = atoi(par[3]);
 if (degree < 1) degree = 1;
 fsize = atoi(par[4]);
 if (fsize < 1) fsize = 1;
 rgen(bstart, brange, fsize, degree, nbytes);
 return 0;
}

