/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#define PI  3.1415926
#define PI2 6.2831853071795862

typedef struct _Uni_save
{
  double u[98];
  double c;
  double cd;
  double cm;
  int ui;
  int uj;
} Uni_save;

/*void seed_uni(int ijkl);
double gaussian();*/
Uni_save uni_data;
GLint wx,wy;
GLint cx,cy;
GLdouble scalef;
time_t t1,t2;
GLint fps;
GLdouble step;
GLdouble l1, l2, l2o;
GLint omi;
GLint eps, normal;
GLdouble* X;
GLdouble* V;
GLdouble* A;
GLdouble aX, aY, aZ;
GLint aa;


double uni()
{
	register double luni;
	luni = uni_data.u[uni_data.ui] - uni_data.u[uni_data.uj];
	if (luni < 0.0) luni += 1.0;
	uni_data.u[uni_data.ui] = luni;
	if (--uni_data.ui == 0) uni_data.ui = 97;
	if (--uni_data.uj == 0) uni_data.uj = 97;
	if ((uni_data.c -= uni_data.cd) < 0.0) uni_data.c += uni_data.cm;
	if ((luni -= uni_data.c) < 0.0) luni += 1.0;
	return ((double) luni);
}


void rstart(int i,int j,int k,int l)
{
	int ii, jj, m;
	double s, t;
	for (ii = 1; ii <= 97; ii++) 
	{
		s = 0.0;
		t = 0.5;
		for (jj = 1; jj <= 24; jj++) 
		{
			m = ((i*j % 179) * k) % 179;
			i = j;
			j = k;
			k = m;
			l = (53*l+1) % 169;
			if (l*m % 64 >= 32)
				s += t;
			t *= 0.5;
		}
		uni_data.u[ii] = s;
	}
	uni_data.c  = 362436.0   / 16777216.0;
	uni_data.cd = 7654321.0  / 16777216.0;
	uni_data.cm = 16777213.0 / 16777216.0;
	uni_data.ui = 97;
	uni_data.uj = 33; 
}

void seed_uni(int ijkl)
{
 int i, j, k, l, ij, kl;
 ijkl %= 900000000;
 if (ijkl == 0)
   {
    ijkl = time((time_t *)0);
    ijkl %= 900000000;
   }
 if ((ijkl < 0) || (ijkl > 900000000))
   {
    printf("seed_uni: ijkl = %d -- out of range\n\n", ijkl);
    exit(3);
   }
	ij = ijkl/30082;
	kl = ijkl - (30082 * ij);
	i = ((ij/177) % 177) + 2;
	j = (ij % 177) + 2;
	k = ((kl/169) % 178) + 1;
	l = kl % 169;
    rstart(i, j, k, l);
}

double gaussian()
{
	register double ran1, ran2;
	do 
	  { 
	   ran1 = (double)uni();
	  } 
	while (ran1 == 0.);
	ran2 = (double)uni();
	return (double)( sqrt(-2.*log(ran1))*cos(PI2*ran2));
}



void randomize()
{
 time_t tm;
 time(&tm);
 srand((int)tm);
 seed_uni((int)tm);
}


void help()
{
 printf("usage: {basename}\n");
 printf("kays:\n1,2\tchange l1\n3,4\tchange l2\n5,6\tincr/dect eps\n7\tdistr. type toggle"
		 "\n-=\tscale\n8,9,0\trotations\nSPACE\tnormal positions\nESC,q\tquit\n");
}

void check_l()
{
 if (l1 < 3.)   l1 = 3.;
 if (l1 > 40.)  l1 = 40.;
 if (l2o < 5.)  l2o = 5.;
 if (l2o > 70.) l2o = 70.;
 if (l2o < l1)  l2o = l1;
 l2 = l2o;
}


void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
        case '-': scalef /= 1.02; break;
	case '=': case '+': scalef *= 1.02; break;
	case '1': l1 /= 1.02;  check_l(); break;
	case '2': l1 *= 1.02;  check_l(); break;
	case '3': l2o /= 1.02; check_l(); break;
	case '4': l2o *= 1.02; check_l(); break;
	case '5': eps++; break;
	case '6': eps--; if (eps <0) eps = 0; break;
	case '7': normal = !normal; break;
	case '8': aX+=2.; if (aX >= 360.)  aX -= 360.; break;
	case '9': aY+=2.; if (aY >= 360.)  aY -= 360.; break;
	case '0': aZ+=2.; if (aZ >= 360.)  aZ -= 360.; break;
	case ' ': eps = 0; normal = 1; aX = aY = aZ = 0.; aa = 0; break;
	case 'a': aa = ! aa; break;
   }
 printf("Eps = %d%%%%, normal = %d, l1=%f, l2=%f, FPS=%d\n", eps, normal, l1, l2, fps);
}

void view(GLsizei x1, GLsizei y1, GLsizei dx, GLsizei dy)
{
 glViewport(x1, y1, dx, dy);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1., 1., -1., 1., 1.5, 2000.);
 glMatrixMode(GL_MODELVIEW);
}


void resize_scene(int w, int h)
{
 wx = w;
 wy = h;
 view(0, 0, (GLsizei)wx, (GLsizei)wy);
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
    sprintf(tstr, "GLScene FPS: %d", fps/(int)(t2-t1));
    /*printf("FPS: %d\n", fps/(int)(t2-t1));*/
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
}

void glCube(GLdouble x, GLdouble y, GLdouble z, GLdouble v)
{
 glBegin(GL_LINE_STRIP);
   glVertex3d(x-v, y-v, z-v);
   glVertex3d(x-v, y+v, z-v);
   glVertex3d(x+v, y+v, z-v);
   glVertex3d(x+v, y-v, z-v);
   glVertex3d(x-v, y-v, z-v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(x-v, y-v, z+v);
   glVertex3d(x-v, y+v, z+v);
   glVertex3d(x+v, y+v, z+v);
   glVertex3d(x+v, y-v, z+v);
   glVertex3d(x-v, y-v, z+v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(x-v, y-v, z-v);
   glVertex3d(x-v, y+v, z-v);
   glVertex3d(x-v, y+v, z+v);
   glVertex3d(x-v, y-v, z+v);
   glVertex3d(x-v, y-v, z-v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(x+v, y-v, z-v);
   glVertex3d(x+v, y+v, z-v);
   glVertex3d(x+v, y+v, z+v);
   glVertex3d(x+v, y-v, z+v);
   glVertex3d(x+v, y-v, z-v);
 glEnd();
}

GLdouble Xc(GLdouble a)
{
 GLdouble x,y,x2,dx;
 if (a > PI2) a -= PI2;
 x = cos(a)*l1;
 y = sin(a)*l1;
 dx = sqrt(l2*l2 - y*y);
 return x + dx;
}

void numerical_computations()
{
 GLdouble f1, f2;
 GLdouble a1, a2;
 GLdouble f12, f22;
 GLdouble v2;
 GLdouble epsilon;
 epsilon = 1e-5;
 
 f1 = X[omi];
 f2 = Xc(((GLdouble)omi*step) + epsilon);
 V[omi] = (f2 - f1) / epsilon;
 
 f12 = f2;
 f22 = Xc(((GLdouble)omi*step) + 2*epsilon);
 v2 = (f22 - f12) / epsilon;

 /*
 printf("f1 = %2.13f, f2 = %2.13f, f3 = %2.13f, df1 = %2.13f, df2= %2.13f\n", f1, f12, f22, f12-f1, f22-f12);
 printf("v1 = %2.13f, v2 = %2.13f, dv = %2.13f\n", V[omi], v2, v2-V[omi]);
 */

 A[omi] = (v2 - V[omi]) / epsilon;
}

void render_circle()
{
 GLdouble om;
 GLdouble x2;
 GLdouble x,y;
 GLdouble dx;
 glTranslated(-l1, 0., 0.);
 glColor3d(1., 1., 0.);
 glBegin(GL_LINE_STRIP);
 for (om=0.;om<PI2;om+=step)
   {
    x = cos(om)*l1;
    y = sin(om)*l1;
    glVertex3d(x, y, 0.);
   }
 glEnd();

 x = cos(step*omi)*l1;
 y = sin(step*omi)*l1;
 glColor3d(1., 0., 0.);
 glCube(x,y,0., 1.);
 
 dx = sqrt(l2*l2 - y*y);
 x2 = x + dx;
 glColor3d(0., 1., 0.);
 glCube(x2,0.,0., 1.);
 glColor3d(0., 1., 1.);
 glCube(0., 0., 0., .5);
 
 glBegin(GL_LINE_STRIP);
   glColor3d(1., 0., 0.);
   glVertex3d(x, y, 0.);
   glColor3d(0., 1., 0.);
   glVertex3d(x2, 0., 0.);
 glEnd();
 
 X[omi] = x2;
 numerical_computations();
}

void next_omi()
{
 omi ++;
 if (omi >= 360) omi = 0;
}


void render_vector(GLdouble* v)
{
 GLint i;
 GLdouble x,y;
 GLdouble max;
 max = -1.;
 for (i=0;i<360;i++) if (fabs(v[i]) > max) max = fabs(v[i]);

 if (max > 0) glScaled( 1., 65./max, 1.);
 
 glBegin(GL_LINE_STRIP);
 for (i=0;i<360;i++)
   {
    x = (GLdouble)(i - 180) / 3.;
    y = v[i];
    glVertex3d(x, y, 0.);
   }
 glEnd();
 glColor3d(.5, 0., 1.);
 glBegin(GL_LINE_STRIP);
 for (i=0;i<360;i++)
   {
    x = (GLdouble)(i - 180) / 3.;
    glVertex3d(x, 0., 0.);
   }
 glEnd();
 glCube((GLdouble)(omi-180) / 3., v[omi], 0, 1.);
}

void render_xv()
{
 GLdouble maxx,maxv,max;
 GLint i;
 maxx = -1.;
 for (i=0;i<360;i++) if (fabs(X[i]) > maxx) maxx = fabs(X[i]);
 maxv = -1.;
 for (i=0;i<360;i++) if (fabs(V[i]) > maxv) maxv = fabs(V[i]);
 max = (maxx > maxv)? maxx : maxv;
 glTranslated(0., -max/2., 0.);
 //printf("Vmax = %f, Xmax = %f\n", maxv, maxx);

 if (maxx >0 && maxv > 0) glScaled(65./max, 65./max, 1.);
 glColor3d(.5, .7, .2);
 glBegin(GL_LINE_STRIP);
 for (i=0;i<360;i++)
   {
    glVertex3d(V[i], X[i], 0.);
   }
 glEnd();
 glCube(V[omi], X[omi], 0., 1.);
}

void world()
{
 glLoadIdentity();
 gluLookAt(0., 0., 100., 0. , 0., 0., 0., 100., 0.);
 glScaled(scalef, scalef, scalef);
 glRotated(aX, 1., 0., 0.);
 glRotated(aY, 0., 1., 0.);
 glRotated(aZ, 0., 0., 1.);
}

GLdouble uniform()
{
 return (GLdouble)((rand() % 1000001) - 500000) / 500000.;
}

void compute_l2()
{
 if (normal) l2 = l2o + (gaussian()*l2o*(GLdouble)eps)/1000.;
 else l2 = l2o + (uniform()*l2o*(GLdouble)eps)/1000.;
 if (l2 < l1) l2 = l1;
 /*printf("normal = %d, eps = %d\n", normal, eps);*/
}


void render_scene(void)
{
 GLint left;
 left = 0;
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 if (aa) glEnable(GL_LINE_SMOOTH);
 else glDisable(GL_LINE_SMOOTH);

 compute_l2();
 
 view(0, 128, 640, 640);
 world();
 render_circle();
 
 view(500, 640, 524, 128);
 world();
 glColor3d(0.7, .2, 0.);
 render_vector(X);
 
 view(500, 512, 524, 128);
 world();
 glColor3d(0., .7, .2);
 render_vector(V);
 
 view(500, 384, 524, 128);
 world();
 glColor3d(.4, 0., .6);
 render_vector(A);
 
 view(640, 0, 384, 384);
 world();
 render_xv();
 
 time_counter();
 glFlush();
 glutSwapBuffers();
 next_omi();
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
 GLint i;
 glClearColor(0.,0.,0.,0.);
 glShadeModel(GL_SMOOTH);
 scalef = 1.;
 step = PI2 / 360.;
 l1 = 20.;
 l2o = 25.;
 omi = 0;
 l2 = l2o;
 eps = 0.;
 normal = 1;
 aa = 0;
 aX = aY = aZ = 0.;
 X = (GLdouble*)malloc(360*sizeof(GLdouble));
 V = (GLdouble*)malloc(360*sizeof(GLdouble));
 A = (GLdouble*)malloc(360*sizeof(GLdouble));
 for (i=0;i<360;i++) X[i] = V[i] = A[i] = 0.;
 randomize();
 help();
}



int main(int lb, char** par)
{
 wx = 1024;
 wy = 768;
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutGameModeString("1024x768:32@85");
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(0, 0);
 glutCreateWindow(par[0]);
 glutDisplayFunc(render_scene);
 glutReshapeFunc(resize_scene);
 glutKeyboardFunc(keyboard);
 glutVisibilityFunc(visible);
 glInit();
 glutFullScreen();
 glutMainLoop();
 return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
