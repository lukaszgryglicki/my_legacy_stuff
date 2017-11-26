#include "parser.h"
#define ID_LEN 32

char** buffer;
int* position;
char* ch;
int* maxpos;
real term();
real factor();
real expression(); 
real exponential();
real *x, *y, *z, *yp, *zp, *v, *vp;
int* err;
int* nvar;
int N;

int fpar_init(int ntab)
{
 int i;
 if (ntab < 0) return 0;
 buffer = (char**)malloc(ntab*sizeof(char*));
 if (!buffer) return 0;
 position = (int*)malloc(ntab*sizeof(int));
 ch = (char*)malloc(ntab*sizeof(char));
 maxpos = (int*)malloc(ntab*sizeof(int));
 x = (real*)malloc(ntab*sizeof(real));
 y = (real*)malloc(ntab*sizeof(real));
 z = (real*)malloc(ntab*sizeof(real));
 v = (real*)malloc(ntab*sizeof(real));
 yp = (real*)malloc(ntab*sizeof(real));
 zp = (real*)malloc(ntab*sizeof(real));
 vp = (real*)malloc(ntab*sizeof(real));
 err = (int*)malloc(ntab*sizeof(int));
 nvar = (int*)malloc(ntab*sizeof(int));
 
 N = ntab;
 for (i=0;i<N;i++)
   {
    buffer[i] = NULL;
    position[i] = 0;
    err[i] = 0;
    nvar[i] = 1;
   }
 return 1;
}

void exception(int num, char* reason)
{
 printf("Parser Exception: %s\n\n", reason);
 printf("Buffer[%d] = '%s'\n", num, buffer[num]);
 err[num]=1;
}

void fpar_info()
{
 printf("supported functions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt,abs\n"
        "supported functions: asin,acos,asinh,acosh,sinh,cosh,cbrt,ceil,sgn\n"
        "supported functions: tanh,atan,atanh,tgh,atg,atgh\n"
        "supported operators: +,-,*,/,^, unary -\n"
        "blank characters are skipped\n");
}

int fpar_ok(int num)
{
 nvar[num]=1;
 fpar_f(num, 0.);
 return !err;
}

int fpar2_ok(int num)
{
 nvar[num]=2;
 fpar2_f(num, 0., 0.);
 return !err[num];
}

int fpar3_ok(int num)
{
 nvar[num]=3;
 fpar3_f(num, 0., 0., 0.);
 return !err[num];
}

int fpar5_ok(int num)
{
 nvar[num]=5;
 fpar5_f(num, 0., 0., 0., 0., 0.);
 return !err[num];
}

int fpar7_ok(int num)
{
 nvar[num]=7;
 fpar7_f(num, 0., 0., 0., 0., 0., 0., 0.);
 return !err[num];
}


int fpar_function(int num, char* tab)
{
 if (!tab) return 1;
 if (buffer[num]) { free(buffer[num]); buffer[num] = 0; }
 buffer[num] = (char*)malloc(strlen(tab)+2);
 if (!buffer[num]) return 1;
 strcpy(buffer[num],tab);
 buffer[num][strlen(buffer[num])]   = 10;
 buffer[num][strlen(buffer[num])+1] = 0;
 maxpos[num] = strlen(buffer[num]);
 return 0;
}

void fpar_free(int num)
{
 if (buffer[num]) { free(buffer[num]); buffer[num] = 0; }
}


void read_next_char(int num)
{
 if (position[num] < maxpos[num] && ch[num] != 10) 
     ch[num] = buffer[num][position[num]++];
}

void skipblanks(int num)
{
 if (ch[num] != 10)
  {
   while (isspace(ch[num]) && ch[num] != 10) 
       read_next_char(num);
  }
}

void read_id(int num, char *ident)
{
 int cnt=0;
 skipblanks(num);
 if (isalpha(ch[num]))
   {
    while (isalpha(ch[num]) || isdigit(ch[num]))
      {
       if (cnt < ID_LEN-1) ident[cnt++] = ch[num];
       read_next_char(num);
      }
    ident[cnt] = 0;
   }
 else exception(num, "Expected: function name or variable.\n");
}

real factor(int num)
{
 real f, minus,tmp;
 char ident[ID_LEN],c;
 minus = 1.0;
 read_next_char(num);
 skipblanks(num);
 while (ch[num]=='+' || ch[num]=='-')
   {
     if (ch[num] == '-') minus *= -1;
     read_next_char(num);
   }
 if (isdigit(ch[num]) || ch[num]=='.')
   {
    buffer[num][--position[num]] = ch[num];
    sscanf(buffer[num]+position[num], "%Lf%c", &f, &ch[num]);
    c=ch[num];
    do read_next_char(num);
    while (ch[num] != c);
   }
 else if (ch[num] == '(')
   {
    f = expression(num);
    skipblanks(num);
    if (ch[num] == ')') read_next_char(num);
    else exception(num, "Expected: '('.\n");
   }
 else
   {
    read_id(num, ident);
    if (!strcmp(ident, "x") || !strcmp(ident, "a") || !strcmp(ident, "t")) f = x[num];
    else if (!strcmp(ident, "y") || !strcmp(ident, "b")) 
      {
       if (nvar[num] < 2) exception(num, "'y' found in one variable function.\n");
       else f = y[num];
      }
    else if (!strcmp(ident, "z") || !strcmp(ident, "c")) 
      {
       if (nvar[num] < 3) exception(num, "'z' found in one or two variable function.\n");
       else f = z[num];
      }
    else if (!strcmp(ident, "yp")) 
      {
       if (nvar[num] < 3) exception(num, "'yp' found in < 3 variable function\n");
       else f = yp[num];
      }
    else if (!strcmp(ident, "zp")) 
      {
       if (nvar[num] < 5) exception(num, "'zp' found in < 5 variable function\n");
       else f = zp[num];
      }
    else if (!strcmp(ident, "v")) 
      {
       if (nvar[num] < 7) exception(num, "'v' found in < 7 variable function\n");
       else f = v[num];
      }
    else if (!strcmp(ident, "vp")) 
      {
       if (nvar[num] < 7) exception(num, "'vp' found in < 7 variable function\n");
       else f = vp[num];
      }
    else if (!strcmp(ident, "sin"))
      {
	skipblanks(num);
	if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = sin(exponential(num));
	  }
	else exception(num, "Expected: '(' after sin.\n");
      }
     else if (!strcmp(ident, "cos"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = cos(exponential(num));
	  }
       else exception(num, "Expected: '(' after cos.\n");
      }
    else if (!strcmp(ident, "sinh"))
      {
	skipblanks(num);
	if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = sinh(exponential(num));
	  }
	else exception(num, "Expected: '(' after sinh.\n");
      }
     else if (!strcmp(ident, "cosh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = cosh(exponential(num));
	  }
       else exception(num, "Expected: '(' after cosh.\n");
      }
     else if (!strcmp(ident, "acos"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = acos(exponential(num));
	  }
       else exception(num, "Expected: '(' after acos.\n");
      }
     else if (!strcmp(ident, "asin"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = asin(exponential(num));
	  }
       else exception(num, "Expected: '(' after asin.\n");
      }
     else if (!strcmp(ident, "asinh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = asinh(exponential(num));
	  }
       else exception(num, "Expected: '(' after asinh.\n");
      }
     else if (!strcmp(ident, "acosh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = acosh(exponential(num));
	  }
       else exception(num, "Expected: '(' after acosh.\n");
      }
     else if (!strcmp(ident, "tg") || !strcmp(ident, "tan"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	 {
	  buffer[num][--position[num]] = ch[num];
	  f = tan(exponential(num));
	 }
        else exception(num, "Expected: '(' after tg or tan.\n");
       }
     else if (!strcmp(ident, "atg") || !strcmp(ident, "atan"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	 {
	  buffer[num][--position[num]] = ch[num];
	  f = atan(exponential(num));
	 }
        else exception(num, "Expected: '(' after atg or atan.\n");
       }
     else if (!strcmp(ident, "tgh") || !strcmp(ident, "tanh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	 {
	  buffer[num][--position[num]] = ch[num];
	  f = tanh(exponential(num));
	 }
        else exception(num, "Expected: '(' after tgh or tanh.\n");
       }
     else if (!strcmp(ident, "atgh") || !strcmp(ident, "atanh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	 {
	  buffer[num][--position[num]] = ch[num];
	  f = atanh(exponential(num));
	 }
        else exception(num, "Expected: '(' after atgh or atanh.\n");
       }
     else if (!strcmp(ident, "ctg") || !strcmp(ident,"ctan"))
       {
	skipblanks(num);
	if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = 1/tan(exponential(num));
	  }
	else exception(num, "Expected: '(' after ctg or ctan.\n");
       }
     else if (!strcmp(ident, "exp") || !strcmp(ident,"e"))
       {
	skipblanks(num);
	if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = exp(exponential(num));
	  }
	else exception(num, "Expected: '(' after exp or e.\n");
	}
	else if (!strcmp(ident, "ln"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = log(exponential(num));
	     }
	   else exception(num, "Expected: '(' after ln.\n");
	  }
	else if (!strcmp(ident, "log"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = log10(exponential(num));
	     }
	   else exception(num, "Expected: '(' after log.\n");
	  }
	else if (!strcmp(ident, "sqrt"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = sqrt(exponential(num));
	     }
	   else exception(num, "Expected: '(' after sqrt.\n");
	  }
	else if (!strcmp(ident, "cbrt"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = cbrt(exponential(num));
	     }
	   else exception(num, "Expected: '(' after cbrt.\n");
	  }
	else if (!strcmp(ident, "ceil"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = ceil(exponential(num));
	     }
	   else exception(num, "Expected: '(' after ceil.\n");
	  }
	else if (!strcmp(ident, "sgn"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      tmp = exponential(num);
	      if (tmp > 0.0) f = 1.;
	      else if (tmp < 0.0) f = -1.;
	      else f = 0.;
	     }
	   else exception(num, "Expected: '(' after sgn.\n");
	  }
	else if (!strcmp(ident, "abs"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = fabs(exponential(num));
	     }
	   else exception(num, "Expected: '(' after abs.\n");
	  }
	else exception(num, "Unknown identifier.\n");
	}
	skipblanks(num);
	return minus*f;		
}

real term(int num)
{
 real f1;
 f1 = exponential(num);
 while(1)
   {
    switch (ch[num])
      {
       case '*': f1 *= exponential(num); break;
       case '/': f1 /= exponential(num); break;
       default: return f1;
      }
   }
}

real expression(int num)
{
 real t1;
 t1 = term(num);
 while(1)
   {
    switch (ch[num])
      {
       case '+': t1 += term(num); break;
       case '-': t1 -= term(num); break;
       default: return t1;
      }
   }
}

real exponential(int num)
{
 real f = factor(num);
 while (ch[num] == '^') f = pow(f, exponential(num));
 return f;
}


real fpar_f(int num, real X)
{
 real e;
 x[num]=X;
 y[num]=0.;
 z[num]=0.;
 v[num]=0.;
 yp[num]=0.;
 zp[num]=0.;
 vp[num]=0.;
 position[num]=0;
 ch[num]=0;
 e=expression(num);
 if (ch[num] != 10 && ch[num] != ';') exception(num, "Garbage in function expression.\n");
 if (err[num]) 
   {
    exception(num, "Value returned MAY BE invalid, exceptions occured.\n");
    return e;
   }
 return e;
}

real fpar2_f(int num, real X, real Y)
{
 real e;
 x[num]=X;
 y[num]=Y;
 v[num]=0.;
 z[num]=0.;
 yp[num]=0.;
 zp[num]=0.;
 vp[num]=0.;
 position[num]=0;
 ch[num]=0;
 e=expression(num);
 if (ch[num] != 10 && ch[num] != ';') exception(num, "Garbage in function expression.\n");
 if (err[num]) 
   {
    exception(num, "Value returned MAY BE invalid, exceptions occured.\n");
    return e;
   }
 return e;
}

real fpar3_f(int num, real X, real Y, real Z)
{
 real e;
 x[num]=X;
 y[num]=Y;
 z[num]=Z;
 v[num]=0.;
 yp[num]=0.;
 zp[num]=0.;
 vp[num]=0.;
 position[num]=0;
 ch[num]=0;
 e=expression(num);
 if (ch[num] != 10 && ch[num] != ';') exception(num, "Garbage in function expression.\n");
 if (err[num]) 
   {
    exception(num, "Value returned MAY BE invalid, exceptions occured.\n");
    return e;
   }
 return e;
}

real fpar5_f(int num, real X, real Y, real YP, real Z, real ZP)
{
 real e;
 x[num]=X;
 y[num]=Y;
 z[num]=Z;
 v[num]=0.;
 yp[num]=YP;
 zp[num]=ZP;
 vp[num]=0.;
 position[num]=0;
 ch[num]=0;
 e=expression(num);
 if (ch[num] != 10 && ch[num] != ';') exception(num, "Garbage in function expression.\n");
 if (err[num]) 
   {
    exception(num, "Value returned MAY BE invalid, exceptions occured.\n");
    return e;
   }
 return e;
}

real fpar7_f(int num, real X, real Y, real YP, real Z, real ZP, real V, real VP)
{
 real e;
 x[num]=X;
 y[num]=Y;
 z[num]=Z;
 v[num]=V;
 yp[num]=YP;
 zp[num]=ZP;
 vp[num]=VP;
 position[num]=0;
 ch[num]=0;
 e=expression(num);
 if (ch[num] != 10 && ch[num] != ';') exception(num, "Garbage in function expression.\n");
 if (err[num]) 
   {
    exception(num, "Value returned MAY BE invalid, exceptions occured.\n");
    return e;
   }
 return e;
}

