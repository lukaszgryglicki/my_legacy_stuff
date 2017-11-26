#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HERE __FILE__, __LINE__

/*#define USE_JPEG*/

#ifdef USE_JPEG
#include <stdarg.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <unistd.h>
#define ERR_CANNOTREAD 1
#define ERR_BADJPEG    2
#define ERR_GRAYJPEG   3
#define ERR_256CJPEG   4
#define JPG_COLOR      0
#define JPG_GRAY       1
#define JPG_RGB        2
#endif

typedef struct _fileinfo
{
	char* fname;
	int processed;
	unsigned int x, y;
	unsigned int r, g, b;
	unsigned int i;
	unsigned int snap_x, snap_y;
	int n_used;
	unsigned char* pixels;
} fileinfo;

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


char* g_img_file_list;
char* g_dbin_file;
char* g_dbout_file;
char* g_imgin_file;
char* g_imgout_file;
char* g_snapdir;
long g_mode_cr;
long g_mode_lf;
long g_snap_x, g_snap_y;
long g_part_x, g_part_y;
long g_num;
long* g_idx_rand;
int g_jqual;
int skip_jpegin, skip_jpegout, skip_bmpin, skip_bmpout;
unsigned int g_img_x, g_img_y;
unsigned int g_img_sx;
unsigned int g_img_sy;
unsigned char* g_img_buf;
unsigned int** g_img_idx;
fileinfo* g_file_info;
int g_rs, g_gs, g_bs, g_rg, g_gg, g_bg;
double (*rgb_distance)(fileinfo*, unsigned char, unsigned char, unsigned char);

#define CR (char)13
#define LF (char)10

void panic(char* file, int line, char* why)
{
	fprintf(stdout, "PANIC: File %s, Line %d, Description \"%s\"\n", file, line, why);
	exit(1);
}

void init_bmp(BMPTag* b)
{
 int i;
 if (!b) panic(HERE, "init_bmp: BMPTag is null");
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

/*
 ret -1: error
 ret 0: no such option
 ret 1: empty option
 ret 2: non-empty option
 */
int get_option(int lb, char** par, char* opt, char** optval)
{
	char* fopt;
	int i;
	if (lb <= 0 || !par || !opt || !optval) return -1;
	*optval = NULL;
	fopt = (char*)malloc((strlen(opt)*sizeof(char)) + 3);
	if (!fopt) panic(HERE, "malloc fopt");
	strcpy(fopt, "--");
	strcat(fopt, opt);
	for (i=0;i<lb;i++)
	{
		if (!strcmp(par[i], fopt))
		{
			if (i != lb - 1)
			{
				if (strlen(par[i+1]) <= 0) 
				{
/*					*optval = (char*)malloc(sizeof(char));	 empty option*/
/*					if (!(*optval)) panic(HERE, "malloc optval");*/
/*					strcpy(*optval, "");*/
					*optval = NULL;
					free((void*)fopt);
					return 1;
				}
				else if (strlen(par[i+1]) == 1) 
				{
					*optval = (char*)malloc(sizeof(char) + 1);
					if (!(*optval)) panic(HERE, "malloc optval");
					strcpy(*optval, par[i+1]);
					free((void*)fopt);
					return 2;
				}
				else
				{
					if (par[i+1][0] == '-' && par[i+1][1] == '-')
					{
/*						*optval = (char*)malloc(sizeof(char));		// empty option*/
/*						if (!(*optval)) panic(HERE, "malloc optval");*/
/*						strcpy(*optval, "");*/
						*optval = NULL;
						free((void*)fopt);
						return 1;
					}
					else
					{
						*optval = (char*)malloc((strlen(par[i+1]) * sizeof(char)) + 1);
						if (!(*optval)) panic(HERE, "malloc optval");
						strcpy(*optval, par[i+1]);
						free((void*)fopt);
						return 2;
					}
				}
			}
			else
			{
/*				*optval = (char*)malloc(sizeof(char));		// empty option*/
/*				if (!(*optval)) panic(HERE, "malloc optval");*/
/*				strcpy(*optval, "");*/
				*optval = NULL;
				free((void*)fopt);
				return 1;
			}
		}
	}
	free((void*)fopt);
	return 0;
}

char* readfile(FILE* f, size_t* size)
{
	char* buf;
	int zn, i;
	fseek(f, 0, SEEK_END);
	*size = ftell(f);
	fseek(f, 0, SEEK_SET);
	buf = (char*)malloc(*size);
	if (!buf) return NULL;
	i = 0;
	while ((zn = fgetc(f)) != EOF) buf[i++] = zn;
	return buf;
}

void read_filelist(char*** fnames, int* num)
{
	FILE* f;
	char* buf;
	char* temp;
	size_t fsize;
	unsigned int i;
	int n, l, maxl, k;
	f = fopen(g_img_file_list, "rb");
	if (!f) panic(HERE, "open g_img_file_list");
	buf = readfile(f, &fsize);
	fclose(f);
	n = 0;
	l = 0;
	maxl = -1;
	for(i=0;i<fsize;i++)
	 {
		 l ++;
		 if (l > maxl) maxl = l;
		 if (g_mode_cr == 1 && g_mode_lf == 0 && buf[i] == CR) { n ++; l = 0; }
		 if (g_mode_cr == 0 && g_mode_lf == 1 && buf[i] == LF) { n ++; l = 0; }
		 if (g_mode_cr == 1 && g_mode_lf == 1)
		 {
			 if (i != fsize - 1)
			 {
				 if (buf[i] == CR && buf[i+1] == LF) { n ++; l = 0; }
			 }
		 }
	 }
   printf("%d files listed.\n", n);
   *fnames = (char**)malloc(sizeof(char*) * n);
   if (!(*fnames)) panic(HERE, "malloc *fnames");
   temp = (char*)malloc((sizeof(char) * maxl) + 1);
   if (!temp) panic(HERE, "malloc temp");
   *num = n;
   n = 0;
   k = 0;
   for(i=0;i<fsize;i++)
	 {
		 if (buf[i] != CR && buf[i] != LF) 
		 {
			 temp[k] = buf[i];
			 k ++;
		 }
		 if (g_mode_cr == 1 && g_mode_lf == 0 && buf[i] == CR) 
		 { 
			 (*fnames)[n] = (char*)malloc((sizeof(char) * k) + 1);
			 strncpy((*fnames)[n], temp, k);
			 (*fnames)[n][k] = 0;
			 n ++; 
			 k = 0;
		 }
		 if (g_mode_cr == 0 && g_mode_lf == 1 && buf[i] == LF) 
		 { 
			 (*fnames)[n] = (char*)malloc((sizeof(char) * k) + 1);
			 strncpy((*fnames)[n], temp, k);
			 (*fnames)[n][k] = 0;
			 n ++; 
			 k = 0; 
		 }
		 if (g_mode_cr == 1 && g_mode_lf == 1)
		 {
			 if (i != fsize - 1)
			 {
				 if (buf[i] == CR && buf[i+1] == LF) 
				 { 
					(*fnames)[n] = (char*)malloc((sizeof(char) * k) + 1);
					strncpy((*fnames)[n], temp, k);
					(*fnames)[n][k] = 0;
					 n ++; 
					 k = 0; 
				 }
			 }
		 }
	 }
 free((void*)temp);
 printf("Input file list generated.\n");
 for (l=0;l<(*num);l++)
	 {
	  printf("%d) %s\n", i, (*fnames)[l]);
	 }
}

void init_fileinfo(fileinfo* fi)
{
	if (!fi) return;
	fi->fname = NULL;
	fi->processed = 0;
	fi->x = 0;
	fi->y = 0;
	fi->r = 0;
	fi->g = 0;
	fi->b = 0;
	fi->i = 0;
	fi->snap_x = g_snap_x;
	fi->snap_y = g_snap_y;
	fi->n_used = 0;
	fi->pixels = NULL;
}

void set_color_uchar(unsigned char* s, int Y, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
 s[3*(Y * x + y)] = 0;
 s[3*(Y * x + y) + 1] = 0;
 s[3*(Y * x + y) + 2] = 0;
 s[3*(Y * x + y) + g_rs] = r;
 s[3*(Y * x + y) + g_gs] = g;
 s[3*(Y * x + y) + g_bs] = b;
}

void get_color_uchar(unsigned char* s, int Y, int x, int y, unsigned char* r, unsigned char* g, unsigned char* b)
{
 *r = s[3*(Y * x + y) + g_rg];
 *g = s[3*(Y * x + y) + g_gg];
 *b = s[3*(Y * x + y) + g_bg];
}

int write_snapshot(fileinfo* fi, unsigned char* pixels);
int write_snapshot_pix(char*, unsigned int x, unsigned int y, unsigned char* pixels);

int make_snapshot(fileinfo* fi, unsigned char* pixels)
{
	unsigned char* spix;
	int nx, ny, i, j;
	unsigned int k, l;
	unsigned int fy1, fy2, fx1, fx2;
	double di, dr, dg, db;
	unsigned char r, g, b;
	printf("Making snapshot (%d x %d) to: %s/%d.bmp\n", fi->snap_x, fi->snap_y, g_snapdir, fi->i);

	nx = fi->snap_x;
	ny = fi->snap_y;
        spix = (unsigned char*)malloc(3*ny*nx*sizeof(unsigned char));
	
	for (i=0;i<ny;i++) 
	{
		fy1 = (int)(((double)i * (double)fi->y) / (double)ny);
		fy2 = (int)(((double)(i+1) * (double)fi->y) / (double)ny);
		if (fy2 == fy1) fy2 ++;
		if (fy1 < 0) fy1 = 0;
		if (fy2 < 0) fy2 = 0;
		if (fy1 > fi->y) fy1 = fi->y;
		if (fy2 > fi->y) fy2 = fi->y;
		for (j=0;j<nx;j++)
		{
			fx1 = (int)(((double)j * (double)fi->x) / (double)nx);
			fx2 = (int)(((double)(j+1) * (double)fi->x) / (double)nx);
			if (fx2 == fx1) fx2 ++;
			if (fx1 < 0) fx1 = 0;
			if (fx2 < 0) fx2 = 0;
			if (fx1 > fi->x) fx1 = fi->x;
			if (fx2 > fi->x) fx2 = fi->x;

			di = dr = dg = db = 0.0;
			for (k=fy1;k<fy2;k++) for (l=fx1;l<fx2;l++)
			{
				get_color_uchar(pixels, fi->y, l, k, &r, &g, &b);
				dr += (double)r;
				dg += (double)g;
				db += (double)b;
				di += 1.;
			}
			if (di > 0.)
			{
				dr /= di;
				dg /= di;
				db /= di;
			}
			r = (unsigned int)dr;
			g = (unsigned int)dg;
			b = (unsigned int)db;
			set_color_uchar(spix, ny, j, i, r, g, b);	
		}
	}
 k = write_snapshot(fi, spix);
 free((void*)spix);
 return k;
}

int load_bmp_file(fileinfo* fi, BMPTag bm_handle, FILE* plik)
{
 unsigned char* pixels;
 int i,j;
 unsigned char r,g,b;
 double dr, dg, db, di;

 fread(&bm_handle.fsize,4,1,plik);
 fread(&bm_handle.dummy,4,1,plik);
 fread(&bm_handle.offset,4,1,plik);
 fread(&bm_handle.dummy2,4,1,plik);
 fread(&bm_handle.bm_x,4,1,plik);
 fread(&bm_handle.bm_y,4,1,plik);
 fread(&bm_handle.planes,2,1,plik);
 fread(&bm_handle.bpp,2,1,plik);
 pixels = NULL;
 if (bm_handle.bpp != 24) 
 {
	 printf("Only 24BPP BMPs suported.\n");
	 return 0;
 }
 fseek(plik,bm_handle.offset,SEEK_SET);
 pixels = (unsigned char*)malloc(3*bm_handle.bm_y*bm_handle.bm_x*sizeof(unsigned char));
 fi->x = bm_handle.bm_x;
 fi->y = bm_handle.bm_y;
 di = dr = dg = db = 0.0;
 for (i=0;i<bm_handle.bm_y;i++)  for (j=0;j<bm_handle.bm_x;j++)
    {
     fscanf(plik,"%c%c%c", &b,&g,&r);
	 di += 1.;
	 dr += (double)r;
	 dg += (double)g;
	 db += (double)b;
	 set_color_uchar(pixels, bm_handle.bm_y, j, i, r, g, b);
    }
 if (di > 0.)
 {
	 dr /= di;
	 dg /= di;
	 db /= di;
	 fi->r = (unsigned int)dr;
	 fi->g = (unsigned int)dg;
	 fi->b = (unsigned int)db;
	 printf("(%3.3f,%3.3f,%3.3f)  [%02x %02x %02x] %.0f pixels\n", dr, dg, db, fi->r, fi->g, fi->b, di);
	 j = make_snapshot(fi, pixels);
	 free((void*)pixels);
	 if (j > 0) fi->processed = 1;
	 return j;
 }
 free((void*)pixels);
 return 0;
}

#ifdef USE_JPEG

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
    (*bits)[cur_row][j] = (b << 0x10) + (g << 0x8) + r;
    }
}

#ifdef USE_JPEG

void jpeg_compute_pixels(unsigned char* pix, unsigned char* bits, int sx, int sy)
{
 int i,j,k;
 
 for (i=0;i<sy;i++)
 for (j=0;j<sx;j++)
 for (k=0;k<3;k++) pix[3*(sx*((sy-i)-1)+j)+k] = bits[3*(sy*j+i)+k];
 
}

void init_pixels(unsigned char** pix, unsigned char* bits, int x, int y)
{
 *pix = (unsigned char*)malloc((x*y*3)*sizeof(unsigned char));
  jpeg_compute_pixels(*pix, bits, x, y);
}

void free_pixels(unsigned char** pix)
{
 free(*pix);
 *pix = NULL;
}


int save_jpeg_file_internal(unsigned char* bits, int x, int y, FILE* outfile)
{
 struct jpeg_compress_struct cinfo;
 struct my_error_mgr jerr;
 unsigned char* pixels;
 JSAMPROW row_pointer[1];
 int row_stride;
 pixels = NULL;
/* args_fprintf(stdout, "x:y=%d:%d\n", x,y);*/
 init_pixels(&pixels, bits, x, y);
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
/* jpeg_set_default_colorspace(&cinfo);*/
 jpeg_set_defaults(&cinfo);
 jpeg_set_quality(&cinfo, g_jqual, FALSE);
 jpeg_start_compress(&cinfo, TRUE);
/* jpeg_write_m_header(&cinfo, TRUE);*/
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

#endif


int load_jpeg_file_internal(unsigned long*** bits, int* x, int* y, FILE* infile)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    JSAMPARRAY buffer;
    int row_stride;
    int i;
    *x = *y = 0;
    *bits = NULL;
    if (infile == NULL) return ERR_CANNOTREAD;
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    errptr = &jerr;
    if (setjmp(jerr.setjmp_buffer))
       {
	jpeg_destroy_decompress(&cinfo);
/*	fclose(infile);*/
	return ERR_BADJPEG;
       }
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
       {
	jpeg_destroy_decompress(&cinfo);
/*	fclose(infile);*/
	return ERR_GRAYJPEG;
       }
    else cinfo.quantize_colors = FALSE;
    jpeg_start_decompress(&cinfo);
    if (cinfo.output_components == 1)
       {
	jpeg_destroy_decompress(&cinfo);
/*	fclose(infile);*/
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
    return 0;
}

int translate_jpeg_to_uchar_format(unsigned long** bits, fileinfo* fi, unsigned char* pixels)
{
 double di, dr, dg, db;
 unsigned char r, g, b;
 unsigned int i,j;
 if (!bits) 
 {
     printf("translate_jpeg_to_uchar: no bits\n");
     return 0;
 }
 di = dr = dg = db = 0.0;
 for (i=0;i<fi->x;i++)
 for (j=0;j<fi->y;j++)
     {
      b = pixels[3*(fi->y*i+j)  ] = bits[j][i] & 0xFF;
      g = pixels[3*(fi->y*i+j)+1] = (bits[j][i] & 0xFF00) >> 0x8;
      r = pixels[3*(fi->y*i+j)+2] = (bits[j][i] & 0xFF0000) >> 0x10;

	  di += 1.;
	  dr += (double)r;
	  dg += (double)g;
	  db += (double)b;
     }
 if (di > 0.)
 {
	 dr /= di;
	 dg /= di;
	 db /= di;
	 fi->r = (unsigned int)dr;
	 fi->g = (unsigned int)dg;
	 fi->b = (unsigned int)db;
	 j = make_snapshot(fi, pixels);
	 free((void*)pixels);
	 fi->processed = 1;
	 printf("(%3.3f,%3.3f,%3.3f)  [%02x %02x %02x] %.0f pixels\n", dr, dg, db, fi->r, fi->g, fi->b, di);
	 return j;
 }
 free((void*)pixels);
 return 1;
}


int load_jpeg_file(fileinfo* fi, FILE* plik)
{
 unsigned long** tex_data;
 unsigned char* pixels;
 int err,i, rcode;
 int tex_x, tex_y;
 fseek(plik, 0, SEEK_SET);
 tex_data = NULL;
 pixels = NULL;
 err = load_jpeg_file_internal(&tex_data, &tex_x, &tex_y, plik);
 fclose(plik);
 if (err) 
 {
 	for (i=0;i<tex_y;i++)
	   {
	    if (tex_data[i]) free((void*)(tex_data[i]));
	    (tex_data[i]) = 0;
	   }
	 if (tex_data) free((void*)tex_data);
	 printf("load_jpeg_texture: jpeg decompress error\n");
	 return 0;
 }
 fi->x = tex_x;
 fi->y = tex_y;
 pixels = (unsigned char*)malloc(3*tex_x*tex_y*sizeof(unsigned char));
 rcode = translate_jpeg_to_uchar_format(tex_data, fi, pixels);
 for (i=0;i<tex_y;i++)
   {
    if (tex_data[i]) free((void*)(tex_data[i]));
    (tex_data[i]) = 0;
   }
 if (tex_data) free((void*)tex_data);
 if (!rcode)
 {
/*	 printf("Error translating JPEG to uchar format.\n");*/
	 return 0;
 }
 return 1;
}

#else
int load_jpeg_file(fileinfo* fi, FILE* plik)
{
 printf("JPEG usupported on MSVC\n");
 return 0;
}

#endif

int write_snapshot(fileinfo* fi, unsigned char* pixels)
{
 BMPTag bm_handle;
 FILE* plik;
 char out_f[1024];
#ifdef USE_JPEG
 char jpeg_out[1024];
#endif
 char tmp[1024];
 unsigned int i,j;
 unsigned char r,g,b;
 sprintf(out_f, "%s/%d.bmp", g_snapdir, fi->i);

 if (!skip_bmpout)
 {
 init_bmp(&bm_handle);
 plik = fopen(out_f, "wb");
 if (!plik)
   {
	   printf("Error writing snapshot: %s\n", out_f);
	   return 0;
   }
 fprintf(plik,"%c%c", 'B','M');

 bm_handle.bm_y = fi->snap_y;
 bm_handle.bm_x = fi->snap_x;
 bm_handle.fsize = sizeof(BMPTag)+(bm_handle.bm_y*bm_handle.bm_x*3);
 fwrite(&bm_handle.fsize,4,1,plik);
 fwrite(&bm_handle.dummy,4,1,plik);
 bm_handle.offset=sizeof(BMPTag);
 bm_handle.planes=1;
 bm_handle.bpp=24;
 bm_handle.nbytes = fi->snap_x * fi->snap_y * 3;
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
 for (i=0;i<fi->snap_y;i++)  for (j=0;j<fi->snap_x;j++)
   {
	   get_color_uchar(pixels, fi->snap_y, j, i, &r, &g, &b);
	   /*
	   if (r != g || g != b || b != r)
	   {
		   printf("HA!\n");
	   }
	   */
	   fprintf(plik,"%c%c%c", b, g, r);
   }
 fclose(plik);
 printf("Snapshot bitmap: %s written.\n", out_f);
 }
 strcpy(tmp, out_f);

#ifdef USE_JPEG
 if (!skip_jpegout)
 {
 strcpy(jpeg_out, out_f);
 if (strstr(out_f, "."))
    {
     i = 0;
     while (jpeg_out[i] != '.') i++;
     jpeg_out[i] = 0;       
     strcat(jpeg_out, ".jpeg");
    }
 else
    {
     strcat(jpeg_out, ".jpeg");
    }
 plik = fopen(jpeg_out, "wb");
 if (!plik) 
   { 
    printf("Cannot write to: %s\n", jpeg_out); 
	return 0;
   }   
 save_jpeg_file_internal(pixels, fi->snap_x, fi->snap_y, plik);
 fclose(plik);
 printf("JPEG: %s written.\n", jpeg_out);
 }
#endif
 return 1;
}

int write_snapshot_pix(char* fn, unsigned int x, unsigned int y, unsigned char* pixels)
{
 BMPTag bm_handle;
 FILE* plik;
 char out_f[1024];
#ifdef USE_JPEG
 char jpeg_out[1024];
#endif
 unsigned int i,j;
 unsigned char r,g,b;

 strcpy(out_f, fn);

 if (!skip_bmpout)
 {
 init_bmp(&bm_handle);
 plik = fopen(out_f, "wb");
 if (!plik)
   {
	   printf("Error writing snapshot: %s\n", out_f);
	   return 0;
   }
 fprintf(plik,"%c%c", 'B','M');

 bm_handle.bm_y = y;
 bm_handle.bm_x = x;
 bm_handle.fsize = sizeof(BMPTag)+(bm_handle.bm_y*bm_handle.bm_x*3);
 fwrite(&bm_handle.fsize,4,1,plik);
 fwrite(&bm_handle.dummy,4,1,plik);
 bm_handle.offset=sizeof(BMPTag);
 bm_handle.planes=1;
 bm_handle.bpp=24;
 bm_handle.nbytes = x * y * 3;
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
	   get_color_uchar(pixels, y, j, i, &r, &g, &b);
	   fprintf(plik,"%c%c%c", b, g, r);
   }
 fclose(plik);
 printf("Snapshot bitmap: %s written.\n", out_f);
 }

#ifdef USE_JPEG
 if (!skip_jpegout)
 {
 strcpy(jpeg_out, fn);
 strcat(jpeg_out, ".jpeg");
 plik = fopen(jpeg_out, "wb");
 if (!plik) 
   { 
    printf("Cannot write to: %s\n", jpeg_out); 
	return 0;
   }   
 save_jpeg_file_internal(pixels, x, y, plik);
 fclose(plik);
 printf("JPEG: %s written.\n", jpeg_out);
 }
#endif
 return 1;
}

int process_file(fileinfo* fi, int num)
{
 BMPTag bm_handle;
 FILE* plik;
 int i, rcode;
 char b, m;
 plik = NULL;
 init_bmp(&bm_handle);
 printf("Processing file (%d/%d): \"%s\"\n", fi->i, num, fi->fname);

 plik = fopen(fi->fname, "rb");
 if (!plik)
   {
	   printf("Cannot open file: %s\n", fi->fname);
	   return 0;
   }
 i = fscanf(plik,"%c%c",&b,&m);
 if (i != 2) 
 {
	 printf("File %s is truncated, skipping\n", fi->fname);
	 return 0;
 }
 rcode = 0;
 if ((b != 'B' || m != 'M') && !skip_jpegin)  rcode = load_jpeg_file(fi, plik);
 else if (!skip_bmpin) rcode = load_bmp_file(fi, bm_handle, plik);
 fclose(plik);
 return rcode;
}

int save_dbfile(int num)
{
 FILE* out;
 int i, nproc;

 out = fopen(g_dbout_file, "w");
 if (!out)
 {
	 printf("Blad zapisu pliku bazy: %s\n", g_dbout_file);
	 return 0;
 }

 nproc = 0;
 for (i=0;i<num;i++) if (g_file_info[i].processed) nproc ++;
 printf("Processed %d files\n", nproc);
 fprintf(out, "SNAPDIR=%s\n", g_snapdir);
 fprintf(out, "PROCESSED=%d\n", nproc);
 fprintf(out, "SNAP=%ld,%ld\n", g_snap_x, g_snap_y);
 for (i=0;i<num;i++) if (g_file_info[i].processed) 
 {
	 fprintf(out, "SIZE=%d,%d SNAP=%d,%d,%d COLOR=%d,%d,%d\n", 
		 g_file_info[i].x, g_file_info[i].y, g_file_info[i].snap_x, g_file_info[i].snap_y, 
		 g_file_info[i].i, g_file_info[i].b, g_file_info[i].g, g_file_info[i].r);
 }
 fclose(out);
 return 1;
}

void process_filelist(char** fnames, int num)
{
 int i;
 g_file_info = (fileinfo*)malloc(sizeof(fileinfo) * num);
 if (!g_file_info) panic(HERE, "malloc fileinfo");
 for (i=0;i<num;i++) 
	{
	  init_fileinfo(&g_file_info[i]);
	  g_file_info[i].fname = (char*)malloc((sizeof(char) * strlen(fnames[i])) + 1);
	  strcpy(g_file_info[i].fname, fnames[i]);
	  g_file_info[i].i = i;
	  free((void*)fnames[i]);
	  fnames[i] = NULL;
	  if (!process_file(&g_file_info[i], num))
		{
			printf("Failed processing: %s\n", g_file_info[i].fname);
		}
	}
 printf("Zapis konfiguracji bazy w pliku %s\n", g_dbout_file);
 save_dbfile(num);
}

void generate_dbfile()
{
 char** fnames;
 int numfiles;
 fnames = NULL;
 numfiles = 0;
 read_filelist(&fnames, &numfiles);
 process_filelist(fnames, numfiles);
 free((void*)fnames);
}

void read_dbfile(fileinfo** fi)
{
 int i, gsx, gsy;
 FILE* f;
 char temp[256];
 f = fopen(g_dbin_file, "r");
 if (!f)
 {
     panic(HERE, "open dbin_file");
 }
 if (fscanf(f, "SNAPDIR=%s\n", temp) != 1) panic(HERE, "read snapdir from DB");
 if (strcmp(g_snapdir, temp))
 {
/*     printf ("\"%s\" != \"%s\"\n", temp, g_snapdir);*/
     printf("WARNING: current snapdir(%s) is not DB snapdir(%s)\n", g_snapdir, temp);
/*     printf("Updating snapdir %d %d\n", strlen(temp), strlen(g_snapdir));*/
     printf("Updating snapdir\n");
     free((void*)g_snapdir);
     g_snapdir = (char*)malloc((sizeof(char) * strlen(temp)) + 1);
     strcpy(g_snapdir, temp);
 }
 if (fscanf(f, "PROCESSED=%ld\n", &g_num) != 1) panic(HERE, "read g_num from DB");
 if (g_num <=0 || g_num > 0xFFFFFF) panic(HERE, "g_num value");
 if (fscanf(f, "SNAP=%d,%d\n", &gsx, &gsy) != 2) panic(HERE, "read g_num from DB");
 if (gsx != g_snap_x || gsy != g_snap_y)
 {
	 printf("WARNING: db snap size (%d, %d) and app snapsize (%ld, %ld) mismatch\n", gsx, gsy, g_snap_x, g_snap_y);
 }

 (*fi) = (fileinfo*)malloc(g_num * sizeof(fileinfo));
 if (!(*fi)) panic(HERE, "malloc fi");
 printf("Allocated %ld DB records\n", g_num);
 for (i=0;i<g_num;i++)
 {
  init_fileinfo(&((*fi)[i]));
  if (fscanf(f, "SIZE=%u,%u SNAP=%u,%u,%u COLOR=%u,%u,%u\n", 
	     &((*fi)[i].x), 
	     &((*fi)[i].y), 
	     &((*fi)[i].snap_x), 
	     &((*fi)[i].snap_y), 
	     &((*fi)[i].i), 
	     &((*fi)[i].r), 
	     &((*fi)[i].g), 
	     &((*fi)[i].b)
	     ) != 8) 
 	{
	    printf("Error reading element: %d/%ld\n", i, g_num);
	    panic(HERE, "read g_num from DB");
	}

/*  if ((*fi)[i].snap_x != (unsigned int)g_snap_x) panic(HERE, "snap_x and g_snap_x mismatch");*/
/*  if ((*fi)[i].snap_y != (unsigned int)g_snap_y) panic(HERE, "snap_y and g_snap_y mismatch");*/
  (*fi)[i].fname = (char*)malloc( (strlen(g_snapdir) + 10) * sizeof(char));
  sprintf((*fi)[i].fname, "%s/%d", g_snapdir, (*fi)[i].i);
  (*fi)[i].processed = 0;
/*  printf("Read snapshot: %s\n", (*fi)[i].fname);*/



 }
 fclose(f);
}

int load_bmp_pix(BMPTag bm_handle, FILE* plik, unsigned char** _pixels, unsigned int* _x, unsigned int* _y)
{
 unsigned char* pixels;
 int i,j;
 unsigned char r,g,b;

 fread(&bm_handle.fsize,4,1,plik);
 fread(&bm_handle.dummy,4,1,plik);
 fread(&bm_handle.offset,4,1,plik);
 fread(&bm_handle.dummy2,4,1,plik);
 fread(&bm_handle.bm_x,4,1,plik);
 fread(&bm_handle.bm_y,4,1,plik);
 fread(&bm_handle.planes,2,1,plik);
 fread(&bm_handle.bpp,2,1,plik);
 pixels = NULL;
 if (bm_handle.bpp != 24) 
 {
	 printf("Only 24BPP BMPs suported.\n");
	 return 0;
 }
 fseek(plik,bm_handle.offset,SEEK_SET);
 pixels = (unsigned char*)malloc(3*bm_handle.bm_y*bm_handle.bm_x*sizeof(unsigned char));
 for (i=0;i<bm_handle.bm_y;i++)  for (j=0;j<bm_handle.bm_x;j++)
    {
     fscanf(plik,"%c%c%c", &b,&g,&r);
	 set_color_uchar(pixels, bm_handle.bm_y, j, i, r, g, b);
    }
 *_pixels = pixels;
 *_x = (unsigned int)bm_handle.bm_x;
 *_y = (unsigned int)bm_handle.bm_y;
 return 1;
}

#ifdef USE_JPEG
int translate_jpeg_to_uchar_format_pix(unsigned long** bits, unsigned int x, unsigned int y, unsigned char* pixels)
{
 unsigned int i,j;
 if (!bits) 
 {
     printf("translate_jpeg_to_uchar: no bits\n");
     return 0;
 }
 for (i=0;i<x;i++)
 for (j=0;j<y;j++)
     {
      pixels[3*(y*i+j)  ] = bits[j][i] & 0xFF;
      pixels[3*(y*i+j)+1] = (bits[j][i] & 0xFF00) >> 0x8;
      pixels[3*(y*i+j)+2] = (bits[j][i] & 0xFF0000) >> 0x10;
     }
 return 1;
}
#endif

int load_jpeg_pix(FILE* plik, unsigned char** _pixels, unsigned int* _x, unsigned int* _y)
{
#ifdef USE_JPEG
 unsigned long** tex_data;
 unsigned char* pixels;
 int err,i, rcode;
 int tex_x, tex_y;
 fseek(plik, 0, SEEK_SET);
 tex_data = NULL;
 pixels = NULL;
 err = load_jpeg_file_internal(&tex_data, &tex_x, &tex_y, plik);
 fclose(plik);
 if (err) 
 {
 	for (i=0;i<tex_y;i++)
	   {
	    if (tex_data[i]) free((void*)(tex_data[i]));
	    (tex_data[i]) = 0;
	   }
	 if (tex_data) free((void*)tex_data);
	 printf("load_jpeg_texture: jpeg decompress error\n");
	 return 0;
 }
 pixels = (unsigned char*)malloc(3*tex_x*tex_y*sizeof(unsigned char));
 rcode = translate_jpeg_to_uchar_format_pix(tex_data, tex_x, tex_y, pixels);
 for (i=0;i<tex_y;i++)
   {
    if (tex_data[i]) free((void*)(tex_data[i]));
    (tex_data[i]) = 0;
   }
 if (tex_data) free((void*)tex_data);
 if (!rcode)  return 0;

 *_pixels = pixels;
 *_x = (unsigned int)tex_x;
 *_y = (unsigned int)tex_y;
 return 1;
#else
 printf("JPEG unsuppiorted under MSVC.\n");
 return 0;
#endif
}

void pow_check(int* d)
{
 double dx, e;
 dx = (double)(*d);
 e = 0.;
 while (pow(2., e) < dx) e += 1.;
 if (dx != pow(2., e)) dx = pow(2., e);
 *d = (int)dx;
}

int make_scale(unsigned char* pixels)
{
	unsigned char* spix;
	int nx, ny, i, j;
	unsigned int k, l;
	unsigned int fy1, fy2, fx1, fx2;
	double di, dr, dg, db;
	unsigned char r, g, b;
	if (g_part_x < 1 || g_part_y < 1 || g_img_x < 1 || g_img_y < 1)
	{
	    printf("Error, bad values (%u %u %ld %ld)\n", g_img_x, g_img_y, g_part_x, g_part_y);
	    return 0;
	}
	printf("Scaling image (%u,%u) using parts (%ld,%ld)\n", g_img_x, g_img_y, g_part_x, g_part_y);

	nx = g_img_x / g_part_x;
	ny = g_img_y / g_part_y;
/*	pow_check(&nx);*/
/*	pow_check(&ny);*/
    spix = (unsigned char*)malloc(3*ny*nx*sizeof(unsigned char));
	
	for (i=0;i<ny;i++) 
	{
		fy1 = (int)(((double)i * (double)g_img_y) / (double)ny);
		fy2 = (int)(((double)(i+1) * (double)g_img_y) / (double)ny);
		if (fy2 == fy1) fy2 ++;
		if (fy1 < 0) fy1 = 0;
		if (fy2 < 0) fy2 = 0;
		if (fy1 > g_img_y) fy1 = g_img_y;
		if (fy2 > g_img_y) fy2 = g_img_y;
		for (j=0;j<nx;j++)
		{
			fx1 = (int)(((double)j * (double)g_img_x) / (double)nx);
			fx2 = (int)(((double)(j+1) * (double)g_img_x) / (double)nx);
			if (fx2 == fx1) fx2 ++;
			if (fx1 < 0) fx1 = 0;
			if (fx2 < 0) fx2 = 0;
			if (fx1 > g_img_x) fx1 = g_img_x;
			if (fx2 > g_img_x) fx2 = g_img_x;

			di = dr = dg = db = 0.0;
			for (k=fy1;k<fy2;k++) for (l=fx1;l<fx2;l++)
			{
				get_color_uchar(pixels, g_img_y, l, k, &r, &g, &b);
				dr += (double)r;
				dg += (double)g;
				db += (double)b;
				di += 1.;
			}
			if (di > 0.)
			{
				dr /= di;
				dg /= di;
				db /= di;
			}
			r = (unsigned int)dr;
			g = (unsigned int)dg;
			b = (unsigned int)db;
			set_color_uchar(spix, ny, j, i, r, g, b);	
		}
	}
 g_img_sx = nx;
 g_img_sy = ny;
 g_img_buf = spix;
 printf("Final scaled size: (%u, %u)\n", g_img_sx, g_img_sy);
 printf("Mozaik output will be: (%ld, %ld)\n", g_img_sx * g_snap_x, g_img_sy * g_snap_y);
 k = write_snapshot_pix("snapshot.bmp", g_img_sx, g_img_sy, spix);
 return k;
}

int read_imgfile()
{
 BMPTag bm_handle;
 unsigned char* pixels;
 FILE* plik;
 int i, rcode;
 unsigned char b, m;
 init_bmp(&bm_handle);
 g_img_x = g_img_y = 0;
 g_img_sx = 0;
 g_img_sy = 0;
 g_img_buf = NULL;
 g_img_idx = NULL;
 pixels = NULL;
 plik = fopen(g_imgin_file, "rb");
 if (!plik)
   {
	   printf("Cannot open file: %s\n", g_imgin_file);
	   return 0;
   }
 i = fscanf(plik,"%c%c",&b,&m);
 if (i != 2) 
   {
	 printf("File %s is truncated, skipping\n", g_imgin_file);
	 return 0;
   }
 rcode = 0;
 if ((b != 'B' || m != 'M') && !skip_jpegin) rcode = load_jpeg_pix(plik, &pixels, &g_img_x, &g_img_y);
 else if (!skip_bmpin) rcode = load_bmp_pix(bm_handle, plik, &pixels, &g_img_x, &g_img_y);
 if (!rcode) printf("Error loading img file.\n");
 rcode = make_scale(pixels);
 if (!rcode) printf("Error rescaling img.\n");
 fclose(plik);
 free((void*)pixels);
 return rcode;
}

double rgb_distance_nearest_rgb(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->r - r) + fabs(fi->g - g) + fabs(fi->b - b);
}

double rgb_distance_nearest_pr(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return 10*fabs(fi->r - r) + fabs(fi->g - g) + fabs(fi->b - b);
}

double rgb_distance_nearest_pg(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->r - r) + 10*fabs(fi->g - g) + fabs(fi->b - b);
}

double rgb_distance_nearest_pb(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->r - r) + fabs(fi->g - g) + 10*fabs(fi->b - b);
}

double rgb_distance_nearest_prpg(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return 10*fabs(fi->r - r) + 10*fabs(fi->g - g) + fabs(fi->b - b);
}

double rgb_distance_nearest_pgpb(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->r - r) + 10*fabs(fi->g - g) + 10*fabs(fi->b - b);
}

double rgb_distance_nearest_prpb(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return 10*fabs(fi->r - r) + fabs(fi->g - g) + 10*fabs(fi->b - b);
}

double rgb_distance_nearest_prpgpb(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return ((rand() % 10) + 1) * fabs(fi->r - r) + ((rand() % 10) + 1) * fabs(fi->g - g) + ((rand() % 10) + 1) * fabs(fi->b - b);
}

double rgb_distance_nearest_ir(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return -fabs((double)fi->r - (double)r);
}

double rgb_distance_nearest_ig(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return -fabs((double)fi->b - (double)b);
}

double rgb_distance_nearest_ib(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return -fabs((double)fi->b - (double)b);
}

double rgb_distance_nearest_irig(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return -fabs((double)fi->r - (double)r) -fabs((double)fi->g - (double)g);
}

double rgb_distance_nearest_igib(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return -fabs((double)fi->g - (double)g) -fabs((double)fi->b - (double)b);
}

double rgb_distance_nearest_irib(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return -fabs((double)fi->r - (double)r) -fabs((double)fi->b - (double)b);
}

double rgb_distance_nearest_irigib(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return -fabs((double)fi->r - (double)r) -fabs((double)fi->g - (double)g) -fabs((double)fi->b - (double)b);
}

double rgb_distance_nearest_r(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->r - r);
}

double rgb_distance_nearest_g(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->g - g);
}

double rgb_distance_nearest_b(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->b - b);
}

double rgb_distance_nearest_rg(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->r - r) + fabs(fi->g - g);
}

double rgb_distance_nearest_gb(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->g - g) + fabs(fi->b - b);
}

double rgb_distance_nearest_rb(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return fabs(fi->r - r) + fabs(fi->b - b);
}

double rgb_distance_nearest_0(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
 return 0.0;
}

int get_nearest_color(fileinfo* fi, unsigned char r, unsigned char g, unsigned char b)
{
	int i;
	double mindist, dist;
	int minidx, num_vals, ri;

	num_vals = 1;
	g_idx_rand[ 0 ] = 0;
	mindist = 1e9;
	minidx = 0;
/*	printf("get nearest color %x\n", g_idx_rand);*/

	for (i=0;i<g_num;i++)
	{
		dist = rgb_distance(&fi[i], r, g, b);
		if (dist == mindist)
		{
/*			printf("EQ: %lf\n", dist);*/
			g_idx_rand[ num_vals ] = i;
			num_vals ++;
		}
		else if (dist < mindist)
		{
/*			printf("LO: %lf\n", dist);*/
			mindist = dist;
			minidx = i;
			g_idx_rand[0] = minidx;
			num_vals = 1;
			/*printf("*** New distance is %f, idx is %d ***\n", mindist, minidx);*/
		}
	}
/* printf("%d/%d matches\n", num_vals, g_num);*/
 /*if (num_vals > 10) printf("rand = %d\n", num_vals);*/
 ri = rand() % num_vals;
/* printf("Distance is %f, idx is %d\n", mindist, ri);*/
 return g_idx_rand[ri];
}

int generate_indices(fileinfo * fi)
{
	unsigned int i, j;
	unsigned char r, g, b;
	int idx;
	g_img_idx = (unsigned int**)malloc(sizeof(unsigned int*) * g_img_sx);
	if (!g_img_idx) panic(HERE, "malloc g_img_idx");
	for (i=0;i<g_img_sx;i++)
	{
		g_img_idx[i] = (unsigned int*)malloc(sizeof(unsigned int) * g_img_sy);
		if (!g_img_idx[i]) panic(HERE, "malloc g_img_idx_item");
	}

	srand(0);
	g_idx_rand = (long*)malloc(g_num * sizeof(long));
	if (!g_idx_rand) panic(HERE, "malloc g_idx_rand");

	printf("Generate idx table...\n");
 	for (i=0;i<g_img_sx;i++) 
	{
		if (!(i % 10)) printf("%d/%d\n", i, g_img_sx);
		for (j=0;j<g_img_sy;j++)
		{
			get_color_uchar(g_img_buf, g_img_sy, i, j, &r, &g, &b);
			idx = get_nearest_color(fi, r, g, b);
			g_img_idx[i][j] = idx;
			fi[idx].n_used ++ ;
		}
	}
	printf("Done generating indices.\n");
 return 1;
}

int make_scale_pix(unsigned char* pixels, unsigned int curr_x, int unsigned curr_y, unsigned char** spix)
{
	int nx, ny, i, j;
	unsigned int k, l;
	unsigned int fy1, fy2, fx1, fx2;
	double di, dr, dg, db;
	unsigned char r, g, b;

	/*printf("Scaling image (%u,%u) using snapsize (%ld,%ld)\n", curr_x, curr_y, g_snap_x, g_snap_y);*/

	nx = g_snap_x;
	ny = g_snap_y;

    *spix = (unsigned char*)malloc(3*ny*nx*sizeof(unsigned char));
	if (!(*spix)) panic(HERE, "malloc *spix");
	
	for (i=0;i<ny;i++) 
	{
		fy1 = (int)(((double)i * (double)curr_y) / (double)ny);
		fy2 = (int)(((double)(i+1) * (double)curr_y) / (double)ny);
		if (fy2 == fy1) fy2 ++;
		if (fy1 < 0) fy1 = 0;
		if (fy2 < 0) fy2 = 0;
		if (fy1 > curr_y) fy1 = curr_y;
		if (fy2 > curr_y) fy2 = curr_y;
		for (j=0;j<nx;j++)
		{
			fx1 = (int)(((double)j * (double)curr_x) / (double)nx);
			fx2 = (int)(((double)(j+1) * (double)curr_x) / (double)nx);
			if (fx2 == fx1) fx2 ++;
			if (fx1 < 0) fx1 = 0;
			if (fx2 < 0) fx2 = 0;
			if (fx1 > curr_x) fx1 = curr_x;
			if (fx2 > curr_x) fx2 = curr_x;

			di = dr = dg = db = 0.0;
			for (k=fy1;k<fy2;k++) for (l=fx1;l<fx2;l++)
			{
				get_color_uchar(pixels, curr_y, l, k, &r, &g, &b);
				dr += (double)r;
				dg += (double)g;
				db += (double)b;
				di += 1.;
			}
			if (di > 0.)
			{
				dr /= di;
				dg /= di;
				db /= di;
			}
			r = (unsigned int)dr;
			g = (unsigned int)dg;
			b = (unsigned int)db;
			set_color_uchar(*spix, ny, j, i, r, g, b);	
		}
	}
 return 1;
}

int prefetch_pixmap(fileinfo* fi)
{
 BMPTag bm_handle;
 FILE* plik;
 char fname[1024];
 int i, rcode;
 unsigned char b, m;
 unsigned int test_x, test_y;
 unsigned char* newpixels;
 newpixels = NULL;
 init_bmp(&bm_handle);
 sprintf(fname, "%s/%d.bmp", g_snapdir, fi->i);
 if (skip_bmpin) plik = NULL;
 else plik = fopen(fname, "rb");
 if (!plik)
   {
	   sprintf(fname, "%s/%d.jpeg", g_snapdir, fi->i);
	   if (skip_jpegin) plik = NULL;
	   else plik = fopen(fname, "rb");
	   if (!plik)
	   {
		   printf("Error opening BMP then JPG: %s\n", fname);
		   return 0;
	   }
	   rcode = load_jpeg_pix(plik, &fi->pixels, &test_x, &test_y);
/*	   printf("rcode = %d\n", rcode);*/
	   if (test_x != (unsigned int)g_snap_x || test_y != (unsigned int)g_snap_y)
	   {
/*		   printf("Size mismatch: (%u, %d) != (%ld, %ld)\n", test_x, test_y, g_snap_x, g_snap_y);*/
		   make_scale_pix(fi->pixels, test_x, test_y, &newpixels);
		   free((void*)fi->pixels);
		   fi->pixels = newpixels;
		   return 1;
	   }
	   fclose(plik);
	   return rcode;
   }
 i = fscanf(plik,"%c%c",&b,&m);
 if (i != 2) 
   {
	 printf("File %s is truncated, skipping\n", g_imgin_file);
	 return 0;
   }
 rcode = 0;
 if ((b != 'B' || m != 'M') && !skip_jpegin) rcode = load_jpeg_pix(plik, &fi->pixels, &test_x, &test_y);
 else if (!skip_bmpin) rcode = load_bmp_pix(bm_handle, plik, &fi->pixels, &test_x, &test_y);
 if (!rcode) printf("Error loading img file.\n");
 fclose(plik);
 if (test_x != (unsigned int)g_snap_x || test_y != (unsigned int)g_snap_y)
	{
/*		printf("Size mismatch: (%u, %d) != (%ld, %ld)\n", test_x, test_y, g_snap_x, g_snap_y);*/
		make_scale_pix(fi->pixels, test_x, test_y, &newpixels);
		free((void*)fi->pixels);
		fi->pixels = newpixels;
		return 1;
	}
 return rcode;
}

int prefetch_pixmaps(fileinfo* fi)
{
 int i, npf;
 npf = 0;
 printf("Prefetching pixmaps...\n");
 for (i=0;i<g_num;i++)
 {
	 if (fi[i].n_used > 0) 
	 {
		 npf ++;
		 if (!(npf % 10)) 
		 {
			 printf(".");
			 fflush(stdout);
		 }
		 if (!prefetch_pixmap(&fi[i]))
		 {
			 printf("Error loading %d snapshot, this is fatal\n", i);
			 return 0;
		 }
		 fi[i].processed = 1;
	 }
 }
 printf("\nPrefetched %d pixmaps\n", npf);
 return 1;
}

int generate_outimg(fileinfo* fi)
{
	unsigned char* spix;
	unsigned int nx, ny, i, j, xidx, yidx;
	int k, l;
	unsigned char r, g, b;


	nx = g_img_sx * g_snap_x;
	ny = g_img_sy * g_snap_y;
	printf("Watchout! final malloc ALL out pixels, needs %.0f Mb memeory!\n", ((double)nx * (double)ny * 3.)/(1024.*1024.));
    spix = (unsigned char*)malloc(3*ny*nx*sizeof(unsigned char));
	
	for (i=0;i<g_img_sx;i++) 
	{
		yidx = i * g_snap_x;
		for (j=0;j<g_img_sy;j++)
		{
			xidx = j * g_snap_y;
			for (k=0;k<g_snap_x;k++) for (l=0;l<g_snap_y;l++)
			{
				if (!fi[g_img_idx[i][j]].processed)
				{
					printf("Pixmap[%d][%d], idx[%d] ==> fi[%d] is not processed - this is fatal\n", 
						i, j, g_img_idx[i][j], g_img_idx[i][j]);
					panic(HERE, "g_img_idx not processed");
				}
				get_color_uchar(fi[g_img_idx[i][j]].pixels, g_snap_y, k, l, &r, &g, &b);
				set_color_uchar(spix, ny, yidx + k, xidx + l, r, g, b);	
			}
		}
	}
 printf("Final write of mozaik scene to %s\n", g_imgout_file);
 k = write_snapshot_pix(g_imgout_file, nx, ny, spix);
 return k;
}

void use_dbfile()
{
 fileinfo* db;
 read_dbfile(&db);
 if (!read_imgfile()) panic(HERE, "read imgfile");
 if (!generate_indices(db)) panic(HERE, "generate_indices");
 if (!prefetch_pixmaps(db)) panic(HERE, "prefetch_pixmaps");
 if (!generate_outimg(db))  panic(HERE, "generate_outimg");

 printf("All succeded.\n");
 free((void*)db);
}

void check_snapshots()
{
 double dx, dy, px, py, e;
 dx = (double)g_snap_x;
 dy = (double)g_snap_y;
 px = (double)g_part_x;
 py = (double)g_part_y;
 e = 0.;
 while (pow(2., e) < dx) e += 1.;
 if (dx != pow(2., e)) dx = pow(2., e);
 e = 0.;
 while (pow(2., e) < dy) e += 1.;
 if (dy != pow(2., e)) dy = pow(2., e);
 e = 0.;
 while (pow(2., e) < py) e += 1.;
 if (py != pow(2., e)) py = pow(2., e);
 e = 0.;
 while (pow(2., e) < px) e += 1.;
 if (px != pow(2., e)) px = pow(2., e);
 g_snap_x = (int)dx;
 g_snap_y = (int)dy;
 g_part_x = (int)px;
 g_part_y = (int)py;
 printf("Snapshots (%ld x %ld)\n", g_snap_x, g_snap_y);
 printf("Parts (%ld x %ld)\n", g_part_x, g_part_y);
}

void help()
{
 printf("options\n");
 printf("--help\n");
 printf("--skip-jpegin\n");
 printf("--skip-jpepout\n");
 printf("--skip-bmpin\n");
 printf("--skip-bmpout\n");
 printf("--func x x = {0,r,g,b,rg,gb,rb,rgb,xr,xg,xb,xrxg,xgxb,xrxb,xrxgxb x={i,p}}\n");
 printf("--pmap Rset Gset Bset Rget Gget Bget (default 0 1 2 0 1 2) values in {0,1,2}\n");
 printf("--jqual quality (1-100)\n");
 printf("--mode-cr --mode-crlf --mode-lf (at least one must be set)\n");
 printf("--snap-dir dir --snap-x x --snap-y y (directory and size of snaphots)\n");
 printf("--part-x x --part-y y (size of parts)\n");
 printf("--generate-db filelist --db-outfile dbfile (must be used both)\n");
 printf("--use-db dbfile --img-infile inputimg --img-outfile outputimg (must be used together)\n");
}

void parse_options(int lb, char** par)
{
 char* optval;
 optval = NULL;
 g_img_file_list = NULL;
 g_dbout_file = NULL;
 g_idx_rand = NULL;
 g_mode_cr = 0;
 g_mode_lf = 0;
 g_num = 0;
 g_snap_x = 64;
 g_snap_y = 64;
 g_jqual = 90;
 skip_jpegin = 0;
 skip_jpegout = 0;
 skip_bmpin = 0;
 skip_bmpout = 0;
 g_part_x = 4;
 g_part_y = 4;
 g_rs = g_rg = 0;
 g_gs = g_gg = 1;
 g_bs = g_bg = 2;
 rgb_distance = rgb_distance_nearest_rgb;

 if (get_option(lb, par, "help", &optval) == 1)  
 { 
	 help();
	 g_mode_cr = 1;
	 g_mode_lf = 1; 
 }

 if (get_option(lb, par, "pmap", &optval) == 2)  
 { 
	 if (sscanf(optval, "%d %d %d %d %d %d", &g_rs, &g_gs, &g_bs, &g_rg, &g_gg, &g_bg) != 6) panic(HERE, "sscanf PMAP");
	 free((void*)optval);
 }

 printf("RGB set map: [%d %d %d]\n", g_rs, g_gs, g_bs);
 printf("RGB get map: [%d %d %d]\n", g_rg, g_gg, g_bg);

 if (get_option(lb, par, "func", &optval) == 2)  
 { 
	if (!strcmp(optval, "0")) rgb_distance = rgb_distance_nearest_0;
	else if (!strcmp(optval, "r")) rgb_distance = rgb_distance_nearest_r;
	else if (!strcmp(optval, "g")) rgb_distance = rgb_distance_nearest_g;
	else if (!strcmp(optval, "b")) rgb_distance = rgb_distance_nearest_b;
	else if (!strcmp(optval, "rg")) rgb_distance = rgb_distance_nearest_rg;
	else if (!strcmp(optval, "gb")) rgb_distance = rgb_distance_nearest_gb;
	else if (!strcmp(optval, "rb")) rgb_distance = rgb_distance_nearest_rb;
	else if (!strcmp(optval, "rgb")) rgb_distance = rgb_distance_nearest_rgb;
	else if (!strcmp(optval, "ir")) rgb_distance = rgb_distance_nearest_ir;
	else if (!strcmp(optval, "ig")) rgb_distance = rgb_distance_nearest_ig;
	else if (!strcmp(optval, "ib")) rgb_distance = rgb_distance_nearest_ib;
	else if (!strcmp(optval, "irig")) rgb_distance = rgb_distance_nearest_irig;
	else if (!strcmp(optval, "igib")) rgb_distance = rgb_distance_nearest_igib;
	else if (!strcmp(optval, "irib")) rgb_distance = rgb_distance_nearest_irib;
	else if (!strcmp(optval, "irigib")) rgb_distance = rgb_distance_nearest_irigib;
	else if (!strcmp(optval, "pr")) rgb_distance = rgb_distance_nearest_pr;
	else if (!strcmp(optval, "pg")) rgb_distance = rgb_distance_nearest_pg;
	else if (!strcmp(optval, "pb")) rgb_distance = rgb_distance_nearest_pb;
	else if (!strcmp(optval, "prpg")) rgb_distance = rgb_distance_nearest_prpg;
	else if (!strcmp(optval, "pgpb")) rgb_distance = rgb_distance_nearest_pgpb;
	else if (!strcmp(optval, "prpb")) rgb_distance = rgb_distance_nearest_prpb;
	else if (!strcmp(optval, "prpgpb")) rgb_distance = rgb_distance_nearest_prpgpb;
	free((void*)optval);
/*	printf("New rgb_distance func: %p\n", (void*)rgb_distance);*/
 }

 if (get_option(lb, par, "skip-jpegin", &optval) == 1)  
 { 
     skip_jpegin = 1;
 }

 if (get_option(lb, par, "skip-jpegout", &optval) == 1)  
 { 
     skip_jpegout = 1;
 }

 if (get_option(lb, par, "skip-bmpin", &optval) == 1)  
 { 
     skip_bmpin = 1;
 }

 if (get_option(lb, par, "skip-bmpout", &optval) == 1)  
 { 
     skip_bmpout = 1;
 }

 if (get_option(lb, par, "mode-crlf", &optval) == 1)  
 { 
	 printf("File format CR/LF set.\n");
	 g_mode_cr = 1;
	 g_mode_lf = 1; 
 }
 if (get_option(lb, par, "mode-lf", &optval) == 1)
 { 
	 printf("File format LF set.\n");
	 g_mode_cr = 0;
	 g_mode_lf = 1; 
 }
 if (get_option(lb, par, "mode-cr", &optval) == 1)
 { 
	 printf("File format CR set.\n");
	 g_mode_cr = 1;
	 g_mode_lf = 0; 
 }

 if (!g_mode_cr && !g_mode_lf) panic(HERE, "CR or LF or CR/LF mode must be set");

 if (get_option(lb, par, "snap-dir", &optval) == 2 && optval) 
 {
	g_snapdir = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
	if (!g_snapdir) panic(HERE, "malloc g_snapdir");
	strcpy(g_snapdir, optval);
	free((void*)optval);
 }
 else
 {
	g_snapdir = (char*)malloc((sizeof(char) * 4) + 1);
	if (!g_snapdir) panic(HERE, "malloc g_snapdir");
	strcpy(g_snapdir, "snap");
 }
 printf("Snapshots dir: %s\n", g_snapdir);

if (get_option(lb, par, "jqual", &optval) == 2 && optval) 
 {
	if (sscanf(optval, "%d", &g_jqual) == 1)
	{
		if (g_jqual < 1)    g_jqual = 1;
		if (g_jqual > 100)  g_jqual = 100;
		printf("New JPEG quality = %d\n", g_jqual);
	}
	else printf("Error scanning jqual from %s\n", optval);
	free((void*)optval);
 }

 if (get_option(lb, par, "snap-x", &optval) == 2 && optval) 
 {
	if (sscanf(optval, "%ld", &g_snap_x) == 1)
	{
		if (g_snap_x < 1)    g_snap_x = 1;
		if (g_snap_x > 2048) g_snap_x = 2048;
		printf("New snap_x = %ld\n", g_snap_x);
	}
	else printf("Error scanning snap_x from %s\n", optval);
	free((void*)optval);
 }

if (get_option(lb, par, "snap-y", &optval) == 2 && optval) 
 {
	if (sscanf(optval, "%ld", &g_snap_y) == 1)
	{
		if (g_snap_y < 1)    g_snap_y = 1;
		if (g_snap_y > 2048) g_snap_y = 2048;
		printf("New snap_y = %ld\n", g_snap_y);
	}
	else printf("Error scanning snap_y from %s\n", optval);
	free((void*)optval);
 }

 if (get_option(lb, par, "part-x", &optval) == 2 && optval) 
 {
	if (sscanf(optval, "%ld", &g_part_x) == 1)
	{
		if (g_part_x < 1)    g_part_x = 1;
		if (g_part_x > 2048) g_part_x = 2048;
		printf("New part_x = %ld\n", g_part_x);
	}
	else printf("Error scanning part_x from %s\n", optval);
	free((void*)optval);
 }

 if (get_option(lb, par, "part-y", &optval) == 2 && optval) 
 {
	if (sscanf(optval, "%ld", &g_part_y) == 1)
	{
		if (g_part_y < 1)    g_part_y = 1;
		if (g_part_y > 2048) g_part_y = 2048;
		printf("New part_y = %ld\n", g_part_y);
	}
	else printf("Error scanning part_y from %s\n", optval);
	free((void*)optval);
 }

/* check_snapshots();*/

 if (get_option(lb, par, "generate-db", &optval) == 2 && optval) 
	{
		g_img_file_list = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
		if (!g_img_file_list) panic(HERE, "malloc g_img_file_list");
		strcpy(g_img_file_list, optval);
		free((void*)optval);
		if (get_option(lb, par, "db-outfile", &optval) == 2 && optval) 
			{
				g_dbout_file = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
				if (!g_dbout_file) panic(HERE, "malloc g_dbout_file");
				strcpy(g_dbout_file, optval);
				free((void*)optval);
				printf("Generate DB mode: infile: %s, outfile: %s\n", g_img_file_list, g_dbout_file);
				generate_dbfile();
			}
	}
 if (get_option(lb, par, "use-db", &optval) == 2 && optval) 
	{
		g_dbin_file = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
		if (!g_dbin_file) panic(HERE, "malloc g_dbin_file");
		strcpy(g_dbin_file, optval);
		free((void*)optval);
		if (get_option(lb, par, "img-outfile", &optval) == 2 && optval) 
			{
				g_imgout_file = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
				if (!g_imgout_file) panic(HERE, "malloc g_imgout_file");
				strcpy(g_imgout_file, optval);
				free((void*)optval);
				if (get_option(lb, par, "img-infile", &optval) == 2 && optval) 
					{
						g_imgin_file = (char*)malloc((sizeof(char) * strlen(optval)) + 1);
						if (!g_imgin_file) panic(HERE, "malloc g_imgin_file");
						strcpy(g_imgin_file, optval);
						free((void*)optval);
						printf("Use DB mode: DBin: %s, img: %s, %s\n", g_dbin_file, g_imgin_file, g_imgout_file);
						use_dbfile();
					}
			}
	}
}

int main(int lb, char** par)
{
	parse_options(lb, par);
	return 0;
}

