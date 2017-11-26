#include <stdio.h>
#include <stdlib.h>

long long nc;
long silent;

unsigned int ackermann(unsigned int m, unsigned int n) 
{
  nc ++;
  if (m == 0)
    return n + 1;
  else if (n == 0) 
    return ackermann(m-1, 1);
  else 
    return ackermann(m-1, ackermann(m, n - 1));
}

double ackermannd(double m, double n) 
{
  nc ++;
  if (m > -1. && m <= 0)
    return n + 1. + m;
  else if (n <= 0.) 
    return ackermannd(m-1., 1.);
  else 
    return ackermannd(m-1., ackermannd(m, n - 1.));
}

unsigned int awp(unsigned int m, unsigned int n)
{
 unsigned int r;
 nc = 0;
 r = ackermann(m, n);
 printf("Ackremann(%d, %d) is %d, computed with %d iterations\n", m, n, r, nc);
 return r;
}

double awpd(double m, double n)
{
 double r;
 nc = 0;
 r = ackermannd(m, n);
 printf("Ackremann(%lf, %lf) is %lf, computed with %d iterations\n", m, n, r, nc);
 return r;
}

unsigned long hypern(unsigned long a, unsigned long n, unsigned long b)
{
 nc ++;
 if (!silent) printf("a=%4d, n=%2d, b=%4d, #%lld\n", a, n, b, nc);
 if (n == 0) return b + 1;
 if (n == 1 && b == 0) return a;
 if (n == 2 && b == 0) return 0;
 if (n >= 3 && b == 0) return 1;
 return hypern(a, n - 1, hypern(a, n, b - 1));
}

unsigned long hyp(unsigned long a, unsigned long n, unsigned long b, unsigned long sil)
{
 unsigned int r;
 nc = 0;
 silent = sil;
 printf("HyperN(%d, %d, %d)...\n", a, n, b);
 r = hypern(a, n, b);
 printf("HyperN(%d, %d, %d) is %d, computed with %lld iterations\n", a, n, b, r, nc);
 return r;
}

double hypernd(double a, unsigned long n, double b)
{
 printf("%lf %d %lf\n", a, n, b); 
 if (n == 0) return b + 1;
 if (n == 1 && b <= 0) return a;
 if (n == 2 && b <= 0) return 0;
 if (n >= 3 && b <= 0) return 1;
 return hypernd(a, n - 1, hypernd(a, n, b - 1));
}

int main(int lb, char** par)
{
 printf("m n\n");
/* printf("%lf\n", hypernd(atof(par[1]), atoi(par[2]), atof(par[3])));*/
 printf("%lf\n", ackermannd(atof(par[1]), atof(par[2])));
 return 0;
}

