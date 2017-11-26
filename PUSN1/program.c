/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>
#define PI 3.1415926F
float angX, angY, angZ, obsD, tStep;
int wx,wy;
int cx,cy;
time_t t1, t2;
int fps;
float ambientLight[] = { 0.3f, 0.5f, 0.8f, 1.0f };
float diffuseLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
float specularLight[] = { 0.f, 0.f, 1.f, 1.0f };
float lightPosition[] = { 1.0f, .8f, 0.6f, 0.0f };
float matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float matDiff[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float ox, oy;
int need_normals;
int mill_stp;
enum {
DISPLAY_TRIANGLES,
DISPLAY_LINES
};
enum {
CENTER_CIRCLE,
CENTER_TANGENT_POINT
};
enum {
MENU_AUTO_MOVE,
MENU_AUTO_ROTATION,
MENU_DISPLAY_MODE,
MENU_DISPLAY_MILL,
MENU_MILL_ALL,
MENU_QUIT
};
int auto_move = 0.;
int auto_rotation = 0.;
int display_mode = DISPLAY_TRIANGLES;
int display_mill = 1.;

typedef struct _Block
{
float** b;
float** bnx;
float** bny;
float** bnz;
float step;
float bx, by, bz, bzmin;
int nx, ny;
} Block;
Block block;

typedef struct _PList
{
struct _PList* next;
struct _PList* prev;
float x,y,z;
} PList;

typedef struct _Mill
{
char mtype;
float d,r;
int center;
float h;
int nx,ny;
float** map;
float x,y,z;
PList* path;
} Mill;
Mill* mill;
int nmill;
GLUquadric* qobj;
int current_mill;

void init(Block* b, float bx, float by, float bz, float bzmin, float istep)
{
int i,j;
GLfloat step;
b->bx = bx;
b->by = by;
b->bz = bz;
ox = bx/2.;
oy = by/2.;
printf("%f, %f\n", ox, oy);
b->bzmin = bzmin;
b->step = istep;
b->nx = b->bx / b->step;
b->ny = b->by / b->step;
step = b->step;
b->b = (float**)malloc(b->nx*sizeof(float*));
for (i=0;i<b->nx;i++) b->b[i] = (float*)malloc(b->ny*sizeof(float));
b->bnx = (float**)malloc((b->nx+2)*sizeof(float*));
for (i=0;i<b->nx+2;i++) b->bnx[i] = (float*)malloc((b->ny+2)*sizeof(float));
b->bny = (float**)malloc((b->nx+2)*sizeof(float*));
for (i=0;i<b->nx+2;i++) b->bny[i] = (float*)malloc((b->ny+2)*sizeof(float));
b->bnz = (float**)malloc((b->nx+2)*sizeof(float*));
for (i=0;i<b->nx+2;i++) b->bnz[i] = (float*)malloc((b->ny+2)*sizeof(float));
for (i=0;i<b->nx;i++)
for (j=0;j<b->ny;j++) b->b[i][j] = b->bz /*- 6*sin(i*step/10.) - 6*sin(j*step/10.)*/;
angX = -60.;
angY = 0.;
angZ = 105.;
need_normals = 1;
mill_stp = 0;
}


void help()
{
printf("usage: program bx by bz bmin step [file1 [file2 ...]]\n");
printf("fileN are mill definition files\n");
printf("rotations: w/s/a/d/e/x\n");
printf("toggle mill: f, toggle lines: l, toggle move: t, toggle rotations: r\n");
printf("y: mill all\n");
printf("menu: rbutton\n");
printf("mill step: n, quit: q/ESC, help: h\n");
}


void path_step_old(Mill* mil)
{
float x1,y1,z1;
float x2,y2,z2;
float dx,dy,dz;
float tmpd;
int i1,j1,i,j;
PList* tmp;
if (!mil->path) { current_mill++; printf("Finished milling.\n"); return; }
/* printf("Frezowanie kolejnej sciezki...\n");*/
/* printf("%p %p %p\n", mil->path->prev, mil->path, mil->path->next);*/
dx = mil->path->x;
dy = mil->path->y;
dz = mil->path->z;
x1 = mil->x;
y1 = mil->y;
z1 = mil->z;
x2 = x1+dx;
y2 = y1+dy;
z2 = z1+dz;
tmp = mil->path;
mil->path = mil->path->prev;
free(tmp);
i1 = (x2-mil->r)/block.step;
j1 = (y2-mil->r)/block.step;
for (i=i1;i<i1+mil->nx;i++)
for (j=j1;j<j1+mil->ny;j++)
{
if (mil->map[i-i1][j-j1] > 1e9) continue; /* nie frezowac czescia nie nalezaca do frezarki */
if (i < 0 || i >= block.nx) continue;
if (j < 0 || j >= block.ny) continue; /* nie frezowac poza klockiem */
tmpd = block.b[i][j];
block.b[i][j] = z2 - mil->map[i-i1][j-j1];
if (block.b[i][j] > tmpd) block.b[i][j] = tmpd;
if ((mil->mtype=='p') && (z2<z1) && (tmpd != block.b[i][j]))
{
block.b[i][j] = tmpd;
printf("Nie mozna frezowac w dol frezem plaskim (Ruch w dol, zmiana materialu, frez plaski)!\n");
continue;
}
if (block.b[i][j] > block.bz) block.b[i][j] = block.bz; /* nie frezujemy nad blokiem */
if (block.b[i][j] < block.bzmin)
{
printf("zbyt glebokie frezowanie (ponizej Zmin)!\n");
block.b[i][j] = block.bz;
}
if (tmpd-block.b[i][j]>mil->h)
{
printf("frezowanie czescia niefrezujaca (ilosc wycietego materialu wieksza niz dl. cz. frezujacej)!\n");
block.b[i][j] = block.bz;
}
/* printf("map: %f, z2: %f, %d,%d/%d,%d/%d,%d/%d,%d\n", mil->map[i-i1][j-j1], z2, i1, j1,i,j,mil->nx,mil->ny,block.nx,block.ny);*/
}
/* printf("(%f,%f,%f) i=%d, j=%d\n", x2, y2, z2, i, j);*/
mil->x = x2;
mil->y = y2;
mil->z = z2;
need_normals = 1;
}


void path_step(Mill* mil)
{
float x1,y1,z1;
float x2,y2,z2;
float dx,dy,dz;
float ddx,ddy,ddz;
float tmpd;
float len,steps,d;
int error;
volatile register int i1,j1,i,j;
int nx,ny;
PList* tmp;
if (!mil->path) { current_mill++; printf("Finished milling.\n"); return; }
/* printf("Frezowanie kolejnej sciezki...\n");*/
/* printf("%p %p %p\n", mil->path->prev, mil->path, mil->path->next);*/
/*mil->y += 100;*/
/*mil->y += 100;*/
dx = mil->path->x-mil->x;
dy = mil->path->y-mil->y;
dz = mil->path->z-mil->z;
x1 = mil->x;
y1 = mil->y;
z1 = mil->z;
/* printf("(%f,%f) %f %f %f d(%f,%f,%f)\n", mil->r, mil->h, x1, y1, z1,dx,dy,dz);*/
len = sqrt(dx*dx+dy*dy+dz*dz);
steps = len/block.step;
ddx = dx/steps;
ddy = dy/steps;
ddz = dz/steps;
tmp = mil->path;
mil->path = mil->path->prev;
free(tmp);
nx = mil->nx;
ny = mil->ny;
error = 0;
for (d=0.;d<steps;d+=1.)
{
x2 = x1 + ddx*d;
y2 = y1 + ddy*d;
z2 = z1 + ddz*d;
i1 = (x2-mil->r)/block.step;
j1 = (y2-mil->r)/block.step;
for (i=i1;i<i1+nx;i++)
for (j=j1;j<j1+ny;j++)
{
if (mil->map[i-i1][j-j1] > 1e9) continue; /* nie frezowac czescia nie nalezaca do frezarki */
if (i < 0 || i >= block.nx) continue;
if (j < 0 || j >= block.ny) continue; /* nie frezowac poza klockiem */
tmpd = block.b[i][j];
block.b[i][j] = z2 - mil->map[i-i1][j-j1];
if (block.b[i][j] > tmpd) block.b[i][j] = tmpd;
if ((mil->mtype=='p') && (z2<z1) && (tmpd != block.b[i][j]))
{
block.b[i][j] = tmpd;
if (!error) printf("Nie mozna frezowac w dol frezem plaskim (Ruch w dol, zmiana materialu, frez plaski)!\n");
error = 1;
continue;
}
if (block.b[i][j] > block.bz) block.b[i][j] = block.bz; /* nie frezujemy nad blokiem */
if (block.b[i][j] < block.bzmin)
{
if (!error) printf("zbyt glebokie frezowanie (ponizej Zmin)!\n");
block.b[i][j] = block.bz;
error = 1;
}
if (tmpd-block.b[i][j]>mil->h)
{
if (!error) printf("frezowanie czescia niefrezujaca (ilosc wycietego materialu wieksza niz dl. cz. frezujacej)!\n");
block.b[i][j] = block.bz;
error = 1;
}
/* printf("map: %f, z2: %f, %d,%d/%d,%d/%d,%d/%d,%d\n", mil->map[i-i1][j-j1], z2, i1, j1,i,j,mil->nx,mil->ny,block.nx,block.ny);*/
}
}
/* printf("(%f,%f,%f) i=%d, j=%d\n", x2, y2, z2, i, j);*/
x2 = x1+dx;
y2 = y1+dy;
z2 = z1+dz;
mil->x = x2;
mil->y = y2;
mil->z = z2;
need_normals = 1;
}


int mill_step()
{
if (current_mill >= nmill) { printf("No more moves!\n"); return 0; }
path_step(&mill[current_mill]);
printf("Milled paths: %d\n", ++mill_stp);
return 1;
}


void toggle_mill()
{
display_mill = !display_mill;
}


void toggle_lines()
{
display_mode = ! display_mode;
}


void keyboard(unsigned char key, int x, int y)
{
switch (key)
{
case 27: case 'q': exit(0); break;
case 'a': angY-=2.0; break;
case 'd': angY+=2.0; break;
case 's': angX+=2.0; break;
case 'w': angX-=2.0; break;
case 'x': angZ+=2.0; break;
case 'e': angZ-=2.0; break;
case 'y': while (mill_step()) ; break;
case ' ': angX=-60.; angY=0.; angZ=15.; break;
case 'n': if (auto_move != 1) mill_step(); break;
case 'f': toggle_mill(); break;
case 'l': toggle_lines(); break;
case 't': auto_move = ! auto_move; break;
case 'r': auto_rotation = ! auto_rotation; break;
}
}


void resize_scene(int w, int h)
{
wx = w;
wy = h;
glViewport(0, 0, (GLsizei)wx, (GLsizei)wy);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glFrustum(-1., 1., -1., 1., 1.5, 100.);
glMatrixMode(GL_MODELVIEW);
}


void time_counter()
{
char tstr[32];
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
sprintf(tstr, "GLScene FPS: %d", fps/(int)(t2-t1));
t1 = t2;
glutSetWindowTitle(tstr);
fps = 0;
}
}


void myNormal_old(int i, int j, float* nx, float* ny, float* nz)
{
GLfloat step = block.step;
GLfloat len,x,y,z;
x = step*(block.b[i+1][j]-block.b[i][j]);
y = step*(block.b[i][j+1]-block.b[i][j]);
z = step*step;
len = sqrt(x*x+y*y+z*z);
*nx = x/len;
*ny = y/len;
*nz = z/len;
/* printf("%f,%f,%f\n", *nx, *ny, *nz);*/
}


void myNormal(int i, int j, float* nx, float* ny, float* nz)
{
GLfloat step = block.step;
GLfloat len,x,y,z;
x = step*(block.b[i+1][j]-block.b[i][j]);
y = step*(block.b[i+1][j+1]-block.b[i+1][j]);
z = step*step;
len = sqrt(x*x+y*y+z*z);
*nx = x/len;
*ny = y/len;
*nz = z/len;
/* printf("%f,%f,%f\n", *nx, *ny, *nz);*/
}


void myNormalT1(int i, int j, float* nx, float* ny, float* nz)
{
GLfloat step = block.step;
GLfloat len,x,y,z;
x = step*(block.b[i+1][j]-block.b[i][j]);
y = step*(block.b[i+1][j+1]-block.b[i+1][j]);
z = step*step;
len = sqrt(x*x+y*y+z*z);
*nx = x/len;
*ny = y/len;
*nz = z/len;
/* printf("%f,%f,%f\n", *nx, *ny, *nz);*/
}


void myNormalT2(int i, int j, float* nx, float* ny, float* nz)
{
GLfloat step = block.step;
GLfloat len,x,y,z;
x = step*(block.b[i+1][j+1]-block.b[i][j+1]);
y = step*(block.b[i][j+1]-block.b[i][j]);
z = step*step;
len = sqrt(x*x+y*y+z*z);
*nx = x/len;
*ny = y/len;
*nz = z/len;
/* printf("%f,%f,%f\n", *nx, *ny, *nz);*/
}


float approx(float** tab, int i, int j)
{
return (tab[i][j]+tab[i+1][j]+tab[i][j+1])/3.;
/* return tab[i][j];*/
}


void interpolate_normals()
{
GLint nx,ny,i,j;
GLint max;
GLfloat** vec;
nx = block.nx-1;
ny = block.ny-1;
max = (nx>ny)?nx:ny+2;
vec = (float**)malloc(max*sizeof(float*));
for (i=0;i<max;i++) vec[i] = (float*)malloc(max*sizeof(float));
for (i=1;i<nx+1;i++)
for (j=1;j<ny+1;j++) vec[i][j] = (block.bnx[i-1][j-1]+block.bnx[i+1][j+1]+block.bnx[i+1][j-1]+block.bnx[i-1][j+1]+block.bnx[i-1][j]+block.bnx[i+1][j]+block.bnx[i][j-1]+block.bnx[i][j+1])/8.;
for (i=1;i<nx+1;i++)
for (j=1;j<ny+1;j++) block.bnx[i][j] = vec[i][j];
for (i=1;i<nx+1;i++)
for (j=1;j<ny+1;j++) vec[i][j] = (block.bny[i-1][j-1]+block.bny[i+1][j+1]+block.bny[i+1][j-1]+block.bny[i-1][j+1]+block.bny[i-1][j]+block.bny[i+1][j]+block.bny[i][j-1]+block.bny[i][j+1])/8.;
for (i=1;i<nx+1;i++)
for (j=1;j<ny+1;j++) block.bny[i][j] = vec[i][j];
for (i=1;i<nx+1;i++)
for (j=1;j<ny+1;j++) vec[i][j] = (block.bnz[i-1][j-1]+block.bnz[i+1][j+1]+block.bnz[i+1][j-1]+block.bnz[i-1][j+1]+block.bnz[i-1][j]+block.bnz[i+1][j]+block.bnz[i][j-1]+block.bnz[i][j+1])/8.;
for (i=1;i<nx+1;i++)
for (j=1;j<ny+1;j++) block.bnz[i][j] = vec[i][j];
for (i=0;i<max;i++) free(vec[i]);
free(vec);
}


void self_move()
{
if (auto_move) mill_step();
if (auto_rotation) angZ += .67;
}


void draw_mill(Mill* m)
{
if (!display_mill) return;
glTranslatef(m->x, m->y, m->z);
if (m->mtype == 'k' && !m->center) glTranslated(0., 0., -m->r);
gluCylinder(qobj, m->r, m->r, m->h, 32, 1);
if (m->mtype == 'k') gluSphere(qobj, m->r, 32, 32);
glTranslatef(0., 0., 1.01*m->h);
gluCylinder(qobj, m->r, m->r, m->h*2., 32, 1);
glTranslatef(-m->x, -m->y, -m->z-1.01*m->h);
if (m->mtype == 'k' && !m->center) glTranslated(0., 0., m->r);
}


void draw_mills()
{
int i;
for (i=0;i<nmill;i++) draw_mill(&mill[i]);
}


void render_scene(void)
{
GLint i,j,nx,ny;
GLfloat step;
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glEnable(GL_DEPTH_TEST);
glEnable(GL_CULL_FACE);
glFrontFace(GL_CCW);
glEnable(GL_LIGHTING);
glMaterialfv(GL_BACK, GL_AMBIENT, matAmbient);
glMaterialfv(GL_BACK, GL_DIFFUSE, matDiff);
glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, 127);
glEnable(GL_LIGHT0);
/* glEnable(GL_NORMALIZE);*/
/* glEnable(GL_AUTO_NORMAL);*/
glLoadIdentity();
gluLookAt(0., 0., 45., 0. , 0., 0., 0., 10., 0.);
glScaled(.15, .15, .15);
glRotatef(angX, 1., 0., 0.);
glRotatef(angY, 0., 1., 0.);
glRotatef(angZ, 0., 0., 1.);
glTranslatef(-block.bx/2., -block.by/2., -block.bz/1.5);
step = block.step;
nx = block.nx-1;
ny = block.ny-1;
if (need_normals)
{
for (i=0;i<nx;i++)
for (j=0;j<ny;j++)
myNormal(i, j, &block.bnx[i+1][j+1], &block.bny[i+1][j+1], &block.bnz[i+1][j+1]);
for (i=0;i<nx+2;i++)
{
block.bnx[i][0] = block.bnx[i][1];
block.bnx[i][ny+1] = block.bnx[i][ny];
block.bny[i][0] = block.bny[i][1];
block.bny[i][ny+1] = block.bny[i][ny];
block.bnz[i][0] = block.bnz[i][1];
block.bnz[i][ny+1] = block.bnz[i][ny];
}
for (j=0;j<ny+2;j++)
{
block.bnx[0][j] = block.bnx[1][j];
block.bnx[nx+1][j] = block.bnx[nx][j];
block.bny[0][j] = block.bny[1][j];
block.bny[nx+1][j] = block.bny[nx][j];
block.bnz[0][j] = block.bnz[1][j];
block.bnz[nx+1][j] = block.bnz[nx][j];
}
interpolate_normals();
need_normals = 0;
}
draw_mills();
if (display_mode == DISPLAY_TRIANGLES)
{
glBegin(GL_TRIANGLES);
/* glBegin(GL_LINES);*/
for (i=0;i<nx;i++)
{
for (j=0;j<ny;j++)
{
glNormal3f(block.bnx[i+1][j+1], block.bny[i+1][j+1], block.bnz[i+1][j+1]);
glVertex3f(i*step, j*step, block.b[i][j]);
glNormal3f(block.bnx[i+2][j+1], block.bny[i+2][j+1], block.bnz[i+2][j+1]);
glVertex3f((i+1)*step, j*step, block.b[i+1][j]);
glNormal3f(block.bnx[i+2][j+2], block.bny[i+2][j+2], block.bnz[i+2][j+2]);
glVertex3f((i+1)*step, (j+1)*step, block.b[i+1][j+1]);
glNormal3f(block.bnx[i+1][j+2], block.bny[i+1][j+2], block.bnz[i+1][j+2]);
glVertex3f(i*step, (j+1)*step, block.b[i][j+1]);
glNormal3f(block.bnx[i+1][j+1], block.bny[i+1][j+1], block.bnz[i+1][j+1]);
glVertex3f(i*step, j*step, block.b[i][j]);
glNormal3f(block.bnx[i+2][j+2], block.bny[i+2][j+2], block.bnz[i+2][j+2]);
glVertex3f((i+1)*step, (j+1)*step, block.b[i+1][j+1]);
}
}
glEnd();
}
else
{
glBegin(GL_LINE_STRIP);
/* glBegin(GL_LINES);*/
for (i=0;i<nx;i++)
{
for (j=0;j<ny;j++)
{
glNormal3f(block.bnx[i+1][j+1], block.bny[i+1][j+1], block.bnz[i+1][j+1]);
glVertex3f(i*step, j*step, block.b[i][j]);
glNormal3f(block.bnx[i+2][j+1], block.bny[i+2][j+1], block.bnz[i+2][j+1]);
glVertex3f((i+1)*step, j*step, block.b[i+1][j]);
glNormal3f(block.bnx[i+2][j+2], block.bny[i+2][j+2], block.bnz[i+2][j+2]);
glVertex3f((i+1)*step, (j+1)*step, block.b[i+1][j+1]);
glNormal3f(block.bnx[i+1][j+2], block.bny[i+1][j+2], block.bnz[i+1][j+2]);
glVertex3f(i*step, (j+1)*step, block.b[i][j+1]);
glNormal3f(block.bnx[i+1][j+1], block.bny[i+1][j+1], block.bnz[i+1][j+1]);
glVertex3f(i*step, j*step, block.b[i][j]);
glNormal3f(block.bnx[i+2][j+2], block.bny[i+2][j+2], block.bnz[i+2][j+2]);
glVertex3f((i+1)*step, (j+1)*step, block.b[i+1][j+1]);
}
}
glEnd();
}
/* glBegin(GL_LINES);*/
glBegin(GL_QUADS);
for (i=0;i<nx;i++)
{
glNormal3f(0.,-1., 0.);
glVertex3f(i*step, 0., 0.);
glVertex3f((i+1)*step, 0., 0.);
glVertex3f((i+1)*step, 0., block.b[i+1][0]);
glVertex3f(i*step, 0., block.b[i][0]);
glNormal3f(0., 1., 0.);
glVertex3f(i*step, ny*step, block.b[i][ny]);
glVertex3f((i+1)*step, ny*step, block.b[i+1][ny]);
glVertex3f((i+1)*step, ny*step, 0.);
glVertex3f(i*step, ny*step, 0.);
}
for (j=0;j<ny;j++)
{
glNormal3f(-1., 0., 0.);
glVertex3f(0., j*step, 0.);
glVertex3f(0., j*step, block.b[0][j]);
glVertex3f(0., (j+1)*step, block.b[0][j+1]);
glVertex3f(0., (j+1)*step, 0.);
glNormal3f(1., 0., 0.);
glVertex3f(nx*step, j*step, 0.);
glVertex3f(nx*step, (j+1)*step, 0.);
glVertex3f(nx*step, (j+1)*step, block.b[nx][j+1]);
glVertex3f(nx*step, j*step, block.b[nx][j]);
}
glEnd();
time_counter();
self_move();
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
else glutIdleFunc(NULL);
}


void glInit()
{
help();
glClearColor(0.,0.,0.,0.);
glShadeModel(GL_SMOOTH);
qobj = gluNewQuadric();
}


void plist_add(PList** pl, float x, float y, float z)
{
PList* tmp;
if (!pl) return;
/* printf("%f %f %f\n", x, y, z);*/
tmp = (PList*)malloc(sizeof(PList));
tmp->x = x;
tmp->y = y;
tmp->z = z;
if (!(*pl)) { tmp->next = 0; tmp->prev = 0; *pl = tmp; }
else { tmp->next = *pl; (*pl)->prev = tmp; tmp->prev = 0; *pl = tmp; }
}


void process_mill_old(char* mname, Mill* mil)
{
FILE* plik;
int ok;
char* nptr;
int i,j;
float dI, dJ, dist, dx, dy, dz;
nptr = mname;
if (!mil) exit(3);
printf("processing mill data file: %s\n", mname);
while (nptr[0] != '.' && nptr[0] != 0) nptr++;
if (!nptr[0])
{
printf("Bad Mill file name: doesn't contain `.'\n");
exit(1);
}
ok = sscanf(nptr, ".%c%f", &mil->mtype, &mil->d);
if (ok != 2)
{
printf("Bad Mill file name: doesn't contain (%%c%%d)\n");
exit(1);
}
if (mil->mtype != 'k' && mil->mtype != 'f')
{
printf("Mill can only be: 'f' or 'k'\n");
exit(1);
}
plik = fopen(mname, "r");
if (!plik)
{
printf("Cannot open file\n");
exit(1);
}
mil->r = mil->d/2.;
if (mil->mtype == 'k')
{
ok=fscanf(plik, "CENTER: %d\n", &mil->center);
if (ok!=1) { printf("Error: no center definition!\n"); exit(4); }
}
ok=fscanf(plik, "HEIGHT: %f\n", &mil->h);
if (ok!=1) { printf("Error: no height definition!\n"); exit(5); }
mil->nx = mil->ny = mil->d/block.step;
printf("Mill map (%dx%d)\n", mil->nx, mil->ny);
mil->map = (float**)malloc(mil->nx*sizeof(float*));
for (i=0;i<mil->nx;i++) mil->map[i] = (float*)malloc(mil->ny*sizeof(float));
for (i=0;i<mil->nx;i++)
for (j=0;j<mil->ny;j++)
{
dI = i*block.step-mil->r;
dJ = j*block.step-mil->r;
dist = sqrt(dI*dI+dJ*dJ);
if (dist > mil->r) { mil->map[i][j] = 1e10; continue; } /* do not mill */
if (mil->mtype == 'p') { mil->map[i][j] = 0.; continue; }
dist = mil->r*mil->r-dI*dI-dJ*dJ;
if (dist < 0.) dist = 0.;
mil->map[i][j] = sqrt(dist);
if (!mil->center) { mil->map[i][j] += mil->r; mil->h += mil->r; }
}
/*for (i=0;i<mil->nx;i++)
{
for (j=0;j<mil->ny;j++) printf("%f ", mil->map[i][j]);
printf("\n");
}*/
ok=fscanf(plik, "START: %f,%f,%f\n", &mil->x, &mil->y, &mil->z);
if (ok != 3){ printf("Error: no start position!\n"); exit(6); }
mil->path = 0;
while ((ok = fscanf(plik, "GO: %f,%f,%f\n", &dx, &dy, &dz)) == 3)
plist_add(&mil->path, dx, dy, dz);
if (mil->path) while (mil->path->next) mil->path = mil->path->next;
fclose(plik);
}


int mscanf(FILE* plik, float* x, float* y, float* z)
{
char* str;
int i,zn,j,ok;
ok = 0;
do
{
/* printf("TRY...\n");*/
str = malloc(1024);
i = 0;
do
{
zn = fgetc(plik);
if (zn >= 'a' && zn <= 'z') zn -= 0x20;
if ((zn >= '0' && zn <= '9') || zn == '.' || zn == 'X' || zn == 'Y' || zn == 'Z' || zn == '-')
{
str[i] = zn;
i++;
}
}
while (zn != '\n' && zn != EOF);
str[i] = 0;
/* printf("STR='%s'\n", str);*/
j = 0;
while (str[j] != 'X' && j < i) j++;
if (j < i)
{
str += j;
/* printf("%s\n", str); */
ok = sscanf(str,"X%fY%fZ%f", x,y,z);
if (ok == 3)
{ *x += ox; *y += oy;
printf("%f,%f,%f\n", *x, *y, *z);
return 3; }
}
free(str-j);
}
while (zn != EOF);
return ok;
}


void process_mill(char* mname, Mill* mil)
{
FILE* plik;
int ok;
char* nptr;
int i,j;
char zn[0x10];
float dI, dJ, dist, dx, dy, dz;
nptr = mname;
if (!mil) exit(3);
printf("processing mill data file: %s\n", mname);
while (nptr[0] != '.' && nptr[0] != 0) nptr++;
if (!nptr[0])
{
printf("Bad Mill file name: doesn't contain `.'\n");
exit(1);
}
ok = sscanf(nptr, ".%c%f", &mil->mtype, &mil->d);
if (ok != 2)
{
printf("Bad Mill file name: doesn't contain (%%c%%d)\n");
exit(1);
}
if (mil->mtype != 'k' && mil->mtype != 'f')
{
printf("Mill can only be: 'f' or 'k'\n");
exit(1);
}
plik = fopen(mname, "r");
if (!plik)
{
printf("Cannot open file\n");
exit(1);
}
mil->r = mil->d/2.;
if (mil->mtype == 'k')
{
printf("(0,0) point in (c) center or (t)angent point? ");
scanf("%s", zn);
if (zn[0] == 'c' || zn[0] == 'C') mil->center = 1;
else mil->center = 0;
}
else mil->center = 1;
if (mil->mtype == 'k')
{
/*ok=fscanf(plik, "CENTER: %d\n", &mil->center);
if (ok!=1) { printf("Error: no center definition!\n"); exit(4); }*/
}
/*ok=fscanf(plik, "HEIGHT: %lf\n", &mil->h);*/
printf("Mill height (milling area in mm): ");
scanf("%f", &mil->h);
/* if (ok!=1) { printf("Error: no height definition!\n"); exit(5); } */
mil->nx = mil->ny = mil->d/block.step;
printf("Mill(%f,%f) map (%dx%d)\n", mil->r, mil->h, mil->nx, mil->ny);
mil->map = (float**)malloc(mil->nx*sizeof(float*));
for (i=0;i<mil->nx;i++) mil->map[i] = (float*)malloc(mil->ny*sizeof(float));
for (i=0;i<mil->nx;i++)
for (j=0;j<mil->ny;j++)
{
dI = i*block.step-mil->r;
dJ = j*block.step-mil->r;
dist = sqrt(dI*dI+dJ*dJ);
if (dist > mil->r) { mil->map[i][j] = 1e10; continue; } /* do not mill */
if (mil->mtype == 'f') { mil->map[i][j] = 0.; continue; }
dist = mil->r*mil->r-dI*dI-dJ*dJ;
if (dist < 0.) dist = 0.;
mil->map[i][j] = sqrt(dist);
if (!mil->center) mil->map[i][j] -= mil->r;
}
if (!mil->center) mil->h += mil->r;
printf("Mill(%f,%f) map (%dx%d)\n", mil->r, mil->h, mil->nx, mil->ny);
for (i=0;i<mil->nx;i++)
{
for (j=0;j<mil->ny;j++) printf("%f ", mil->map[i][j]);
printf("\n");
}
ok=mscanf(plik, &mil->x, &mil->y, &mil->z);
if (ok != 3){ printf("Error: no start position!\n"); exit(6); }
mil->path = 0;
while ((ok = mscanf(plik, &dx, &dy, &dz)) == 3)
plist_add(&mil->path, dx, dy, dz);
if (mil->path) while (mil->path->next) mil->path = mil->path->next;
fclose(plik);
}


void menu(int sel)
{
switch(sel)
{
case MENU_AUTO_MOVE: auto_move = ! auto_move; break;
case MENU_AUTO_ROTATION: auto_rotation = ! auto_rotation; break;
case MENU_DISPLAY_MODE: display_mode = ! display_mode; break;
case MENU_DISPLAY_MILL: display_mill = ! display_mill; break;
case MENU_MILL_ALL: while (mill_step()) ; break;
case MENU_QUIT: exit(0); break;
}
}


int main(int lb, char** par)
{
int i;
wx = 800;
wy = 600;
if (lb < 5)
{
printf("usage: %s bx by bz bmin step [file1 [file2 ...]]\n", par[0]);
return 1;
}
glutInit(&lb, par);
glutInitDisplayMode(GLUT_DOUBLE);
glutInitWindowSize(wx, wy);
glutInitWindowPosition(100, 100);
glutCreateWindow(par[0]);
glutCreateMenu(menu);
glutAddMenuEntry("Toggle auto move", MENU_AUTO_MOVE);
glutAddMenuEntry("Toggle auto rotation", MENU_AUTO_ROTATION);
glutAddMenuEntry("Toggle display mode", MENU_DISPLAY_MODE);
glutAddMenuEntry("Toggle display mill", MENU_DISPLAY_MILL);
glutAddMenuEntry("Mill all", MENU_MILL_ALL);
glutAddMenuEntry("Quit", MENU_QUIT);
glutAttachMenu(GLUT_RIGHT_BUTTON);
glutAttachMenu(GLUT_LEFT_BUTTON);
glInit();
init(&block, atof(par[1]), atof(par[2]), atof(par[3]), atof(par[4]), atof(par[5]));
mill = (Mill*)malloc((lb-5)*sizeof(Mill));
nmill = lb-6;
current_mill = 0;
display_mill = 1;
display_mode = DISPLAY_TRIANGLES;
for (i=6;i<lb;i++) process_mill(par[i], &mill[i-6]);
glutDisplayFunc(render_scene);
glutReshapeFunc(resize_scene);
glutKeyboardFunc(keyboard);
glutVisibilityFunc(visible);
glutMainLoop();
return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */

