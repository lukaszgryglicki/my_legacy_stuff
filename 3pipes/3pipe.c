
#include <X11/Xlib.h>			/*ANSI C 100% compatible source by Morgoth DBMA*/
#include <stdio.h>			/*X11R6 required*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define START_AX -10		/*30*/
#define START_AY -15		/*-25*/
#define START_AZ 0		/*15*/
#define START_DZ 0.0
#define START_DY 0.0
#define START_DX 0.0
#define half_arc 180.F
#define MAXZ 6.0
#define X_PERSP perspective
#define ZMAGIC 0X6FFFFFFF
#define FAR_AWAY 1e7
#define CX 350
#define CY 250
typedef unsigned long ulong;
static FILE* _ran_device;		/* where to get random values */

struct Point3D
{
 float x,y,z;
 char p;
 ulong c;
};

struct Zbuff					/*test depth*/
{
 ulong c;
 float Z;
};

static int  size  = 0;
static struct Point3D*** shape;
static struct Point3D*** shape_buff;
static struct Zbuff** zbuff;
static int done = 0;			/*quit when done*/
static Display* dsp;			/*Xserver specifics*/
static Window win;
static GC gc;
static int angle_x = START_AX;		/*rotations*/
static int angle_y = START_AY;
static int angle_z = START_AZ;
static int cx = CX;			/* window sizes */
static int cy = CY;
static int hx = CX/2;
static int hy = CY/2;
static float perspective = 0.;		/* perspective */
static float sines[360];		/*tables of function values*/
static float cosines[360];
static float delta_z=0.0;
static float delta_y=0.0;
static float delta_x=0.0;

#define BITS16
/*#define BITS24*/
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
int Randomize()   /* init random engine */
{
 if ((_ran_device=fopen("/dev/urandom","r"))==NULL)
   {
    printf("RANLIB Ooops:\nCannot open device: /dev/urandom");
    return 0;
   }
 else return 1;
}


int Random(int lb)   /* randomize number from 0 to lb-1 */
{
 int ins[4],i;
 ulong result;
 for (i=0;i<4;i++)
     ins[i] = fgetc(_ran_device);
 result = ins[0] + 0x100*ins[1] + 0x10000*ins[2]+0x1000000*ins[3];
 result /= (0xffffffff/lb);
 return (int)result;
}


void Kill_random()  /* close random seed */
{
  if (_ran_device) fclose(_ran_device);
}



void copy_from_buffers()		 /*set start values before another transformation*/
{
 int i,j,k;
 for (i=0;i<size;i++)  
 for (j=0;j<size;j++)  
 for (k=0;k<size;k++)  
    {
     if (!shape[i][j][k].p) continue;
     memcpy((void*)(&shape[i][j][k]),(void*)(&shape_buff[i][j][k]),sizeof(struct Point3D));
    }
}


void init() /*create sine/cosine tables and board*/
{
 int i,j,k;
 int from;
 for (i=0;i<360;i++)
   {
    sines[i] = sin(((float)i*3.1415926F)/half_arc);
    cosines[i] = cos(((float)i*3.1415926F)/half_arc);
   }
 for (i=0;i<size;i++)  
 for (j=0;j<size;j++)  
 for (k=0;k<size;k++) 
   {
    shape[i][j][k].x = ((float)i-(float)(size/2))*(5.0/(float)size);
    shape[i][j][k].y = ((float)j-(float)(size/2))*(5.0/(float)size);
    shape[i][j][k].z = ((float)k-(float)(size/2))*(5.0/(float)size);
    shape[i][j][k].c = RGB((i*255)/size, (j*255)/size, (k*255)/size);
    shape[i][j][k].p = 0;
   }
 for (i=0;i<size;i++)
   {
    from = sqrt(((size*size)/4)-((i-(size/2))*(i-(size/2))));
    for (j=(size/2)-from;j<(size/2)+from;j++) 
    for (k=0;k<size;k++) shape[i][j][k].p++;
   }
 for (i=1;i<size;i++)
   {
    from = sqrt(((size*size)/4)-((i-(size/2))*(i-(size/2))));
    for (k=(size/2)-from;k<(size/2)+from;k++) 
    for (j=0;j<size;j++) shape[i][j][k].p++;
   }
 for (j=1;j<size;j++)
   {
    from = sqrt(((size*size)/4)-((j-(size/2))*(j-(size/2))));
    for (k=(size/2)-from;k<(size/2)+from;k++) 
    for (i=0;i<size;i++) shape[i][j][k].p++;
   }
 for (i=0;i<size;i++)  
 for (j=0;j<size;j++)  
 for (k=0;k<size;k++) if (shape[i][j][k].p!=3) shape[i][j][k].p = 0;
}


void copy_buffers() /*create dat buffer, once on init*/
{
 int i,j,k;
 for (i=0;i<size;i++)  
 for (j=0;j<size;j++)  
 for (k=0;k<size;k++) 
    {
     if (!shape[i][j][k].p) continue;
     memcpy((void*)(&shape_buff[i][j][k]),(void*)(&shape[i][j][k]),sizeof(struct Point3D));
    }
}


void check_angles()			/* czy wszystkie dane sa sensowne ? */
{
 if (angle_x<0)     angle_x +=  360;
 if (angle_x>=360)  angle_x -=  360;
 if (angle_y<0)     angle_y +=  360;
 if (angle_y>=360)  angle_y -=  360;
 if (angle_z<0)     angle_z +=  360;
 if (angle_z>=360)  angle_z -=  360;
 if (delta_z<-MAXZ) delta_z  =- MAXZ;
 if (delta_z>MAXZ)  delta_z  =  MAXZ;
 if (delta_x<-MAXZ) delta_x  =- MAXZ;
 if (delta_x>MAXZ)  delta_x  =  MAXZ;
 if (delta_y<-MAXZ) delta_y  =- MAXZ;
 if (delta_y>MAXZ)  delta_y  =  MAXZ;
}


void world_transforms()		/* przeksztalcenia swiata nie uzywam ZADNYCH macierzy */
{
 int i,j,k;
 float y,x;
 check_angles();
 for (i=0;i<size;i++)
 for (j=0;j<size;j++)
 for (k=0;k<size;k++)
  {
   if (!shape[i][j][k].p) continue;
   shape[i][j][k].x += delta_x;
   shape[i][j][k].y += delta_x;
   shape[i][j][k].z += delta_x;
  }
 for (i=0;i<size;i++)
 for (j=0;j<size;j++)
 for (k=0;k<size;k++)
   {
   if (!shape[i][j][k].p) continue;
    y                = shape[i][j][k].y*cosines[angle_x] - shape[i][j][k].z*sines[angle_x];
    shape[i][j][k].z = shape[i][j][k].y*sines[angle_x]   + shape[i][j][k].z*cosines[angle_x];
    shape[i][j][k].y = y;
   }
 for (i=0;i<size;i++)
 for (j=0;j<size;j++)
 for (k=0;k<size;k++)
   {
   if (!shape[i][j][k].p) continue;
    x                = shape[i][j][k].x*cosines[angle_y] - shape[i][j][k].z*sines[angle_y];
    shape[i][j][k].z = shape[i][j][k].x*sines[angle_y]   + shape[i][j][k].z*cosines[angle_y];
    shape[i][j][k].x = x;
   }
 for (i=0;i<size;i++)
 for (j=0;j<size;j++)
 for (k=0;k<size;k++)
   {
   if (!shape[i][j][k].p) continue;
    x                = shape[i][j][k].x*cosines[angle_z] - shape[i][j][k].y*sines[angle_z];
    shape[i][j][k].y = shape[i][j][k].x*sines[angle_z]   + shape[i][j][k].y*cosines[angle_z];
    shape[i][j][k].x = x;
   }
}
void create_zbuff()
{
 int i;
 zbuff = (struct Zbuff**)malloc(sizeof(struct ZBuff*)*cx);
 for (i=0;i<cx;i++) zbuff[i] = (struct Zbuff*)malloc(sizeof(struct Zbuff)*cy);
}

void delete_zbuff()
{
 int i;
 for (i=0;i<cx;i++) free(zbuff[i]);
 free(zbuff);
}

void clear_Zbuff()	/* Wyczysc Z-buffer przed kolejna klatka animacji */
{
 int i,j;
 for (i=0;i<cx;i++)
	 for (j=0;j<cy;j++) { zbuff[i][j].c = ZMAGIC; zbuff[i][j].Z = FAR_AWAY; }
}

void flush_Zbuff()	/* wyrzuc Z-Buffer na ekran */
{
 int i,j;
 for (i=0;i<cx;i++)
	 for (j=0;j<cy;j++)
	 {
	   if (zbuff[i][j].c != ZMAGIC) 
	   {
	    XSetForeground(dsp,gc,zbuff[i][j].c);
	    XDrawPoint(dsp,win,gc,i,j);
	   }
	  zbuff[i][j].c   = ZMAGIC;
	  zbuff[i][j].Z   = FAR_AWAY;
	 }
}

void DrawScene(int isBlack)	/* Rysuje cala scene */
{
 int i,j,k,lx,ly;
 if (isBlack) { XClearWindow(dsp,win); return; }
 copy_from_buffers();
 world_transforms();
 perspective = (float)cx*1.4;
 for (i=0;i<size;i++)
 for (j=0;j<size;j++)
 for (k=0;k<size;k++)
   {
    if (!shape[i][j][k].p) continue;
    XSetForeground(dsp,gc,shape[i][j][k].c);
    lx = hx + (int)(X_PERSP*shape[i][j][k].x/(20.0+shape[i][j][k].z));
    ly = hy - (int)(X_PERSP*shape[i][j][k].y/(20.0+shape[i][j][k].z));
    lx = (lx<0)?0:lx;
    ly = (ly<0)?0:ly;
    lx = (lx>=cx)?cx-1:lx;
    ly = (ly>=cy)?cy-1:ly;
    if (zbuff[lx][ly].Z>shape[i][j][k].z) 
      {
       zbuff[lx][ly].Z = shape[i][j][k].z; 
       zbuff[lx][ly].c = shape[i][j][k].c; 
      }
   }
 flush_Zbuff();
}


void help()
{
 printf("\tdummy.\n");
}

void dyna_create(int lb)
{
 int i,j;
 if (lb<2 || lb>256) exit(0);
 size=lb;
 shape = (struct Point3D***)malloc(size* sizeof(struct Point3D**));
 for (i=0;i<size;i++) shape[i] = (struct Point3D**)malloc(size*sizeof(struct Point3D*));
 for (i=0;i<size;i++) 
 for (j=0;j<size;j++) shape[i][j] = (struct Point3D*)malloc(size*sizeof(struct Point3D));
 shape_buff = (struct Point3D***)malloc(size* sizeof(struct Point3D**));
 for (i=0;i<size;i++) shape_buff[i] = (struct Point3D**)malloc(size*sizeof(struct Point3D*));
 for (i=0;i<size;i++) 
 for (j=0;j<size;j++) shape_buff[i][j] = (struct Point3D*)malloc(size*sizeof(struct Point3D));
}

void dyna_free()
{
 int i,j;
 for (i=0;i<size;i++) 
 for (j=0;j<size;j++) 
	 free(shape[i][j]);
 for (i=0;i<size;i++) 
	 free(shape[i]);
 free(shape);
 for (i=0;i<size;i++) 
 for (j=0;j<size;j++) 
	 free(shape_buff[i][j]);
 for (i=0;i<size;i++) 
	 free(shape_buff[i]);
 free(shape_buff);

}


int main(int lb, char** par)		/* tutaj UNIX przekazuje sterowanie do Xengine */
{
  int s_num;
  int dx,dy;
  int font_h;
  XFontStruct* font_info;
  char* font_name = "*-helvetica-*-12-*";
  XEvent an_event;
  printf("ADD 2D LINE GRID, OPAQUE OF 3D SHAPE\nIMPLEMENT AS ListOfListsOfPoints\n");
  if (lb<2) { printf("%s need one argument, for example 60, but more than 2 and less than 256\n", par[0]); return 1; }
  help();
  dsp = XOpenDisplay(NULL);
  if (!dsp) {printf("X-server error %s\n", par[0]); return 1;}
  s_num = DefaultScreen(dsp);
  dx = DisplayWidth(dsp, s_num);
  dy = DisplayHeight(dsp, s_num);
  win = XCreateSimpleWindow
  (dsp, RootWindow(dsp, s_num),0, 0, cx, cy, 1,WhitePixel(dsp, s_num),BlackPixel(dsp, s_num));
  XMapWindow(dsp, win);
  XFlush(dsp);
  gc = XCreateGC(dsp, win, 0, NULL);
  if (gc<(GC)0) {printf("GC failed to create!\n"); return 2;}
  XSetForeground(dsp, gc, WhitePixel(dsp, s_num));
  XSetBackground(dsp, gc, BlackPixel(dsp, s_num));
  XSelectInput(dsp, win, ExposureMask | KeyPressMask | StructureNotifyMask);
  font_info = XLoadQueryFont(dsp, font_name);
  if (!font_info){ printf("XLoadQueryFont: failed loading font '%s'\n", font_name);return 4;}
  XSetFont(dsp, gc, font_info->fid);
  font_h = font_info->ascent + font_info->descent;
  dyna_create(atoi(par[1]));
  init();
  copy_buffers();
  Randomize();
  create_zbuff();
  clear_Zbuff();
  DrawScene(0);
  while (!done)
    {
      XNextEvent(dsp, &an_event);
      switch (an_event.type)
        {
         case Expose:
		DrawScene(0);
             break;
        case ConfigureNotify:
	        delete_zbuff();
                cx = an_event.xconfigure.width;
                cy = an_event.xconfigure.height;
		hx = cx/2;
		hy = cy/2;
		create_zbuff();
		clear_Zbuff();
		DrawScene(1);
		DrawScene(0);
             break;
	case KeyPress:			/* sa tylko scan-kody klawiszy, nie pamietam co ktory znaczy :-) */
	     if (an_event.xkey.keycode == 0x18) done=1;		/* Q */
	     if (an_event.xkey.keycode == 25  )                 /* W */
		{ DrawScene(1); angle_x+=15; DrawScene(0); }
	     if (an_event.xkey.keycode == 39 )                  /* S */
                { DrawScene(1);  angle_x-=15; DrawScene(0);}
	     if (an_event.xkey.keycode == 27  )                 /* R */
		{ DrawScene(1); angle_z+=15; DrawScene(0); }
	     if (an_event.xkey.keycode == 41 )                  /* F */
                { DrawScene(1);  angle_z-=15; DrawScene(0);}
	     if (an_event.xkey.keycode == 26  )                 /* E */
		{ DrawScene(1); angle_y+=15; DrawScene(0); }
	     if (an_event.xkey.keycode == 40 )                  /* D */
                { DrawScene(1);  angle_y-=15; DrawScene(0);}
	     if (an_event.xkey.keycode == 65 )                  /* SPACEBAR */
                {
		 XClearWindow(dsp,win);
		 angle_x=START_AX;
		 angle_y=START_AY;
		 angle_z=START_AZ;
		 delta_z=START_DZ;
		 delta_y=START_DY;
		 delta_x=START_DX;
		 DrawScene(0);
		}
	     if (an_event.xkey.keycode == 38 )                  /* A */
                {
		 XClearWindow(dsp,win);
		 angle_x=0;
		 angle_y=0;
		 angle_z=0;
		 delta_z=0.;
		 delta_y=0.;
		 delta_x=0.;
		 DrawScene(0);
		}
	     if (an_event.xkey.keycode == 28 )                 /* T */
                { DrawScene(1);  delta_z+=0.5; DrawScene(0);}
	     if (an_event.xkey.keycode == 42 )                 /* G */
                { DrawScene(1);  delta_z-=0.5; DrawScene(0);}
	     if (an_event.xkey.keycode == 54 )                 /* C */
                { DrawScene(1);  delta_x+=0.5; DrawScene(0);}
	     if (an_event.xkey.keycode == 55 )                 /* V */
                { DrawScene(1);  delta_x-=0.5; DrawScene(0);}
	     if (an_event.xkey.keycode == 56 )                 /* B */
                { DrawScene(1);  delta_y+=0.5; DrawScene(0);}
	     if (an_event.xkey.keycode == 57 )                 /* N */
                { DrawScene(1);  delta_y-=0.5; DrawScene(0);}
	     if (an_event.xkey.keycode == 43 ) help();         /* H */
	      /* printf("%d,%d,%d,%f,%f,%f,%d\n", angle_x,angle_y,angle_z,delta_z,delta_y,delta_x,an_event.xkey.keycode);  */
             break;
        }
   }
 delete_zbuff();
 dyna_free();
 return 0;
}

/* CopyLeft Morgoth DBMA, +48693582014, morgothdbma@o2.pl, heroine@o2.pl */
