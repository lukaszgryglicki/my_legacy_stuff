#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void tyre_comp(
	int d1, int p1, int r1, 
	int d2, int p2, int r2)
{
    double s1, s2, s, p;

    s1 = (double)r1 * 26.5 + (double)p1 / 50. * (double)d1;
    s2 = (double)r2 * 26.5 + (double)p2 / 50. * (double)d2;

    s = fabs(s1 - s2);
    p = ((s2 - s1) / s1) * 100.;

    printf("S1 = %3.1f mm\n", s1);
    printf("S2 = %3.1f mm\n", s2);
    printf("D  = %3.1f mm\n", s);
    printf("D%% = %3.3f %%\n", p);

}

int main(int lb, char** par)
{
    if (lb < 7)
    {
	printf("args: 225 55 17 235 65 17\n");
	return 0;
    }

    tyre_comp(
	    atoi(par[1]), atoi(par[2]), atoi(par[3]), 
	    atoi(par[4]), atoi(par[5]), atoi(par[6]) 
	    );

    return 0;
}
