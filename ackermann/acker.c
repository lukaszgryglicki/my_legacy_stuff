#include <stdlib.h>
#include <math.h>
#include <gmp.h>

#define LONG64 __int64_t

#include <complex>
using namespace std;

mpz_t maxmod;

void hypernz(mpz_t y, int degree, mpz_t a, mpz_t b)
{
 if (degree == 0) 
 {
     mpz_add_ui(y, b, 1);
     return;
 }
 else if (degree == 1) 
 {
     mpz_add(y, a, b);
     return;
 }
 else if (degree == 2) 
 {
     mpz_mul(y, a, b);
     return;
 }
 else if (degree == 3) 
 {
     mpz_powm(y, a, b, maxmod);
     return;
 }
 else 
 {
     
     if (mpz_cmp_ui(b, 0) == 0) 
     {
	 mpz_set_ui(y, 1);
	 return ;
     }
     else 
     {
	 mpz_t tmp_y, dec_b;

	 mpz_init( tmp_y );
	 mpz_init( dec_b );
	 mpz_sub_ui( dec_b, b, 1 );

	 hypernz( tmp_y, degree, a, dec_b ); 
	 hypernz( y, degree - 1, a, tmp_y );

	 mpz_clear( tmp_y );
	 mpz_clear( dec_b );
	 
	 return;
     }
 }
}

complex<long double> hypernc(long degree, complex<long double> a, complex<long double> b)
{
 if (degree == 0) return b + complex<long double>(1., 0.);
 else if (degree == 1) return a + b;
 else if (degree == 2) return a * b;
 else if (degree == 3) return pow(a, b);
 else 
 {
     if (abs(b) < 1.0e-13) return 1.;
     else return hypernc(degree - 1, a, hypernc(degree, a, b - complex<long double>(1., 0.)));
 }
}

long double hypernd(long degree, long double a, long double b)
{
 if (degree == 0) return b + 1.;
 else if (degree == 1) return a + b;
 else if (degree == 2) return a * b;
 else if (degree == 3) return pow(a, b);
 else 
 {
     if (fabs(b) < 1.0e-13) return 1.;
     else return hypernd(degree - 1, a, hypernd(degree, a, b - 1));
 }
}

/*
void hypernf(mpf_t y, int degree, mpf_t a, mpf_t b)
{
 if (degree == 0) 
 {
     mpf_add_ui(y, b, 1);
     return;
 }
 else if (degree == 1) 
 {
     mpf_add(y, a, b);
     return;
 }
 else if (degree == 2) 
 {
     mpf_mul(y, a, b);
     return;
 }
 else if (degree == 3) 
 {
     mpf_powm(y, a, b, maxmod);
     return;
 }
 else 
 {
     
     if (mpf_cmp_ui(b, 0) == 0) 
     {
	 mpf_set_ui(y, 1);
	 return ;
     }
     else 
     {
	 mpf_t tmp_y;
	 mpf_t dec_b;

	 mpf_init( tmp_y );
	 mpf_init( dec_b );
	 mpf_sub_ui( dec_b, b, 1 );

	 hypernf( tmp_y, degree, a, dec_b ); 
	 hypernf( y, degree - 1, a, tmp_y );

	 mpf_clear( tmp_y );
	 mpf_clear( dec_b );
	 
	 return;
     }
 }
}
*/
int main(int lb, char** par)
{
    printf("tetra[base] --> base ^^ inf\n");
    if (lb < 2) return 1;
    long double dr, di, ab;
    long loop, i;
    sscanf(par[1], "%Lf", &dr);
    sscanf(par[2], "%Lf", &di);
    complex<long double> d(dr, di);
    complex<long double> y, py;
    loop = 1;
    y = d;
    i = 1;
    printf("(%.19Lf,%.19Lf)\n", d.real(), d.imag());
    while (loop)
    {
	i ++;
	py = y;
	y = hypernc(4, d, (long double)i);
	ab = abs(py - y);
/*	printf("AB = %Lf\n", ab);*/
	if (isinf(ab) || isnan(ab)) loop = 0;
	if (ab < 1e-15) loop = 0;
	if (ab > 1e20) loop = 0;
/*	printf("# (%.19Lf,%.19Lf) ^^ inf = (%.19Lf, %.19Lf) %ld iters\n", d.real(), d.imag(), y.real(), y.imag(), i);*/
	if (!loop) printf("(%.19Lf,%.19Lf) ^^ inf = (%.19Lf, %.19Lf) %ld iters\n", d.real(), d.imag(), y.real(), y.imag(), i);
    }
    return 0;
}
/*
int main(int lb, char** par)
{
    printf("tetra[num, iters, step, start]\n");
    if (lb < 5) return 1;
    long double d, y;
    long n, i, s, a;
    sscanf(par[1], "%Lf", &d);
    sscanf(par[2], "%ld", &n);
    sscanf(par[3], "%ld", &s);
    sscanf(par[4], "%ld", &a);
    printf("base = %.19Lf, iters = %d, step = %d, start = %d\n", d, n, s, a);
    for (i=a;i<n;i+=s)
    {
	y = hypernd(4, d, (long double)i);
	printf("%.19Lf ^^ %d = %.19Lf\n", d, i, y);
    }
    return 0;
}
*/

/*
int main(int lb, char** par)
{
 mpz_t a, b, y;
 int d, v;

 //printf("degree(int) a(mpz) b(mpz) modlimit(2^(1 << v))\n");
 printf("degree(int) a(mpz) b(mpz) maxresult(1 << v)\n");
 if (lb < 5) return 1;

 mpz_init( a );
 mpz_init( b );
 mpz_init( y );
 mpz_init( maxmod );

 sscanf(par[1], "%d", &d);
 sscanf(par[4], "%d", &v);
 gmp_sscanf(par[2], "%Zd", &a);
 gmp_sscanf(par[3], "%Zd", &b);

 char* bignum;
 int i,n;
 n = 1 << v;
 bignum = (char*)malloc( n + 1 );
 bignum[0] = '1';
 for (i=1;i<n;i++) bignum[i] = '0';
 bignum[n] = 0;
 mpz_set_str(maxmod, bignum, 16);
 free((void*)bignum);
 //mpz_ui_pow_ui( maxmod, 2, 0x1 << v );
// gmp_printf("ModLimit:\n%Zd\n", maxmod);
 gmp_printf("%Zd [%d] %Zd:\n", a, d, b);
 hypernz(y, d, a, b);

 gmp_printf("%Zd\n", y);
 mpz_clear( a );
 mpz_clear( b );
 mpz_clear( y );
 mpz_clear( maxmod );
 return 0;
}


double d_expm1, d_exp0m1;

double hyperpowerd(int degree, double a, double b)
{
 if (degree == 0) return b + 1;
 else if (degree == 1) return a + b;
 else if (degree == 2) return a * b;
 else if (degree == 3) return pow(a, b);
 else 
 {
     if (b == 0) return 1.;
     else if (b < 0)
     {
 	printf("WARNING, using interpolation: h(%d,%lf,%lf) \n", degree, a, b);
	return (exp(b) - d_expm1) / d_exp0m1;
     }
     else return hyperpowerd(degree - 1, a, hyperpowerd(degree, a, b - 1));
 }
}

LONG64 hyperpowerl(LONG64 degree, LONG64 a, LONG64 b)
{
 if (degree == 0LL) return b + 1LL;
 else if (degree == 1LL) return a + b;
 else if (degree == 2LL) return a * b;
 else if (degree == 3) return (LONG64)pow((long double)a, (long double)b);
 else 
 {
     if (b == 0LL) return 1LL;
     else return (LONG64)hyperpowerl(degree - 1LL, a, hyperpowerl(degree, a, b - 1LL));
 }
}
*/

/*
int main(int lb, char** par)
{
 LONG64 d, a, b, y;
 printf("degree(long64) a(long64) b(long64)\n");
 if (lb < 4) return 1;
 sscanf(par[1], "%lld", &d);
 sscanf(par[2], "%lld", &a);
 sscanf(par[3], "%lld", &b);
 switch(d)
 {
     case 0: printf("zeration\n"); break;
     case 1: printf("addiction\n"); break;
     case 2: printf("multiplation\n"); break;
     case 3: printf("exponation\n"); break;
     case 4: printf("tetration\n"); break;
     case 5: printf("pentation\n"); break;
     case 6: printf("hexation\n"); break;
     default: printf("beyond hexation: ARE YOU MAD?\n"); break;

 };
 y = hyperpowerl(d, a, b);
 printf("result = %lld\n", y);
 return 0;
}


int main(int lb, char** par)
{
 int d;
 double a, b, y;
 printf("degree(int) a(double) b(double)\n");
 if (lb < 4) return 1;
 sscanf(par[1], "%d", &d);
 sscanf(par[2], "%lf", &a);
 sscanf(par[3], "%lf", &b);
 d_expm1  = exp( -1. );
 d_exp0m1 = exp( 0. ) - d_expm1; 
 y = hyperpowerd(d, a, b);
 printf("result = %lf\n", y);
 return 0;
}
*/
