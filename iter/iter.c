#include <stdio.h>
#define ULL unsigned long long
#define LD long double
#define SI short

LD hyper(SI n, LD a, ULL b)
{
    if (n==0) return b+1ULL;
    if (b==0)
    {
	if (n==1) return a;
	else if (n==2) return 0;
	else if (n>=3) return 1;
    }
    return hyper(n-1,a,hyper(n,a,b-1));
}

int main(int lb, char** par)
{
    SI n;
    LD a, r;
    ULL b;
    sscanf(par[1], "%d", &n);
    sscanf(par[2], "%lf", &a);
    sscanf(par[3], "%llu", &b);
    r=hyper(n, a, b);
    printf("hyper(%d, %lf, %llu)\t:-->\t%lf\n", n, a, b, r);
}

