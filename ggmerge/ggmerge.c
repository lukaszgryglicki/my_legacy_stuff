#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void close_fds(FILE** in, int n, FILE* out, FILE* otmp)
{
    int i;
    for (i=0;i<n;i++) if (in[i]) fclose(in[i]);

    free((void*)in);
    if (out) fclose(out);
    if (otmp) fclose(otmp);
}

int ggmerge(int lb, char** par)
{
    FILE   *otmp, *out, **in;
    int    i, idx, n, done;
    time_t *tm, mintm;
    char   **ggcmd, **ggname, **ggtxt, *txt;
    int    *ggnum, ltm, l, j;
    long   *fpos;
    char   hdr[256], sdt[32], stub[256];
    struct tm* t;

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

    otmp = fopen("chat.txt", "w");

    if (!otmp)
    {
	printf("Cannot open output file: %s\n", "chat.txt");
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
	    close_fds(in, n, out, otmp);
	    return 2;
	}
    }

    /*
	chatrcv,4728811,Kocur,1254210007,1254208408,Ten skype wcale nie jest taki tani
	chatrcv,4728811,Kocur,1254210007,1254208414,"<span style=\"color:#000000\">za darmo jest do internetu</span>"
	chatrcv,4728811,Kocur,1254210007,1254208423,"<span style=\"color:#000000\">na komórki w Polsce jest do¶æ drogo</span>"
	chatrcv,4728811,Kocur,1254210007,1254208446,"<span style=\"color:#000000\">Na stacjonarne jeszcze w miarê, ale op³ata sta³a za nawi±zanie po³±czenia oraz trzeba dokupic mikrofon</span>"
	chatsend,4728811,Kocur,1254210059,czesc 
    */

    txt    = (char*)malloc(0x10000*sizeof(char));

    tm     =  (time_t*)malloc(n*sizeof(time_t));
    ggnum  = (int*)malloc(n*sizeof(int));
    fpos   = (long*)malloc(n*sizeof(long));

    ggcmd  = (char**)malloc(n*sizeof(char*));
    ggname = (char**)malloc(n*sizeof(char*));
    ggtxt  = (char**)malloc(n*sizeof(char*));

    for (i=0;i<n;i++) 
    {
	ggcmd[i]  = (char*)malloc(12*sizeof(char));
	ggname[i] = (char*)malloc(128*sizeof(char));
	ggtxt[i] = (char*)malloc(0x10000*sizeof(char));
    }

    done = 0;
    while (!done)
    {
        for (i=0;i<n;i++)
	{
	    if (in[i])
	    {
/*		printf("i=%d\n", i);*/
		fpos[i] = ftell(in[i]);
		if ((l = fscanf(in[i], "%[^,],%d,%[^,],%d,", ggcmd[i], &ggnum[i], ggname[i], &ltm)) != 4)
		{
		    fclose( in[i] );
		    in[i] = NULL;
		    if (l >= 0) printf("E1: i=%d, cmd=%s, l=%d\n", i, ggcmd[i], l);
		}
		else
		{
		    tm[i] = (time_t)ltm;

		    l = strlen( ggcmd[i] );
		    for (j=0;j<l;j++) if (ggcmd[i][j] >= 'A' && ggcmd[i][j] <= 'Z') ggcmd[i][j] += 0x20;

		    if (!strcmp(ggcmd[i], "chatrcv")) 
		    {
		        l = fscanf(in[i], "%d,", &ltm);
		        if (l < 1) 
		        {
		    	    fclose( in[i] );
		    	    in[i] = NULL;
		    	    printf("E2\n");
		        }
			else tm[i] = (time_t)ltm;	/* FIXME: tak naprawde to nie wiadomo ktory z dwoch czasow wziac dla chatrcv */
		    }

		    if (in[i]) l = fscanf(in[i], "%[^\n]\n", ggtxt[i]);
		}

	    }
	    else fpos[i] = -1;

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
/*	printf("idx = %d\n", idx);*/

	if (idx >= 0)
	{
            t = localtime(&tm[idx]);

            sprintf(sdt, "%04d-%02d-%02d %02d:%02d:%02d", 
	        1900 + t->tm_year, t->tm_mon + 1, t->tm_mday, 
	        t->tm_hour, t->tm_min, t->tm_sec);

	    if (!strcmp(ggcmd[idx], "chatsend")) sprintf(hdr, "%s: [%3d] ---> %-12s: ", sdt, idx, ggname[idx]);
	    else sprintf(hdr, "%s: [%3d] <--- %-12s: ", sdt, idx, ggname[idx]);

	    strcpy(txt, ggtxt[idx]);

	    if (!strncmp(txt, "\"<span", 6))
	    {
/*		printf("1->(%s)\n", ggtxt[idx]);*/
		strcpy(txt, &(ggtxt[idx][1]));
		txt[strlen(txt)-1] = 0;
		strcpy(ggtxt[idx], txt);
/*		printf("1<-(%s)\n", txt);*/
	    }

	    if (!strncmp(txt, "<span", 5))
	    {
/*		printf("2->(%s)\n", ggtxt[idx]);*/
		sscanf(ggtxt[idx], "%[^>]>%[^<]</span>", stub, txt);
/*		printf("2<-(%s,%s)\n", stub, txt);*/
	    }
	    /*
	    */
	    l = strlen(txt);
	    for (j=0;j<l;j++) if (txt[j] < 0x0) txt[j] = '_';

	    fprintf(out, "%s%s\n", hdr,txt);
	    fprintf(otmp, "%s\n", txt);
	}

	for (i=0;i<n;i++)
	{
	    if (in[i])
	    {
		if (i != idx && tm[i] != mintm) 
		{
		    fseek(in[i], fpos[i], SEEK_SET);
		}

/*		printf("%d ", ftell(in[i]));*/
	    }
	}
/*	printf("\n");*/

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

    close_fds(in, n, out, otmp);
    return 0;
}

int main(int lb, char** par)
{
    if (lb <= 2)
    {
	printf("%s: result1.txt hist1 [hist2 ...]\n", par[0]);
	return 1;
    }
    else return ggmerge(lb, par);
}

