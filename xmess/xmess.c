#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int lb, char** par)
{
    printf("%s num 'text'\n", par[0]);

    if (lb < 3) return 0;

    long i, n;
    char cmd[1280];
    n = atoi(par[1]);

    for (i=0;i<n;i++)
    {
	sprintf(cmd, "xmessage '%s' &", par[2]);
/*	printf("%s\n", cmd);*/
	system(cmd);
    }

    return 0;

}

