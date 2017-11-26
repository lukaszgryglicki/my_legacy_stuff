#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

#define ERR_CANNOTREAD  1
#define ERR_BADJPEG     2
#define ERR_GRAYJPEG    3
#define ERR_256CJPEG    4
#define ERR_CANNOTWRITE 5

#define JQUAL 90

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

void set_rows(int cur_row, JSAMPARRAY pixel_data, int width, unsigned long*** bits)
{
 JSAMPROW ptr;
 int j;
 int r, g, b;
 ptr = pixel_data[0];
 for (j=0;j<width;j++)
   {
    r=GETJSAMPLE(*ptr++);
    g=GETJSAMPLE(*ptr++);
    b=GETJSAMPLE(*ptr++);
    (*bits)[cur_row][j] = RGB(r,g,b);
    }
}

int load_jpeg_file(unsigned long*** bits, int* x, int* y, char* filename)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    FILE* infile;
    JSAMPARRAY buffer;	
    int row_stride;	
    int i;
    if ((infile = fopen(filename, "rb")) == NULL) return ERR_CANNOTREAD;
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    errptr = &jerr;		
    if (setjmp(jerr.setjmp_buffer))
       {
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return ERR_BADJPEG;
       }
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
       {
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return ERR_GRAYJPEG;
       }
    else cinfo.quantize_colors = FALSE;
    jpeg_start_decompress(&cinfo);
    if (cinfo.output_components == 1) 
       {
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return ERR_256CJPEG;
       }
    *x = cinfo.output_width;
    *y = cinfo.output_height;
    *bits = (unsigned long**)calloc(*y, sizeof(unsigned long*));
    for (i=0;i<*y;i++) (*bits)[i] = (unsigned long*)calloc(*x, sizeof(unsigned long));
    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    i = (*y)-1;
    while (cinfo.output_scanline < cinfo.output_height)
       {
	jpeg_read_scanlines(&cinfo, buffer, 1);
	set_rows(i,buffer,*x, bits);
	i--;
       }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
}

void jpeg_compute_pixels(unsigned char* pix, unsigned long** tab, int x, int y)
{
 int i,j;
 unsigned char col[3];
 for (i=0;i<y;i++)
 for (j=0;j<x;j++)
 {
/*        printf("(%d,%d): %d\n", j, i, tab[i][j]);*/
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

void translate_jpeg_file(unsigned long** v, int cx, int cy, double* f)
{
 int i,j;
 unsigned char r, g, b;
 double nr, ng, nb;

 double RR, RG, RB, GR, GG, GB, BR, BG, BB;
 double tFR, tTR, tFG, tTG, tFB, tTB;

 RR = f[0];
 RG = f[1];
 RB = f[2];
 GR = f[3];
 GG = f[4];
 GB = f[5];
 BR = f[6];
 BG = f[7];
 BB = f[8];
 tFR = f[9]  * 255.;
 tTR = f[10] * 255.;
 tFG = f[11] * 255.;
 tTG = f[12] * 255.;
 tFB = f[13] * 255.;
 tTB = f[14] * 255.;

 for (i=0;i<cy;i++)
 for (j=0;j<cx;j++)
 {
     dRGB(&r, &g, &b, v[i][j]);

     nr = RR * (double)r + RG * (double)g + RB * (double)b;
     ng = GR * (double)r + GG * (double)g + GB * (double)b;
     nb = BR * (double)r + BG * (double)g + BB * (double)b;

     if (nr < tFR) nr = tFR;
     if (ng < tFG) ng = tFG;
     if (nb < tFB) nb = tFB;

     if (nr > tTR) nr = tTR;
     if (ng > tTG) ng = tTG;
     if (nb > tTB) nb = tTB;

     r = (unsigned char)nr;
     g = (unsigned char)ng;
     b = (unsigned char)nb;

     v[i][j] = RGB(r, g, b);
 }
}

int trajpeg(char* fn1, char* fn2, double* f)
{
    int err, cx, cy;
    unsigned long** bits;

    err = load_jpeg_file(&bits, &cx, &cy, fn1);
    if (err) 
    {
	printf("Cannot load %s\n", fn1);
	return -1;
    }

    translate_jpeg_file(bits, cx, cy, f);

    err = save_jpeg_file(bits, cx, cy, fn2);
    if (err) 
    {
	printf("Cannot save %s\n", fn2);
	return -2;
    }

    return 0;
}

/* Filter */
/* R filer:  R := r * RR + g * RG + b * RB */
/* G filter: G := r * GR + g * GG + b * GB */
/* B filter: B := r * BR + g * BG + b * BB */

void usage(char* prog)
{
    printf("%s infile.jpg outfile.jpg RR RG RB GR GG GB BR BG BB tFR tTR tFG tTG tFB tTB\n", prog);
}


int main(int lb, char** par)
{
    double f[15];

    if (lb < 18) usage(par[0]);
    else 
    {
    	f[0] = atof(par[3]);
    	f[1] = atof(par[4]);
    	f[2] = atof(par[5]);
    	f[3] = atof(par[6]);
    	f[4] = atof(par[7]);
    	f[5] = atof(par[8]);
    	f[6] = atof(par[9]);
    	f[7] = atof(par[10]);
    	f[8] = atof(par[11]);
    	f[9] = atof(par[12]);
    	f[10] = atof(par[13]);
    	f[11] = atof(par[14]);
    	f[12] = atof(par[15]);
    	f[13] = atof(par[16]);
    	f[14] = atof(par[17]);

	return trajpeg(par[1], par[2], f);
    }
    return 1;
}


