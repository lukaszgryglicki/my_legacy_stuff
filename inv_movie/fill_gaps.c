#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mscan(const char* s, char* s1, int* v, char* s2)
{
    int i, l, n1, n2, r;

    l = strlen(s);

    i = 0;
    while (!(s[i] >= '0' && s[i] <= '9') && i < l) i ++;

    if (i == 0) strcpy(s1, "");
    if (i == l) 
    {
	printf("No number in \"%s\"\n", s);
	return 0;
    }

    strncpy(s1, s, i);
    s1[i] = 0;

/*    printf("s1 = %s\n", s1);*/

    n1 = i;
    while (s[i] >= '0' && s[i] <= '9' && i < l) i++;
    n2 = i;

/*    printf("n1=%d, n2=%d\n", n1, n2);*/

    strncpy(s2, &s[n1], n2 - n1);
    s2[n2-n1] = 0;

/*    printf("s2 = %s\n", s2);*/
    r = sscanf(s2, "%d", v);

    if (r != 1)
    {
       printf("Cannot scan integer value from %s, \"%s\"\n", s2, s);
       return 1;
    }

/*    printf("v = %d\n", *v);*/

   if (n2 == l) 
   {
       strcpy(s2, "");
       return 3;
   }

   strncpy(s2, &s[n2], l - n2);
   s2[l - n2] = 0;

/*   printf("s2 = %s\n", s2);*/

/*   printf("merged: %s%d%s\n", s1, *v, s2);*/

   return 3;
}

int main(int lb, char** par)
{
    int i, n, j, m, k, l, f, ci;
    char tmps1[1024], tmps2[64], tmps3[1024], tmps4[64], scmd[2048];

    ci = 0;
    for (i=1;i<lb;i++)
    {
	ci ++;
	n = mscan(par[i], tmps1, &k, tmps2);
	if (n < 3)
	{
	    printf("Cannot scan number from: %s: %d\n", par[i], n);
	    return 1;
	}

	if (k != ci)
	{
/*	    printf("K=%d, CI=%d\n", k, ci);*/
	    f = 0;
	    for (j=i;j<lb;j++)
	    {
		m = mscan(par[j], tmps3, &l, tmps4);
		if (m < 3) 
		{
	           printf("Cannot scan number from: %s: %d\n", par[j], m);
	           return 1;
		}

		if (l == ci)
		{
		    printf("Skipping the same idx: %d\n", l);
		    continue;
		}

		sprintf(scmd, "cp %s%08d%s %s%08d%s", tmps3, l, tmps4, tmps1, ci, tmps2);
		printf("Copy: %s\n", scmd);
		system(scmd);
		f = 1;
		break;
	    }

	    if (f) 
	    {
		ci ++;
/*		printf("ci=%d, k=%d\n", ci, k);*/
	        if (ci < k) { i--; ci --; }
		continue;
	    }

	    for (j=i-1;j>=0;j--)
	    {
		m = mscan(par[j], tmps3, &l, tmps4);
		if (m < 3) 
		{
	           printf("Cannot scan number from: %s: %d\n", par[j], m);
	           return 1;
		}

		if (l == ci)
		{
		    printf("Skipping the same idx: %d\n", l);
		    continue;
		}

		sprintf(scmd, "cp %s%08d%s %s%08d%s", tmps3, l, tmps4, tmps1, ci, tmps2);
		printf("Copy: %s\n", scmd);
		system(scmd);
		f = 1;
		break;
	    }

	    if (!f)
	    {
		printf("File not found to fill the gap at %d\n", i);
		return 1;
	    }

	    ci ++;
	    if (ci < k) { i--; ci--; }
/*	    printf("ci=%d, k=%d\n", ci, k);*/
	}
    }

    return 0;
}

