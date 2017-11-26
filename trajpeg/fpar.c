#include "fpar.h"
#define ID_LEN 32
#define THIS_LINE	__FILE__,__LINE__

static unsigned long long gcnt;
char** buffer;			/* definintions[nbuf] */
int* position;			/* positions in buffer[nbuf] */
char* ch;			/* read chars[nbuf] */
int* maxpos;			/* lengths[nbuf] (of defs) */

real term();			/* parsing routines */
real factor();
real expression(); 
real exponential();
real ***y;		/* VALUES ! [nfunc][ranks][nvars] */
real *t;		/* TIME for equations [ntab] */
int* err;		/* errors [nbuf] (occured ?) */
int* nvar;		/* number of variables in each function [nbuf] */
int* ranks;		/* ranks of each function[nfunc] (how many */
int N;			/* (derivatives to comute), N-ntab */

int fpar_init(int ntab)	/* initializes ntab functions of 1 variable and 1st ranks */
{
 int i;
 if (ntab < 0) return 0;
 buffer = (char**)malloc(ntab*sizeof(char*));
 if (!buffer) return 0;
 
 position = (int*)malloc(ntab*sizeof(int));
 ch = (char*)malloc(ntab*sizeof(char));
 maxpos = (int*)malloc(ntab*sizeof(int));
 y = (real***)malloc(ntab*sizeof(real**));
 err = (int*)malloc(ntab*sizeof(int));
 nvar = (int*)malloc(ntab*sizeof(int));
 ranks = (int*)malloc(ntab*sizeof(int));
 t = (real*)malloc(ntab*sizeof(real));
 N = ntab;
 for (i=0;i<N;i++)
   {
    buffer[i] = NULL;
    position[i] = 0;
    err[i] = 0;
    nvar[i] = 1;
    ranks[i] = 1;
    y[i] = NULL;
   }
 gcnt = 0;
 return 1;
}

void exception(char* fn, int ln, int num, char* reason)
{
 printf("Parser Exception: %s\n\n", reason);
 printf("Buffer[%d] = '%s'\n", num, buffer[num]);
 printf("Occured at: File: %s, Line: %d\n", fn, ln);
 err[num]=1;
}

void fpar_info()
{
 printf("supported functions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt,abs\n"
        "supported functions: asin,acos,asinh,acosh,sinh,cosh,cbrt,ceil,sgn\n"
        "supported functions: tanh,atan,atanh,tgh,atg,atgh,neg\n"
        "supported bin-functions: max,min,mina,minb,maxa,maxb,and,or,xor,pow,mod\n"
        "supported tri-functions: if(cond,cond>0,cond<=0)\n"
        "supported rgb-functions: gr,gg,gb,sr,sg,sb,ggs,sgs\n");
 printf("supported operators: +,-,*,/,^, unary -\n"
	"supported user defined function: asmf[123](lx), udf1(x), udf2(x,y), udf3(x,y,z)\n"
	"and more...(see udf.c), special \"doing nothing\" none\n");
 printf("blank characters are skipped\n");
}

int fpar_ok(int num)
{
 real** tes;
 int i, j, nv, rn;
 nv = nvar[num];
 rn = ranks[num];
 tes = (real**)malloc(rn*sizeof(real*));
 for (i=0;i<rn;i++) 
   {
    tes[i] = (real*)malloc(nv*sizeof(real));
    for (j=0;j<nv;j++) tes[i][j] = (real)(i * j);
   }
 fpar_f(num, 1., tes);
 			/* num - which function
			 1. - time value
			 tes [ranks][nvars] - values of 
			 all rankss of each variable */
 free(tes);
 return !err[num];
}
			/* num - number in [N]
			 nvars - number of variables
			 nranks - desired ranks of function
			 tab definition */
int fpar_function(int num, int nvars, int nranks, char* tab)
{
 int i;
 if (!tab) return 0;
 if (buffer[num]) { free(buffer[num]); buffer[num] = 0; }
 buffer[num] = (char*)malloc(strlen(tab)+2);
 if (!buffer[num]) return 0;
 strcpy(buffer[num],tab);
 buffer[num][strlen(buffer[num])]   = 10;
 buffer[num][strlen(buffer[num])+1] = 0;
 maxpos[num] = strlen(buffer[num]);
 nvar[num] = nvars;
 nranks ++; 		/* adding function self */
 ranks[num] = nranks;  /* derivatives plus function itself */
 y[num] = (real**)malloc(nranks*sizeof(real*));
 for (i=0;i<nranks;i++) y[num][i] = (real*)malloc(nvars*sizeof(real));
 return 1;
}

		/* free num function: its vars and rankss */
void fpar_free(int num)
{
 int i;
 if (buffer[num]) { free(buffer[num]); buffer[num] = 0; }
 for (i=0;i<ranks[num];i++)
   {
    if (y[num][i]) { free(y[num][i]); y[num][i] = 0; }
   }
 if (y[num]) { free(y[num]); y[num] = 0; }
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
 free(err);	 err = NULL;
 free(nvar);	 nvar = NULL;
 free(ranks);	 ranks = NULL;
 printf("Internal parser calls: %lld\n", gcnt);
 gcnt = 0;
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
    while (isalpha(ch[num]) || isdigit(ch[num]) || ch[num] == '\'' || ch[num] == '_')
      {
       if (cnt < ID_LEN-1) ident[cnt++] = ch[num];
       read_next_char(num);
      }
    ident[cnt] = 0;
   }
 else exception(THIS_LINE, num, "Expected: function name or variable.\n");
 gcnt ++;
}

real factor(int num)
{
 real f, minus, tmp, t1, t2, t3;
 int ynum, rnum;
 char ident[ID_LEN],c,last;
 unsigned long utmp, utmp2, utmp3, utmp4;
 minus = 1.0;
 gcnt ++;
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
/*    sscanf(buffer[num]+position[num], "%Lf%c", &f, &ch[num]);*/
    sscanf(buffer[num]+position[num], "%lf%c", &f, &ch[num]);
    c=ch[num];
    do read_next_char(num);
    while (ch[num] != c);
   }
 else if (ch[num] == '(' || ch[num] == ',')	/* expects opening pars or , as pars separator */
   {
    f = expression(num);
    skipblanks(num);
/*    printf("ch[num] = '%c' %x\n", ch[num], ch[num]);*/
    if (ch[num] == ')') read_next_char(num);
    else if (ch[num] == ',') 			/* like ')' closes this one param */
    {
/*	printf("GOT ,\n");*/
	return minus*f;
    }
    else exception(THIS_LINE, num, "Expected: ')'.\n");
   }
 else
   {
    read_id(num, ident);
    last = ident[strlen(ident)-1];
    if (!strcmp(ident, "x") || !strcmp(ident, "t") || !strcmp(ident, "a")) f = t[num];
    else if (sscanf(ident, "y%d_%d", &ynum, &rnum) == 2 && last == '\'') 
      {
       if (nvar[num] < ynum) exception(THIS_LINE, num, "found y in definition, but i > numer_of_args\n");
       else if (ynum <= 0) exception(THIS_LINE, num, "found y, but i <= 0\n");
       else if (ranks[num] <= rnum) exception(THIS_LINE, num, "found y in definition, but yranks > ranks of this\n");
       else if (rnum < 0) exception(THIS_LINE, num, "found y, but ranks < 0\n");
       else f = y[num][rnum][ynum-1];
      }
    else if (sscanf(ident, "y%d", &ynum) == 1 && last != '\'') 
      {
       rnum = 0;
       if (nvar[num] < ynum) exception(THIS_LINE, num, "found y in definition, but i > numer_of_args\n");
       else if (ynum <= 0) exception(THIS_LINE, num, "found y, but i <= 0\n");
       else if (ranks[num] <= rnum) exception(THIS_LINE, num, "found y in definition, but yranks > ranks of this\n");
       else f = y[num][rnum][ynum-1];
      }
    else if (!strcmp(ident, "pi"))
      {
	skipblanks(num);
	f = M_PI;
      }
    else if (!strcmp(ident, "sin"))
      {
	skipblanks(num);
	if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = sin(exponential(num));
	  }
	else exception(THIS_LINE, num, "Expected: '(' after sin.\n");
      }
     else if (!strcmp(ident, "cos"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = cos(exponential(num));
	  }
       else exception(THIS_LINE, num, "Expected: '(' after cos.\n");
      }
    else if (!strcmp(ident, "sinh"))
      {
	skipblanks(num);
	if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = sinh(exponential(num));
	  }
	else exception(THIS_LINE, num, "Expected: '(' after sinh.\n");
      }
     else if (!strcmp(ident, "cosh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = cosh(exponential(num));
	  }
       else exception(THIS_LINE, num, "Expected: '(' after cosh.\n");
      }
     else if (!strcmp(ident, "acos"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = acos(exponential(num));
	  }
       else exception(THIS_LINE, num, "Expected: '(' after acos.\n");
      }
     else if (!strcmp(ident, "asin"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = asin(exponential(num));
	  }
       else exception(THIS_LINE, num, "Expected: '(' after asin.\n");
      }
     else if (!strcmp(ident, "asinh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = asinh(exponential(num));
	  }
       else exception(THIS_LINE, num, "Expected: '(' after asinh.\n");
      }
     else if (!strcmp(ident, "acosh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = acosh(exponential(num));
	  }
       else exception(THIS_LINE, num, "Expected: '(' after acosh.\n");
      }
     else if (!strcmp(ident, "tg") || !strcmp(ident, "tan"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	 {
	  buffer[num][--position[num]] = ch[num];
	  f = tan(exponential(num));
	 }
        else exception(THIS_LINE, num, "Expected: '(' after tg or tan.\n");
       }
     else if (!strcmp(ident, "atg") || !strcmp(ident, "atan"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	 {
	  buffer[num][--position[num]] = ch[num];
	  f = atan(exponential(num));
	 }
        else exception(THIS_LINE, num, "Expected: '(' after atg or atan.\n");
       }
     else if (!strcmp(ident, "tgh") || !strcmp(ident, "tanh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	 {
	  buffer[num][--position[num]] = ch[num];
	  f = tanh(exponential(num));
	 }
        else exception(THIS_LINE, num, "Expected: '(' after tgh or tanh.\n");
       }
     else if (!strcmp(ident, "atgh") || !strcmp(ident, "atanh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
	 {
	  buffer[num][--position[num]] = ch[num];
	  f = atanh(exponential(num));
	 }
        else exception(THIS_LINE, num, "Expected: '(' after atgh or atanh.\n");
       }
     else if (!strcmp(ident, "ctg") || !strcmp(ident,"ctan"))
       {
	skipblanks(num);
	if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = 1.0/tan(exponential(num));
	  }
	else exception(THIS_LINE, num, "Expected: '(' after ctg or ctan.\n");
       }
     else if (!strcmp(ident, "exp") || !strcmp(ident,"e"))
       {
	skipblanks(num);
	if (ch[num]=='(')
	  {
	   buffer[num][--position[num]] = ch[num];
	   f = exp(exponential(num));
	  }
	else exception(THIS_LINE, num, "Expected: '(' after exp or e.\n");
	}
	else if (!strcmp(ident, "ln"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = log(exponential(num));
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after ln.\n");
	  }
	else if (!strcmp(ident, "log"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = log10(exponential(num));
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after log.\n");
	  }
	else if (!strcmp(ident, "sqrt"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = sqrt(exponential(num));
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after sqrt.\n");
	  }
	else if (!strcmp(ident, "cbrt"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = cbrt(exponential(num));
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after cbrt.\n");
	  }
	else if (!strcmp(ident, "ceil"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = ceil(exponential(num));
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after ceil.\n");
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
	   else exception(THIS_LINE, num, "Expected: '(' after sgn.\n");
	  }
	else if (!strcmp(ident, "abs"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = fabs(exponential(num));
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after abs.\n");
	  }
	else if (!strcmp(ident, "udf1"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      f = udf1(exponential(num));
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after udf1.\n");
	  }
	else if (!strcmp(ident, "gb"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp = (unsigned long)exponential(num);
	      utmp &= 0xFF;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after gb.\n");
	  }
	else if (!strcmp(ident, "gg"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp = (unsigned long)exponential(num);
	      utmp = (utmp & 0xFF00) >> 0x8;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after gg.\n");
	  }
	else if (!strcmp(ident, "gr"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp = (unsigned long)exponential(num);
	      utmp = (utmp & 0xFF0000) >> 0x10;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after gr.\n");
	  }
	else if (!strcmp(ident, "sb"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp = (unsigned long)exponential(num);
	      utmp &= 0xFF;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after sb.\n");
	  }
	else if (!strcmp(ident, "sg"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp = (unsigned long)exponential(num);
	      utmp = (utmp & 0xFF) << 0x8;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after sg.\n");
	  }
	else if (!strcmp(ident, "sr"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp = (unsigned long)exponential(num);
	      utmp = (utmp & 0xFF) << 0x10;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after sr.\n");
	  }
	else if (!strcmp(ident, "sgs"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp2 = (unsigned long)exponential(num);
	      utmp2 = (utmp2 & 0xFF);
	      utmp = utmp2 + (utmp2 << 8) + (utmp2 << 0x10);
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after sgs.\n");
	  }
	else if (!strcmp(ident, "ggs"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp = (unsigned long)exponential(num);
	      utmp2 = utmp & 0xFF;
	      utmp3 = (utmp & 0xFF00) >> 0x8;
	      utmp4 = (utmp & 0xFF0000) >> 0x10;
	      utmp = (utmp2 + utmp3 + utmp4) / 3;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after ggs.\n");
	  }
	else if (!strcmp(ident, "neg"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      utmp = (unsigned long)exponential(num);
	      utmp = ~utmp;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after neg.\n");
	  }
	else if (!strcmp(ident, "pow"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in pow(..., ...).\n"); 

	      f = pow(t1, t2);
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after pow.\n");
	  }
	else if (!strcmp(ident, "udf2"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in udf2(..., ...).\n"); 

	      f = udf2(t1, t2);
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after max.\n");
	  }
	else if (!strcmp(ident, "max"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in max(..., ...).\n"); 

	      f = (t1 >  t2) ? t1 : t2;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after max.\n");
	  }
	else if (!strcmp(ident, "min"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in min(..., ...).\n"); 

	      f = (t1 < t2) ? t1 : t2;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after min.\n");
	  }
	else if (!strcmp(ident, "mina"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in mina(..., ...).\n"); 

	      f = t2;
	      if (f < t1) f = t1;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after mina.\n");
	  }
	else if (!strcmp(ident, "minb"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in minb(..., ...).\n"); 

	      f = t1;
	      if (f < t2) f = t2;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after minb.\n");
	  }
	else if (!strcmp(ident, "maxa"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in maxa(..., ...).\n"); 

	      f = t2;
	      if (f > t1) f = t1;

	     }
	   else exception(THIS_LINE, num, "Expected: '(' after maxa.\n");
	  }
	else if (!strcmp(ident, "maxb"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in maxb(..., ...).\n"); 

	      f = t1;
	      if (f > t2) f = t2;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after maxb.\n");
	  }
	else if (!strcmp(ident, "mod"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in mod(..., ...).\n"); 

	      utmp = (unsigned long)t1;
	      utmp2 = (unsigned long)t2;

	      utmp = utmp % utmp2;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after and.\n");
	  }
	else if (!strcmp(ident, "and"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in and(..., ...).\n"); 

	      utmp = (unsigned long)t1;
	      utmp2 = (unsigned long)t2;

	      utmp = utmp & utmp2;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after and.\n");
	  }
	else if (!strcmp(ident, "or"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in or(..., ...).\n"); 

	      utmp = (unsigned long)t1;
	      utmp2 = (unsigned long)t2;

	      utmp = utmp | utmp2;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after or.\n");
	  }
	else if (!strcmp(ident, "xor"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      skipblanks(num);

	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: ',' in xor(..., ...).\n"); 

	      utmp = (unsigned long)t1;
	      utmp2 = (unsigned long)t2;

	      utmp = utmp ^ utmp2;
	      f = (double)utmp;
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after xor.\n");
	  }
	else if (!strcmp(ident, "if"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      t3 = t1;

	      skipblanks(num);
	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: 1st ',' in if(..., ..., ...).\n"); 

	      skipblanks(num);
	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t3 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: 2nd ',' in if(..., ..., ...).\n"); 

	      if (t1 > 0.) f = t2;
	      else f = t3;

	     }
	   else exception(THIS_LINE, num, "Expected: '(' after max.\n");
	  }
	else if (!strcmp(ident, "udf3"))
	  {
	   skipblanks(num);
	   if (ch[num]=='(')
	     {
	      buffer[num][--position[num]] = ch[num];
	      t1 = expression(num);
	      t2 = t1;
	      t3 = t1;

	      skipblanks(num);
	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t2 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: 1st ',' in udf3(..., ..., ...).\n"); 

	      skipblanks(num);
	      if (ch[num] == ',') 
	      {
	          skipblanks(num);
	          buffer[num][--position[num]] = ch[num];
		  t3 = expression(num); 
	      }
	      else exception(THIS_LINE, num, "Expected: 2nd ',' in udf3(..., ..., ...).\n"); 

	      f = udf3(t1, t2, t3);
	     }
	   else exception(THIS_LINE, num, "Expected: '(' after max.\n");
	  }
	else exception(THIS_LINE, num, "Unknown identifier.\n");
	}
	skipblanks(num);
	return minus*f;		
}

real term(int num)
{
 real f1;
 gcnt ++;
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
 gcnt ++;
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
 real f;
 gcnt ++;
 f = factor(num);
 while (ch[num] == '^') f = pow(f, exponential(num));
 return f;
}


real fpar_f(int num, real targ, real** yargs)
{
 real e;
 int i,j,nv,rn;
 gcnt ++;
 t[num] = targ;
 nv = 1;
 if (yargs)
   {
    nv = nvar[num];
    rn = ranks[num];
    for (i=0;i<rn;i++) for (j=0;j<nv;j++) y[num][i][j] = yargs[i][j];
   }
 else for (i=0;i<nv;i++) for (j=0;j<nv;j++)  y[num][i][j] = 0.;
 
 position[num]=0;
 ch[num]=0;
 e=expression(num);
 if (ch[num] != 10 && ch[num] != ';') exception(THIS_LINE, num, "Garbage in function expression.\n");
 if (err[num]) 
   {
    exception(THIS_LINE, num, "Value returned MAY BE invalid, exceptions occured.\n");
    return e;
   }
 return e;
}

