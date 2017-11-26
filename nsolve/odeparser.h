#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#define real long double

real fpar_f(int, real, real*, real*, real*);	/* which_func, t, y[], y'[] y"[]*/
int fpar_function(int, int, char* tab); /* which, #of_args, def */
void fpar_info();
void fpar_free(int);
void fpar_deinit();
int fpar_ok(int);
int fpar_init(int);

/* supported funnctions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt,abs
 * supported functions: asin,acos,asinh,acosh,sinh,cosh,cbrt,ceil,sgn
 * supported functions: tanh,atan,atanh,tgh,atg,atgh
 * supported operators: +,-,*,/,^, unary -
 * blank characters are skipped */
