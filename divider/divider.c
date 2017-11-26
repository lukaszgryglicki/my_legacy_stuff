#include <stdio.h>

int dividend, divisor, r;

int division(int tdividend, int tdivisor)
{
    int q, d;

    printf("call(%d,%d)\n", tdividend, tdivisor);

    q = 1;

    if (tdivisor == tdividend)
    {
	r = 0;
	printf("%d == %d --> r = %d, return 1\n", tdivisor, tdividend, r);
	return 1;
    }
    else if (tdividend < tdivisor)
    {
	r = tdividend;
	printf("%d < %d --> r = %d, return 0\n", tdividend, tdivisor, r);
	return 0;
    }

    while (tdivisor <= tdividend)
    {
	printf("loop into (%d<=%d, q=%d)\n", tdivisor, tdividend, q);
	tdivisor <<= 1;
	q <<= 1;
    }
    printf("after loop (%d>%d, q=%d)\n", tdivisor, tdividend, q);

    tdivisor >>= 1;
    q >>= 1;

    printf("now tdivisor, tdividend, q: %d, %d, %d\n", tdivisor, tdividend, q);

    d = division(tdividend - tdivisor, divisor);

    printf("recursion (%d, %d) returned: %d\n", tdividend - tdivisor, divisor, d);

    q = q + d;
    printf("new q = %d + %d = %d\n", q-d, d, q);
    return q;
}

int main(int lb, char** par)
{
    int d;
    dividend = atoi(par[1]);
    divisor  = atoi(par[2]);

    d = division(dividend, divisor);
    printf("%d / %d = %d;%d\n", dividend, divisor, d, r);
    return 0;
}

