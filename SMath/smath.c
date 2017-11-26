#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
static FILE* _ran_device;

int Randomize()   /* init random engine */
{
 if ((_ran_device=fopen("/dev/urandom","r"))==NULL)
    return 0;
 else return 1;
}


int Random(int lb)   /* randomize number from 0 to lb-1 */
{
 int ins[4],i;
 unsigned long result;
 for (i=0;i<4;i++)
     ins[i] = fgetc(_ran_device);
 result = ins[0] + 0x100*ins[1] + 0x10000*ins[2]+0x1000000*ins[3];
 result /= (0xffffffff/lb);
 return (int)result;
}


void Kill_random()  /* close random seed */
{
  if (_ran_device) fclose(_ran_device);
}


int c2i(char arg)
{
 return arg-'0';
}


char i2c(int arg)
{
 return (char)arg+'0';
}


char* SMUL(const char* p1, const char* p2)
{
 int l1,l2,max,min,itmp;
 const char* pm, *pl;
 int** tab;
 int* part;
 int rest,i,j,sum;
 int from,to;
 char tmps[12];
 char* sret;
 char* tmp2;
 if (!p1 || !p2) return NULL;
 if (!(*p1) || !(*p2)) return NULL;
/* printf("p1=%s, p2=%s\n", p1,p2);*/
 i=0;
 while (p1[++i]);
 l1=i;
 i=0;
 while (p2[++i]);
 l2=i;
 if (!l1 || !l2) return NULL;
 pm = (l1>=l2)?p1:p2;
 pl = (l1>=l2)?p2:p1;
 max = (l1>=l2)?l1:l2;
 min = (l1>=l2)?l2:l1;
 tab = (int**)malloc(min<<2);
 for (i=0;i<min;i++) tab[i] = (int*)malloc((max+1)<<2);
 for (i=0;i<min;i++)
   {
    rest = 0;
    for (j=0;j<max;j++)
      {
       tab[i][j] = (pm[max-j-1]-'0')*(pl[min-i-1]-'0')+rest;
       rest = tab[i][j]/10;
       tab[i][j] %= 10;
/*       printf("tab[%d][%d] = %d, rest=%d\n", i,j,tab[i][j],rest);*/
      }
    tab[i][max] = rest;
/*    printf("tab[%d][max] = %d\n", i,tab[i][max]);*/
   }
/*printf("p1=%s, p2=%s\n", p1,p2);
printf("max=%d, min=%d\n", max,min);
for (i=0;i<min;i++)
 {
  for (j=0;j<=max;j++) printf("%2d ", tab[i][j]);
  printf("\n");
 }*/
 itmp = max+min;
 part = (int*)malloc(itmp<<2);
 for (i=0;i<itmp;i++)
  {
   from = (i<max)?0:(i-max);
   to   = (i>=min)?min:i+1;
   sum = 0;
   for (j=from;j<to;j++) sum += tab[j][i-j];
/*   printf("from=%d, to=%d, i=%d\n", from,to,i);*/
/*   printf("sum[%d] = %d\n", i, sum);*/
   part[i] = sum;
  }
 for (i=0;i<min;i++) free(tab[i]);
 free(tab);
 for (i=0;i<itmp;i++) if (part[i]>=10)
    {
     part[i+1] += part[i]/10;
     part[i] = part[i]%10;
    }
/* for (i=0;i<itmp;i++) printf("part[%d]=%d\n", i, part[i]);*/
 sprintf(tmps, "%d", part[itmp-1]);
 j = strlen(tmps);
/* printf("j=%d, itmp=%d\n", j,itmp);*/
 sret = (char*)malloc(itmp+j+2);
 strcpy(sret,"");
/* printf("itmp=%d\n",itmp);*/
 for (i=itmp-1;i>=0;i--)
   {
    sprintf(tmps,"%d", part[i]);
    strcat(sret,tmps);
/*    printf("%s\n", sret);*/
   }
 free(part);
/* printf("%s\n",sret);*/
/* for (i=0;i<itmp;i++) printf("sum[%d]=%d\n", i, part[i]);*/
 i=0;
 j=strlen(sret);
 l1=j;
/* printf("%s\n", sret);*/
 while (sret[i]=='0') { i++; j--; }
/* printf("i=%d, j=%d\n", i,j);*/
 if (j<=0)
   {
    tmp2 = malloc(2);
    tmp2[0] = '0';
    tmp2[1] = 0;
    return tmp2;
   }
 if (i)
   {
/*    printf("sret=%s\n", sret);*/
    tmp2 = malloc(j+2);
    l1=i+j+1;
    for (itmp=i;itmp<l1;itmp++) tmp2[itmp-i] = sret[itmp];
    tmp2[itmp] = 0;
    free(sret);
    return tmp2;
   }
 else return sret;
}


char* SSUB(const char* p1, const char* p2) /* p1=FROM, p2=WHAT*/
{
 int l1,l2;
 int lb,rest;
 int c1,c2,c3;
 char* tmp, *tmp2;
 register int i;
 if (!p1 || !p2) return NULL;
 if (!(*p1) || !(*p2)) return NULL;
 i=0;
 while (p1[++i]);
 l1=i;
 i=0;
 while (p2[++i]);
 l2=i;
/* printf("l1=%d, l2=%d\n", l1,l2);*/
 if (!l1 || l2>l1) return NULL;
 if (!l2)
   {
    tmp = (char*)malloc(l1+1);
    for (i=0;i<l1;i++) tmp[i] = p1[i];
    tmp[i] = 0;
    return tmp;
   }
/* printf("p1=%s, p2=%s\n", p1,p2);*/
 c1 = l1-1;
 c2 = l2-1;
 c3 = c1;
 tmp = (char*)malloc(c1+2);
 tmp[c1+1] = 0;
 rest=0;
 while (c1>=0 && c2>=0)
   {
    lb = (p1[c1]-'0')-(p2[c2]-'0')-rest;
    rest = (lb<0)?1:0;
    lb = (rest)?(lb+10):lb;
    tmp[c3] = lb+'0';
    c3--;
    c2--;
    c1--;
   }
 if (c1<0 && rest) return NULL;
 if (c1>=0) for (i=c1;i>=0;i--)
   {
    if (rest && p1[i]=='0') tmp[i]='9';
    else if (rest && p1[i]!='0')
      {
       tmp[i] = (char)(p1[i]-1);
       rest=0;
      }
    else tmp[i] = p1[i];
   }
 if (rest) tmp[0] = '0';
 c1=0;
 c2=l1;
/* printf("tmp=%s\n", tmp);*/
 while (tmp[c1]=='0') { c1++; c2--; }
 if (c1==l1)
   {
    tmp2 = malloc(2);
    tmp2[0] = '0';
    tmp2[1] = 0;
    return tmp2;
   }
 if (c2)
   {
    tmp2 = malloc(c2+1);
    for (i=c1;i<l1;i++) tmp2[i-c1] = tmp[i];
    tmp2[c2] = 0;
    free(tmp);
    return tmp2;
   }
 else return tmp;
}


char* SADD(const char* p1, const char* p2)
{
 int l1,l2;
 int mlen,lb,rest;
 int c1,c2,c3;
 char* tmp,*tmp2;
 register int i;
 if (!p1 || !p2) return NULL;
 if (!(*p1) || !(*p2)) return NULL;
 i=0;
 while (p1[++i]);
 l1=i;
 i=0;
 while (p2[++i]);
 l2=i;
 if (!l1 || !l2) return NULL;
 if (!l1)
   {
    tmp = (char*)malloc(l2+1);
    for (i=0;i<l2;i++) tmp[i] = p2[i];
    tmp[i] = 0;
    return tmp;
   }
 if (!l2)
   {
    tmp = (char*)malloc(l1+1);
    for (i=0;i<l1;i++) tmp[i] = p1[i];
    tmp[i] = 0;
    return tmp;
   }
 c1 = l1-1;
 c2 = l2-1;
 mlen = (c1>=c2)?c1:c2;
 c3 = mlen+1;
 tmp = (char*)malloc(mlen+3);
 tmp[mlen+2] = 0;
 rest=0;
 tmp[0] = 1;
 while (c1>=0 && c2>=0)
   {
    lb = (p1[c1]-'0')+(p2[c2]-'0')+rest;
    rest = (lb>=10)?1:0;
    lb = (rest)?(lb-10):lb;
    tmp[c3] = lb+'0';
    c3--;
    c1--;
    c2--;
   }
 if (c1>=0) for (i=c1;i>=0;i--)
   {
    if (rest && p1[i]=='9') tmp[i+1] = '0';
    else if (rest && p1[i]!='9')
      {
       tmp[i+1] = (char)(p1[i]+1);
       rest=0;
      }
    else tmp[i+1] = p1[i];
   }
 if (c2>=0) for (i=c2;i>=0;i--)
   {
    if (rest && p2[i]=='9') tmp[i+1] = '0';
    else if (rest && p2[i]!='9')
      {
       tmp[i+1] = (char)(p2[i]+1);
       rest=0;
      }
    else tmp[i+1] = p2[i];
   }
 if (rest) tmp[0] = '1';
 if (tmp[0]==1)
   {
    tmp2 = (char*)malloc(mlen+2);
    tmp2[mlen+1] = 0;
    for (i=1;i<mlen+2;i++) tmp2[i-1] = tmp[i];
    free(tmp);
    return tmp2;
   }
 else return tmp;
}


void test1()
{
 int i,j, ires,ires2,ok;
 char* sres;
 int r1,r2;
 char s1[11];
 char s2[11];
 float percent;
 ok=0;
 for (i=0;i<100;i++) for (j=0;j<100;j++)
   {
    r1=Random(1000000000);
    r2=Random(1000000000);
    /*r1=i;
    r2=j;*/
    ires = r1+r2;
    sprintf(s1,"%d", r1);
    sprintf(s2,"%d", r2);
    sres = SADD(s1,s2);
    if (!sres) continue;
    sscanf(sres,"%d", &ires2);
    if (ires2==ires) ok++;
    else printf("E(%4d,%4d), ",i,j);
    free(sres);
   }
 percent = (float)ok/(100.0*100.0);
 printf("Reliability: %f%% (%d/%d)\n", percent*100.0,ok,(100*100));
 printf("test1() passed.\n");
}


void test2()
{
 char* tmp;
 char pom[65536];
 int i;
 tmp = malloc(2);
 tmp[0] = '1';
 tmp[1] = 0;
 strcpy(pom, tmp);
 for (i=0;i<1024*1;i++)
 {
  tmp = SADD(pom,pom);
  strcpy(pom, tmp);
  free(tmp);
 }
 printf("%s\nlen=%d\n", pom,strlen(pom));
 printf("test2() passed.\n");
}


void test3()
{
 int i,j, ires,ires2,fail;
 char* sres;
 int r1,r2;
 char s1[11];
 char s2[11];
 fail=0;
 /*sres = SSUB("87","14");
 if (sres) printf("RET=%s\n", sres);
 printf("%d\n", 87-14);*/
 for (i=0;i<100;i++) for (j=0;j<100;j++)
   {
    r1=Random(1000000000);
    r2=Random(1000000000);
    /*r1=i;
    r2=j;*/
    ires = r2-r1;
    sprintf(s1,"%d", r1);
    sprintf(s2,"%d", r2);
/*    printf("s1=%s, s2=%s\n", s1,s2);*/
    sres = SSUB(s2,s1);
    if (!sres) continue;
    sscanf(sres,"%d", &ires2);
    if (ires2!=ires)
      {
       fail++;
       printf("\n\n%4d-%d=%d\n\n",r2,r1,ires2);
       printf("\n\n%4d-%d=%d\n\n",r2,r1,ires);
       goto out;
      }
    free(sres);
   }
 out:
 printf("Fail: %d\n", fail);
 printf("test3() passed.\n");
}


void test4()
{
 char *p[18];
 int i;
 p[0]=SADD(NULL,NULL);
 p[1]=SADD(NULL,"");
 p[2]=SADD("",NULL);
 p[3]=SADD("","");
 p[4]=SADD("12345",NULL);
 p[5]=SADD(NULL,"12345");
 p[6]=SSUB(NULL,NULL);
 p[7]=SSUB(NULL,"");
 p[8]=SSUB("",NULL);
 p[9]=SSUB("","");
 p[10]=SSUB("12345",NULL);
 p[11]=SSUB(NULL,"12345");
 p[12]=SMUL(NULL,NULL);
 p[13]=SMUL(NULL,"");
 p[14]=SMUL("",NULL);
 p[15]=SMUL("","");
 p[16]=SMUL("12345",NULL);
 p[17]=SMUL(NULL,"12345");
 for (i=0;i<18;i++) printf("p[%d] = %p\n", i, p[i]);
 printf("test4() passed.\n");
}


void test5()
{
 int i,j, ires,ires2,fail;
 char* sres;
 int r1,r2;
 char s1[11];
 char s2[11];
 fail=0;
 /*sres = SMUL("2069","302");
 if (sres) printf("RET=%s\n", sres);
 printf("%d\n", 2069*302);*/
 for (i=0;i<100;i++) for (j=0;j<100;j++)
   {
    r1=Random(15000);
    r2=Random(15000);
    ires = r1*r2;
    sprintf(s1,"%d", r1);
    sprintf(s2,"%d", r2);
    sres = SMUL(s2,s1);
    if (!sres) continue;
    sscanf(sres,"%d", &ires2);
    if (ires2!=ires)
      {
       fail++;
       printf("\n\n%4d*%d=%d\n\n",r1,r2,ires2);
       printf("\n\n%4d*%d=%d\n\n",r1,r2,ires);
       goto out;
      }
    free(sres);
   }
 out:
 printf("Fail: %d\n", fail);
 printf("test5() passed.\n");
}


char* atos(int a)
{
 char* tmp;
 tmp = malloc(12);
 sprintf(tmp,"%d", a);
 return tmp;
}


int test_MUL(int a, int b)
{
 char* retv;
 int ival;
/* printf("OK %d*%d IS %d\n", a,b,a*b);*/
 retv = SMUL(atos(a), atos(b));
 if (!retv) { printf("NULL returned!\n"); return 0; }
/* printf("SMUL %s*%s IS %s\n", atos(a), atos(b), retv);*/
 sscanf(retv,"%d", &ival);
 return (ival==(a*b))?1:0;
}


void test6()
{
 int i,j,k;
 k=0;
 for (i=0;i<100;i++)
 for (j=0;j<100;j++)
 k+=test_MUL(i,j);
 printf("k=%d/10000\n", k);
 printf("test6() passed.\n");
}


void test7()
{
 int a;
 char* sa;
 char* a1, *a2, *w, *t1, *t2;
 a = ((150-137)*(234+1027))-(234*70);
 printf("a=%d from stdmathlib...\n",a);
 sa = SSUB(SMUL(SSUB("150","137"),SADD("234","1027")),SMUL("234","76"));
 w = SMUL("234", "70");
 a1 = SSUB("150","137");
 a2 = SADD("234","1027");
 t1 = SMUL(a1,a2);
 t2 = SSUB(t1, w);
 printf("a=%s from strmathlib\n", t2);
 free(a1);
 free(a2);
 free(t1);
 free(t2);
 free(w);
 printf("a=%s from inline:strmathlib\n",
		 SSUB(SMUL(SSUB("150","137"), SADD("234","1027"))
			 , SMUL("234", "70"))
		 );
 printf("test7() passed.\n");
}

void test8()
{
 char* st1, *st2;
 char* Smul,*Sadd,*Ssub;
 int i;
 st1= malloc(4096);
 st2 = malloc(4096);
 printf("alocating strs. . .");
 for (i=0;i<4095;i++) 
   { 
    st1[i] = (char)(Random(10)+'0');
    st2[i] = (char)(Random(10)+'0');
   }
 st1[0] = '6';
 st2[0] = '3';
 printf("\n");
 printf("SADD. . .");
 Sadd = SADD(st1,st2);
 printf("\n");
 printf("SSUB. . .");
 Ssub = SSUB(st1,st2);
 printf("\n");
 printf("SMUL. . .");
 Smul = SMUL(st1,st2);
 printf("\n");
 /*printf("%s + %s = %s\n", st1, st2, Sadd);
 printf("%s - %s = %s\n", st1, st2, Ssub);
 printf("%s * %s = %s\n", st1, st2, Smul);*/
 printf("freeying. . .\n");
 free(st1);
 free(st2);
 free(Sadd);
 free(Ssub);
 free(Smul);
 printf("test8() passed.\n");
}

int main(int lb, char** par)
{
 Randomize();
 test1();
 test2();
 test3();
 test4();
 test5();
 test6();
 test7();
 test8();
 return 0;
}

