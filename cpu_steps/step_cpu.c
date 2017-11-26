#include <stdio.h>

extern void sCPU(int);

int main(int lb, char** par)
{
 if (lb-2) {printf("usage:\t%s [number_loops]\n", par[0]); return -1;}
 int loo;
 sscanf(par[1],"%d", &loo);
 sCPU(loo);		//8+3*param CPU instructions
}

