#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "fpar.h"

#ifdef MTHR
#include <pthread.h>
#endif

#ifdef MM
#include <sys/mman.h>
#endif

#ifdef MPNG
#include <png.h>
#define PNG_DEBUG 3
#endif

#ifdef MJPEG
#include <jpeglib.h>
#include <setjmp.h>
#define ERR_CANNOTREAD  1
#define ERR_BADJPEG     2
#define ERR_GRAYJPEG    3
#define ERR_256CJPEG    4
#define ERR_CANNOTWRITE 5
int jqual = 100;
#endif

#ifdef MUSECASE
#define mystrstr strcasestr
#else
#define mystrstr strstr
#endif


#ifdef MASM
int ualoop = 1;
#endif

int overwrite_mode = 1;

#ifdef MASM
extern unsigned long asmf1(unsigned long);
extern unsigned long asmf2(unsigned long, unsigned long);
extern unsigned long asmf3(unsigned long, unsigned long, unsigned long);

/* MODE, N, X, Y, CT, NT, PTR*** iv, PTR** ov */
extern void loop_asm(int, int, int, int, int, int, void*, void*);

/* unsigned char* pix, unsigned long** tab, int x, int y) */
extern void compute_asm(void* pix, void* tab, int x, int y);
#endif

void translate_file(int n, unsigned long*** iv, unsigned long** ov, int x, int y, int ct, int nt, int udfmode);

typedef struct _BMPTag
{
 char ident[2];
 int fsize;
 int dummy;
 int offset;
 int dummy2;
 int bm_x;
 int bm_y;
 short planes;
 short bpp;
 int compress;
 int nbytes;
 int no_matter[4];
} BMPTag;

#ifdef MJPEG
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
#endif

typedef struct _thread_args
{
    int n, ocx, ocy, ct, nt, udfmode;
    unsigned long ***bits, **obits;
} thread_args;

typedef struct _img_cache
{
    char* fn;
    unsigned long** bits;
    int x, y;

} img_cache;

img_cache* icache;
int ncache;

void alloc_cache(int n)
{
/*    printf("Allocated %d element cache\n", n);*/
    ncache = 0;
    icache = (img_cache*)malloc(n*sizeof(img_cache));
}

int lookup_cache(char* fn, unsigned long*** bits, int* x, int* y)
{
    int i;

    for (i=0;i<ncache;i++)
    {
	if (strcmp(fn, icache[i].fn) == 0)
	{
#ifndef SILENT
	    printf("File %s already in cache at idx: %d\n", fn, i);
#endif
	    *bits = icache[i].bits;
	    *x    = icache[i].x;
	    *y    = icache[i].y;

	    return 0;
	}
    }

/*    printf("File %s not found in cache\n", fn);*/
    return 1;
}

void insert_cache(char* fn, unsigned long** bits, int x, int y)
{
    icache[ncache].fn = malloc((strlen(fn)+1)*sizeof(char));

    strcpy(icache[ncache].fn, fn);
    icache[ncache].x = x;
    icache[ncache].y = y;
    icache[ncache].bits = bits;

    ncache ++;
/*    printf("Num entires in cache: %d\n", ncache);*/
}

#ifdef MM
int fmmf(FILE* fp, void** op, off_t* siz)
{
    struct stat sb;
    int fd;
    void* p;

    *op  = NULL;
    *siz = 0;
/*    return 1;*/

    fd = fileno(fp);
    if (fstat(fd, &sb) < 0)
    {
	perror("fstat");
	return 1;
    }
              
    p = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE/* | MAP_PREFAULT_READ*/, fd, 0);
    if (p == MAP_FAILED) 
    {
	perror ("mmap");
        return 2; 
    }

#ifndef SILENT
    printf("%d (size=%lld), mmapped to %p\n", fd, sb.st_size, p);
#endif

    *op = p;
    *siz = sb.st_size;

    return 0;
}
#endif

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

#ifdef MJPEG
void set_jpeg_rows_array(int cur_row, int n_rows, JSAMPARRAY pixel_data, int width, unsigned long** bits, int gs)
{
 JSAMPROW ptr;
 register volatile int i, j, r;

 if (gs)
 {
     for (i=0;i<n_rows;i++)
     {
         ptr = pixel_data[i];
         for (j=0;j<width;j++)
         {
	     r = GETJSAMPLE(*ptr++);
	     bits[cur_row-i][j] = (r << 0x10) + (r << 0x8) + r;
         }
     }
 }
 else
 {
     for (i=0;i<n_rows;i++)
     {
         ptr = pixel_data[i];
         for (j=0;j<width;j++)
         {
	     r = GETJSAMPLE(*ptr++) << 0x10;
	     r += GETJSAMPLE(*ptr++) << 0x8;
	     bits[cur_row-i][j] = r + GETJSAMPLE(*ptr++);
         }
     }
 }
}

void set_jpeg_rows(int cur_row, JSAMPARRAY pixel_data, int width, unsigned long*** bits)
{
 JSAMPROW ptr;
 register volatile int j;
 register volatile int r, g, b;
 ptr = pixel_data[0];
 printf("slow\n");
 for (j=0;j<width;j++)
   {
    r=GETJSAMPLE(*ptr++);
    g=GETJSAMPLE(*ptr++);
    b=GETJSAMPLE(*ptr++);
    (*bits)[cur_row][j] = RGB(r,g,b);
    }
}
#endif

/*****************************************/
#ifdef MPNG
void set_pngrow_pointers(int x, int y, png_bytep* rows, unsigned long** bits)
{
 	register volatile int i, j, i2;
 	png_byte* row;

	i2 = y;
 	for (i=0;i<y;i++)
 	{
		i2 --;
     		row = rows[i];
     		for (j=0;j<x;j++)
     		{
		        row[j*3+2] = (bits[i2][j] & 0xff);
		        row[j*3+1] = (bits[i2][j] & 0xff00) >> 0x8;
		        row[j*3]   = (bits[i2][j] & 0xff0000) >> 0x10;
			/*
			printf("%d(%d-%d): %02x %02x %02x <-- bits[%d][%d]: %08x\n",
				i, 3*j, 3*j+2, 
				(int)(row[j*3] & 0xff), 
				(int)(row[j*3+1] & 0xff), 
				(int)(row[j*3+2] & 0xff), 
				i, j, bits[i][j]);
			*/
     		}
 	}
}


void set_pngrows_array(int x, int y, png_bytep* rows, unsigned long** bits, int dim)
{
 register volatile int i, j, r, i2;
 png_byte* row;

 i2 = y;
 if (dim == 4) /* RGBA */
 {
 	for (i=0;i<y;i++)
 	{
	    	i2 --;
     		row = rows[i];
     		for (j=0;j<x;j++)
     		{
	 		r 		=  (row[(j<<2)]   & 0xff) << 0x10;
	 		r 		+= (row[(j<<2)+1] & 0xff) << 0x8;
	 		r 		+=  row[(j<<2)+2] & 0xff;
	 		bits[i2][j] 	= r;
     		}
 	}
 }
 else if (dim == 3) /* RGB */
 {
 	for (i=0;i<y;i++)
 	{
	    	i2 --;
     		row = rows[i];
     		for (j=0;j<x;j++)
     		{
	 		r 		=  (row[3*j]   & 0xff) << 0x10;
	 		r 		+= (row[3*j+1] & 0xff) << 0x8;
	 		r 		+=  row[3*j+2] & 0xff;
	 		bits[i2][j] 	= r;
/*			printf("RGB: bits[%d][%d] <-- %06x (from: %d(%d-%d))\n", i, j, r, i, 3*j, 3*j+2);*/
     		}
 	}
 }
 else if (dim == 1) /* GREY */
 {
 	for (i=0;i<y;i++)
 	{
	    	i2 --;
     		row = rows[i];
     		for (j=0;j<x;j++)
     		{
	 		r 		= row[j] & 0xff;
	 		bits[i2][j] 	= (r << 0x10) + (r << 0x8) + r;
/*			printf("RGB: bits[%d][%d] <-- %06x (from: %02x %d(%d))\n", i, j, bits[i][j], r, i, j);*/
     		}
 	}
 }


 /*
        for (y=0; y<height; y++) {
                png_byte* row = row_pointers[y];
                for (x=0; x<width; x++) {
                        png_byte* ptr = &(row[x*4]);
                        printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
                               x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

                        // set red value to 0 and green value to the blue one
                        ptr[0] = 0;
                        ptr[1] = ptr[2];
                }
        }
 */
}
#endif

void init_bmp(BMPTag* b)
{
 	int i;
 	b->ident[0]='B';
 	b->ident[1]='M';
 	b->fsize=0;
 	b->dummy=0;
 	b->offset=sizeof(BMPTag);
 	b->bm_x=b->bm_y=0x20;
 	b->dummy2=40;
 	b->bpp=0x18;
 	b->planes=1;
 	b->compress=0;
 	b->nbytes=3*32*32;
 	for (i=0;i<4;i++) b->no_matter[i]=0;
}

int load_bmp_file(unsigned long*** bits, int* x, int* y, char* file_name)
{
 BMPTag bm_handle;
 int i, j, width, height, bpp;
 unsigned char r,g,b,a;
 FILE* plik;
 void* mem;
 unsigned char* mbuf;
#ifdef MM
 off_t siz;
#endif
/* int i2;*/

 if (lookup_cache(file_name, bits, x, y) == 0) return 0;

 plik = fopen(file_name, "rb");
 if (!plik)
 {
     printf("load_bmp: cannot open: %s\n", file_name);
     return 1;
 }

 mem = NULL;
#if MM
 if (fmmf(plik, &mem, &siz) != 0) printf("load_bmp: warning: mmap file failed, using normal buffered I/O: %s\n", file_name);
#endif

 init_bmp(&bm_handle);

 i = fscanf(plik,"%c%c",&r,&g);
 if (i != 2) 
 {
	 printf("load_bmp: file %s is truncated, skipping\n", file_name);
#ifdef MM
	 if (mem) munmap(mem, siz);
#endif
	 fclose(plik);
	 return 2;
 }

 if ((r != 'B' || g != 'M'))
 {
	 printf("load_bmp: file %s doesn't start with BM - not recognized as a bitmap\n", file_name);
#ifdef MM
	 if (mem) munmap(mem, siz);
#endif
	 fclose(plik);
	 return 3;
 }

 fread(&bm_handle.fsize,4,1,plik);
 fread(&bm_handle.dummy,4,1,plik);
 fread(&bm_handle.offset,4,1,plik);
 fread(&bm_handle.dummy2,4,1,plik);
 fread(&bm_handle.bm_x,4,1,plik);
 fread(&bm_handle.bm_y,4,1,plik);
 fread(&bm_handle.planes,2,1,plik);
 fread(&bm_handle.bpp,2,1,plik);
 bpp = bm_handle.bpp;

 if (bpp != 8 && bpp != 24 && bpp != 32) 
 {
	 printf("Only 8,24,32 BPP BMPs suported: current bpp: %d\n", bm_handle.bpp);
#ifdef MM
	 if (mem) munmap(mem, siz);
#endif
	 fclose(plik);
	 return 4;
 }

 fseek(plik,bm_handle.offset,SEEK_SET);

 width  = bm_handle.bm_x;
 height = bm_handle.bm_y;
	
 *bits = (unsigned long**)calloc(height, sizeof(unsigned long*));
 for (i=0;i<height;i++) (*bits)[i] = (unsigned long*)calloc(width, sizeof(unsigned long));

 mbuf = (unsigned char*)mem;

 if (mbuf)
 {
/*        printf("mem\n");*/
        fclose(plik);
	plik = NULL;
        mbuf += bm_handle.offset;

 	if (bpp == 8)
 	{
 		for (i=0;i<height;i++)  
 		{
     			for (j=0;j<width;j++)
     			{
				r = *mbuf++;
	 			(*bits)[i][j] = (r << 0x10) + (r << 0x8) + r;
     			}
		}
 	}
 	else if (bpp == 24)
 	{
 		for (i=0;i<height;i++)  
 		{
     			for (j=0;j<width;j++)
     			{
				b = *mbuf++;
				g = *mbuf++;
				r = *mbuf++;
	 			(*bits)[i][j] = (r << 0x10) + (g << 0x8) + b;
     			}
		}
 	}
 	else if (bpp == 32)
 	{
 		for (i=0;i<height;i++)  
 		{
     			for (j=0;j<width;j++)
     			{
				mbuf++;
				b = *mbuf++;
				g = *mbuf++;
				r = *mbuf++;
	 			(*bits)[i][j] = (r << 0x10) + (g << 0x8) + b;
     			}
		}
 	}
 }
 else
 {
 	if (bpp == 8)
 	{
 		for (i=0;i<height;i++)  
 		{
     			for (j=0;j<width;j++)
     			{
         			fscanf(plik,"%c", &r);
	 			(*bits)[i][j] = (r << 0x10) + (r << 0x8) + r;
     			}
		}
 	}
 	else if (bpp == 24)
 	{
 		for (i=0;i<height;i++)  
 		{
     			for (j=0;j<width;j++)
     			{
         			fscanf(plik,"%c%c%c", &b,&g,&r);
	 			(*bits)[i][j] = (r << 0x10) + (g << 0x8) + b;
     			}
		}
 	}
 	else if (bpp == 32)
 	{
 		for (i=0;i<height;i++)  
 		{
     			for (j=0;j<width;j++)
     			{
         			fscanf(plik,"%c%c%c%c", &a,&b,&g,&r);
	 			(*bits)[i][j] = (r << 0x10) + (g << 0x8) + b;
     			}
		}
 	}

	fclose(plik);
 }

 *x     = width;
 *y     = height;
	
#ifdef MM
 if (mem) munmap(mem, siz);
#endif
 insert_cache(file_name, *bits, *x, *y);

 return 0;

}

#ifdef MPNG
int load_png_file(unsigned long*** bits, int* x, int* y, char* file_name)
{
	int i, width, height, dim;
	png_byte ct/*, bit_depth*/;
	void* mem;
	png_structp png_ptr;
	png_infop info_ptr;
/*	int number_of_passes;*/
	png_bytep* row_pointers;
#ifdef MM
	off_t siz;
#endif
        png_byte header[8]; 
	FILE* fp;

        if (lookup_cache(file_name, bits, x, y) == 0) return 0;

        /* open file and test for it being a png */
        fp = fopen(file_name, "rb");
        if (!fp)
	{
                printf("pngio: file %s could not be opened for reading", file_name);
		return 1;
	}

	mem = NULL;
#ifdef MM
        if (fmmf(fp, &mem, &siz) != 0) printf("load_png: warning: mmap file failed, using normal buffered I/O: %s\n", file_name);
#endif

        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8))
	{
                printf("pngio: File %s is not recognized as a PNG file", file_name);
#ifdef MM
	 	if (mem) munmap(mem, siz);
#endif
		fclose(fp);
		return 2;
	}

        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
	{
                printf("pngio: png_create_read_struct failed");
#ifdef MM
	 	if (mem) munmap(mem, siz);
#endif
		fclose(fp);
		return 3;
	}

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
	{
                printf("pngio: png_create_info_struct failed");
#ifdef MM
	 	if (mem) munmap(mem, siz);
#endif
		fclose(fp);
		return 4;
	}

        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: error during init_io");
#ifdef MM
	 	if (mem) munmap(mem, siz);
#endif
		fclose(fp);
		return 5;
	}

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width  = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        ct     = png_get_color_type(png_ptr, info_ptr);
/*        bit_depth = png_get_bit_depth(png_ptr, info_ptr);*/

/*        number_of_passes = png_set_interlace_handling(png_ptr);*/
        png_read_update_info(png_ptr, info_ptr);
/*	printf("bit_depth: %d, color_type: %d, num_of_passes: %d\n", bit_depth, ct, number_of_passes);*/

        /* read file */
        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: error during read_image");
#ifdef MM
	 	if (mem) munmap(mem, siz);
#endif
		fclose(fp);
		return 6;
	}

        if (ct == PNG_COLOR_TYPE_RGB)
	{
	    dim = 3;
	}
	else if (ct == PNG_COLOR_TYPE_RGBA)
	{
	    dim = 4;
	}
	else if (ct == PNG_COLOR_TYPE_GRAY)
	{
	    dim = 1;
	}
	else
	{
	    printf("pngio: unsupported color type: %d\n", ct);
#ifdef MM
	    if (mem) munmap(mem, siz);
#endif
	    fclose(fp);
	    return 7;
	}


        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for (i=0;i<height;i++)
	{
/*                row_pointers[i] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));*/
                row_pointers[i] = (png_byte*)malloc(sizeof(png_byte) * width * dim);
	}

        png_read_image(png_ptr, row_pointers);
        fclose(fp);

	*x = width;
	*y = height;
    
	*bits = (unsigned long**)calloc(*y, sizeof(unsigned long*));
	for (i=0;i<height;i++) (*bits)[i] = (unsigned long*)calloc(*x, sizeof(unsigned long));

	set_pngrows_array(*x, *y, row_pointers, *bits, dim);

        for (i=0;i<height;i++) free(row_pointers[i]);
        free(row_pointers);

#ifdef MM
	if (mem) munmap(mem, siz);
#endif
        insert_cache(file_name, *bits, *x, *y);

	return 0;
}
#endif

int save_bmp_file(unsigned long** bits, int x, int y, char* file_name)
{
 BMPTag bm_handle;
 FILE* plik;
 int i, j;
 unsigned char r, g, b;

 init_bmp(&bm_handle);

 plik = fopen(file_name, "wb");
 if (!plik)
 {
     printf("save_bmp: error writing file: %s\n", file_name);
     return 1;
 }

 fprintf(plik,"%c%c", 'B','M');

 bm_handle.bm_y = y;
 bm_handle.bm_x = x;
 bm_handle.fsize = sizeof(BMPTag)+(y*x*3);

 fwrite(&bm_handle.fsize,4,1,plik);
 fwrite(&bm_handle.dummy,4,1,plik);

 bm_handle.offset=sizeof(BMPTag);
 bm_handle.planes=1;
 bm_handle.bpp=24;
 bm_handle.nbytes = x*y*3;

 fwrite(&bm_handle.offset,4,1,plik);
 fwrite(&bm_handle.dummy2,4,1,plik);
 fwrite(&bm_handle.bm_x,4,1,plik);
 fwrite(&bm_handle.bm_y,4,1,plik);
 fwrite(&bm_handle.planes,2,1,plik);
 fwrite(&bm_handle.bpp,2,1,plik);
 fwrite(&bm_handle.compress,4,1,plik);
 fwrite(&bm_handle.nbytes,4,1,plik);
 for (i=0;i<4;i++)  fwrite(&bm_handle.no_matter[i],4,1,plik);

 fseek(plik,bm_handle.offset,SEEK_SET);
 for (i=0;i<y;i++)  for (j=0;j<x;j++)
 {
       r = (bits[i][j] & 0xff0000) >> 0x10;
       g = (bits[i][j] & 0xff00) >> 0x8;
       b =  bits[i][j] & 0xff;
       fprintf(plik,"%c%c%c", b, g, r);
 }

 fclose(plik);
 return 0;
}

#ifdef MPNG
int save_png_file(unsigned long** bits, int x, int y, char* file_name)
{
	png_structp png_ptr;
	png_infop info_ptr;
	FILE* fp;
	int i, width, height, dim;
	png_byte color_type, bit_depth;
	png_bytep* row_pointers;

        /* create file */
        fp = fopen(file_name, "wb");
        if (!fp)
	{
                printf("pngio: File %s could not be opened for writing", file_name);
		return 1;
	}

        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
	{
                printf("pngio: png_create_write_struct failed");
		fclose(fp);
		return 2;
	}

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
	{
                printf("pngio: png_create_info_struct failed");
		fclose(fp);
		return 3;
	}

        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during init_io");
		fclose(fp);
		return 4;
	}

        png_init_io(png_ptr, fp);

        /* write header */
        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during writing header");
		fclose(fp);
		return 5;
	}

	width  = x;
	height = y;
	bit_depth = 8;
	color_type = PNG_COLOR_TYPE_RGB;
	dim = 3;

        png_set_IHDR(png_ptr, info_ptr, width, height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);

        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during writing bytes");
		fclose(fp);
		return 6;
	}

        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for (i=0;i<height;i++)
	{
                row_pointers[i] = (png_byte*)malloc(sizeof(png_byte) * (width + 9) * dim);	/* to allow lazy 64 bit assembler copyout */
	}

	set_pngrow_pointers(x, y, row_pointers, bits);

        png_write_image(png_ptr, row_pointers);

        /* end write */
        if (setjmp(png_jmpbuf(png_ptr)))
	{
                printf("pngio: Error during end of write");
		fclose(fp);
		return 7;
	}

        png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
        for (i=0;i<height;i++) free(row_pointers[i]);
        free(row_pointers);

        fclose(fp);

	return 0;
}
/*****************************************/
#endif

#ifdef MJPEG
int load_jpeg_file(unsigned long*** bits, int* x, int* y, char* filename)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    FILE* infile;
    JSAMPARRAY buffer;	
    int row_stride;	
    int i, l, gs;
    void* mem;
    off_t siz;
 
    if (lookup_cache(filename, bits, x, y) == 0) return 0;

    if ((infile = fopen(filename, "rb")) == NULL) return ERR_CANNOTREAD;

    mem = NULL;
#ifdef MM
    if (fmmf(infile, &mem, &siz) != 0) printf("load_jpg: warning: mmap file failed, using normal buffered I/O: %s\n", filename);
#else
    siz = 0;
#endif

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    errptr = &jerr;		
    if (setjmp(jerr.setjmp_buffer))
       {
	jpeg_destroy_decompress(&cinfo);
#ifdef MM
	if (mem) munmap(mem, siz);
#endif
	fclose(infile);
	printf("setjmp error\n");
	return ERR_BADJPEG;
       }
    jpeg_create_decompress(&cinfo);

    if (mem) 
    {
/*	printf("mem\n");*/
	jpeg_mem_src(&cinfo, (unsigned char*)mem, (unsigned long)siz);
	fclose(infile);
	infile = NULL;
    }
    else
    {
    	jpeg_stdio_src(&cinfo, infile);
    }
    jpeg_read_header(&cinfo, TRUE);
    gs = 0;
    if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
       {
	gs = 1;
/*	jpeg_destroy_decompress(&cinfo);*/
/*	if (infile) fclose(infile);*/
/*	return ERR_GRAYJPEG;*/
#ifndef SILENT
	printf("warning: grayscale jpeg\n");
#endif
       }
    else cinfo.quantize_colors = FALSE;
    jpeg_start_decompress(&cinfo);
/*    if (cinfo.output_components == 1) */
/*       {*/
/*	jpeg_destroy_decompress(&cinfo);*/
/*	if (infile) fclose(infile);*/
/*	return ERR_256CJPEG;*/
/*	printf("warning: jpeg with components=1\n");*/
/*       }*/
    *x = cinfo.output_width;
    *y = cinfo.output_height;
    *bits = (unsigned long**)calloc(*y, sizeof(unsigned long*));
    for (i=0;i<*y;i++) (*bits)[i] = (unsigned long*)calloc(*x, sizeof(unsigned long));
    row_stride = cinfo.output_width * cinfo.output_components;
    /*
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    i = (*y)-1;
    while (cinfo.output_scanline < cinfo.output_height)
       {
	jpeg_read_scanlines(&cinfo, buffer, 1);
	set_jpeg_rows(i,buffer,*x, bits);
	i--;
       }
    */

    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 0x20);
    i = (*y)-1;
    while (cinfo.output_scanline < cinfo.output_height)
       {
	   l = jpeg_read_scanlines(&cinfo, buffer, 0x20);
	   set_jpeg_rows_array(i, l, buffer,*x, *bits, gs);
	   i-=l;
/*	   printf("l=%d, i=%d, y=%d, sline=%d, h=%d\n", l, i, *y, cinfo.output_scanline, cinfo.output_height);*/
       }
/*    printf("%p (%d x %d) %d:%d\n", buffer, *x, *y, cinfo.output_scanline, cinfo.output_height);*/

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

#ifdef MM
    if (mem) munmap(mem, siz);
#endif
    insert_cache(filename, *bits, *x, *y);

    if (infile) fclose(infile);
    return 0;
}
#endif

void compute_pixels(unsigned char* pix, unsigned long** tab, int x, int y)
{
 register volatile int i,j, i2;
 unsigned char col[3];

 i2 = y;
 for (i=0;i<y;i++)
 {
     i2 --;
     for (j=0;j<x;j++)
     {
/*        printf("(%d,%d): %d\n", j, i, tab[i][j]);*/
        dRGB(&col[0], &col[1], &col[2], tab[i][j]);
    	pix[3*(x*i2+j)]   = col[0];
	pix[3*(x*i2+j)+1] = col[1];
	pix[3*(x*i2+j)+2] = col[2];
     }
 }
}

void init_pixels(unsigned char** pix, unsigned long** tab, int x, int y)
{
    *pix = (unsigned char*)malloc((x*y*3+9)*sizeof(unsigned char));	/* +9 to allow lazy assembler copyout */
#ifdef MASM
    if (ualoop)
    {
/*	printf("pix=%d pix[0]=%d, tab=%d tab[0]=%d tab[0][0]=%d\n",*/
/*		(int)(*pix), (int)((*pix)[0]), (int)tab, (int)tab[0], (int)tab[0][0]);*/
	compute_asm(*pix, tab, x, y);
    }
    else
    {
        compute_pixels(*pix, tab, x, y);
    }
#else
    compute_pixels(*pix, tab, x, y);
#endif
}

void free_pixels(unsigned char** pix)
{
 free(*pix);
 *pix = NULL;
}

#ifdef MJPEG
int save_jpeg_file(unsigned long** tab, int x, int y, char* filename)
{
 struct jpeg_compress_struct cinfo;
 struct my_error_mgr jerr;
 unsigned char* pixels;
 JSAMPROW* row_pointer;
 int row_stride, i;
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
 jpeg_set_quality(&cinfo, jqual, FALSE);
 jpeg_start_compress(&cinfo, TRUE);

 row_stride = x*3;
 row_pointer = (JSAMPROW*)malloc(y*sizeof(JSAMPROW));
 for (i=0;i<y;i++) row_pointer[i] = &pixels[row_stride*i];
 i = jpeg_write_scanlines(&cinfo, row_pointer, y);
 if (i < y)
 {
     printf("Failed to write jpeg scanlines: written: %d/%d\n", i, y);
     fclose(outfile);
     return 1;
 }

 jpeg_finish_compress(&cinfo);
 jpeg_destroy_compress(&cinfo);
 /* speed */
 /* free_pixels(&pixels);*/
 /* free(row_pointer); */
 fclose(outfile);
#ifndef SILENT
 printf("Saved jpeg at quality: %d\n", jqual);
#endif
 return 0;
}

int save_jpeg_file_old(unsigned long** tab, int x, int y, char* filename)
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
 jpeg_set_quality(&cinfo, jqual, FALSE);
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
 /* free_pixels(&pixels);*/
 fclose(outfile);
 return 0;
}
#endif


void translate_file(int n, unsigned long*** iv, unsigned long** ov, int x, int y, int ct, int nt, int udfmode)
{
     register volatile int i, j;
     int z, pi, pj, ni, nj;
#ifndef SILENT
     double perc;
#endif
     double **yv, xv;
#ifdef MASM
     struct timeval at1, at2, atuse;
#endif

     if (udfmode)
     {
#ifdef MASM
	 if (ualoop && (udfmode == 4 || udfmode == 11 || udfmode == 12))
	 {
	     /* MODE, N, X, Y, CT, NT, PTR*** iv, PTR** ov */
/*	     printf("Assembler call\n");*/
#ifndef SILENT
	     printf("Using singlethreaded fast assembler loop mode\n");
#endif
    	     gettimeofday(&at1, NULL);
	     loop_asm(udfmode, n, x, y, ct, nt, (void*)iv, (void*)ov);
    	     gettimeofday(&at2, NULL);
#ifdef MUSETIMER
             timersub(&at2, &at1, &atuse);
#endif
             printf("%d.%06ds ", (int)atuse.tv_sec, (int)atuse.tv_usec);

	     /*
             for (i=0;i<y;i++) for (j=0;j<x;j++)
	     {
		 printf("[%d][%d]: asmf1(%x) = %x\n", i, j, iv[0][i][j], ov[i][j]);
	     }
	     */

	     return;
	 }
#endif

	 z = (x + y) >> 1;

         for (i=ct;i<y;i+=nt)
         {
#ifndef SILENT
	     perc = (100. * (double)i) / (double)y;
	     if (!ct)
	     {
	         for (z=0;z<20;z++) printf("\b");
	         printf("Converting: %.4f%%", perc);
	         fflush(stdout);
	     }
#endif

#ifdef MASM
	     if (udfmode == 4)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = asmf1(iv[0][i][j]);
                 }
	     }
	     else if (udfmode == 11)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = asmf2(iv[0][i][j], iv[1][i][j]);
                 }
	     }
	     else if (udfmode == 12)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = asmf3(iv[0][i][j], iv[1][i][j], iv[2][i][j]);
                 }
	     }
	     else if (udfmode == 1)
#else
	     if (udfmode == 1)
#endif
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = (unsigned long)udf1(iv[0][i][j]);
                 }
	     }
	     else if (udfmode == 2)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = (unsigned long)udf2(iv[0][i][j], iv[1][i][j]);
                 }
	     }
	     else if (udfmode == 3)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = (unsigned long)udf3(iv[0][i][j], iv[1][i][j], iv[2][i][j]);
                 }
	     }
	     else if (udfmode == 5)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = udfi1(iv[0][i][j]);
                 }
	     }
	     else if (udfmode == 6)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = udfi2(iv[0][i][j], iv[1][i][j]);
                 }
	     }
	     else if (udfmode == 7)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = udfi3(iv[0][i][j], iv[1][i][j], iv[2][i][j]);
                 }
	     }
	     else if (udfmode == 8)
	     {
		 if (i > 0) pi = i - 1;
		 else pi = y - 1;

                 for (j=0;j<x;j++)
                 {
		     if (j > 0) pj = j - 1;
		     else pj = x - 1;

	             ov[i][j] = udfi2(iv[0][i][j], iv[0][pi][pj]);
                 }
	     }
	     else if (udfmode == 9)
	     {
		 if (i > 0) pi = i - 1;
		 else pi = y - 1;

                 for (j=0;j<x;j++)
                 {
		     if (j > 0) pj = j - 1;
		     else pj = x - 1;

	             ov[i][j] = udfi3(iv[0][i][j], iv[0][pi][pj], z);
                 }
	     }
	     else if (udfmode == 10)
	     {
                 for (j=0;j<x;j++)
                 {
	             ov[i][j] = udfi6(iv[0][i][j], iv[1][i][j], iv[2][i][j], iv[3][i][j], iv[4][i][j], iv[5][i][j]);
                 }
	     }
         }

#ifndef SILENT
         if (!ct)
         {
             perc = 100.;
             for (z=0;z<20;z++) printf("\b");
             printf("\nConverted: %.4f%%\n", perc);
         }
#endif

	 return;
     }

     yv = (double**)malloc(9 * sizeof(double*));
     for (i=0;i<9;i++) yv[i] = (double*)malloc((n+1) * sizeof(double));
     xv = (double)n;

     yv[0][n] = (double)(rand() % 0x1000000);

     yv[2][n] = (double)x;
     yv[4][n] = (double)y;
     yv[5][n] = (double)ct;
     yv[6][n] = (double)nt;

#ifndef SILENT
     if (!ct) printf("\nCall equation: %dK times\n", (x * y) / 1000);
#endif

     for (i=ct;i<y;i+=nt)
     {

         yv[3][n] = (double)i;

#ifndef SILENT
	 perc = (100. * (double)i) / (double)y;
	 if (!ct)
	 {
	     for (z=0;z<20;z++) printf("\b");
	     printf("Converting: %.4f%%", perc);
	     fflush(stdout);
	 }
#endif

         for (j=0;j<x;j++)
         {
	     ni = i + 1;
	     nj = j + 1;
	     pi = i - 1;
	     pj = j - 1;

	     if (ni == y) ni = 0;
	     if (nj == x) nj = 0;
	     if (pi == -1) pi = y - 1; 
	     if (pj == -1) pj = x - 1; 

	     for (z=0;z<n;z++) yv[0][z] = (double)iv[z][i][j];
	     for (z=0;z<n;z++) yv[1][z] = (double)iv[z][pi][pj];
	     for (z=0;z<n;z++) yv[2][z] = (double)iv[z][pi][j];
	     for (z=0;z<n;z++) yv[3][z] = (double)iv[z][pi][nj];
	     for (z=0;z<n;z++) yv[4][z] = (double)iv[z][i][pj];
	     for (z=0;z<n;z++) yv[5][z] = (double)iv[z][i][nj];
	     for (z=0;z<n;z++) yv[6][z] = (double)iv[z][ni][pj];
	     for (z=0;z<n;z++) yv[7][z] = (double)iv[z][ni][j];
	     for (z=0;z<n;z++) yv[8][z] = (double)iv[z][ni][nj];

             yv[1][n] = (double)j;

	     ov[i][j] = (unsigned long)fpar_f(ct, xv, yv);
/*	     printf("%ld\n", ov[i][j]);*/
         }
     }

     for (i=0;i<9;i++) free((void*)yv[i]);
     free((void*)yv);

#ifndef SILENT
     if (!ct)
     {
         perc = 100.;
         for (z=0;z<20;z++) printf("\b");
         printf("\nConverted: %.4f%%\n", perc);
     }
#endif
}

void tests()
{
    char* eq;
    char ieq[1024];
    int n;
    double y, r, **yv;

    n = 1;
    strcpy(ieq, "1-(y1^y1)");
    eq = (char*)malloc((strlen(ieq) + 3) * sizeof(char));

    strcpy(eq, ieq);
    strcat(eq, "\n");
    fpar_init(1);
    fpar_function(0, n, 0, eq);
    if (!fpar_ok(0)) 
    { 
	printf("Function not accepted.\n"); 
	return; 
    }

    yv = (double**)malloc(sizeof(double*));
    yv[0] = (double*)malloc(n * sizeof(double));

    for (y=0.0;y<=1.1;y+=0.05)
    {
	yv[0][0] = y;
	r = fpar_f(0, 0., yv);
	printf("f(%f) = %f\n", y, r);
    }

    free((void*)yv[0]);
    free((void*)yv);

    fpar_deinit();
}

void* convert_thread(void* ptr)
{
    int n, ocx, ocy, ct, nt, udfmode;
    unsigned long ***bits, **obits;
    thread_args* vargs;

    vargs = (thread_args*)ptr;

    n = vargs->n;
    ocx = vargs->ocx;
    ocy = vargs->ocy;
    ct = vargs->ct;
    nt = vargs->nt;
    bits = vargs->bits;
    obits = vargs->obits;
    udfmode = vargs->udfmode;

#ifndef SILENT
    printf("\nThread %d/%d converting\n", ct+1, nt);
#endif
    
    translate_file(n, bits, obits, ocx, ocy, ct, nt, udfmode);
#ifndef SILENT
    printf("\nThread %d/%d finished\n", ct+1, nt);
#endif

    free((void*)vargs);

    return NULL;
}

int fjpg(char* ofn, char* ieq, int n, char** ifn, int nt)
{
    int err, *cx, *cy, ocx, ocy, i, z, udfmode;
    unsigned long ***bits, **obits;
#ifndef SILENT
    double perc;
    time_t tm;
#endif
    char* eq;
    FILE* fp;
#ifdef MTHR
    pthread_t *thr;
#endif
    thread_args args, *vargs;
#ifdef MUSETIMER
    struct timeval tod1, tod2, tuse;
#ifndef SILENT
    gettimeofday(&tod1, NULL);
#endif
#endif


    if (nt < 1)
    {
	printf("No work threads, exiting\n");
	return 0;
    }

#ifndef MTHR
    nt = 1;
#endif

    alloc_cache(n);

    eq = (char*)malloc((strlen(ieq) + 3) * sizeof(char));
    bits = (unsigned long***)malloc(n * sizeof(unsigned long**));
    cx = (int*)malloc(n * sizeof(int));
    cy = (int*)malloc(n * sizeof(int));
#ifdef MTHR
    thr = (pthread_t*)malloc(nt * sizeof(pthread_t));
#endif
    ocx = 0xFFFF;
    ocy = 0xFFFF;
    udfmode = 0;

    if (!strcmp(ieq, "none"))                 udfmode = 13;
#ifdef MASM
    else if (!strcmp(ieq, "asmf1"))           udfmode = 4;
    else if (!strcmp(ieq, "asmf2") && n >= 2) udfmode = 11;
    else if (!strcmp(ieq, "asmf3") && n >= 3) udfmode = 12;
#endif
    else if (!strcmp(ieq, "udf1") && n >= 1)  udfmode = 1;
    else if (!strcmp(ieq, "udfi1") && n >= 1) udfmode = 5;
    else if (!strcmp(ieq, "udf2") && n >= 2)  udfmode = 2;
    else if (!strcmp(ieq, "udfi2") && n >= 2) udfmode = 6;
    else if (!strcmp(ieq, "udf3") && n >= 3)  udfmode = 3;
    else if (!strcmp(ieq, "udfi3") && n >= 3) udfmode = 7;
    else if (!strcmp(ieq, "udfpi2"))          udfmode = 8;
    else if (!strcmp(ieq, "udfpi3"))          udfmode = 9;
    else if (!strcmp(ieq, "udfi6") && n >= 6) udfmode = 10;
#ifdef MASM
    else if (!strcmp(ieq, "asmf2") && n < 2) 
    {
	printf("To use asmf2 fast mode, you need to set at least 2 input images\n");
	return 32;
    }
    else if (!strcmp(ieq, "asmf3") && n < 3) 
    {
	printf("To use asmf3 fast mode, you need to set at least 3 input images\n");
	return 32;
    }
#endif
    else if (!strcmp(ieq, "udf2") && n < 2) 
    {
	printf("To use udf2 fast mode, you need to set at least 2 input images\n");
	return 32;
    }
    else if (!strcmp(ieq, "udf3") && n < 3) 
    {
	printf("To use udf3 fast mode, you need to set at least 3 input images\n");
	return 64;
    }
    else if (!strcmp(ieq, "udfi2") && n < 2) 
    {
	printf("To use udfi2 fast mode, you need to set at least 2 input images\n");
	return 32;
    }
    else if (!strcmp(ieq, "udfi3") && n < 3) 
    {
	printf("To use udfi3 fast mode, you need to set at least 3 input images\n");
	return 64;
    }
    else if (!strcmp(ieq, "udfi6") && n < 6) 
    {
	printf("To use udfi6 fast mode, you need to set at least 6 input images\n");
	return 64;
    }

#ifdef MASM
    if (ualoop && (udfmode == 4 || udfmode == 11 || udfmode == 12))
    {
#ifndef SILENT
	printf("Switing to full assembler processing, singlethreaded call\n");
#endif
	nt = 1;
    }
#endif


#ifndef SILENT
    time(&tm);
    srand((long)tm);

    if (nt > 1) printf("Starting MT version, threads: %d, output: %s\n", nt, ofn);
    else printf("Starting single thread version, output: %s\n", ofn);
#endif

    fp = fopen(ofn, "r");
    if (fp)
    {
	if (overwrite_mode)
	{
#ifndef SILENT
	    printf("warning: overwritting file %s\n", ofn);
#endif
	    fclose(fp);
	}
	else
	{
	    printf("File %s exists, not overwriting\n", ofn);
	    fclose(fp);
	    return 1;
	}
    }

    if (!udfmode)
    {
#ifndef SILENT
        printf("Testing equation\n");
#endif
        strcpy(eq, ieq);
        strcat(eq, "\n");

        fpar_init(nt);
        for (z=0;z<nt;z++) fpar_function(z, n+1, 8, eq);

        if (!fpar_ok(0)) 
        { 
	    printf("Function not accepted.\n"); 
	    return 2; 
        }
    }
#ifndef SILENT
    else 
    {
	printf("Use fast call mode %s\n", ieq);
    }
#endif

#ifndef SILENT
    printf("Loading input JPGs\n");
#endif

    for (i=0;i<n;i++)
    {
#ifndef SILENT
	perc = (100. * (double)i) / (double)n;
	for (z=0;z<20;z++) printf("\b");
	printf("Load JPGs: %.3f%%", perc);
	fflush(stdout);
#endif

	if (mystrstr(ifn[i], ".jpg") || mystrstr(ifn[i], ".jpeg"))
	{
#ifdef MJPEG
	    err = load_jpeg_file(&bits[i], &cx[i], &cy[i], ifn[i]);
#else
	    printf("Error: JPG not compiled in\n");
	    err = 1;
#endif
	}
	else if (mystrstr(ifn[i], ".png"))
	{
#ifdef MPNG
	    err = load_png_file(&bits[i], &cx[i], &cy[i], ifn[i]);
#else
	    printf("Error: PNG not compiled in\n");
	    err = 1;
#endif
	}
	else if (mystrstr(ifn[i], ".bmp"))
	{
	    err = load_bmp_file(&bits[i], &cx[i], &cy[i], ifn[i]);
	}
	else
	{
	    err = 1;
	    printf("Unknown file type: %s, supported are JPG, PNG, BMP\n", ifn[i]);
	}

        if (err) 
        {
	    printf("Cannot load %s\n", ifn[i]);
	    return 4;
        }

	if (cx[i] < ocx) 
	{
	    if (ocx < 0xFFFF) printf("\nWarning X-resolution downgrade: %d --> %d\n", ocx, cx[i]);
	    ocx = cx[i];
	}
	else if (cx[i] > ocx) printf("\nWarning X-resolution bigger than actual: %d > %d\n", cx[i], ocx);

	if (cy[i] < ocy)
	{
	    if (ocy < 0xFFFF) printf("\nWarning Y-resolution downgrade: %d --> %d\n", ocy, cy[i]);
	    ocy = cy[i];
	}
	else if (cy[i] > ocy) printf("\nWarning Y-resolution bigger than actual: %d > %d\n", cy[i], ocy);

/*	printf("Loaded (%04d/%04d): %s --> (%d x %d)\n", i+1, n, ifn[i], ocx, ocy);*/
    }

#ifndef SILENT
    perc = 100.0;
    for (z=0;z<20;z++) printf("\b");
    printf("Loaded JPGs: %.2f%%", perc);
    printf("\n");
    
    printf("Output resolution: %dx%d, memory: %dKb, %dMb\n", ocx, ocy, (ocx * ocy) >> 8, (ocx * ocy) >> 18);
#endif

    obits = (unsigned long**)calloc(ocy, sizeof(unsigned long*));
    for (i=0;i<ocy;i++) obits[i] = (unsigned long*)calloc(ocx, sizeof(unsigned long));
    
#ifndef SILENT
    if (!udfmode) printf("Using equation\n%s\n", ieq);
#endif

    if (nt > 1 && udfmode != 13)
    {
        args.n = n;
        args.bits = bits;
        args.obits = obits;
        args.ocx = ocx;
        args.ocy = ocy;
        args.nt = nt;
	args.udfmode = udfmode;

        for (z=0;z<nt;z++)
        {
            args.ct = z;
	    vargs = (thread_args*)malloc(sizeof(thread_args));
	    memcpy((void*)vargs, (void*)&args, sizeof(thread_args));
#ifdef MTHR
            if (pthread_create(&thr[z], NULL, convert_thread, (void*)vargs)) 
	    {
	        printf("Failed to create %d/%d thread\n", z+1, nt);
	        return 8;
	    }
#else
	    convert_thread((void*)vargs);
#endif
        }

#ifdef MTHR
        for (z=0;z<nt;z++)
        {
#ifndef SILENT
	    printf("Wait for thread %d/%d\n", z+1,nt);
#endif
	    pthread_join(thr[z], NULL);
#ifndef SILENT
	    printf("Finished thread %d/%d\n", z+1,nt);
#endif
        }
#endif
    }
    else
    {
    	if (udfmode == 13) 
    	{
#ifndef SILENT
		printf("none mode: translating input to output without image data change\n");
#endif
		obits = bits[0];
    	}
    	else
    	{
        	translate_file(n, bits, obits, ocx, ocy, 0, 1, udfmode);
    	}
    }

    if (!udfmode)
    {
        fpar_deinit();
    }

#ifndef SILENT
    printf("Saving to: %s\n", ofn);
#endif

    if (mystrstr(ofn, ".jpg") || mystrstr(ofn, ".jpeg"))
    {
#ifdef MJPEG
    	err = save_jpeg_file(obits, ocx, ocy, ofn);
#else
	printf("Error: saving to JPG not compiled in.\n");
	err = 1;
#endif
    }
    else if (mystrstr(ofn, ".png"))
    {
#ifdef MPNG
    	err = save_png_file(obits, ocx, ocy, ofn);
#else
	printf("Error: saving to PNG not compiled in.\n");
	err = 1;
#endif
    }
    else if (mystrstr(ofn, ".bmp"))
    {
    	err = save_bmp_file(obits, ocx, ocy, ofn);
    }
    else
    {
        err = 1;
        printf("Unknown file type: %s, supported are JPG, PNG, BMP\n", ofn);
    }

    if (err) 
    {
	printf("Cannot save %s\n", ofn);
	return 16;
    }

#ifndef SILENT
    printf("Saved %s (%dx%d)\n", ofn, ocx, ocy);
#endif

#ifdef MUSETIMER
    gettimeofday(&tod2, NULL);
    timersub(&tod2, &tod1, &tuse);
#ifndef SILENT
    printf("Computation took %d.%06ds\n", (int)tuse.tv_sec, (int)tuse.tv_usec);
#else
/*    printf(".");*/
/*    fflush(stdout);*/
#endif
    printf("%s ", ofn);
#endif


    return 0;
}
    
void separate_options(int lb, char** par, int* lb_o, char*** par_o, int* lb_f, char*** par_f)
{
    int i, l, t0, t1;
    char c;
    short *t;
    
    t = (short*)malloc(lb*sizeof(short));
    *lb_o = *lb_f = 1;

    for (i=1;i<lb;i++)
    {
	l = strlen(par[i]);
	if (l < 2)
	{
	    (*lb_f) ++;
	    t[i] = 0;	/* 0 - opcja nie getopt */
	    continue;
	}

	if (par[i][0] != '-')
	{
	    (*lb_f) ++;
	    t[i] = 0;
	    continue;
	}

	c = par[i][1];

	if ((c >= 'a' && c < 'x') || (c >= 'A' && c < 'X'))
	{
	    if ((l > 2 && par[i][2] >= '0' && par[i][2] <= '9') || l == 2)
	    {
	    	(*lb_o) ++;
	    	t[i] = 1;
	    }
	    else
	    {
		(*lb_f) ++;
		t[i] = 0;
	    }
	}
    }

    *par_f = (char**)malloc(*lb_f*sizeof(char*));
    *par_o = (char**)malloc(*lb_o*sizeof(char*));

    (*par_f)[0] = (char*)malloc((strlen(par[0])+1)*sizeof(char));
    (*par_o)[0] = (char*)malloc((strlen(par[0])+1)*sizeof(char));

    strcpy((*par_f)[0], par[0]);
    strcpy((*par_o)[0], par[0]);

    t0 = t1 = 1;
    for (i=1;i<lb;i++)
    {
	if (t[i])
	{
	    (*par_o)[t1] = (char*)malloc((strlen(par[i])+1)*sizeof(char));
	    strcpy((*par_o)[t1], par[i]);
	    t1 ++;
	}
	else
	{
	    (*par_f)[t0] = (char*)malloc((strlen(par[i])+1)*sizeof(char));
	    strcpy((*par_f)[t0], par[i]);
	    t0 ++;
	}
    }
/*
    printf("lb_f = %d\n", *lb_f);
    for (i=0;i<t0;i++) printf("par_f[%d] = \"%s\"\n", i, (*par_f)[i]);

    printf("lb_o = %d\n", *lb_o);
    for (i=0;i<t1;i++) printf("par_o[%d] = \"%s\"\n", i, (*par_o)[i]);
*/
}

void usage(char* prog)
{
    printf("%s outfile.jpg 'equation' n_threads infile(s).jpg [...] [-a|-n|-q val]\n", prog);
    printf("\t-a\tdisable faster but singlethread assembler loop processing\n");
    printf("\t-n\tdisable automatic output file overwrite\n");
    printf("\t-q\tjpeg quality[100]\n");
    fpar_info();
}

int main(int lb, char** par)
{
    int n, i, r;
    char **infiles, u;

    int lb_f, lb_o;
    char **par_f, **par_o;
#ifdef MUSETIMER
    struct timeval at1, at2, atuse;
    gettimeofday(&at1, NULL);
#endif
    
    separate_options(lb, par, &lb_o, &par_o, &lb_f, &par_f);
/*
    printf("lb_f = %d\n", lb_f);
    for (i=0;i<lb_f;i++) printf("par_f[%d] = \"%s\"\n", i, par_f[i]);

    printf("lb_o = %d\n", lb_o);
    for (i=0;i<lb_o;i++) printf("par_o[%d] = \"%s\"\n", i, par_o[i]);
*/
    while ((u = getopt(lb_o, par_o,"hanq:"))!=-1)
    {
	switch (u)
	{
#ifdef MASM
		case 'a': ualoop = 0;		break;
#else
		case 'a': printf("Warning: ASM mode not compiled in.\n"); break;
#endif
		case 'n': overwrite_mode = 0;	break;
#ifdef MJPEG
		case 'q': jqual = atoi(optarg);	break;
#else
		case 'q': printf("Warning: JPEG not compiled in, setting quality is meanless\n"); break;
#endif
		case 'h': usage(par_f[0]); 	return 0;
		default: printf("Unrecognized option\n");       return 1;
	}
    }

    if (lb_f < 4) usage(par_f[0]);
    else 
    {
	n = lb_f - 4;
	infiles = (char**)malloc(n * sizeof(char*));
	for (i=4;i<lb_f;i++) 
	{
	    infiles[i-4] = (char*)malloc((strlen(par_f[i]) + 1) * sizeof(char));
	    strcpy(infiles[i-4], par_f[i]);
	}

#ifndef SILENT
	printf("Using %d input files\n", n);
#endif
	r = fjpg(par_f[1], par_f[2], n, infiles, atoi(par_f[3]));
#ifdef MUSETIMER
    	gettimeofday(&at2, NULL);
        timersub(&at2, &at1, &atuse);
        printf("%d.%06ds\n", (int)atuse.tv_sec, (int)atuse.tv_usec);
#endif
	return r;

    }
    return 32;
}

