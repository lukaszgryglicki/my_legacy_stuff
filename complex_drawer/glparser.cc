#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/glut.h>
#include "parser.h"
GLdouble sx, ex, sz, ez, sy, ey, x, dx, dz, dy;
GLint nx, nz;
int use_sy = 0;
char win_title[256];
time_t t1, t2;
int fps;
void Init(char* func_def)
{
 t1 = t2 = (time_t)(0);
 nx = 64;
 nz = 64;
 dx = .03;
 dz = .03;
 dy = .03;
 if (!func_def) return;
 fpar_init(1);
 fpar_function(0, 1, func_def);
 if (!fpar_ok(0)) { printf("Function not accepted.\n"); return; }
 fpar_info();
 cldouble r = fpar_f(0, cldouble(1., 1.0),  NULL, NULL, NULL);
 printf("R = %Lf + i%Lf\n", r.real(), r.imag());
 if (!fpar_ok(0)) { printf("Function not accepted.\n"); return; }
}

void DeInit()
{
 fpar_free(0);
 fpar_deinit();
}

void Help()
{
 printf("Kolory: czerwona: os OY, niebieska: os OX, zielona: os OZ, biala: f(t)\n");
 printf("Klawiatura:\n");
 printf("Zwiekszanie zmniejszanie o dx:  o/p\n");
 printf("Zwiekszanie zmniejszanie o dz:  u/i\n");
 printf("Zwiekszanie zmniejszanie o dy:  1/2\n");
 printf("Zwiekszanie zmniejszanie dx  :  n/m\n");
 printf("Zwiekszanie zmniejszanie dz  :  v/b\n");
 printf("Zwiekszanie zmniejszanie dx  :  4/3\n");
 printf("Zwiekszanie zmniejszanie lenx:  k/l\n");
 printf("Zwiekszanie zmniejszanie lenz:  h/j\n");
 printf("Zwiekszanie zmniejszanie leny:  5/6\n");
 printf("Zwiekszanie zmniejszanie nx  :  t/y\n");
 printf("Zwiekszanie zmniejszanie nz  :  e/r\n");
 printf("Uzywanie granic dy/dx: 7 \n");
 printf("Zakoncz program: q/ESC\n");
}


void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char kez, int x, int y)
{
   double l, m;
   switch (kez)
   {
	  case 27 : case 'q': DeInit(); exit(0); break;
	  case 'o': sx -= dx; ex -= dx; break;
	  case 'p': sx += dx; ex += dx; break;
	  case 'u': sz -= dz; ez -= dz; break;
	  case 'i': sz += dz; ez += dz; break;
	  case '1': sy -= dy; ey -= dy; break;
	  case '2': sy += dy; ey += dy; break;
	  case 'm': dx *= 1.05; break;
	  case 'n': dx /= 1.05; break;
	  case 'b': dz *= 1.05; break;
	  case 'v': dz /= 1.05; break;
	  case '3': dy *= 1.05; break;
	  case '4': dy /= 1.05; break;
	  case '7': use_sy = !use_sy; break;
	  case 'k': m = (sx+ex)/2.; l = ex-sx; l*=1.05; sx = m-.5*l; ex = m+.5*l; break;
	  case 'l': m = (sx+ex)/2.; l = ex-sx; l/=1.05; sx = m-.5*l; ex = m+.5*l; break;
	  case 'h': m = (sz+ez)/2.; l = ez-sz; l*=1.05; sz = m-.5*l; ez = m+.5*l; break;
	  case 'j': m = (sz+ez)/2.; l = ez-sz; l/=1.05; sz = m-.5*l; ez = m+.5*l; break;
	  case '5': m = (sy+ey)/2.; l = ey-sy; l*=1.05; sy = m-.5*l; ey = m+.5*l; break;
	  case '6': m = (sy+ey)/2.; l = ey-sy; l/=1.05; sy = m-.5*l; ey = m+.5*l; break;
	  case 't': nx = (nx*5)/6; if (nx<6) nx = 6; break;
	  case 'y': nx = (nx*6)/5; if (nx>128) nx = 128; break;
	  case 'e': nz = (nz*5)/6; if (nz<6) nz = 6; break;
	  case 'r': nz = (nz*6)/5; if (nz>128) nz = 128; break;
	  case ' ': Init(NULL); break;
   }
}


void anim(void)
{
 glutPostRedisplay();
}

void visible(int vis)
{
  if (vis == GLUT_VISIBLE)    glutIdleFunc(anim);
  else    glutIdleFunc(NULL);
}

void minmax_compute(double* minyr, double* maxyr, double* minyi, double* maxyi)
{
 GLdouble t, tstep, u, ustep;
 cldouble x, y;
 *minyr = 9e99;
 *maxyr = -9e99;
 *minyi = 9e99;
 *maxyi = -9e99;
 tstep = (ex-sx)/(GLdouble)nx;
 ustep = (ez-sz)/(GLdouble)nz;
 for (t=sx;t<=ex;t+=tstep)
 for (u=sz;u<=ez;u+=ustep)
   {
    //val = fpar2_f(t, u);
    x = cldouble(t, u);
    y = fpar_f(0, x, NULL, NULL, NULL);
    if (y.real() > (*maxyr)) *maxyr = y.real();
    if (y.real() < (*minyr)) *minyr = y.real();
    if (y.imag() > (*maxyi)) *maxyi = y.imag();
    if (y.imag() < (*minyi)) *minyi = y.imag();
   }
}

void opaque(double miny, double maxy)
{
 glColor3f(1.,1.,0.);
 glBegin(GL_LINES);
   glVertex3d(sx, miny, sz);
   glVertex3d(sx, maxy, sz);
   glVertex3d(ex, miny, sz);
   glVertex3d(ex, maxy, sz);
   glVertex3d(sx, miny, ez);
   glVertex3d(sx, maxy, ez);
   glVertex3d(ex, miny, ez);
   glVertex3d(ex, maxy, ez);
   glVertex3d(sx, miny, sz);
   glVertex3d(sx, miny, ez);
   glVertex3d(ex, miny, sz);
   glVertex3d(ex, miny, ez);
   glVertex3d(sx, maxy, sz);
   glVertex3d(sx, maxy, ez);
   glVertex3d(ex, maxy, sz);
   glVertex3d(ex, maxy, ez); 
   glVertex3d(sx, miny, sz);
   glVertex3d(ex, miny, sz);
   glVertex3d(sx, maxy, sz);
   glVertex3d(ex, maxy, sz);
   glVertex3d(sx, miny, ez);
   glVertex3d(ex, miny, ez);
   glVertex3d(sx, maxy, ez);
   glVertex3d(ex, maxy, ez);
 glEnd();
}

int time_counter()
{
 int fp;
 if (t1 == (time_t)0)
   {
    time(&t1);
    time(&t2);
    return 0;
   }
 fps++;
 time(&t2);
 if (t2 > t1)
   {
    t1 = t2;
    fp = fps;
    fps = 0;
    return fp;
   }
 return 0;
}


void make_hsv(double im, double* r, double* g, double *b, double yrange)
{
 im = (im / yrange) + .5;
 im -= floor(im);
 if (im >= 0. && im < .1666666667)
 {
     *r = 1.;
     *g = (im - 0.) * 6.;	/* G up */
     *b = 0.;
 }
 else if (im >= .1666666667 && im < .3333333333)
 {
     *r = (.3333333333 - im) * 6.; /* R down */
     *g = 1.;
     *b = 0.;
 }
 else if (im >= .3333333333 && im < .5)
 {
     *r = 0.;
     *g = 1.;
     *b = (im - .3333333333) * 6.; /* B up */
 }
 else if (im >= .5 && im < .6666666667)
 {
     *r = 0.;
     *g = (.6666666667 - im) * 6.; /* G down */
     *b = 1.;
 }
 else if (im >= .6666666667 && im <= .8333333333)
 {
     *r = (im - .6666666667) * 6; /* R up */
     *g = 0.;
     *b = 1.;
 }
 else if (im >= .8333333333 && im <= 1.)
 {
     *r = 1.;
     *g = 0.;
     *b = (1. - im) * 6.;	/* B down */
 }
 else 
 {
     *r = *b = *g = 0.;
 }

}



void display(void)
{
  GLdouble scalex,scalez,scaley,miny,maxy,minyr,maxyr,minyi,maxyi,mx,my,mz,t,tstep,u,ustep;
  int fp;
  cldouble xx[4], yy[4];
  GLdouble rr[4], gg[4], bb[4], yrange;
  static int rot=0;
  if (use_sy) 
  {
      maxy = ey;
      miny = sy;
      yrange = fabs( ey - sy );
      if (yrange == 0.0) yrange = .0001;
  }
  else
  {
   minmax_compute(&minyr, &maxyr, &minyi, &maxyi);
   miny = (minyr < minyi) ? minyr : minyi;
   maxy = (maxyr > maxyi) ? maxyr : maxyi;
   yrange = fabs( maxyi - minyi );
   if (yrange == 0.0) yrange = .0001;
  }
  if (miny == maxy) 
    {
     u = (ex+ez-sx-sz)/8.;
     maxy += u;
     miny -= u;
    }
  scalex = 3./(ex-sx);
  scaley = 3./(maxy-miny);
  scalez = 3./(ez-sz);
  mx = (sx+ex)/2.;
  my = (miny+maxy)/2.;
  mz = (sz+ez)/2.;
  tstep = (ex-sx)/(GLdouble)nx;
  ustep = (ez-sz)/(GLdouble)nz;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
  gluLookAt(0.,0.,5.,0.,0.,0.,0.,1.,0.);
  glRotated((double)(rot++), 1., 1., 1.);
  glScaled(scalex, scaley, scalez);
  glTranslated(-mx, -my, -mz);
  if ((fp=time_counter()) != 0) sprintf(win_title, "FPS%d x(%3.3f,%3.3f),y(%3.3f,%3.3f),z(%3.3f,%3.3f),X(n=%d,d=%3.3f,l=%3.3f),Z(n=%d,d=%3.3f,l=%3.3f)",fp,sx,ex,miny,maxy,sz,ez,nx,dx,ex-sx,nz,dz,ez-sz);
  glutSetWindowTitle(win_title);
  glColor3f(1.,1.,1.);
  /*glBegin(GL_LINE_STRIP);*/
  glBegin(GL_TRIANGLES);
  for (t=sx;t<ex;t+=tstep) 
  for (u=sz;u<ez;u+=ustep) 
    {
     xx[0] = cldouble(t, u);
     yy[0] = fpar_f(0, xx[0], NULL, NULL, NULL);
     make_hsv(yy[0].imag(), &rr[0], &gg[0], &bb[0], yrange);
     xx[1] = cldouble(t+tstep, u);
     yy[1] = fpar_f(0, xx[1], NULL, NULL, NULL);
     make_hsv(yy[1].imag(), &rr[1], &gg[1], &bb[1], yrange);
     xx[2] = cldouble(t, u+ustep);
     yy[2] = fpar_f(0, xx[2], NULL, NULL, NULL);
     make_hsv(yy[2].imag(), &rr[2], &gg[2], &bb[2], yrange);
     xx[3] = cldouble(t+tstep, u+ustep);
     yy[3] = fpar_f(0, xx[3], NULL, NULL, NULL);
     make_hsv(yy[3].imag(), &rr[3], &gg[3], &bb[3], yrange);

     glColor3d(rr[0], gg[0], bb[0]);
     glVertex3d(t, yy[0].real(), u);
     glColor3d(rr[1], gg[1], bb[1]);
     glVertex3d(t+tstep, yy[1].real(), u);
     glColor3d(rr[3], gg[3], bb[3]);
     glVertex3d(t+tstep, yy[3].real(), u+ustep);

     glColor3d(rr[0], gg[0], bb[0]);
     glVertex3d(t, yy[0].real(), u);
     glColor3d(rr[3], gg[3], bb[3]);
     glVertex3d(t+tstep, yy[3].real(), u+ustep);
     glColor3d(rr[2], gg[2], bb[2]);
     glVertex3d(t, yy[2].real(), u+ustep);


/*     
     glColor3d(0., 0., 1.);
     glVertex3d(t, yy[0].imag(), u);
     glVertex3d(t+tstep, yy[1].imag(), u);
     glVertex3d(t, yy[0].imag(), u);
     glVertex3d(t, yy[2].imag(), u+ustep);
     glVertex3d(t, yy[0].imag(), u);
     glVertex3d(t+tstep, yy[3].imag(), u+ustep);
*/     

    }
  glEnd();
  opaque(miny, maxy);
  if (sx < 0. && ex > 0.)
    {
     glColor3f(1.,0.,0.);
     glBegin(GL_LINES);
	   glVertex3d(0., miny, 0.);
	   glVertex3d(0., maxy, 0.);
     glEnd();
    }
  if (sz < 0. && ez > 0.)
    {
     glColor3f(0.,1.,0.);
     glBegin(GL_LINES);
	   glVertex3d(0., 0., sz);
	   glVertex3d(0., 0., ez);
     glEnd();
    }
  if (miny < 0. && maxy > 0.)
    {
     glColor3f(0.,0.,1.);
     glBegin(GL_LINES);
	   glVertex3d(sx, 0., 0.);
	   glVertex3d(ex, 0., 0.);
     glEnd();
    }
  glFlush();
  glutSwapBuffers();
}


int main(int argc, char** argv)
{
  if (argc < 8) { printf("Function definition not provided!\n%s 'func(x)' sx ex sz ez sy ey\n",argv[0]); fpar_info(); return 1; }
  sx = atof(argv[2]);
  ex = atof(argv[3]);
  sz = atof(argv[4]);
  ez = atof(argv[5]);
  sy = atof(argv[6]);
  ey = atof(argv[7]);
  Init(argv[1]);
  Help();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(1024, 768);
  glutInitWindowPosition(0, 0);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutVisibilityFunc(visible);
  glutMainLoop();
  return 0;
}

