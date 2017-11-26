#include <stdio.h>
#include <stdlib.h>
#include <mran.h>

static int n=10000;

inline int cmpint(void* a, void* b)
{
 return ((int)a<(int)b);
}

void qsort(int l, int r, void** A, int (*cmpf)(void*, void*))
{
 int i=l,j=r+1;
 void* v=A[l],*tmp;
 do
   {
    do i++; while (cmpf(A[i],v));
    do j--; while (cmpf(v,A[j]));
    if (i<j) {tmp=A[i]; A[i]=A[j]; A[j]=tmp;}
   }
 while (i<j);
 A[l]=A[j]; A[j]=v;
 if (j-1>l) qsort(l,j-1, A, cmpf);
 if (j+1<r) qsort(j+1,r, A, cmpf);
}

void show(int* table)
{
 for (int i=0;i<n;i++) printf("%d, ", table[i]);
 putchar('\n');
}

int main(int lb, char** par)
{
 if (lb==2) sscanf(par[1],"%d",&n);
 randomize();
 int* tablica = new int[n];
 if (!tablica) return -1;
 for (int i=0;i<n;i++) tablica[i] = random(1000);
 void** ptr = new (void*)[n];
 for (int i=0;i<n;i++) ptr[i] = (void*)tablica[i];
 //show((int*)ptr);
 qsort(0, n, ptr, cmpint);
 //show((int*)ptr);
}

