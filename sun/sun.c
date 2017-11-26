#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TILT 23.45
#define PI 3.141592653589793
#define DECEMBER_OFFSET 14400.0

void setcolor(unsigned char* r, unsigned char* g, unsigned char* b, double v);
void setcolor_edge(unsigned char* r, unsigned char* g, unsigned char* b, double v);

typedef struct _timestruct
{
	long year, month, day, hour, minute, second, microsecond;

} timestruct;

typedef struct _vector
{
	double x, y, z;
} vector;

#ifdef USEJPEG
#define JQUAL 100

#ifndef NOJPEG
#include <jpeglib.h>
#include <setjmp.h>
#define ERR_CANNOTREAD 1
#define ERR_BADJPEG    2
#define ERR_GRAYJPEG   3
#define ERR_256CJPEG   4
#endif


static int img_counter = 0;

struct my_error_mgr
{
 struct jpeg_error_mgr pub;
 jmp_buf setjmp_buffer;
};
typedef struct my_error_mgr * my_error_ptr;
static my_error_ptr errptr = NULL;

static void my_error_exit (j_common_ptr cinfo)
{
 my_error_ptr myerr = (my_error_ptr) cinfo->err;
 (*cinfo->err->output_message) (cinfo);
 longjmp(myerr->setjmp_buffer, 1);
}

void jpeg_compute_pixels(unsigned char* pix, double** tab, int x, int y)
{
 int i,j;
 unsigned char col[3];
 for (i=0;i<y;i++)
 for (j=0;j<x;j++)
 {
	setcolor_edge(&col[0], &col[1], &col[2], tab[j][i]);
    	pix[3*(x*((y-i)-1)+j)] = col[0];
	pix[3*(x*((y-i)-1)+j)+1] = col[1];
	pix[3*(x*((y-i)-1)+j)+2] = col[2];
 }
}

void init_pixels(unsigned char** pix, double** tab, int x, int y)
{
    *pix = (unsigned char*)malloc((x*y*3)*sizeof(unsigned char));
    jpeg_compute_pixels(*pix, tab, x, y);
}

void free_pixels(unsigned char** pix)
{
 free(*pix);
 *pix = NULL;
}


int save_jpeg_file(double** tab, int x, int y, FILE* outfile)
{
 struct jpeg_compress_struct cinfo;
 struct my_error_mgr jerr;
 unsigned char* pixels;
 JSAMPROW row_pointer[1];
 int row_stride;
 pixels = NULL;
 
 init_pixels(&pixels, tab, x, y);
 cinfo.err = jpeg_std_error(&jerr.pub);
 jerr.pub.error_exit = my_error_exit;
 errptr = &jerr;
 if (setjmp(jerr.setjmp_buffer))
 {
	jpeg_destroy_compress(&cinfo);
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
 return 0;
}

void write_output_jpg(double** tab, long sizex, long sizey, double lat, double lon)
{
 FILE* plik;
 
 char fname[100];
/* sprintf(fname, "sun-%f-%f.jpeg", lat, lon);*/
 sprintf(fname, "sun-%06d.jpeg", img_counter ++);
 
 plik = fopen(fname,"wb");
 if (!plik) 
 { 
	 printf("Error writing JPG: %s\n", fname); 
	 return ; 
 }

 save_jpeg_file(tab, sizex, sizey, plik);

 fclose(plik);
}

#endif


long getmonthdays(long month, long year)
{
	switch (month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			if (year % 4) return 28;
			else return 29;
	}
	return 0;
}

void setdate(timestruct* t, long year, long month, long day, long hour, long minute, long second, long microsecond)
{
	long maxdays;

	t->year			= year;
	if (month < 1)  month = 1;
	if (month > 12) month = 12;
	t->month		= month;
	if (day < 1) day = 1;
	maxdays = getmonthdays(t->month, t->year);
	if (day > maxdays) day = maxdays;
	t->day			= day;
	if (hour < 0)  hour = 0;
	if (hour > 23) hour = 23;
	t->hour			= hour;
	if (minute < 0) minute = 0;
	if (minute > 59) minute = 59;
	t->minute		= minute;
	if (second < 0) second = 0;
	if (second > 59) second = 59;
	t->second		= second;
	if (microsecond < 0) microsecond = 0;
	if (microsecond > 999999) microsecond = 999999;
	t->microsecond	= microsecond;
}

long getdaynumber(timestruct* t)
{
	long i, m, s;
	m = t->month;
	s = 0;
	for (i=1;i<m;i++) s += getmonthdays(i, t->year);
	s += t->day - 1;
	return s;
}

long getyeardays(timestruct* t)
{
	if (t->year % 4) return 365;
	else return 366;
}

void rotatez(vector* v, double angle)
{
	double x, y;
	angle /= 180./PI;
	x = cos(angle) * v->x  + sin(angle) * v->y;
	y = -sin(angle) * v->x + cos(angle) * v->y;
	v->x = x;
	v->y = y;
}

void rotatey(vector* v, double angle)
{
	double x, z;
	angle /= 180./PI;
	x = cos(angle) * v->x - sin(angle) * v->z;
	z = sin(angle) * v->x + cos(angle) * v->z;
	v->x = x;
	v->z = z;
}

void normalize(vector* v)
{
	double len;
	len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x /= len;
	v->y /= len;
	v->z /= len;
}

double dot(vector* v, vector* w)
{
	normalize(v);
	normalize(w);
	return v->x * w->x + v->y * w->y + v->z * w->z;
}

/* latitude given in degrees from -90 (90S) to 90 (90N) */
/* longitude given in degrees from -180 (180W) to 180 (180E) */
double calculate_sun_position(double latitude, double longitude, timestruct t)
{
	vector pos_radius, sun_radius;
	double hour_angle, day_angle, scalar_product, angle;

	/*Initial positions*/
	sun_radius.x = -1.;			/* from obserwer to sun (at december 21/22th) */
	sun_radius.y = 0.;
	sun_radius.z = 0.;
	pos_radius.x = 1.;			/* from earth centre to obserwer (at midnight) */
	pos_radius.y = 0.;
	pos_radius.z = 0.;

	/* Calculate hour earth rotation angle, and suns rotation (day of year)
	   Daily rotation angle */
	hour_angle = t.microsecond + 1000000. * t.second + 60000000. * t.minute + 3600000000. * t.hour;
	hour_angle /= 240000000.;
	/* Yearly rotation angle */
	day_angle = DECEMBER_OFFSET;
	day_angle += 1440 * getdaynumber(&t) + 60* t.hour + t.minute;
	day_angle /= getyeardays(&t) * 4.;

	/*Apply hour earth rotation */
	rotatez(&pos_radius, -latitude);
	rotatey(&pos_radius, longitude);
	rotatey(&pos_radius, day_angle);
	rotatey(&pos_radius, hour_angle);
	rotatez(&pos_radius, TILT);
	
	
	rotatey(&sun_radius, day_angle);

	scalar_product = dot(&pos_radius, &sun_radius);
	angle = 90. - (acos(scalar_product) * 180./PI);

	return angle;
}

void setcolor_edge(unsigned char* r, unsigned char* g, unsigned char* b, double v)
{
	if (v < -200.) *r = *g = *b = 0x00;
	else if (v >= -18.5 && v <= -17.5)
	{
	    *r = 0x00;
	    *g = 0x00;
	    *b = 0x5f;
	}
	else if (v >= -12.5 && v <= -11.5)
	{
	    *r = 0x00;
	    *g = 0x00;
	    *b = 0xaf;
	}
	else if (v >= -5.5 && v <= -5.5)
	{
	    *r = 0x00;
	    *g = 0x00;
	    *b = 0xff;
	}
	else if (v >= -.5 && v <= .5)
	{
	    *r = 0x00;
	    *g = 0xff;
	    *b = 0x00;
	}
	else if (v >= 5.5 && v <= 6.5)
	{
	    *r = 0x5f;
	    *g = 0x00;
	    *b = 0x00;
	}
	else if (v >= 11.5 && v <= 12.5)
	{
	    *r = 0xaf;
	    *g = 0x00;
	    *b = 0x00;
	}
	else if (v >= 17.5 && v <= 18.5)
	{
	    *r = 0xff;
	    *g = 0x00;
	    *b = 0x00;
	}
	else if (v >= 59.5 && v <= 60.5)
	{
	    *r = 0xff;
	    *g = 0xff;
	    *b = 0x00;
	}
	else
	{
		*r = 0xff;
		*g = 0xff;
		*b = 0xff;
		return;
	}
}

void setcolor(unsigned char* r, unsigned char* g, unsigned char* b, double v)
{
	unsigned char res;
	if (v < -200.) *r = *g = *b = 0x00;
	else if (v >= -200.0 && v < -18.) 
	{
		*r = 0;
		*g = 0;
		*b = 0x3f;
		return;
	}
	else if (v >= -18. && v < -0.5)
	{
		*r = 0;
		*g = 0;
		*b = (unsigned char)(63. + 192. * ((v + 18.)/17.5));
		return;
	}
	else if (v >= -0.5 && v <= 0.5)
	{
		*r = 0;
		*g = 0xff;
		*b = 0;
		return;
	}
	else if (v > 0.5 && v < 10.)
	{
		*r = (unsigned char)(91. + 164. * ((v - .5)/9.5));
		*g = 0;
		*b = 0;
		return;
	}
	else if (v >= 10. && v < 40.)
	{
		*r = 0xff;
		res = (unsigned char)(15. + 240. * ((v - 10.)/30.));
		*g = res;
		*b = 0;
		return;
	}
	else if (v >= 40. && v < 65.)
	{
		*r = 0xff;
		*g = 0xff;
		res = (unsigned char)(15. + 240. * ((v - 40.)/25.));
		*b = res;
		return;
	}
	else
	{
		*r = 0xff;
		*g = 0xff;
		*b = 0xff;
		return;
	}
	
	/*
	else if (v < -12.0) 
	{
		*r = 0;
		*g = 0;
		*b = 0x6f;
	}
	else if (v < -6.0) 
	{
		*r = 0;
		*g = 0;
		*b = 0x9f;
	}
	else if (v < -0.5) 
	{
		*r = 0;
		*g = 0;
		*b = 0xcf;
	}
	else if (v < 0.5) 
	{
		*r = 0xff;
		*g = 0;
		*b = 0;
	}
	else if (v < 6) 
	{
		*r = 0xcf;
		*g = 0x7f;
		*b = 0;
	}
	else if (v < 60) 
	{
		*r = 0xff;
		*g = 0xff;
		*b = 0;
	}
	else
	{
		*r = 0xff;
		*g = 0xff;
		*b = 0xff;
	}
	*/
	
}

void write_output_ppm(double** tab, long sizex, long sizey, double lat, double lon)
{
 FILE* plik;
 
 char fname[100];
 int i,j;
 unsigned char col[3];
 double v;
/* sprintf(fname, "sun-%f-%f.ppm", lat, lon);*/
 sprintf(fname, "sun-%06d.ppm", img_counter ++);
 
 plik = fopen(fname,"wb");
 if (!plik) 
 { 
	 printf("Error writing PPM: %s\n", fname); 
	 return ; 
 }
 fprintf(plik,"P6\n%d %d\n255\n",(int)sizex, (int)sizey);
 for (i=sizey;i>=0;i--)  for (j=0;j<sizex;j++)
	{
		v = tab[j][i];
		setcolor_edge(&col[0], &col[1], &col[2], v);
		fwrite((const void*)col, 3, 1, plik);
	}
 fclose(plik);
}

void calculations(double lat, double lon)
{
	timestruct t;
	int i,j,k,l,n,nd,xsize,ysize,idx1,idx2;
	double **tab, v;
	t.year = 2008;
	nd = getyeardays(&t);
	xsize = (nd + 11);
	ysize = 24 * 30 + 23;
	tab = (double**)malloc((xsize + 1) * sizeof(double*));
	for (i=0;i<xsize;i++)
	{
		tab[i] = (double*)malloc((ysize + 1) * sizeof(double));
		for (j=0;j<ysize;j++) tab[i][j] = -1000.;

	}

	for (i=1;i<=12;i++)
	{
		n = getmonthdays(i, t.year);
		for (j=1;j<=n;j++)
		{
			setdate(&t, t.year, i, j, 0, 0, 0, 0);
			idx1 = getdaynumber(&t) + i-1;
			for (k=0;k<24;k++) for (l=0;l<60;l+=2)
			{
				idx2 = 31*k + l/2;
				setdate(&t, t.year, i, j, k, l, 0, 0);
				v = calculate_sun_position(lat, lon, t);
				tab[idx1][idx2] = v;
			}
		}
	}
#ifdef USEJPEG
	write_output_jpg(tab, xsize, ysize, lat, lon);
#else
	write_output_ppm(tab, xsize, ysize, lat, lon);
#endif
	for (i=0;i<xsize;i++)
	{
		free((void*)tab[i]);
		tab[i] = NULL;
	}

	free((void*)tab);
	tab = NULL;

}

int main(int argc, char** argv)
{
	double lat1, lon1;
	double lat2, lon2;
	double step, x, y, perc;

	if (argc < 6)
	{
		printf("%s lat1 lat2 lon1 lon2 step\n", argv[0]);
		return 0;
	}
	lat1 = atof(argv[1]);
	lat2 = atof(argv[2]);
	lon1 = atof(argv[3]);
	lon2 = atof(argv[4]);
	step = atof(argv[5]);

	for (y=lat1;y<=lat2;y+=step)
	for (x=lon1;x<=lon2;x+=step)
	{
	    	if (lat2 > lat1) perc = ((y-lat1)/(lat2-lat1)) * 100.;
		else perc = 100.;
		printf("\r\r\r\r\r\r\r\r%3.3f%%", perc);
		fflush(stdout);		
		calculations(y, x);
	}
	printf("\n");
	return 0;
}
