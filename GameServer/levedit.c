#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include "gcomdefs.h"
#define BITS16
/*#define BITS24*/
unsigned char* matrix;
FILE* f;
Display* dsp;
Window win;
GC gc;
int x,y;
int prevx, prevy;
typedef unsigned long ulong;
typedef unsigned char uchar;
uchar pix[0x100][8][8][3];
ulong pixf[0x100][8][8];
uchar last;
#ifdef BITS24

unsigned long RGB(int r, int g, int b)  /*set color from r,g,b*/
{
 return ((r&0xFF)<<0X10)+((g&0XFF)<<0X8)+b;
}


int ReturnRed(ulong col)
{
 return (int)((0xff0000&col)>>0X10);
}


int ReturnGreen(ulong col)
{
 return (int)((0x00ff00&col)>>0X8);
}


int ReturnBlue(ulong col)
{
 return (int)(0x0000ff&col);
}

#endif
#ifdef BITS16

unsigned long RGB(int r, int g, int b)  /*set color from r,g,b*/
{
 return (((r>>3)&0x1F)<<0X0B)+(((g>>2)&0X3F)<<0X5)+((b>>3)&0x1F);
}


int ReturnRed(ulong col)
{
 return (int)(((0XF800&col)>>0XB)<<0X3);
}


int ReturnGreen(ulong col)
{
 return (int)(((0x7e0&col)>>0X5)<<0X2);
}


int ReturnBlue(ulong col)
{
 return (int)((0X1F&col)<<0X3);
}

#endif

void error(char* fmt, ...)
{
 va_list lst;
 va_start(lst,fmt);
 printf("CRITICAL ERROR: ");
 vprintf(fmt,lst);
 printf("\nSERVER HALTED.\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}


void warning(char* fmt, ...)
{
 va_list lst;
 va_start(lst,fmt);
 printf("WARNING: ");
 vprintf(fmt,lst);
 fflush(stdout);
}


void alloc_mem()
{
 matrix = (unsigned char*)malloc(0x1001*sizeof(unsigned char));
 if (!matrix) error("malloc main ptr_table");
 printf("MEMORY ALLOCATED.\n");
}


void free_mem()
{
 if (!matrix) return ;
 if (matrix) { free(matrix); matrix=0; }
 matrix = 0;
 printf("MEMORY FREED.\n");
}


void flush_matrix()
{
 int i;
 fseek(f,0,SEEK_SET);
 for (i=0;i<0x1000;i++) fputc((int)matrix[i],f);
}


void catch_int()
{
 flush_matrix();
 fclose(f);
 free_mem();
}


void setup_signals()
{
 static struct sigaction act;
 act.sa_handler = catch_int;
 sigfillset(&(act.sa_mask));
 sigaction(SIGINT, &act, NULL);
}


void draw_field(int x, int y, int idx)
{
 int i,j;
 for (i=0;i<8;i++)
 for (j=0;j<8;j++)
   {
    XSetForeground(dsp,gc,pixf[(uint)matrix[idx]][i][j]);
    XDrawPoint(dsp,win,gc, 8*x+i,8*y+j);
   }
}


void refresh_window()
{
 draw_field(prevx, prevy, prevx*0x40+prevy);
 XSetForeground(dsp,gc,RGB(0,0xFF,0));
 XDrawRectangle(dsp,win,gc,8*x,8*y,7,7);
}


void draw_entire()
{
 int i,j;
 for (i=0;i<0x40;i++)
 for (j=0;j<0x40;j++) draw_field(i, j, i*0x40+j);
}


void move_up()
{
 y--;
 if (y<0) y = 0x3F;
 refresh_window();
 prevy = y;
}


void move_down()
{
 y++;
 if (y>=0x40) y = 0;
 refresh_window();
 prevy = y;
}


void move_right()
{
 x++;
 if (x>=0x40) x = 0;
 refresh_window();
 prevx = x;
}


void move_left()
{
 x--;
 if (x<0) x = 0x3F;
 refresh_window();
 prevx = x;
}


void enter()
{
 matrix[0x40*x+y]++;
 if (matrix[0x40*x+y]>=0xFF) matrix[0x40*x+y] = 0X21;
 last = matrix[0x40*x+y];
 draw_field(x,y,x*0x40+y);
 refresh_window();
}


void space()
{
 matrix[0x40*x+y]--;
 if (matrix[0x40*x+y]<0x21) matrix[0x40*x+y] = 0XFF;
 last = matrix[0x40*x+y];
 draw_field(x,y,x*0x40+y);
 refresh_window();
}


void last_selected()
{
 matrix[0x40*x+y] = last;
 draw_field(x,y,x*0x40+y);
 refresh_window();
}


void X()
{
 int s_num;
 XEvent an_event;
 if ((dsp=XOpenDisplay(NULL))==NULL) error("connect to X");
 s_num = DefaultScreen(dsp);
 win = XCreateSimpleWindow(dsp, RootWindow(dsp, s_num),0, 0, 0x200, 0x200, 1,WhitePixel(dsp, s_num),BlackPixel(dsp, s_num));
 XMapWindow(dsp, win);
 XFlush(dsp);
 gc = XCreateGC(dsp, win, 0, NULL);
 if ((int)gc<0) error("create GC");
 XSetForeground(dsp, gc, WhitePixel(dsp, s_num));
 XSetBackground(dsp, gc, BlackPixel(dsp, s_num));
 XSelectInput(dsp, win, ExposureMask | KeyPressMask | StructureNotifyMask);
 x=y=prevx=prevy=0x1F;
 draw_entire();
 refresh_window();
 while (1)
    {
     XNextEvent(dsp, &an_event);
     switch (an_event.type)
        {
         case Expose:
	     draw_entire();
	     refresh_window();
             break;
        case ConfigureNotify:
             XResizeWindow(dsp,win,0x200,0x200);
	     draw_entire();
	     refresh_window();
             break;
        case KeyPress:
	     if (an_event.xkey.keycode==KEY_Q) return;
	     else if (an_event.xkey.keycode==KEY_UP)    move_up();
	     else if (an_event.xkey.keycode==KEY_DOWN)  move_down();
	     else if (an_event.xkey.keycode==KEY_RIGHT) move_right();
	     else if (an_event.xkey.keycode==KEY_LEFT)  move_left();
	     else if (an_event.xkey.keycode==KEY_CR)    enter();
	     else if (an_event.xkey.keycode==KEY_SPC)   space();
	     else if (an_event.xkey.keycode==KEY_M)     last_selected();
	     else if (an_event.xkey.keycode>=KEY_1 && an_event.xkey.keycode<=KEY_0)
	       {
		last = FIELD_NONE-KEY_1+an_event.xkey.keycode;
		last_selected();
	       }
	     else if (an_event.xkey.keycode>=KEY_A && an_event.xkey.keycode<=KEY_L)
	       {
		last = FIELD_GRASS-KEY_A+an_event.xkey.keycode;
		last_selected();
	       }
	     else if (an_event.xkey.keycode>=KEY_Z && an_event.xkey.keycode<=KEY_N)
	       {
		last = FIELD_WALL_D3-KEY_Z+an_event.xkey.keycode;
		last_selected();
	       }
	     else if (an_event.xkey.keycode>=KEY_W && an_event.xkey.keycode<=KEY_P)
	       {
		last = FIELD_GRASS_FDOWN-KEY_W+an_event.xkey.keycode;
		last_selected();
	       }
/*	     printf("%x\n", an_event.xkey.keycode);*/
             break;
        default:
/*	     refresh_window();*/
             break;
        }
   }
}


void read_file(int recover)
{
 int i,z;
 for (i=0;i<0x1000;i++)
   {
    z = fgetc(f);
    if (z==EOF) error("EOF while expecting data");
    matrix[i] = (char)z;
    if (recover && matrix[i]<FIELD_NONE) matrix[i] = FIELD_NONE;
   }
 matrix[0x1000] = 0;
}


void transform_pixels()
{
 int i,j,k;
 printf("TRANSFORMING PIXEL DATA...\n");
 last = FIELD_NONE;
 for (i=0;i<0x100;i++)
 for (j=0;j<0x8;j++)
 for (k=0;k<0x8;k++) pixf[i][j][k] = RGB(pix[i][j][k][0],pix[i][j][k][1],pix[i][j][k][2]);
}


void load_pixels(char* fn)
{
 FILE* dat;
 unsigned int i,j,k;
 int tmp,tmp2;
 char str[0x100];
 dat = fopen(fn,"r");
 if (!dat) { flush_matrix(); free_mem(); fclose(f); error("cannot read DATAFILE: %s", fn); }
 for (i=0;i<0x100;i++)
  {
   tmp = fscanf(dat,"%s %02x\n", str,(uint*)(&tmp2));
   if (tmp!=2) { flush_matrix(); free_mem(); fclose(f); error("bad DATAFILE format: %s, FIELD=%x", fn,i); }
   if (strcmp(str,"FIELD")) { flush_matrix(); free_mem(); fclose(f); error("bad DATAFILE format: %s", fn); }
   for (j=0;j<8;j++)
     {
      for (k=0;k<8;k++) if (fscanf(dat,"(%02x,%02x,%02x) ", (uint*)(&pix[i][j][k][0]), (uint*)(&pix[i][j][k][1]), (uint*)(&pix[i][j][k][2]))!=3)
         { flush_matrix(); free_mem(); fclose(f); error("bad DATAFILE format: %s, FIELD:%x:%x:%x", fn,i,j,k); }
      fscanf(dat,"\n");
     }
  }
 fclose(dat);
}


void help()
{
 printf("KEYS: ARROWS, ENTER (NEXT FIELD TYPE), SPACE (8 PREV), D (LAST)\n");
 printf("NUMBERS, AND LETTERS BELOW RECALL COMMON FIELDS, THEN USE 'M'\n");
 printf("RECOVER [-r], changes all fields less than FIELD_NONE to FIELD_NONE\n");
 printf("TYPICAL USAGE: levedit graphics.dat game.dat\n");
 printf("graphics.dat - DEFINES how to display board elements.\n");
 printf("game.dat     - DEFINES level to play on it.\n");
}


void initial_matrix()
{
 int i;
 for (i=0;i<0x1000;i++) matrix[i] = FIELD_NONE;
}


void ledit(char* dat, char* fn, int recover)
{
 setup_signals();
 alloc_mem();
 help();
 f = fopen(fn,"r");
 if (f) { read_file(recover); fclose(f); }
 else initial_matrix();
 f = fopen(fn,"w");
 if (!f) error("cannot write to file: %s", fn);
 load_pixels(dat);
 transform_pixels();
 X();
 flush_matrix();
 free_mem();
 fclose(f);
}


int main(int lb, char** par)
{
 int recover;
 if (lb<3) { printf("usage: %s datafile filename [-r]\n", par[0]); help(); return 1; }
 recover = 0;
 if (lb>=4 && !strcmp(par[3],"-r")) recover = 1;
 ledit(par[1], par[2], recover);
 return 0;
}

