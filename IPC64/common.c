#include "common.h"

void push(int q, int val)
{
 char str[11];
 printf("%d\n", val);
 sprintf(str,"%d", val);
 if (mmsgsendtxt(q,str,1)==-1) fatal("mmsgsend");
}

int pop(int q)
{
 char str[100];
 int index;
 if (mmsgrecvtxt(q,str,1,1)==-1) fatal("mmsgrecvtxt");
 printf("str=%s\n", str);
 if (sscanf(str, "%d", &index)<0) fatal("scanf");
 return index;
}
