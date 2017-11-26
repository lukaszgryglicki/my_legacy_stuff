#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int binary_gap(int N)
{
    char brep[32];
    int done, r, l, in, i, n0, mn0;

    strcpy(brep, "");

    done = 0;
    i = 0;
    while (!done)
    {
	r = N % 2;

	if (r) brep[i] = '1';
	else brep[i] = '0';

	i ++;

	N /= 2;
	if (!N) done = 1;
    }
    brep[i] = 0;

    printf("reversed binrep: %s\n", brep);

    l = i;

    in = 0;
    n0 = 0;
    mn0 = 0;
    for (i=0;i<l;i++)
    {
	if (brep[i] == '1' && in == 0) in = 1; 
	else if (brep[i] == '0' && in == 0) ;
	else if (brep[i] == '1' && in == 1) ;
	else if (brep[i] == '0' && in == 1) 
	{
	    in = 2;
	    n0 ++;
	}
	else if (brep[i] == '0' && in == 2) n0 ++;
	else if (brep[i] == '1' && in == 2) 
	{
	    in = 1;
	    if (n0 > mn0) mn0 = n0;
	    printf("gap: %d\n", n0);
	    n0 = 0;
	}
    }

    printf("max gap: %d\n", n0);


    return mn0;
}

int main(int lb, char** par)
{
    int n, ret, gap;
    long long n2;

    if (lb < 2)
    {
	printf("usage: %s number\n", par[0]);
	return 1;
    }

    ret = sscanf(par[1], "%d", &n);

    if (ret != 1)
    {
	printf("%s is not a number\n", par[1]);
	return 2;
    }

    ret = sscanf(par[1], "%lld", &n2);

    if ((unsigned long long)n != n2)
    {
	printf("Value scanned is out of range, expected range: [0,%d], scanned: %lld\n", 0x7fffffff, n2);
	return 3;
    }

    if (n < 0)
    {
	printf("Expected positive number in range: [0-%d], scanned: %d\n", 0x7fffffff, n);
	return 4;
    }


    gap = binary_gap( n );

    printf("%d has binary gap: %d\n", n, gap);

    return 0;
}


