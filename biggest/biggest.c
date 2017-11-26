#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef struct _filesize
{
    off_t size;
    char *name;
} filesize;
off_t s_k, s_m, s_g, s_t, s_e;

void print_wait(int num, int div)
{
    long sdiv;
    char sdel[13];
    strcpy(sdel, "\b\b\b\b\b\b\b\b\b\b\b\b");
    sdiv = div / 4;
/*    if (num == 0)
    {
	printf("\n");
	fflush(stdout);
    }*/
    if (num > 0 && (num % div) == 0)
    {
	printf("%s%d|", sdel, num);
	fflush(stdout);
    }
    else if (num > 0 && (num % div) == sdiv)
    {
	printf("%s%d/", sdel, num);
	fflush(stdout);
    }
    else if (num > 0 && (num % div) == 2*sdiv)
    {
	printf("%s%d-", sdel, num);
	fflush(stdout);
    }
    else if (num > 0 && (num % div) == 3*sdiv)
    {
	printf("%s%d\\", sdel, num);
	fflush(stdout);
    }
}


char* getline(FILE* fp)
{
    off_t pos;
    int done, bytes, zn, i;
    char *line;
    pos = ftello( fp );
    done = 0;
    bytes = 0;
    do
    {
	zn = fgetc( fp );
	if (zn == EOF || zn == '\n') done = 1;
	else bytes ++;
    }
    while (!done);
    if (bytes == 0) return NULL;
    fseeko( fp, pos, SEEK_SET );
    line = (char*)malloc((bytes + 1) * sizeof(char));
    for (i=0;i<bytes;i++) line[ i ] = fgetc( fp );
    line[ bytes ] = 0;
    fgetc( fp );
    return line;
}


void getsize(filesize* fs)
{
    FILE* fp;
    if (!fs) return;
    fp = fopen( fs->name, "r" );
    if (!fp)
    {
	fs->size = -1;
	return;
    }
    fseeko(fp, 0, SEEK_END);
    fs->size = ftello( fp );
    fclose(fp);
}


int compare_filesize(const void* fsv1, const void* fsv2)
{
    filesize *fs1, *fs2;
    fs1 = (filesize*)fsv1;
    fs2 = (filesize*)fsv2;
    if (fs1->size < fs2->size) return -1;
    else if (fs1->size < fs2->size) return 0;
    else return 1;
}


int biggest(char* path, off_t minsize)
{
	char *cmd, *line;
	int idx, i, n, k;
	off_t l, f, summary;
	FILE *fp;
	filesize *fs, *fs2;
	f = 9;
	s_k = f << 10;
	s_m = f << 20;
	s_g = f << 30;
	s_t = f << 40;
	s_e = f << 50;
	printf("-------------------START-----------------\nMinimal size is %lld bytes\n", minsize);
	/*
	printf("K boundary --> %lld\n", s_k);
	printf("M boundary --> %lld\n", s_m);
	printf("G boundary --> %lld\n", s_g);
	printf("T boundary --> %lld\n", s_t);
	printf("E boundary --> %lld\n", s_e);
	*/
	cmd = (char*)malloc((strlen(path) + 0x100) * sizeof(char));
	if (!cmd) return 1;
	sprintf(cmd, "find \"%s\" -name \"*\" > /tmp/biggest_find.tmp", path);
	system(cmd);
	free((void*)cmd);
	printf("Find cache completed\n");
	fp = fopen("/tmp/biggest_find.tmp", "r");
	idx = 0;
	while ((line = getline(fp)) != NULL)
	{
	    idx ++;
	    print_wait( idx, 800 );
/*	    printf("[%9d]=-> \"%s\"\n", idx, line);*/
	    free((void*)line);
	}
	fseeko(fp, 0, SEEK_SET);
	printf("\nNum items counted: %d\n", idx);
	fs = (filesize*)malloc(idx * sizeof(filesize));
	i = 0;
	summary = 0;
	for (i=0;i<idx;i++)
	{
	    print_wait( i, 40 );
	    fs[ i ].name = getline( fp );
	    getsize( &fs[ i ] );
	    if (fs[ i ].size > 0) summary += fs[ i ].size;
	}
	fclose(fp);
	printf("\nAll sizes computed, summary size %lld(%lldMB, %lldGB, %lldTB)\n", summary, summary >> 20, summary >> 30, summary >> 40);
	n = 0;
	for (i=0;i<idx;i++)
	{
	    print_wait( i, 1600 );
	    if (fs[ i ].size > 0) n ++;
	}
	fs2 = (filesize*)malloc(n * sizeof(filesize));
	k = 0;
	for (i=0;i<idx;i++)
	{
	    print_wait( i, 800 );
	    if (fs[ i ].size > 0)
	    {
		memcpy((void*)&fs2[ k ], (void*)&fs[ i ], sizeof( filesize ));
		k ++;
	    }
	}
	printf("\nNumber of nonzero files: %d\n", n);
	qsort((void*)fs2, (size_t)n, sizeof(filesize), compare_filesize);
	printf("Filesizes sorted\n");
	for (i=0;i<n;i++)
	{
	    l = fs2[ i ].size;
	    if (l < minsize) continue;
	    if (l < s_k) printf("[%9d/%9d][%15lldb]\"%s\"\n", i, n, fs2[ i ].size, fs2[ i ].name);
	    else if (l < s_m) printf("[%9d/%9d][%15lldk]\"%s\"\n", i, n, fs2[ i ].size >> 10, fs2[ i ].name);
	    else if (l < s_g) printf("[%9d/%9d][%15lldm]\"%s\"\n", i, n, fs2[ i ].size >> 20, fs2[ i ].name);
	    else if (l < s_t) printf("[%9d/%9d][%15lldg]\"%s\"\n", i, n, fs2[ i ].size >> 30, fs2[ i ].name);
	    else if (l < s_e) printf("[%9d/%9d][%15lldk]\"%s\"\n", i, n, fs2[ i ].size >> 40, fs2[ i ].name);
	}
	unlink("/tmp/biggest_find.tmp");
	printf("All done\n--------------------END------------------\n");
	return 0;
}


int main(int lb, char** par)
{
    	off_t size, num;
	char c;
	if (lb < 3)
	{
	    printf("usage: %s dir minsize\n", par[0]);
	    return 0;
	}
	else
	{
	    sscanf(par[2], "%lld%c", &num, &c);
	    if (c == 'b' || c == 'B') size = num;
	    else if (c == 'k' || c == 'K') size = num << 10;
	    else if (c == 'm' || c == 'M') size = num << 20;
	    else if (c == 'g' || c == 'G') size = num << 30;
	    else if (c == 't' || c == 'T') size = num << 40;
	    else if (c == 'e' || c == 'E') size = num << 50;
	    else size = num;
	    return biggest( par[1], size );
	}
}

