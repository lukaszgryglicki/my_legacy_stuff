#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <complex>
#include <iostream>

using namespace std;

GLdouble sx, ex, dx;
GLdouble defsx = -1.;
GLdouble defex = 1.;
GLint nx;

GLdouble sy, ey, dy;
GLdouble defsy = -1.;
GLdouble defey = 1.;
GLint ny;

GLdouble ax,ay,az,scl;
GLdouble Minv, Maxv;

bool invert;

char win_title[256];

typedef complex<double> Complex;

Complex fpar_f(Complex x)
{
/* return exp(x);*/
 return pow(x, x);
}

void Init()
{
 sx = defsx;
 ex = defex;
 nx = 100;
 dx = .03;

 scl = 1.;

 invert = false;

 ax = 300;
 ay = 0;
 
 sy = defsy;
 ey = defey;
 ny = 100;
 dy = .03;
}

void Help()
{
 printf("Kolory: czerwona: os OY, niebieska: os OX, zielona: f(t)\n");
 printf("Klawiatura:\n");
 printf("r: swap real/imaginated values\n");
 printf("F: full screen\n");
 printf("Pomoc: h\n");
 printf("Obroty: w/s/a/d/e/x\n");
 printf("Skalowanie: z/c\n");
 printf("Zwiekszanie zmniejszanie o dx:  o/p\n");
 printf("Zwiekszanie zmniejszanie o dy:  u/i\n");
 printf("Zwiekszanie zmniejszanie dx  :  n/m\n");
 printf("Zwiekszanie zmniejszanie dy  :  b/v\n");
 printf("Zwiekszanie zmniejszanie lx  :  k/l\n");
 printf("Zwiekszanie zmniejszanie ly  :  h/j\n");
 printf("Zwiekszanie zmniejszanie nx  :  t/y\n");
 printf("Zwiekszanie zmniejszanie ny  :  f/g\n");
 printf("Przywroc stan poczatkowy: SPACJA\n");
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

void keyboard(unsigned char key, int x, int y)
{
   double l, m;
   Help();
   switch (key)
   {
	  case 27 : case 'q': exit(0); break;
	  case 'r': invert = ! invert; break;
	  case 'c': scl *= 1.05; break;
	  case 'z': scl /= 1.05; break;
	  case 'w': ax += 5.; break;
	  case 's': ax -= 5.; break;
	  case 'a': ay += 5.; break;
	  case 'd': ay -= 5.; break;
	  case 'e': az += 5.; break;
	  case 'x': az -= 5.; break;
	  case 'o': sx -= dx; ex -= dx; break;
	  case 'p': sx += dx; ex += dx; break;
	  case 'u': sy -= dy; ey -= dy; break;
	  case 'i': sy += dy; ey += dy; break;
	  case 'm': dx *= 1.05; break;
	  case 'n': dx /= 1.05; break;
	  case 'b': dy *= 1.05; break;
	  case 'v': dy /= 1.05; break;
	  case 'k': m = (sx+ex)/2; l = ex-sx; l*=1.05; sx = m-.5*l; ex = m+.5*l; break;
	  case 'l': m = (sx+ex)/2; l = ex-sx; l/=1.05; sx = m-.5*l; ex = m+.5*l; break;
	  case 'h': m = (sy+ey)/2; l = ey-sy; l*=1.05; sy = m-.5*l; ey = m+.5*l; break;
	  case 'j': m = (sy+ey)/2; l = ey-sy; l/=1.05; sy = m-.5*l; ey = m+.5*l; break;
	  case 't': nx = (nx*5)/6; if (nx<6) nx = 6; break;
	  case 'y': nx = (nx*6)/5; if (nx>512) nx = 512; break;
	  case 'f': ny = (ny*5)/6; if (ny<6) ny = 6; break;
	  case 'g': ny = (ny*6)/5; if (ny>512) ny = 512; break;
	  case ' ': Init(); break;
	  case 'F': glutFullScreen(); break;
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

void minmax_compute(double* minz, double* maxz, double* minv, double* maxv)
{
 GLdouble tz, tv, tstepz, tstepv, valz, valv;
 Complex val;
 Complex tmp;
 *minz = 9e99;
 *maxz = -9e99;
 *minv = 9e99;
 *maxv = -9e99;
 tstepz = (ex-sx)/(GLdouble)nx;
 tstepv = (ey-sy)/(GLdouble)ny;
 for (tz=sx;tz<=ex;tz+=tstepz)
   {
    for (tv=sy;tv<=ey;tv+=tstepv)
      {
       tmp = Complex(tz, tv);
       val = fpar_f(tmp);
       valz = val.real();
       valv = val.imag();
       if (valz > (*maxz)) *maxz = valz;
       if (valz < (*minz)) *minz = valz;
       if (valv > (*maxv)) *maxv = valv;
       if (valv < (*minv)) *minv = valv;
      }
   }
}

void colorFor(double ang, double* rr, double* gg, double* bb)
{
 float r,g,b;
 ang = ang - Minv;
 ang = 345 * (ang / (Maxv - Minv)) - 10;
 if (ang<0.0)   ang+=360.0;
 if (ang>360.0) ang+=360.0;
 r = ang;
 g = fabs(ang-120.0);
 b = fabs(ang-240.0);
 if (r<=120.0) r=(120.0-r)/120.0;
 else if (r>=240.0) r=(r-240)/120.0;
 else r=0.0;
 if (g>=120.0) g=0.0; else g=(120.0-g)/120.0;
 if (b>=120.0) b=0.0; else b=(120.0-b)/120.0;
 r += 0.16;
 g += 0.16;
 b += 0.16;
 if (r > 1.) r = 1.;
 if (g > 1.) g = 1.;
 if (b > 1.) b = 1.;
 *rr = r;
 *gg = g;
 *bb = b;
}

void display(void)
{
  GLdouble scalex,scaley,scalez,scalev,minz,maxz,minv,maxv,mx,my,mz,mv,tx,ty,tstepx,tstepy;
  Complex val1,val2,val3,val4;
  GLdouble r1,g1,b1;
  GLdouble r2,g2,b2;
  GLdouble r3,g3,b3;
  GLdouble r4,g4,b4;
  GLdouble v1r, v2r, v3r, v4r;
  GLdouble v1i, v2i, v3i, v4i;
  
  minmax_compute(&minz, &maxz, &minv, &maxv);

  Minv = minv;
  Maxv = maxv;
  
  scalex = 2./(ex-sx);
  scaley = 2./(ey-sy);
  scalez = 2./(maxz-minz);
  scalev = 2./(maxv-minv);

  /*
  printf("scaler: %lf %lf %lf %lf\n", scalex, scaley, scalez, scalev);
  printf("min:    %lf %lf %lf %lf\n", sx, sy, minz, minv);
  printf("max:    %lf %lf %lf %lf\n", ex, ey, maxz, maxv);
  */
  
  mx = (sx+ex)/2.;
  my = (sy+ey)/2.;
  mz = (minz+maxz)/2.;
  mv = (minv+maxv)/2.;
  
  tstepx = (ex-sx)/(GLdouble)nx;
  tstepy = (ey-sy)/(GLdouble)ny;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();
  gluLookAt(0.,0.,3.34,0.,0.,0.,0.,1.,0.);

  glRotated(ax, 1., 0., 0.);
  glRotated(ay, 0., 1., 0.);
  glRotated(az, 0., 0., 1.);
  
  glScaled(scalex, scaley, scalez);
  glTranslated(-mx, -my, -mz);

  glScaled(scl, scl, scl);

  glBegin(GL_TRIANGLES);
  
  for (tx=sx;tx<ex;tx+=tstepx) 
    {
     for (ty=sy;ty<ey;ty+=tstepy) 
       {
	val1 = fpar_f(Complex(tx, ty));
	val2 = fpar_f(Complex(tx+tstepx, ty));
	val3 = fpar_f(Complex(tx+tstepx, ty+tstepy));
	val4 = fpar_f(Complex(tx, ty+tstepy));

	if (invert)
	  {
	   v1r = val1.imag();
	   v2r = val2.imag();
	   v3r = val3.imag();
	   v4r = val4.imag();
	
	   v1i = val1.real();
	   v2i = val2.real();
	   v3i = val3.real();
	   v4i = val4.real();
	  }
	else
	  {
	   v1i = val1.imag();
	   v2i = val2.imag();
	   v3i = val3.imag();
	   v4i = val4.imag();
	
	   v1r = val1.real();
	   v2r = val2.real();
	   v3r = val3.real();
	   v4r = val4.real();
	  }
	
	colorFor(v1i, &r1, &g1, &b1);
	colorFor(v2i, &r2, &g2, &b2);
	colorFor(v3i, &r3, &g3, &b3);
	colorFor(v4i, &r4, &g4, &b4);

	glColor3d(r1, g1, b1);
        glVertex3d(tx, ty, v1r);
	glColor3d(r2, g2, b2);
        glVertex3d(tx+tstepx, ty, v2r);
	glColor3d(r3, g3, b3);
        glVertex3d(tx+tstepx, ty+tstepy, v3r);
	
	glColor3d(r1, g1, b1);
        glVertex3d(tx, ty, v1r);
	glColor3d(r3, g3, b3);
        glVertex3d(tx+tstepx, ty+tstepy, v3r);
	glColor3d(r4, g4, b4);
        glVertex3d(tx, ty+tstepy, v4r);
       }
    }
  glEnd();

  glColor3d(0.6, 0.6, 0.6);
  
  for (tx=sx;tx<ex;tx+=tstepx) 
    {
     for (ty=sy;ty<ey;ty+=tstepy) 
       {
	glBegin(GL_LINE_STRIP);
	
	val1 = fpar_f(Complex(tx, ty));
	val2 = fpar_f(Complex(tx+tstepx, ty));
	val3 = fpar_f(Complex(tx+tstepx, ty+tstepy));
	val4 = fpar_f(Complex(tx, ty+tstepy));

	if (invert)
	  {
	   v1i = val1.real();
	   v2i = val2.real();
	   v3i = val3.real();
	   v4i = val4.real();
	  }
	else
	  {
	   v1i = val1.imag();
	   v2i = val2.imag();
	   v3i = val3.imag();
	   v4i = val4.imag();
	  }
        glVertex3d(tx, ty, v1i);
        glVertex3d(tx+tstepx, ty, v2i);
        glVertex3d(tx+tstepx, ty+tstepy, v3i);
        glVertex3d(tx, ty+tstepy, v4i);
	
	glEnd();
       }
    }
//  glTranslated(-mx, -my, 0.);
//  sprintf(win_title, "x1=%3.3f, x2=%3.3f, minY=%3.3f, maxY=%3.3f, nx=%d, dx=%3.3f, len=%3.3f",sx,ex,miny,maxy,nx,dx,ex-sx);
//  glutSetWindowTitle(win_title);
  glBegin(GL_LINES);
    glColor3d(1., 0., 0.);
    glVertex3d(sx, 0., 0.);
    glVertex3d(ex, 0., 0.);
    
    glColor3d(0., 1., 0.);
    glVertex3d(0., sy, 0.);
    glVertex3d(0., ey, 0.);
    
    glColor3d(0., 0., 1.);
    glVertex3d(0., 0., minz);
    glVertex3d(0., 0., maxz);
  glEnd();
  glFlush();
  glutSwapBuffers();
}


int main(int argc, char** argv)
{
  Init();
  Help();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutVisibilityFunc(visible);
  glutMainLoop();
  return 0;
}

