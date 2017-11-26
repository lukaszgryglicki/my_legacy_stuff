#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include "parser.h"

#define real long double

const real a = 4.905;
const real b = 0.156;
real *ttab, *ytab, *yptab, *ztab, *zptab;
double angX, angY, angZ, obsD, tStep, scale;	
double tx, ty, tz;
int wx,wy;		
int cx,cy;
int Nt, ct, cstep;

void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
	case 'w': angY += 1.; break;
	case 's': angY -= 1.; break;
	case 'a': angX += 1.; break;
	case 'd': angX -= 1.; break;
	case 'e': angZ += 1.; break;
	case 'x': angZ -= 1.; break;
	case '1': scale /= 1.02; break;
	case '2': scale *= 1.02; break;
	case 'i': ty += 0.05*scale; break;
	case 'k': ty -= 0.05*scale; break;
	case 'j': tx += 0.05*scale; break;
	case 'l': tx -= 0.05*scale; break;
	case 'o': tz += 0.05*scale; break;
	case 'm': tz -= 0.05*scale; break;
   }
}

void resize_scene(int w, int h)
{
 wx = w;
 wy = h;
 glViewport(0, 0, (GLsizei)wx, (GLsizei)wy);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-5, 5, -5, 5, 0., 2000000.);
 glMatrixMode(GL_MODELVIEW);
}

void render_integral_curve()
{
 int i;
 glColor3d(0., 1., 0.);
 glBegin(GL_LINE_STRIP);
 for (i=0;i<=Nt;i++)
   {
    glVertex3d(ttab[i], ytab[i], ztab[i]);
   }
 glEnd();
 glColor3d(0.2, 0.2, 0.2);
 glBegin(GL_LINE_STRIP);
 for (i=0;i<=Nt;i++)
   {
    glVertex3d(ttab[i], yptab[i], zptab[i]);
   }
 glEnd();
}

void render_object()
{
 glColor3d(1., 0., 0.);
 glPushMatrix();
 glTranslated(ttab[ct], ytab[ct], ztab[ct]);
 glutWireSphere(.1/scale, 8, 8);
 glPopMatrix();
}

void render_scene(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_BLEND);
 glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
 glLoadIdentity();
 gluLookAt(0., 0., 10., 0. , 0., 0., 0., 1., 0.);
 glRotated(angX, 1., 0., 0.);
 glRotated(angY, 0., 1., 0.);
 glRotated(angZ, 0., 0., 1.);
 glTranslated(-ttab[Nt]/2., 0., 0.);
 glTranslated(tx, ty, tz);
 glScaled(scale, scale, scale);
 render_integral_curve();
 render_object();
 glFlush();
 glutSwapBuffers();
 ct += cstep;
 if (ct == Nt) { cstep = -1; }
 if (ct == 0)  { cstep = 1; }
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

void Init()
{
 scale = 1.;
 ct = 0;
 cstep = 1;
 angX = angZ = tx = ty = tz = 0.;
 angY = 0.;
 glClearColor(0.,0.,0.,0.);
 glShadeModel(GL_SMOOTH);
}

void opengl_display()
{
 int dlb;
 char** dpar;
 dlb = 1;
 dpar = (char**)malloc(sizeof(char*));
 dpar[0] = (char*)malloc(sizeof(char)*10);
 strcpy(dpar[0], "nsolver");
 wx = 500;
 wy = 500;
 glutInit(&dlb, dpar);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(1, 1);
 glutCreateWindow("Differential solver");
 Init();
 glutDisplayFunc(render_scene);
 glutReshapeFunc(resize_scene);
 glutKeyboardFunc(keyboard);
 glutVisibilityFunc(visible);
 glutMainLoop();
}

real friction_pendulum(real x, real y, real yp)
{
 return -a*sin(y)-b*yp;
}

real equation1(real x, real y, real yp, real z, real zp)
{
 return -y + zp;
}

real equation2(real x, real y, real yp, real z, real zp)
{
 return -zp + yp;
}

real parser_func1(real x, real y, real yp, real z, real zp)
{
 return fpar5_f(0, x, y, yp, z, zp);
}

real parser_func2(real x, real y, real yp, real z, real zp)
{
 return fpar5_f(1, x, y, yp, z, zp);
}

void runge_kutta_dsystem_single_step(
	real (*func1)(real, real, real, real, real),
	real (*func2)(real, real, real, real, real),
	real  x0, 
	real  y0, real  yp0, 
	real  z0, real  zp0, 
	real h,
	real* x1, 
	real* y1, real* yp1,
	real* z1, real* zp1
	)
{
 real k1,k2,k3,k4;
 real l1,l2,l3,l4;
 real kk0, kk1;
 real ll0, ll1;
 real h2;

 h2 = h/2.L;

 k1 = h * h2 * func1(x0, y0, yp0, z0, zp0);
 l1 = h * h2 * func2(x0, y0, yp0, z0, zp0);
 
 k2 = h * h2 * func1(x0+h2, y0+h2*yp0+k1/4.L, yp0+k1/h, z0+h2*zp0+l1/4.L, zp0+l1/h);
 l2 = h * h2 * func2(x0+h2, y0+h2*yp0+k1/4.L, yp0+k1/h, z0+h2*zp0+l1/4.L, zp0+l1/h);
 
 k3 = h * h2 * func1(x0+h2, y0+h2*yp0+k1/4.L, yp0+k2/h, z0+h2*zp0+l1/4.L, zp0+l2/h);
 l3 = h * h2 * func2(x0+h2, y0+h2*yp0+k1/4.L, yp0+k2/h, z0+h2*zp0+l1/4.L, zp0+l2/h);
 
 k4 = h * h2 * func1(x0+h, y0+h*yp0+k3, yp0+(2.L*k3)/h, z0+h*zp0+l3, zp0+(2.L*l3)/h);
 l4 = h * h2 * func2(x0+h, y0+h*yp0+k3, yp0+(2.L*k3)/h, z0+h*zp0+l3, zp0+(2.L*l3)/h);

 kk0 = (k1+k2+k3)/3.L;
 kk1 = (k1+(2.L*k2)+(2.L*k3)+k4)/3.L;
 
 ll0 = (l1+l2+l3)/3.L;
 ll1 = (l1+(2.L*l2)+(2.L*l3)+l4)/3.L;

/* printf("k1=%Lf, k2=%Lf, k3=%Lf, k4=%Lf\n", k1, k2, k3, k4);
 printf("kk0=%Lf, kk1=%Lf\n", kk0, kk1);
 printf("l1=%Lf, l2=%Lf, l3=%Lf, l4=%Lf\n", l1, l2, l3, l4);
 printf("ll0=%Lf, ll1=%Lf\n", ll0, ll1);*/

 *x1  = x0 + h;
 *y1  = y0 + h*yp0 + kk0;
 *yp1 = yp0 + kk1/h;
 *z1  = z0 + h*zp0 + ll0;
 *zp1 = zp0 + ll1/h;

/* printf("t=%Lf, y(t)=%Lf, y'(t)=%Lf, z(t)=%Lf, z'(t)=%Lf\n", *x1, *y1, *yp1, *z1, *zp1);*/
}

void runge_kutta_single_step(
	real (*func)(real, real, real),
	real  x0, real  y0, real  yp0, 
	real h,
	real* x1, real* y1, real* yp1)
{
 real k1,k2,k3,k4;
 real kk0, kk1;
 real h2;

 h2 = h/2.L;

 k1 = h * h2 * func(x0, y0, yp0);
 k2 = h * h2 * func(x0+h2, y0+h2*yp0+k1/4.L, yp0+k1/h);
 k3 = h * h2 * func(x0+h2, y0+h2*yp0+k1/4.L, yp0+k2/h);
 k4 = h * h2 * func(x0+h, y0+h*yp0+k3, yp0+(2.L*k3)/h);

 kk0 = (k1+k2+k3)/3.L;
 kk1 = (k1+(2.L*k2)+(2.L*k3)+k4)/3.L;

/* printf("k1=%Lf, k2=%Lf, k3=%Lf, k4=%Lf\n", k1, k2, k3, k4);*/
/* printf("kk0=%Lf, kk1=%Lf\n", kk0, kk1);*/

 *x1  = x0 + h;
 *y1  = y0 + h*yp0 + kk0;
 *yp1 = yp0 + kk1/h;

/* printf("%Lf, %Lf, %Lf\n", *x1, *y1, *yp1);*/
}

void runge_kutta_nsolve(
	real (*f)(real, real, real),
	real x0, real x1,
	real y0, real yp0,
	real h, int* nn,
	real** rx, real** ry, real** ryp
	)
{
 int n,i;
 real *resX, *resY, *resYP;
 n = (int)((x1 - x0)/h);
/* printf("steps: %d\n", n);*/
 if (n <= 0) 
   {
    *nn = 0;
    return;
   }
 resX  = (real*)malloc((n+1)*sizeof(real));
 resY  = (real*)malloc((n+1)*sizeof(real));
 resYP = (real*)malloc((n+1)*sizeof(real));
 if (!resX || !resY || !resYP)
   {
    *nn = 0;
    return;
   }
 resX[0] = x0;
 resY[0] = y0;
 resYP[0] = yp0;
 for (i=1;i<=n;i++)
   {
    runge_kutta_single_step(f, 
	    resX[i-1], resY[i-1], resYP[i-1], 
	    h, 
	    &resX[i], &resY[i], &resYP[i]);
   }
 *rx  = resX;
 *ry  = resY;
 *ryp = resYP;
 *nn = n;
}

void runge_kutta_nsolve_dsystem(
	real (*f1)(real, real, real, real, real),
	real (*f2)(real, real, real, real, real),
	real x0, real x1,
	real y0, real yp0,
	real z0, real zp0,
	real h, int* nn,
	real** rx, 
	real** ry, 
	real** ryp,
	real** rz, 
	real** rzp
	)
{
 int n,i;
 real *resX;
 real *resY, *resYP;
 real *resZ, *resZP;
 n = (int)((x1 - x0)/h);
/* printf("steps: %d\n", n);*/
 if (n <= 0) 
   {
    *nn = 0;
    return;
   }
 resX  = (real*)malloc((n+1)*sizeof(real));
 resY  = (real*)malloc((n+1)*sizeof(real));
 resYP = (real*)malloc((n+1)*sizeof(real));
 resZ  = (real*)malloc((n+1)*sizeof(real));
 resZP = (real*)malloc((n+1)*sizeof(real));
 if (!resX || !resY || !resYP || !resZ || !resZP)
   {
    *nn = 0;
    return;
   }
 resX[0]  = x0;
 resY[0]  = y0;
 resYP[0] = yp0;
 resZ[0]  = z0;
 resZP[0] = zp0;
 for (i=1;i<=n;i++)
   {
    runge_kutta_dsystem_single_step(f1, f2,
	    resX[i-1], 
	    resY[i-1], resYP[i-1], 
	    resZ[i-1], resZP[i-1], 
	    h, 
	    &resX[i], 
	    &resY[i], &resYP[i],
	    &resZ[i], &resZP[i]
	    );
   }
 *rx  = resX;
 *ry  = resY;
 *ryp = resYP;
 *rz  = resZ;
 *rzp = resZP;
 *nn = n;
}

void nsolve1()
{
 int n;
 real *ttab, *ytab, *yptab;
 runge_kutta_nsolve(
	 friction_pendulum, /* what solve */
	 0, 10., 	    /* time range */
	 M_PI/2., 0,        /* y(0), y'(0) */
	 0.001,		    /* h: integral step */
	 &n, &ttab, &ytab, &yptab /* ilosc el i tablice x,y,y' */
	 );
}

void nsolve2()
{
 int n;
 real *ttab, *ytab, *yptab, *ztab, *zptab;
 runge_kutta_nsolve_dsystem(
	 equation1, equation2, /* what solve */
	 0, 10., 	    /* time range */
	 1., 0.,        /* y(0), y'(0) */
	 0., 0.,        /* y(0), y'(0) */
	 0.001,		    /* h: integral step */
	 &n, &ttab, 	/* time and taime array */
	 &ytab, &yptab,
	 &ztab, &zptab
	 );
}

void parser_ndsolver(real t0, real t1, 
		real y0, real yp0,
		real z0, real zp0,
		real h)
{
/* real *ttab, *ytab, *yptab, *ztab, *zptab;*/
 runge_kutta_nsolve_dsystem(
	 parser_func1, parser_func2, /* what solve */
	 t0, t1, 	    /* time range */
	 y0, yp0,        /* y(0), y'(0) */
	 z0, zp0,        /* z(0), z'(0) */
	 h,		    /* h: integral step */
	 &Nt, &ttab, 	/* time and taime array */
	 &ytab, &yptab,
	 &ztab, &zptab
	 );
 opengl_display();
}

void help()
{
 printf("parameters: func_def1 func_def2 t0 t1 y0 yp0 z0 zp0 h\n");
 printf("Example: You want to solve 2 differential equations:\n");
 printf("y''=z'+y-t and z''=z'+y'-cos(z)-sin(y)\n");
 printf("y(0)=1, z(0)=0, y'(0)=-1 z'(0)=2\n");
 printf("in t=[0,5], timestep: 0.01, then:\n");
 printf("Solver will give You y(t) and y'(t) in [t0,t1] with step=h\n");
 printf("./nsolve 'zp-y-t' 'zp-yp-cos(z)-sin(y)' 0 5 1 -1 0 2 0.01\n");
}

int main(int lb, char** par)
{
/* nsolve1();*/
/* nsolve2();*/
 if (lb < 10) { help(); return 1; }
 fpar_init(2);
 fpar_function(0, par[1]);
 fpar_function(1, par[2]);
 if (!fpar5_ok(0) || !fpar5_ok(1))
   {
    printf("Errors in function definitions.\n");
    return 2;
   }
/* printf("%Lf %Lf\n", fpar5_f(0, 1, 2, 3, 4, 5), fpar5_f(1, 10, 20, 30, 40, 50));*/
 parser_ndsolver(
	 atof(par[3]),	/*  t0 */
	 atof(par[4]),	/*  t1 */
	 atof(par[5]),	/*  y0 */
	 atof(par[6]),	/*  yp0 */
	 atof(par[7]),	/*  z0 */
	 atof(par[8]),	/*  zp0 */
	 atof(par[9])	/*  h */
	     );
 return 0;
}

