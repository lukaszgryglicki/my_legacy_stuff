#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

int max_path_len = 0;
int max_pname_len = 0;
int max_dfiles_len = 0;
int max_descr_len = 0;
int max_fulldescr_len = 0;

int size_from = -1;
int size_to = -1;
int size_width = -1;

char* pname_s = NULL;
char* pname_n = NULL;
int pname_width = -1;

char* descr_s = NULL;
char* descr_n = NULL;
int descr_width = -1;

char* dfiles_s = NULL;
char* dfiles_n = NULL;
int dfiles_width = -1;

char* fulldescr_s = NULL;
char* fulldescr_n = NULL;
int fulldescr_width = -1;

int curr_col = 0;

int aux = 0;
char** aux_data;

int cols[5];

typedef struct _distdata
{
    char* path;
    char* pname;
    char* distfiles;
    char* descr;
    char* fulldescr;
    int size;
} distdata;

void init_aux(char** t1, char** t2, char** t3, char** t4, char** t5)
{
    int l;

    if (!aux)
    {
	l = 5;
	aux_data = (char**)malloc(l*sizeof(char*));
	aux_data[0] = (char*)malloc(128*sizeof(char));
	aux_data[1] = (char*)malloc(4096*sizeof(char));
	aux_data[2] = (char*)malloc(512*sizeof(char));
	aux_data[3] = (char*)malloc(16384*sizeof(char));
	aux_data[4] = (char*)malloc(65536*sizeof(char));

	aux = 1;
    }

    *t1 = aux_data[0];
    *t2 = aux_data[1];
    *t3 = aux_data[2];
    *t4 = aux_data[3];
    *t5 = aux_data[4];
}

int process_port(char* path, distdata* dd)
{
    FILE* f;
    int i1, i2, i, l, e, s, done, n;
    char *cmd, *fname, *key, *str, *dfiles;

    init_aux(&cmd, &fname, &key, &str, &dfiles);

/*    printf("%s\n", path);*/
    strcpy(dfiles, "");

    l = strlen(path);
    if (l > max_path_len) max_path_len = l;

    i = l - 1;

    while (path[i] != '/' && i >= 0)
    {
	i --;
    }

    i2 = i;

    i --;
    while (path[i] != '/' && i >= 0) i--;
    i --;
    while (path[i] != '/' && i >= 0) i--;
    i ++;
    i1 = i;

/*    printf("<%d,%d>\n", i1, i2);*/

    dd->pname = (char*)malloc(((i2-i1)+1)*sizeof(char));
    for (i=i1;i<i2;i++)
    {
	dd->pname[i-i1] = path[i];
    }

    dd->pname[i2-i1] = 0;
    l = strlen(dd->pname);

    if (l > max_pname_len) max_pname_len = l;

    for (i=0;i<i2;i++) str[i] = path[i];

    str[i2] = 0;
    strcat(str, "/pkg-descr");

    f = fopen(str, "r");
    if (!f)
    {
/*	fprintf(stderr, "WARNING: Cannot open port description: %s\n", str);*/
	dd->descr = (char*)malloc(sizeof(char));
	dd->descr[0] = 0;
	dd->fulldescr = (char*)malloc(sizeof(char));
	dd->fulldescr[0] = 0;
    }
    else
    {
	fscanf(f, "\n");
	if (fscanf(f, "%[^\n]", str) == 0)
	{
/*	    fprintf(stderr, "WARNING: Cannot read any port description: %s\n", str);*/
	    dd->descr = (char*)malloc(sizeof(char));
	    dd->descr[0] = 0;
	}
	else
	{
	    l = strlen(str);
	    if (l > max_descr_len) max_descr_len = l;
	    dd->descr = (char*)malloc((l+1)*sizeof(char));
	    strcpy(dd->descr, str);
	}

	fseek(f, 0, SEEK_END);
	l = (int)ftell(f);
	fseek(f, 0, SEEK_SET);

        dd->fulldescr = (char*)malloc((l+1)*sizeof(char));
	fread(dd->fulldescr, l, 1, f);
	dd->fulldescr[l] = 0;

        fclose(f);

	for (i=0;i<l;i++) if (dd->fulldescr[i] == '\n') dd->fulldescr[i] = ' ';

	if (l > max_fulldescr_len) max_fulldescr_len = l;
    }

    dd->path = path;

    f = fopen(path, "r");
    if (!f)
    {
	fprintf(stderr, "Cannot open port distinfo: %s\n", path);
	return 1;
    }

    done = dd->size = n = 0;

    while (!done)
    {
        if ((e=fscanf(f, "%s (%[^)]) = %s\n", cmd, fname, key)) != 3)
        {
	    if (e == -1) 
	    {
		if (dd->size > 0) 
		{
		    done = 1;
		    continue;
		}
		else
		{
/*		    fprintf(stderr, "WARNING: %s has no size info\n", path);*/
		}
	    }
	    else 
	    {
		if (!strcmp(cmd, "#") || cmd[0] == '#') 
		{
/*		    fprintf(stderr, "WARNING: %s has no valid entries\n", path);*/
		    fscanf(f, "%[^\n]\n", fname);
		    continue;
		}
		fprintf(stderr, "Error parsing distinfo file: %s: %d\n", path, e);
	    }

	    fclose(f);

	    if (e >= 0) return 2;
	    else 
	    {
		done = 1;
		continue;
	    }
        }

	if (!strcmp(cmd, "SIZE"))
	{
/*	    done = 1;*/
	    if (sscanf(key, "%d", &s) != 1)
	    {
	        fprintf(stderr, "Cannot scan size value from: %s\n", key);
	        fclose(f);
	        return 3;
	    }

	    dd->size += s;


	    if (!strcmp(dfiles, "")) 
	    {
		strcpy(dfiles, fname);
	    }
	    else
	    {
		if (strlen(dfiles) < 32768)
		{
		    strcat(dfiles, ", ");
		    strcat(dfiles, fname);
		}
	    }
/*	    printf("%s --> %d bytes\n", path, dd->size);*/
	}

	n ++;

	if (n == 1000) done = 1;
    }

    if (dd->size <= 0)
    {
/*	fprintf(stderr, "WARNING: Cannot found size data for: %s", path);*/
    }

    l = strlen(dfiles);
    if (l > max_dfiles_len) max_dfiles_len = l;

    dd->distfiles = (char*)malloc((l+1)*sizeof(char));
    strcpy(dd->distfiles, dfiles);

    fclose(f);

    return 0;
}

int read_distfiles(char*** arr, int* n)
{
    FILE* f;
    int l, i;
    char str[2048];

    f = fopen("distinfos.dat", "r");
    if (!f)
    {
	printf("No distinfos.dat file, generating new one.\n");
	system("find /usr/ports/ -depth 3 -name \"distinfo\" > ./distinfos.dat");
    }

    f = fopen("distinfos.dat", "r");
    if (!f)
    {
	fprintf(stderr, "No distinfos.dat file and cannot generate new one.\n");
	return 1;
    }

    l = 0;
    while (fscanf(f, "%s\n", str) == 1) l ++;

/*    printf("Found %d ports to process\n", l);*/
    if (l <= 0)
    {
	fprintf(stderr, "No ports to work.\n");
	fclose(f);
	return 2;
    }

    *arr = (char**)malloc(l*sizeof(char*));
    fseek(f, SEEK_SET, 0);

    for (i=0;i<l;i++)
    {
	if (fscanf(f, "%s\n", str) < 1)
	{
	    fprintf(stderr, "Cannot scan %d/%d line in distinfos.dat\n", i+1, l);
	    fclose(f);
	    return 2;
	}

	(*arr)[i] = (char*)malloc((strlen(str) + 1) * sizeof(char));
	strcpy((*arr)[i], str);
    }

    *n = l;
    fclose(f);

    return 0;
}

void parse_subopts(void (*func)(char*), char* str)
{
    int i, l, j;
    char* s;

    l = strlen( str );
    s = (char*)malloc((l+1)*sizeof(char));

    j = 0;
    for (i=0;i<l;i++)
    {
	if (str[i] == ',' || i == (l-1))
	{
	    strncpy(s, &str[j], (i-j)+1);

	    if (i == (l-1)) s[(i-j)+1] = 0;
	    else s[(i-j)] = 0;

	    j = i+1;
            func(s);
	}
    }
}

void parse_name(char* str)
{
    int e, ival;
    char cval;
    char sval[1024];

    if ((e=sscanf(str, "%c%s", &cval, sval)) < 2)
    {
	printf("Cannot scan 2 values from '%s', scanned: %d, template: , Xval, X=s,n,w\n", str, e);
	exit(1);
    }

    if (cval == 'c' || cval == 'C')
    {
	if (sscanf(sval, "%d", &ival) < 1)
	{
	    printf("cannot scan numeric value from '%s'\n", sval);
	    exit(1);
	}

        if (ival < 0) 
        {
	    cols[curr_col] = -1;
        }
        else if (ival >= 0 && ival < 5) 
        {
	    cols[ival] = curr_col;
        }
	else
	{
	    printf("Bad column value: %d, allowed: {-1,0,1,2,3,4}\n", ival);
	    exit(1);
	}
    }
    else if (cval == 's' || cval == 'S')
    {
	if (curr_col == 1)
	{
	    if (pname_s)
	    {
	        printf("Pname search pattern already given: %s\n", pname_s);
	        exit(1);
	    }
	}
	else if (curr_col == 2)
	{
	    if (descr_s)
	    {
	        printf("Description search pattern already given: %s\n", descr_s);
	        exit(1);
	    }
	}
	else if (curr_col == 3)
	{
	    if (dfiles_s)
	    {
	        printf("Distfiles search pattern already given: %s\n", dfiles_s);
	        exit(1);
	    }
	}
	else if (curr_col == 4)
	{
	    if (fulldescr_s)
	    {
	        printf("Full description search pattern already given: %s\n", fulldescr_s);
	        exit(1);
	    }
	}

	if (!strcmp(sval, ""))
	{
	    printf("Error: empty string given for option\n");
	    exit(1);
	}

	if (curr_col == 1)
	{
	    pname_s = (char*)malloc((strlen(sval)+1)*sizeof(char));
	    strcpy(pname_s, sval);
	}
	else if (curr_col == 2)
	{
	    descr_s = (char*)malloc((strlen(sval)+1)*sizeof(char));
	    strcpy(descr_s, sval);
	}
	else if (curr_col == 3)
	{
	    dfiles_s = (char*)malloc((strlen(sval)+1)*sizeof(char));
	    strcpy(dfiles_s, sval);
	}
	else if (curr_col == 4)
	{
	    fulldescr_s = (char*)malloc((strlen(sval)+1)*sizeof(char));
	    strcpy(fulldescr_s, sval);
	}
    }
    else if (cval == 'n' || cval == 'N')
    {
	if (curr_col == 1)
	{
	    if (pname_n)
	    {
	        printf("Pname exclude search pattern already given: %s\n", pname_n);
	        exit(1);
	    }
	}
	else if (curr_col == 2)
	{
	    if (descr_n)
	    {
	        printf("Description exclude search pattern already given: %s\n", descr_n);
	        exit(1);
	    }
	}
	else if (curr_col == 3)
	{
	    if (dfiles_n)
	    {
	        printf("Distfiles exclude search pattern already given: %s\n", dfiles_n);
	        exit(1);
	    }
	}
	else if (curr_col == 4)
	{
	    if (fulldescr_n)
	    {
	        printf("Full description exclude search pattern already given: %s\n", fulldescr_n);
	        exit(1);
	    }
	}

	if (!strcmp(sval, ""))
	{
	    printf("Error: empty string given for -n n option\n");
	    exit(1);
	}

	if (curr_col == 1)
	{
	    pname_n = (char*)malloc((strlen(sval)+1)*sizeof(char));
	    strcpy(pname_n, sval);
	}
	else if (curr_col == 2)
	{
	    descr_n = (char*)malloc((strlen(sval)+1)*sizeof(char));
	    strcpy(descr_n, sval);
	}
	else if (curr_col == 3)
	{
	    dfiles_n = (char*)malloc((strlen(sval)+1)*sizeof(char));
	    strcpy(dfiles_n, sval);
	}
	else if (curr_col == 4)
	{
	    fulldescr_n = (char*)malloc((strlen(sval)+1)*sizeof(char));
	    strcpy(fulldescr_n, sval);
	}
    }
    else if (cval == 'w' || cval == 'W')
    {
	if (curr_col == 1)
	{
	    if (pname_width >= 0)
	    {
	        printf("Error: pname width already set\n");
	        exit(1);
	    }
	}
	else if (curr_col == 2)
	{
	    if (descr_width >= 0)
	    {
	        printf("Error: description width already set\n");
	        exit(1);
	    }
	}
	else if (curr_col == 3)
	{
	    if (dfiles_width >= 0)
	    {
	        printf("Error: distfiles width already set\n");
	        exit(1);
	    }
	}
	else if (curr_col == 4)
	{
	    if (fulldescr_width >= 0)
	    {
	        printf("Error: full description width already set\n");
	        exit(1);
	    }
	}

	if (sscanf(sval, "%d", &ival) < 1)
	{
	    printf("cannot scan numeric value from '%s'\n", sval);
	    exit(1);
	}

        if (ival >= 1) 
	{
	    if (curr_col == 1) pname_width = ival;
	    else if (curr_col == 2) descr_width = ival;
	    else if (curr_col == 3) dfiles_width = ival;
	    else if (curr_col == 4) fulldescr_width = ival;
	}
	else
	{
	    printf("Bad width value: %d, allowed >= 1\n", ival);
	    exit(1);
	}
    }
    else
    {
	printf("Unknown suboption %c, allowed: c,s,n,w\n", cval);
	exit(1);
    }
}

void parse_size(char* str)
{
    int ival, e;
    char cval;

    if ((e=sscanf(str, "%c%d", &cval, &ival)) < 2)
    {
	printf("Cannot scan 2 values from '%s', scanned: %d, template: , Xnum, X=c,f,t,w, num=>=-1\n", str, e);
	exit(1);
    }

    if (cval == 'c' || cval == 'C')
    {
        if (ival < 0) 
        {
	    cols[0] = -1;
        }
        else if (ival >= 0 && ival < 5) 
        {
	    cols[ival] = 0;
        }
	else
	{
	    printf("Bad column value: %d, allowed: {-1,0,1,2,3,4}\n", ival);
	    exit(1);
	}
    }
    else if (cval == 'f' || cval == 'F')
    {
	if (size_from >= 0)
	{
	    printf("Error: size from already set\n");
	    exit(1);
	}

        if (ival >= -1) 
	{
	   size_from = ival;
	}
	else
	{
	    printf("Bad size from value: %d, allowed >= -1\n", ival);
	    exit(1);
	}
    }
    else if (cval == 't' || cval == 'T')
    {
	if (size_to >= 0)
	{
	    printf("Error: size to already set\n");
	    exit(1);
	}

        if (ival >= -1) 
	{
	   size_to = ival;
	}
	else
	{
	    printf("Bad size to value: %d, allowed >= -1\n", ival);
	    exit(1);
	}
    }
    else if (cval == 'w' || cval == 'W')
    {
	if (size_width >= 0)
	{
	    printf("Error: size width already set\n");
	    exit(1);
	}

        if (ival >= 1) 
	{
	   size_width = ival;
	}
	else
	{
	    printf("Bad size width value: %d, allowed >= 1\n", ival);
	    exit(1);
	}
    }
    else
    {
	printf("Unknown suboption for -s: %c, allowed: c,f,t,w\n", cval);
	exit(1);
    }
}

void display_port(distdata* dd)
{
    /*
    // cols[idx]
    // idx:
    // 0 = size
    // 1 = port_name
    // 2 = short desc
    // 3 = distfiles
    // 4 = full desc
    */

    int i;
    char** strs;
    char fmt[128], pfmt[16];

    if (size_from >= 0 && dd->size < size_from) return;
    if (size_to >= 0 && dd->size > size_to) return;

    if (pname_s && !strstr(dd->pname, pname_s)) return;
    if (pname_n && strstr(dd->pname, pname_n)) return;

    if (descr_s && !strstr(dd->descr, descr_s)) return;
    if (descr_n && strstr(dd->descr, descr_n)) return;

    if (dfiles_s && !strstr(dd->distfiles, dfiles_s)) return;
    if (dfiles_n && strstr(dd->distfiles, dfiles_n)) return;

    if (fulldescr_s && !strstr(dd->fulldescr, fulldescr_s)) return;
    if (fulldescr_n && strstr(dd->fulldescr, fulldescr_n)) return;

    strcpy(fmt, "");
    strs = (char**)malloc(5*sizeof(char*));

    for (i=0;i<5;i++)
    {
	if (cols[i] == 0) 
	{
	    if (size_width < 0)
	    {
	        strcpy(pfmt, "%-10s ");
	        strs[i] = (char*)malloc(11*sizeof(char));
	        sprintf(strs[i], "%d", dd->size);
	    }
	    else
	    {
	        sprintf(pfmt, "%%-%ds ", size_width);
	        strs[i] = (char*)malloc((size_width+1)*sizeof(char));
	        sprintf(strs[i], "%d", dd->size);
		strs[i][size_width] = 0;

	    }
	    strcat(fmt, pfmt);
	}
	else if (cols[i] == 1) 
	{
	    if (pname_width < 0)
	    {
	        sprintf(pfmt, "%%-%ds ", max_pname_len);
	        strs[i] = (char*)malloc((strlen(dd->pname) + 1)*sizeof(char));
	        strcpy(strs[i], dd->pname);
	    }
	    else
	    {
	        sprintf(pfmt, "%%-%ds ", pname_width);
	        strs[i] = (char*)malloc((pname_width + 1)*sizeof(char));
	        strcpy(strs[i], dd->pname);
		strs[i][pname_width] = 0;
	    }
	    strcat(fmt, pfmt);
	}
	else if (cols[i] == 2) 
	{
	    if (descr_width < 0)
	    {
	        sprintf(pfmt, "%%-%ds ", max_descr_len);
	        if (!strcmp(dd->descr, ""))
	        {
	             strs[i] = (char*)malloc(16*sizeof(char));
	             strcpy(strs[i], "no description");
	        }
	        else
	        {
	             strs[i] = (char*)malloc((strlen(dd->descr) + 1)*sizeof(char));
	             strcpy(strs[i], dd->descr);
	        }
	    }
	    else
	    {
	        sprintf(pfmt, "%%-%ds ", descr_width);
	        if (!strcmp(dd->descr, ""))
	        {
	             strs[i] = (char*)malloc((descr_width+1)*sizeof(char));
	             strcpy(strs[i], "no description");
	        }
	        else
	        {
	             strs[i] = (char*)malloc((descr_width+1)*sizeof(char));
	             strcpy(strs[i], dd->descr);
	        }
		strs[i][descr_width] = 0;
	    }
	    strcat(fmt, pfmt);
	}
	else if (cols[i] == 3) 
	{
	    if (dfiles_width < 0)
	    {
	        sprintf(pfmt, "%%-%ds ", max_dfiles_len);
	        if (!strcmp(dd->distfiles, ""))
	        {
	             strs[i] = (char*)malloc(16*sizeof(char));
	             strcpy(strs[i], "no distfiles");
	        }
	        else
	        {
	             strs[i] = (char*)malloc((strlen(dd->distfiles) + 1)*sizeof(char));
	             strcpy(strs[i], dd->distfiles);
	        }
	    }
	    else
	    {
	        sprintf(pfmt, "%%-%ds ", dfiles_width);
	        if (!strcmp(dd->distfiles, ""))
	        {
	             strs[i] = (char*)malloc((dfiles_width+1)*sizeof(char));
	             strcpy(strs[i], "no distfiles");
	        }
	        else
	        {
	             strs[i] = (char*)malloc((dfiles_width + 1)*sizeof(char));
	             strcpy(strs[i], dd->distfiles);
	        }
		strs[i][dfiles_width] = 0;
	    }
	    strcat(fmt, pfmt);
	}
	else if (cols[i] == 4) 
	{
	    if (fulldescr_width < 0)
	    {
	        sprintf(pfmt, "%%-%ds ", max_fulldescr_len);
	        if (!strcmp(dd->fulldescr, ""))
	        {
	             strs[i] = (char*)malloc(16*sizeof(char));
	             strcpy(strs[i], "no description");
	        }
	        else
	        {
	             strs[i] = (char*)malloc((strlen(dd->fulldescr) + 1)*sizeof(char));
	             strcpy(strs[i], dd->fulldescr);
	        }
	    }
	    else
	    {
	        sprintf(pfmt, "%%-%ds ", fulldescr_width);
	        if (!strcmp(dd->fulldescr, ""))
	        {
	             strs[i] = (char*)malloc((fulldescr_width+1)*sizeof(char));
	             strcpy(strs[i], "no description");
	        }
	        else
	        {
	             strs[i] = (char*)malloc((fulldescr_width + 1)*sizeof(char));
	             strcpy(strs[i], dd->fulldescr);
	        }
		strs[i][fulldescr_width] = 0;
	    }
	    strcat(fmt, pfmt);
	}
	else
	{
	    strcpy(pfmt, "%-4s ");
	    strs[i] = (char*)malloc(5*sizeof(char));
	    strcpy(strs[i], "--");
	    strcat(fmt, pfmt);
	}

    }

    strcat(fmt, "\n");
/*    printf("\"%s\"\n", fmt);*/

    printf(fmt, strs[0], strs[1], strs[2], strs[3], strs[4]);

    for (i=0;i<5;i++) free((void*)strs[i]);
    free((void*)strs);
}

int portscan()
{
    char** dists;
    int n, i;

    distdata* dinfo;

    if (read_distfiles(&dists, &n) != 0)
    {
	fprintf(stderr, "Error reading distfiles info\n");
	return 1;
    }

    dinfo = (distdata*)malloc(n * sizeof(distdata));

    for (i=0;i<n;i++)
    {
	if (process_port( dists[i], &dinfo[i] ) != 0)
	{
	    fprintf(stderr, "Error processing %d/%d port: %s\n", i+1, n, dists[i]);
	    return 2;
	}
    }

    for (i=0;i<n;i++)
    {
	display_port( &dinfo[i] );
    }

    return 0;
}

void help()
{
    printf("Options are:\n");
    printf("Columns are numbered from 0 to 4: {0,1,2,3,4}\n");
    printf("-s size args, -n name args, -d descr args, -i distfiles args, -f full descr args\n");
    printf("\t-s c# f# t# w#\t[c - number of col display, f - from, t - to, -1 means skip, -w width of col, # - number]\n");
    printf("\t-[n|d|i|f] c# sstr nstr w#\t[c - number of col display, s - search str, n - exclude str, -1 means skip, -w width of col, # - number]\n");
}

int main(int lb, char** par)
{
    int i, u;

    for (i=0;i<5;i++) cols[i] = i;

    while ((u = getopt(lb,par,"hs:n:d:i:f:"))!=-1)
    {
         switch (u)
         {
              case 'h': help(); return 0;
	      case 's': parse_subopts(parse_size, optarg); break;
	      case 'n': curr_col = 1; parse_subopts(parse_name, optarg); break;
	      case 'd': curr_col = 2; parse_subopts(parse_name, optarg); break;
	      case 'i': curr_col = 3; parse_subopts(parse_name, optarg); break;
	      case 'f': curr_col = 4; parse_subopts(parse_name, optarg); break;
              default: printf("Unrecognized option\n"); return 1;
         }
    }

    return portscan();
}

