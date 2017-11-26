#include <X11/Xlib.h>
#include <math.h>
#include <stdio.h>
#include "function.h"

static int f_h=8;
static int rotX=15;
static int rotY=20;
static double sx=-3.0;
static double sy=-3.0;
static double ey=3.0;
static double ex=3.0;
static double sz=-5.0;
static double ez=10.0;
static int p1x,p1y,p2x,p2y,p3x,p3y,p4x,p4y,xlen,ylen;
typedef double D;
static int GRID_X = 40;
static int GRID_Y = 20;
static int IRES   = 3;
static int show_grid=1;
static int drawf=1;
static int drawfp=0;
static double n=10;
static double h=1;
static int sgx=1;
static int sgy=1;
static int show_g2=1;
static double hh=0.00001;
static int boom=1;
static int drawfpp=0;
static double (*function_ptr)(double, double)=NULL;
static void** f=NULL;

inline unsigned long RGBs(int r, int g, int b)
{
   return 0x800*r+0x20*g+b;
}


Window create_simple_window(Display* display, int width, int height, int x, int y)
{
  int screen_num = (((_XPrivDisplay) display )->default_screen) ;
  int win_border_width = 1;
  Window win;
  win = XCreateSimpleWindow(display, ((&((_XPrivDisplay)  display  )->screens[   screen_num  ]) ->root) ,
                            x, y, width, height, win_border_width,
                            ((&((_XPrivDisplay)  display  )->screens[   screen_num  ]) ->black_pixel) ,
                            ((&((_XPrivDisplay)  display  )->screens[   screen_num  ]) ->white_pixel) );
  XMapWindow(display, win);
  XFlush(display);
  return win;
}


GC create_gc(Display* display, Window win)
{
  GC gc;
  gc = XCreateGC(display, win, 0, 0);
  return gc;
}


inline double funcprim(double x, double y)
{
 return (function(x,y+hh)+function(x+hh,y)-2*function(x,y))/hh;
}


inline double funcprimp(double x, double y)
{
 return (funcprim(x,y+hh)+funcprim(x+hh,y)-2*funcprim(x,y))/hh;
}

void ReactKey(unsigned int key)
{
 double mv;
 printf("key scancode=%u\n", key);
 if (key==0x14) {rotX--; return;}
 if (key==0x15) {rotX++; return;}
 if (key==0x22) {rotY--; return;}
 if (key==0x23) {rotY++; return;}
 if (key==0x9)  {rotX=15; rotY=20;sx=-3.0;sy=-3.0;sz=-3.0;ex=2.0;ey=5.0;ez=10.0;GRID_X=40; GRID_Y=20; IRES=4;sgx=1;sgy=1; return;}
 if (key==0x26) {rotX=0; rotY=0;GRID_X=3; GRID_Y=3; IRES=3;sgx=2;sgy=1; return;}
 if (key==0x2e) {rotX=40; rotY=40;GRID_X=63; GRID_Y=40; IRES=3;sgx=1;sgy=1;show_grid=0; return;}
 if (key==0x2f) {mv=(ex-sx)/18;sx+=mv;ex+=mv; return;}
 if (key==0x30) {mv=(ex-sx)/18;sx-=mv;ex-=mv; return;}
 if (key==0x3c) {mv=(ey-sy)/18;sy+=mv;ey+=mv; return;}
 if (key==0x3d) {mv=(ey-sy)/18;sy-=mv;ey-=mv; return;}
 if (key==0x31) {mv=(ez-sz)/18;sz+=mv;ez+=mv; return;}
 if (key==0x6a) {mv=(ez-sz)/18;sz-=mv;ez-=mv; return;}
 if (key==0x34) {mv=(ex-sx)/8;sx-=mv;ex+=mv; return;}
 if (key==0x36) {mv=(ey-sy)/8;sy-=mv;ey+=mv; return;}
 if (key==0x38) {mv=(ez-sz)/8;sz-=mv;ez+=mv; return;}
 if (key==0x35) {mv=(ex-sx)/16;sx+=mv;ex-=mv; return;}
 if (key==0x37) {mv=(ey-sy)/16;sy+=mv;ey-=mv; return;}
 if (key==0x39) {mv=(ez-sz)/16;sz+=mv;ez-=mv; return;}
 if (key==0x63) {mv=(ex-sx)/8;sx-=mv;ex+=mv;mv=(ey-sy)/8;sy-=mv;ey+=mv;mv=(ez-sz)/8;sz-=mv;ez+=mv; return;}
 if (key==0x69) {mv=(ex-sx)/16;sx+=mv;ex-=mv;mv=(ey-sy)/16;sy+=mv;ey-=mv;mv=(ez-sz)/16;sz+=mv;ez-=mv; return;}
 if (key==0xa)  {GRID_X--; return;}
 if (key==0xb)  {GRID_X++; return;}
 if (key==0xc)  {GRID_Y--; return;}
 if (key==0xd)  {GRID_Y++; return;}
 if (key==0xe)  {IRES--;   return;}
 if (key==0xf)  {IRES++;   return;}
 if (key==0x29) {show_grid=0; return;}
 if (key==0x2a) {show_grid=1; return;}
 if (key==0x1e) {drawf=0; return;}
 if (key==0x1f) {drawf=1; return;}
 if (key==0x20) {drawfp=0; return;}
 if (key==0x1a) {drawfpp=1; return;}
 if (key==0x21) {drawfp=1; return;}
 if (key==0x19) {drawfpp=0; return;}
 if (key==0x1c) {sgx = !sgx; return;}
 if (key==0x1d) {sgy = !sgy; return;}
 if (key==0x1b)
   {
    if (drawf)  {drawf=0;drawfp=1;drawfpp=0;return;}
    if (drawfp) {drawf=0;drawfp=0;drawfpp=1;return;}
    if (drawfpp){drawf=1;drawfp=0;drawfpp=0;return;}
   }
 if (key==0x28) { if (show_g2) show_g2=0; else show_g2=1; return;}
 if (key==0x10)  {GRID_X*=2; return;}
 if (key==0x11)  {GRID_X/=2; return;}
 if (key==0x12)  {GRID_Y*=2; return;}
 if (key==0x13)  {GRID_Y/=2; return;}
 if (key==0x2c)  {if (boom) boom = !boom; else boom=1;}
}


inline double ToRad(int angle)
{
 return ((D)angle*3.1415926)/180.0;
}



void draw_grid(Display* d, GC& g, Window& w)
{
 for (int i=1;i<GRID_X;i++)
 XDrawLine(d,w,g,p1x+((p2x-p1x)*i)/GRID_X,p1y-((p1y-p2y)*i)/GRID_X,p3x+((p4x-p3x)*i)/GRID_X,p3y-((p3y-p4y)*i)/GRID_X);
 for (int i=1;i<GRID_Y;i++)
 XDrawLine(d,w,g,p1x+((p3x-p1x)*i)/GRID_Y,p1y-((p1y-p3y)*i)/GRID_Y,p2x+((p4x-p2x)*i)/GRID_Y,p2y-((p2y-p4y)*i)/GRID_Y);
}


void draw_limes(Display* d, GC& g, Window& w)
{
 XSetForeground(d,g,RGBs(0,0x20,0x10));
 XDrawLine(d,w,g,p1x,p1y-100,p1x,p1y+50);
 XDrawLine(d,w,g,p2x,p2y-100,p2x,p2y+50);
 XDrawLine(d,w,g,p3x,p3y-100,p3x,p3y+50);
 XDrawLine(d,w,g,p4x,p4y-100,p4x,p4y+50);
 XDrawLine(d,w,g,p1x,p1y-100,p2x,p2y-100);
 XDrawLine(d,w,g,p1x,p1y-100,p3x,p3y-100);
 XDrawLine(d,w,g,p2x,p2y-100,p4x,p4y-100);
 XDrawLine(d,w,g,p4x,p4y-100,p3x,p3y-100);
 XDrawLine(d,w,g,p1x,p1y+50,p2x,p2y+50);
 XDrawLine(d,w,g,p1x,p1y+50,p3x,p3y+50);
 XDrawLine(d,w,g,p2x,p2y+50,p4x,p4y+50);
 XDrawLine(d,w,g,p4x,p4y+50,p3x,p3y+50);
}


void draw_function(Display* d, GC& g, Window& w, int cx, int cy, int num=0)
{
 double tx;
 double ty;
 int Dy;
 function_ptr = (double(*)(double,double))(f[num]);
 if (IRES<=1) IRES=1;
 XPoint* apt = new XPoint[xlen/IRES];
 XSetForeground(d,g,RGBs(0x15*(num==0),0x30*(num==1),0x15*(num==2)));
 if (sgy)
   {
 for (int i=0;i<=GRID_Y;i++)
   {
    ty = sy + ((D)i*(ey-sy))/(D)GRID_Y;
    for (int j=0;j<xlen;j+=IRES)
       {
	tx = sx + ((D)j*(ex-sx))/xlen;
	apt[j/IRES].x = p1x + ((p2x-p1x)*j)/xlen + ((p3x-p1x)*i)/GRID_Y;
	apt[j/IRES].y = p1y - ((p1y-p2y)*j)/xlen - ((p1y-p3y)*i)/GRID_Y;
	Dy =  int(((function_ptr(tx,ty)-sz)*100.0)/(ez-sz)-50.0);
	if (boom)
          {
           if (Dy>100) Dy=100;
	   if (Dy<-50) Dy=-50;
          }
	apt[j/IRES].y -= Dy;
       }
    XDrawLines(d,w,g,apt,xlen/IRES,0 );
   }
 }
 delete [] apt;
 apt = new XPoint[ylen/IRES];
 if (sgx)
   {
 for (int i=0;i<=GRID_X;i++)
   {
    tx = sx + ((D)i*(ex-sx))/(D)GRID_X;
    for (int j=0;j<ylen;j+=IRES)
       {
	ty = sy + ((D)j*(ey-sy))/ylen;
	apt[j/IRES].x = p1x + ((p3x-p1x)*j)/ylen + ((p2x-p1x)*i)/GRID_X;
	apt[j/IRES].y = p1y - ((p1y-p3y)*j)/ylen - ((p1y-p2y)*i)/GRID_X;
	Dy =  int(((function_ptr(tx,ty)-sz)*100.0)/(ez-sz)-50.0);
        if (boom) {
	if (Dy>100) Dy=100;
	if (Dy<-50) Dy=-50; }
	apt[j/IRES].y -= Dy;
       }
    XDrawLines(d,w,g,apt,ylen/IRES,0 );
   }
  }
 delete [] apt;
}




void draw_turrus(Display* d, GC& g, Window& w, int cx, int cy)
{
     p1x   = cx/16;
     p1y   = cy/2;
 int p1len = 4*cx/5;
     p2y   = int(p1y - (D)p1len*sin(ToRad(rotX)));
     p2x   = int(p1x + (D)p1len*cos(ToRad(rotX)));
 int p2len = (cy*rotY)/60;
     p3x   = int(p1x + (D)p2len*sin(ToRad(rotX)));
     p3y   = int(p1y + (D)p2len*cos(ToRad(rotX)));
 int offx  = p3x-p1x;
 int offy  = p3y-p1y;
     p4x   = int(p2x + offx);
     p4y   = int(p2y + offy);
 p1y  = cy-p1y;
 p2y  = cy-p2y;
 p3y  = cy-p3y;
 p4y  = cy-p4y;
 xlen = p1len;
 ylen = p2len;
 XSetForeground(d,g,RGBs(0x5,0x10,0x5));
 XDrawPoint(d,w,g,p1x,p1y);
 XDrawPoint(d,w,g,p2x,p2y);
 XDrawPoint(d,w,g,p3x,p3y);
 XDrawPoint(d,w,g,p4x,p4y);
 XDrawLine(d,w,g,p1x,p1y,p2x,p2y);
 XDrawLine(d,w,g,p1x,p1y,p3x,p3y);
 XDrawLine(d,w,g,p2x,p2y,p4x,p4y);
 XDrawLine(d,w,g,p4x,p4y,p3x,p3y);
 XSetForeground(d,g,RGBs(0,0x25,0));
 if (show_grid)
   {
    draw_limes(d,g,w);
    if (show_g2) draw_grid(d,g,w);
   }
}


inline void std_check()
{
 if (GRID_X<3)   GRID_X=3;
 if (GRID_X>255) GRID_X=255;
 if (GRID_Y<3)   GRID_Y=3;
 if (GRID_Y>255) GRID_Y=255;
 if (IRES<1)     IRES=1;
 if (IRES>10)    IRES=10;
 if (rotX<0)  rotX=  0;
 if (rotX>50) rotX= 50;
 if (rotY<0)  rotY=  0;
 if (rotY>45) rotY= 45;
}


void MakeStatus(Display* dsp, GC& gc, Window& win, int cx, int cy)
{
 std_check();
 draw_turrus(dsp,gc,win,cx,cy);
 if (drawf)   draw_function(dsp,gc,win,cx,cy);
 if (drawfp)  draw_function(dsp,gc,win,cx,cy,1);
 if (drawfpp) draw_function(dsp,gc,win,cx,cy,2);
}


int main(int lb, char** par)
{
  Display* display;
  int screen_num;
  Window win;
  unsigned int display_width, display_height;
  unsigned int cx, cy;
  GC gc;
  display = XOpenDisplay(0 );
  if (!display)
     return -1;
  screen_num = (((_XPrivDisplay) display )->default_screen) ;
  display_width = ((&((_XPrivDisplay)  display  )->screens[   screen_num  ]) ->width) ;
  display_height = ((&((_XPrivDisplay)  display  )->screens[   screen_num  ]) ->height) ;
  cx = 400;
  cy = 300;
  win = create_simple_window(display, cx, cy, 0, 0);
  gc = create_gc(display, win);
  XSelectInput(display, win, (1L<<15)  | (1L<<0)  | (1L<<2)  | (1L<<8)  | (1L<<9)  | (1L<<17) );
  XFontStruct* font_info;
  char* font_name = "*-clean-*-10-*";
  font_info = XLoadQueryFont(display, font_name);
  if (!font_info)
       return -2;
  XSetFont(display, gc, font_info->fid);
  f_h = font_info->ascent + font_info->descent;
  f = new void*[3];
  f[0] = (void*)function;
  f[1] = (void*)funcprim;
  f[2] = (void*)funcprimp;
  h = (ex-sx)/n;
  int done = 0;
  int x,y;
  XEvent an_event;
  std_check();
  MakeStatus(display, gc, win, cx, cy);
  while (!done)
    {
      XNextEvent(display, &an_event);
      switch (an_event.type)
        {
         case 12 :
             XClearWindow(display,win);
             MakeStatus(display, gc, win, cx, cy);
             break;
        case 22 :
             cx = an_event.xconfigure.width;
             cy = an_event.xconfigure.height;
	     XClearWindow(display,win);
	     MakeStatus(display, gc, win, cx, cy);
             break;
        case 2 :
	     if (an_event.xkey.keycode == 0x18) { done=1; break; }
             ReactKey(an_event.xkey.keycode);
	     XClearWindow(display,win);
	     MakeStatus(display, gc, win, cx, cy);
             break;
        default:
             break;
        }
   }
  XFreeGC(display, gc);
  XCloseDisplay(display);
}

