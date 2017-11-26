
#include <X11/Xlib.h>			/*ANSI C 100% compatible source by Morgoth DBMA*/
#include <stdio.h>			/*X11R6 required*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define START_AX -10
#define START_AY -15	
#define START_AZ 0
#define START_DZ 0.0
#define START_DY 0.0
#define START_DX 0.0
#define half_arc 180.F
#define MAXZ 6.0
#define X_PERSP perspective
#define CX 880
#define CY 660
typedef unsigned long ulong;
typedef unsigned char uchar;

struct Point3DList			/* will store points and their buffer */
{
 float x,y,z;
 float bx,by,bz;
 struct Point3DList* next;
};


static struct Point3DList ***list_t, ***list_h;		/* 2D ARRAY of lists, 3 X SIZE, each list defines surface */
static int frame_skip = 1;				/* every N frame show, user defines */
static int  size  = 0;					/* 5=MIN, 384=MAX, user defines */
static uchar*** shape;					/* TEMPORARY RAME-EATING MATRIX: SIZE x SIZE x SIZE, uuuhh */
static int done = 0;			/*quit when done*/
static Display* dsp;			/*Xserver specifics*/
static Window win;
static GC gc;
static int angle_x = START_AX;		/*rotations*/
static int angle_y = START_AY;
static int angle_z = START_AZ;
static int cx = CX;			/* window sizes */
static int cy = CY;
static int hx = CX/2;			/* window center */
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

void copy_buffers() /*create dat buffer, once on init*/
{
 int i,j;
 struct Point3DList* tmp;
 for (i=0;i<3;i++)  			/* MAIC VALUE 3, because this is 3D image, no more ;-) */
 for (j=0;j<size;j++)  
   {
    tmp = list_h[i][j];
    while (tmp)
      {
       tmp->bx = tmp->x;
       tmp->by = tmp->y;
       tmp->bz = tmp->z;
       tmp = tmp->next;
      }
   }
}

void copy_from_buffers()		 /*set start values before another transformation*/
{
 int i,j;
 struct Point3DList* tmp;
 for (i=0;i<3;i++)  
 for (j=0;j<size;j++)  
   {
    tmp = list_h[i][j];
    while (tmp)
      {
       tmp->x = tmp->bx;
       tmp->y = tmp->by;
       tmp->z = tmp->bz;
       tmp = tmp->next;
      }
   }
}

void add_as_head(int which, int index, int I, int J, int K)	/* add element to 'which' list, on 'index' list in array */
{								/* makethis element head of list, and compute its location in 3D */
 struct Point3DList* tmp;					/* using coordinates I,J,K */
 if (list_h[which][index]==0)
 {
  list_h[which][index] = (struct Point3DList*)malloc(sizeof(struct Point3DList));
  list_h[which][index]->next = 0;
  list_h[which][index]->x = ((float)I-(float)(size/2))*(5.0/(float)size);
  list_h[which][index]->y = ((float)J-(float)(size/2))*(5.0/(float)size);
  list_h[which][index]->z = ((float)K-(float)(size/2))*(5.0/(float)size);
  list_h[which][index]->bx = list_h[which][index]->x;
  list_h[which][index]->by = list_h[which][index]->y;
  list_h[which][index]->bz = list_h[which][index]->z;
  list_t[which][index] = list_h[which][index];
 }
 else
 {
  tmp = (struct Point3DList*)malloc(sizeof(struct Point3DList));
  tmp->next = list_h[which][index];
  tmp->x = ((float)I-(float)(size/2))*(5.0/(float)size);
  tmp->y = ((float)J-(float)(size/2))*(5.0/(float)size);
  tmp->z = ((float)K-(float)(size/2))*(5.0/(float)size);
  tmp->bx = tmp->x;
  tmp->by = tmp->y;
  tmp->bz = tmp->z;
  list_h[which][index] = tmp;  
 }
}

void add_as_tail(int which, int index, int I, int J, int K)		/* same as add_as_head, but makes new element tail of list */
{
 struct Point3DList* tmp;
 if (list_t[which][index]==0)
 {
  list_t[which][index] = (struct Point3DList*)malloc(sizeof(struct Point3DList));
  list_t[which][index]->next = 0;
  list_t[which][index]->x = ((float)I-(float)(size/2))*(5.0/(float)size);
  list_t[which][index]->y = ((float)J-(float)(size/2))*(5.0/(float)size);
  list_t[which][index]->z = ((float)K-(float)(size/2))*(5.0/(float)size);
  list_t[which][index]->bx = list_t[which][index]->x;
  list_t[which][index]->by = list_t[which][index]->y;
  list_t[which][index]->bz = list_t[which][index]->z;
  list_h[which][index] = list_t[which][index];
 }
 else
 {
  tmp = (struct Point3DList*)malloc(sizeof(struct Point3DList));
  list_t[which][index]->next = tmp;
  tmp->next = 0;
  tmp->x = ((float)I-(float)(size/2))*(5.0/(float)size);
  tmp->y = ((float)J-(float)(size/2))*(5.0/(float)size);
  tmp->z = ((float)K-(float)(size/2))*(5.0/(float)size);
  tmp->bx = tmp->x;
  tmp->by = tmp->y;
  tmp->bz = tmp->z;
  list_t[which][index] = tmp;
 }
}

void init()						/* create temporary matrix, compute it and fill lists */
{
 int i,j,k;
 int from,first;
 for (i=0;i<360;i++)					/* function tables */
   {
    sines[i] = sin(((float)i*3.1415926F)/half_arc);
    cosines[i] = cos(((float)i*3.1415926F)/half_arc);
   }
 for (i=0;i<size;i++)  
 for (j=0;j<size;j++)  
 for (k=0;k<size;k++) shape[i][j][k] = 0;
 for (i=0;i<size;i++)
   {
    from = sqrt(((size*size)/4)-((i-(size/2))*(i-(size/2))));
    for (j=(size/2)-from;j<(size/2)+from;j++) 
    for (k=0;k<size;k++) shape[i][j][k]++;
   }
 for (i=0;i<size;i++)
   {
    from = sqrt(((size*size)/4)-((i-(size/2))*(i-(size/2))));
    for (k=(size/2)-from;k<(size/2)+from;k++) 
    for (j=0;j<size;j++) shape[i][j][k]++;
   }
 for (j=0;j<size;j++)
   {
    from = sqrt(((size*size)/4)-((j-(size/2))*(j-(size/2))));
    for (k=(size/2)-from;k<(size/2)+from;k++) 
    for (i=0;i<size;i++) shape[i][j][k]++;
   }
 for (i=0;i<size;i++)
 for (j=0;j<size;j++)
   {
    first = 1;
    for (k=0;k<size;k++)
      {
       if (shape[i][j][k]==3 && first)  
         {
	  first = 0; 
	  add_as_head(0,j, i,j,k);
	 }
       if (shape[i][j][k]<3 && !first)  
         { 
	  add_as_tail(0,j, i,j,k);
	  break; 
	 }
      }
   }
 for (j=0;j<size;j++)
 for (k=0;k<size;k++)
   {
    first = 1;
    for (i=0;i<size;i++)
      {
       if (shape[i][j][k]==3 && first)  
         {
	  first = 0; 
	  add_as_head(1,k, i,j,k);
	 }
       if (shape[i][j][k]<3 && !first)  
         { 
	  add_as_tail(1,k, i,j,k);
	  break; 
	 }
      }
   }
 for (k=0;k<size;k++)
 for (i=0;i<size;i++)
   {
    first = 1;
    for (j=0;j<size;j++)
      {
       if (shape[i][j][k]==3 && first)  
         {
	  first = 0; 
	  add_as_head(2,i, i,j,k);
	 }
       if (shape[i][j][k]<3 && !first)  
         { 
	  add_as_tail(2,i, i,j,k);
	  break; 
	 }
      }
   }
 for (i=0;i<size;i++) 			/* no more needed HUGE matrix */
 for (j=0;j<size;j++) 
	 free(shape[i][j]);
 for (i=0;i<size;i++) 
	 free(shape[i]);
 free(shape);
}




void check_angles()			/* are all data ok ? */
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


void world_transforms()		/* world transformations, no matrixes used */
{
 int i,j;
 float y,x;
 struct Point3DList* tmp;
 check_angles();
 for (i=0;i<3;i++)
 for (j=0;j<size;j++)
  {
   tmp = list_h[i][j];
   while (tmp)
     { 
      tmp->x += delta_x;
      tmp->y += delta_y;
      tmp->z += delta_z;
      tmp = tmp->next;
     }
  }
 for (i=0;i<3;i++)
 for (j=0;j<size;j++)
   {
   tmp = list_h[i][j];
   while (tmp)
     {
      y      = tmp->y*cosines[angle_x] - tmp->z*sines[angle_x];
      tmp->z = tmp->y*sines[angle_x]   + tmp->z*cosines[angle_x];
      tmp->y = y;
      tmp = tmp->next;
     }
   }
 for (i=0;i<3;i++)
 for (j=0;j<size;j++)
   {
   tmp = list_h[i][j];
   while (tmp)
     {
      x      = tmp->x*cosines[angle_y] - tmp->z*sines[angle_y];
      tmp->z = tmp->x*sines[angle_y]   + tmp->z*cosines[angle_y];
      tmp->x = x;
      tmp = tmp->next;
     }
   }
 for (i=0;i<3;i++)
 for (j=0;j<size;j++)
   {
   tmp = list_h[i][j];
   while (tmp)
     {
      x      = tmp->x*cosines[angle_z] - tmp->y*sines[angle_z];
      tmp->y = tmp->x*sines[angle_z]   + tmp->y*cosines[angle_z];
      tmp->x = x;
      tmp = tmp->next;
     }
   }
}
void DrawScene(int isBlack)	/* Draw out scene */
{
 int i,j,l1x,l1y,l2x,l2y;
 struct Point3DList* tmp;
 if (isBlack) { XClearWindow(dsp,win); return; }
 copy_from_buffers();
 world_transforms();
 perspective = (float)cx*1.4;
 XSetForeground(dsp,gc,RGB(0x0,0x80,0xFF));
 for (i=0;i<3;i++)
 for (j=0;j<size;j++)
   {
    tmp = list_h[i][j];
    if (!tmp) continue;
    if (j%frame_skip) continue;
    while (tmp->next)
      {
       l1x = hx + (int)(X_PERSP*tmp->x/(20.0+tmp->z));
       l1y = hy - (int)(X_PERSP*tmp->y/(20.0+tmp->z));
       l2x = hx + (int)(X_PERSP*tmp->next->x/(20.0+tmp->next->z));
       l2y = hy - (int)(X_PERSP*tmp->next->y/(20.0+tmp->next->z));
       XDrawLine(dsp,win,gc,l1x,l1y, l2x, l2y);
       tmp = tmp->next;
      }
    l1x = hx + (int)(X_PERSP*tmp->x/(20.0+tmp->z));
    l1y = hy - (int)(X_PERSP*tmp->y/(20.0+tmp->z));
    l2x = hx + (int)(X_PERSP*list_h[i][j]->x/(20.0+list_h[i][j]->z));
    l2y = hy - (int)(X_PERSP*list_h[i][j]->y/(20.0+list_h[i][j]->z));
    XDrawLine(dsp,win,gc,l1x,l1y, l2x, l2y);
   }
}


void help()			/* not implemented, not needed. */
{
 printf("\tdummy.\n");
}

void dyna_create(int lb)	/* malloc all stuff */
{
 int i,j;
 if (lb<5 || lb>384) exit(0);
 size=lb;
 shape = (uchar***)malloc(size* sizeof(uchar**));
 for (i=0;i<size;i++) shape[i] = (uchar**)malloc(size*sizeof(uchar*));
 for (i=0;i<size;i++) 
 for (j=0;j<size;j++) shape[i][j] = (uchar*)malloc(size*sizeof(uchar));
 list_t = (struct Point3DList***)malloc(3*sizeof(struct Point3DList**));
 list_h = (struct Point3DList***)malloc(3*sizeof(struct Point3DList**));
 for (j=0;j<3;j++)
   {
    list_t[j] = (struct Point3DList**)malloc(size*sizeof(struct Point3DList*));
    for (i=0;i<size;i++) list_t[j][i] = NULL;
    list_h[j] = (struct Point3DList**)malloc(size*sizeof(struct Point3DList*));
    for (i=0;i<size;i++) list_h[j][i] = NULL;
   }
}

void demalloc_list(struct Point3DList* list)   /* free given list */
{
 struct Point3DList* tmp, *prev;
 tmp = list;
 prev = tmp;
 while (tmp)
 {
  prev = tmp;
  tmp = tmp->next;
  free(prev);
 }
}

void dyna_free()				/* free memory, after this function program can do only exit ;-) */
{
 int i,j;
 for (j=0;j<3;j++)
   {
    for (i=0;i<size;i++) demalloc_list(list_h[j][i]); /* LOOP BY J & I  AND LOOP IN demalloc_free, so 3 DIMNESIONS OF PTR ARE FREED */
   }						      /* 2 DIM PTR ARE FREED AS A HEADS OF A LIST, REMAIN 1 DIM */
 free(list_h);					      /* HERE ! */
 free(list_t);
 list_t = 0;
 list_h = 0;
}


int main(int lb, char** par)		/* NO COMMENT */
{
  int s_num;
  int dx,dy;
  int font_h;
  XFontStruct* font_info;
  char* font_name = "*-helvetica-*-12-*";
  XEvent an_event;
  if (lb<3) 
    { 
     printf("%s need two arguments, for example 60 and 2\n"
	    "first stands for matrix calculations, 10 is awfully small, but 384 is big and consumes lots of RAM\n"
	    "because it creates 384X384X384 matrix and operates on it!\n"
	    "the second argument is line skip, for example if you pass 10 then\n"
	    "only one per 10 frame will be displayed\n"
	    "recomended: %s 100 5\n", 
	    par[0],par[0]); 
     return 1; 
    }
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
  frame_skip = atoi(par[2]);
  if (frame_skip<=0) return 2;
  init();
  copy_buffers();
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
                cx = an_event.xconfigure.width;
                cy = an_event.xconfigure.height;
		hx = cx/2;
		hy = cy/2;
		DrawScene(1);
		DrawScene(0);
             break;
	case KeyPress:			/* kay codes used by X */
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
 dyna_free();
 return 0;
}

/* CopyLeft Morgoth DBMA, +48693582014, morgothdbma@o2.pl, heroine@o2.pl */
