#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define PI 3.141592653

static int cx = 800;
static int cy = 600;
static int done = 0;

double omega1 = 95.0;    /**/
double omega2 = 100.0;   /**/
double omega3 = 105.0;   /**/
double A  = 390.0;       /**/ 
double B  = 290.0;       /**/
double C  = 230.9;       /**/
double fi = PI;          /**/
double fi2= PI/2; 	 /**/
double inter = 0.0005;  /**/
double ax = 0.0;        /**/
double ay = 5.0;        /**/
double az = 5.0;        /**/
double distance=1800.0; /**/

#define BITS16
/*#define BITS24*/
#ifdef BITS24

unsigned long RGB(int r, int g, int b)  /*set color from r,g,b*/
{
 return ((r&0xFF)<<0X10)+((g&0XFF)<<0X8)+b;
}
#endif
#ifdef BITS16

unsigned long RGB(int r, int g, int b)  /*set color from r,g,b*/
{
 return (((r>>3)&0x1F)<<0X0B)+(((g>>2)&0X3F)<<0X5)+((b>>3)&0x1F);
}
#endif

void process_key(unsigned long key)
{
 switch (key)
 {
  case 24: omega1+=0.2; break;
  case 38: omega1-=0.2; if (omega1<1.0) omega1=1.0;break;
  case 25: omega2+=0.2; break;
  case 39: omega2-=0.2; if (omega2<1.0) omega2=1.0;break;
  case 26: omega3+=0.2; break;
  case 40: omega3-=0.2; if (omega3<1.0) omega3=1.0;break;
  case 27: A+=2.0; if (A>=(double)(cx)) A=(double)(cx); break;
  case 41: A-=2.0; if (A<=2.0) A=2.0; break;
  case 28: B+=2.0; if (B>=(double)(cy)) B=(double)(cy); break;
  case 42: B-=2.0; if (B<=2.0) B=2.0; break;
  case 29: C+=2.0; if (C>=(double)(cy)) C=(double)(cy); break;
  case 43: C-=2.0; if (C<=2.0) B=2.0; break;
  case 30: fi+=PI/300; if (fi>=2*PI) fi-=2*PI; break;
  case 44: fi-=PI/300; if (fi<=0) fi+=2*PI; break;
  case 31: fi2+=PI/300; if (fi2>=2*PI) fi2-=2*PI; break;
  case 45: fi2-=PI/300; if (fi2<=0) fi2+=2*PI; break;
  case 32: inter*=1.03; if (inter>=0.01) inter=0.01; break;
  case 46: inter/=1.03; if (inter<=0.00000001) inter=0.00000001; break;
  case 52: ax+=0.03; if (ax>=2*PI) ax-=2*PI; break;
  case 54: ay+=0.03; if (ay>=2*PI) ay-=2*PI; break;
  case 56: az+=0.03; if (az>=2*PI) az-=2*PI; break;
  case 53: ax-=0.03; if (ax<0.) ax+=2*PI; break;
  case 55: ay-=0.03; if (ay<0.) ay+=2*PI; break;
  case 57: az-=0.03; if (az<0.) az+=2*PI; break;
  case 58: distance*=1.03; if (distance>20000.0) distance=20000.; break;
  case 59: distance/=1.03; if (distance<800.0) distance=800.; break;
  case 65: omega1=omega2=omega3=100.0; A=B=cy; C=cy/2; fi=fi2=PI; ax=ay=az=0.; distance=2000.; inter=0.0004; break;
 }
 printf("%d:%4.1f,%4.1f,%4.1f,%4.1f,%4.1f,%4.1f,%4.3f,%4.3f,%4.2f,%4.2f,%4.2f,%1.6f,%3.0f\n",(int)key,omega1,omega2,omega3,A,B,C,fi,fi2,ax,ay,az,inter,distance);
}
void draw_scene(Display* dsp, Window win, GC gc, int sizx, int sizy)
{
 double t;
 double ccx,ccy,ccz,nnx,nny,nnz;
 double dtmp;
 double perspective;
 int hx,hy,l1x,l2x,l1y,l2y;
 hx = sizx/2;
 hy = sizy/2;
 perspective = sizx;
 XClearWindow(dsp,win);
 XSetForeground(dsp,gc,RGB(0xFF,0xFF,0xFF));
 for (t=0.0;t<=1.0;t+=inter)
   {
    ccx = A*cos(omega1*t);
    ccy = B*cos(omega2*t+fi);
    ccz = C*cos(omega3*t+fi2);
    nnx = A*cos(omega1*(t+inter));
    nny = B*cos(omega2*(t+inter)+fi);
    nnz = C*cos(omega3*(t+inter)+fi2); 

    dtmp= ccy*cos(ax)+ccz*sin(ax);
    ccz = ccy*sin(ax)+ccz*cos(ax);
    ccy = dtmp;
    dtmp= ccx*cos(ay)+ccz*sin(ay);
    ccz = ccx*sin(ay)+ccz*cos(ay);
    ccx = dtmp;
    dtmp= ccx*cos(az)-ccy*sin(az);
    ccy = ccx*sin(az)+ccy*cos(az);
    ccx = dtmp;

    dtmp= nny*cos(ax)+nnz*sin(ax);
    nnz = nny*sin(ax)+nnz*cos(ax);
    nny = dtmp;
    dtmp= nnx*cos(ay)+nnz*sin(ay);
    nnz = nnx*sin(ay)+nnz*cos(ay);
    nnx = dtmp;
    dtmp= nnx*cos(az)-nny*sin(az);
    nny = nnx*sin(az)+nny*cos(az);
    nnx = dtmp;
    
    l1x = hx + (int)(perspective*ccx/(distance+ccz));
    l1y = hy - (int)(perspective*ccy/(distance+ccz));
    l2x = hx + (int)(perspective*nnx/(distance+nnz));
    l2y = hy - (int)(perspective*nny/(distance+nnz));
    XDrawLine(dsp,win,gc,l1x,l1y, l2x, l2y);
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
  (dsp, RootWindow(dsp, s_num),0, 0, cx, cy, 1,WhitePixel(dsp, s_num),BlackPixel(dsp, s_num));
  XMapWindow(dsp, win);
  XFlush(dsp);
  gc = XCreateGC(dsp, win, 0, NULL);
  if ((int)gc<0) {printf("GC failed to create!\n"); return -2;}
  XSetForeground(dsp, gc, BlackPixel(dsp, s_num));
  XSetBackground(dsp, gc, WhitePixel(dsp, s_num));
  XSelectInput(dsp, win, ExposureMask | KeyPressMask | StructureNotifyMask);
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
	     if (an_event.xkey.keycode == 0x9) done=1;
	     process_key(an_event.xkey.keycode);
	     draw_scene(dsp,win,gc,cx,cy);
             break;
        }
   }
 return 0;
}

