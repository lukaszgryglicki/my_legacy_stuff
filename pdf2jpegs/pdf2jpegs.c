#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pdf2jpegs(char* fn, int np, int q)
{
    int i, n;
    char syscmd[0x200], nfn[0x100];

    if (np < 1)
    {
	printf("Bad page num: %d\n", np);
	return 1;
    }

    if (q < 0 || q > 100)
    {
	printf("quality not in [0,100]: %d\n", q);
	return 2;
    }

    printf("processing file %s %d pages at %d%% jpeg quality\n", fn, np, q);

    n = strlen(fn);
    for (i=0;i<n;i++)
    {
	if (fn[i] == '.') break;
	nfn[i] = fn[i];
    }

    nfn[i] = 0;
    printf("output root: %s\n", nfn);

    for (i=1;i<=np;i++)
    {
	sprintf(syscmd, "pdf2svg %s %s%d.svg %d", fn, nfn, i, i);
	printf("p%-6d/%-6d(1 of 3): \"%s\"\n", i, np, syscmd);
	system(syscmd);
	sprintf(syscmd, "convert %s%d.svg -quality %d%% %s%08d.jpeg", nfn, i, q, nfn, i);
	printf("p%-6d/%-6d(2 of 3): \"%s\"\n", i, np, syscmd);
	system(syscmd);
	sprintf(syscmd, "rm -f %s%d.svg", nfn, i);
	printf("p%-6d/%-6d(3 of 3): \"%s\"\n", i, np, syscmd);
	system(syscmd);
    }

    return 0;
}

int main(int lb, char** par)
{
    printf("usage: %s file.pdf n_pages quality\n", par[0]);

    if (lb < 4) return 1;

    return pdf2jpegs(par[1], atoi(par[2]), atoi(par[3]));
}

