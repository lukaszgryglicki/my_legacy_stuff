/* Written by MorgothDBMA: morgothdbma@o2.pl, tel +48693582014 */
/* license: BSD */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#ifndef NOJPEG
#include <setjmp.h>
#include <jpeglib.h>

#define ERR_CANNOTREAD  1
#define ERR_BADJPEG     2
#define ERR_GRAYJPEG    3
#define ERR_256CJPEG    4
#define ERR_CANNOTWRITE 5

#define JQUAL 100
#endif

typedef struct _PtrList
{
 struct _PtrList *next, *prev;
 void* ptr;
} PtrList;

typedef struct _StatData
{
    char sname[0x10];
    char status;
    double perc;
    int c, n;
} StatData;

int stat_num, stat_num_safe;
int *stat_order, *stat_idx;
int** occ_sday;
int** occ_smonth;
int** occ_iday;
int** occ_ihour;
int** occ_iminute;
int** occ_isecond;
int** occ_iyear;
int** occ_idate;
int** occ_idecade;
int** occ_iweek;

int cmp_statdata(void* p1, void* p2)
{
    StatData *s1, *s2;

    s1 = (StatData*)p1;
    s2 = (StatData*)p2;

    return s1->perc < s2->perc;
}

void disp_stat(PtrList* p)
{
    StatData* sd;

    while (p)
    {
	sd = (StatData*)p->ptr;

	printf("%7.3f%% ", sd->perc);
	p = p->next;
    }

    printf("\n");
}

void ptrlist_add_sorted(PtrList** head, void* t, int (*cmp)(void*, void*))
{
 PtrList *temp, *i, *prev, *next;

 if (*head == NULL)
   {
    *head = (PtrList*)(malloc(sizeof(PtrList)));
    (*head)->ptr = (void*)t;
    (*head)->next = NULL;
    (*head)->prev = NULL;
    return;
   }

  i = *head;

  while (i)
  {
      if (cmp(t, i->ptr)) 
      {
          prev = i;
	  i = i->next;
          next = i;
      }
      else
      {
          prev = i->prev;
          next = i;
	  break;
      }
  } 

  temp = (PtrList*)(malloc(sizeof(PtrList)));
  temp->ptr = (void*)t;
  temp->prev = prev;
  temp->next = next;

  if (prev) prev->next = temp;
  if (next) next->prev = temp;

  if (!prev) *head = temp;

/*  disp_stat(*head);*/
}

void ptrlist_add(PtrList** head, void* t)
{
 PtrList* temp;
 if (*head == NULL)
   {
    *head = (PtrList*)(malloc(sizeof(PtrList)));
    (*head)->ptr = (void*)t;
    (*head)->next = NULL;
    (*head)->prev = NULL;
    return;
   }
  temp = (PtrList*)(malloc(sizeof(PtrList)));
  temp->ptr = (void*)t;
  temp->next = *head;
  temp->prev = NULL;
  (*head)->prev = temp;
  *head = temp;
}


PtrList* ptrlist_get_tail(PtrList* head)
{
 if (!head) return NULL;
 while (head->next)
   {
    head = head->next;
   }
 return head;
}

void ptrlist_free(PtrList** head, int free_ptr)
{
 PtrList* temp;
 if (!head) return;
 while (*head)
   {
    temp = *head;
    if (free_ptr) free(temp->ptr);
    *head = (*head)->next;
    free(temp);
   }
 *head = NULL;
}

#ifndef NOJPEG
struct my_error_mgr
{
 struct jpeg_error_mgr pub;
 jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

static my_error_ptr errptr = NULL;

static void my_error_exit(j_common_ptr cinfo)
{
 my_error_ptr myerr = (my_error_ptr) cinfo->err;
 (*cinfo->err->output_message) (cinfo);
 longjmp(myerr->setjmp_buffer, 1);
}

unsigned long RGB(int r, int g, int b)
{
    return (r << 0x10) + (g << 0x8) + b;
}

void dRGB(unsigned char* r, unsigned char* g, unsigned char* b, unsigned long v)
{
    *r = (v & 0xff0000) >> 0x10;
    *g = (v & 0xff00) >> 0x8;
    *b = v & 0xff;
}

void jpeg_compute_pixels(unsigned char* pix, unsigned long** tab, int x, int y)
{
 int i,j;
 unsigned char col[3];
 for (i=0;i<y;i++)
 for (j=0;j<x;j++)
 {
        dRGB(&col[0], &col[1], &col[2], tab[i][j]);
    	pix[3*(x*((y-i)-1)+j)] = col[0];
	pix[3*(x*((y-i)-1)+j)+1] = col[1];
	pix[3*(x*((y-i)-1)+j)+2] = col[2];
 }
}

void init_pixels(unsigned char** pix, unsigned long** tab, int x, int y)
{
    *pix = (unsigned char*)malloc((x*y*3)*sizeof(unsigned char));
    jpeg_compute_pixels(*pix, tab, x, y);
}

void free_pixels(unsigned char** pix)
{
 free(*pix);
 *pix = NULL;
}

int save_jpeg_file(unsigned long** tab, int x, int y, char* filename)
{
 struct jpeg_compress_struct cinfo;
 struct my_error_mgr jerr;
 unsigned char* pixels;
 JSAMPROW row_pointer[1];
 int row_stride;
 FILE* outfile;
 pixels = NULL;

 if ((outfile = fopen(filename, "wb")) == NULL) return ERR_CANNOTWRITE;
 
 init_pixels(&pixels, tab, x, y);
 cinfo.err = jpeg_std_error(&jerr.pub);
 jerr.pub.error_exit = my_error_exit;
 errptr = &jerr;
 if (setjmp(jerr.setjmp_buffer))
 {
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
	return ERR_BADJPEG;
 }
 jpeg_create_compress(&cinfo);
 jpeg_stdio_dest(&cinfo, outfile);
 cinfo.image_width = x;
 cinfo.image_height = y;
 cinfo.input_components = 3;
 cinfo.in_color_space = JCS_RGB;

 jpeg_set_defaults(&cinfo);
 jpeg_set_quality(&cinfo, JQUAL, FALSE);
 jpeg_start_compress(&cinfo, TRUE);

 row_stride = x*3;
 while (cinfo.next_scanline < cinfo.image_height)
   {
    row_pointer[0] = &pixels[cinfo.next_scanline * row_stride];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
   }
 jpeg_finish_compress(&cinfo);
 jpeg_destroy_compress(&cinfo);
 free_pixels(&pixels);
 fclose(outfile);
 return 0;
}

void make_jpeg(PtrList* h, int n)
{
    unsigned long** bits;
    int i, j, k, m, x, y, ch;

    m = (int)sqrt((double)n);
    m ++;

    x = y = m;
    h = ptrlist_get_tail(h);

    bits = (unsigned long**)malloc(x * sizeof(unsigned long*));
    for (i=0;i<x;i++) bits[i] = (unsigned long*)malloc(y * sizeof(unsigned long));

    k = 0;

    for (i=0;i<x;i++)
    {
	for (j=0;j<y;j++)
	{
	    if (h)
	    {
	        ch = (int)h->ptr;
		h = h->prev;
	    }
	    else
	    {
		ch = '?';
	    }
	    k ++;

	    if (ch == '+')      bits[i][j] = RGB(0, 255, 0);
	    else if (ch == '<') bits[i][j] = RGB(0, 255, 255);
	    else if (ch == '>') bits[i][j] = RGB(0, 0, 255);
	    else if (ch == 'C') bits[i][j] = RGB(255, 255, 255);
	    else if (ch == '!') bits[i][j] = RGB(255, 128, 0);
	    else if (ch == '-') bits[i][j] = RGB(255, 0, 0);
	    else if (ch == '?') bits[i][j] = RGB(128, 128, 128);
	    else                bits[i][j] = RGB(0,0,0);
	}
    }

    if (save_jpeg_file(bits, x, y, "readdb.jpeg") != 0)
    {
	printf("Error saving JPEG: readdb.jpeg\n");
	exit(11);
    }
}

#endif

static char* hdir;
static int usehome;

void separator_line(FILE* f, char c)
{
    int i;
    for (i=0;i<80;i++) fprintf(f, "%c", c);
    fprintf(f, "\n");
}

void init_stat()
{
    int i, j, ndates;

    stat_num = 4;
    stat_num_safe = 6;

    stat_order = (int*)malloc(stat_num_safe * sizeof(int));
    stat_idx = (int*)malloc(0x100 * sizeof(int));

    stat_order[0] = '+';
    stat_order[1] = '-';
    stat_order[2] = '<';
    stat_order[3] = '!';
    stat_order[4] = 'C';
    stat_order[5] = '?';

    for (i=0;i<0x100;i++) stat_idx[i] = 5;

    stat_idx['+'] = 0;
    stat_idx['-'] = 1;
    stat_idx['<'] = 2;
    stat_idx['!'] = 3;
    stat_idx['C'] = 4;
    stat_idx['?'] = 5;

    ndates = 32 * 13 * 100;

    occ_sday = (int**)malloc(7 * sizeof(int*));
    occ_smonth = (int**)malloc(12 * sizeof(int*));
    occ_iday = (int**)malloc(32 * sizeof(int*));
    occ_ihour = (int**)malloc(24 * sizeof(int*));
    occ_iminute = (int**)malloc(60 * sizeof(int*));
    occ_isecond = (int**)malloc(60 * sizeof(int*));
    occ_iyear = (int**)malloc(100 * sizeof(int*));
    occ_idate = (int**)malloc(ndates * sizeof(int*));
    occ_idecade = (int**)malloc(3 * sizeof(int*));
    occ_iweek = (int**)malloc(5 * sizeof(int*));

    for (i=0;i<7;i++) occ_sday[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<12;i++) occ_smonth[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<32;i++) occ_iday[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<24;i++) occ_ihour[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<60;i++) occ_iminute[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<60;i++) occ_isecond[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<100;i++) occ_iyear[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<ndates;i++) occ_idate[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<3;i++) occ_idecade[i] = (int*)malloc(stat_num_safe * sizeof(int));
    for (i=0;i<5;i++) occ_iweek[i] = (int*)malloc(stat_num_safe * sizeof(int));

    for (i=0;i<7;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_sday[i][j] = 0;
        }
    }

    for (i=0;i<12;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_smonth[i][j] = 0;
        }
    }

    for (i=0;i<32;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_iday[i][j] = 0;
        }
    }

    for (i=0;i<24;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_ihour[i][j] = 0;
        }
    }

    for (i=0;i<60;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_iminute[i][j] = 0;
        }
    }

    for (i=0;i<60;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_isecond[i][j] = 0;
        }
    }

    for (i=0;i<100;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_iyear[i][j] = 0;
        }
    }

    for (i=0;i<ndates;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_idate[i][j] = 0;
        }
    }

    for (i=0;i<3;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_idecade[i][j] = 0;
        }
    }

    for (i=0;i<5;i++)
    {
        for (j=0;j<stat_num_safe;j++)
        {
	    occ_iweek[i][j] = 0;
        }
    }
}
	
void make_stat(FILE* sf, char* sday, char* smonth, int iday, int ihour, int iminute, int isecond, int iyear, unsigned int ch, int idx, int* outmonth)
{
    int idate, imonth;

    if (idx == 0)
    {
	init_stat();
    }

    if (!strcmp(sday, "Mon"))      occ_sday[0][stat_idx[ch]] ++;
    else if (!strcmp(sday, "Tue")) occ_sday[1][stat_idx[ch]] ++;
    else if (!strcmp(sday, "Wed")) occ_sday[2][stat_idx[ch]] ++;
    else if (!strcmp(sday, "Thu")) occ_sday[3][stat_idx[ch]] ++;
    else if (!strcmp(sday, "Fri")) occ_sday[4][stat_idx[ch]] ++;
    else if (!strcmp(sday, "Sat")) occ_sday[5][stat_idx[ch]] ++;
    else if (!strcmp(sday, "Sun")) occ_sday[6][stat_idx[ch]] ++;
    else 
    {
	fprintf(sf, "Unknown day: \"%s\"", sday);
	exit(4);
    }

    if (!strcmp(smonth, "Jan"))      { occ_smonth[0][stat_idx[ch]] ++; imonth = 1; }
    else if (!strcmp(smonth, "Feb")) { occ_smonth[1][stat_idx[ch]] ++; imonth = 2; }
    else if (!strcmp(smonth, "Mar")) { occ_smonth[2][stat_idx[ch]] ++; imonth = 3; }
    else if (!strcmp(smonth, "Apr")) { occ_smonth[3][stat_idx[ch]] ++; imonth = 4; }
    else if (!strcmp(smonth, "May")) { occ_smonth[4][stat_idx[ch]] ++; imonth = 5; }
    else if (!strcmp(smonth, "Jun")) { occ_smonth[5][stat_idx[ch]] ++; imonth = 6; }
    else if (!strcmp(smonth, "Jul")) { occ_smonth[6][stat_idx[ch]] ++; imonth = 7; }
    else if (!strcmp(smonth, "Aug")) { occ_smonth[7][stat_idx[ch]] ++; imonth = 8; }
    else if (!strcmp(smonth, "Sep")) { occ_smonth[8][stat_idx[ch]] ++; imonth = 9; }
    else if (!strcmp(smonth, "Oct")) { occ_smonth[9][stat_idx[ch]] ++; imonth = 10; }
    else if (!strcmp(smonth, "Nov")) { occ_smonth[10][stat_idx[ch]] ++; imonth = 11; }
    else if (!strcmp(smonth, "Dec")) { occ_smonth[11][stat_idx[ch]] ++; imonth = 12; }
    else 
    {
	fprintf(sf, "Unknown month: \"%s\"", smonth);
	exit(5);
    }

    *outmonth = imonth;

    occ_iday[iday][stat_idx[ch]] ++;
    occ_ihour[ihour][stat_idx[ch]] ++;
    occ_iminute[iminute][stat_idx[ch]] ++;
    occ_isecond[isecond][stat_idx[ch]] ++;
    occ_iyear[iyear-2010][stat_idx[ch]] ++;

    idate = 32 * 13 * (iyear-2010) + (32*imonth) + iday;
    occ_idate[idate][stat_idx[ch]] ++;

    if (iday <= 10) occ_idecade[0][stat_idx[ch]] ++;
    else if (iday <= 20) occ_idecade[1][stat_idx[ch]] ++;
    else occ_idecade[2][stat_idx[ch]] ++;

    occ_iweek[(iday - 1) / 7][stat_idx[ch]] ++;
}

void summary_stat(FILE* sf)
{
    char sday[7][8], smonth[12][8];
    char sNum[10], sName[10];
    int *sums, i, j, k, ndates, ty, tm, td;
    double perc;
    PtrList *plist, *plist2, *temp;
    StatData* sdata;
    FILE* csv;


    ndates = 100 * 32 * 13;
    sums = (int*)malloc(ndates*sizeof(int));
    plist = plist2 = temp = NULL;

    strcpy(sday[0], "Mon");
    strcpy(sday[1], "Tue");
    strcpy(sday[2], "Wed");
    strcpy(sday[3], "Thu");
    strcpy(sday[4], "Fri");
    strcpy(sday[5], "Sat");
    strcpy(sday[6], "Sun");

    strcpy(smonth[0], "Jan");
    strcpy(smonth[1], "Feb");
    strcpy(smonth[2], "Mar");
    strcpy(smonth[3], "Apr");
    strcpy(smonth[4], "May");
    strcpy(smonth[5], "Jun");
    strcpy(smonth[6], "Jul");
    strcpy(smonth[7], "Aug");
    strcpy(smonth[8], "Sep");
    strcpy(smonth[9], "Oct");
    strcpy(smonth[10], "Nov");
    strcpy(smonth[11], "Dec");

    separator_line(sf, '=');
    for (i=0;i<7;i++)
    {
	sums[i] = 0;
	for (j=0;j<stat_num;j++) sums[i] += occ_sday[i][j];
    }

    csv = fopen("weekdays_summary.csv", "w");
    fprintf(csv, "\"Week Day\";\"Percent\";\"Status\"\n");
    for (k=0;k<stat_num;k++)
    {
	j = stat_order[k];
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);

	for (i=0;i<7;i++)
	{
	    if (occ_sday[i][k] > 0)
	    {
	        perc = ((double)occ_sday[i][k] * 100.) / (double)sums[i];

		sdata = (StatData*)malloc(sizeof(StatData));

		strcpy(sdata->sname, sday[i]);
		sdata->status = (char)j;
		sdata->perc = perc;
		sdata->c = occ_sday[i][k];
		sdata->n = sums[i];

		ptrlist_add_sorted(&plist, (void*)sdata, cmp_statdata);
		ptrlist_add(&plist2, (void*)sdata);
	    }
	}

	temp = plist;

	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Sorted:  \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);

	    temp = temp->next;
	}

	if (plist) separator_line(sf, '-');

	temp = ptrlist_get_tail(plist2);
	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Summary: \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);
	    sprintf(sNum, "%07.3f", sdata->perc);
	    sNum[3] = ',';
	    if (csv) fprintf(csv, "\"%s\";%s;\"%c\"\n", sdata->sname, sNum, sdata->status);

	    temp = temp->prev;
	}

	if (plist2) separator_line(sf, '_');

	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);
    }
    fclose(csv);

    separator_line(sf, '=');
    for (i=0;i<12;i++)
    {
	sums[i] = 0;
	for (j=0;j<stat_num;j++) sums[i] += occ_smonth[i][j];
    }

    csv = fopen("months_summary.csv", "w");
    fprintf(csv, "\"Month\";\"Percent\";\"Status\"\n");
    for (k=0;k<stat_num;k++)
    {
	j = stat_order[k];
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);

	for (i=0;i<12;i++)
	{
	    if (occ_smonth[i][k] > 0)
	    {
	        perc = ((double)occ_smonth[i][k] * 100.) / (double)sums[i];

		sdata = (StatData*)malloc(sizeof(StatData));

		strcpy(sdata->sname, smonth[i]);
		sdata->status = (char)j;
		sdata->perc = perc;
		sdata->c = occ_smonth[i][k];
		sdata->n = sums[i];

		ptrlist_add_sorted(&plist, (void*)sdata, cmp_statdata);
		ptrlist_add(&plist2, (void*)sdata);
	    }
	}

	temp = plist;

	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Sorted:  \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);

	    temp = temp->next;
	}

	if (plist) separator_line(sf, '-');

	temp = ptrlist_get_tail(plist2);
	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Summary: \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);
	    sprintf(sNum, "%07.3f", sdata->perc);
	    sNum[3] = ',';
	    if (csv) fprintf(csv, "\"%s\";%s;\"%c\"\n", sdata->sname, sNum, sdata->status);

	    temp = temp->prev;
	}

	if (plist2) separator_line(sf, '_');
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);
    }
    fclose(csv);

    separator_line(sf, '=');
    for (i=0;i<32;i++)
    {
	sums[i] = 0;
	for (j=0;j<stat_num;j++) sums[i] += occ_iday[i][j];
    }

    sName[2] = 0;
    csv = fopen("monthdays_summary.csv", "w");
    fprintf(csv, "\"Month Day\";\"Percent\";\"Status\"\n");
    for (k=0;k<stat_num;k++)
    {
	j = stat_order[k];
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);

	for (i=0;i<32;i++)
	{
	    if (occ_iday[i][k] > 0)
	    {
	        perc = ((double)occ_iday[i][k] * 100.) / (double)sums[i];

		sdata = (StatData*)malloc(sizeof(StatData));

		sprintf(sdata->sname, "%2d day", i);
		sdata->status = (char)j;
		sdata->perc = perc;
		sdata->c = occ_iday[i][k];
		sdata->n = sums[i];

		ptrlist_add_sorted(&plist, (void*)sdata, cmp_statdata);
		ptrlist_add(&plist2, (void*)sdata);
	    }
	}

	temp = plist;

	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Sorted:  \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);

	    temp = temp->next;
	}

	if (plist) separator_line(sf, '-');

	temp = ptrlist_get_tail(plist2);
	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Summary: \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);
	    sprintf(sNum, "%07.3f", sdata->perc);
	    sNum[3] = ',';
	    strncpy(sName, sdata->sname, 2);
	    if (csv) fprintf(csv, "%s;%s;\"%c\"\n", sName, sNum, sdata->status);

	    temp = temp->prev;
	}

	if (plist2) separator_line(sf, '_');
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);
    }
    fclose(csv);

    separator_line(sf, '=');
    for (i=0;i<24;i++)
    {
	sums[i] = 0;
	for (j=0;j<stat_num;j++) sums[i] += occ_ihour[i][j];
    }

    sName[2] = 0;
    csv = fopen("datehours_summary.csv", "w");
    fprintf(csv, "\"Date Hour\";\"Percent\";\"Status\"\n");
    for (k=0;k<stat_num;k++)
    {
	j = stat_order[k];
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);

	for (i=0;i<24;i++)
	{
	    if (occ_ihour[i][k] > 0)
	    {
	        perc = ((double)occ_ihour[i][k] * 100.) / (double)sums[i];

		sdata = (StatData*)malloc(sizeof(StatData));

		sprintf(sdata->sname, "%2d hour", i);
		sdata->status = (char)j;
		sdata->perc = perc;
		sdata->c = occ_ihour[i][k];
		sdata->n = sums[i];

		ptrlist_add_sorted(&plist, (void*)sdata, cmp_statdata);
		ptrlist_add(&plist2, (void*)sdata);
	    }
	}

	temp = plist;

	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Sorted:  \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);

	    temp = temp->next;
	}

	if (plist) separator_line(sf, '-');

	temp = ptrlist_get_tail(plist2);
	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Summary: \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);
	    sprintf(sNum, "%07.3f", sdata->perc);
	    sNum[3] = ',';
	    strncpy(sName, sdata->sname, 2);
	    if (csv) fprintf(csv, "%s;%s;\"%c\"\n", sName, sNum, sdata->status);

	    temp = temp->prev;
	}

	if (plist2) separator_line(sf, '_');
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);
    }
    fclose(csv);

    sName[4] = 0;
    csv = fopen("years_summary.csv", "w");
    fprintf(csv, "\"Year\";\"Percent\";\"Status\"\n");
    separator_line(sf, '=');
    for (i=0;i<100;i++)
    {
	sums[i] = 0;
	for (j=0;j<stat_num;j++) sums[i] += occ_iyear[i][j];
    }

    for (k=0;k<stat_num;k++)
    {
	j = stat_order[k];
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);

	for (i=0;i<100;i++)
	{
	    if (occ_iyear[i][k] > 0)
	    {
	        perc = ((double)occ_iyear[i][k] * 100.) / (double)sums[i];

		sdata = (StatData*)malloc(sizeof(StatData));

		sprintf(sdata->sname, "%4d year", i+2010);
		sdata->status = (char)j;
		sdata->perc = perc;
		sdata->c = occ_iyear[i][k];
		sdata->n = sums[i];

		ptrlist_add_sorted(&plist, (void*)sdata, cmp_statdata);
		ptrlist_add(&plist2, (void*)sdata);
	    }
	}

	temp = plist;

	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Sorted:  \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);

	    temp = temp->next;
	}

	if (plist) separator_line(sf, '-');

	temp = ptrlist_get_tail(plist2);
	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Summary: \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);
	    sprintf(sNum, "%07.3f", sdata->perc);
	    sNum[3] = ',';
	    strncpy(sName, sdata->sname, 4);
	    if (csv) fprintf(csv, "%s;%s;\"%c\"\n", sName, sNum, sdata->status);

	    temp = temp->prev;
	}

	if (plist2) separator_line(sf, '_');
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);
    }
    fclose(csv);

    csv = fopen("dates_summary.csv", "w");
    fprintf(csv, "\"Date\";\"Percent\";\"Status\"\n");
    separator_line(sf, '=');
    for (i=0;i<ndates;i++)
    {
	sums[i] = 0;
	for (j=0;j<stat_num;j++) sums[i] += occ_idate[i][j];
    }

    for (k=0;k<stat_num;k++)
    {
	j = stat_order[k];
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);

	for (i=0;i<ndates;i++)
	{
	    if (occ_idate[i][k] > 0)
	    {
	        perc = ((double)occ_idate[i][k] * 100.) / (double)sums[i];

		td = i % 32;
		tm = (i / 32) % 13;
		ty = 2010 + (i / (32 * 13));

		sdata = (StatData*)malloc(sizeof(StatData));

		sprintf(sdata->sname, "%04d-%02d-%02d", ty, tm, td);
		sdata->status = (char)j;
		sdata->perc = perc;
		sdata->c = occ_idate[i][k];
		sdata->n = sums[i];

		ptrlist_add_sorted(&plist, (void*)sdata, cmp_statdata);
		ptrlist_add(&plist2, (void*)sdata);
	    }
	}

	temp = plist;

	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Sorted:  \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);

	    temp = temp->next;
	}

	if (plist) separator_line(sf, '-');

	temp = ptrlist_get_tail(plist2);
	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Summary: \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);
	    sprintf(sNum, "%07.3f", sdata->perc);
	    sNum[3] = ',';
	    if (csv) fprintf(csv, "%s;%s;\"%c\"\n", sdata->sname, sNum, sdata->status);

	    temp = temp->prev;
	}

	if (plist2) separator_line(sf, '_');
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);
    }
    fclose(csv);

    separator_line(sf, '=');
    for (i=0;i<3;i++)
    {
	sums[i] = 0;
	for (j=0;j<stat_num;j++) sums[i] += occ_idecade[i][j];
    }

    sName[1] = 0;
    csv = fopen("monthdecades_summary.csv", "w");
    fprintf(csv, "\"Month Decade\";\"Percent\";\"Status\"\n");
    for (k=0;k<stat_num;k++)
    {
	j = stat_order[k];
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);

	for (i=0;i<3;i++)
	{
	    if (occ_idecade[i][k] > 0)
	    {
	        perc = ((double)occ_idecade[i][k] * 100.) / (double)sums[i];

		sdata = (StatData*)malloc(sizeof(StatData));

		sprintf(sdata->sname, "%d decade", i+1);
		sdata->status = (char)j;
		sdata->perc = perc;
		sdata->c = occ_idecade[i][k];
		sdata->n = sums[i];

		ptrlist_add_sorted(&plist, (void*)sdata, cmp_statdata);
		ptrlist_add(&plist2, (void*)sdata);
	    }
	}

	temp = plist;

	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Sorted:  \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);

	    temp = temp->next;
	}

	if (plist) separator_line(sf, '-');

	temp = ptrlist_get_tail(plist2);
	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Summary: \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);
	    sprintf(sNum, "%07.3f", sdata->perc);
	    sNum[3] = ',';
	    sName[0] = sdata->sname[0];
	    if (csv) fprintf(csv, "%s;%s;\"%c\"\n", sName, sNum, sdata->status);

	    temp = temp->prev;
	}

	if (plist2) separator_line(sf, '_');
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);
    }
    fclose(csv);

    separator_line(sf, '=');
    for (i=0;i<5;i++)
    {
	sums[i] = 0;
	for (j=0;j<stat_num;j++) sums[i] += occ_iweek[i][j];
    }

    sName[1] = 0;
    csv = fopen("monthweeks_summary.csv", "w");
    fprintf(csv, "\"Month Week\";\"Percent\";\"Status\"\n");
    for (k=0;k<stat_num;k++)
    {
	j = stat_order[k];
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);

	for (i=0;i<5;i++)
	{
	    if (occ_iweek[i][k] > 0)
	    {
	        perc = ((double)occ_iweek[i][k] * 100.) / (double)sums[i];

		sdata = (StatData*)malloc(sizeof(StatData));

		sprintf(sdata->sname, "%d week", i+1);
		sdata->status = (char)j;
		sdata->perc = perc;
		sdata->c = occ_iweek[i][k];
		sdata->n = sums[i];

		ptrlist_add_sorted(&plist, (void*)sdata, cmp_statdata);
		ptrlist_add(&plist2, (void*)sdata);
	    }
	}

	temp = plist;

	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Sorted:  \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);

	    temp = temp->next;
	}

	if (plist) separator_line(sf, '-');

	temp = ptrlist_get_tail(plist2);
	while (temp)
	{
	    sdata = (StatData*)temp->ptr;
            fprintf(sf, "Summary: \"%s\", status '%c' is %7.3f%% (%8d/%8d)\n", 
		    sdata->sname, sdata->status, sdata->perc, sdata->c, sdata->n);
	    sprintf(sNum, "%07.3f", sdata->perc);
	    sNum[3] = ',';
	    sName[0] = sdata->sname[0];
	    if (csv) fprintf(csv, "%s;%s;\"%c\"\n", sName, sNum, sdata->status);

	    temp = temp->prev;
	}

	if (plist2) separator_line(sf, '_');
	ptrlist_free(&plist, 1);
	ptrlist_free(&plist2, 0);
    }
    fclose(csv);

    separator_line(sf, '=');
}

void readdb()
{
    FILE *fp, *sf, *df, *sqlf, *csvf, *csvr;
    int done, imonth;
    unsigned long lgh, lc, lw, lr, occ[0x100], i, n;
    time_t tm;
    char ch, fname[1024], *tstr;
    double dgh, dc, dw, dr, mgh, mc, mw, mr, migh, mic, miw, mir;
    unsigned long ngh, nc, nw, nr;
    PtrList* h;
    char sday[8], smonth[8], sinv[20];
    int inetq[0x100], iday, ihour, iminute, isecond, iyear, ns, j;

    sf = fopen("stats.txt", "w");
    if (!sf)
    {
	perror("fopen");
	printf("Error creating statistics file: stats.txt\n");
	exit(10);
    }

    sqlf = fopen("data.sql", "w");
    if (!sqlf)
    {
	perror("fopen");
	printf("Error creating SQL data file: data.sql\n");
	exit(16);
    }

    csvf = fopen("data.csv", "w");
    if (!csvf)
    {
	perror("fopen");
	printf("Error creating CSV data file: data.csv\n");
	exit(17);
    }
    fprintf(csvf, "\"Datetime\";\"Timestamp\";\"Net status\";\"Time gethost\";\"Time connect\";\"Time write\";\"Time read\"\n");

    csvr = fopen("readtimes.csv", "w");
    if (!csvf)
    {
	perror("fopen");
	printf("Error creating CSV data file: readtimes.sql\n");
	exit(18);
    }
    fprintf(csvr, "\"Timestamp\";\"Inverted read time\"\n");

    df = fopen("data.txt", "w");
    if (!df)
    {
	perror("fopen");
	printf("Error creating data file: data.txt\n");
	exit(12);
    }

    h = NULL;
    if (usehome)
    {
        sprintf(fname, "%s/.nettest/network.db", hdir);
    }
    else
    {
	strcpy(fname, hdir);
    }

    fp = fopen(fname, "r");

    if (!fp) 
    { 
	perror("fopen");
	printf("Error opening %s\n", fname); 
	exit(2);
    }

/*    printf("Opened database file: %s\n", fname);*/

    for (i=0;i<0x100;i++) occ[i] = inetq[i] = 0;

    done = 0;
    i = 0;
    dgh = dc = dw = dr = 0.;
    ngh = nc = nw = nr = 0;
    mgh = mc = mw = mr = 0;
    migh = mic = miw = mir = 1e10;

    inetq['-'] = 0;
    inetq['C'] = 0;
    inetq['!'] = 1;
    inetq['<'] = 2;
    inetq['+'] = 3;

    while (!done)
    {
        if (fread((void*)&tm, sizeof(time_t), 1, fp) <= 0) { done = 1; break; }
        if (fread((void*)&ch, 1, 1, fp) <= 0) { done = 1; break; }
        if (fread((void*)&lgh, sizeof(unsigned long), 1, fp) <= 0) { done = 1; break; }
        if (fread((void*)&lc, sizeof(unsigned long), 1, fp) <= 0) { done = 1; break; }
        if (fread((void*)&lw, sizeof(unsigned long), 1, fp) <= 0) { done = 1; break; }
        if (fread((void*)&lr, sizeof(unsigned long), 1, fp) <= 0) { done = 1; break; }

	tstr = ctime( &tm );
	tstr[ strlen(tstr) - 1 ] = 0;
/*	"Sun Dec 11 18:21:21 2011"*/

	ns = sscanf(tstr, "%s %s %d %d:%d:%d %d", sday, smonth, &iday, &ihour, &iminute, &isecond, &iyear); 

	if (ns < 7)
	{
	    printf("Error scanning values from \"%s\"\n", tstr);
	    fclose(fp);
	    fclose(sf);
	    fclose(df);
	    exit(3);
	}

	if (iyear < 2010)
	{
	    printf("Warning: wrong date %s\n", tstr);
	    continue;
	}

	make_stat(sf, sday, smonth, iday, ihour, iminute, isecond, iyear, (unsigned int)ch, i, &imonth);

	occ[(unsigned int)ch] ++;

	if (lgh > 0) { dgh += (double)lgh; ngh ++; } 
	if (lc >  0) { dc  += (double)lc;  nc ++; } 
	if (lw >  0) { dw  += (double)lw;  nw ++; } 
	if (lr >  0) { dr  += (double)lr;  nr ++; } 

	if (lgh > 0 && (double)lgh > mgh) mgh = (double)lgh;
	if (lc > 0 && (double)lc > mc) mc = (double)lc;
	if (lw > 0 && (double)lw > mw) mw = (double)lw;
	if (lr > 0 && (double)lr > mr) mr = (double)lr;

	if (lgh > 0 && (double)lgh < migh) migh = (double)lgh;
	if (lc > 0 && (double)lc < mic) mic = (double)lc;
	if (lw > 0 && (double)lw < miw) miw = (double)lw;
	if (lr > 0 && (double)lr < mir) mir = (double)lr;

	i ++;

	fprintf(df, "%s: status: %c %9ldus %9ldus %9ldus %9ldus\n", tstr, ch, lgh, lc, lw, lr);
	fprintf(sqlf, "INSERT INTO M_NETSTATS(M_L_TIME, M_S_DATE, M_S_TIME, M_S_STATUS, M_UTM_GETHOST, M_UTM_CONNECT, M_UTM_WRITE, M_UTM_READ) "
                      "VALUES(%9d, '%04d-%02d-%02d', '%02d:%02d:%02d', '%c', %8ld, %8ld, %8ld, %8ld);\n", 
		      tm, iyear, imonth, iday, ihour, iminute, isecond, ch, lgh, lc, lw, lr);

	fprintf(csvf, "%04d-%02d-%02d %02d:%02d:%02d;%d;%d;%ld;%ld;%ld;%ld\n", 
		iyear, imonth, iday, ihour, iminute, isecond, tm, inetq[(int)ch], lgh, lc, lw, lr);

	if (ch != '+') fprintf(csvr, "%d;0;\n", tm);
	else
	{
	    sprintf(sinv, "%f", 3333333.333333 / (double)lr);
	    ns = strlen(sinv);
	    for (j=0;j<ns;j++) if (sinv[j] == '.') sinv[j] = ',';
	    fprintf(csvr, "%d;%s;\n", tm, sinv);
	}

	ptrlist_add(&h, (void*)((int)ch));
    }

    n = i;
    if (n > 0)
    {
        fprintf(df, "Records: %ld\n", i);
        for (i=0;i<255;i++)
        {
	    if (occ[i] > 0) fprintf(df, "Status: '%c', number: %-9ld/%-9ld\t%-3.4f%%\n", (char)i, occ[i], n, ((double)occ[i] * 100.) / (double)n);
        }

	if (ngh > 0) fprintf(df, "Avg gethostbyname: %3.6fs\n", (dgh / (double)ngh) / 1000000.);
	if (nc  > 0) fprintf(df, "Avg connect:       %3.6fs\n", (dc  / (double)nc)  / 1000000.);
	if (nw  > 0) fprintf(df, "Avg write:         %3.6fs\n", (dw  / (double)nw)  / 1000000.);
	if (nr  > 0) fprintf(df, "Avg read:          %3.6fs\n", (dr  / (double)nr)  / 1000000.);

	if (mgh > 0) fprintf(df, "Max gethostbyname: %3.6fs\n", mgh / 1000000.);
	if (mc  > 0) fprintf(df, "Max connect:       %3.6fs\n", mc  / 1000000.);
	if (mw  > 0) fprintf(df, "Max write:         %3.6fs\n", mw  / 1000000.);
	if (mr  > 0) fprintf(df, "Max read:          %3.6fs\n", mr  / 1000000.);

	if (migh > 0) fprintf(df, "Min gethostbyname: %3.6fs\n", migh / 1000000.);
	if (mic  > 0) fprintf(df, "Min connect:       %3.6fs\n", mic  / 1000000.);
	if (miw  > 0) fprintf(df, "Min write:         %3.6fs\n", miw  / 1000000.);
	if (mir  > 0) fprintf(df, "Min read:          %3.6fs\n", mir  / 1000000.);
    }

    fclose(fp);
    fclose(df);
    printf("Data file: data.txt created\n");

    fclose(sqlf);
    printf("SQL data file: data.sql created\n");

    fclose(csvf);
    printf("CSV data file: data.csv created\n");

    fclose(csvr);
    printf("CSV data file: readtimes.csv created\n");

    summary_stat(sf);
    fclose(sf);
    printf("Statistics file: stats.txt created\n");

#ifndef NOJPEG
    make_jpeg(h, n);
    printf("JPEG: readdb.jpeg created\n");
#endif

    ptrlist_free(&h, 0);

/*    printf("Done\n");*/
}

int main(int lb, char** par)
{
    if (lb == 1)
    {
        hdir = getenv("HOME");
        if (!hdir)
        {
	    printf("Current user is homeless, cannot fetch config\n");
	    exit(3);
        }
	usehome = 1;
    }
    else
    {
	hdir = (char*)malloc((strlen(par[1]) + 1)*sizeof(char));
	strcpy(hdir, par[1]);
	usehome = 0;
    }

    readdb();

    return 0;
}

