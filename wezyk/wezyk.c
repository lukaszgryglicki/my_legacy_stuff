#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

Display* dsp=NULL;
Window win;
int cx=900;
int cy=700;
int lx=135;
int ly=105;
GC gc;
int done=0;
int usec=0;
int pid=-1;
bool up=false;
bool left=true;
bool right=false;
bool down=false;

struct Wezyk
{
 int x;
 int y;
 Wezyk* next;
 Wezyk* prev;
 Wezyk();
};

Wezyk :: Wezyk()
{
 next=NULL;
 prev=NULL;
 x=y=0;
}

Wezyk* wezyk=NULL;
Wezyk* lastw=NULL;

void draw_body(Wezyk* w)
{
 if (!w) return ;
 XSetForeground(dsp,gc,0);
 XDrawRectangle(dsp,win,gc,
 w->x*(cx/lx), w->y*(cy/ly), (cx/lx), (cy/ly));
}

void draw_fade(Wezyk* w)
{
 if (!w) return ;
 XSetForeground(dsp,gc,0X00FFFFFF);
 XDrawRectangle(dsp,win,gc,
 w->x*(cx/lx), w->y*(cy/ly), (cx/lx), (cy/ly));
}

void handle_expose()
{
 Wezyk* www=wezyk;
 while (www)
   {
    draw_body(www);
    www = www->next;
   }
}

void fade_down()
{
 Wezyk* www=wezyk;
 while (www)
   {
    draw_fade(www);
    www = www->next;
   }
}

void clear()
{
 up=down=left=right=false;
}

void handle_keydown(unsigned long code)
{
 if (code==100) { clear(); left =true; }
 if (code==102) { clear(); right=true; }
 if (code==98)  { clear(); down =true; }
 if (code==104) { clear(); up   =true; }
}

void quit_notify(int signo)
{
 printf("end timer.\n");
 exit(0);
}

void signal_timer()
{
 signal(SIGUSR1, quit_notify);
 int i=0;
 int start_val=usec;
 usleep(usec*2);
 while (true)
   {
    kill(getppid(), SIGUSR1);
    usleep(usec);
    i++;
    if (!(i%10) && (usec>=(start_val/10))) usec= (usec*9)/10;
   }
}

void make_move();


void timer_proc(int signo)
{
 signal(SIGUSR1, SIG_IGN);
 draw_fade(lastw);
 make_move();
 draw_body(wezyk);
 signal(SIGUSR1, timer_proc);
}

void add_head(int wx, int wy)
{
 Wezyk* www = new Wezyk;
 www->x = wx % lx;
 www->y = wy % ly;
 www->next = wezyk;
 if (wezyk) wezyk->prev = www;
 if (!wezyk) lastw=www;
 wezyk = www;
}

void remove_tail()
{
 if (!lastw) return ;
 Wezyk* www = lastw;
 www = www->prev;
 if (!www) { delete lastw; lastw=wezyk=NULL; }
 else
   {
    delete www->next;
    www->next = NULL;
    lastw=www;
   }
}

void make_move()
{
 if (!wezyk) return;
 Wezyk* www=wezyk;
 Wezyk* neu = new Wezyk;
 if (left ) { neu->x = www->x-1; neu->y=www->y-0; }
 if (right) { neu->x = www->x+1; neu->y=www->y-0; }
 if (up   ) { neu->x = www->x-0; neu->y=www->y+1; }
 if (down ) { neu->x = www->x-0; neu->y=www->y-1; }
 add_head(neu->x, neu->y);
 remove_tail();
}

void free_memory()
{
  Wezyk* free_later=wezyk;
  Wezyk* tmp2=free_later;
  while (free_later)
   {
    tmp2=free_later;
    free_later=free_later->next;
    delete tmp2;
   }
}

void w_init()
{
 add_head(lx/2-2, ly/2);
 add_head(lx/2-1, ly/2);
 add_head(lx/2-0, ly/2);
 add_head(lx/2+1, ly/2);
 add_head(lx/2+2, ly/2);
}

int main(int lb, char** par)				
{
  int s_num;
  int dx,dy;
  int font_h;
  int x,y;					
  dsp = XOpenDisplay(NULL);
  if (!dsp)
    {
     printf("%s: X-server error\npress any key... ", par[0]);
     return -1;
    }
  if (lb>=2) usec=atoi(par[1]);
  else usec=300000;
  signal(SIGUSR1, timer_proc);
  if (!(pid=fork())) signal_timer(); 
  s_num = DefaultScreen(dsp);
  dx = DisplayWidth(dsp, s_num);
  dy = DisplayHeight(dsp, s_num);
  win = XCreateSimpleWindow
  (dsp, RootWindow(dsp, s_num),0, 0, cx, cy, 1,BlackPixel(dsp, s_num),WhitePixel(dsp, s_num));
  XMapWindow(dsp, win);
  XFlush(dsp);
  gc = XCreateGC(dsp, win, 0, NULL);		
  if (gc < 0) {printf("GC failed to create!\n"); return -2;}
  XSetForeground(dsp, gc, WhitePixel(dsp, s_num));
  XSetBackground(dsp, gc, BlackPixel(dsp, s_num));
  XSelectInput(dsp, win, ExposureMask | KeyPressMask  | StructureNotifyMask);
  XFontStruct* font_info;
  char* font_name = "*-helvetica-*-12-*";
  font_info = XLoadQueryFont(dsp, font_name);
  if (!font_info){ printf("XLoadQueryFont: failed loading font '%s'\n", font_name);return -4;}
  XSetFont(dsp, gc, font_info->fid);
  font_h = font_info->ascent + font_info->descent;
  XEvent an_event;
  w_init();
  while (!done)			
    {
      XNextEvent(dsp, &an_event);
      switch (an_event.type)
        {
        case Expose:
	     handle_expose();
             break;
        case ConfigureNotify:
	     XResizeWindow(dsp,win,cx,cy);
	     handle_expose();
             break;
        case KeyPress:
	     if (an_event.xkey.keycode == 0x18) done=1;
	     handle_keydown(an_event.xkey.keycode);
             break;
        default:
             break;
        }
   }
 usleep(30000);
 kill(pid,SIGUSR1);
 printf("waiting...");
 wait(NULL);
 free_memory();
 printf("\n");
 return 0;		
}


