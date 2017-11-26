#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define PI 3.141592653

static int cx = 800;
static int cy = 600;
static int done = 0;

double omega1 = 100;
double omega2 = 100;
double A  = 390;
double B  = 290;
double fi = PI;
double inter = 0.00003;

void process_key(unsigned long key)
{
 printf("o1=%f, o2=%f, A=%f, B=%f, fi=%f, int=%f\n",omega1,omega2,A,B,fi,inter);
 switch (key)
 {
  case 25: omega1+=0.2; break;
  case 39: omega1-=0.2; if (omega1<1.0) omega1=1.0;break;
  case 26: omega2+=0.2; break;
  case 40: omega2-=0.2; if (omega2<1.0) omega2=1.0;break;
  case 27: A+=2.0; if (A>=(double)(cx/2)) A=(double)(cx/2); break;
  case 41: A-=2.0; if (A<=2.0) A=2.0; break;
  case 28: B+=2.0; if (B>=(double)(cx/2)) B=(double)(cx/2); break;
  case 42: B-=2.0; if (B<=2.0) B=2.0; break;
  case 29: fi+=PI/300; if (fi>=2*PI) fi-=2*PI; break;
  case 43: fi-=PI/300; if (fi<=0) fi+=2*PI; break;
  case 30: inter*=1.03; if (inter>=0.01) inter=0.01; break;
  case 44: inter/=1.03; if (inter<=0.00000001) inter=0.00000001; break;
 }
}

void draw_scene(Display* dsp, Window win, GC gc, int sizx, int sizy)
{
 double t;
 double ccx,ccy,nx,ny;
 int hx,hy;
 printf("draw_scene\n");
 hx = sizx/2;
 hy = sizy/2;
 XClearWindow(dsp,win);
 XSetForeground(dsp,gc,0);
 for (t=0.0;t<=1.0;t+=inter)
   {
    ccx = hx + (int)(A*cos(omega1*t));
    ccy = hy + (int) (B*cos(omega2*t+fi));
    nx = hx + (int)(A*cos(omega1*(t+inter)));
    ny = hy + (int) (B*cos(omega2*(t+inter)+fi));
    XDrawLine(dsp,win,gc,ccx,ccy,nx,ny);
   }
}

int main(int lb, char** par)
{
  Display* dsp;
  Window win;
  XFontStruct* font_info;
  GC gc;
  XEvent an_event;
  int s_num;
  int dx,dy;
  int font_h;
  char* font_name = "*-helvetica-*-12-*";
  dsp = XOpenDisplay(NULL);
  if (!dsp) {printf("%s: X-server error\n", par[0]); return -1;}
  s_num = DefaultScreen(dsp);
  dx = DisplayWidth(dsp, s_num);
  dy = DisplayHeight(dsp, s_num);
  win = XCreateSimpleWindow
  (dsp, RootWindow(dsp, s_num),0, 0, cx, cy, 1,BlackPixel(dsp, s_num),WhitePixel(dsp, s_num));
  XMapWindow(dsp, win);
  XFlush(dsp);
  gc = XCreateGC(dsp, win, 0, NULL);
  if ((int)gc<0) {printf("GC failed to create!\n"); return -2;}
  XSetForeground(dsp, gc, WhitePixel(dsp, s_num));
  XSetBackground(dsp, gc, BlackPixel(dsp, s_num));
  XSelectInput(dsp, win, ExposureMask | KeyPressMask | ButtonPressMask | Button1MotionMask | Button2MotionMask | StructureNotifyMask);
  font_info = XLoadQueryFont(dsp, font_name);
  if (!font_info){ printf("XLoadQueryFont: failed loading font '%s'\n", font_name);return -4;}
  XSetFont(dsp, gc, font_info->fid);
  font_h = font_info->ascent + font_info->descent;
  draw_scene(dsp,win,gc,cx,cy);
  while (!done)
    {
      XNextEvent(dsp, &an_event);
      switch (an_event.type)
        {
         case Expose:
	     draw_scene(dsp,win,gc,cx,cy);
             break;
         case ConfigureNotify:
             cx = an_event.xconfigure.width;
             cy = an_event.xconfigure.height;
	     draw_scene(dsp,win,gc,cx,cy);
             break;
         case KeyPress:
	     if (an_event.xkey.keycode == 0x18) done=1;
	     process_key(an_event.xkey.keycode);
	     draw_scene(dsp,win,gc,cx,cy);
             break;
        }
   }
 return 0;
}

