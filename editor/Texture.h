#ifndef __TEXTURE_H_MDBMA_
#define __TEXTURE_H_MDBMA_
typedef struct _Texture
{
 unsigned char* pixels;
 int x,y;
} Texture;

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


int create_texture(Texture*, char*);
#endif