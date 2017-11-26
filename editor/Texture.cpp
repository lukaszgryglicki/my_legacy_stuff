#include "stdafx.h"
#include "Texture.h"

int init_bmp(BMPTag* b)
{
 int i;
 if (!b) return 0;
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
 b->nbytes=4*32*32;
 for (i=0;i<4;i++) b->no_matter[i]=0;
 return 1;
}

void set_color(Texture* s, int x, int y, int r, int g, int b, int a)
{
 if (x >= 0 && x < s->x && y >= 0 && y < s->y)
   {
    s->pixels[4*(s->y * x + y)   ] = r;
    s->pixels[4*(s->y * x + y) +1] = g;
    s->pixels[4*(s->y * x + y) +2] = b;
	s->pixels[4*(s->y * x + y) +3] = a;
   }
 else exit(1);
}

void resize_texture_to_gl_format(Texture* t)
{
 int x,y;
 int i,j,k;
 //char tmp[1000];
 unsigned char* mem;
 if (!t) return;
 x = y = 1;
 while (x < t->x) x *= 2;
 if (x != t->x) x /= 2;
 while (y < t->y) y *= 2;
 if (y != t->y) y /= 2;
 mem = new unsigned char[4*x*y];
 for (i=0;i<y;i++)
 for (j=0;j<x;j++)
 for (k=0;k<4;k++)
       mem[4*(x*i+j)+k] = t->pixels[4*(t->y*j+i)+k];
 delete[] t->pixels;
 t->x = x;
 t->y = y;
 t->pixels = mem;
 //sprintf(tmp, "x=%d, y=%d, mem=%p\n", t->x, t->y, t->pixels);
 //MessageBox(0, tmp, tmp, 0);
}

int create_texture(Texture* t, char* fn)
{
 FILE* plik;
 char r,g,b,m;
 int i,j;
 BMPTag bm_handle;
 plik = fopen(fn, "rb");
 if (!plik) return 0;
 if (!t) return 0;
 if (!init_bmp(&bm_handle)) return 0;
 i = fscanf(plik,"%c%c",&b,&m);
 if (i != 2) return 0;
 if (b != 'B' || m != 'M') return 0;
 fread(&bm_handle.fsize,4,1,plik);
 fread(&bm_handle.dummy,4,1,plik);
 fread(&bm_handle.offset,4,1,plik);
 fread(&bm_handle.dummy2,4,1,plik);
 fread(&bm_handle.bm_x,4,1,plik);
 fread(&bm_handle.bm_y,4,1,plik);
 fread(&bm_handle.planes,2,1,plik);
 fread(&bm_handle.bpp,2,1,plik);
 if (bm_handle.bpp != 24) return 0;
 fseek(plik,bm_handle.offset,SEEK_SET);
 t->pixels = new unsigned char[4*bm_handle.bm_y*bm_handle.bm_x];
 t->x = bm_handle.bm_x;
 t->y = bm_handle.bm_y;
 for (i=0;i<bm_handle.bm_y;i++)  for (j=0;j<bm_handle.bm_x;j++)
    {
     fscanf(plik,"%c%c%c", &b,&g,&r);
     set_color(t, j, i, r, g, b, 235);
    }
 fclose(plik);
 resize_texture_to_gl_format(t);
 return 1;
}
