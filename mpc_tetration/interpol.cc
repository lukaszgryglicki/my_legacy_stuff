#include <math.h>
#include <stdlib.h>
#include <stdio.h>

long double g(long double x, long double z)
{
    long double r = pow(x, (1. + erf(2.*z - 1.))/2.);
    //long double r = pow(x, (1. + erf(.5*z - .25))/2.);
    //printf("g(%.15Lf, %.15Lf) --> %.15Lf\n", x, z, r);
    return r;
}

long double t(long double x, long double z, int n)
{
    if (n == 0) 
    {
    	//printf("last: t(%.15Lf, %.15Lf, %d)\n", x, z, n);
	return g(x, z);
    }
    else 
    {
	long double pg = g(x, z);
	//printf("PG: %.15Lf (%d, %.15Lf, %.15Lf)\n", pg, n, x, z);
	if (fabs(pg-1.) < 1e-15)
	{
	    //printf("cut out: %d\n", n);
	    return 1.0;
	}
	return pow(pg, t(x, z-1., n-1));
    }
}

long double h(long double x, long double z)
{
    return t(x, z, 1000);
}

long double h2(long double x, long double z, int n)
{
    // log(a, b) = log(e, b) / log(e, a);
    if (n == 0) return h(x, z);
    else 
    {
	/*
	long double tt;
	tt = log(h2(x, z+1., n-1));
	printf("%.15Lf %.15Lf\n", tt, log(x));
	*/
	return log(h2(x, z+1., n-1)) / log(x);
    }
}

long double h3(long double x, long double z)
{
    return h2(x, z, 2);
}

long double bias(long double x)
{
    return 0.019*x;
}

long double h4(long double x, long double z)
{
    return h3(x, z + bias(x));
}

int main(int lb, char** par)
{
    long double x = 2.0, z;
    for (z=-2.5;z<=3.501;z+=0.1)
    {
	//printf("g(%.15Lf, %.15Lf) = %.15Lf\n", x, z, g(x, z));
    }
	
    int n = 20;
    z = 2.;
    printf("t(%.15Lf, %.15Lf, %d) = %.15Lf\n", x, z, n, t(x, z, n));
    printf("h(%.15Lf, %.15Lf) = %.15Lf\n", x, z, h(x, z));
    n = 2;
    printf("h2(%.15Lf, %.15Lf, %d) = %.15Lf\n", x, z, n, h2(x, z, n));
    printf("h3(%.15Lf, %.15Lf) = %.15Lf\n", x, z, h3(x, z));
    printf("h4(%.15Lf, %.15Lf) = %.15Lf\n", x, z, h4(x, z));

    //for (z=-2.;z<=4.1;z+=0.1) printf("g(%.15Lf, %.15Lf) = %.15Lf\n", x, z, g(x, z));
    return 0;
}

