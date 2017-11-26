#include "odeparser.h"
#define ID_LEN 32

char** buffer;
int* position;
char* ch;
int* maxpos;
real term();
real factor();
real expression(); 
real exponential();
real *t;
real **y;
real **yp;
real **ypp;
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
 t = (real*)malloc(ntab*sizeof(real));
 y = (real**)malloc(ntab*sizeof(real*));
 yp = (real**)malloc(ntab*sizeof(real*));
 ypp = (real**)malloc(ntab*sizeof(real*));
 err = (int*)malloc(ntab*sizeof(int));
 nvar = (int*)malloc(ntab*sizeof(int));
 
 N = ntab;
 for (i=0;i<N;i++)
   {
    buffer[i] = NULL;
    position[i] = 0;
    err[i] = 0;
    nvar[i] = 1;
    y[i] = yp[i] = ypp[i] = NULL;
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
 real* tes;
 int i, nv;
 nv = nvar[num];
 tes = (real*)malloc(nv*sizeof(real));
 for (i=0;i<nv;i++) tes[i] = 0.L;
 fpar_f(num, 0., tes, tes, tes);
 free(tes);
 return !err[num];
}

int fpar_function(int num, int nvars, char* tab)
{
 if (!tab) return 0;
 if (buffer[num]) { free(buffer[num]); buffer[num] = 0; }
 buffer[num] = (char*)malloc(strlen(tab)+2);
 if (!buffer[num]) return 1;
 strcpy(buffer[num],tab);
 buffer[num][strlen(buffer[num])]   = 10;
 buffer[num][strlen(buffer[num])+1] = 0;
 maxpos[num] = strlen(buffer[num]);
 nvar[num] = nvars;
 y[num] = (real*)malloc(nvars*sizeof(real));
 yp[num] = (real*)malloc(nvars*sizeof(real));
 ypp[num] = (real*)malloc(nvars*sizeof(real));
 return 1;
}

void fpar_free(int num)
{
 if (buffer[num]) { free(buffer[num]); buffer[num] = 0; }
 if (y[num]) { free(y[num]); y[num] = 0; }
 if (yp[num]) { free(yp[num]); yp[num] = 0; }
 if (ypp[num]) { free(ypp[num]); ypp[num] = 0; }
}

void fpar_deinit()
{
 int i;
 for (i=0;i<N;i++) fpar_free(i);
 free(buffer);	 buffer = NULL;
 free(position); position = NULL;	
 free(ch);	 ch = NULL;
 free(maxpos);	 maxpos = NULL;
 free(t);	 t = NULL;
 free(y);	 y = NULL;
 free(yp);	 yp = NULL;
 free(ypp);	 ypp = NULL;
 free(err);	 err = NULL;
 free(nvar);	 nvar = NULL;
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
    while (isalpha(ch[num]) || isdigit(ch[num]) || ch[num] == '\'' || ch[num] == '\"')
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
 int ynum;
 char ident[ID_LEN],c,last;
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
    last = ident[strlen(ident)-1];
    if (!strcmp(ident, "x") || !strcmp(ident, "t") || !strcmp(ident, "a")) f = t[num];
    else if (sscanf(ident, "y%d", &ynum) == 1 && last == '\"') 
      {
/*	  printf("y'\n");*/
       if (nvar[num] < ynum) exception(num, "found yppi in definition, but i > numer_of_args\n");
       else if (ynum <= 0) exception(num, "found yppi, but i <= 0\n");
       else f = ypp[num][ynum-1];
      }
    else if (sscanf(ident, "y%d", &ynum) == 1 && last == '\'') 
      {
/*	  printf("y'\n");*/
       if (nvar[num] < ynum) exception(num, "found ypi in definition, but i > numer_of_args\n");
       else if (ynum <= 0) exception(num, "found ypi, but i <= 0\n");
       else f = yp[num][ynum-1];
      }
    else if (sscanf(ident, "y%d", &ynum) == 1) 
      {
/*	  printf("y\n");*/
       if (nvar[num] < ynum) exception(num, "found yi in definition, but i > numer_of_args\n");
       else if (ynum <= 0) exception(num, "found yi, but i <= 0\n");
       else f = y[num][ynum-1];
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


real fpar_f(int num, real targ, real* yargs, real* ypargs, real* yppargs)
{
 real e;
 int i,nv;
 t[num] = targ;
 nv = 1;
 if (yargs)
   {
    nv = nvar[num];
    for (i=0;i<nv;i++) y[num][i] = yargs[i];
   }
 else for (i=0;i<nv;i++) y[num][i] = 0.;
 if (ypargs)
   {
    nv = nvar[num];
    for (i=0;i<nv;i++) yp[num][i] = ypargs[i];
   }
 else for (i=0;i<nv;i++) yp[num][i] = 0.;
 if (yppargs)
   {
    nv = nvar[num];
    for (i=0;i<nv;i++) ypp[num][i] = yppargs[i];
   }
 else for (i=0;i<nv;i++) ypp[num][i] = 0.;
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

