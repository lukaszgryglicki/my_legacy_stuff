#ifndef __EMU_XLIB_BY_VESA_
#define __EMU_XLIB_BY_VESA_
#include <vga.h>
#include "mh.h"

typedef void* _unused_;
typedef struct _xconfigure
 {
  _xconfigure() { width=height=0; }
  int width, height;
 };
typedef struct _xbutton
 {
  _xbutton() { x=y=button=0; }
  int x,y;
  int button;
 };
typedef struct _xkey 
 {
  _xkey() { }
  unsigned long keycode;
 };

typedef struct _Event 
 { 
	 _Event() { type = 0x00040000; }
	 unsigned long type; 
	 _xbutton    xbutton;
	 _xkey       xkey;
	 _xconfigure xconfigure;
 };

typedef struct _Window
{
 _Window() { sx=sy=random(200); lx=ly=0x100;}
 _Window(int a) { sx=sy=a; lx=ly=0x100; colb=0xffffff; colf=0x0;}
 int sx,sy;
 int lx,ly;
 unsigned long colb, colf;
 bool operator!() { return !(bool)this; }
 operator bool() { return (bool)this; }
};

typedef struct _FontData
{
 _FontData() { for (int i=0;i<12;i++) for (int j=0;j<6;j++) fdt[i][j] = 1; }
 unsigned char fdt[12][6];
};

static _FontData fmatrix[0x100];

typedef struct _Font  
  { 
   _Font() { fid = 0xdeeb; ascent=6; descent=6;}
   unsigned long fid;  
   int ascent, descent;
  };

typedef _unused_ Display;
typedef _unused_ GC;
typedef _Window Window;
typedef _Font XFontStruct;
typedef _Event XEvent;
typedef _unused_ XTextProperty;
typedef unsigned long ulong;

#define ExposureMask 		0x00000001
#define KeyPressMask 		0x00000002
#define KeyReleaseMask		0x00000004
#define ButtonPressMask		0x00000008
#define ButtonReleaseMask	0x00000010
#define Button1MotionMask	0x00000020
#define Button2MotionMask	0x00000040
#define Button3MotionMask	0x00000080
#define PointerMotionMask	0x00000100
#define StructureNotifyMask	0x00000200


#define Expose			0x00000400
#define ConfigureNotify		0x00000800
#define ButtonPress		0x00001000
#define Button1			0x00002000
#define Button2			0x00004000
#define Button3			0x00008000
#define ButtonRelease		0x00010000
#define MotionNotify		0x00020000
#define KeyPress		0x00040000
#define KeyRelease		0x00080000

ulong RGB(int r, int g, int b)
{
 r/=8; g/=4; b/=8;
 return 0x800*(r%0x20)+0x20*(g%0x40)+(b%0x20);
}

inline void XDrawLine(Display*, Window&, GC&,int,int,int,int);
inline void XSetForeground(Display* dsp, GC& gc, ulong col);

void DHaFr(Display* dsp, Window& w, ulong col, GC& gc, long fade, int thin, int sx, int ex, int sy, int ey, bool upper)
{
 for (int i=0;i<thin;i++)
   {
    XSetForeground(dsp, gc, col+i*fade);
    if (upper)
     {
      XDrawLine(dsp, w ,gc, sx+i, sy+i, ex-i, sy+i);
      XDrawLine(dsp, w ,gc, sx+i, sy+i, sx+i, ey-i);
     }
    else
     {
      XDrawLine(dsp, w ,gc, sx+i, ey-i, ex-i, ey-i);
      XDrawLine(dsp, w ,gc, ex-i, sy+i, ex-i, ey-i);
     }
   }
}

inline void XUnmapWindow(Display* d, Window& w) 
{ 
 vga_setcolor(0);
 for (int i=w.sx;i<=w.sx+w.lx;i++) vga_drawline(i, w.sy, i, w.sy+w.ly);
 //vga_clear();
 return; 
}
inline void XDestroyWindow(Display* d, Window w) 
{
 Window* ptr = &w;
 delete ptr;
 return; 
}

inline void ClearArea(Display* d, Window& w, GC& g, int x, int y)
{
 vga_setcolor(0);
 for (int i=w.sx;i<=w.sx+x;i++) vga_drawline(i, w.sy, i, w.sy+y);
}

inline int XStringListToTextProperty(char** n, int a, XTextProperty* wt)
{
 return 0;
}

inline void XNextEvent(Display* d, XEvent* ev)
{
 ulong setd = getch();
 if (setd==0x1b) { setd=0x1b0000; setd += 0x100*getch(); setd += getch();} 
 ev->xkey.keycode = setd;
}

void flush_string(char* t)
{
 FILE* f = fopen("out", "a");
 fprintf(f,"%s\n",t);
 fclose(f);
}

ulong Xemul(ulong x)
{
#ifdef DGB
 char str[0x30];
 sprintf(str,"0x%x", x);
 printf("#### XEMUL: KEYCODE:0x%x\n", x);
 flush_string(str);
#endif 
 if (x==36)   return 0x20;
 if (x==24)   return 0x71;
 if (x==34)   return 0x51;
 if (x==23)   return 0x1b5b42;
 if (x==100)  return 0x1b5b43;
 if (x==104)  return 0x9;
 if (x==98)   return 0x1b5b41;
 if (x==102)  return 0x1b5b44;
 if (x==0xfe) return 0x1b5b46; //ENTER is F12
 if (x==0xfb) return 0x8;
 if (x==0xfa) return 0x1b5b57; //ESCAPE is F11
 if (x==198)  return 0x1b5b42;
 if (x==1104) return 0x1b5b41;
 if (x==123)  return 0x9;
}

inline void XDrawPoint(Display* d, Window& w, GC& g, int x, int y)
{
 vga_drawpixel(w.sx+x,w.sy+y);
}

inline void XSetWMName(Display* d, Window& w, XTextProperty* wt)
{
 return;
}

int load_font_file(FILE* dat)
{
 int num;
 int l;
 int ok = fscanf(dat,"%d\n", &num);
 if (ok<0) return 1;
 for (int x=0;x<num;x++)
   {
    ok = fscanf(dat,"0x%x\n", &l);
    if (ok<0) return 1;
    for (int i=0;i<12;i++)
       {
        for (int j=0;j<6;j++)
	  {
           ok = fscanf(dat,"%c", &fmatrix[l].fdt[i][j]);
	   if (ok<0) return 1;
	   fmatrix[l].fdt[i][j] -= '0';
	  }
	ok = fscanf(dat,"\n");
	if (ok<0) return 1;
       }
   }
 return 0;
}

inline void XSetFont(Display* d, GC& g, ulong id)
{
 FILE* font = fopen("font.dat","r");
 if (!font) { vga_setmode(TEXT); exit(1); }
 int err = load_font_file(font);
 fclose(font);
 if (err)   { vga_setmode(TEXT); exit(1); }
 return ;
}

inline void XSelectInput(Display* d, Window& w, ulong p)
{
 return ;
}

inline XFontStruct* XLoadQueryFont(Display* d, char* name)
{
 XFontStruct* fs = new XFontStruct;
 return fs;
}

inline void XResizeWindow(Display* d, Window w, int x, int y)
{
 w.lx = x;
 w.ly = y;
 return ;
}

void XPutLetter(int x, int y, char lit)
{
 for (int i=0;i<12;i++)
 for (int j=0;j<6;j++) if (fmatrix[lit].fdt[i][j]) vga_drawpixel(x+j,y+i);
}

inline void XDrawString(Display* d, Window& w, GC& g, int x, int y, char* txt, int len)
{
 vga_setcolor(RGB(0XFF, 0XFF, 0XFF));
 for (int i=0;i<len;i++) XPutLetter(w.sx+x+i*7, w.sy+y-9, txt[i]);
}

inline void XDrawStringCol(Display* d, Window& w, GC& g, ulong ccc, int x, int y, char* txt, int len)
{
 vga_setcolor(ccc);
 for (int i=0;i<len;i++) XPutLetter(w.sx+x+i*7, w.sy+y-9, txt[i]);
}
inline GC XCreateGC(Display* d, Window& w, int n, void* p)
{
 return (GC)1;
}

inline Window XCreateSimpleWindow(Display* d, Window w2, int x1, int x2, int x3
		,int x4, int x5, ulong p1, ulong p2)
{
 Window* w = new Window;
 w->sx = (x1) ? x1 : random( ((500-x3)>0) ? (500-x3) : 1);
 w->sy = (x2) ? x2 : random( ((400-x4)>0) ? (400-x4) : 1);
 //w->sx = 100;
 //w->sy = 100;
 w->lx = x3;
 w->ly = x4;
 w->colb = p1;
 w->colf = p2;
 w2 = *w;
 return w2;
}

inline void XMapWindow(Display* disp, Window win) 
{ 
 GC gc;
 ClearArea(disp, win, gc, win.lx, win.ly);
 DHaFr(disp, win, RGB(0xf1, 0xf1, 0xf1), gc, -RGB(0x14,0x14,0x14),
 3, 0, win.lx, 0, win.ly, 1);
 DHaFr(disp, win, RGB(0xf1, 0xf1, 0xf1), gc, -RGB(0x14,0x14,0x14),
 3, 0, win.lx, 0, win.ly, 0);
}

void draw_mouse_pointer(Window& w, int x, int y)
{
 vga_setcolor(RGB(0x60,0x78,0x60));
 vga_drawline(w.sx+x-3, w.sy+y-3, w.sx+x+3, w.sy+y-3);
 vga_drawline(w.sx+x-2, w.sy+y-2, w.sx+x+3, w.sy+y-2);
 vga_drawline(w.sx+x-3, w.sy+y-3, w.sx+x-3, w.sy+y+3);
 vga_drawline(w.sx+x-2, w.sy+y-3, w.sx+x-2, w.sy+y+3);
 vga_drawline(w.sx+x-3, w.sy+y-3, w.sx+x+5, w.sy+y+5);
 vga_drawline(w.sx+x-2, w.sy+y-3, w.sx+x+6, w.sy+y+5);
 vga_drawline(w.sx+x-3, w.sy+y-2, w.sx+x+5, w.sy+y+6);
}


void draw_faded_pointer(Window& w, int x, int y)
{
 vga_setcolor(0);
 vga_drawline(w.sx+x-3, w.sy+y-3, w.sx+x+3, w.sy+y-3);
 vga_drawline(w.sx+x-2, w.sy+y-2, w.sx+x+3, w.sy+y-2);
 vga_drawline(w.sx+x-3, w.sy+y-3, w.sx+x-3, w.sy+y+3);
 vga_drawline(w.sx+x-2, w.sy+y-3, w.sx+x-2, w.sy+y+3);
 vga_drawline(w.sx+x-3, w.sy+y-3, w.sx+x+5, w.sy+y+5);
 vga_drawline(w.sx+x-2, w.sy+y-3, w.sx+x+6, w.sy+y+5);
 vga_drawline(w.sx+x-3, w.sy+y-2, w.sx+x+5, w.sy+y+6);
}

inline void EmuXMouse(Display* d, Window& w, GC& g, XEvent& xev, int& mx, int& my)
{
 ulong code = xev.xkey.keycode;
 draw_faded_pointer(w,mx,my);
 if (code==0x1b5b52) 
   { 
    my += 5; xev.type = MotionNotify; 
    xev.xbutton.x = mx;
    xev.xbutton.y = my;
   } 
 if (code==0x1b5b53) 
   { 
    my -= 5; xev.type = MotionNotify; 
    xev.xbutton.x = mx;
    xev.xbutton.y = my;
   } 
 if (code==0x1b5b54) 
   { 
    mx += 5; xev.type = MotionNotify; 
    xev.xbutton.x = mx;
    xev.xbutton.y = my;
   } 
 if (code==0x1b5b51) 
   { 
    mx -= 5; xev.type = MotionNotify; 
    xev.xbutton.x = mx;
    xev.xbutton.y = my;
   } 
 if (code==0x1b5b50) 
   { 
    xev.type = ButtonPress; 
    xev.xbutton.x = mx;
    xev.xbutton.y = my;
    xev.xbutton.button=1;
   } 
 if (code==0x1b5b55) 
   { 
    xev.type = ButtonPress; 
    xev.xbutton.x = mx;
    xev.xbutton.y = my;
    xev.xbutton.button=2;
   } 
 if (code==0x1b5b56) 
   { 
    xev.type = ButtonPress; 
    xev.xbutton.x = mx;
    xev.xbutton.y = my;
    xev.xbutton.button=3;
   } 
 if (mx < 6) mx=6; if (my<6) my=6;
 if (mx>w.lx-6) mx=w.lx-6;
 if (my>w.ly-6) my=w.ly-6;
 draw_mouse_pointer(w,mx,my);
}

inline void XFlush(Display* d) { return; }


inline Window RootWindow(Display* d, int num)
{
 return (Window)1;
}

inline ulong WhitePixel(Display* d, int num) { return RGB(0xff,0xff,0xff); }
inline ulong BlackPixel(Display* d, int num) { return RGB(0,0,0); }

inline void XSetForeground(Display* dsp, GC& gc, ulong col)
{
 vga_setcolor(col);
}

inline void XSetBackground(Display* dsp, GC& gc, ulong col)
{
 vga_setcolor(col); //??
}

inline void XDrawLine(Display* d, Window& w, GC& gc, int x1, int y1, int x2, int y2)
{
 vga_drawline(w.sx+x1,w.sy+y1,w.sx+x2,w.sy+y2);
}

inline Display* XOpenDisplay(char* text)
{
 int ok = vga_setmode(21);
 //int ok=0x0;
 if (ok) return (Display*)NULL;
 else return (Display*)(0xDEAD);
}

inline int DefaultScreen(Display* dsp) { return 0; }
inline int DisplayWidth(Display* dsp, int a) { return 0; }
inline int DisplayHeight(Display* dsp, int a) { return 0; }

#endif

