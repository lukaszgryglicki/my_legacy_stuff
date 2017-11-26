#include "libfparser.h"
#define ID_LEN 32

char* buffer;
int position = 0;
char ch = 0;
int maxpos;
complex<double> term();
complex<double> factor();
complex<double> expression(); 
complex<double> exponential();
complex<double> x, y, z;
static int err=0;
int nvar=1;

void err_exception(char* reason)
{
 printf("Parser Exception: %s\n\n", reason);
 err=1;
}

void fpar_info()
{
 printf("supported functions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt,abs\n"
        "supported functions: asin,acos,asinh,acosh,sinh,cosh,cbrt,ceil,sgn\n"
        "supported functions: tanh,atan,atanh,tgh,atg,atgh\n"
        "supported operators: +,-,*,/,^, unary -\n"
        "blank characters are skipped\n");
}

int fpar_ok()
{
 nvar=1;
 fpar_f(complex<double>(0., 0.));
 return !err;
}

int fpar2_ok()
{
 nvar=2;
 fpar2_f(complex<double>(0., 0.), complex<double>(0., 0.));
 return !err;
}

int fpar3_ok()
{
 nvar=3;
 fpar3_f(complex<double>(0., 0.), complex<double>(0., 0.), complex<double>(0., 0.));
 return !err;
}

int fpar_set_vars(int nvars)
{
 if (nvars < 1 || nvars> 3) return 0;
 nvar = nvars;
 return 0;
}


int fpar_function(char* tab)
{
 if (!tab) return 1;
 if (buffer) { free(buffer); buffer = 0; }
 buffer = (char*)malloc(strlen(tab)+2);
 if (!buffer) return 1;
 strcpy(buffer,tab);
 buffer[strlen(buffer)]   = 10;
 buffer[strlen(tab)+1] = 0;
 maxpos = (int)strlen(buffer);
 return 0;
}

void fpar_free()
{
 if (buffer) { free(buffer); buffer = 0; }
}


void read_next_char()
{
 if (position < maxpos && ch != 10) ch = buffer[position++];
}

void skipblanks()
{
 if (ch!=10)
  {
   while (isspace(ch) && ch!=10) read_next_char();
  }
}

void read_id(char *ident)
{
 int cnt=0;
 skipblanks();
 if (isalpha(ch))
   {
    while (isalpha(ch) || isdigit(ch))
      {
       if (cnt < ID_LEN-1) ident[cnt++] = ch;
       read_next_char();
      }
    ident[cnt] = 0;
   }
 else err_exception("Expected: function name or variable.\n");
}

complex<double> factor()
{
 complex<double> f(0., 0.), minus(-1., 0.),tmp(0., 0.);
 char ident[ID_LEN],c;
 read_next_char();
 skipblanks();
 while (ch=='+' || ch=='-')
   {
     if (ch == '-') minus = minus * complex<double>(-1, 0.);
     read_next_char();
   }
 if (isdigit(ch) || ch=='.')
   {
    buffer[--position] = ch;
    sscanf(buffer+position, "%lf%c", &f, &ch);
    c=ch;
    do read_next_char();
    while (ch != c);
   }
 else if (ch == '(')
   {
    f = expression();
    skipblanks();
    if (ch == ')') read_next_char();
    else err_exception("Expected: '('.\n");
   }
 else
   {
    read_id(ident);
    if (!strcmp(ident, "x")) f = x;
    else if (!strcmp(ident, "y")) 
      {
       if (nvar < 2) err_exception("'y' found in one variable function.\n");
       else f = y;
      }
    else if (!strcmp(ident, "z")) 
      {
       if (nvar < 3) err_exception("'z' found in one or two variable function.\n");
       else f = z;
      }
	else if (!strcmp(ident, "xi")) f = complex<double>(0., x.imag());
    else if (!strcmp(ident, "yi")) 
      {
       if (nvar < 2) err_exception("'y' found in one variable function.\n");
       else f = complex<double>(0., y.imag());
      }
    else if (!strcmp(ident, "zi")) 
      {
       if (nvar < 3) err_exception("'z' found in one or two variable function.\n");
       else f = complex<double>(0., z.imag());
      }
	else if (!strcmp(ident, "xr")) f = complex<double>(x.real(), 0.);
    else if (!strcmp(ident, "yr")) 
      {
       if (nvar < 2) err_exception("'y' found in one variable function.\n");
	   else f = complex<double>(y.real(), 0.);
      }
    else if (!strcmp(ident, "zr")) 
      {
       if (nvar < 3) err_exception("'z' found in one or two variable function.\n");
	   else f = complex<double>(z.real(), 0.);
      }
    else if (!strcmp(ident, "sin"))
      {
	skipblanks();
	if (ch=='(')
	  {
	   buffer[--position] = ch;
	   f = sin(exponential());
	  }
	else err_exception("Expected: '(' after sin.\n");
      }
     else if (!strcmp(ident, "cos"))
      {
       skipblanks();
       if (ch=='(')
	  {
	   buffer[--position] = ch;
	   f = cos(exponential());
	  }
       else err_exception("Expected: '(' after cos.\n");
      }
    else if (!strcmp(ident, "sinh"))
      {
	skipblanks();
	if (ch=='(')
	  {
	   buffer[--position] = ch;
	   f = sinh(exponential());
	  }
	else err_exception("Expected: '(' after sinh.\n");
      }
     else if (!strcmp(ident, "cosh"))
      {
       skipblanks();
       if (ch=='(')
	  {
	   buffer[--position] = ch;
	   f = cosh(exponential());
	  }
       else err_exception("Expected: '(' after cosh.\n");
      }
     else if (!strcmp(ident, "acos"))
      {
       skipblanks();
       if (ch=='(')
	  {
	   buffer[--position] = ch;
	   //f = acos(exponential());
	   f = complex<double>(acos(exponential().real()), 0.);
	  }
       else err_exception("Expected: '(' after acos.\n");
      }
     else if (!strcmp(ident, "asin"))
      {
       skipblanks();
       if (ch=='(')
	  {
	   buffer[--position] = ch;
	   //f = asin(exponential());
	   f = complex<double>(asin(exponential().real()), 0.);
	  }
       else err_exception("Expected: '(' after asin.\n");
      }

     else if (!strcmp(ident, "tg") || !strcmp(ident, "tan"))
      {
       skipblanks();
       if (ch=='(')
	 {
	  buffer[--position] = ch;
	  f = tan(exponential());
	 }
        else err_exception("Expected: '(' after tg or tan.\n");
       }
     else if (!strcmp(ident, "atg") || !strcmp(ident, "atan"))
      {
       skipblanks();
       if (ch=='(')
	 {
	  buffer[--position] = ch;
	  //f = atan(exponential());
	  f = complex<double>(atan(exponential().real()), 0.);
	 }
        else err_exception("Expected: '(' after atg or atan.\n");
       }
     else if (!strcmp(ident, "tgh") || !strcmp(ident, "tanh"))
      {
       skipblanks();
       if (ch=='(')
	 {
	  buffer[--position] = ch;
	  f = tanh(exponential());
	 }
        else err_exception("Expected: '(' after tgh or tanh.\n");
       }

     else if (!strcmp(ident, "ctg") || !strcmp(ident,"ctan"))
       {
	skipblanks();
	if (ch=='(')
	  {
	   buffer[--position] = ch;
	   f = 1. / tan(exponential());
	  }
	else err_exception("Expected: '(' after ctg or ctan.\n");
       }
     else if (!strcmp(ident, "exp") || !strcmp(ident,"e"))
       {
	skipblanks();
	if (ch=='(')
	  {
	   buffer[--position] = ch;
	   f = exp(exponential());
	  }
	else err_exception("Expected: '(' after exp or e.\n");
	}
	else if (!strcmp(ident, "ln"))
	  {
	   skipblanks();
	   if (ch=='(')
	     {
	      buffer[--position] = ch;
	      f = log(exponential());
	     }
	   else err_exception("Expected: '(' after ln.\n");
	  }
	else if (!strcmp(ident, "log"))
	  {
	   skipblanks();
	   if (ch=='(')
	     {
	      buffer[--position] = ch;
	      f = log10(exponential());
	     }
	   else err_exception("Expected: '(' after log.\n");
	  }
	else if (!strcmp(ident, "sqrt"))
	  {
	   skipblanks();
	   if (ch=='(')
	     {
	      buffer[--position] = ch;
	      f = sqrt(exponential());
	     }
	   else err_exception("Expected: '(' after sqrt.\n");
	  }
	else if (!strcmp(ident, "ceil"))
	  {
	   skipblanks();
	   if (ch=='(')
	     {
	      buffer[--position] = ch;
		  double dtr = ceil(exponential().real());
		  double dti = ceil(exponential().imag());
	      f = complex<double>(dtr, dti);
	     }
	   else err_exception("Expected: '(' after ceil.\n");
	  }
	else if (!strcmp(ident, "sgn"))
	  {
	   skipblanks();
	   if (ch=='(')
	     {
	      buffer[--position] = ch;
	      tmp = exponential();
		  if (tmp.real() > 0.0) f = complex<double>(1., 0.);
		  else if (tmp.real() < 0.0) f = complex<double>(-1., 0.);
	      else f = complex<double>(0., 0.);
	     }
	   else err_exception("Expected: '(' after sgn.\n");
	  }
	else if (!strcmp(ident, "abs"))
	  {
	   skipblanks();
	   if (ch=='(')
	     {
	      buffer[--position] = ch;
	      f = abs(exponential());
	     }
	   else err_exception("Expected: '(' after abs.\n");
	  }
	else err_exception("Unknown identifier.\n");
	}
	skipblanks();
	return minus*f;		
}

complex<double> term()
{
 complex<double> f1;
 f1 = exponential();
 while(1)
   {
    switch(ch)
      {
       case '*': f1 = f1 * exponential(); break;
       case '/': f1 = f1 / exponential(); break;
       default: return f1;
      }
   }
}

complex<double> expression()
{
 complex<double> t1;
 t1 = term();
 while(1)
   {
    switch(ch)
      {
       case '+': t1 = t1 + term(); break;
       case '-': t1 = t1 - term(); break;
       default: return t1;
      }
   }
}

complex<double> exponential()
{
 complex<double> f = factor();
 while (ch == '^') f = pow(f, exponential());
 return f;
}


complex<double> fpar_f(complex<double> X)
{
 complex<double> e;
 x=X;
 y=complex<double>(0., 0.);
 z=complex<double>(0., 0.);
 position=0;
 ch=0;
 e=expression();
 if (ch != 10 && ch != ';') err_exception("Garbage in function expression.\n");
 if (err) 
   {
    err_exception("Value returned MAY BE invalid, err_exceptions occured.\n");
    return e;
   }
 return e;
}

complex<double> fpar2_f(complex<double> X, complex<double> Y)
{
 complex<double> e;
 x=X;
 y=Y;
 z=complex<double>(0., 0.);
 position=0;
 ch=0;
 e=expression();
 if (ch != 10 && ch != ';') err_exception("Garbage in function expression.\n");
 if (err) 
   {
    err_exception("Value returned MAY BE invalid, err_exceptions occured.\n");
    return e;
   }
 return e;
}

complex<double> fpar3_f(complex<double> X, complex<double> Y, complex<double> Z)
{
 complex<double> e;
 x=X;
 y=Y;
 z=Z;
 position=0;
 ch=0;
 e=expression();
 if (ch != 10 && ch != ';') err_exception("Garbage in function expression.\n");
 if (err) 
   {
    err_exception("Value returned MAY BE invalid, err_exceptions occured.\n");
    return e;
   }
 return e;
}

