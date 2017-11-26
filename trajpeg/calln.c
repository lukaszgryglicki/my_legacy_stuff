#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int lb, char** par)
{
    int i, j, n, num, np;
    char ext[128];
    char tmp[128];
    char scmd[256];

    if (lb < 3) return 1;

    strcpy(scmd, "");
    np = atoi(par[1]);
    if (np < 0 || np > 1000) np = 0;

    for (i=2;i<lb;i++)
    {
	n = sscanf(par[i], "%d.%s", &num, ext);
	if (n == 2)
	{
	    for (j=0;j<np;j++)
	    {
	        sprintf(tmp, "%08d.%s ", num + j, ext);
	        strcat(scmd, tmp);
	    }
	}
	else 
	{
	    sprintf(tmp, "%s ", par[i]);
	    strcat(scmd, tmp);
	}
    }
/*    printf("%s\n", scmd);*/
    system(scmd);
    return 0;
}

