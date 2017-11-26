#include <stdlib.h>
#include <stdio.h>

extern unsigned long asmf1(unsigned long);
extern unsigned long asmf2(unsigned long, unsigned long);
extern unsigned long asmf3(unsigned long, unsigned long, unsigned long);
/* MODE, N, X, Y, CT, NT, PTR*** iv, PTR** ov */
extern void loop_asm(int udfmode, int n, int x, int y, int ct, int nt, void* iv, void* ov);

/*void translate_jpeg_file(int n, unsigned long*** iv, unsigned long** ov, int x, int y, int ct, int nt, int udfmode); */
/*for (i=ct;i<y;i+=nt)*/
/*for (j=0;j<x;j++)*/
/*ov[i][j] = asmf1(iv[0][i][j]);*/

void test_aloop()
{
    unsigned long ***iv, **ov;
    int udfmode[3], n, x, y, ct, nt;
    int i,j,k;

    udfmode[0] = 4;
    udfmode[1] = 11;
    udfmode[2] = 12;

    n       = 3;
    nt      = 2;

    x       = 4;
    y       = 4;

    iv = (unsigned long***)malloc(n*sizeof(unsigned long**));
    for (k=0;k<n;k++) iv[k] = (unsigned long**)malloc(y*sizeof(unsigned long*));
    for (k=0;k<n;k++) for (i=0;i<y;i++) iv[k][i] = (unsigned long*)malloc(x*sizeof(unsigned long));
    for (k=0;k<n;k++) for (i=0;i<y;i++) for (j=0;j<x;j++) iv[k][i][j] = (k << 0x15) + (i << 0xD) + (j << 0x5);

    ov = (unsigned long**)malloc(y*sizeof(unsigned long*));
    for (i=0;i<y;i++) ov[i] = (unsigned long*)malloc(x*sizeof(unsigned long));
    for (i=0;i<y;i++) for(j=0;j<x;j++) ov[i][j] = 0x10000*i + 0x100*j;

    printf("iv=%p ov=%p iv[0]=%p iv[1]=%p iv[1][0]=%p iv[1][0][0]=%d\n", 
	    iv, ov, iv[0], iv[1], iv[1][0], iv[1][0][0]);
    for(k=0;k<1;k++) for (ct=0;ct<nt;ct++) loop_asm(udfmode[k], n, x, y, ct, nt, iv, ov);

    for (i=0;i<y;i++) 
    {
	for (j=0;j<x;j++) printf("%08lx ", ov[i][j]); 
	printf("\n");
    }

}

int main()
{
/*    int x, y, z;*/
    int z;

    test_aloop();


/*    x = asmf1(0xAABBCCDD);*/
/*    y = asmf2(0x10111213, 0x14151617);*/
/*    z = asmf3(0x20212223, 0xA0A1A2A3, 0xE0E1E2E3);*/
/*    z = asmf3(0x10111213, 0x20212223, 0x30313233);*/
    z = asmf3(0x00FF03AA, 0x000204BB, 0x0003ffCC);

/*    printf("%x %x %x\n", x, y, z);*/
    printf("%08x\n", z);

    return 0;
}

