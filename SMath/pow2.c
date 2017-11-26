#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

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
 tab = (int**)malloc(min*sizeof(int*));
 for (i=0;i<min;i++) tab[i] = (int*)malloc((max+1)*sizeof(int));
 for (i=0;i<min;i++)
   {
    rest = 0;
    for (j=0;j<max;j++)
      {
       tab[i][j] = (pm[max-j-1]-'0')*(pl[min-i-1]-'0')+rest;
       rest = tab[i][j]/10;
       tab[i][j] %= 10;
      }
    tab[i][max] = rest;
   }
 itmp = max+min;
 part = (int*)malloc(itmp*sizeof(int));
 for (i=0;i<itmp;i++)
  {
   from = (i<max)?0:(i-max);
   to   = (i>=min)?min:i+1;
   sum = 0;
   for (j=from;j<to;j++) sum += tab[j][i-j];
   part[i] = sum;
  }
 for (i=0;i<min;i++) free(tab[i]);
 free(tab);
 for (i=0;i<itmp;i++) if (part[i]>=10)
    {
     part[i+1] += part[i]/10;
     part[i] = part[i]%10;
    }
 sprintf(tmps, "%d", part[itmp-1]);
 j = strlen(tmps);
 sret = (char*)malloc(itmp+j+2);
 strcpy(sret,"");
 for (i=itmp-1;i>=0;i--)
   {
    sprintf(tmps,"%d", part[i]);
    strcat(sret,tmps);
   }
 free(part);
 i=0;
 j=strlen(sret);
 l1=j;
 while (sret[i]=='0') { i++; j--; }
 if (j<=0)
   {
    tmp2 = malloc(2);
    tmp2[0] = '0';
    tmp2[1] = 0;
    return tmp2;
   }
 if (i)
   {
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
 if (!l1 || l2>l1) return NULL;
 if (!l2)
   {
    tmp = (char*)malloc(l1+1);
    for (i=0;i<l1;i++) tmp[i] = p1[i];
    tmp[i] = 0;
    return tmp;
   }
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


void licz_pow2(int pow)
{
 char* tmp;
 char pom[2*1024*1024];
 int i;
 tmp = malloc(2);
 tmp[0] = '1';
 tmp[1] = 0;
 strcpy(pom, tmp);
 for (i=0;i<pow;i++)
 {
  tmp = SADD(pom,pom);
  strcpy(pom, tmp);
  free(tmp);
 }
 printf("%s\n", pom,strlen(pom));
}


int main(int lb, char** par)
{
 if (lb<2) { printf("number required.\n"); return 1; }
 licz_pow2(atoi(par[1]));
 return 0;
}

