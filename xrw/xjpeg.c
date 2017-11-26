#include <X11/Xlib.h>
#include <stdio.h>

extern int ARGB(int,int,int);
extern int load_jpeg_file(unsigned long***, int*, int*, const char*);
#define RGB ARGB24
static int cx;
static int cy;
static int done = 0;
unsigned long** bits;

void draw_jpeg(Display* dsp, Window win, GC gc, unsigned long** jbits)
{
 register int I=cy-1;			
 for (register int i=0;i<cy;i++,I--) for (register int j=0;j<cx;j++)
   {
    XSetForeground(dsp,gc,jbits[i][j]);
    XDrawPoint(dsp,win,gc,j,I);
   }
}

int main(int lb, char** par)
{
  Display* dsp;
  int s_num;
  int dx,dy;
  int font_h;
  int x,y;
  if (lb<2) { printf("usage:\t%s file.jpg\n", par[0]); return 2; }
  //*************//
  int err = load_jpeg_file(&bits, &cx, &cy, par[1]);
  //*************//
  if (err) { printf("load_jpeg_file failed!\n"); return 8; }
  dsp = XOpenDisplay(NULL);
  if (!dsp) {printf("X-server error\n", par[0]); return 1;}
  s_num = DefaultScreen(dsp);
  dx = DisplayWidth(dsp, s_num);
  dy = DisplayHeight(dsp, s_num);
  Window win;
  win = XCreateSimpleWindow
  (dsp, RootWindow(dsp, s_num),0, 0, cx, cy, 1,BlackPixel(dsp, s_num),WhitePixel(dsp, s_num));
  XMapWindow(dsp, win);
  XFlush(dsp);
  GC gc;
  gc = XCreateGC(dsp, win, 0, NULL);
  if (gc < 0) {printf("GC failed to create!\n"); return 4;}
  XSetForeground(dsp, gc, WhitePixel(dsp, s_num));
  XSetBackground(dsp, gc, BlackPixel(dsp, s_num));
  XSelectInput(dsp, win, ExposureMask | KeyPressMask | StructureNotifyMask);
  XEvent an_event;
  while (!done)
    {
      XNextEvent(dsp, &an_event);
      switch (an_event.type)
        {
         case Expose:
		 draw_jpeg(dsp,win,gc,bits);
             break;
        case ConfigureNotify:
             XResizeWindow(dsp,win,cx,cy);
             break;
        case KeyPress:
	     if (an_event.xkey.keycode == 0x18 || an_event.xkey.keycode == 0x9) done=1;
	     printf("code=%d\n", an_event.xkey.keycode);
             break;
        }
   }
 return 0;
}
