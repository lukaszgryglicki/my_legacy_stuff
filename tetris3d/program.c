/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <GL/glut.h>
#define PI  3.1415926F
#define PI2 6.2831853F
#define GLUT_KEY_DELETE 127
/*#define DEBUG*/

/*
 *	0 -brak obiektu
 *	1 -srodek obrotu
 *	2 -obiekt
 *	3 -osadzony obiekt
 *
 * */

typedef struct _area
{
 int*** pos;
 int** mem;
 int x,y,z;
 int rx,ry,rz;
} area;

typedef struct _BMPTag		
{
 char ident[2];
 int fsize;
 int dummy;
 int offset;
 int dummy2;
 int bm_x;
 int bm_y;
 short planes;
 short bpp;
 int compress;
 int nbytes;
 int no_matter[4];
} BMPTag;

unsigned char* bits;
int bmx, bmy;
int wx,wy;
int cx,cy;
int auto_move;
int nmoves;
int glines;
GLfloat angX, angY, angZ;
GLfloat scalef = 1.;
GLfloat scx, scy, scz;
int moving;
static int end = 0;
float sp;
int cspeed;
int dspeed;
int gpoints;
int burst;
int Ax,Ay,Az,Bl;
int paused;
int tenabled;
int dirx, diry, dirz;
int dtype;
int bltype;
float slowdown;
unsigned int tid;
area ar;
pthread_t thr;
pthread_mutex_t mut;
char* texname;
static int fps = 0;
static time_t t1 = 0, t2 = 0;
float *pxp, *pxm;
float *pyp, *pym;
float *pzp, *pzm;
float** ptbl;
int pcnt;

void help()
{
 printf("\nTetris3D by morgothdbma; morgothdbma@o2.pl +48693582014\n");
 printf("Compiled: %s %s\n\n", __DATE__, __TIME__);
 printf("keys: arrows (move)\n");
 printf("pgup/pgdown/home/end/ins/del (rotate block)\n");
 printf("space (go down)\n");
 printf("ikjlom (rotate scene)\n");
 printf("12 (toggle speed)\n");
 printf("n (back to normals)\n");
 printf("-= (scale)\n");
 printf("p (pause)\n");
 printf("r (fullscreen)\n");
 printf("q ESC (quit)\n");
 printf("z (burst down)\n");
 printf("cvb (fast rotations)\n");
 printf("567890 (x,y,z scaling)\n");
 printf("4 (automove)\n");
 printf("sd (toggle dtype)\n");
 printf("a (toggle bltype)\n");
 printf("fg (resize window)\n");
 printf("we (maniputate dspeed)\n");
 printf("bltype is: 0 (3D), 1 (variation), 2 (2D), 3 (manually)\n");
 printf("cmdline: ax ay az bl speed bltype texname rndfile\n");
 printf("if bltype is 3 then rndfile is used\n");
 printf("example rndfile is: default.rnd\n");
 printf("other bltype causes 1D randomize\n");
 printf("texname: points to BMP image, dimnesions are powers of 2\n");
}

void time_counter()
{
 char tstr[128];
 if (t1 == (time_t)0)
   {
    time(&t1);
    time(&t2);
    return;
   }
 fps++;
 time(&t2);
 if (t2 > t1)
   {
    sprintf(tstr, "Points: %d, FPS: %d, speed: %f, moves: %d, lines: %d", 
	    gpoints, fps/(int)(t2-t1), 5000000. / (float)cspeed, nmoves, glines);
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
}


void init_bmp(BMPTag* b)
{
 int i;
 if (!b) return;
 b->ident[0]='B';
 b->ident[1]='M';
 b->fsize=0;
 b->dummy=0;
 b->offset=sizeof(BMPTag);
 b->bm_x=b->bm_y=0x20;
 b->dummy2=40;
 b->planes=0;
 b->bpp=0x18;
 b->planes=1;
 b->compress=0;
 b->nbytes=3*32*32;
 for (i=0;i<4;i++) b->no_matter[i]=0;
}

int load_bmp(char* fn)
{
 FILE* plik;
 int i;
 char b,m;
 BMPTag bm_handle;
 plik = fopen(fn, "rb");
 if (!plik) return 0;
 init_bmp(&bm_handle);
 i = fscanf(plik,"%c%c",&b,&m);
 if (i != 2) return 0;
 if (b != 'B' || m != 'M') return 0;
 fread(&bm_handle.fsize,4,1,plik);
 fread(&bm_handle.dummy,4,1,plik);
 fread(&bm_handle.offset,4,1,plik);
 fread(&bm_handle.dummy2,4,1,plik);
 fread(&bm_handle.bm_x,4,1,plik);
 fread(&bm_handle.bm_y,4,1,plik);
 fread(&bm_handle.planes,2,1,plik);
 fread(&bm_handle.bpp,2,1,plik);
 if (bm_handle.bpp != 24) return 0;
 fseek(plik,bm_handle.offset,SEEK_SET);
 bits = (unsigned char*)malloc(3*bm_handle.bm_y*bm_handle.bm_x*sizeof(unsigned char));
 bmx = bm_handle.bm_x;
 bmy = bm_handle.bm_y;
 fread(bits, 3*bmx*bmy, sizeof(unsigned char), plik);
 printf("Image read (%dx%d).\n", bmx, bmy);
 return 1;
}


void inCS()
{
 pthread_mutex_lock(&mut);
}

void outCS()
{
 pthread_mutex_unlock(&mut);
}

void init_area(area* a, int x, int y, int z)
{
 int i,j,k;
 if (!a || x <=0 || y <= 0 || z <=0) return;
 a->pos = (int***)malloc(x*sizeof(int**));
 for (i=0;i<x;i++) a->pos[i] = (int**)malloc(y*sizeof(int*));
 for (i=0;i<x;i++) for (j=0;j<y;j++) 
     a->pos[i][j] = (int*)malloc(z*sizeof(int));
 for (i=0;i<x;i++) for (j=0;j<y;j++) for (k=0;k<z;k++)
     a->pos[i][j][k] = 0;
 a->mem = (int**)malloc(Bl*sizeof(int*));
 for (i=0;i<Bl;i++) a->mem[i] = (int*)malloc(3*sizeof(int));
 a->x = x;
 a->y = y;
 a->z = z;
 a->rx = a->ry = a->rz = 0;
}

void make_direction(int* dx, int* dy, int* dz, int typ, int step)
{
 int r;
 int a,b,c;
 float dr;
 if (typ == 0)	/* standard 3D */
  {
   r = rand() % 6;
   switch (r)
    {
     case 0: *dx = 1;  *dy = 0;  *dz = 0;  break;
     case 1: *dx = -1; *dy = 0;  *dz = 0;  break;
     case 2: *dx = 0;  *dy = 1;  *dz = 0;  break;
     case 3: *dx = 0;  *dy = -1; *dz = 0;  break;
     case 4: *dx = 0;  *dy = 0;  *dz = 1;  break;
     case 5: *dx = 0;  *dy = 0;  *dz = -1; break;
    }
 }
 else if (typ == 1)	/* variation */
   {
    *dx = (rand() % 3) - 1;
    *dy = (rand() % 3) - 1;
    *dz = (rand() % 3) - 1;
   }
 else if (typ == 2)	/* planar 2D*/
   {
    r = rand() % 4;
    a = b = c = 0;
    switch (r)
      {
       case 0: b = 1;  c = 0;  break;
       case 1: b = -1; c = 0;  break;
       case 2: b = 0;  c = 1;  break;
       case 3: b = 0;  c = -1;  break;
      }
    if (Ax < Az) { *dx = a; *dy = b; *dz = c; }
    else         { *dz = a; *dy = b; *dx = c; }
   }
 else if (typ == 3)	/* weighted 3D */
  {
   dr = (float)(rand() % 1000000000) / 1000000000.;
   *dx = *dy = *dz = 0;
   if (step >= pcnt) step = pcnt;
   if (dr >= ptbl[step][0] && dr < ptbl[step][1])  *dx = 1;
   if (dr >= ptbl[step][1] && dr < ptbl[step][2])  *dx = -1;
   if (dr >= ptbl[step][2] && dr < ptbl[step][3])  *dy = 1;
   if (dr >= ptbl[step][3] && dr < ptbl[step][4])  *dy = -1;
   if (dr >= ptbl[step][4] && dr < ptbl[step][5])  *dz = 1;
   if (dr >= ptbl[step][5] && dr <= ptbl[step][6]) *dz = -1;
 }
 else	/* 1D */
   {
    r = rand() % 2;
    a = b = c = 0;
    switch (r)
      {
       case 0: c = 1;  break;
       case 1: c = -1;  break;
      }
    if (Ax < Az) { *dx = a; *dy = b; *dz = c; }
    else         { *dz = a; *dy = b; *dx = c; }
   }
}

int check_lines(area* a)
{
 int lined;
 int x,y,z;
 int c;
 char title[64];
 int xi,yi,zi;
 float p,m;
 xi = yi = zi = 0;
 lined = 0;
 for (x=0;x<a->x;x++)
 for (y=0;y<a->y;y++)
   {
    c = 0;
    for (z=0;z<a->z;z++) if (a->pos[x][y][z] == 3) c++;
    if (c == a->z && c > Bl) 
      {
       for (z=0;z<a->z;z++) a->pos[x][y][z] = 0;
       lined ++;
       xi ++;
      }
   }
 for (x=0;x<a->x;x++)
 for (z=0;z<a->z;z++)
   {
    c = 0;
    for (y=0;y<a->y;y++) if (a->pos[x][y][z] == 3) c++;
    if (c == a->y && c > Bl) 
      {
       for (y=0;y<a->y;y++) a->pos[x][y][z] = 0;
       lined ++;
       yi++;
      }
   }
 for (z=0;z<a->z;z++)
 for (y=0;y<a->y;y++)
   {
    c = 0;
    for (x=0;x<a->x;x++) if (a->pos[x][y][z] == 3) c++;
    if (c == a->x && c > Bl) 
      {
       for (x=0;x<a->x;x++) a->pos[x][y][z] = 0;
       lined ++;
       zi ++;
      }
   }
 
 if (lined)
   {
    p = 22050. * (float)Ax * (float)Az * pow((float)Bl/2., (float)lined) * Bl;
    m = ((float)cspeed / 5000.) * ((float)cspeed / 5000.) / 500.;
    p /= m * slowdown * sqrt(slowdown);
    p /= (float)Ay/2.;
    if (lined >= Bl) p *= 3;
    if ((xi && yi) || (yi && zi) || (zi && xi)) p *= 4;
    if (Ax > Bl && Az > Bl) p *= (float)Bl/2.;
    if (bltype == 0 || bltype == 3) p ++;
    else if (bltype == 1) p *= 5;
    else if (bltype == 2) p /= 5.;
    else p = 1;
    gpoints += (int)p;
    sprintf(title, "Points: %d, speed: %f, moves: %d, lines: %d\n"
	    , gpoints, 5000000. / (float)cspeed, nmoves, glines);
    glutSetWindowTitle(title);
    for (x=0;x<a->x;x++)
    for (y=0;y<a->y;y++)
    for (z=0;z<a->z;z++) if (a->pos[x][y][z] == 3) 
      {
       c = rand() % (int)(100.*(float)lined*(float)lined*sqrt((float)lined));
       if (c > 20*lined+4) a->pos[x][y][z] = 4;
       else a->pos[x][y][z] = 3;
      }
   }
 glines += lined;
 return lined;
}

float distance(area* a, int i, int j)
{
 float dx,dy,dz;
 dx = (float)(a->mem[i][0] - a->mem[j][0]);
 dy = (float)(a->mem[i][1] - a->mem[j][1]);
 dz = (float)(a->mem[i][2] - a->mem[j][2]);
 return dx*dx + dy*dy + dz*dz;
}

float max_dist(area* a, int id)
{
 int i;
 float md, d;
 md = -1e10;
 for (i=0;i<Bl;i++)
   {
    if (i == id) continue;
    if ((d = distance(a, i, id)) > md) md = d;
   }
 return md;
}

void compute_rcenter(area* a)
{
 int i, id;
 float min, md;
 min = 1e10;
 id = 0;
 for (i=0;i<Bl;i++)
   {
    if ((md = max_dist(a, i)) < min)
      {
       min = md;
       id = i; 
      }
   }
 a->rx = a->mem[id][0];
 a->ry = a->mem[id][1];
 a->rz = a->mem[id][2];
 a->pos[a->rx][a->ry][a->rz] = 1;
}

void create_object(area* a)
{
 int x2,z2,y,i;
 int cx,cy,cz;
 int dx,dy,dz;
 int f,ne,k;
 x2 = a->x / 2;
 z2 = a->z / 2;
 y = 1;
 ne = 0;
 a->pos[x2][y][z2] = 2;	
 a->rx = 0;
 a->ry = 0;
 a->rz = 0;
 cx = x2;
 cy = y;
 cz = z2;
 a->mem[0][0] = cx;
 a->mem[0][1] = cy;
 a->mem[0][2] = cz;
 cspeed -= dspeed;
 if (cspeed < 20000) cspeed = 20000;
 for (i=0;i<Bl-1;i++)
   {
    rnd:
    make_direction(&dx, &dy, &dz, bltype, i);
    k = rand() % (i+1);
    cx = a->mem[k][0];
    cy = a->mem[k][1];
    cz = a->mem[k][2];
    if (cx + dx >= a->x || cx + dx < 0) goto rnd;
    if (cy + dy >= a->y || cy + dy < 0) goto rnd;
    if (cz + dz >= a->z || cz + dz < 0) goto rnd;
    f=a->pos[cx+dx][cy+dy][cz+dz];
    if (f && f == 3) 
      {
       printf("Near the end!\n");
       ne ++;
       if (ne == 3) 
         {
	  printf("GAME OVER!\n");
	  printf("Points: %d\n", gpoints);
	  printf("Lines: %d\n", glines);
	  printf("Moves: %d\n", nmoves);
	  printf("Speed: %f\n", 5000000./(float)cspeed);
	  exit(0);
	 }
      }
    if (f) goto rnd;
    a->pos[cx+dx][cy+dy][cz+dz] = 2;	/* obiekt */
    cx += dx;
    cy += dy;
    cz += dz;
    a->mem[i+1][0] = cx;
    a->mem[i+1][1] = cy;
    a->mem[i+1][2] = cz;
   }
 compute_rcenter(a);
 inCS();
 moving = 1;
 burst = 0;
 outCS();
 nmoves ++;
}

void rotate_object(area* a, int rtx, int rty, int rtz)
{
 int x,y,z;
 int rx,ry,rz;
 int nx,ny,nz;
 int dx,dy,dz;
 int f,ft,cm,cnm;

 nx = ny = nz = 0;

 if (paused) return;

 cm = cnm = 0;

 inCS();
 
 rx = a->rx;
 ry = a->ry;
 rz = a->rz;
/* printf("rcenter: %d,%d,%d\n", rx,ry,rz);*/
 for (x=0;x<a->x;x++)
 for (y=0;y<a->y;y++)
 for (z=0;z<a->z;z++)
   {
    f = a->pos[x][y][z];
    if (f != 2 && f != 1) continue;
    dx = rx - x;
    dy = ry - y;
    dz = rz - z;
/*    printf("d=(%d,%d,%d)\n", dx,dy,dz);*/
       if (rtx)
         {
          nx = x;
          ny = ry + rtx * dz;
          nz = rz - rtx * dy;
         }
       else if (rty)
         {
          nx = rx + rty * dz;
          ny = y;
          nz = rz - rty * dx;
         }
       else if (rtz)
         {
          nx = rx + rtz * dy;
          ny = ry - rtz * dx;
          nz = z;
         }
    if (nx < 0 || nx >= a->x) continue;
    if (ny < 0 || ny >= a->y) continue;
    if (nz < 0 || nz >= a->z) continue;
    ft = a->pos[nx][ny][nz];
    if (f == 1 || f == 2)	/* jezeli to ruchomy klocek */
       {
        if (ft <= 2) cm ++;
	else cnm ++;
       }
   }
/* printf("can rotate = (%d,%d)\n", cm, cnm);*/
 if (cm == Bl)
   {
    for (x=0;x<a->x;x++)
    for (y=0;y<a->y;y++)
    for (z=0;z<a->z;z++)
      {
       f = a->pos[x][y][z];
/*       if (f) printf(" f = %d\n", f);*/
       if ((f & 0xf) != 2) continue;
       dx = rx - x;
       dy = ry - y;
       dz = rz - z;
/*    printf("d=(%d,%d,%d)\n", dx,dy,dz);*/
       if (rtx)
         {
          nx = x;
          ny = ry + rtx * dz;
          nz = rz - rtx * dy;
         }
       else if (rty)
         {
          nx = rx + rty * dz;
          ny = y;
          nz = rz - rty * dx;
         }
       else if (rtz)
         {
          nx = rx + rtz * dy;
          ny = ry - rtz * dx;
          nz = z;
         }
       a->pos[nx][ny][nz] = 
	   ((a->pos[x][y][z] & 0xf) << 0x4) + (a->pos[nx][ny][nz] & 0xf);
       a->pos[x][y][z] >>= 0x4;
      }
    for (x=0;x<a->x;x++)
    for (y=0;y<a->y;y++)
    for (z=0;z<a->z;z++)
      {
       f = a->pos[x][y][z];
       if ((f >> 0x4) == 2) a->pos[x][y][z] = 2;
      }
   }   
 outCS();
}

void translate_object(area* a, int dx, int dz)
{
 int x,y,z,f,ft;
 int cm,cnm;
 int sx,ex,dix;
 int sz,ez,diz;

 if (paused) return;
 
 cm = cnm = 0;

 if (dx > 0) { sx = a->x-1; ex = -1;   dix = -1; }
 else        { sx = 0;      ex = a->x; dix = 1;  }
 if (dz > 0) { sz = a->z-1; ez = -1;   diz = -1; }
 else        { sz = 0;      ez = a->z; diz = 1;  }
 
 inCS();

 for (x=0;x<a->x;x++)
 for (y=0;y<a->y;y++)
 for (z=0;z<a->z;z++)
   {
    f = a->pos[x][y][z];
    if (x+dx < 0 || x+dx >= a->x) continue;
    if (z+dz < 0 || z+dz >= a->z) continue;
    ft = a->pos[x+dx][y][z+dz];
    if (f == 1 || f == 2)	/* jezeli to ruchomy klocek */
       {
        if (ft <= 2) cm ++;
	else cnm ++;
       }
   }
 if (cm == Bl)
   {
    for (x=sx;x!=ex;x+=dix)
    for (y=0; y<a->y;y++)
    for (z=sz;z!=ez;z+=diz)
      {
       f = a->pos[x][y][z];
       if (x+dx < 0 || x+dx >= a->x) continue;
       if (z+dz < 0 || z+dz >= a->z) continue;
       if (f == 1 || f == 2)
         {
          a->pos[x+dx][y][z+dz] = a->pos[x][y][z];
          a->pos[x][y][z] = 0;
	 }
      }
    a->rx += dx;
    a->rz += dz;
   }
 
 outCS();
}

void drop_rest(area* a)
{
 int x,y,z,f,fup;
 int needed;
 needed = 0;
 for (x=0;x<a->x;x++)
 for (z=0;z<a->z;z++)
     if (a->pos[x][a->y-1][z] == 4) a->pos[x][a->y-1][z] = 3;
 for (y=a->y-2;y>=0;y--)
   {
    for (x=0;x<a->x;x++)
    for (z=0;z<a->z;z++)
      {
       f = a->pos[x][y][z];
       if (f != 4) continue;
       fup = a->pos[x][y+1][z];
       if (fup == 0 || fup == 4)
         {
          a->pos[x][y+1][z] = f;
	  a->pos[x][y][z] = 0;
         }
       else 
         {
	  a->pos[x][y][z] = 3;
	  needed ++;
	 }
      }
   }
 if (needed) check_lines(a);
}

void move_object(area* a)
{
 int x,y,z,f,fup;
 int cm,cnm;
 cm = 0;
 cnm = 0;

 inCS();

 drop_rest(a);
 
 for (y = a->y-2;y>=0;y--)
   {
    for (x=0;x<a->x;x++)
    for (z=0;z<a->z;z++)
      {
       f = a->pos[x][y][z];
       fup = a->pos[x][y+1][z];
       if (f == 1 || f == 2)	/* jezeli to ruchomy klocek */
         {
          if (fup <= 2) cm ++;
	  else cnm ++;
         }
      }
   }
 if (cm == Bl)
   {
    for (y = a->y-2;y>=0;y--)
    for (x=0;x<a->x;x++)
    for (z=0;z<a->z;z++)
      {
       f = a->pos[x][y][z];
       if (f == 1 || f == 2)
         {
          a->pos[x][y+1][z] = a->pos[x][y][z];
          a->pos[x][y][z] = 0;
	 }
      }
    a->ry ++;
   }
 else
   {
    for (y = a->y-1;y>=0;y--)
    for (x=0;x<a->x;x++)
    for (z=0;z<a->z;z++)
      {
       f = a->pos[x][y][z];
       if (f == 1 || f == 2) 
         {
	  a->pos[x][y][z] = 3;
	 }
      }
    moving = check_lines(a);
    a->rx = 0;
    a->ry = 0;
    a->rz = 0;
   }

 outCS();
}

void* game_thr(void* dummy)
{
 printf("game started.\n");
 while (!end)
  {
   if (!moving) create_object(&ar);
   else 
     {
      while (paused) usleep(50000);
      if (!burst) usleep(cspeed);
      move_object(&ar);
     }
  }
 return NULL;
}

void skeyboard(int key, int x, int y)
{
 switch (key)
   {
    case GLUT_KEY_LEFT:  translate_object(&ar, -1, 0); break;
    case GLUT_KEY_RIGHT: translate_object(&ar, 1, 0);  break;
    case GLUT_KEY_UP:    translate_object(&ar, 0, 1);  break;
    case GLUT_KEY_DOWN:  translate_object(&ar, 0, -1); break;
    case GLUT_KEY_PAGE_UP:   rotate_object(&ar, 1, 0, 0);  break;
    case GLUT_KEY_PAGE_DOWN: rotate_object(&ar, -1, 0, 0); break;
    case GLUT_KEY_HOME:      rotate_object(&ar, 0, 1, 0);  break;
    case GLUT_KEY_END:       rotate_object(&ar, 0, -1, 0); break;
    case GLUT_KEY_INSERT:    rotate_object(&ar, 0, 0, -1); break;
   }
}

void toggle_pause()
{
 paused = ! paused;
 if (paused) glutSetWindowTitle("Paused!");
 else glutSetWindowTitle("./program");
}

void go_faster()
{
 if (paused) return;
 if (!moving) create_object(&ar);
 else move_object(&ar);
}

void normalize_angles()
{
 if (angX < 0.)    angX += 360.;
 if (angX >= 360.) angX -= 360.;
 if (angY < 0.)    angY += 360.;
 if (angY >= 360.) angY -= 360.;
 if (angZ < 0.)    angZ += 360.;
 if (angZ >= 360.) angZ -= 360.;
}

void grow_window(float factor)
{
 int nx,ny;
 nx = (int)((float)wx * factor);
 ny = (int)((float)wy * factor);
 glutReshapeWindow(nx, ny);
}

void sinfo()
{
 char str[128];
 sprintf(str, "Current speed: %d, dspeed: %d\n", cspeed, dspeed);
 glutSetWindowTitle(str);
}


void keyboard(unsigned char key, int x, int y)
{
 normalize_angles();
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
	case 'r': glutFullScreen(); break;
	case 'w': dspeed = (int)((float)dspeed / 1.1); sinfo(); break;
	case 'e': dspeed = (int)((float)dspeed * 1.1); sinfo(); break;
	case 'f': grow_window(1./1.05); break;
	case 'g': grow_window(1.05); break;
	case 's': dtype++; if (dtype >= 16) dtype = 0; break;
	case 'd': dtype--; if (dtype <= -1) dtype = 15; break;
	case 'a': bltype++; if (bltype == 5) bltype = 0; break;
        case 'i': angX -= 2.; break;
        case 'k': angX += 2.; break;
        case 'j': angY += 2.; break;
        case 'l': angY -= 2.; break;
        case 'o': angZ += 2.; break;
        case 'm': angZ -= 2.; break;
        case '-': scalef /= 1.05; break;
	case '=': case '+': scalef *= 1.05; break;
	case ' ': go_faster(); break;
	case '4': auto_move = ! auto_move; break;
	case '5': scx /= 1.05; break;
	case '6': scx *= 1.05; break;
	case '7': scy /= 1.05; break;
	case '8': scy *= 1.05; break;
	case '9': scz /= 1.05; break;
	case '0': scz *= 1.05; break;
	case 'p': toggle_pause(); break;
	case '1': cspeed *= 2; slowdown /= 2; break;
	case '2': cspeed /= 2; slowdown *= 2; break;
	case 'z': burst = 1; break;
	case 'n': scx = scy = scz = 1.; scalef = 1.; angY = 10.; angZ = 0; angX = 170.; auto_move = 0; break;
        case GLUT_KEY_DELETE:    rotate_object(&ar, 0, 0, 1);  break;
        case 'c':    rotate_object(&ar, 1, 0, 0);  break;
        case 'v':    rotate_object(&ar, 0, 1, 0);  break;
        case 'b':    rotate_object(&ar, 0, 0, 1);  break;
   }
/* printf("key: %d\n", key);*/
}


void resize_scene(int w, int h)
{
 wx = w;
 wy = h;
 glViewport(0, 0, (GLsizei)wx, (GLsizei)wy);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1., 1., -1., 1., 1.5, 2000.);
 glMatrixMode(GL_MODELVIEW);
}

void myBox(float x, float y, float z)
{
 glPushMatrix();
   glScalef(x, y, z);
   glDisable(GL_LIGHTING);
   glLineWidth(3.);
   glutWireCube(1.);
   glLineWidth(1.);
   glEnable(GL_LIGHTING);
 glPopMatrix();
}

void material(float r, float g, float b);

void aWall()
{
 glBegin(GL_QUADS);
   glNormal3d(1., 0., 0.);
   
   glTexCoord2d(0., 0.);
   glVertex3d(.52, -.52, -.52);
   
   glTexCoord2d(0., 1.);
   glVertex3d(.52, -.52, .52);
   
   glTexCoord2d(1., 1.);
   glVertex3d(.52, .52, .52);
   
   glTexCoord2d(1., 0.);
   glVertex3d(.52, .52, -.52);
 glEnd();
}

void mySolidCube()
{
 glBindTexture(GL_TEXTURE_2D, tid);
   aWall();
 glPushMatrix();
   glRotated(90., 0., 1., 0.);
   aWall();
   glRotated(90., 0., 1., 0.);
   aWall();
   glRotated(90., 0., 1., 0.);
   aWall();
   glRotated(90., 0., 1., 0.);
   glRotated(90., 0., 0., 1.);
   aWall();
   glRotated(180., 0., 0., 1.);
   aWall();
 glPopMatrix();
 	

   
 glEnd();
}

void draw_3d_object()
{
 if (dtype == 0)
   {
    if (tenabled) mySolidCube();
    else glutSolidCube(1.05);
   }
 else if (dtype == 1) glutSolidCube(.52);
 else if (dtype == 2) glutSolidCube(1.05);
 else if (dtype == 3) glutWireCube(1.05);
 else if (dtype == 4) glutWireCube(.52);
 else if (dtype == 5) { glScaled(.4, .4, .4); glutSolidDodecahedron(); }
 else if (dtype == 6) { glScaled(.4, .4, .4); glutWireDodecahedron(); }
 else if (dtype == 7) { glScaled(.8, .8, .8); glutSolidOctahedron(); }
 else if (dtype == 8) { glScaled(.8, .8, .8); glutWireOctahedron(); }
 else if (dtype == 9) { glScaled(.8, .8, .8); glutSolidTetrahedron(); }
 else if (dtype == 10) { glScaled(.8, .8, .8); glutWireTetrahedron(); }
 else if (dtype == 11) { glScaled(.7, .7, .7); glutSolidIcosahedron(); }
 else if (dtype == 12) { glScaled(.7, .7, .7); glutWireIcosahedron(); }
 else if (dtype == 13) { glRotated(180., 1., 0., .3); glutSolidTeapot(.45); }
 else if (dtype == 14) { glRotated(180., 1., 0., .3); glutWireTeapot(.45); }
/*   glutWireIcosahedron();*/
}


void render_field3d(area* a, int x, int y, int z)
{
 float dx,dy,dz;
 int f;
/* float c;*/
 f = a->pos[x][y][z];
 if (!f) return;
 /*printf("f = %d\n",f);*/
 dx = (float)x - (float)a->x / 2. + .5; 
 dy = (float)y - (float)a->y / 2. + .5; 
 dz = (float)z - (float)a->z / 2. + .5; 
 switch (f)
   {
    case 1: material(.6, 9., 9.); break;
    case 2: material(.9, .9, 9.); break;
    case 3: material(.6, .6, .9); break;
    case 4: material(.9, .4, .4); break;
   }
 glPushMatrix();
   glTranslated(dx, dy, dz);
   draw_3d_object();
 glPopMatrix();
}

void render_field2d(area* a, int x, int y, int z)
{
 float ax,ay,az;
 float dx,dy,dz;
 int f;
 f = a->pos[x][y][z];
 if (!f) return ;
 
 dx = (float)x - (float)a->x / 2. + .5; 
 dy = (float)y - (float)a->y / 2. + .5; 
 dz = (float)z - (float)a->z / 2. + .5; 
 
 ax = (float)a->x / 2.; 
 ay = (float)a->y / 2.; 
 az = (float)a->z / 2.; 
 
 glDisable(GL_LIGHTING);
 glDisable(GL_TEXTURE_2D);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 if (f >= 3) glColor4d(.26, .26, 0., 0.42);
 else glColor4d(0., .45, 0., 0.42);
 glBegin(GL_QUADS);
   glVertex3d(dx-.5, dy-.5, az);
   glVertex3d(dx-.5, dy+.5, az);
   glVertex3d(dx+.5, dy+.5, az);
   glVertex3d(dx+.5, dy-.5, az);
 glEnd();
 glBegin(GL_QUADS);
   glVertex3d(dx-.5, dy-.5, -az);
   glVertex3d(dx-.5, dy+.5, -az);
   glVertex3d(dx+.5, dy+.5, -az);
   glVertex3d(dx+.5, dy-.5, -az);
 glEnd();
 glBegin(GL_QUADS);
   glVertex3d(ax, dy-.5, dz-.5);
   glVertex3d(ax, dy-.5, dz+.5);
   glVertex3d(ax, dy+.5, dz+.5);
   glVertex3d(ax, dy+.5, dz-.5);
 glEnd();
 glBegin(GL_QUADS);
   glVertex3d(-ax, dy-.5, dz-.5);
   glVertex3d(-ax, dy-.5, dz+.5);
   glVertex3d(-ax, dy+.5, dz+.5);
   glVertex3d(-ax, dy+.5, dz-.5);
 glEnd();
 if (f >= 3) glColor4d(.9, .9, 0., 0.4);
 else glColor4d(0., .9, 0., 0.4);
 glBegin(GL_QUADS);
   glVertex3d(dx-.5, ay, dz-.5);
   glVertex3d(dx-.5, ay, dz+.5);
   glVertex3d(dx+.5, ay, dz+.5);
   glVertex3d(dx+.5, ay, dz-.5);
 glEnd();
 glBegin(GL_QUADS);
   glVertex3d(dx-.5, -ay, dz-.5);
   glVertex3d(dx-.5, -ay, dz+.5);
   glVertex3d(dx+.5, -ay, dz+.5);
   glVertex3d(dx+.5, -ay, dz-.5);
 glEnd();
 glLineWidth(2.);
 if (f >= 3) glColor4d(.32, .32, 0., 0.45);
 else glColor4d(0., .5, 0., 0.45);
 glBegin(GL_LINE_LOOP);
   glVertex3d(dx-.5, dy-.5, az);
   glVertex3d(dx-.5, dy+.5, az);
   glVertex3d(dx+.5, dy+.5, az);
   glVertex3d(dx+.5, dy-.5, az);
 glEnd();
 glBegin(GL_LINE_LOOP);
   glVertex3d(dx-.5, dy-.5, -az);
   glVertex3d(dx-.5, dy+.5, -az);
   glVertex3d(dx+.5, dy+.5, -az);
   glVertex3d(dx+.5, dy-.5, -az);
 glEnd();
 glBegin(GL_LINE_LOOP);
   glVertex3d(ax, dy-.5, dz-.5);
   glVertex3d(ax, dy-.5, dz+.5);
   glVertex3d(ax, dy+.5, dz+.5);
   glVertex3d(ax, dy+.5, dz-.5);
 glEnd();
 glBegin(GL_LINE_LOOP);
   glVertex3d(-ax, dy-.5, dz-.5);
   glVertex3d(-ax, dy-.5, dz+.5);
   glVertex3d(-ax, dy+.5, dz+.5);
   glVertex3d(-ax, dy+.5, dz-.5);
 glEnd();
 if (f >= 3) glColor4d(.9, .9, 0., 0.5);
 else glColor4d(0., .9, 0., 0.5);
 glBegin(GL_LINE_LOOP);
   glVertex3d(dx-.5, ay, dz-.5);
   glVertex3d(dx-.5, ay, dz+.5);
   glVertex3d(dx+.5, ay, dz+.5);
   glVertex3d(dx+.5, ay, dz-.5);
 glEnd();
 glBegin(GL_LINE_LOOP);
   glVertex3d(dx-.5, -ay, dz-.5);
   glVertex3d(dx-.5, -ay, dz+.5);
   glVertex3d(dx+.5, -ay, dz+.5);
   glVertex3d(dx+.5, -ay, dz-.5);
 glEnd();
 glLineWidth(1.);
 glEnable(GL_TEXTURE_2D);
 glEnable(GL_LIGHTING);
 glDisable(GL_BLEND);
}

void render_shadow()
{
 glDisable(GL_LIGHTING);
 glDisable(GL_TEXTURE_2D);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glColor4d(.15, .15, .15, 0.5);
 glBegin(GL_QUADS);
   glVertex3d(-.52, -.54, -.52);
   glVertex3d(.52, -.54, -.52);
   glVertex3d(.52, -.54, .52);
   glVertex3d(-.52, -.54, .52);
 glEnd();
 glEnable(GL_TEXTURE_2D);
 glEnable(GL_LIGHTING);
 glDisable(GL_BLEND);
}

void render_shadows(area* a)
{
 int x,y,z;
 int f,j,ft;
 int ns;
 float dx,dy,dz;
 for (x=0;x<a->x;x++)
 for (y=0;y<a->y;y++)
 for (z=0;z<a->z;z++)
   {
    f = a->pos[x][y][z];
    if (f != 1 && f != 2) continue;
    ns = 0;
    for (j=y+1;j<a->y;j++)
      {
       ft = a->pos[x][j][z];
       if (ft == 3 || ft == 4) 
         {
          ns = 1;
	  break;
         }
      }
    if (!ns) continue;
    dx = (float)x - (float)a->x / 2. + .5; 
    dy = (float)j - (float)a->y / 2. + .5; 
    dz = (float)z - (float)a->z / 2. + .5; 
/*    printf("rendering shadow: %f,%f,%f\n", dx, dy, dz);*/
    glPushMatrix();
      glTranslated(dx, dy, dz);
      render_shadow();
    glPopMatrix();
   }
}
	

void render_area(area* a)
{
 int i,j,k;
 if (!a) return;
 glColor3d(0., 0., 1.);
 glDisable(GL_TEXTURE_2D);
 myBox(a->x, a->y, a->z);
 glEnable(GL_TEXTURE_2D);
 for (i=0;i<a->x;i++)
 for (j=0;j<a->y;j++)
 for (k=0;k<a->z;k++) render_field3d(&ar, i, j ,k);
 for (i=0;i<a->x;i++)
 for (j=0;j<a->y;j++)
 for (k=0;k<a->z;k++) render_field2d(&ar, i, j ,k);
 render_shadows(&ar);
}

float smallrand() { return (float)(rand() % 100) / 500.; }

void automove()
{
 normalize_angles();
/* printf("%f,%f,%f\n", angX, angY, angZ);*/
 if (dirx > 0)
   {
    angX += .331 + smallrand();
    if (angX >= 205.) dirx = -1.;
   }
 else
  {
   angX -= .331 + smallrand();
   if (angX <= 155.) dirx = 1.;
  }
 if (diry > 0)
   {
    angY += .472 + smallrand();
    if (angY >= 25. && angY < 180.) diry = -1.;
   }
 else
  {
   angY -= .472 + smallrand();
   if (angY <= 335. && angY > 180.) diry = 1.; 
  }
 if (dirz > 0)
   {
    angZ += .137 + smallrand();
    if (angZ >= 15. && angZ < 180.) dirz = -1.;
   }
 else
  {
   angZ -= .137 + smallrand();
   if (angZ <= 345. && angZ > 180.) dirz = 1.;
  }
}

void render_scene(void)
{
 time_counter();
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 gluLookAt(0., 0., 20., 0. , 0., 0., 0., 1., 0.);
 glScaled(scalef, scalef, scalef);
 glScaled(scx, scy, scz);
 glRotatef(angX, 1., 0. , 0.);
 glRotatef(angY, 0., 1. , 0.);
 glRotatef(angZ, 0., 0. , 1.);

 render_area(&ar);
 
 if (auto_move)
  {
   automove();
  }
 glFlush();
 glutSwapBuffers();
}


void anim(void)
{
 glutPostRedisplay();
}


void visible(int vis)
{
 if (vis == GLUT_VISIBLE) glutIdleFunc(anim);
 else                     glutIdleFunc(NULL);
}


void compute_ptbl()
{
 float sum;
 int i;
/* int j;*/
 for (i=0;i<=pcnt;i++)
  {
   sum = pxp[i] + pxm[i] + pyp[i] + pym[i] + pzp[i] + pzm[i];
   if (fabs(sum) < 1e-6) 
     {
      pxp[i] = pxm[i] = pyp[i] = pym[i] = pzp[i] = pzm[i] = 1.;
     }
   pxp[i] /= sum;
   pxm[i] /= sum;
   pyp[i] /= sum;
   pym[i] /= sum;
   pzp[i] /= sum;
   pzm[i] /= sum;
   ptbl[i][0] = 0.;
   ptbl[i][1] = pxp[i];
   ptbl[i][2] = ptbl[i][1] + pxm[i];
   ptbl[i][3] = ptbl[i][2] + pyp[i];
   ptbl[i][4] = ptbl[i][3] + pym[i];
   ptbl[i][5] = ptbl[i][4] + pzp[i];
   ptbl[i][6] = ptbl[i][5] + pzm[i];
  }
 /*for (i=0;i<=pcnt;i++)
   {
    printf("********************\n");
    for (j=0;j<7;j++) printf("ptbl[%d][%d] = %f\n", i, j, ptbl[i][j]);
   }*/
}


void Init()
{
 time_t tm;
 help();
 angZ = 0;
 angY = 20.;
 angX = -180.;
 dspeed = 1000;
 cspeed = (int)(5000000. / sp) + dspeed;
 slowdown = 1.;
 nmoves = 0;
 glines = 0;
 dtype = 0;
 scx = scy = scz = 1.;
 dirx = diry = dirz = 1;
 time(&tm);
 srand((int)tm);
 if (Ax < 1) Ax = 1;
 if (Ay < 6) Ay = 6;
 if (Az < 1) Az = 1;
 if (Bl < 2) Bl = 2;
 init_area(&ar, Ax, Ay, Az);
 moving = 0;
 paused = 0;
 gpoints = 0;
 pthread_mutex_init(&mut, NULL);
 pthread_create(&thr, NULL, game_thr, NULL);
}


void material(float r, float g, float b)
{
 GLfloat mat_diffuse[4]; /* = { 0.6, 0.6, 0.6, 1.0 }; */
 GLfloat mat_diffuse2[4]; /* = { 0.6, 0.6, 0.6, 1.0 }; */
 GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
 GLfloat mat_shininess[1] = { 127.0 };
 GLfloat mat_specular2[4] = { 1.0, 1.0, 1.0, 1.0 };
 GLfloat mat_shininess2[1] = { 127.0 };
 mat_diffuse[0] = r;
 mat_diffuse[1] = g;
 mat_diffuse[2] = b;
 mat_diffuse[4] = 0.7;
 mat_diffuse2[0] = r;
 mat_diffuse2[1] = g;
 mat_diffuse2[2] = b;
 mat_diffuse2[4] = 0.7;
 glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
 glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
 glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
 glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse2);
 glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular2);
 glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess2);
}


void glInit()
{
 GLfloat lightPos0[] = { -22.322, 27.64244, -52.266, 0.};
 GLfloat lightPos1[] = { 36.511, 22.733, 43.355, 0.};
 GLfloat lightPos2[] = { -2.322, 67.64244, -2.266, 0.};
 GLfloat lightPos3[] = { 6.511, -52.733, 3.355, 0.};
 GLfloat lightColor0[] = { 1.0f, 1.0f, 0.5f, 0.7 };
 GLfloat lightColor1[] = { 0.5f, 1.0f, 1.0f, 0.7 };
 GLfloat lightColor2[] = { 1.0f, 0.5f, 1.0f, 0.7 };
 GLfloat lightColor3[] = { 0.7f, 0.7f, 1.0f, 0.7 };
 glClearColor(0.15, 0.1, 0.2, 0.);
 glShadeModel(GL_SMOOTH);
 material(.9, .9, .9);
 glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
 glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
 glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
 glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
 glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightColor0);
 glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
 glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightColor1);
 glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1);
 glLightfv(GL_LIGHT2, GL_DIFFUSE,  lightColor2);
 glLightfv(GL_LIGHT2, GL_SPECULAR, lightColor2);
 glLightfv(GL_LIGHT3, GL_DIFFUSE,  lightColor3);
 glLightfv(GL_LIGHT3, GL_SPECULAR, lightColor3);
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHT1);
 glEnable(GL_LIGHT2);
 glEnable(GL_LIGHT3);
 glEnable(GL_DEPTH_TEST);
 if (load_bmp(texname))
   {
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmx, bmy, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);
    glBindTexture(GL_TEXTURE_2D, tid);
    printf("Texturing enabled.\n");
    tenabled = 1;
   }
 else tenabled = 0;
}

void make_texname(char* nt)
{
 texname = (char*)malloc(strlen(nt)+2);
 strcpy(texname, nt);
}

void default_values()
{
 int i;
 pcnt = 1;
 pxp = (float*)malloc((pcnt+1)*sizeof(float));
 pyp = (float*)malloc((pcnt+1)*sizeof(float));
 pzp = (float*)malloc((pcnt+1)*sizeof(float));
 pxm = (float*)malloc((pcnt+1)*sizeof(float));
 pym = (float*)malloc((pcnt+1)*sizeof(float));
 pzm = (float*)malloc((pcnt+1)*sizeof(float));
 ptbl = (float**)malloc((pcnt+1)*sizeof(float*));
 for (i=0;i<=pcnt;i++)
   {
    ptbl[i] = (float*)malloc(7*sizeof(float));
    pxp[i] = 1.;
    pxm[i] = 1.;
    pyp[i] = 1.;
    pym[i] = 1.;
    pzp[i] = 1.;
    pzm[i] = 1.;
   }
 compute_ptbl();
 printf("default vaules used.\n");
}

void file_values(FILE* f)
{
 int i;
 int ns;
 pcnt = -1;
 ns = fscanf(f, "Steps: %d\n", &pcnt);
 if (pcnt < 0 || ns != 1) { default_values(); return; }
 pxp = (float*)malloc((pcnt+1)*sizeof(float));
 pyp = (float*)malloc((pcnt+1)*sizeof(float));
 pzp = (float*)malloc((pcnt+1)*sizeof(float));
 pxm = (float*)malloc((pcnt+1)*sizeof(float));
 pym = (float*)malloc((pcnt+1)*sizeof(float));
 pzm = (float*)malloc((pcnt+1)*sizeof(float));
 ptbl = (float**)malloc((pcnt+1)*sizeof(float*));
 for (i=0;i<pcnt;i++)
   {
    ptbl[i] = (float*)malloc(7*sizeof(float));
    ns = fscanf(f, "%f %f %f %f %f %f\n", 
	    &pxp[i], &pxm[i], &pyp[i], &pym[i], &pzp[i], &pzm[i]);
    if (ns != 6 || pxp[i] < 0. || pxm[i] < 0. || pyp[i] < 0. || pym[i] < 0. || pzp[i] < 0. || pzm[i] < 0.)
      {
       default_values();
       return;
      }
   }
 ptbl[pcnt] = (float*)malloc(7*sizeof(float));
 ns = fscanf(f, "default: %f %f %f %f %f %f\n", 
	    &pxp[pcnt], &pxm[pcnt], &pyp[pcnt], &pym[pcnt], &pzp[pcnt], &pzm[pcnt]);
 if (ns != 6 || pxp[pcnt] < 0. || pxm[pcnt] < 0. || pyp[pcnt] < 0. || pym[pcnt] < 0. || pzp[pcnt] < 0. || pzm[pcnt] < 0.)
   {
    default_values();
    return;
   }
 compute_ptbl();
 printf("file vaules used.\n");
}

void read_rndinfo(char* fn)
{
 FILE* fp;
 fp = fopen(fn, "r");
 if (!fp) default_values();
 else { file_values(fp); fclose(fp); }
}

void parse_cmdline(int lb, char**par)
{
 Ax = (lb >= 2) ? atoi(par[1]) : 8;
 Ay = (lb >= 3) ? atoi(par[2]) : 22;
 Az = (lb >= 4) ? atoi(par[3]) : 8;
 Bl = (lb >= 5) ? atoi(par[4]) : 5;
 sp = (lb >= 6) ? atof(par[5]) : 1.;
 bltype = (lb >= 7) ? atoi(par[6]) : 0;
 make_texname((lb >= 8) ? par[7] : "texture.bmp");
 read_rndinfo((lb >= 9) ? par[8] : "default.rnd");
}


int main(int lb, char** par)
{
 wx = 500;
 wy = 500;
 parse_cmdline(lb, par);
 Init();
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(10, 10);
 glutCreateWindow(par[0]);
 glInit();
 glutDisplayFunc(render_scene);
 glutReshapeFunc(resize_scene);
 glutKeyboardFunc(keyboard);
 glutSpecialFunc(skeyboard);
 glutVisibilityFunc(visible);
 glutMainLoop();
 return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
