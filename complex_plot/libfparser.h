#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <complex>

using namespace std;


#define FUNC_F 0
#define FUNC_U0 1
#define FUNC_U1 2
#define FUNC_G 3

int fpar_set_vars(int);
complex<double> fpar_f(complex<double>);
complex<double> fpar2_f(complex<double>, complex<double>);
complex<double> fpar3_f(complex<double>, complex<double>, complex<double>);
int fpar_function(char* tab);
void fpar_info();
void fpar_free();
int fpar_ok();
int fpar2_ok();
int fpar3_ok();

/* supported funnctions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt,abs
 * supported functions: asin,acos,asinh,acosh,sinh,cosh,cbrt,ceil,sgn
 * supported functions: tanh,atan,atanh,tgh,atg,atgh
 * supported operators: +,-,*,/,^, unary -
 * blank characters are skipped */
