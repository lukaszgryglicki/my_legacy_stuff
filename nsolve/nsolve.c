#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include "parser.h"

#define real long double
#define FUNC1 0
#define FUNC2 1
#define FUNC3 2
#define FUNC_MAPX 3
#define FUNC_MAPY 4
#define FUNC_MAPZ 5

const real a = 4.905;
const real b = 0.156;
real *ttab, *ytab, *yptab, *ztab, *zptab, *vtab, *vptab;
real *Xprep, *Yprep, *Zprep;
double angX, angY, angZ, obsD, tStep, scale;	
double tx, ty, tz;
int wx,wy;		
int cx,cy;
int Nt, ct, cstep, npts;
int cskip, allCurve;
FILE* file;

void normals()
{
 scale = 1.;
 ct = 0;
 cskip = 1;
 allCurve = 0;
 cstep = cskip;
 angX = angZ = tx = ty = tz = 0.;
 angY = 0.;
}

void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
	case 'w': angX += 1.; break;
	case 's': angX -= 1.; break;
	case 'a': angY += 1.; break;
	case 'd': angY -= 1.; break;
	case 'e': angZ += 1.; break;
	case 'x': angZ -= 1.; break;
	case '1': scale /= 1.02; break;
	case '2': scale *= 1.02; break;
	case 'i': ty += 0.05*scale; break;
	case 'k': ty -= 0.05*scale; break;
	case 'j': tx -= 0.05*scale; break;
	case 'l': tx += 0.05*scale; break;
	case 'o': tz += 0.05*scale; break;
	case 'm': tz -= 0.05*scale; break;
	case '3': npts = (int)((float)npts / 1.2); break;
	case '4': npts = (int)((float)npts * 1.2); break;
	case '5': cskip--; break;
	case '6': cskip++; break;
	case '7': allCurve = !allCurve; break;
	case ' ': normals(); break;
   }
 if (cskip < 1) cskip = 1;
 if (cskip > Nt/10) cskip = Nt/10;
 if (npts < 5) npts = 5;
 if (npts > Nt) npts = Nt;
 if (cstep > 0) cstep = cskip;
 if (cstep < 0) cstep = -cskip;
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
 int si,ei;
 glBegin(GL_LINE_STRIP);
 if (cstep > 0)
   {
    glColor3d(0., .5, 0.);
    si = ct - npts;
    if (si < 0) si = 0;
    ei = ct;
    if (allCurve) { si = 0; ei = Nt; }
    for (i=si;i<=ei;i++)
      {
/*     glVertex3d(ttab[i], ytab[i], ztab[i]);*/
/*     glVertex3d(cos(a)*sin(b), -cos(b), sin(a)*sin(b));*/
       glVertex3d(Xprep[i], Yprep[i], Zprep[i]);
      }
   }
 else
   {
    glColor3d(0., 0., .5);
    si = ct;
    ei = ct + npts;
    if (ei > Nt) ei = Nt;
    if (allCurve) { si = 0; ei = Nt; }
    for (i=si;i<=ei;i++)
      {
/*     glVertex3d(ttab[i], ytab[i], ztab[i]);*/
/*     glVertex3d(cos(a)*sin(b), -cos(b), sin(a)*sin(b));*/
       glVertex3d(Xprep[i], Yprep[i], Zprep[i]);
      }
   }
 glEnd();
 /*glColor3d(0.2, 0.2, 0.2);
 glBegin(GL_LINE_STRIP);
 for (i=0;i<=Nt;i++)
   {
    glVertex3d(ttab[i], yptab[i], zptab[i]);
   }
 glEnd();*/
}

void render_object()
{
 real x,y,z;
 if (cstep > 0)  glColor3d(.7, .5, 0.);
 else            glColor3d(.7, 0., .5);
 glPushMatrix();
/* glTranslated(ttab[ct], ytab[ct], ztab[ct]);*/
/* glTranslated(x=cos(a)*sin(b), y=-cos(b), z=sin(a)*sin(b));*/
 glTranslated(x=Xprep[ct], y=Yprep[ct], z=Zprep[ct]);
 glutWireSphere(.1/scale, 8, 8);
 glPopMatrix();
 glBegin(GL_LINES);
  glVertex3d(0., 0., 0.);
  glVertex3d(x, y, z);
 glEnd();
}

void render_zero()
{
 glBegin(GL_LINES);
   glColor3d(1., 0., 0.);
   glVertex3d(-.1/scale, 0., 0.);
   glVertex3d(1./scale, 0., 0.);
   
   glColor3d(0., 1., 0.);
   glVertex3d(0., -.1/scale, 0.);
   glVertex3d(0., 1./scale, 0.);
   
   glColor3d(0., 0., 1.);
   glVertex3d(0., 0., -.1/scale);
   glVertex3d(0., 0., 1./scale);
 glEnd();
}

void render_scene(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_BLEND);
 glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
 glLoadIdentity();
 gluLookAt(0., 0., 10., 0. , 0., 0., 0., 1., 0.);
 glTranslated(tx, ty, tz);
 glRotated(angX, 1., 0., 0.);
 glRotated(angY, 0., 1., 0.);
 glRotated(angZ, 0., 0., 1.);
/* glTranslated(-ttab[Nt]/2., 0., 0.);*/
 glScaled(scale, scale, scale);
 render_integral_curve();
 render_object();
 render_zero();
 glFlush();
 glutSwapBuffers();
 ct += cstep;
 if (ct >= Nt) { cstep = -cskip; }
 if (ct <= 0)  { cstep = cskip; }
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

void write_line(int i)
{
 if (file) 
  fprintf(file, "t=%Lf, y(t)=%Lf, y'(t)=%Lf, z(t)=%Lf, z'(t)=%Lf, v(t)=%Lf, v'(t)=%Lf, X(t)=%Lf, Y(t)=%Lf, Z(t)=%Lf\n",
	  ttab[i], ytab[i], yptab[i], ztab[i], zptab[i], vtab[i], vptab[i], Xprep[i], Yprep[i], Zprep[i]);

}

void preprocess_results()
{
 int i;
 Xprep = (real*)malloc((Nt+1)*sizeof(real));
 Yprep = (real*)malloc((Nt+1)*sizeof(real));
 Zprep = (real*)malloc((Nt+1)*sizeof(real));
 for (i=0;i<=Nt;i++)
   {
    Xprep[i] = fpar3_f(FUNC_MAPX, ytab[i], ztab[i], vtab[i]);
    Yprep[i] = fpar3_f(FUNC_MAPY, ytab[i], ztab[i], vtab[i]);
    Zprep[i] = fpar3_f(FUNC_MAPZ, ytab[i], ztab[i], vtab[i]);
    if (file) write_line(i);
   }
 if (file) fclose(file);
}

void Init()
{
 scale = 1.;
 ct = 0;
 allCurve = 0;
 cskip = 1;
 cstep = cskip;
 npts = 40;
 angX = angZ = tx = ty = tz = 0.;
 angY = 0.;
 preprocess_results();
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

/*real friction_pendulum(real x, real y, real yp)
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
}*/

real parser_func1(real x, real y, real yp, real z, real zp, real v, real vp)
{
 return fpar7_f(FUNC1, x, y, yp, z, zp, v, vp);
}

real parser_func2(real x, real y, real yp, real z, real zp, real v, real vp)
{
 return fpar7_f(FUNC2, x, y, yp, z, zp, v, vp);
}

real parser_func3(real x, real y, real yp, real z, real zp, real v, real vp)
{
 return fpar7_f(FUNC3, x, y, yp, z, zp, v, vp);
}

void runge_kutta_dsystem_single_step(
	real (*func1)(real, real, real, real, real, real, real),
	real (*func2)(real, real, real, real, real, real, real),
	real (*func3)(real, real, real, real, real, real, real),
	real  x0, 
	real  y0, real  yp0, 
	real  z0, real  zp0, 
	real  v0, real  vp0, 
	real h,
	real* x1, 
	real* y1, real* yp1,
	real* z1, real* zp1,
	real* v1, real* vp1
	)
{
 real k1,k2,k3,k4;
 real l1,l2,l3,l4;
 real m1,m2,m3,m4;
 real kk0, kk1;
 real ll0, ll1;
 real mm0, mm1;
 real h2;

 h2 = h/2.L;

 k1 = h * h2 * func1(x0, y0, yp0, z0, zp0, v0, vp0);
 l1 = h * h2 * func2(x0, y0, yp0, z0, zp0, v0, vp0);
 m1 = h * h2 * func3(x0, y0, yp0, z0, zp0, v0, vp0);
 
 k2 = h * h2 * func1(x0+h2, y0+h2*yp0+k1/4.L, yp0+k1/h, z0+h2*zp0+l1/4.L, zp0+l1/h, v0+h2*vp0+m1/4.L, vp0+m1/h);
 l2 = h * h2 * func2(x0+h2, y0+h2*yp0+k1/4.L, yp0+k1/h, z0+h2*zp0+l1/4.L, zp0+l1/h, v0+h2*vp0+m1/4.L, vp0+m1/h);
 m2 = h * h2 * func3(x0+h2, y0+h2*yp0+k1/4.L, yp0+k1/h, z0+h2*zp0+l1/4.L, zp0+l1/h, v0+h2*vp0+m1/4.L, vp0+m1/h);
 
 k3 = h * h2 * func1(x0+h2, y0+h2*yp0+k1/4.L, yp0+k2/h, z0+h2*zp0+l1/4.L, zp0+l2/h, v0+h2*vp0+m1/4.L, vp0+m2/h);
 l3 = h * h2 * func2(x0+h2, y0+h2*yp0+k1/4.L, yp0+k2/h, z0+h2*zp0+l1/4.L, zp0+l2/h, v0+h2*vp0+m1/4.L, vp0+m2/h);
 m3 = h * h2 * func3(x0+h2, y0+h2*yp0+k1/4.L, yp0+k2/h, z0+h2*zp0+l1/4.L, zp0+l2/h, v0+h2*vp0+m1/4.L, vp0+m2/h);
 
 k4 = h * h2 * func1(x0+h, y0+h*yp0+k3, yp0+(2.L*k3)/h, z0+h*zp0+l3, zp0+(2.L*l3)/h, v0+h*vp0+m3, vp0+(2.L*m3)/h);
 l4 = h * h2 * func2(x0+h, y0+h*yp0+k3, yp0+(2.L*k3)/h, z0+h*zp0+l3, zp0+(2.L*l3)/h, v0+h*vp0+m3, vp0+(2.L*m3)/h);
 m4 = h * h2 * func3(x0+h, y0+h*yp0+k3, yp0+(2.L*k3)/h, z0+h*zp0+l3, zp0+(2.L*l3)/h, v0+h*vp0+m3, vp0+(2.L*m3)/h);

 kk0 = (k1+k2+k3)/3.L;
 kk1 = (k1+(2.L*k2)+(2.L*k3)+k4)/3.L;
 
 ll0 = (l1+l2+l3)/3.L;
 ll1 = (l1+(2.L*l2)+(2.L*l3)+l4)/3.L;
 
 mm0 = (m1+m2+m3)/3.L;
 mm1 = (m1+(2.L*m2)+(2.L*m3)+m4)/3.L;

/* printf("k1=%Lf, k2=%Lf, k3=%Lf, k4=%Lf\n", k1, k2, k3, k4);
 printf("kk0=%Lf, kk1=%Lf\n", kk0, kk1);
 printf("l1=%Lf, l2=%Lf, l3=%Lf, l4=%Lf\n", l1, l2, l3, l4);
 printf("ll0=%Lf, ll1=%Lf\n", ll0, ll1);*/

 *x1  = x0 + h;
 
 *y1  = y0 + h*yp0 + kk0;
 *yp1 = yp0 + kk1/h;
 
 *z1  = z0 + h*zp0 + ll0;
 *zp1 = zp0 + ll1/h;
 
 *v1  = v0 + h*vp0 + mm0;
 *vp1 = vp0 + mm1/h;

/* if (file) 
     fprintf(file, "t=%Lf, y(%Lf)=%Lf, y'(%Lf)=%Lf, z(%Lf)=%Lf, z'(%Lf)=%Lf, v(%Lf)=%Lf, v'(%Lf)=%Lf\n",
	     *x1, *x1, *y1, *x1, *yp1, *x1, *z1, *x1, *zp1, *x1, *v1, *x1, *vp1);*/
}

/*void runge_kutta_single_step(
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


 *x1  = x0 + h;
 *y1  = y0 + h*yp0 + kk0;
 *yp1 = yp0 + kk1/h;

}*/

/*void runge_kutta_nsolve(
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
}*/

void runge_kutta_nsolve_dsystem(
	real (*f1)(real, real, real, real, real, real, real),
	real (*f2)(real, real, real, real, real, real, real),
	real (*f3)(real, real, real, real, real, real, real),
	real x0, real x1,
	real y0, real yp0,
	real z0, real zp0,
	real v0, real vp0,
	real h, int* nn,
	real** rx, 
	real** ry, 
	real** ryp,
	real** rz, 
	real** rzp,
	real** rv, 
	real** rvp
	)
{
 int n,i;
 real *resX;
 real *resY, *resYP;
 real *resZ, *resZP;
 real *resV, *resVP;
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
 resV  = (real*)malloc((n+1)*sizeof(real));
 resVP = (real*)malloc((n+1)*sizeof(real));
 if (!resX || !resY || !resYP || !resZ || !resZP || !resV || !resVP)
   {
    *nn = 0;
    return;
   }
 resX[0]  = x0;
 resY[0]  = y0;
 resYP[0] = yp0;
 resZ[0]  = z0;
 resZP[0] = zp0;
 resV[0]  = v0;
 resVP[0] = vp0;
 for (i=1;i<=n;i++)
   {
    runge_kutta_dsystem_single_step(f1, f2, f3,
	    resX[i-1], 
	    resY[i-1], resYP[i-1], 
	    resZ[i-1], resZP[i-1], 
	    resV[i-1], resVP[i-1], 
	    h, 
	    &resX[i], 
	    &resY[i], &resYP[i],
	    &resZ[i], &resZP[i],
	    &resV[i], &resVP[i]
	    );
   }
 *rx  = resX;
 *ry  = resY;
 *ryp = resYP;
 *rz  = resZ;
 *rzp = resZP;
 *rv  = resV;
 *rvp = resVP;
 *nn = n;
}

void parser_ndsolver(real t0, real t1, 
		real y0, real yp0,
		real z0, real zp0,
		real v0, real vp0,
		real h, char* fn)
{
/* real *ttab, *ytab, *yptab, *ztab, *zptab;*/
 file = NULL;
 if (strcmp(fn, "null")) file = fopen(fn, "w");
 runge_kutta_nsolve_dsystem(
	 parser_func1, 
	 parser_func2,
	 parser_func3, /* what solve */
	 t0, t1, 	    /* time range */
	 y0, yp0,        /* y(t0), y'(t0) */
	 z0, zp0,        /* z(t0), z'(t0) */
	 v0, vp0,        /* v(t0), v'(t0) */
	 h,		    /* h: integral step */
	 &Nt, &ttab, 	/* time and taime array */
	 &ytab, &yptab,
	 &ztab, &zptab,
	 &vtab, &vptab
	 );
 opengl_display();
}

void help()
{
 printf("parameters: func_def1 func_def2 func_def3\n");
 printf("pardef1 pardef2 pardef3 t0 t1 y0 yp0 z0 zp0 v0 vp0 h fileout\n");
 printf("Example: You want to solve 3 differential equations:\n");
 printf("y''=z'+y-t and z''=z'+y'-cos(z)-sin(y) and v''=-v+t\n");
 printf("y(t0)=1, z(t0)=0, v(t0)=-3, y'(t0)=-1, z'(t0)=2, v'(t0)=4\n");
 printf("in t=[0,5], timestep: 0.01, then:\n");
 printf("Solver will give You {y,y',z,z',v,v'}(t) in [t0,t1] with step=h\n");
 printf("You can define parametrizationusing: y(t)=a, z(t)=b, v(t)=c\n");
 printf("on circle 'cos(a)*sin(b)', '-cos(b)', 'sin(a)*sin(b)'\n");
 printf("./nsolve 'zp-y-t' 'zp-yp-cos(z)-sin(y)' '-v+t' 'cos(a)*sin(b)'\n");
 printf("'-cos(b)' 'sin(a)*sin(b)' 0 5 1 -1 0 2 -3 4 0.01 out.txt\n");
 printf("keys: wsadex(rotations), ikjlom(translations), 12(scale)\n");
 printf("34(curve_length), SPACE(reset), q,ESC(quit)\n");
 printf("56(speed), 7(allCurve) if fileout==null then no write\n");
}

int main(int lb, char** par)
{
/* nsolve1();*/
/* nsolve2();*/
 if (lb < 17) { help(); return 1; }
 fpar_init(6);
 fpar_function(0, par[1]);
 fpar_function(1, par[2]);
 fpar_function(2, par[3]);
 fpar_function(3, par[4]);
 fpar_function(4, par[5]);
 fpar_function(5, par[6]);
 if (!fpar7_ok(0) || !fpar7_ok(1) || !fpar7_ok(2) 
	 || !fpar3_ok(3) || !fpar3_ok(4) || !fpar3_ok(5))
   {
    printf("Errors in function definitions.\n");
    return 2;
   }
/* printf("%Lf %Lf\n", fpar5_f(0, 1, 2, 3, 4, 5), fpar5_f(1, 10, 20, 30, 40, 50));*/
 parser_ndsolver(
	 atof(par[7]),	/*  t0 */
	 atof(par[8]),	/*  t1 */
	 atof(par[9]),	/*  y0 */
	 atof(par[10]),	/*  yp0 */
	 atof(par[11]),	/*  z0 */
	 atof(par[12]),	/*  zp0 */
	 atof(par[13]),	/*  v0 */
	 atof(par[14]),	/*  vp0 */
	 atof(par[15]),	/*  h */
	 par[16]	/*  h */
	     );
 return 0;
}

