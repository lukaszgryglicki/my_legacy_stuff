#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

/*#define DEBUG*/
/*#define DEBUGALG*/
#define INFO

typedef unsigned long ulong;

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

int info(char* fmt, ...)  /* print out INFO information if INFO defined */
{
#ifdef INFO
 va_list ap;
 int err;
 va_start(ap,fmt);
 err = vprintf(fmt,ap);
 va_end(ap);
 return err;
#endif
#ifndef INFO
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
	time_t tm;
	time(&tm);
	srand((unsigned int)tm);
	return 1;
}

int Random(int lb)   /* randomize number from 0 to lb-1 */
{
 return rand() % lb;
}


static int selected = 0;		/* AKTUALNIE ZAZNACZONE POLE */
static int board[0x100];		/* board 0-empty, 1-human, 2-CPU */
static int done = 0;			/*quit when done*/
static int end_cond = 0;		/* CPU win or lost currently */
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



void init() 
{
 int i;
 info("Clearing board\n");
 for (i=0;i<0x100;i++) board[i] = 0;
}


void move_x(int arg)			/* move selected +/- X */
{
 int tst;
 int tst2;
 debug("move_x:%d\n", arg);
 tst = (selected%4);			/* x is last bits 0..3 */
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


void move_y(int arg)			/* move selected +/- Y */
{
 int tst;
 int tst2;
 debug("move_y:%d\n", arg);
 tst = ((selected/4)%4);		/* y is 4*sel: 0,4,8,12 */
 if (arg>0)
   {
    selected+=4;
    tst2 = ((selected/4)%4);
    if (tst2<tst) selected -=0x10;
   }
 else
   {
    selected-=4;
    if (selected<0) { selected +=0x10; return; }
    tst2 = ((selected/4)%4);
    if (tst2>tst) selected +=0x10;
   }
}


void move_z(int arg)			/* move selected +/- Z */
{
 int tst;
 int tst2;
 debug("move_z:%d\n", arg);
 tst = ((selected/0x10)%4);		/* z is 4*sel: 0,16,32,48 */
 if (arg>0)
   {
    selected+=0x10;
    tst2 = ((selected/0x10)%4);
    if (tst2<tst) selected -=0x40;
   }
 else
   {
    selected-=0x10;
    if (selected<0) { selected +=0x40; return; }
    tst2 = ((selected/0x10)%4);
    if (tst2>tst) selected +=0x40;
   }
}

void move_v(int arg)		/* move selected +/- V */
{
 int tst;
 debug("move_v:%d\n", arg);
 tst = selected/0x40;		/* v is 64*sel: 0,64,128,192 */
 if (arg>0)
   {
    selected+=0x40;
    if (selected>=0x100) selected-=0x100;
   }
 else
   {
    selected-=0x40;
    if (selected<0) selected+=0x100;
   }
}

void set_at(int x, int y, int z, int v, int value)
{
 board[x+(y<<2)+(z<<4)+(v<<6)] = value;
}


int get_at(int x, int y, int z, int v)
{
 return board[x+(y<<2)+(z<<4)+(v<<6)];
}


/* The brain of the game */
void heuristic_count_moves_to_win(int player, int* rmin, int* rways)
{
 int xc,yc,zc,vc,i;
 int vec[4];
 int cmin;
 int min;
 int ways;
 debug("MOVES_TO_WIN FOR PLAYER: %d\n", player);
 min = 5;
 ways = 0;
 /*
		1D lines through all posible directions (there are 4: X,Y,Z,V)
		fixing one param x,y,z or v and change all 3 remianing from 0..3
		this gives:
		loops: 64 (p1,p2,p3) -> (0,1,2,3)
		wariants: 4 (all principle axes: X,Y,Z,V)
		cases in loop: 1, (0,1,2,3) because the inverse is the same line
		finally #1D: 64 x 4 x 1 = 256 lines
 */

 /* GO BY PARALLEL LINES */
 /* BY V */
 for (xc=0;xc<4;xc++)
 for (yc=0;yc<4;yc++)
 for (zc=0;zc<4;zc++)
   {
    for (vc=0;vc<4;vc++) vec[vc] = get_at(xc,yc,zc,vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways ++;
    if (cmin<min) min=cmin;
   }
 /* BY Z */
 for (xc=0;xc<4;xc++)
 for (yc=0;yc<4;yc++)
 for (vc=0;vc<4;vc++)
   {
    for (zc=0;zc<4;zc++) vec[zc] = get_at(xc,yc,zc,vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* BY Y */
 for (xc=0;xc<4;xc++)
 for (zc=0;zc<4;zc++)
 for (vc=0;vc<4;vc++)
   {
    for (yc=0;yc<4;yc++) vec[yc] = get_at(xc,yc,zc,vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* BY X */
 for (yc=0;yc<4;yc++)
 for (zc=0;zc<4;zc++)
 for (vc=0;vc<4;vc++)
   {
    for (xc=0;xc<4;xc++) vec[xc] = get_at(xc,yc,zc,vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* PRZEKATNE 2D */
/*
		2D lines through all posible faces, all face fixes 2 axes, so remain 2 axes in 4D
		giving any posible 2-axes combination: XY, XZ, XV, YZ, YV, ZV 9just as rotations in 4D)
		fixing two param xy,xz,xv,yz,yv or zv and change all 2 remianing from 0..3
		this gives:
		loops: 16 (p1,p2) -> (0,1,2,3)
		wariants: 6 (all principle planes: XY,XZ,XV,YZ,YV,ZV)
		cases in loop: 2, because face (square) has 2 unique dissections (0123)(0123) and (0123)(3210)
		dissections in square: (2^2dimensions) - combinations / 2 (2ends) = 4/2 = 2
		finally #2D: 16 x 6 x 2 = 192 lines
 */
 /* WGLAB ZV */
 for (zc=0;zc<4;zc++)
 for (vc=0;vc<4;vc++)
   {
    vec[0] = get_at(0,3, zc, vc);
    vec[1] = get_at(1,2, zc, vc);
    vec[2] = get_at(2,1, zc, vc);
    vec[3] = get_at(3,0, zc, vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
		{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;

    vec[0] = get_at(0,0, zc, vc);
    vec[1] = get_at(1,1, zc, vc);
    vec[2] = get_at(2,2, zc, vc);
    vec[3] = get_at(3,3, zc, vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* WGLAB YV */
 for (yc=0;yc<4;yc++)
 for (vc=0;vc<4;vc++)
   {
    vec[0] = get_at(0,yc,3,vc);
    vec[1] = get_at(1,yc,2,vc);
    vec[2] = get_at(2,yc,1,vc);
    vec[3] = get_at(3,yc,0,vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;

    vec[0] = get_at(0,yc,0,vc);
    vec[1] = get_at(1,yc,1,vc);
    vec[2] = get_at(2,yc,2,vc);
    vec[3] = get_at(3,yc,3,vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
		{
		    cmin=6;
			ways--;
		    break;
		}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* WGLAB XV */
 for (xc=0;xc<4;xc++)
 for (vc=0;vc<4;vc++)
   {
    vec[0] = get_at(xc,3,0,vc);
    vec[1] = get_at(xc,2,1,vc);
    vec[2] = get_at(xc,1,2,vc);
    vec[3] = get_at(xc,0,3,vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;

    vec[0] = get_at(xc,0,0,vc);
    vec[1] = get_at(xc,1,1,vc);
    vec[2] = get_at(xc,2,2,vc);
    vec[3] = get_at(xc,3,3,vc);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* WGLAB YZ */
 for (yc=0;yc<4;yc++)
 for (zc=0;zc<4;zc++)
   {
    vec[0] = get_at(3,yc,zc,0);
    vec[1] = get_at(2,yc,zc,1);
    vec[2] = get_at(1,yc,zc,2);
    vec[3] = get_at(0,yc,zc,3);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;

    vec[0] = get_at(3,yc,zc,3);
    vec[1] = get_at(2,yc,zc,2);
    vec[2] = get_at(1,yc,zc,1);
    vec[3] = get_at(0,yc,zc,0);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* WGLAB XZ */
 for (xc=0;xc<4;xc++)
 for (zc=0;zc<4;zc++)
   {
    vec[0] = get_at(xc,3,zc,0);
    vec[1] = get_at(xc,2,zc,1);
    vec[2] = get_at(xc,1,zc,2);
    vec[3] = get_at(xc,0,zc,3);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;

    vec[0] = get_at(xc,3,zc,3);
    vec[1] = get_at(xc,2,zc,2);
    vec[2] = get_at(xc,1,zc,1);
    vec[3] = get_at(xc,0,zc,0);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* WGLAB XY */
 for (xc=0;xc<4;xc++)
 for (yc=0;yc<4;yc++)
   {
    vec[0] = get_at(xc,yc,3,0);
    vec[1] = get_at(xc,yc,2,1);
    vec[2] = get_at(xc,yc,1,2);
    vec[3] = get_at(xc,yc,0,3);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;

    vec[0] = get_at(xc,yc,3,3);
    vec[1] = get_at(xc,yc,2,2);
    vec[2] = get_at(xc,yc,1,1);
    vec[3] = get_at(xc,yc,0,0);
    cmin=4;
    for (i=0;i<4;i++)
       {
        if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
		if (vec[i]==player) cmin--;
       }
    ways++;
    if (cmin<min) min=cmin;
   }
 /* PRZEKATNE 3D */
/*
		3D lines through all posible cells, all cells fixes 3 axes, so remain 1 axis in 4D
		so fixed can be: XYZ, XYV, XZV, YZV
		fixing three param xyz,xyv,xzv,yzv or zv and change all one remianing from 0..3
		this gives:
		loops: 4 (p1) -> (0,1,2,3) [each param can be from this set]
		wariants: 4 (fixing just x,y,z or v) - choosing one of 4 3D sub-spaces, each has 4 loops and 4 dissections within
		cases in loop: 4, because cube has 4 unique dissections 2^3 = 8 / 2  = 4 (because each dissection has 2 ends)
		example dissection (0123)(0123)(3210) is identical to (3210)(3210)(0123) so 2^3/2 = 4
		finally #2D: 4 x 4 x 4 = 64
 */
 /* WOKOL X */
 for (xc=0;xc<4;xc++)
 {
	vec[0] = get_at(xc,0,0,0);
	vec[1] = get_at(xc,1,1,1);
	vec[2] = get_at(xc,2,2,2);
	vec[3] = get_at(xc,3,3,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(xc,3,0,0);
	vec[1] = get_at(xc,2,1,1);
	vec[2] = get_at(xc,1,2,2);
	vec[3] = get_at(xc,0,3,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
		if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(xc,0,3,0);
	vec[1] = get_at(xc,1,2,1);
	vec[2] = get_at(xc,2,1,2);
	vec[3] = get_at(xc,3,0,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(xc,0,0,3);
	vec[1] = get_at(xc,1,1,2);
	vec[2] = get_at(xc,2,2,1);
	vec[3] = get_at(xc,3,3,0);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
 }
 /* WOKOL Y */
 for (yc=0;yc<4;yc++)
 {
	vec[0] = get_at(0,yc,0,0);
	vec[1] = get_at(1,yc,1,1);
	vec[2] = get_at(2,yc,2,2);
	vec[3] = get_at(3,yc,3,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(3,yc,0,0);
	vec[1] = get_at(2,yc,1,1);
	vec[2] = get_at(1,yc,2,2);
	vec[3] = get_at(0,yc,3,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(0,yc,3,0);
	vec[1] = get_at(1,yc,2,1);
	vec[2] = get_at(2,yc,1,2);
	vec[3] = get_at(3,yc,0,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(0,yc,0,3);
	vec[1] = get_at(1,yc,1,2);
	vec[2] = get_at(2,yc,2,1);
	vec[3] = get_at(3,yc,3,0);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
 }
 /* WOKOL Z */
 for (zc=0;zc<4;zc++)
 {
	vec[0] = get_at(0,0,zc,0);
	vec[1] = get_at(1,1,zc,1);
	vec[2] = get_at(2,2,zc,2);
	vec[3] = get_at(3,3,zc,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
				{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(3,0,zc,0);
	vec[1] = get_at(2,1,zc,1);
	vec[2] = get_at(1,2,zc,2);
	vec[3] = get_at(0,3,zc,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(0,3,zc,0);
	vec[1] = get_at(1,2,zc,1);
	vec[2] = get_at(2,1,zc,2);
	vec[3] = get_at(3,0,zc,3);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(0,0,zc,3);
	vec[1] = get_at(1,1,zc,2);
	vec[2] = get_at(2,2,zc,1);
	vec[3] = get_at(3,3,zc,0);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
				{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
 }
 /* WOKOL V */
for (vc=0;vc<4;vc++)
 {
	vec[0] = get_at(0,0,0,vc);
	vec[1] = get_at(1,1,1,vc);
	vec[2] = get_at(2,2,2,vc);
	vec[3] = get_at(3,3,3,vc);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(3,0,0,vc);
	vec[1] = get_at(2,1,1,vc);
	vec[2] = get_at(1,2,2,vc);
	vec[3] = get_at(0,3,3,vc);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(0,3,0,vc);
	vec[1] = get_at(1,2,1,vc);
	vec[2] = get_at(2,1,2,vc);
	vec[3] = get_at(3,0,3,vc);
	cmin=4;
	for (i=0;i<4;i++)
			{
			if (vec[i] && vec[i]!=player)
				{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
	vec[0] = get_at(0,0,3,vc);
	vec[1] = get_at(1,1,2,vc);
	vec[2] = get_at(2,2,1,vc);
	vec[3] = get_at(3,3,0,vc);
	cmin=4;
	for (i=0;i<4;i++)
		{
			if (vec[i] && vec[i]!=player)
			{
				cmin=6;
				ways--;
				break;
			}
			if (vec[i]==player) cmin--;
		}
	ways++;
	if (cmin<min) min=cmin;
 }
 /* KONIEC PRZEKATNYCH 3D */
/*
		4D lines through hypercube, no loops, just one hypercube 4x4x4x4
		no wariants, just one hypercube 
		cells in loop 2(2ends)^4(#combinations in 4D cube)/2(num ends) (2^4)/2 = 8 dissections
		there are 8 other which are equal, ex: (3210)(0123)(0123)(3210) = (0123)(3210)(3210)(0123)
		loops: 1 (just 4D cube)
		wariants: 1 (just 4D cube)
		cases in loop: 8 (number of hyper dissections of 4D-cube)
		finally #2D: 1 x 1 x 8 = 8
 */
 /* HIPERPRZEKATNE 4D  - 8 cases and their equivalent 8 mirros gives all posible 16 2^4*/
 vec[0] = get_at(0,0,0,0);
 vec[1] = get_at(1,1,1,1);
 vec[2] = get_at(2,2,2,2);
 vec[3] = get_at(3,3,3,3);
 cmin=4;
 for (i=0;i<4;i++)
	{
		if (vec[i] && vec[i]!=player)
			{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
	}
 ways++;
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,0,0,3);
 vec[1] = get_at(1,1,1,2);
 vec[2] = get_at(2,2,2,1);
 vec[3] = get_at(3,3,3,0);
 cmin=4;
 for (i=0;i<4;i++)
	{
		if (vec[i] && vec[i]!=player)
			{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
	}
 ways++;
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,0,3,0);
 vec[1] = get_at(1,1,2,1);
 vec[2] = get_at(2,2,1,2);
 vec[3] = get_at(3,3,0,3);
 cmin=4;
 for (i=0;i<4;i++)
	{
		if (vec[i] && vec[i]!=player)
			{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
	}
 ways++;
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,3,0,0);
 vec[1] = get_at(1,2,1,1);
 vec[2] = get_at(2,1,2,2);
 vec[3] = get_at(3,0,3,3);
 cmin=4;
 for (i=0;i<4;i++)
	{
		if (vec[i] && vec[i]!=player)
			{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
	}
 ways++;
 if (cmin<min) min=cmin;
 vec[0] = get_at(3,0,0,0);
 vec[1] = get_at(2,1,1,1);
 vec[2] = get_at(1,2,2,2);
 vec[3] = get_at(0,3,3,3);
 cmin=4;
 for (i=0;i<4;i++)
	{
		if (vec[i] && vec[i]!=player)
			{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
	}
 ways++;
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,0,3,3);
 vec[1] = get_at(1,1,2,2);
 vec[2] = get_at(2,2,1,1);
 vec[3] = get_at(3,3,0,0);
 cmin=4;
 for (i=0;i<4;i++)
	{
		if (vec[i] && vec[i]!=player)
			{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
	}
 ways++;
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,3,0,3);
 vec[1] = get_at(1,2,1,2);
 vec[2] = get_at(2,1,2,1);
 vec[3] = get_at(3,0,3,0);
 cmin=4;
 for (i=0;i<4;i++)
	{
		if (vec[i] && vec[i]!=player)
			{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
	}
 ways++;
 if (cmin<min) min=cmin;
 vec[0] = get_at(0,3,3,0);
 vec[1] = get_at(1,2,2,1);
 vec[2] = get_at(2,1,1,2);
 vec[3] = get_at(3,0,0,3);
 cmin=4;
 for (i=0;i<4;i++)
	{
		if (vec[i] && vec[i]!=player)
			{
			cmin=6;
			ways--;
			break;
		}
		if (vec[i]==player) cmin--;
	}
 ways--;
 if (cmin<min) min=cmin;
 /*
 All possible ways to win:
 dim:loops:variants:#inside loop:all
 1D:	64	4	1	256
 2D:	16	6	2	192
 3D:	4	4	4	64
 4D:	1	1	8	8
 sum:	85	15	15	520 ways to win


 */
 *rways = ways;
 *rmin = min;
 debug("MIN = %d\n", min); 
 /*info("\t[%d,%d,%d] ", player, min, ways);*/
}


void clear_board()
{
 int i;
 debug("clear_board()\n");
 info("Clearing board\n");
 for (i=0;i<0x100;i++) board[i] = 0;
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


/* test entire hypercube for possible best move */
int forseen_best_move(int h1, int h2, int h3, int h4, int cpu, int opp)
{
 int i,j,c,draww;
 int *hx1,*hx2,*hx3,*hx4, *at,*val,*vam;
 int *gmove;
 int nmoves,min,hmov,cmov;

 info("\n");
 nmoves=0;
 Debug("foreseen_best_move()\n");
 for (i=0;i<0x100;i++) if (!board[i]) nmoves++;
 hx1 = (int*)malloc(nmoves<<2);
 hx2 = (int*)malloc(nmoves<<2);
 hx3 = (int*)malloc(nmoves<<2);
 hx4 = (int*)malloc(nmoves<<2);
 at  = (int*)malloc(nmoves<<2);
 val = (int*)malloc(nmoves<<2);
 vam = (int*)malloc(nmoves<<2);
 j=0;
 info("possible moves: %d\n", nmoves);
 for (i=0;i<0x100;i++)
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
 info("\n");
 draww = check_draw(hx3, hx4, nmoves);
 if (draww)
 {
  info("draw\n");
  draw();
  return 0;
 }
 c=0;
 for (i=0;i<0x100;i++) if (board[i]==1) c++;

 Debug("h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
 for (i=0;i<nmoves;i++)
   Debug("hx1=%d,hx2=%d,hx3=%d,hx4=%d,at=%d,val=%d,i=%d\n",hx1[i],hx2[i],hx3[i],hx4[i],at[i],val[i],i);
 Debug("matrix:\n");
 for (i=0;i<nmoves;i++) if (hx1[i]<=0) /* gdy komputer moze wygrac */
   {
    board[at[i]] = cpu;
    printf("HEURISTICS BEFORE: h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
    printf("WIN HEURISTICS: hx1=%d,hx2=%d,hx3=%d,hx4=%d,at=%d,val=%d,i=%d\n",hx1[i],hx2[i],hx3[i],hx4[i],at[i],val[i],i);
    printf("ONBOARD:%d>> (%d,%d,%d,%d)\n",i,i%4,(i/4)%4,(i/16)%4, i/64);
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
    info("defending\n");
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
    info("one possible move - to be close victory\n");
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
    info("one possible move - to get as many as possible ways to victory\n");
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
 cmov=1000;							/* MOZLIWOŒCI w 4d(4) = 256 */
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
    info("one possible move to minimalize enemy's ways to victory\n");
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
  info("detected very bad situation - defending attempt\n");
  Debug("SYTUACJA JEST BARDZO ZLA!!!\n");
  hmov=5;							/* FIXME ?? */
  for (i=0;i<nmoves;i++) if (hx1[i]<hmov)  hmov = hx1[i];
  for (i=0;i<nmoves;i++) if (hx1[i]==hmov) { board[at[i]] = cpu; break; }
  /* board[at[Random(nmoves)]] = cpu; */
 }
 else
   {
    info("possible %d good moves, randomizing\n", c);
    gmove = (int*)malloc(c<<2);
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
 heuristic_count_moves_to_win(cpu, h1, h3);				/*  CPU  */
 heuristic_count_moves_to_win(opp, h2, h4);				/* HUMAN */
 debug("H1=%d, H2=%d, H3=%d, H4=%d\n", *h1, *h2, *h3, *h4);
 if ((*h2)<=0) return -1;
 else return 0;
}

void halt_btx()
{
 halt = 1;
}


void cpu_msi_move(int player)
{
 int h1,h2,h3,h4,ret;
 debug("CPU_MSI_MOVE()\n");
 info("moving player: %d\n", player);
 end_cond=0;
 if (player) ret = get_current_heuristics(&h1, &h2, &h3, &h4, 2, 1);
 else ret = get_current_heuristics(&h1, &h2, &h3, &h4, 1, 2);
 if (ret==-1)
   {
    printf("HEURISTICS WHEN DEFETED: h1=%d,h2=%d,h3=%d,h4=%d\n",h1,h2,h3,h4);
    printf("PANIC: MSI COMPUTED RESULT, CPU LOST\n");
    printf("\nCPU: I HAVE LOST!\n\n");
    end_cond=1;
    return ;
   }
 if (player) ret = forseen_best_move(h1,h2,h3,h4, 2, 1);
 else ret = forseen_best_move(h1,h2,h3,h4, 1, 2);
 if (ret)
   {
    printf("\nCPU: I HAVE WIN!\n");
    stats();
    end_cond=1;
   }
 info("\n");
}


void draw()
{
 printf("\nCPU: DRAW DETECTED!\n");
 end_cond=1;
 halt_btx();
 stats();
}

void check_full()
{
 int i;
 int cnt;
 cnt=0;
 for (i=0;i<0x100;i++) if (board[i]) cnt++;
 printf("Board Full: %f%%\n", ((float)cnt*100.0)/256.0);
 if (cnt==0x100) draw();
}

void help()
{
	printf("keys\n");
	printf("\tq:\tquit\n");
	printf("\th:\tthis help\n");
	printf("\tz:\tmove\n");
	printf("\tx:\tcpu move\n");
	printf("\tjl:\tmove x\n");
	printf("\tki:\tmove y\n");
	printf("\tmo:\tmove z\n");
	printf("\tun:\tmove v\n");
}

void draw_scene()
{
 int i;
 printf("\n");
 printf("-----+----+----+-----\n|");
 for (i=0;i<0x100;i++)
   {
    if ((i%16) && !(i%4)) printf("|");
    if (i==selected && board[i]==1) printf("X");
    if (i!=selected && board[i]==1) printf("x");
    if (i==selected && board[i]==2) printf("O");
    if (i!=selected && board[i]==2) printf("o");
    if (i==selected && board[i]==0) printf("+");
    if (i!=selected && board[i]==0) printf(" ");
    if ((i%16)==15) printf("|\n|");
	if ((i%64)==63) printf("----+----+----+----|\n|");
   }
 printf("\n");
}

int main(int lb, char** par)		/* tutaj UNIX przekazuje sterowanie do Xengine */
{
  unsigned char zn;
  char str[1024];
  init();
  Randomize();
  if (Random(20)>=10) cpu_msi_move(1);

  while(!done)
  {
     draw_scene();
     printf("command>> ");
     scanf("%s", str);
	 zn = (unsigned char)str[0];
     printf("\n");
     if (zn>='A' && zn <= 'Z') zn += 0x20;
     if (zn=='q') done=1;
     if (zn=='k') move_z(1);
     if (zn=='i') move_z(-1);
     if (zn=='j') move_x(-1);
     if (zn=='l') move_x(1);
     if (zn=='m') move_y(-1);
     if (zn=='o') move_y(1);
	 if (zn=='u') move_v(-1);
     if (zn=='n') move_v(1);
     if (zn=='h') help();
     if (zn=='z')
     {
		 draw_scene();
		 if (!board[selected])
		   {
		    board[selected] = 1;
		    cpu_msi_move(1);
		   }
		 draw_scene();
		 if (end_cond)
		   {
		    clear_board();
  		    if (Random(20)>=11) cpu_msi_move(1);
		    end_cond=0;
		   }
	  }
     if (zn=='x')
     {
		 cpu_msi_move(0);
		 cpu_msi_move(1);
		 if (end_cond)
		   {
		    clear_board();
  		    if (Random(20)>=11) cpu_msi_move(1);
		    end_cond=0;
		   }
	  }
  }
 return 0;
}

/* CopyLeft Morgoth DBMA, +48693582014, morgothdbma@o2.pl, heroine@o2.pl */
