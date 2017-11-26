#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void close_fds(FILE** in, int n, FILE* out)
{
    int i;
    for (i=0;i<n;i++) if (in[i]) fclose(in[i]);

    free((void*)in);
    if (out) fclose(out);
}

int mergedb(int lb, char** par)
{
    FILE *out, **in;
    int i, idx, n, done;
    time_t* tm, mintm;
    char* ch;
    unsigned long *lgh, *lc, *lw, *lr;

    out = fopen(par[1], "r");
    if (out)
    {
	printf("File: %s exists, merger will not overwrite it\n", par[1]);
	return 2;
    }

    out = fopen(par[1], "w");

    if (!out)
    {
	printf("Cannot open output file: %s\n", par[1]);
	return 1;
    }

    in = (FILE**)malloc((lb-2)*sizeof(FILE*));
    n = lb-2;
    for (i=0;i<n;i++) in[i] = NULL;

    for (i=2;i<lb;i++)
    {
	idx = i - 2;
	in[idx] = fopen(par[i], "r");

	if (!in[idx])
	{
	    printf("Cannot open input file: %s\n", par[i]);
	    close_fds(in, n, out);
	    return 2;
	}
    }

    tm =  (time_t*)malloc(n*sizeof(time_t));
    ch =  (char*)malloc(n*sizeof(char));
    lgh = (unsigned long*)malloc(n*sizeof(unsigned long));
    lc =  (unsigned long*)malloc(n*sizeof(unsigned long));
    lw =  (unsigned long*)malloc(n*sizeof(unsigned long));
    lr =  (unsigned long*)malloc(n*sizeof(unsigned long));

    done = 0;
    while (!done)
    {
        for (i=0;i<n;i++)
	{
	    if (in[i])
	    {
                if (fread((void*)&tm[i], sizeof(time_t), 1, in[i]) <= 0)              { fclose(in[i]); in[i] = NULL; }
		else if (fread((void*)&ch[i], 1, 1, in[i]) <= 0)                      { fclose(in[i]); in[i] = NULL; }
		else if (fread((void*)&lgh[i], sizeof(unsigned long), 1, in[i]) <= 0) { fclose(in[i]); in[i] = NULL; }
		else if (fread((void*)&lc[i], sizeof(unsigned long), 1, in[i]) <= 0)  { fclose(in[i]); in[i] = NULL; }
		else if (fread((void*)&lw[i], sizeof(unsigned long), 1, in[i]) <= 0)  { fclose(in[i]); in[i] = NULL; }
		else if (fread((void*)&lr[i], sizeof(unsigned long), 1, in[i]) <= 0)  { fclose(in[i]); in[i] = NULL; }
	    }

	}

	mintm = -1;
	idx = -1;
	for (i=0;i<n;i++)
	{
	    if (in[i])
	    {
		if (idx < 0 || tm[i] < mintm)
		{
		    idx = i;
		    mintm = tm[i];
		}
	    }
	}
/*	printf("%d ", idx);*/

	if (idx >= 0)
	{
            fwrite((void*)&tm[idx], sizeof(time_t), 1, out);
            fwrite((void*)&ch[idx], 1, 1, out);
            fwrite((void*)&lgh[idx], sizeof(unsigned long), 1, out);
            fwrite((void*)&lc[idx], sizeof(unsigned long), 1, out);
            fwrite((void*)&lw[idx], sizeof(unsigned long), 1, out);
            fwrite((void*)&lr[idx], sizeof(unsigned long), 1, out);
	}

	for (i=0;i<n;i++)
	{
	    if (in[i])
	    {
		if (i != idx && tm[i] != mintm) 
		{
		    fseek(in[i], -21, SEEK_CUR);
		}
	    }
	}

	idx = 0;
	for (i=0;i<n;i++)
	{
	    if (in[i]) idx ++;
	}

	if (!idx) 
	{
	    done = 1;
	}
    }

    close_fds(in, n, out);
    return 0;
}

int main(int lb, char** par)
{
    if (lb <= 3)
    {
	printf("%s: result.db input1.db input2.db [input3.db ...]\n", par[0]);
	return 1;
    }
    else return mergedb(lb, par);
}

