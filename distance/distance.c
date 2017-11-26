#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <setjmp.h>

#define ERR_CANNOTREAD  1
#define ERR_BADJPEG     2
#define ERR_GRAYJPEG    3
#define ERR_256CJPEG    4
#define ERR_CANNOTWRITE 5

#define JQUAL 100

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

__inline unsigned long RGB(int r, int g, int b)
{
    return (r << 0x10) + (g << 0x8) + b;
}

__inline void dRGB(unsigned char* r, unsigned char* g, unsigned char* b, unsigned long v)
{
    *r = (v & 0xff0000) >> 0x10;
    *g = (v & 0xff00) >> 0x8;
    *b = v & 0xff;
}

void set_rows(int cur_row, JSAMPARRAY pixel_data, int width, unsigned long*** bits)
{
 JSAMPROW ptr;
 register volatile int j;
 register volatile int r, g, b;
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
 register volatile int i,j;
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
 /* speed */
 free_pixels(&pixels);
 fclose(outfile);
 return 0;
}

int distance(char* fn1, char* fn2)
{
    int err;
    unsigned long **bits1, **bits2, **obits;
    int x1, x2, y1, y2, x, y;

    err = load_jpeg_file(&bits1, &x1, &y1, fn1);
    if (err)
    {
	printf("Cannot load file: %s\n", fn1);
	return 1;
    }

    err = load_jpeg_file(&bits2, &x2, &y2, fn2);
    if (err)
    {
	printf("Cannot load file: %s\n", fn2);
	return 1;
    }

    if (x1 != x2 || y1 != y2)
    {
	printf("resolition mismatch: %dx%d vs %dx%d\n", x1, y1, x2, y2);
	return 1;
    }

    x = x1;
    y = y1;

    obits = bits1;

    err = save_jpeg_file(obits, x, y, "distance.jpeg");
    if (err)
    {
	printf("Cannot save file: %s\n", "distance.jpeg");
	return 1;
    }

    return 0;
}

int main(int lb, char** par)
{
    if (lb < 3)
    {
	printf("usage: %s file1.jpeg file2.jpeg\n", par[0]);
	return 1;
    }
    return distance(par[1], par[2]);
}

