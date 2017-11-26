#include <stdio.h>
#include <stdlib.h>
#include <mran.h>

static int n=10000;


void bsort(int* A)
{
 int tmp;
 for (int i=0;i<n-2;i++)
   {
    for (int j=0;j<=n-1-i;j++) if (A[j]>A[j+1])
      {
       tmp=A[j];  A[j]=A[j+1];  A[j+1]=tmp;
      }
   }
}

int main(int lb, char** par)
{
 if (lb==2) sscanf(par[1],"%d",&n);
 randomize();
 int* tablica = new int[n];
 if (!tablica) return -1;
 for (int i=0;i<n;i++) tablica[i] = random(1000);
 bsort(tablica);
}

