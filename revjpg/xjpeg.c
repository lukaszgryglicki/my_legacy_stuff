#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

#define effect_in(par) ((par) & (0xFF))
#define effect_out(par) (par)

extern int ARGB(int,int,int);
extern int load_jpeg_file(unsigned long***, int*, int*, const char*);
#define RGB ARGB16
static int cx = 0;
static int cy = 0;
static int done = 0;
static int less = 0;
unsigned long** bits;

double** create_matrix(int dim)
{
 double** m;
 int i;
 m = NULL;
 m = (double**)malloc(dim*sizeof(double*));
 for (i=0;i<dim;i++) m[i] = (double*)malloc(dim*sizeof(double));
 return m;
}

int try_swap(double** m, int idx)
{
 register int x;
 for (x=idx;x<less;x++) if (m[idx][x]) return x;
 return -1;
}

double** invert_matrix(double** srcC)
{
 double** src;
 double** dst;
 double div, pom;
 register int x,k;
 int i,swit;
 double* vectmp;
 src = create_matrix(less);
 dst = create_matrix(less);
 vectmp = (double*)malloc(less*sizeof(double));
 for (x=0;x<less;x++) for (k=0;k<less;k++) src[x][k] = srcC[x][k];
 for (x=0;x<less;x++) for (k=0;k<less;k++) if (k==x) dst[x][k] = 1.; else dst[k][x] = 0.;
 for (i=0;i<less;i++)
   {
       printf("%d\n", i);
    div = src[i][i];
    if (div == 0.0)
      {
       swit = try_swap(src, i);
       if (swit < 0)
         {
	   printf("UNINVERTABLE_MATRIX\n");
	   exit(1);
	 }
       for (x=0;x<less;x++) vectmp[x]    = src[x][i];
       for (x=0;x<less;x++) src[x][i]    = src[x][swit];
       for (x=0;x<less;x++) src[x][swit] = vectmp[x];
       for (x=0;x<less;x++) vectmp[x]    = dst[x][i];
       for (x=0;x<less;x++) dst[x][i]    = dst[x][swit];
       for (x=0;x<less;x++) dst[x][swit] = vectmp[x];
       div = src[i][i];
      }
    for (x=0;x<less;x++)
      {
       src[x][i] /= div;
       dst[x][i] /= div;
      }
    for (k=0;k<less;k++)
      {
       pom = src[i][k];
       if (k-i)
         {
          for (x=0;x<less;x++) src[x][k] -= pom* src[x][i];
          for (x=0;x<less;x++) dst[x][k] -= pom* dst[x][i];
         }
      }
   }
 for (k=0;k<less;k++) free(src[k]);
 free(src);
 free(vectmp);
 return dst;
}
 
void revert_jpeg(unsigned long** b, int x, int y)
{
 double** matrix;
 double** rev;
 register int i,j;
 printf("reverting... ");
 fflush(stdout);
 less = (y>x)?x:y;
/* less = 200;*/
 matrix = (double**)malloc(less*sizeof(double*));
 for (i=0;i<less;i++) matrix[i] = (double*)malloc(less*sizeof(double));
 for (i=0;i<less;i++) for (j=0;j<less;j++) 
   {
/*    matrix[i][j] = ((double)(b[i][j]-0x7fff))/128.;*/
      matrix[i][j] = effect_in(b[i][j]);
   }
/* rev = invert_matrix(matrix);*/
 for (i=0;i<less;i++) for (j=0;j<less;j++) 
   {
/*    b[i][j] = (unsigned long)((rev[i][j]+0x7fff)*128.);*/
      b[i][j] = effect_out(matrix[i][j]);
   } 
 for (i=0;i<less;i++) free(matrix[i]);
 free(matrix);
 printf("revert done.\n");
}

void draw_jpeg(Display* dsp, Window win, GC gc, unsigned long** jbits)
{
 register int i,j,I=cy-1;			
 for (i=0;i<less;i++,I--) for (j=0;j<less;j++)
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
  int err;
  Window win;
  GC gc;
  XEvent an_event;
  if (lb<2) { printf("usage:\t%s file.jpg\n", par[0]); return 2; }
  err = load_jpeg_file(&bits, &cx, &cy, par[1]);
  if (err) { printf("load_jpeg_file failed!\n"); return 8; }
  revert_jpeg(bits, cy, cx);
  dsp = XOpenDisplay(NULL);
  if (!dsp) {printf("%s: X-server error\n", par[0]); return 1;}
  s_num = DefaultScreen(dsp);
  dx = DisplayWidth(dsp, s_num);
  dy = DisplayHeight(dsp, s_num);
  win = XCreateSimpleWindow
  (dsp, RootWindow(dsp, s_num),0, 0, less, less, 1,BlackPixel(dsp, s_num),WhitePixel(dsp, s_num));
  XMapWindow(dsp, win);
  XFlush(dsp);
  gc = XCreateGC(dsp, win, 0, NULL);
  if (gc < (GC)0) {printf("GC failed to create!\n"); return 4;}
  XSetForeground(dsp, gc, WhitePixel(dsp, s_num));
  XSetBackground(dsp, gc, BlackPixel(dsp, s_num));
  XSelectInput(dsp, win, ExposureMask | KeyPressMask | StructureNotifyMask);
  draw_jpeg(dsp,win,gc,bits);
  while (!done)
    {
      XNextEvent(dsp, &an_event);
      switch (an_event.type)
        {
         case Expose:
		 draw_jpeg(dsp,win,gc,bits);
             break;
        case ConfigureNotify:
             XResizeWindow(dsp,win,less,less);
             break;
        case KeyPress:
	     if (an_event.xkey.keycode == 0x18 || an_event.xkey.keycode == 0x9) done=1;
	     printf("code=%d\n", an_event.xkey.keycode);
             break;
        }
   }
 return 0;
}
