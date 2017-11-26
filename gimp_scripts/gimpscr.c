#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GEXEC "/data/dev/gimp_scripts/gimpexec.txt"
#define GSCR "/data/dev/gimp_scripts/gimpscr.txt"

char* gimpexec()
{
    char* str;
    FILE* f;
    int l;

    f = fopen(GEXEC, "rb");

    if (!f)
    {
	str = (char*)malloc(10 * sizeof(char));
	strcpy(str, "gimp");
	return str;
    }

    fseek(f, 0, SEEK_END);
    l = (int)ftell(f);
    fseek(f, 0, SEEK_SET);

    str = (char*)malloc((l+1) * sizeof(char));

    fread((void*)str, (size_t)l, 1, f);
    str[l-1] = 0;

    return str;

    fclose(f);
}

char* gimpscrtxt()
{
    char* str;
    FILE* f;
    int l;

    f = fopen(GSCR, "rb");

    if (!f)
    {
	str = (char*)malloc(16 * sizeof(char));
	strcpy(str, "(gimp-quit 0)");
	return str;
    }

    fseek(f, 0, SEEK_END);
    l = (int)ftell(f);
    fseek(f, 0, SEEK_SET);

    str = (char*)malloc((l+1) * sizeof(char));

    fread((void*)str, (size_t)l, 1, f);
    str[l-1] = 0;

    return str;

    fclose(f);
}

int gimpscr(int lb, char** par)
{
    char* gexec;
    char* scr;   
    int i, j, l;
    char *nscr, *scmd, s2[2]; 
    
    gexec = gimpexec();
    printf("Gimp executable is: \"%s\"\n", gexec);
    scr = gimpscrtxt();
    printf("Gimp script is:\n%s\n", scr);

    nscr = (char*)malloc(2048*sizeof(char));
    scmd = (char*)malloc(2048*sizeof(char));
    s2[1] = 0;

    for (i=1;i<lb;i++)
    {
	strcpy(nscr, "");
	l = strlen(scr);

	for (j=0;j<l;j++)
	{
	    if (scr[j] == '~')
	    {
		j ++;
		if (scr[j] == '1')
		{
		    strcat(nscr, "\\\"");
		    strcat(nscr, par[i]);
		    strcat(nscr, "\\\"");
		}
		else if (scr[j] == '2')
		{
		    strcat(nscr, "\\\"scaled_");
		    strcat(nscr, par[i]);
		    strcat(nscr, "\\\"");
		}
		else
		{
		    strcat(nscr, "\\\"MorgothV8 Gimped\\\"");
		}
	    }
	    else
	    {
		s2[0] = scr[j];
		strcat(nscr, s2);
	    }
	}
	sprintf(scmd, "%s \"%s\"", gexec, nscr);
	fprintf(stdout, "%s\n", scmd);
	system( scmd );
    }

    return 0;
}

void help()
{
    printf("GIMP executable (full path to EXE) should be in text file: \"%s\"\n", GEXEC);
    printf("SCRIPT to run should be in text file \"%s\"\n", GSCR);
}

int main(int lb, char** par)
{
    help();
    return gimpscr(lb, par);
}

