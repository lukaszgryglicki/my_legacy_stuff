#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>
#define ERR_CANNOTREAD 1
#define ERR_BADJPEG    2
#define ERR_GRAYJPEG   3
#define ERR_256CJPEG   4
extern int _Z6ARGB24iii(int,int,int);
extern int _Z6ARGB16iii(int,int,int);
#define RGB _Z6ARGB16iii 
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
    *bits = calloc(*y, sizeof(unsigned long*));
    for (i=0;i<*y;i++) (*bits)[i] = calloc(*x, sizeof(unsigned long));
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

