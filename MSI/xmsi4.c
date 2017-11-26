#include <X11/Xlib.h>			/*ANSI C 100% compatible source by Morgoth DBMA*/
#include <stdio.h>			/*X11R6 required*/
#include <stdlib.h>
#include <stdarg.h>
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
#define BLACK 0
#define WHITE RGB(0XFF,0XFF,0XFF)
#define RED   RGB(0XFF,0,0)
#define BLUE  RGB(0,0,0XFF)
#define GREEN RGB(0,0XFF,0)
#define NLINES    12			/* count lines in cube */
#define NFLOATS   72			/* number of float data in cube */
#define NPOINTS   24			/* number of points in cube (start and end) */
/* ******* */
typedef unsigned long ulong;
static FILE* _ran_device;		/* where to grt random values */

int debug(char* fmt, ...)  /* print out DEBUG information if DEBUG defined */
{
#ifdef DEBUG
 va_list ap;
 int err;
 va_start(ap,fmt);
 err = vprintf(fmt,ap);
 va_end(ap);
 return err;
#endif
#ifndef DEBUG
 return 0;
#endif
}


int Debug(char* fmt, ...)  /* print out DEBUG information if DEBUG defined */
{
#ifdef DEBUGALG
 va_list ap;
 int err;
 va_start(ap,fmt);
 err = vprintf(fmt,ap);
 va_end(ap);
 return err;
#endif
#ifndef DEBUGALG
 return 0;
#endif
}


int Randomize()   /* init random engine */
{
 debug("Randomize\n");
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
 debug("Random\n");
 for (i=0;i<4;i++)
     ins[i] = fgetc(_ran_device);
 result = ins[0] + 0x100*ins[1] + 0x10000*ins[2]+0x1000000*ins[3];
 result /= (0xffffffff/lb);
 return (int)result;
}


void Kill_random()  /* close random seed */
{
 debug("Kill_random\n");
  if (_ran_device) fclose(_ran_device);
}


float board_lines[NFLOATS] =
{
 /* Xp,Yp,Zp, Xk, Yk, Zk */   /* size is 6 X 12 */
 -6,6,-6,-6,-6,-6,
 -6,-6,-6,6,-6,-6,
 6,-6,-6,6,6,-6,
 6,6,-6,-6,6,-6, /* FIRST MAIN WALL */
 -6,6,-6,-6,6,6,
 -6,6,6,6,6,6,
 6,6,6,6,6,-6, /* SECOND WALL */
 6,6,6,6,-6,6,
 6,-6,6,6,-6,-6, /* THIRD WALL */
 -6,-6,-6,-6,-6,6,
 -6,-6,6,-6,6,6, /* FORTH WALL */
 -6,-6,6,6,-6,6, /* FIFTH AND SIXTH WALL */
};
float board_lines_buff[NFLOATS]; /* size 6 X 12 */
static int selected = 0;		/* AKTUALNIE ZAZNACZONE POLE */
static int board[64];			/* board 0-empty, 1-human, 2-CPU */
static int done = 0;			/*quit when done*/
static int end_cond = 0;		/* CPU win or lost currently */
static Display* dsp;			/*Xserver specifics*/
static Window win;
static GC gc;
static int angle_x = START_AX;		/*rotations*/
static int angle_y = START_AY;
static int angle_z = START_AZ;
static float sines[360];		/*tables of function values*/
static float cosines[360];
static int cx = 600;			/* window sizes */
static int cy = 450;
static int hx = 300;
static int hy = 225;
static float perspective = 0.;		/* perspective */
static float sines[360];		/*tables of function values*/
static float cosines[360];
static float delta_z=0.0;
static float delta_y=0.0;
static float delta_x=0.0;
static int cpu_wins=0;
static int hum_wins=0;
static int halt=0;

void stats()
{
 printf("\n***********************\n");
 printf("\n\nHUMAN/CPU: %d/%d\n\n",hum_wins,cpu_wins);
 printf("\n***********************\n");
 if ((hum_wins-cpu_wins)>5)
   {
    printf("GRATULACJE WYGRALES(AS)\n\n\n");
    exit(0);
   }
 if ((hum_wins-cpu_wins)<-5)
   {
    printf("CPU WYGRAL 6 RAZY WIECEJ NIZ TY\n\n\n");
    exit(0);
   }
 printf("Nacisnij 'N' aby zagrac jeszcze raz!\n");
}

/*#define BITS16*/
#define BITS24
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

void copy_from_buffers()		 /*set start values before another transformation*/
{
 int i;;
 debug("copy_from_buffers\n");
 for (i=0;i<NFLOATS;i++)  board_lines[i] = board_lines_buff[i];
}


void init() /*create sine/cosine tables and board*/
{
 int i;
 debug("create_f_table\n");
 for (i=0;i<360;i++)
   {
    sines[i] = sin(((float)i*3.1415926F)/half_arc);
    cosines[i] = cos(((float)i*3.1415926F)/half_arc);
   }
 for (i=0;i<64;i++) board[i] = 0;
}


void copy_buffers() /*create dat buffer, once on init*/
{
 int i;
 debug("copy_buffers\n");
 for (i=0;i<NFLOATS;i++)  board_lines_buff[i] = board_lines[i];
}


void check_angles()			/* czy wszystkie dane sa sensowne ? */
{
#ifdef DEBUG
 printf("check_angles\n");
#endif
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
 if (selected<0)    selected += 64;
 if (selected>=64)  selected -= 64;
}


void world_transforms()		/* przeksztalcenia swiata nie uzywam ZADNYCH macierzy */
{
 int i;
 float y,x;
 debug("world_transforms\n");
 check_angles();
 for (i=0;i<NPOINTS;i++)  board_lines[3*i+2] += delta_z;		/* przesuniecia wzgledem Z */
 for (i=0;i<NPOINTS;i++)  board_lines[3*i  ] += delta_x;		/* przesuniecia wzgledem X */
 for (i=0;i<NPOINTS;i++)  board_lines[3*i+1] += delta_y;		/* przesuniecia wzgledem Y */
 for (i=0;i<NPOINTS;i++)	/* obrot wokow X */
   {
    y             = board_lines[3*i+1]*cosines[angle_x] - board_lines[3*i+2]*sines[angle_x];
    board_lines[3*i+2] = board_lines[3*i+1]*sines[angle_x] + board_lines[3*i+2]*cosines[angle_x];
    board_lines[3*i+1] = y;
   }
 for (i=0;i<NPOINTS;i++)	/* obrot wokol Y */
   {
    x             = board_lines[3*i]*cosines[angle_y] - board_lines[3*i+2]*sines[angle_y];
    board_lines[3*i+2] = board_lines[3*i]*sines[angle_y]   + board_lines[3*i+2]*cosines[angle_y];
    board_lines[3*i]   = x;
   }
 for (i=0;i<NPOINTS;i++)	/* obrot wokol Z */
   {
    x             = board_lines[3*i]*cosines[angle_z] - board_lines[3*i+1]*sines[angle_z];
    board_lines[3*i+1] = board_lines[3*i]*sines[angle_z]   + board_lines[3*i+1]*cosines[angle_z];
    board_lines[3*i] = x;
   }
}


void get_2D_selected_pos(int* dx, int *dy) /* selected to 2D output screen */
{
 int xp,yp,zp;
 *dx = ((cx*61)>>6)-(cx>>1);
 *dy = (cy*5 )>>6;
 xp = (selected%4)-3;
 yp = ((selected/4)%4)-2;
 zp = (selected/16)-1;
 *dx += xp*(cx>>5);
 *dy -= yp*(cy>>5);
 *dx += zp*(cx>>2);
}

void draw_2D_field(int x, int y, int col, int sel)
{
 int offx,offy;
 if (col==0) XSetForeground(dsp,gc,WHITE);
 else if (col==1) XSetForeground(dsp,gc,RED);
 else if (col==2) XSetForeground(dsp,gc,BLUE);
 offx=cx>>6;
 offy=cy>>2;
 if (!col)
   {
    XDrawLine(dsp,win,gc, offx+x*(cx>>6),     offy+y*(cy>>6)    ,offx+x*(cx>>6)    ,offy+(y+1)*(cy>>6));
    XDrawLine(dsp,win,gc, offx+(x+1)*(cx>>6), offy+y*(cy>>6)    ,offx+(x+1)*(cx>>6),offy+(y+1)*(cy>>6));
    XDrawLine(dsp,win,gc, offx+x*(cx>>6),     offy+y*(cy>>6)    ,offx+(x+1)*(cx>>6),offy+y*(cy>>6));
    XDrawLine(dsp,win,gc, offx+x*(cx>>6),     offy+(y+1)*(cy>>6),offx+(x+1)*(cx>>6),offy+(y+1)*(cy>>6));
   }
 else XFillRectangle(dsp,win,gc, offx+x*(cx>>6),offy+y*(cy>>6),cx>>6,cy>>6);

 if (sel)
   {
    XSetForeground(dsp,gc,GREEN);
    XDrawLine(dsp,win,gc, offx+x*(cx>>6),     offy+y*(cy>>6)    ,offx+x*(cx>>6)    ,offy+(y+1)*(cy>>6));
    XDrawLine(dsp,win,gc, offx+(x+1)*(cx>>6), offy+y*(cy>>6)    ,offx+(x+1)*(cx>>6),offy+(y+1)*(cy>>6));
    XDrawLine(dsp,win,gc, offx+x*(cx>>6),     offy+y*(cy>>6)    ,offx+(x+1)*(cx>>6),offy+y*(cy>>6));
    XDrawLine(dsp,win,gc, offx+x*(cx>>6),     offy+(y+1)*(cy>>6),offx+(x+1)*(cx>>6),offy+(y+1)*(cy>>6));
   }
 
}

void draw_2D_objects()			/* draw 2D objects */
{
 int xpoints[5];
 int ypoints[5];
 int i,j,tmps,x;
 debug("draw_2D_objects\n");
 XSetForeground(dsp,gc,WHITE);
 /* grid */
 xpoints[4] = (cx*31)>>5;
 xpoints[3] = (cx*30)>>5;
 xpoints[2] = (cx*29)>>5;
 xpoints[1] = (cx*28)>>5;
 xpoints[0] = (cx*27)>>5;
 ypoints[0] = (cy*1 )>>5;
 ypoints[1] = (cy*2 )>>5;
 ypoints[2] = (cy*3 )>>5;
 ypoints[3] = (cy*4 )>>5;
 ypoints[4] = (cy*5 )>>5;
 for (i=0;i<4;i++)
        {
 	 XDrawLine(dsp,win,gc, xpoints[4], ypoints[0], xpoints[4], ypoints[4]);
 	 XDrawLine(dsp,win,gc, xpoints[3], ypoints[0], xpoints[3], ypoints[4]);
	 XDrawLine(dsp,win,gc, xpoints[2], ypoints[0], xpoints[2], ypoints[4]);
	 XDrawLine(dsp,win,gc, xpoints[1], ypoints[0], xpoints[1], ypoints[4]);
	 XDrawLine(dsp,win,gc, xpoints[0], ypoints[0], xpoints[0], ypoints[4]);
	 XDrawLine(dsp,win,gc, xpoints[4], ypoints[0], xpoints[0], ypoints[0]);
	 XDrawLine(dsp,win,gc, xpoints[4], ypoints[1], xpoints[0], ypoints[1]);
	 XDrawLine(dsp,win,gc, xpoints[4], ypoints[2], xpoints[0], ypoints[2]);
	 XDrawLine(dsp,win,gc, xpoints[4], ypoints[3], xpoints[0], ypoints[3]);
	 XDrawLine(dsp,win,gc, xpoints[4], ypoints[4], xpoints[0], ypoints[4]);
	 for (j=0;j<5;j++) xpoints[j] -= cx>>2;
	}
 /* selected */
 get_2D_selected_pos(&i, &j);
 XSetForeground(dsp,gc,GREEN);
 XDrawLine(dsp,win,gc, i-(cx>>6), j-(cx>>6),i+(cx>>6),j-(cx>>6));
 XDrawLine(dsp,win,gc, i-(cx>>6), j+(cx>>6),i+(cx>>6),j+(cx>>6));
 XDrawLine(dsp,win,gc, i-(cx>>6), j-(cx>>6),i-(cx>>6),j+(cx>>6));
 XDrawLine(dsp,win,gc, i+(cx>>6), j-(cx>>6),i+(cx>>6),j+(cx>>6));
 /* game state */
 for (x=0;x<64;x++) if (board[x])
   {
    tmps = selected;
    selected = x;
    get_2D_selected_pos(&i, &j);
    XSetForeground(dsp,gc,(board[selected]==1)?RED:BLUE);
    XFillRectangle(dsp,win,gc, i-(cx>>7), j-(cx>>7),cx>>6,cy>>6);
    selected = tmps;
   }
 for (i=0;i<64;i++) draw_2D_field(i%8, i/8, board[i],(i==selected)?1:0);
}


void move_x(int arg) /* move selected +/- X */
{
 int tst;
 int tst2;
 debug("move_x:%d\n", arg);
 tst = (selected%4);
 if (arg>0)
   {
    selected++;
    tst2 = (selected%4);
    if (tst2<tst) selected -=4;
   }
 else
   {
    selected--;
    if (selected<0) { selected += 4; return ; }
    tst2 = (selected%4);
    if (tst2>tst) selected +=4;
   }
}


void move_y(int arg) /* move selected +/- Y */
{
 int tst;
 int tst2;
 debug("move_y:%d\n", arg);
 tst = ((selected/4)%4);
 if (arg>0)
   {
    selected+=4;
    tst2 = ((selected/4)%4);
    if (tst2<tst) selected -=16;
   }
 else
   {
    selected-=4;
    if (selected<0) { selected +=16; return; }
    tst2 = ((selected/4)%4);
    if (tst2>tst) selected +=16;
   }
}


void move_z(int arg) /* move selected +/- Z */
{
 int tst;
 debug("move_z:%d\n", arg);
 tst = (selected/16);
 if (arg>0)
   {
    selected+=16;
    if (selected>=64) selected-=64;
   }
 else
   {
    selected-=16;
    if (selected<0) selected+=64;
   }
}


void get_3D_selected_pos(float* dx, float* dy, float* dz)
{
 *dx = (float)((selected%4)-1.5)*3.0;
 *dy = (float)(((selected/4)%4)-1.5)*3.0;
 *dz = (float)((selected/16)-1.5)*3.0;
 debug("get_3D_selected_point(%f,%f,%f)\n",*dx,*dy,*dz);
}


void set_at(int x, int y, int z, int value)
{
 board[x+4*y+16*z] = value;
 /*debug("set_at(%d,%d,%d=>%d,%d)\n",x,y,z,x+4*y+16*z,value);*/
}


int get_at(int x, int y, int z)
{
 /* debug("get_at(%d,%d,%d=>%d):%d\n",x,y,z,x+4*y+16*z,board[x+4*y+16*z]);*/
 return board[x+4*y+16*z];
}


int heuristic_count_moves_to_win(int player)
{
 int xc,yc,zc,i;
 int vec[4];
 int cmin;
 int min;
 debug("MOVES_TO_WIN FOR PLAYER: %d\n", player);
 min = 5;
 /* GO BY Z PARALLEL LINES */
 /* BY Z */
 for (xc=0;xc<4;xc++)
 for (yc=0;yc<4;yc++)
   {
    for (zc=0;zc<4;zc++) vec[zc] = get_at(xc,yc,zc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* BY Y */
 for (xc=0;xc<4;xc++)
 for (zc=0;zc<4;zc++)
   {
    for (yc=0;yc<4;yc++) vec[yc] = get_at(xc,yc,zc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* BY X */
 for (zc=0;zc<4;zc++)
 for (yc=0;yc<4;yc++)
   {
    for (xc=0;xc<4;xc++) vec[xc] = get_at(xc,yc,zc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* PRZEKATNE 2D */
 /* WGLAB Z */
 for (zc=0;zc<4;zc++)
   {
    vec[0] = get_at(0,3, zc);
    vec[1] = get_at(1,2, zc);
    vec[2] = get_at(2,1, zc);
    vec[3] = get_at(3,0, zc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
    vec[0] = get_at(0,0, zc);
    vec[1] = get_at(1,1, zc);
    vec[2] = get_at(2,2, zc);
    vec[3] = get_at(3,3, zc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* WGLAB Y */
 for (yc=0;yc<4;yc++)
   {
    vec[0] = get_at(0,yc,3);
    vec[1] = get_at(1,yc,2);
    vec[2] = get_at(2,yc,1);
    vec[3] = get_at(3,yc,0);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
    vec[0] = get_at(0,yc, 0);
    vec[1] = get_at(1,yc, 1);
    vec[2] = get_at(2,yc, 2);
    vec[3] = get_at(3,yc, 3);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* WGLAB X */
 for (xc=0;xc<4;xc++)
   {
    vec[0] = get_at(xc,3,0);
    vec[1] = get_at(xc,2,1);
    vec[2] = get_at(xc,1,2);
    vec[3] = get_at(xc,0,3);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
    vec[0] = get_at(xc,0,0);
    vec[1] = get_at(xc,1,1);
    vec[2] = get_at(xc,2,2);
    vec[3] = get_at(xc,3,3);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
	   {
	    cmin=6;
	    break;
	   }
	if (vec[i]==player) cmin--;
       }
    if (cmin<min) min=cmin;
   }
 /* PRZEKATNE 3D */
 vec[0] = get_at(0,0,0);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(2,2,2);
 vec[3] = get_at(3,3,3);
 cmin=4;
 for (i=0;i<4;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 cmin=6;
	 break;
	}
     if (vec[i]==player) cmin--;
    }
 if (cmin<min) min=cmin;
 vec[0] = get_at(3,0,0);
 vec[1] = get_at(2,1,1);
 vec[2] = get_at(1,2,2);
 vec[3] = get_at(0,3,3);
 cmin=4;
 for (i=0;i<4;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 cmin=6;
	 break;
	}
     if (vec[i]==player) cmin--;
    }
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,3,0);
 vec[1] = get_at(1,2,1);
 vec[2] = get_at(2,1,2);
 vec[3] = get_at(3,0,3);
 cmin=4;
 for (i=0;i<4;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 cmin=6;
	 break;
	}
     if (vec[i]==player) cmin--;
    }
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,0,3);
 vec[1] = get_at(1,1,2);
 vec[2] = get_at(2,2,1);
 vec[3] = get_at(3,3,0);
 cmin=4;
 for (i=0;i<4;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 cmin=6;
	 break;
	}
     if (vec[i]==player) cmin--;
    }
 if (cmin<min) min=cmin;
 /* KONIEC PRZEKATNYCH 3D */
 debug("MIN = %d\n", min); 
 return min;
}


int heuristic_count_ways_to_win(int player)
{
 int ways;
 int vec[4];
 int xc,yc,zc,i;
 debug("WAYS_TO_WIN FOR PLAYER: %d\n", player);
 ways = 0;
 /* GO BY Z PARALLEL LINES */
 /* BY Z */
 for (xc=0;xc<4;xc++)
 for (yc=0;yc<4;yc++)
   {
    for (zc=0;zc<4;zc++) vec[zc] = get_at(xc,yc,zc);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* BY Y */
 for (xc=0;xc<4;xc++)
 for (zc=0;zc<4;zc++)
   {
    for (yc=0;yc<4;yc++) vec[yc] = get_at(xc,yc,zc);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* BY X */
 for (zc=0;zc<4;zc++)
 for (yc=0;yc<4;yc++)
   {
    for (xc=0;xc<4;xc++) vec[xc] = get_at(xc,yc,zc);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* BY Y */
 /* PRZEKATNE 2D */
 /* WGLAB Z */
 for (zc=0;zc<4;zc++)
   {
    vec[0] = get_at(0,3, zc);
    vec[1] = get_at(1,2, zc);
    vec[2] = get_at(2,1, zc);
    vec[3] = get_at(3,0, zc);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
    vec[0] = get_at(0,0, zc);
    vec[1] = get_at(1,1, zc);
    vec[2] = get_at(2,2, zc);
    vec[3] = get_at(3,3, zc);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* WGLAB Y */
 for (yc=0;yc<4;yc++)
   {
    vec[0] = get_at(0,yc,3);
    vec[1] = get_at(1,yc,2);
    vec[2] = get_at(2,yc,1);
    vec[3] = get_at(3,yc,0);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
    vec[0] = get_at(0,yc, 0);
    vec[1] = get_at(1,yc, 1);
    vec[2] = get_at(2,yc, 2);
    vec[3] = get_at(3,yc, 3);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* WGLAB X */
 for (xc=0;xc<4;xc++)
   {
    vec[0] = get_at(xc,3,0);
    vec[1] = get_at(xc,2,1);
    vec[2] = get_at(xc,1,2);
    vec[3] = get_at(xc,0,3);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
    vec[0] = get_at(xc,0,0);
    vec[1] = get_at(xc,1,1);
    vec[2] = get_at(xc,2,2);
    vec[3] = get_at(xc,3,3);
    for (i=0;i<4;i++)
        if (vec[i] && vec[i]!=player)
	   {
	    ways--;
	    break;
	   }
    ways++;
   }
 /* PRZEKATNE 3D */
 vec[0] = get_at(0,0,0);
 vec[1] = get_at(1,1,1);
 vec[2] = get_at(2,2,2);
 vec[3] = get_at(3,3,3);
 for (i=0;i<4;i++)
     if (vec[i] && vec[i]!=player)
	{
	 ways--;
	 break;
	}
 ways++;
 vec[0] = get_at(3,0,0);
 vec[1] = get_at(2,1,1);
 vec[2] = get_at(1,2,2);
 vec[3] = get_at(0,3,3);
 for (i=0;i<4;i++)
     if (vec[i] && vec[i]!=player)
	{
	 ways--;
	 break;
	}
 ways++;
 vec[0] = get_at(0,3,0);
 vec[1] = get_at(1,2,1);
 vec[2] = get_at(2,1,2);
 vec[3] = get_at(3,0,3);
 for (i=0;i<4;i++)
    {
     if (vec[i] && vec[i]!=player)
	{
	 ways--;
	 break;
	}
    }
 ways++;
 vec[0] = get_at(0,0,3);
 vec[1] = get_at(1,1,2);
 vec[2] = get_at(2,2,1);
 vec[3] = get_at(3,3,0);
 for (i=0;i<4;i++)
     if (vec[i] && vec[i]!=player)
	{
	 ways--;
	 break;
	}
 ways++;
 /* KONIEC PRZEKATNYCH 3D */
 debug("WAYS = %d\n", ways); 
 return ways;
}


void clear_board()
{
 int i;
 debug("clear_board()\n");
 for (i=0;i<64;i++) board[i] = 0;
}

int get_current_heuristics(int*,int*,int*,int*,int,int);

int check_draw(int* h1, int* h2, int n)
{
 int i;
 int nz;
 nz=0;
 for (i=0;i<n;i++) if (h1[i] || h2[i]) nz++;
 if (nz) return 0;
 else return 1;
}

void draw();


int forseen_best_move(int h1, int h2, int h3, int h4, int cpu, int opp)
{
 int i,j,c,draww;
 int *hx1,*hx2,*hx3,*hx4, *at,*val,*vam;
 int *gmove;
 int nmoves,min,hmov,cmov;
 nmoves=0;
 Debug("foreseen_best_move()\n");
 for (i=0;i<64;i++) if (!board[i]) nmoves++;
 hx1 = (int*)malloc(nmoves*sizeof(int));
 hx2 = (int*)malloc(nmoves*sizeof(int));
 hx3 = (int*)malloc(nmoves*sizeof(int));
 hx4 = (int*)malloc(nmoves*sizeof(int));
 at  = (int*)malloc(nmoves*sizeof(int));
 val = (int*)malloc(nmoves*sizeof(int));
 vam = (int*)malloc(nmoves*sizeof(int));
 j=0;
 for (i=0;i<64;i++)
   if (!board[i]) 		/* ten ruch mozliwy */
     {
      board[i] = cpu;		/* zrob go */
      get_current_heuristics(&hx1[j], &hx2[j], &hx3[j], &hx4[j], cpu, opp);
      				/* wez ststystyki po ruchu */
      at[j] = i;		/* gdzie */
      val[j] = 1;
      board[i] = 0;
      j++;			/* ilosc dozwolonych ruchow */
     }
 draww = check_draw(hx3, hx4, nmoves);
 if (draww)
 {
  draw();
  return 0;
 }
 c=0;
 for (i=0;i<64;i++) if (board[i]==1) c++;
 if (c==1 && (board[21]==1 || board[22]==1 || board[25]==1 || board[26]==1 || board[37]==1 || board[38]==1 || board[41]==1 || board[42]==1)) 
	 printf("CPU: Uzytkownik zaczal od srodkowego pola, bedzie trudno wygrac!\n");
 Debug("h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
 for (i=0;i<nmoves;i++)
   Debug("hx1=%d,hx2=%d,hx3=%d,hx4=%d,at=%d,val=%d,i=%d\n",hx1[i],hx2[i],hx3[i],hx4[i],at[i],val[i],i);
 Debug("matrix:\n");
 for (i=0;i<nmoves;i++) if (hx1[i]<=0) /* gdy komputer moze wygrac */
   {
    board[at[i]] = cpu;
    printf("HEURISTICS BEFORE: h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
    printf("WIN HEURISTICS: hx1=%d,hx2=%d,hx3=%d,hx4=%d,at=%d,val=%d,i=%d\n",hx1[i],hx2[i],hx3[i],hx4[i],at[i],val[i],i);
    printf("ONBOARD:%d>> (%d,%d,%d)\n",i,i%4,(i/4)%4,i/16);
    printf("MSI COMPUTED RESULT\n");
    free(at);
    free(val);
    free(vam);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    /* VICTORY HERE! */
    return 1;
   }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 Debug("First heuristics passed.\n");
 /* CO Z PRZECIWNIKIEM PO RUCHU */
 for (i=0;i<nmoves;i++) vam[i] = val[i];		/* FIXME HERE */
 for (i=0;i<nmoves;i++) if (hx2[i]<=2) val[i] = 0;	/* GDY PO TYM I NASTEPNYM RUCHU PRZECIWNIK MOZE WYGRAC */
 for (i=0;i<nmoves;i++) if (hx2[i]<=1) vam[i] = 0;	/* GDY PO TYM RUCHU PRZECIWNIK MOZE WYGRAC */
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==0) for (i=0;i<nmoves;i++) val[i] = vam[i];
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==1) for (i=0;i<nmoves;i++) if (val[i])   /* RATUJ SIE PRZED PRZEGRANA */
   {
    board[at[i]] = cpu;
    free(at);
    free(val);
    free(vam);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    /* UARATOWANY?  LUB 2 MOZLIWOSCI DLA CZLOWIEKA I ZGON */
    return 0;
   }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 /* ZEBY PO RUCHU (JAK NIE DA SIE WYGRAC I NIE MA BEZPOSREDNIEGO ZAGROZENIA PRZEGRANA)
  * BYC JAK NAJBLIZEJ ZWYCIESTWA */
 min = 6;
 for (i=0;i<nmoves;i++) if (hx1[i]<min && val[i]) min = hx1[i];
 for (i=0;i<nmoves;i++) if (hx1[i]>min) val[i] = 0;
 Debug("min=%d\n", min);
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==1) for (i=0;i<nmoves;i++) if (val[i])   /* JAK TYLKO JEDEN MOZLIWY RUCH */
   {
    board[at[i]] = cpu;
    free(at);
    free(val);
    free(vam);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    /* POWINNA BYC DOBRA SYTUACJA */
    return 0;
   }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 /* ABY JAK NAJWIECEJ MOZLIWOSCI RUCHU PO AKTUALNYM*/
 cmov=0;
 for (i=0;i<nmoves;i++) if (hx3[i]>cmov && val[i]) cmov = hx3[i];
 Debug("cmov=%d\n", cmov);
 for (i=0;i<nmoves;i++) if (hx3[i]<cmov) val[i] = 0;
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==1) for (i=0;i<nmoves;i++) if (val[i])   /* JAK TYLKO JEDEN MOZLIWY RUCH */
   {
    board[at[i]] = cpu;
    free(at);
    free(val);
    free(vam);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    return 0;
   }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 /* ABY CZLOWIEK MAIL JAK NAJMNIEJ MOZLIWOSCI RUCH PO AKTUALNYM */
 cmov=100;							/* NIE WIEM ILE MOZLIWOSCI W 4X4 */
 for (i=0;i<nmoves;i++) if (hx4[i]<cmov && val[i]) cmov = hx4[i];
 Debug("cmov=%d\n", cmov);
 for (i=0;i<nmoves;i++) 
 {
  if (cmov!=hx4[i]) val[i] = 0;
 }
 for (i=0;i<nmoves;i++) Debug("%d", val[i]); Debug("\n");
 c=0;
 for (i=0;i<nmoves;i++) if (val[i]) c++;
 if (c==1) for (i=0;i<nmoves;i++) if (val[i])   /* JAK TYLKO JEDEN MOZLIWY RUCH */
   {
    board[at[i]] = cpu;
    free(at);
    free(val);
    free(vam);
    free(hx1);
    free(hx2);
    free(hx3);
    free(hx4);
    return 0;
   }
 Debug("\nThere are %d possible good moves\n", c);
 if (c==0) /* NIE MA DOBREGO RUCHU, PRAKTYCZNIE PRZEGRANA, ZROB COKOLWIEK */
 {
  Debug("SYTUACJA JEST BARDZO ZLA!!!\n");
  hmov=5;							/* FIXME ?? */
  for (i=0;i<nmoves;i++) if (hx1[i]<hmov)  hmov = hx1[i];
  for (i=0;i<nmoves;i++) if (hx1[i]==hmov) { board[at[i]] = cpu; break; }
  /* board[at[Random(nmoves)]] = cpu; */
 }
 else
   {
    gmove = (int*)malloc(c*sizeof(int));
    j=0;
    for (i=0;i<nmoves;i++)
      {
       if (val[i])
         {
          gmove[j] = at[i];
	  j++;
         }
      }
    board[gmove[Random(c)]] = cpu;
    free(gmove);
   }
 free(at);
 free(val);
 free(vam);
 free(hx1);
 free(hx2);
 free(hx3);
 free(hx4);
 return 0;
}


int get_current_heuristics(int* h1, int* h2, int* h3, int* h4, int cpu, int opp)
{
 *h1 = heuristic_count_moves_to_win(cpu);				/*  CPU  */
 *h2 = heuristic_count_moves_to_win(opp);				/* HUMAN */
 *h3 = heuristic_count_ways_to_win(cpu);				/*  CPU  */
 *h4 = heuristic_count_ways_to_win(opp);	        		/* HUMAN */
 debug("H1=%d, H2=%d, H3=%d, H4=%d\n", *h1, *h2, *h3, *h4);
 if ((*h2)<=0) return -1;
 else return 0;
}

void halt_btx()
{
 halt = 1;
}


void cpu_msi_move()
{
 int h1,h2,h3,h4,ret;
 debug("CPU_MSI_MOVE()\n");
 end_cond=0;
 /*set_at(Random(3),Random(3),Random(3), 2);*/
 ret = get_current_heuristics(&h1, &h2, &h3, &h4, 2, 1);
 if (ret==-1)
   {
    printf("HEURISTICS WHEN DEFETED: h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
    printf("PANIC: MSI COMPUTED RESULT, CPU LOST\n");
    printf("\n%c%c%c\nCPU: I HAVE LOST!\n%c%c%c\n",7,7,7,7,7,7);
    end_cond=1;
    hum_wins++;
    halt_btx();
    stats();
    return ;
   }
 ret = forseen_best_move(h1,h2,h3,h4, 2, 1);
 if (ret)
   {
    printf("\n%c%c%c\nCPU: I HAVE WIN!\n%c%c%c\n",7,7,7,7,7,7);
    end_cond=1;
    cpu_wins++;
    halt_btx();
    stats();
   }
 /*set_at(Random(3),Random(3),Random(3), 2);*/
}

void draw()
{
 printf("\n%c%c%c\nCPU: DRAW DETECTED!\n%c%c%c\n",7,7,7,7,7,7);
 end_cond=1;
 halt_btx();
 stats();
}

void check_full()
{
 int i;
 int cnt;
 cnt=0;
 for (i=0;i<64;i++) if (board[i]) cnt++;
 printf("Board Full: %f%%\n", ((float)cnt*100.0)/64.0);
 if (cnt==64) draw();
}


void DrawScene(int isBlack)	/* Rysuje cala scene */
{
 int i,l1x,l2x,l1y,l2y;
 float dx,dy,dz;
 int nelem,j,tmps;
 int* eltable;
 debug("DrawScene(%d)\n", isBlack);
 if (isBlack) { XClearWindow(dsp,win); return; }
 perspective = (float)cx/4.0;
 check_full();
 draw_2D_objects();
 copy_from_buffers();
 world_transforms();
 XSetForeground(dsp,gc,WHITE);
 for (i=0;i<NLINES;i++)
   {
    l1x = hx + (int)(X_PERSP*board_lines[6*i  ]/(16.0+board_lines[6*i+2]));
    l2x = hx + (int)(X_PERSP*board_lines[6*i+3]/(16.0+board_lines[6*i+5]));
    l1y = hy - (int)(X_PERSP*board_lines[6*i+1]/(16.0+board_lines[6*i+2]));
    l2y = hy - (int)(X_PERSP*board_lines[6*i+4]/(16.0+board_lines[6*i+5]));
    XDrawLine(dsp,win,gc,l1x,l1y,l2x,l2y);
   }
 /* now draw elements */
 nelem=0;
 for (i=0;i<64;i++) if (board[i]) nelem++;
 if (nelem)
  {
   eltable = (int*)malloc(nelem*sizeof(int));
   j=0;
   for (i=0;i<64;i++) if (board[i])
     {
      eltable[j] = i;
      j++ ;
     }
   for (j=0;j<nelem;j++)
     {
      tmps = selected;
      selected = eltable[j];
      copy_from_buffers();
      for (i=0;i<NFLOATS;i++) board_lines[i] /= 10;
      get_3D_selected_pos(&dx,&dy,&dz);
      XSetForeground(dsp,gc, (board[selected]==1)?RED:BLUE);
      for (i=0;i<NPOINTS;i++)
         {
          board_lines[3*i  ] += dx;
          board_lines[3*i+1] += dy;
          board_lines[3*i+2] += dz;
         }
     world_transforms();
     for (i=0;i<NLINES;i++)
        {
         l1x = hx + (int)(X_PERSP*board_lines[6*i  ]/(16.0+board_lines[6*i+2]));
         l2x = hx + (int)(X_PERSP*board_lines[6*i+3]/(16.0+board_lines[6*i+5]));
         l1y = hy - (int)(X_PERSP*board_lines[6*i+1]/(16.0+board_lines[6*i+2]));
         l2y = hy - (int)(X_PERSP*board_lines[6*i+4]/(16.0+board_lines[6*i+5]));
         XDrawLine(dsp,win,gc,l1x,l1y,l2x,l2y);
        }
      selected = tmps;
     }
   free(eltable);
  }
 /* now draw selected */
 copy_from_buffers();
 for (i=0;i<NFLOATS;i++) board_lines[i] /= 4;
 get_3D_selected_pos(&dx, &dy, &dz);
 for (i=0;i<NPOINTS;i++)
   {
    board_lines[3*i  ] += dx;
    board_lines[3*i+1] += dy;
    board_lines[3*i+2] += dz;
   }
 world_transforms();
 XSetForeground(dsp,gc,GREEN);
 for (i=0;i<NLINES;i++)
   {
    l1x = hx + (int)(X_PERSP*board_lines[6*i  ]/(16.0+board_lines[6*i+2]));
    l2x = hx + (int)(X_PERSP*board_lines[6*i+3]/(16.0+board_lines[6*i+5]));
    l1y = hy - (int)(X_PERSP*board_lines[6*i+1]/(16.0+board_lines[6*i+2]));
    l2y = hy - (int)(X_PERSP*board_lines[6*i+4]/(16.0+board_lines[6*i+5]));
    XDrawLine(dsp,win,gc,l1x,l1y,l2x,l2y);
   }
}


void help()
{
 printf("Program by Morgoth DBMA - Lukasz Gryglicki MiNI M1: morgothdbma@o2.pl; 693582014\n");
 printf(
	"W/S           ROTATE_X\n"
	"R/F           ROTATE_Z\n"
	"E/D           ROTATE_Y\n"
	"T/G           MOVE +/- Z\n"
	"SPACE         BACK TO NORMAL POSITIONS\n"
	"A             BACK TO ZERO POSITIONS\n"
	"Z             RESET GAME\n"
	"H             HELP\n"
	"J/L           MOVE SELECTED +/-X\n"
	"K/I           MOVE SELECTED +/-Y\n"
	"M/O           MOVE SELECTED +/-Z\n"
	"Q             EXIT PROGRAM\n");
}


int main(int lb, char** par)		/* tutaj UNIX przekazuje sterowanie do Xengine */
{
  int s_num;
  int dx,dy;
  int font_h;
  XFontStruct* font_info;
  char* font_name = "*-helvetica-*-12-*";
  XEvent an_event;
  help();
  /*_malloc_options = "A";*/
  dsp = XOpenDisplay(NULL);
  if (!dsp) {printf("X-server error %s\n", par[0]); return 1;}
  s_num = DefaultScreen(dsp);
  dx = DisplayWidth(dsp, s_num);
  dy = DisplayHeight(dsp, s_num);
  cx = 400;
  cy = 400;
  hx = 200;
  hy = 200;
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
  init();
  copy_buffers();
  Randomize();
  if (Random(20)>=10) cpu_msi_move();
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
	case KeyPress:			/* sa tylko scan-kody klawiszy, nie pamietam co ktory znaczy :-) */
	     if (an_event.xkey.keycode == 0x18) done=1;		/* Q */
	     if (an_event.xkey.keycode == 25  )                 /* W */
		{ DrawScene(1); angle_x+=3; DrawScene(0); }
	     if (an_event.xkey.keycode == 39 )                  /* S */
                { DrawScene(1);  angle_x-=3; DrawScene(0);}
	     if (an_event.xkey.keycode == 27  )                 /* R */
		{ DrawScene(1); angle_z+=3; DrawScene(0); }
	     if (an_event.xkey.keycode == 41 )                  /* F */
                { DrawScene(1);  angle_z-=3; DrawScene(0);}
	     if (an_event.xkey.keycode == 26  )                 /* E */
		{ DrawScene(1); angle_y+=3; DrawScene(0); }
	     if (an_event.xkey.keycode == 40 )                  /* D */
                { DrawScene(1);  angle_y-=3; DrawScene(0);}
	     if (an_event.xkey.keycode == 45 )                  /* K */
                { DrawScene(1);  move_y(-1); DrawScene(0);}
	     if (an_event.xkey.keycode == 31 )                  /* I */
                { DrawScene(1);  move_y(1); DrawScene(0);}
	     if (an_event.xkey.keycode == 44 )                  /* J */
                { DrawScene(1);  move_x(-1); DrawScene(0);}
	     if (an_event.xkey.keycode == 46 )                  /* L */
                { DrawScene(1);  move_x(1); DrawScene(0);}
	     if (an_event.xkey.keycode == 58 )                  /* M */
                { DrawScene(1);  move_z(-1); DrawScene(0);}
	     if (an_event.xkey.keycode == 32 )                  /* O */
                { DrawScene(1);  move_z(1); DrawScene(0);}
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
                { DrawScene(1);  delta_z+=0.2; DrawScene(0);}
	     if (an_event.xkey.keycode == 42 )                 /* G */
                { DrawScene(1);  delta_z-=0.2; DrawScene(0);}
	     if (an_event.xkey.keycode == 54 )                 /* C */
                { DrawScene(1);  delta_x+=0.2; DrawScene(0);}
	     if (an_event.xkey.keycode == 55 )                 /* V */
                { DrawScene(1);  delta_x-=0.2; DrawScene(0);}
	     if (an_event.xkey.keycode == 56 )                 /* B */
                { DrawScene(1);  delta_y+=0.2; DrawScene(0);}
	     if (an_event.xkey.keycode == 57 )                 /* N */
                {
		 if (halt) 
		   {
		    clear_board();
  		    if (Random(20)>=8) cpu_msi_move();
		    printf("\n%c%c%c\n", 7,7,7);
		    end_cond=0;
		    halt=0;
		    DrawScene(1);
		    DrawScene(0);
		   }
		 else { DrawScene(1);  delta_y-=0.2; DrawScene(0); }
		}
	     if (an_event.xkey.keycode == 43 ) help();         /* H */
	     if (an_event.xkey.keycode == 36 )                 /* ENTER */
                {
	         DrawScene(1);
		 if (!end_cond && !board[selected])
		   {
		    board[selected] = 1;
		    cpu_msi_move();
		   }
		 DrawScene(0);
		}
	       /*printf("%d,%d,%d,%f,%f,%f,%d\n", angle_x,angle_y,angle_z,delta_z,delta_y,delta_x,an_event.xkey.keycode); */
             break;
        }
   }
 printf("\n%c%c%cMSI CPU ShutDown\n\n%c%c%c\n", 7,7,7,7,7,7);
 return 0;
}

/* CopyLeft Morgoth DBMA, +48693582014, morgothdbma@o2.pl, heroine@o2.pl */
