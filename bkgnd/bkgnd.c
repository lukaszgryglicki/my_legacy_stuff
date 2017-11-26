#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

FILE* logfile;
char** files;
int nfiles;
int dbg = 0;

//go to daemon mode
//after fork leave daemon child with all descriptors closed
//parent exits
void daemonize()
{
 if (fork()) exit(0);
 close(0);
 close(1);
 close(2);
}

int lg(char* fmt, ...)
{
 va_list ap;
 int err;
 va_start(ap,fmt);
 err = vfprintf(logfile,fmt,ap);
 va_end(ap);
 fflush(logfile);
 return err;
}

void create_flist()
{
    FILE* l;
    int zn, lin, idx, midx, i;

    l = fopen("/tmp/bkgnd_list", "r");
    if (!l)
    {
	lg("Cannot open file list\n");
	fclose(logfile);
	exit(1);
    }

    lin = 0;
    idx = 0;
    midx = 0;
    while ((zn = fgetc(l)) != EOF)
    {
	if (zn == '\n') 
	{
	    lin ++;
	    idx = 0;
	}
	else 
	{
	    idx ++;
	    if (idx >midx) midx = idx;
	}

    }
    fseek(l, 0, SEEK_SET);

    files = (char**)malloc(lin * sizeof(char*));
    nfiles = lin;
    for (i=0;i<nfiles;i++) 
    {
	files[i] = (char*)malloc(midx * sizeof(char));
	strcpy(files[i], "null");
    }

    lin = 0;
    idx = 0;
    while ((zn = fgetc(l)) != EOF)
    {
	if (zn == '\n')
	{
	    files[lin][idx] = 0;
	    lin ++;
	    idx = 0;
	}
	else
	{
	    files[lin][idx] = zn;
	    idx ++;
	}

    }
    lg("NFILES: %d\nMAXNAME: %d\n", nfiles, midx);
    for (i=0;i<nfiles;i++)
    {
	lg("FILE %d: %s\n", i+1, files[i]);
    }

    fclose(l);

}


int bkgnd(char* path, char* mask, int secs)
{
    char scmd[1024];
    time_t tm;
    int idx;

    if (secs < 0)
    {
	printf("Seconds must be >= 0\n");
	return 1;
    }

    logfile = fopen("/tmp/bkgnd.log", "w");
    if (!logfile)
    {
	printf("Cannot open log: /tmp/bkgnd.log\n");
	return 1;
    }

    daemonize();

    time(&tm);
    srand((int)tm);

    lg("Running Background daemon:\nPATH: %s\nMASK: %s\nSECONDS: %d\n", path, mask, secs);

    sprintf(scmd, "find %s -name \"%s\" > /tmp/bkgnd_list", path, mask);
    lg("SYSCMD: %s\n", scmd);
    system(scmd);

    create_flist();

    while(1)
    {
	idx = rand() % nfiles;
	lg("Choosen: [%d] %s\n", idx, files[idx]);

	sprintf(scmd, "convert %s -resize 1920x1200! /tmp/bkgnd_img.jpg", files[idx]);
	if(dbg) lg("CONVERT: %s\n", scmd);
	system(scmd);

	sprintf(scmd, "xv -root /tmp/bkgnd_img.jpg -quit");
	if (dbg) lg("DISPLAY: %s\n", scmd);
	system(scmd);

	sleep(secs);
    }

    /* Never reached */
    return 0;
}

int main(int lb, char** par)
{
    if (lb < 4)
    {
	printf("No parameters: path mask time\n");
	printf("Needs: xv, ImageMagick\n");
	return 1;
    }
    return bkgnd(par[1], par[2], atoi(par[3]));
}

