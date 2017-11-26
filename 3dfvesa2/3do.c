#include <vga.h>
#include <math.h>
#include <getch.h>
#include "function.h"

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
static int show_g2=0;
static double hh=0.00001;
static int boom=1;
static int drawfpp=0;
static double (*function_ptr)(double, double)=NULL;
static void** f=NULL;
static int nclr=0;

struct XPoint
{
 public:
 int x,y;
};

void init_palette_RGB_323()
{
 for (int i=0;i<0x100;i++)
  vga_setpalette(i,
  (i&0x07)*0x8,
  (i&0x18)*0x2,
  (i&0xe0)/0x4
);
}

inline unsigned long RGBs(int r, int g, int b)
{
 g /= 2;
 r %= 0x8;
 g %= 0x4;
 b %= 0x8;
 return 0x20*b+0x8*g+r;
}

inline void vga_line(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2)
{
 if (x>=800 || x2>=800 || y>=600 || y2>=600) return;
 vga_drawline(x,y,x2,y2);
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
 if (key=='-') {rotX--; return;}
 if (key=='=') {rotX++; return;}
 if (key=='[') {rotY--; return;}
 if (key==']') {rotY++; return;}
 if (key=='\\')  {rotX=15; rotY=20;sx=-3.0;sy=-3.0;sz=-3.0;ex=2.0;ey=5.0;ez=10.0;GRID_X=40; GRID_Y=20; IRES=4;sgx=1;sgy=1; return;}
 if (key=='a') {rotX=0; rotY=0;GRID_X=3; GRID_Y=3; IRES=3;sgx=2;sgy=1; return;}
 if (key=='l') {rotX=40; rotY=40;GRID_X=63; GRID_Y=40; IRES=3;sgx=1;sgy=1;show_grid=0; return;}
 if (key==';') {mv=(ex-sx)/18;sx+=mv;ex+=mv; return;}
 if (key=='\'') {mv=(ex-sx)/18;sx-=mv;ex-=mv; return;}
 if (key=='.') {mv=(ey-sy)/18;sy+=mv;ey+=mv; return;}
 if (key=='/') {mv=(ey-sy)/18;sy-=mv;ey-=mv; return;}
 if (key=='o') {mv=(ez-sz)/18;sz+=mv;ez+=mv; return;}
 if (key=='p') {mv=(ez-sz)/18;sz-=mv;ez-=mv; return;}
 if (key=='z') {mv=(ex-sx)/8;sx-=mv;ex+=mv; return;}
 if (key=='c') {mv=(ey-sy)/8;sy-=mv;ey+=mv; return;}
 if (key=='b') {mv=(ez-sz)/8;sz-=mv;ez+=mv; return;}
 if (key=='x') {mv=(ex-sx)/16;sx+=mv;ex-=mv; return;}
 if (key=='v') {mv=(ey-sy)/16;sy+=mv;ey-=mv; return;}
 if (key=='n') {mv=(ez-sz)/16;sz+=mv;ez-=mv; return;}
 if (key=='1') {mv=(ex-sx)/8;sx-=mv;ex+=mv;mv=(ey-sy)/8;sy-=mv;ey+=mv;mv=(ez-sz)/8;sz-=mv;ez+=mv; return;}
 if (key=='2') {mv=(ex-sx)/16;sx+=mv;ex-=mv;mv=(ey-sy)/16;sy+=mv;ey-=mv;mv=(ez-sz)/16;sz+=mv;ez-=mv; return;}
 if (key=='g') {show_grid=!show_grid; return;}
 if (key=='y') {drawf=!drawf; return;}
 if (key=='u') {drawfp=!drawfp; return;}
 if (key=='i') {drawfpp=!drawfpp; return;}
 if (key=='h') {sgx = !sgx; return;}
 if (key=='k') {sgy = !sgy; return;}
 if (key=='r')
   {
    if (drawf)  {drawf=0;drawfp=1;drawfpp=0;return;}
    if (drawfp) {drawf=0;drawfp=0;drawfpp=1;return;}
    if (drawfpp){drawf=1;drawfp=0;drawfpp=0;return;}
   }
 if (key=='d') { if (show_g2) show_g2=0; else show_g2=1; return;}
 if (key=='7')  {GRID_X*=2; return;}
 if (key=='8')  {GRID_X/=2; return;}
 if (key=='9')  {GRID_Y*=2; return;}
 if (key=='0')  {GRID_Y/=2; return;}
 if (key=='j')  {if (boom) boom = !boom; else boom=1;}
 if (key==' ')  {vga_clear();}
}


inline double ToRad(int angle)
{
 return ((D)angle*3.1415926)/180.0;
}

void draw_grid()
{
 for (int i=1;i<GRID_X;i++)
 vga_line(p1x+((p2x-p1x)*i)/GRID_X,p1y-((p1y-p2y)*i)/GRID_X,p3x+((p4x-p3x)*i)/GRID_X,p3y-((p3y-p4y)*i)/GRID_X);
 for (int i=1;i<GRID_Y;i++)
 vga_line(p1x+((p3x-p1x)*i)/GRID_Y,p1y-((p1y-p3y)*i)/GRID_Y,p2x+((p4x-p2x)*i)/GRID_Y,p2y-((p2y-p4y)*i)/GRID_Y);
}

void draw_limes()
{
 vga_setcolor(RGBs(0,6,4));
 vga_line(p1x,p1y-100,p1x,p1y+50);
 vga_line(p2x,p2y-100,p2x,p2y+50);
 vga_line(p3x,p3y-100,p3x,p3y+50);
 vga_line(p4x,p4y-100,p4x,p4y+50);
 vga_line(p1x,p1y-100,p2x,p2y-100);
 vga_line(p1x,p1y-100,p3x,p3y-100);
 vga_line(p2x,p2y-100,p4x,p4y-100);
 vga_line(p4x,p4y-100,p3x,p3y-100);
 vga_line(p1x,p1y+50,p2x,p2y+50);
 vga_line(p1x,p1y+50,p3x,p3y+50);
 vga_line(p2x,p2y+50,p4x,p4y+50);
 vga_line(p4x,p4y+50,p3x,p3y+50);
}


void draw_function(int cx, int cy, int num=0)
{
 double tx;
 double ty;
 int Dy;
 function_ptr = (double(*)(double,double))(f[num]);
 if (IRES<=1) IRES=1;
 XPoint* apt = new XPoint[xlen/IRES];
 vga_setcolor(RGBs(0x5*(num==0),0x5*(num==1),0x5*(num==2)));
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
    for (int u=0;u<xlen/IRES-1;u++) vga_line(apt[u].x,apt[u].y,apt[u+1].x,apt[u+1].y);
    //XDrawLines(d,w,g,apt,xlen/IRES,0 );
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
    //XDrawLines(d,w,g,apt,ylen/IRES,0 );
    for (int u=0;u<ylen/IRES-1;u++) vga_line(apt[u].x,apt[u].y,apt[u+1].x,apt[u+1].y);
   }
  }
 delete [] apt;
}

void draw_turrus(int cx, int cy)
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
 vga_setcolor(RGBs(2,4,2));
 vga_drawpixel(p1x,p1y);
 vga_drawpixel(p2x,p2y);
 vga_drawpixel(p3x,p3y);
 vga_drawpixel(p4x,p4y);
 vga_line(p1x,p1y,p2x,p2y);
 vga_line(p1x,p1y,p3x,p3y);
 vga_line(p2x,p2y,p4x,p4y);
 vga_line(p4x,p4y,p3x,p3y);
 vga_setcolor(RGBs(0,6,0));
 if (show_grid)
   {
    draw_limes();
    if (show_g2) draw_grid();
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
 if (rotX<0)     rotX=  0;
 if (rotX>50)    rotX= 50;
 if (rotY<0)     rotY=  0;
 if (rotY>45)    rotY= 45;
}


void MakeStatus(int cx, int cy)
{
 std_check();
 draw_turrus(cx,cy);
 if (drawf)   draw_function(cx,cy);
 if (drawfp)  draw_function(cx,cy,1);
 if (drawfpp) draw_function(cx,cy,2);
}

void dispatch(int lb, char** par)
{
 system("cat ./COPYRIGHT");
 if (lb >=2)
 if (!strcmp(par[1],"--help") || !strcmp(par[1],"-h")) { system("cat ./README"); exit(0); }
 else if (!strcmp(par[1],"--noclr")) nclr=1;
 else { printf("%s parameter is unknown, aborting...\n", par[1]); exit(1);}
}

int main(int lb, char** par)
{
  dispatch(lb, par);
  vga_init();
  vga_setmode(G800x600x256);
  init_palette_RGB_323();
  int cx = 800;
  int cy = 600;
  f = new void*[3];
  f[0] = (void*)function;
  f[1] = (void*)funcprim;
  f[2] = (void*)funcprimp;
  h = (ex-sx)/n;
  int done = 0;
  int x,y;
  int zn=0;
  std_check();
  MakeStatus(cx, cy);
  while (zn!='q') { zn = getch(); ReactKey(zn); if (!nclr) vga_clear(); MakeStatus(cx, cy);}
  getch();
  vga_setmode(TEXT);
  printf("returned to VGA_TEXT_MODE\n");
  return 0;
}

