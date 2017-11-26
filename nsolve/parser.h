#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#define real long double

real fpar_f(int, real);
real fpar2_f(int, real, real);
real fpar3_f(int, real, real, real);
real fpar5_f(int, real, real, real, real, real);
real fpar7_f(int, real, real, real, real, real, real, real);
int fpar_function(int, char* tab);
void fpar_info();
void fpar_free(int);
int fpar_ok(int);
int fpar2_ok(int);
int fpar3_ok(int);
int fpar5_ok(int);
int fpar7_ok(int);
int fpar_init(int);

/* supported funnctions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt,abs
 * supported functions: asin,acos,asinh,acosh,sinh,cosh,cbrt,ceil,sgn
 * supported functions: tanh,atan,atanh,tgh,atg,atgh
 * supported operators: +,-,*,/,^, unary -
 * blank characters are skipped */
