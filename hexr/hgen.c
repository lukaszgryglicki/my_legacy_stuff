#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void hgen(int nr)
{
    time_t tm;
    int i, n, j, m;
    char *s1, *s2, tmp[3];

    time(&tm);
    srand((unsigned int)tm);

    n = (rand() % (nr+1)) + nr;
    s1 = (char*)malloc(64);
    s2 = (char*)malloc(64);

    for (i=0;i<n;i++)
    {
	strcpy(s1, "");
	strcpy(s2, "");

	m = 2 + (rand() % 5);
	for (j=0;j<m;j++)
	{
	    sprintf(tmp, "%02x", rand() % 0x100);
	    strcat(s1, tmp);
	}

	m = 1 + (rand() % 6);
	for (j=0;j<m;j++)
	{
	    sprintf(tmp, "%02x", rand() % 0x100);
	    strcat(s2, tmp);
	}

	printf("%s\t%s\n", s1, s2);

    }

}

int main(int lb, char** par)
{
    	if (lb < 2)  hgen(100);
	else hgen(atoi(par[1]));
	return 0;
}

