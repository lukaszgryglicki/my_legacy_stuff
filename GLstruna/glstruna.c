#define nmax            2048
#define mmax            2048
#define wy               256
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

typedef double tab[nmax+1];
long n, m, i, j, wx;
long ny2, zero;
double a, a2, l, tt, t, h, tau, t2;
double roz[nmax+1][mmax+1];
tab xi, c, d, aa, ab, ac, ba, bb, bc, ca, cb, cc, af, ax;
int gargc;
char** gargv;
long jjj;
int verbose;
long FORLIM, FORLIM1;
int control;
int stopped;

double f(double x)
{
  if (x < 1.0) return (x/2.0);
  else return (2-x);
}

double g(double x)
{
  return 0.0;
}

double v(long i, double x)
{
  if (i == 0)
    return (1 - x / h);
  else if (i == n)
    return ((x - l) / h + 1);
  else {
    if (x >= xi[i-1] && x <= xi[i]) {
      return ((x - xi[i-1]) / h);
    } else if (x >= xi[i] && x <= xi[i+1]) {
      return ((xi[i+1] - x) / h);
    } else {
      return 0.0;
    }
  }
}


double dv(long i, double x)
{
  if (i == 0)
    return (-1 / h);
  else if (i == n)
    return (1 / h);
  else {
    if (x >= xi[i-1] && x < xi[i]) {
      return (1 / h);
    } else if (x > xi[i] && x <= xi[i+1]) {
      return (-1 / h);
    } else {
      return 0.0;
    }
  }
}


double uu(double x)
{
  long i;
  i = (long)(x / h);
  if (i == n)
    return (ax[n]);
  else {
    return (((ax[i+1] - ax[i]) * x + xi[i+1] * ax[i] - xi[i] * ax[i+1]) / h);
  }
}

double gg(long k, long i, long j, double x)
{
  double Result;
  Result = 0.;
  switch (k) {
  case 0:
    Result = dv(i, x) * dv(j, x);
    break;
  case 1:
    Result = v(i, x) * v(j, x);
    break;
  case 2:
    Result = f(x) * v(i, x);
    break;
  case 3:
    Result = g(x) * v(i, x);
    break;
  }
  return Result;
}

static double calka(long k, long i, long j, double alfa_, double beta)
{
  double tau, s, t, a1, a2;
  tau = (beta - alfa_) / 2;
  s = (beta + alfa_) / 2;
  t = tau * sqrt(3.0) / 3;
  a1 = s - t;
  a2 = s + t;
  return (tau * (gg(k, i, j, a1) + gg(k, i, j, a2)));
}

void trojdiag(long n, double* a_, double* b, double* c, double* f_, double* x)
{
  tab a, f;
  long k;
  double m;
  memcpy(a, a_, sizeof(tab));
  memcpy(f, f_, sizeof(tab));
  for (k = 2; k <= n; k++) {
    m = b[k] / a[k-1];
    a[k] -= m * c[k-1];
    f[k] -= m * f[k-1];
  }
  x[n] = f[n] / a[n];
  for (k = n - 1; k >= 1; k--) {
    x[k] = (f[k] - c[k] * x[k+1]) / a[k];
  }
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
   switch (key)
   {
      case 27 : exit(0); break;
      case 'q': exit(0); break;
      case ' ': stopped = !stopped; break;
   }
}

void anim_strunka(void)
{
 if (stopped) return;
 if (jjj < FORLIM) jjj ++;
 else jjj = 0;
 glutPostRedisplay();
}

void visible_strunka(int vis)
{
  if (vis == GLUT_VISIBLE)    glutIdleFunc(anim_strunka);
  else    glutIdleFunc(NULL);
}

void display_strunka(void)
{
  long j;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glScaled(5./(GLdouble)(FORLIM1*wx), 3./(GLdouble)wy, 1.);
  glTranslated(-((GLdouble)(FORLIM1*wx))/2., -((GLdouble)wy)/2., 0.);
  if (control > 1)
    {
     glEnable(GL_LINE_SMOOTH);
     glLineWidth((GLfloat)control);
    }
  j = jjj;
  zero = 160;
  ny2 = zero - 50;
  wx = (long)(nmax / n); /* was 200 / n */
  FORLIM = m;
  FORLIM1 = n;
  for (i = 0; i < FORLIM1; i++) 
     {
	glBegin(GL_LINES);
	  glColor3f((GLdouble)i/(GLdouble)FORLIM, ((GLdouble)(FORLIM-i))/(GLdouble)FORLIM, roz[i][j]);
	  glVertex3d((GLdouble)(wx*i), (GLdouble)(ny2 - (long)floor(wy * roz[i][j] + 0.5)), 0.);
	  glVertex3d((GLdouble)(wx*(i+1)), (GLdouble)(ny2 - (long)floor(wy * roz[i+1][j] + 0.5)), 0.);
	glEnd();
     }
  glFlush();
  glutSwapBuffers();
}

void strunka_gl(long FORLIM, long FORLIM1)
{
  jjj = 0;
  stopped = 0;
  glutInit(&gargc, gargv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(gargv[0]);
  glutDisplayFunc(display_strunka);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutVisibilityFunc(visible_strunka);
  glutMainLoop();
}

void rozniczkuj()
{
    h = l / n;
    tau = tt / m;
    t2 = tau * tau / 2;
    a2 = a * a;
    FORLIM = n;
    for (i = 0; i < FORLIM; i++)
      xi[i] = i * h;
    xi[n] = l;
    FORLIM = n;
    for (i = 2; i < FORLIM; i++)
      ab[i] = a2 * calka(0L, i - 1, i, xi[i-1], xi[i]);
    FORLIM = n;
    for (i = 1; i < FORLIM; i++) {
      aa[i] = a2 * (calka(0L, i, i, xi[i-1], xi[i]) + calka(0L, i, i, xi[i],xi[i+1]));
    }
    FORLIM = n - 2;
    for (i = 1; i <= FORLIM; i++)
      ac[i] = a2 * calka(0L, i + 1, i, xi[i], xi[i+1]);
    FORLIM = n;
    for (i = 2; i < FORLIM; i++)
      bb[i] = calka(1L, i - 1, i, xi[i-1], xi[i]);
    FORLIM = n;
    for (i = 1; i < FORLIM; i++) {
      ba[i] = calka(1L, i, i, xi[i-1], xi[i]) + calka(1L, i, i, xi[i], xi[i+1]);
    }
    FORLIM = n - 2;
    for (i = 1; i <= FORLIM; i++)
      bc[i] = calka(1L, i + 1, i, xi[i], xi[i+1]);
    FORLIM = n;
    for (i = 1; i < FORLIM; i++) {
      c[i] = calka(2L, i, i, xi[i-1], xi[i]) + calka(2L, i, i, xi[i], xi[i+1]);
    }
    FORLIM = n;
    for (i = 2; i < FORLIM; i++)
      cb[i] = bb[i] + t2 * ab[i];
    FORLIM = n;
    for (i = 1; i < FORLIM; i++)
      ca[i] = ba[i] + t2 * aa[i];
    FORLIM = n - 2;
    for (i = 1; i <= FORLIM; i++)
      cc[i] = bc[i] + t2 * ac[i];
    FORLIM = n;
    for (i = 0; i <= FORLIM; i++)
      roz[i][0] = f(xi[i]);
    if (verbose) {
      FORLIM = n;
      for (i = 0; i <= FORLIM; i++)
	ax[i] = roz[i][0];
      t = 0.0;
      printf("WARTOSC t=%8.4f\n", t);
      printf("NR WEZLA      WARTOSC x       ROZWIAZANIE\n");
      FORLIM = n;
      for (i = 0; i <= FORLIM; i++) {
	printf("%5ld    %12.6f      %12.6f\n", i, xi[i], ax[i]);
      }
      printf("\nCZY RYSOWAC WYKRES ROZWIAZANIA ? (T/N) ");
      putchar('\n');
    }
    af[1] = ba[1] * roz[1][0] + bc[1] * roz[2][0] + tau * d[1];
    FORLIM = n - 2;
    for (i = 2; i <= FORLIM; i++) {
      af[i] = bb[i] * roz[i-1][0] + ba[i] * roz[i][0] + bc[i] * roz[i+1][0] +
	      tau * d[i];
    }
    af[n-1] = bb[n-1] * roz[n-2][0] + ba[n-1] * roz[n-1][0] + tau * d[n-1];
    trojdiag(n - 1, ba, bb, bc, af, ax);
    ax[0] = 0.0;
    ax[n] = 0.0;
    FORLIM = n;
    for (i = 0; i <= FORLIM; i++)
      roz[i][1] = ax[i];
    if (verbose) {
      t = tau;
      printf("WARTOSC t=%8.4f\n", t);
      FORLIM = n;
      for (i = 0; i <= FORLIM; i++) {
	printf("%5ld    %12.6f      %12.6f\n", i, xi[i], ax[i]);
      }
    }
    FORLIM = m;
    for (j = 1; j < FORLIM; j++) {
      af[1] = 2 * (ba[1] * roz[1][j] + bc[1] * roz[2][j]) - ca[1] * roz[1]
		[j-1] - cc[1] * roz[2][j-1];
      FORLIM1 = n - 2;
      for (i = 2; i <= FORLIM1; i++) {
	af[i] = 2 * (bb[i] * roz[i-1][j] + ba[i] * roz[i][j] + bc[i] * roz[i+1]
		       [j]) - cb[i] * roz[i-1][j-1] - ca[i] * roz[i][j-1] -
		cc[i] * roz[i+1][j-1];
      }
      af[n-1] = 2 * (bb[n-1] * roz[n-2][j] + ba[n-1] * roz[n-1][j]) -
		cb[n-1] * roz[n-2][j-1] - ca[n-1] * roz[n-1][j-1];
      trojdiag(n - 1, ca, cb, cc, af, ax);
      ax[0] = 0.0;
      ax[n] = 0.0;
      FORLIM1 = n;
      for (i = 0; i <= FORLIM1; i++)
	roz[i][j+1] = ax[i];
      if (verbose) {
	t = (j + 1) * tau;
	printf("WARTOSC t=%8.4f\n", t);
	printf("NR WEZLA      WARTOSC x       ROZWIAZANIE\n");
	FORLIM1 = n;
	for (i = 0; i <= FORLIM1; i++) {
	  printf("%5ld    %12.6f      %12.6f\n", i, xi[i], ax[i]);
	}
      }
    }
    strunka_gl(FORLIM, FORLIM1);
}


int main(int argc, char** argv)
{
    gargc = argc;
    gargv = argv;
    if (argc < 7)
      {
	printf("usage: %s [control=int] [a=double] [l=double] [T=double] [N=int] [M=int]\n", argv[0]);
	printf("control = 0 then verbose logging\n");
	printf("control = 1 then fastest version\n");
	printf("control > 1 then line-width = control + antialiasing\n");
	return 1;
      }
    verbose = !atoi(argv[1]);
    control = atoi(argv[1]);
    a = atof(argv[2]);
    l = atof(argv[3]);
    if (l<=0.) return 2;
    tt = atof(argv[4]);
    if (tt <= 0.) return 4;
    n = atoi(argv[5]);
    if (n<2 || n>nmax) return 8;
    m = atoi(argv[6]);
    if (m<2 || m > mmax) return 16;
    rozniczkuj();
    return 0;
}

