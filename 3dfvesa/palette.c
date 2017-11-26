#include <vga.h>
#include <stdio.h>

void init_palette_RGB_323()
{
 for (int i=0;i<0x100;i++)
  vga_setpalette(i,
  (i&0x07)*0x8,
  (i&0x18)*0x2,
  (i&0xe0)/0x4
);
}

int main()
{
 vga_setmode(G800x600x256);
 int col=0;
 init_palette_RGB_323();
 for (int i=0;i<0x300;i++)
   for (int j=0x100;j<0x200;j++)
      { col = i/3 ; vga_setcolor(col); vga_drawpixel(i,j); }

 scanf("%d", &col);
 vga_setmode(TEXT);
// getch();
 return 0;
}
