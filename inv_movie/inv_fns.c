#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getnum(char* fn)
{
    FILE* f;
    int n, i;

    f = fopen(fn, "r");
    if (!f)
    {
	printf("Cannot open: %s\n", fn);
	return 0;
    }

    n = 0;
    while (fscanf(f, "./%d.jpeg\n", &i) == 1) n ++;

    fclose(f);

    return n;
}

int main(int lb, char** par)
{
    int i, n;
    char syscmd[64];
    double perc;

    printf("\n");

    n = getnum(par[1]);
/*    printf("n=%d\n", n);*/

    for (i=1;i<=n;i++)
    {
	sprintf(syscmd, "mv %08d.jpeg p_%08d.jpeg", i, (n - i) + 1);
	if (!(i%100)) 
	{
	    perc = ((double)i * 50.) / (double)n;
	    printf("\b\b\b\b\b\b\b\b\b%03.3f%%", perc);
            fflush(stdout);
	}
/*	printf("%s\n", syscmd);*/
	system(syscmd);
    }

    for (i=1;i<=n;i++)
    {
	sprintf(syscmd, "mv p_%08d.jpeg %08d.jpeg", i, i);
	if (!(i%100)) 
	{
	    perc = 50. + ((double)i * 50.) / (double)n;
	    printf("\b\b\b\b\b\b\b\b\b%03.3f%%", perc);
            fflush(stdout);
	}
/*	printf("%s\n", syscmd);*/
	system(syscmd);
    }

    printf("\b\b\b\b\b\b\b\b100.000%%\n");
    fflush(stdout);

    return 0;
}

