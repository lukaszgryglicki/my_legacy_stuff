/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
int wx,wy;
int cx,cy;
int n1,n2,p1,p2,m1,m2,s,si,sj;
GLfloat angX, angY, angZ;
GLfloat*** D;
GLfloat* t1;
GLfloat* knot1;
GLfloat* t2;
GLfloat* knot2;
GLfloat* ctlpts;
GLUnurbsObj *theNurb;
GLfloat scalef = 1.;
GLfloat precision;
GLfloat step = 5.;
time_t tm1,tm2;
int fps;
int want_d = 0;
int auto_move = 1;
int moving = 0;
void full_re_interpolate(int num);
void init_ctlpts();

void help()
{
 printf("usage: {basename} nurbs_file\n");
 printf("1: display D toggle\n");
 printf("2: auto_move toogle\n");
 printf("+/-: scaling\n");
 printf("n/b: next/prev point U.\n");
 printf("c/v: next/prev point V.\n");
 printf("3/4/5/6: increase/decrease degree1/degree2\n");
 printf("r: write BSpline surface\n");
}



void next_point()
{
 sj++;
 if (sj == n2+1) { si++; sj = 0; }
 if (si == n1+1) si = 0;
}


void prev_point()
{
 sj--;
 if (sj == -1) { si--; sj = n2; }
 if (si == -1) si = n1;
}


void right_point()
{
 si++;
 if (si == n1+1) { sj++; si = 0; }
 if (sj == n2+1) sj = 0;
}


void left_point()
{
 si--;
 if (si == -1) { sj--; si = n1; }
 if (sj == -1) sj = n2;
}


void moving_point()
{
 if (moving) 
  {
   moving = 0;
   full_re_interpolate(1);
   full_re_interpolate(2);
  }
 else moving = 1;
}


void move_point(GLfloat x, GLfloat y, GLfloat z)
{
 if (!moving) return;
 D[si][sj][0] += x;
 D[si][sj][1] += y;
 D[si][sj][2] += z;
}


void write_bspline()
{
 int i,j,k;
 FILE* plik;
 char fname[32];
 i = 1;
 while (1)
 {
  sprintf(fname, "bspline_surface%d", i);
  plik = fopen(fname, "r");
  if (plik) { fclose(plik); i++; }
  else break;
 }
 plik = fopen(fname, "w");
 if (!plik) { printf("cannot write to: %s\n", fname); return; }
 printf("writing bspline surface definition to: %s\n", fname);
 fprintf(plik, "%d,%d,%d,%d,%d\n", n1+1,n2+1,p1,p2,s);
 for (i=0;i<=n1;i++)
   {
    for (j=0;j<=n2;j++) 
      {
       fprintf(plik, "(");
       for (k=0;k<s-1;k++) fprintf(plik, "%f,", D[i][j][k]);
       fprintf(plik, "%f) ", D[i][j][s-1]);
      }
    fprintf(plik, "\n");
   }
 fclose(plik);
}


void full_re_interpolate(int num)
{
 int i,j;
 if (num == 1)
   {
    if (knot1) free(knot1);
    knot1 = (GLfloat*)malloc((m1+1)*sizeof(GLfloat));
    for (i=0;i<=p1;i++) knot1[i] = 0.;
    for (j=1;j<=n1-p1;j++)
      {
       knot1[j+p1] = 0.;
       for (i=j;i<=j+p1-1;i++) knot1[j+p1] += t1[i];
       knot1[j+p1] /= (GLfloat)p1;
      }
    for (i=m1-p1;i<=m1;i++) knot1[i] = 1.;
   }
 if (num == 2)
   {
    if (knot2) free(knot2);
    knot2 = (GLfloat*)malloc((m2+1)*sizeof(GLfloat));
    for (i=0;i<=p2;i++) knot2[i] = 0.;
    for (j=1;j<=n2-p2;j++)
      {
       knot2[j+p2] = 0.;
       for (i=j;i<=j+p2-1;i++) knot2[j+p2] += t2[i];
       knot2[j+p2] /= (GLfloat)p2;
      }
    for (i=m2-p2;i<=m2;i++) knot2[i] = 1.;
   }
 free(ctlpts);
 init_ctlpts();
}


void lower_deg(int num)
{
 if (num == 1)
  {
   p1--;
   if (p1 < 1) { p1 = 1; return; }
   m1 = n1+p1+1;
  }
 if (num == 2)
  {
   p2--;
   if (p2 < 1) { p2 = 1; return; }
   m2 = n2+p2+1;
  }
 full_re_interpolate(num);
}


void higher_deg(int num)
{
 if (num == 1)
  {
   p1++;
   if (p1 > n1) { p1 = n1; return; }
   m1 = n1+p1+1;
  }
 if (num == 2)
  {
   p2++;
   if (p2 > n2) { p2 = n2; return; }
   m2 = n2+p2+1;
  }
 full_re_interpolate(num);
}


void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
        case 'r': write_bspline(); break;
	case 'd': move_point(step, 0., 0.); break;
	case 'a': move_point(-step, 0., 0.); break;
	case 's': move_point(0., -step, 0.); break;
	case 'w': move_point(0., step, 0.); break;
	case 'e': move_point(0., 0., step); break;
	case 'x': move_point(0., 0., -step); break;
        case 'i': angX -= 10.; break;
        case 'k': angX += 10.; break;
        case 'j': angY += 10.; break;
        case 'l': angY -= 10.; break;
        case 'o': angZ += 10.; break;
        case 'm': angZ -= 10.; break;
	case 'p': moving_point(); break;
	case 'n': next_point(); break;
	case 'b': prev_point(); break;
	case 'v': right_point(); break;
	case 'c': left_point(); break;
	case '1': want_d = ! want_d; break;
        case '2': auto_move = ! auto_move; break;
        case '-': scalef /= 1.3; break;
	case '=': case '+': scalef *= 1.3; break;
	case '3': lower_deg(1); break;
	case '4': higher_deg(1); break;
	case '5': lower_deg(2); break;
	case '6': higher_deg(2); break;
	case ' ': scalef = 1.; angY = angZ = 0.; angX = -180.; auto_move = 0; break;
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


void free_matrix3(GLfloat*** m, int siz1, int siz2)
{
 int i,j;
 if (siz1 <= 0 || siz2 <= 0) return;
 for (i=0;i<siz1;i++)
 for (j=0;j<siz2;j++) free(m[i][j]);
 for (i=0;i<siz1;i++) free(m[i]);
 free(m);
}


void init_ctlpts()
{
 int i,j;
  ctlpts = (GLfloat*)malloc(((n1+1)*(n2+1)*3+1)*sizeof(GLfloat));
  for (i=0;i<=n1;i++)
  for (j=0;j<=n2;j++)
    {
     ctlpts[3*i*(n1+1)+3*j] = D[i][j][0];
     ctlpts[3*i*(n1+1)+3*j+1] = D[i][j][1];
     ctlpts[3*i*(n1+1)+3*j+2] = D[i][j][2];
    }
}


void time_counter()
{
 char tstr[64];
 if (tm1 == (time_t)0)
   {
    time(&tm1);
    time(&tm2);
    return;
   }
 fps++;
 time(&tm2);
 if (tm2 > tm1)
   {
    sprintf(tstr, "N=(%dx%d), Dim=(%d,%d), GLScene FPS: %d", n1+1, n2+1, p1, p2, fps/(int)(tm2-tm1));
    tm1 = tm2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
}


void render_scene(void)
{
 int i,j;
 GLfloat v;
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glPolygonMode(GL_FRONT, GL_LINE);
 glPolygonMode(GL_BACK, GL_LINE);
 glLoadIdentity();
 gluLookAt(0., 0., 100., 0. , 0., 0., 0., 100., 0.);
 glScaled(scalef, scalef, scalef);
 glRotatef(angX, 1., 0. , 0.);
 glRotatef(angY, 0., 1. , 0.);
 glRotatef(angZ, 0., 0. , 1.);
 for (i=0;i<=n1;i++)
 for (j=0;j<=n2;j++)
 {
 if (i == si && j == sj) { glColor3d(1., 1., 0.); v = 2.; }
 else { glColor3d(0., .4, .9); v = .7; }
 glBegin(GL_LINE_STRIP);
   glVertex3d(D[i][j][0]-v, D[i][j][1]-v, D[i][j][2]-v);
   glVertex3d(D[i][j][0]-v, D[i][j][1]+v, D[i][j][2]-v);
   glVertex3d(D[i][j][0]+v, D[i][j][1]+v, D[i][j][2]-v);
   glVertex3d(D[i][j][0]+v, D[i][j][1]-v, D[i][j][2]-v);
   glVertex3d(D[i][j][0]-v, D[i][j][1]-v, D[i][j][2]-v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(D[i][j][0]-v, D[i][j][1]-v, D[i][j][2]+v);
   glVertex3d(D[i][j][0]-v, D[i][j][1]+v, D[i][j][2]+v);
   glVertex3d(D[i][j][0]+v, D[i][j][1]+v, D[i][j][2]+v);
   glVertex3d(D[i][j][0]+v, D[i][j][1]-v, D[i][j][2]+v);
   glVertex3d(D[i][j][0]-v, D[i][j][1]-v, D[i][j][2]+v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(D[i][j][0]-v, D[i][j][1]-v, D[i][j][2]-v);
   glVertex3d(D[i][j][0]-v, D[i][j][1]+v, D[i][j][2]-v);
   glVertex3d(D[i][j][0]-v, D[i][j][1]+v, D[i][j][2]+v);
   glVertex3d(D[i][j][0]-v, D[i][j][1]-v, D[i][j][2]+v);
   glVertex3d(D[i][j][0]-v, D[i][j][1]-v, D[i][j][2]-v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(D[i][j][0]+v, D[i][j][1]-v, D[i][j][2]-v);
   glVertex3d(D[i][j][0]+v, D[i][j][1]+v, D[i][j][2]-v);
   glVertex3d(D[i][j][0]+v, D[i][j][1]+v, D[i][j][2]+v);
   glVertex3d(D[i][j][0]+v, D[i][j][1]-v, D[i][j][2]+v);
   glVertex3d(D[i][j][0]+v, D[i][j][1]-v, D[i][j][2]-v);
 glEnd();
 }
 if (want_d)
 {
 glColor3d(1., 0., 0.);
 glBegin(GL_QUADS);
 for (i=0;i<n1;i++)
 for (j=0;j<n2;j++)
   {
    glVertex3d(D[i][j][0], D[i][j][1], D[i][j][2]);
    glVertex3d(D[i+1][j][0], D[i+1][j][1], D[i+1][j][2]);
    glVertex3d(D[i+1][j+1][0], D[i+1][j+1][1], D[i+1][j+1][2]);
    glVertex3d(D[i][j+1][0], D[i][j+1][1], D[i][j+1][2]);
   }
 glEnd();
 }
 glPolygonMode(GL_FRONT, GL_FILL);
 glPolygonMode(GL_BACK, GL_FILL);
 gluBeginSurface(theNurb);
 gluNurbsSurface(theNurb, m1+1, knot1, m2+1, knot2, (n1+1)*3, 3, ctlpts, p1+1, p2+1, GL_MAP2_VERTEX_3);
 gluEndSurface(theNurb);
 if (auto_move)
  {
   angX += 2.3;
   angY += 2.5;
   angZ += 2.7;
  }
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


GLfloat* vector(int siz)
{
 if (siz <= 0) return NULL;
 return (GLfloat*)malloc(siz*sizeof(GLfloat));
}


GLfloat** matrix2(int siz1, int siz2)
{
 GLfloat** mem;
 int i;
 if (siz1 <= 0 || siz2 <= 0) return NULL;
 mem = (GLfloat**)malloc(siz1*sizeof(GLfloat*));
 for (i=0;i<siz1;i++) mem[i] = vector(siz2);
 return mem;
}


GLfloat*** matrix3(int siz1, int siz2, int siz3)
{
 GLfloat*** mem;
 int i;
 if (siz1 <= 0 || siz2 <= 0 || siz3 <= 0) return NULL;
 mem = (GLfloat***)malloc(siz1*sizeof(GLfloat**));
 for (i=0;i<siz1;i++) mem[i] = matrix2(siz2, siz3);
 return mem;
}

void read_curve_data(char* fn)
{
    int i,j,k;
    int ok,kn;
    FILE* plik;
    plik = fopen(fn, "r");
    if (!plik) { printf("cannot read from: %s\n", fn); exit(9); }
    ok = fscanf(plik,"n1=%d,n2=%d,p1=%d,p2=%d,s=%d,knots=%d\n", &n1, &n2, &p1, &p2, &s, &kn);
    if (ok != 6) { printf("scanf error!\n"); exit(17); }
    n1--; n2--;
    if (n1 < 2 || n1 > 128) exit(2);
    if (p1 < 1 || p1 > 12)  exit(3);
    if (n2 < 2 || n2 > 128) exit(2);
    if (p2 < 1 || p2 > 12)  exit(3);
    D = matrix3(n1+1, n2+1, s);
    for (i=0;i<=n1;i++)
    {
	for (j=0;j<=n2;j++)
	{
	fscanf(plik,"(");
	for (k=0;k<s-1;k++) 
	  { 
	   ok=fscanf(plik,"%f,", &D[i][j][k]); 
	   if (ok !=1) 
	     { 
	      printf("scanf error inline!\n"); 
	      exit(18); 
	     }
	  }
	ok = fscanf(plik,"%f) ", &D[i][j][s-1]);
	if (ok != 1) { printf("scanf error while reading 'num)'\n"); exit(19); }
	}
    fscanf(plik, "\n");
    }
    t1 = (GLfloat*)malloc((n1+1)*sizeof(GLfloat));
    t2 = (GLfloat*)malloc((n2+1)*sizeof(GLfloat));
    for (i=0;i<=n1;i++) t1[i] = (GLfloat)i/(GLfloat)n1;
    for (i=0;i<=n2;i++) t2[i] = (GLfloat)i/(GLfloat)n2;
    m1 = n1+p1+1;
    m2 = n2+p2+1;
    knot1 = (GLfloat*)malloc((m1+1)*sizeof(GLfloat));
    knot2 = (GLfloat*)malloc((m2+1)*sizeof(GLfloat));
    for (i=0;i<=p1;i++) knot1[i] = 0.;
    for (i=0;i<=p2;i++) knot2[i] = 0.;
    for (j=1;j<=n1-p1;j++)
    {
	knot1[j+p1] = 0.;
	for (i=j;i<=j+p1-1;i++) knot1[j+p1] += t1[i];
	knot1[j+p1] /= (GLfloat)p1;
    }
    for (j=1;j<=n2-p1;j++)
    {
	knot2[j+p2] = 0.;
	for (i=j;i<=j+p2-1;i++) knot2[j+p2] += t2[i];
	knot2[j+p2] /= (GLfloat)p2;
    }
    for (i=m1-p1;i<=m1;i++) knot1[i] = 1.;
    for (i=m2-p2;i<=m2;i++) knot2[i] = 1.;
    if (kn)
      {
       printf("reading user knots m1=%d, m2=%d\n", m1,m2);
       for (i=0;i<=m1;i++) 
         { 
          ok = fscanf(plik, "%f,", &knot1[i]); 
	  if (ok != 1) 
	    { 
	     printf("knot1 scan error!\n"); 
	     exit(41); 
	    } 
	 }
       fscanf(plik, "\n");
       for (i=0;i<=m2;i++) 
         { 
          ok = fscanf(plik, "%f,", &knot2[i]); 
	  if (ok != 1) 
	    { 
	     printf("knot2 scan error!\n"); 
	     exit(42); 
	    } 
	 }
      }
 printf("knot1={");
 for (i=0;i<m1-1;i++) printf("%f,", knot1[i]);
 printf("%f}\n", knot1[m1-1]);
 printf("knot2={");
 for (i=0;i<m2-1;i++) printf("%f,", knot2[i]);
 printf("%f}\n", knot2[m2-1]);
}


void InitFile(char* fn)
{
 read_curve_data(fn);
 help();
 angY = angZ = 0.;
 angX = -180.;
 si = sj = 0;
}


void glInit()
{
 glClearColor(1.,1.,1.,0.);
 glShadeModel(GL_SMOOTH);
}


void nurbInit()
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
   init_ctlpts();
   precision = 15.;
   theNurb = gluNewNurbsRenderer();
   gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, precision);
   gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}


int main(int lb, char** par)
{
 wx = 800;
 wy = 600;
 if (lb < 2)
   {
    help();
    exit(20);
   }
 InitFile(par[1]);
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(100, 100);
 glutCreateWindow(par[0]);
 nurbInit();
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
