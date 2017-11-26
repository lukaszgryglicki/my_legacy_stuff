#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int lb, char** par)
{
  Display* dsp;
  Window win;
  GC gc;
  int s_num;
  XFontStruct* font_info;
  XEvent an_event;
  int font_h,rc, done;
  char buf[0x100];
  char* font_name = "*-helvetica-*-10-*";

  int dx,dy;
  int x,y;

  dsp = XOpenDisplay(NULL);
  if (!dsp) 
  { 
      printf("Cannot connect to X server\n"); 
      return 1;
  }

  s_num = DefaultScreen(dsp);
  dx = DisplayWidth(dsp, s_num);
  dy = DisplayHeight(dsp, s_num);
  win = XRootWindow(dsp, s_num);
  win = XCreateSimpleWindow(dsp, RootWindow(dsp, s_num),0, 0, dx>>1, dy>>1, 1,BlackPixel(dsp, s_num),WhitePixel(dsp, s_num));
  XMapWindow(dsp, win);
  XFlush(dsp);
  gc = XCreateGC(dsp, win, 0, NULL);
  if (gc < 0) 
  {
      printf("GC failed to create!\n"); 
      return 2;
  }

  font_info = XLoadQueryFont(dsp, font_name);
  if (!font_info)
  { 
      printf("XLoadQueryFont: failed loading font '%s'\n", font_name);
      return 3;
  }
  XSetFont(dsp, gc, font_info->fid);
  font_h = font_info->ascent + font_info->descent;

  XSetForeground(dsp, gc, WhitePixel(dsp, s_num));
  XSetBackground(dsp, gc, BlackPixel(dsp, s_num));
  XSelectInput(dsp, win, ExposureMask);

  strcpy(buf, "Hello world");

  while (!done)
    {
      XNextEvent(dsp, &an_event);
      switch (an_event.type)
        {
         case Expose:
	        printf("Expose!\n");
 		XSetForeground(dsp, gc, 0xff << 0x8);
  		XDrawString(dsp, win, gc, 50, 50, buf, strlen(buf));
		XFlush(dsp);
             	break;
        default:
             break;
        }
   }

  /*
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
   */
    return 0;
}

