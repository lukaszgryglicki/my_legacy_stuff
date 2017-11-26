#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

void bmptag(BMPTag* b)
{
 int i;
 if (!b) return;
 b->ident[0]='B';
 b->ident[1]='M';
 b->fsize=0;
 b->dummy=0;
 b->offset=sizeof(BMPTag);
 b->bm_x=b->bm_y=0x20;
 b->dummy2=40;
 b->planes=0;
 b->bpp=0x18;
 b->planes=1;
 b->compress=0;
 b->nbytes=3*32*32;
 for (i=0;i<4;i++) b->no_matter[i]=0;
}

static BMPTag bm_handle;
static unsigned long** bits;
static int cx,cy;

int jpeg_ReturnRed(unsigned long col)
{
 return (int)((0xff0000 & col)>>0x10);
}


int jpeg_ReturnGreen(unsigned long col)
{
 return (int)((0x00ff00 & col)>>0x08);
}


int jpeg_ReturnBlue(unsigned long col)
{
 return (int)(0x0000ff & col);
}

unsigned long RGB(int r, int g, int b)
{
 return (r<<0x10) + (g<<0x08) + b;
}

void alien_bits(int *ri, int *gi, int *bi, float ang)
{
 float r,g,b;
 ang=1.-ang-0.15167;
 r = ang;
 g = fabs(ang-.33333);
 b = fabs(ang-.66667);
 if (r<=.33333) r=(.33333-r)*3.;
 else if (r>=.66667) r=(r-.66667)*3.;
 else r=0.0;
 if (g>=.33333) g=0.0; else g=(.33333-g)*3.;
 if (b>=.33333) b=0.0; else b=(.33333-b)*3.;
 *ri = (int)(r*255.0);
 *gi = (int)(g*255.0);
 *bi = (int)(b*255.0);
 if (*ri>255) *ri=510-2*(*ri);
}

void alien_trans(unsigned long** jbits)
{
 int ar,ag,ab;
 static float** big_map;
 register int i,j;
 big_map = (float**)malloc(cy*sizeof(float*));
  for (i=0;i<cy;i++) 
   {
    big_map[i] = (float*)malloc(cx*sizeof(float));
    for (j=0;j<cx;j++) big_map[i][j] = 0.0;
   }
 for (i=0;i<cy;i++)
 for (j=0;j<cx;j++)
 {
    big_map[i][j]=
    (
     ((0.299*(float)jpeg_ReturnRed(jbits[i][j]))+
     (0.587*(float)jpeg_ReturnGreen(jbits[i][j]))+
     (0.114*(float)jpeg_ReturnBlue(jbits[i][j]))
     )/256.0);
 }
	 
 for (i=0;i<cy;i++) for (j=0;j<cx;j++)
     {
      alien_bits(&ar,&ag,&ab,big_map[i][j]);
      jbits[i][j] = RGB(ar,ag,ab);
     }
 for (i=0;i<cy;i++) { if (big_map[i]) free(big_map[i]); big_map[i] = NULL; }
 if (big_map) free(big_map);
 big_map = NULL;
}

void bits_swap(unsigned long** bits)
{
 register int i,j;
 for (i=0;i<cy;i++)
 for (j=0;j<cx;j++) bits[i][j] = RGB(0xFF, 0xFF, 0xFF) - bits[i][j];
}

void free_C_memory(unsigned long*** map, int cx, int cy)
{
 int i;
 if (!(*map)) return ;
 if (cy==0 || cx==0) return ;
 for (i=0;i<cy;i++) { if ((*map)[i]) free((void*)(*map)[i]); (*map)[i] = NULL; }
 if (*map) free((void*)(*map));
 *map = NULL;
}

void write_bmp()
{
 register int i,j;
 FILE* plik;
 char fname[0x100];
 printf("File name? ");
 scanf("%s", fname);
 plik = fopen(fname,"w");
 if (!plik) { printf("Error writing BMP: %s\n", fname); return ; }
 fprintf(plik,"%c%c",'B', 'M');
 bm_handle.bm_y=cy;
 bm_handle.bm_x=cx;
 bm_handle.fsize = sizeof(BMPTag)+(bm_handle.bm_y*bm_handle.bm_x*3);
 fwrite(&bm_handle.fsize,4,1,plik);
 fwrite(&bm_handle.dummy,4,1,plik);
 bm_handle.offset=sizeof(BMPTag);
 bm_handle.planes=1;
 bm_handle.bpp=24;
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
 for (i=0;i<bm_handle.bm_y;i++)  for (j=0;j<bm_handle.bm_x;j++)
   fprintf(plik,"%c%c%c", (unsigned char)jpeg_ReturnBlue(bits[i][j]),
		          (unsigned char)jpeg_ReturnGreen(bits[i][j]),
			  (unsigned char)jpeg_ReturnRed(bits[i][j]));
 fclose(plik);
 printf("\nBITMAP WRITTEN: %s\n\n", fname);
}

void load_bmp(unsigned long*** bit, int* x, int* y, char* fname)
{
 char bm_b, bm_m;
 char r,g,b;
 register int i,j;
 FILE* plik = fopen(fname,"r");
 if (!plik) { printf("Error opening BMP: %s\n", fname); exit(1); }
 fscanf(plik,"%c%c",&bm_b, &bm_m);
 if (bm_b != 'B' || bm_m != 'M') { printf("Probably not a bitmap image: %s\n", fname); exit(2); }
 bm_handle.bm_y=cy;
 bm_handle.bm_x=cx;
 bm_handle.fsize = sizeof(BMPTag)+(bm_handle.bm_y*bm_handle.bm_x*3);
 fread(&bm_handle.fsize,4,1,plik);
 fread(&bm_handle.dummy,4,1,plik);
 bm_handle.offset=sizeof(BMPTag);
 bm_handle.planes=1;
 bm_handle.bpp=24;
 fread(&bm_handle.offset,4,1,plik);
 fread(&bm_handle.dummy2,4,1,plik);
 fread(&bm_handle.bm_x,4,1,plik);
 fread(&bm_handle.bm_y,4,1,plik);
 fread(&bm_handle.planes,2,1,plik);
 fread(&bm_handle.bpp,2,1,plik);
 fread(&bm_handle.compress,4,1,plik);
 fread(&bm_handle.nbytes,4,1,plik);
 for (i=0;i<4;i++)  fread(&bm_handle.no_matter[i],4,1,plik);
 fseek(plik,bm_handle.offset,SEEK_SET);
 *x = bm_handle.bm_x;
 *y = bm_handle.bm_y;
 *bit = (unsigned long**)malloc(bm_handle.bm_y*sizeof(unsigned long*));
 for (i=0;i<bm_handle.bm_y;i++) (*bit)[i] = (unsigned long*)malloc(bm_handle.bm_x*sizeof(unsigned long));
 for (i=0;i<bm_handle.bm_y;i++)  for (j=0;j<bm_handle.bm_x;j++)
   {
    fscanf(plik,"%c%c%c", &r,&g,&b);
    (*bit)[i][j] = RGB((int)b,(int)g,(int)r);
   }
 fclose(plik);
 printf("\nBITMAP READ: %s\n\n", fname);
}

void write_tex_solid()
{
 unsigned int x,y,r,g,b,a;
 FILE* plik;
 char fname[0x100];
 printf("File name? ");
 scanf("%s", fname);
 plik = fopen(fname,"w");
 if (!plik) { printf("Error writing TEX: %s\n", fname); return ; }
 x = y = 1;
 fprintf(plik,"%d,%d\n",x,y);
 printf("R,G,B,A? (write in correct order in hex)? ");
 scanf("%x,%x,%x,%x", &r, &g, &b, &a);
 fprintf(plik,"%c%c%c%c", r,g,b,a);
 printf("Buffer was (%dx%d), Texture Alpha(%d) (%dx%d) written to %s\n", cx,cy,a,x,y,fname);
 fclose(plik);
}

void write_tex()
{
 int x,y,a;
 register int i,j;
 FILE* plik;
 char fname[0x100];
 unsigned char tm;
 printf("File name? ");
 scanf("%s", fname);
 plik = fopen(fname,"w");
 if (!plik) { printf("Error writing TEX: %s\n", fname); return ; }
 x = 1;
 y = 1;
 while (x < cx) x *= 2;
 while (y < cy) y *= 2;
 if (x > cx) x /= 2;
 if (y > cy) y /= 2;
 printf("Alpha (0-255)? ");
 scanf("%d", &a);
 if (a < 0) a = 0;
 if (a > 0xFF) a = 0xFF;
 fwrite((void*)(&x), sizeof(int), 1, plik);
 fwrite((void*)(&y), sizeof(int), 1, plik);
 for (i=0;i<y;i++)
 for (j=0;j<x;j++)
   {
    tm = (unsigned char)jpeg_ReturnRed(bits[i][j]),
    fwrite((void*)(&tm), sizeof(unsigned char), 1, plik);
    tm = (unsigned char)jpeg_ReturnGreen(bits[i][j]),
    fwrite((void*)(&tm), sizeof(unsigned char), 1, plik);
    tm = (unsigned char)jpeg_ReturnBlue(bits[i][j]),
    fwrite((void*)(&tm), sizeof(unsigned char), 1, plik);
    tm = (unsigned char)a;
    fwrite((void*)(&tm), sizeof(unsigned char), 1, plik);
   }
 printf("Buffer was (%dx%d), Texture Alpha(%d) (%dx%d) written to %s\n", cx,cy,a,x,y,fname);
 x = 0XDEADBEEF;
 fwrite((void*)(&x), sizeof(int), 1, plik);
 fclose(plik);
}

int main(int lb, char** par)
{
 char cmd;
 char fn[0x100];
 bmptag(&bm_handle); 
 cmd = ' ';
 if (lb >= 2) load_bmp(&bits, &cx, &cy, par[1]);
 else 
   {
    printf("Bitmap file name? ");
    scanf("%s", fn);
    load_bmp(&bits, &cx, &cy, fn);
   }
 do
   {
    if (cmd != '\n') printf("a\t - alien transform\ni\t - invert transform\nw\t - write bmp result\nt\t - write texture result\ns\t - write solid texture\n");
    scanf("%c", &cmd);
    switch (cmd)
      {
	case '\n': break;
	case 'a': alien_trans(bits); break;
        case 'i': bits_swap(bits); break;
        case 'w': write_bmp(); break;
	case 't': write_tex(); break;
	case 's': write_tex_solid(); break;
	case 'q': printf("Bye!\n"); break;
	default: printf("Unknown cmd.\n");
      }
   }
 while (cmd != 'q');
 free_C_memory(&bits,cx,cy);
 return 0;
}

