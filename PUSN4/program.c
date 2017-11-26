/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#define PI  3.1415926F
#define PI2 6.2831853F
/*#define DEBUG*/

typedef struct _F
{
 GLdouble dx,dy,dz;
 GLdouble rx,ry,rz;
 GLdouble** matrix;
} Frame;

typedef struct _KC
{
  GLint nchains;
  Frame* chain;
  GLdouble x,y,z;
  GLdouble** matrix;
} KChain;

GLint wx,wy;
GLint cx,cy;
GLint idx;
GLdouble angX, angY, angZ;
GLdouble scalef;
time_t t1,t2;
GLint fps;
GLint auto_move;
KChain chain;

void randomize()
{
 time_t tm;
 time(&tm);
 srand((int)tm);
}


GLdouble* vector(GLint siz)
{
 if (siz <= 0) return NULL;
 return (GLdouble*)malloc(siz*sizeof(GLdouble));
}


GLdouble** matrix(GLint siz)
{
 GLdouble** mem;
 int i;
 if (siz <= 0) return NULL;
 mem = (GLdouble**)malloc(siz*sizeof(GLdouble*));
 for (i=0;i<siz;i++) mem[i] = vector(siz);
 return mem;
}


void copy_matrix(GLdouble** dst, GLdouble** src, GLint siz)
{
 int i,j;
 if (!src || !dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++) dst[i][j] = src[i][j];
}


void I_matrix(GLdouble** dst, GLint siz)
{
 int i,j;
 if (!dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++)
   {
    if (i == j) dst[i][j] = 1.;
    else dst[i][j] = 0.;
   }
}


int try_swap(GLdouble** m, GLint idx, GLint dim)
{
 register int x;
 for (x=idx;x<dim;x++) if (m[idx][x]) return x;
 return -1;
}


GLdouble** invert_matrix(GLdouble** srcC, GLint dim)
{
 GLdouble** src, **dst;
 GLdouble div, pom;
 register GLint x,k;
 GLint i,swit;
 GLdouble* vectmp;
 src = matrix(dim);
 dst = matrix(dim);
 vectmp = vector(dim);
 copy_matrix(src, srcC, dim);
 I_matrix(dst, dim);
 for (i=0;i<dim;i++)
   {
    div = src[i][i];
    if (div == 0.0)
      {
       swit = try_swap(src, i, dim);
       if (swit < 0)
         {
	   printf("UNINVERTABLE_MATRIX\n");
	   exit(1);
	 }
       for (x=0;x<dim;x++) vectmp[x]    = src[x][i];
       for (x=0;x<dim;x++) src[x][i]    = src[x][swit];
       for (x=0;x<dim;x++) src[x][swit] = vectmp[x];
       for (x=0;x<dim;x++) vectmp[x]    = dst[x][i];
       for (x=0;x<dim;x++) dst[x][i]    = dst[x][swit];
       for (x=0;x<dim;x++) dst[x][swit] = vectmp[x];
       div = src[i][i];
      }
    for (x=0;x<dim;x++)
      {
       src[x][i] /= div;
       dst[x][i] /= div;
      }
    for (k=0;k<dim;k++)
      {
       pom = src[i][k];
       if (k-i)
         {
          for (x=0;x<dim;x++) src[x][k] -= pom* src[x][i];
          for (x=0;x<dim;x++) dst[x][k] -= pom* dst[x][i];
         }
      }
   }
 return dst;
}


GLdouble** matrix_mul(GLdouble** m, GLdouble** n, GLint ma, GLint mb, GLint na, GLint nb)
{
 GLdouble** dst;
 register GLint k,j,i;
 if (ma <= 0 || mb  <= 0 || na <= 0 || nb <=0 || mb != na) return NULL;
 if (!m || !n) return NULL;
 dst = (GLdouble**)malloc(ma*sizeof(GLdouble*));
 if (!dst) return NULL;
 for (i=0;i<ma;i++) dst[i] = (GLdouble*)malloc(nb*sizeof(GLdouble));
 for (i=0;i<ma;i++)
 for (j=0;j<nb;j++)
    {
     dst[i][j] = 0.0;
     for (k=0;k<mb;k++) dst[i][j] += m[i][k] * n[k][j];
    }
 return dst;
}

void matrix_mul_vector(GLdouble* dst, GLdouble** m, GLdouble* v, GLint len)
{
 GLint i,j;
 for (i=0;i<len;i++)
    {
     dst[i] = 0.0;
     for (j=0;j<len;j++) dst[i] += v[j] * m[i][j];
    }
}


void free_matrix(GLdouble** m, GLint siz)
{
 GLint i;
 if (siz <= 0) return;
 for (i=0;i<siz;i++) free(m[i]);
 free(m);
}


void rotatex(GLdouble** m, GLdouble ang)
{
 ang /= 180./PI;
 m[1][1] = cos(ang);
 m[2][1] = sin(ang);
 m[1][2] = -sin(ang);
 m[2][2] = cos(ang);
}


void rotatey(GLdouble** m, GLdouble ang)
{
 ang /= 180./PI;
 m[0][0] = cos(ang);
 m[2][0] = -sin(ang);
 m[0][2] = sin(ang);
 m[2][2] = cos(ang);
}


void rotatez(GLdouble** m, GLdouble ang)
{
 ang /= 180./PI;
 m[0][0] = cos(ang);
 m[1][0] = sin(ang);
 m[0][1] = -sin(ang);
 m[1][1] = cos(ang);
}

void translatex(GLdouble** m, GLdouble arg)
{
 m[0][3] = arg;
}

void translatey(GLdouble** m, GLdouble arg)
{
 m[1][3] = arg;
}

void translatez(GLdouble** m, GLdouble arg)
{
 m[2][3] = arg;
}

void translate(GLdouble** m, GLdouble x, GLdouble y, GLdouble z)
{
 translatex(m, x);
 translatey(m, y);
 translatez(m, z);
}

void print_frame(Frame* f)
{
 GLint i,j;
 if (!f) { printf("Frame (null)\n"); return; }
 printf("Frame (%p)\n", (void*)f);
 printf("D(%f,%f,%f), R(%f,%f,%f)\n", f->dx, f->dy, f->dz, f->rx, f->ry, f->rz);
 for (i=0;i<4;i++)
   {
    for (j=0;j<4;j++) printf("%f\t ", f->matrix[i][j]);
    printf("\n");
   }
}

void init_frame(Frame* f)
{
 if (!f) return;
 f->dx = f->dy = f->dz = 0.;
 f->rx = f->ry = f->rz = 0.;
 f->matrix = matrix(4);
 I_matrix(f->matrix,4);
}

void free_frame(Frame* f)
{
 free_matrix(f->matrix, 4);
}

void move_frameD(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
  GLdouble **m, **res;
  if (!f) return ;
  m = matrix(4);
  I_matrix(m, 4);
  translate(m, x, y, z);
  res = matrix_mul(f->matrix, m, 4, 4, 4, 4);
  copy_matrix(f->matrix, res, 4);
  free_matrix(res, 4);
  free_matrix(m, 4);
  f->dx += x;
  f->dy += y;
  f->dz += z;
/*  print_frame(f);*/
}

void set_frameD(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
  if (!f) return ;
  I_matrix(f->matrix, 4);
  translate(f->matrix, x, y, z);
  f->dx = x;
  f->dy = y;
  f->dz = z;
/*  print_frame(f);*/
}

void make_angles(Frame* f)
{
 if (!f) return;
 while (f->rx < 0.) f->rx += 360.;
 while (f->ry < 0.) f->ry += 360.;
 while (f->rz < 0.) f->rz += 360.;
 while (f->rx >= 360.) f->rx -= 360.;
 while (f->ry >= 360.) f->ry -= 360.;
 while (f->rz >= 360.) f->rz -= 360.;
}

void rotate_frameR(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
 GLdouble **m1,**m2,**m3, **mt, **m, **res;
 if (!f) return;
 m1 = matrix(4);
 m2 = matrix(4);
 m3 = matrix(4);
 mt = matrix(4);
 I_matrix(m1, 4);
 I_matrix(m2, 4);
 I_matrix(m3, 4);
 rotatex(m1, x);
 rotatey(m2, y);
 rotatez(m3, z);
 mt = matrix_mul(m1, m2, 4, 4, 4, 4);
 m  = matrix_mul(mt, m3, 4, 4, 4, 4);
 res = matrix_mul(f->matrix, m, 4, 4, 4, 4);
 copy_matrix(f->matrix, res, 4);
 free_matrix(res, 4);
 free_matrix(m1, 4);
 free_matrix(m2, 4);
 free_matrix(m3, 4);
 free_matrix(mt, 4);
 free_matrix(m, 4);
 f->rx += x;
 f->ry += y;
 f->rz += z;
 make_angles(f);
/* print_frame(f);*/
}

void set_frameR(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
 GLdouble **m1,**m2,**m3, **mt, **m;
 if (!f) return;
 m1 = matrix(4);
 m2 = matrix(4);
 m3 = matrix(4);
 mt = matrix(4);
 I_matrix(m1, 4);
 I_matrix(m2, 4);
 I_matrix(m3, 4);
 rotatex(m1, x);
 rotatey(m2, y);
 rotatez(m3, z);
 mt = matrix_mul(m1, m2, 4, 4, 4, 4);
 m  = matrix_mul(mt, m3, 4, 4, 4, 4);
 copy_matrix(f->matrix, m, 4);
 free_matrix(m1, 4);
 free_matrix(m2, 4);
 free_matrix(m3, 4);
 free_matrix(mt, 4);
 free_matrix(m, 4);
 f->rx = x;
 f->ry = y;
 f->rz = z;
 make_angles(f);
/* print_frame(f);*/
}

void set_frameRD(Frame* f, GLdouble rx, GLdouble ry, GLdouble rz, GLdouble dx, GLdouble dy, GLdouble dz)
{
 if (!f) return;
 set_frameR(f, rx, ry, rz);
 set_frameD(f, dx, dy, dz);
}

void set_frameDR(Frame* f, GLdouble dx, GLdouble dy, GLdouble dz, GLdouble rx, GLdouble ry, GLdouble rz)
{
 if (!f) return;
 set_frameD(f, dx, dy, dz);
 set_frameR(f, rx, ry, rz);
}

void print_chain(KChain* c)
{
 GLint i,j;
 if (!c) { printf("Chain (null)\n"); return; }
 printf("Chain (%p)\n", (void*)c);
 printf("POS(%f,%f,%f)\n", c->x, c->y, c->z);
 for (i=0;i<4;i++)
   {
    for (j=0;j<4;j++) printf("%f\t ", c->matrix[i][j]);
    printf("\n");
   }
}

void init_chain(KChain* kc, GLint n)
{
 GLint i;
 if (!kc) return;
 if (n <= 1) { printf("not enough chains: %d\n", n); exit(1); }
 kc->x = kc->y = kc->z = 0.;
 kc->nchains = n;
 kc->chain = (Frame*)malloc(n * sizeof(Frame));
 for (i=0;i<n;i++) init_frame(&kc->chain[i]);
 kc->matrix = matrix(4);
}

void free_chain(KChain* kc)
{
 GLint i;
 if (!kc) return;
 for (i=0;i<kc->nchains;i++) free_frame(&kc->chain[i]);
 free(kc->chain);
 free_matrix(kc->matrix, 4);
}

void make_example()
{
 GLint i,n;
 GLdouble rx,ry,rz,dx,dy,dz;
 randomize();
 n = chain.nchains;
 for (i=0;i<n;i++)
   {
    rx = (GLdouble)((rand() % 31) - 15);
    ry = (GLdouble)((rand() % 31) - 15);
    rz = (GLdouble)((rand() % 31) - 15);
    dx = (GLdouble)((rand() % 21) - 10);
    dy = (GLdouble)((rand() % 21) - 10);
    dz = (GLdouble)((rand() % 21) - 10);
    set_frameR(&chain.chain[i], 0., 0., 0.);
    set_frameD(&chain.chain[i], 0., 0., 0.);
    rotate_frameR(&chain.chain[i], rx, ry, rz);
    move_frameD(&chain.chain[i], dx, dy, dz);
   }
}


void help()
{
 printf("usage: {basename}\n");
 printf("space:        normal values\n");
 printf("1:            toggle auto move\n");
 printf("+/-:          scale\n");
 printf("z/x:          change indexes\n");
 printf("i/k/j/l/o/m/: rotations\n");
 printf("r/t/f/g/v/b:  move current frame\n");
 printf("w/s/a/d/e/c:  rotate current frame\n");
 printf("p:            example\n");
}


void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  free_chain(&chain); exit(0); break;
	case 'p': make_example(&chain); break;
	case 'r': move_frameD(&chain.chain[idx], 5., 0., 0.); break;
	case 't': move_frameD(&chain.chain[idx], -5., 0., 0.); break;
	case 'f': move_frameD(&chain.chain[idx], 0., 5., 0.); break;
	case 'g': move_frameD(&chain.chain[idx], 0., -5., 0.); break;
	case 'v': move_frameD(&chain.chain[idx], 0., 0., 5.); break;
	case 'b': move_frameD(&chain.chain[idx], 0., 0., -5.); break;
	case 'w': rotate_frameR(&chain.chain[idx], 5., 0., 0.); break;
	case 's': rotate_frameR(&chain.chain[idx], -5., 0., 0.); break;
	case 'a': rotate_frameR(&chain.chain[idx], 0., 5., 0.); break;
	case 'd': rotate_frameR(&chain.chain[idx], 0., -5., 0.); break;
	case 'e': rotate_frameR(&chain.chain[idx], 0., 0., 5.); break;
	case 'c': rotate_frameR(&chain.chain[idx], 0., 0., -5.); break;
	case 'z': idx--; if (idx < 0) idx += chain.nchains; break;
	case 'x': idx++; if (idx >= chain.nchains) idx -= chain.nchains; break;
        case 'i': angX -= 10.; break;
        case 'k': angX += 10.; break;
        case 'j': angY += 10.; break;
        case 'l': angY -= 10.; break;
        case 'o': angZ += 10.; break;
        case 'm': angZ -= 10.; break;
        case '1': auto_move = ! auto_move; break;
        case '-': scalef /= 1.3; break;
	case '=': case '+': scalef *= 1.3; break;
	case ' ': scalef = 1.; angX = angY = angZ = 0.; auto_move = 0; break;
   }
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

void time_counter()
{
 char tstr[0x100];
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
    sprintf(tstr, "GLScene FPS: %d, chain (%d)", fps/(int)(t2-t1), idx);
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
}

void randomColor()
{
 GLint col;
 col = rand() % 8;
 switch (col)
 {
  case 0: glColor4d(0., 0., 0., .4); break;
  case 1: glColor4d(0., 0., 1., .4); break;
  case 2: glColor4d(0., 1., 0., .4); break;
  case 3: glColor4d(0., 1., 1., .4); break;
  case 4: glColor4d(1., 0., 0., .4); break;
  case 5: glColor4d(1., 0., 1., .4); break;
  case 6: glColor4d(1., 1., 0., .4); break;
  case 7: glColor4d(1., 1., 1., .4); break;
 }
}

void renderChains()
{
 GLdouble** m;
 GLdouble** res;
 GLdouble* v;
 GLdouble* rv;
 GLint i,n;
 n = chain.nchains;
 v = vector(4);
 rv = vector(4);
 m = matrix(4);
 I_matrix(m, 4);
 glLineWidth(3.);
 glBegin(GL_LINE_STRIP);
 randomColor();
 glVertex3d(chain.x, chain.y, chain.z);
   for (i=0;i<n;i++)
     {
      res = matrix_mul(m, chain.chain[i].matrix, 4, 4, 4, 4);
      copy_matrix(m, res, 4);
      free_matrix(res, 4);
      v[0] = chain.x;
      v[1] = chain.y;
      v[2] = chain.z;
      v[3] = 1.;
      matrix_mul_vector(rv, m, v, 4);
      randomColor();
      glVertex3d(rv[0], rv[1], rv[2]);
     }
 glEnd();
 free(v);
 free(rv);
 copy_matrix(chain.matrix, m, 4);
 free_matrix(m, 4);
 print_chain(&chain);
}

void glCube(GLdouble x, GLdouble y, GLdouble z, GLdouble v)
{
 glBegin(GL_LINE_STRIP);
   randomColor();
   glVertex3d(x-v, y-v, z-v);
   randomColor();
   glVertex3d(x-v, y+v, z-v);
   randomColor();
   glVertex3d(x+v, y+v, z-v);
   randomColor();
   glVertex3d(x+v, y-v, z-v);
   randomColor();
   glVertex3d(x-v, y-v, z-v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   randomColor();
   glVertex3d(x-v, y-v, z+v);
   randomColor();
   glVertex3d(x-v, y+v, z+v);
   randomColor();
   glVertex3d(x+v, y+v, z+v);
   randomColor();
   glVertex3d(x+v, y-v, z+v);
   randomColor();
   glVertex3d(x-v, y-v, z+v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   randomColor();
   glVertex3d(x-v, y-v, z-v);
   randomColor();
   glVertex3d(x-v, y+v, z-v);
   randomColor();
   glVertex3d(x-v, y+v, z+v);
   randomColor();
   glVertex3d(x-v, y-v, z+v);
   randomColor();
   glVertex3d(x-v, y-v, z-v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   randomColor();
   glVertex3d(x+v, y-v, z-v);
   randomColor();
   glVertex3d(x+v, y+v, z-v);
   randomColor();
   glVertex3d(x+v, y+v, z+v);
   randomColor();
   glVertex3d(x+v, y-v, z+v);
   randomColor();
   glVertex3d(x+v, y-v, z-v);
 glEnd();

}

void renderPoints()
{
 GLdouble** m;
 GLdouble** res;
 GLdouble* v;
 GLdouble* rv;
 GLint i,n;
 n = chain.nchains;
 v = vector(4);
 rv = vector(4);
 m = matrix(4);
 I_matrix(m, 4);
 glLineWidth(3.);
 if (idx == 0) glCube(chain.x, chain.y, chain.z, 2.);
 else glCube(chain.x, chain.y, chain.z, 1.);
   for (i=0;i<n;i++)
     {
      res = matrix_mul(m, chain.chain[i].matrix, 4, 4, 4, 4);
      copy_matrix(m, res, 4);
      free_matrix(res, 4);
      v[0] = chain.x;
      v[1] = chain.y;
      v[2] = chain.z;
      v[3] = 1.;
      matrix_mul_vector(rv, m, v, 4);
      glVertex3d(rv[0], rv[1], rv[2]);
      if (idx == i+1) glCube(rv[0], rv[1], rv[2], 2.);
      else glCube(rv[0], rv[1], rv[2], 1.);
     }
 free(v);
 free(rv);
 copy_matrix(chain.matrix, m, 4);
 free_matrix(m, 4);
 print_chain(&chain);
}


void render_scene(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glLoadIdentity();
 gluLookAt(0., 0., 100., 0. , 0., 0., 0., 100., 0.);
 glScaled(scalef, scalef, scalef);
 glRotatef(angX, 1., 0. , 0.);
 glRotatef(angY, 0., 1. , 0.);
 glRotatef(angZ, 0., 0. , 1.);
 if (auto_move)
  {
   angX += 2.3;
   angY += 2.5;
   angZ += 2.7;
  }
 srand(1);
 renderChains();
 renderPoints();
 time_counter();
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


void glInit()
{
 glClearColor(.5,.5,.5,0.);
 glShadeModel(GL_SMOOTH);
 angX = angY =angZ = 0.;
 auto_move = 1;
 scalef = 1.;
 idx = 0;
 randomize();
}


void lightInit()
{
   GLfloat mat_diffuse[] = { 0.6, 0.6, .6, 1.0 };
   GLfloat mat_specular[] = { 1., 1.0, 1., 1.0 };
   GLfloat mat_shininess[] = { 127.0 };
   GLfloat mat_diffuse2[] = { 0.6, 0.6, 0.6, 1.0 };
   GLfloat mat_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess2[] = { 127.0 };
   GLfloat lightPos0[] = { 0., 0., -1., 0.};
   GLfloat lightPos1[] = { 0., 0., 1., 0.};
   GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };
   GLfloat lightColor1[] = { .7f, 1.0f, 0.0f, 1.0 };
   glClearColor(1., 1., 1., 0.);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse2);
   glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular2);
   glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess2);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
   glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor1);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
   glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);
}


int main(int lb, char** par)
{
 wx = 800;
 wy = 600;
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(100, 100);
 glutCreateWindow(par[0]);
 glutDisplayFunc(render_scene);
 glutReshapeFunc(resize_scene);
 glutKeyboardFunc(keyboard);
 glutVisibilityFunc(visible);
 glInit();
 if (lb < 2) init_chain(&chain, 5);
 else init_chain(&chain, atof(par[1]));
 glutMainLoop();
 return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
