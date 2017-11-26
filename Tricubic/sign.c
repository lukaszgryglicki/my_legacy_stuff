/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>
#define PI 3.1415926F

typedef struct _VList
{
 struct _VList *next, *prev;
 GLfloat x,y,z;
} List;

GLfloat angX, angY, angZ;	
int wx,wy;		
int p,n,m,nelem;
int fps;
time_t t1,t2;
GLfloat *knots, *ctlpts, *datpts;
GLfloat curx, cury, curz;
GLfloat angX, angY, angZ;
List* list;

void help()
{
 printf("usage: ./draw [positive_number]\n");
}

void list_add(List** head, GLfloat x, GLfloat y, GLfloat z)
{
 List* temp;
 if (*head == NULL)
   {
    *head = (List*)(malloc(sizeof(List)));
    if (!(*head)) { printf("malloc problem! list_add\n"); exit(1); }
    (*head)->x = x;
    (*head)->y = y;
    (*head)->z = z;
    (*head)->next = NULL;
    (*head)->prev = NULL;
    return;
   }
  temp = (List*)(malloc(sizeof(List)));
  if (!temp) { printf("malloc problem! list_add\n"); exit(1); }
  temp->x = x;
  temp->y = y;
  temp->z = z;
  temp->next = *head;
  temp->prev = NULL;
  (*head)->prev = temp;
  *head = temp;
}

List* get_list_tail(List* head)
{
 List* temp;
 temp = head;
 while (head) { temp = head; head = head->next; }
 return temp;
}

void update_curve();
void anti_world(GLfloat* cux, GLfloat* cuy, GLfloat* cuz);
GLfloat curve(GLfloat t, int xyz);


void delta(GLint wsp, GLfloat offset)
{
 GLfloat x,y,z;
 x = y = z = 0.;
 if (wsp == 0) x += offset;
 if (wsp == 1) y += offset;
 if (wsp == 2) z += offset;
 anti_world(&x, &y, &z);
 curx += x;
 cury += y;
 curz += z;
}

void write_signature()
{
 GLfloat u,x,y,z,uStep,nx,ny;
 GLfloat minx,miny,maxx,maxy;
 GLfloat startx, endx, starty, endy;
 GLint i;
 FILE* plik;
 if (n < p) return;
 plik = fopen("sign.k2", "w");
 if (!plik) { printf("cannot write to: sign.k2\n"); return; }
 minx = miny = 1e10;
 maxx = maxy = -1e10;
 startx = -70.; /* w gore */
 endx   =  -20.;
 starty = -70.;
 endy =  0.;	/* poziomo do raczki */
 uStep = 0.005/n;
 z = 25.;
 for (u=-0.;u<1.;u+=uStep) 
  {
   x =curve(u, 0);
   y = curve(u, 1); 
   if (x < minx) minx = x;
   if (x > maxx) maxx = x;
   if (y < miny) miny = y;
   if (y > maxy) maxy = y;
  }
 printf("(%f,%f) --> (%f,%f)\n", minx,miny,maxx,maxy);
 i = 0;
 uStep = 0.1/n;
 for (u=-0.;u<1.;u+=uStep) 
  {
   x = curve(u, 0);
   y = curve(u, 1); 
   nx = startx + ((x-minx)/(maxx-minx))*(endx-startx);
   ny = starty + ((y-miny)/(maxy-miny))*(endy-starty);
   if (i == 0)
     {
      fprintf(plik, "X%fY%fZ%f\n", -90., -90., 60.);
      fprintf(plik, "X%fY%fZ%f\n", nx, ny, 60.);
     }
   i++;
   fprintf(plik, "X%fY%fZ%f\n", nx, ny, z);
  }
 fprintf(plik, "X%fY%fZ%f\n", -90., -90., 60.);
 printf("Done %d paths.\n", i);
}

void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
        case 'h':  help(); break;
	case 'i': delta(1, 10.);  break;
	case 'k': delta(1, -10.); break;
	case 'j': delta(0, -10.); break;
	case 'l': delta(0, 10.);  break;
	case 'o': delta(2, -10.); break;
	case 'm': delta(2, 10.);  break;
        case 'w': angX -= 5.; break;
        case 's': angX += 5.; break;
        case 'a': angY += 5.; break;
        case 'd': angY -= 5.; break;
        case 'e': angZ += 5.; break;
        case 'x': angZ -= 5.; break;
	case 'z': write_signature(); break;
	case ' ': list_add(&list, curx, cury, curz); update_curve(); break;
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


GLfloat basis_bspline(GLfloat val, int dim, int off, int I, int J)
{
 register GLfloat up;
 register GLfloat down;
 GLfloat factor1;
 GLfloat factor2;
 int Ioff, Joff;
 factor1 = factor2 = 0.0;
 Ioff = I+off;
 Joff = J+off;
 if (I>J)
   {
     printf("I more than J in BSPLINE basis");
     exit(1);
     return 0.0;
   }
 if (dim == 0)
   {
    if (val >= knots[Ioff] && val <= knots[Joff]) return 1.0;
    else return 0.0;
   }
 up = (val-knots[Ioff]) * basis_bspline(val, dim-1, off, I, J-1);
 down = knots[Joff-1] - knots[Ioff];
 if (down==0.0) factor1 = 0.0;
 else factor1 = up/down;
 up = (knots[Joff]-val) * basis_bspline(val, dim-1, off, I+1, J);
 down = knots[Joff] - knots[Ioff+1];
 if (down==0.0) factor2 += 0.0;
 else factor2 = up/down;
 return factor1+factor2;
}

GLfloat bsp(GLfloat t, int dim, int i)
{
 return basis_bspline(t, dim, dim, i-dim, i+1);
}

GLfloat curve(GLfloat t, int xyz)
{
 int i;
 GLfloat up,down;
 GLfloat factor;
 GLfloat b;
 up = 0.0;
 down = 0.0;
 for (i=0;i<=n;i++)
   {
    b = bsp(t, p, i);
    factor = ctlpts[3*i+xyz]*b;
    up += factor;
    down += b;
   }
 if (down==0.0) return 0.0;
 return up/down;
}

void NURBScurve()
{
 GLfloat u,x,y,z,uStep;
 if (n < p) return;
 glBegin(GL_LINE_STRIP);
 uStep = 0.03/n;
 for (u=-0.;u<1.;u+=uStep) 
  {
   x =curve(u, 0);
   y = curve(u, 1); 
   z = curve(u, 2);
   glVertex3d(x,y,z);
  }
 glEnd();
}

void Point3D(GLfloat x, GLfloat y, GLfloat z, GLfloat v)
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

void CTLpoints()
{
 int i;
 if (n < p) return;
 glColor3d(0., 0., 1.);
   for (i=0;i<=n;i++) Point3D(ctlpts[3*i], ctlpts[3*i+1], ctlpts[3*i+2], 5.);
 glColor3d(1., 0., 0.);
   for (i=0;i<=n;i++) Point3D(datpts[3*i], datpts[3*i+1], datpts[3*i+2], 5.);
}

void Cursor()
{
 glColor3d(0., 1., 0.);
 Point3D(curx, cury, curz, 10.);
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
    sprintf(tstr, "(%f,%f,%f) GLScene FPS: %d", curx, cury, curz, fps/(int)(t2-t1));
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
}

void render_scene(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_BLEND);
 glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
 glLoadIdentity();
 gluLookAt(0., 0., 600, 0. , 0., 0., 0., 1., 0.);
 glRotated(angX, 1., 0., 0.);
 glRotated(angY, 0., 1., 0.);
 glRotated(angZ, 0., 0., 1.);
 glColor3d(1., 1., 0.);
 Cursor();
 NURBScurve();
 CTLpoints();
 time_counter();
 glFlush();
 glutSwapBuffers();
}



void copy_matrix(GLfloat** dst, GLfloat** src, int siz)
{
 int i,j;
 if (!src || !dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++) dst[i][j] = src[i][j];
}

void I_matrix(GLfloat** dst, int siz)
{
 int i,j;
 if (!dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++) 
   {
    if (i == j) dst[i][j] = 1.;
    else dst[i][j] = 0.;
   }
}

int try_swap(GLfloat** m, int idx, int dim)
{
 register int x;
 for (x=idx;x<dim;x++) if (m[idx][x]) return x;
 return -1;
}

GLfloat** invert_matrix(GLfloat** srcC, int dim)
{
 GLfloat** src, **dst;
 GLfloat div, pom;
 register int x,k;
 int i,swit;
 GLfloat* vectmp;
 src = (GLfloat**)malloc(dim*sizeof(GLfloat*));
 for (i=0;i<dim;i++) src[i] = (GLfloat*)malloc(dim*sizeof(GLfloat));
 dst = (GLfloat**)malloc(dim*sizeof(GLfloat*));
 for (i=0;i<dim;i++) dst[i] = (GLfloat*)malloc(dim*sizeof(GLfloat));
 vectmp = (GLfloat*)malloc(dim*sizeof(GLfloat));
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

void compute_n(GLfloat** N, GLfloat* t)
{
 int i,j;
 GLfloat sum;
 for (i=0;i<=n;i++)
  for (j=0;j<=n;j++) 
    N[i][j] = bsp(t[i], p, j);
 for (i=0;i<=n;i++)
 {
  sum = 0.;
  for (j=0;j<=n;j++) sum += N[i][j];
/*  printf("sum=%f\n", sum);*/
  for (j=0;j<=n;j++) N[i][j] /= sum;
 }
}

GLfloat** matrix_mul(GLfloat** m, GLfloat** n, int ma, int mb, int na, int nb)
{
 GLfloat** dst;
 register int k,j,i;
 if (ma <= 0 || mb  <= 0 || na <= 0 || nb <=0 || mb != na) return NULL;
 if (!m || !n) return NULL;
 dst = (GLfloat**)malloc(ma*sizeof(GLfloat*));
 if (!dst) return NULL;
 for (i=0;i<ma;i++) dst[i] = (GLfloat*)malloc(nb*sizeof(GLfloat));
 
 for (i=0;i<ma;i++)
 for (j=0;j<nb;j++)
    {
     dst[i][j] = 0.0;
     for (k=0;k<mb;k++) dst[i][j] += m[i][k] * n[k][j];
    }
 return dst;
}

void update_curve()
{
 GLfloat* t;
 GLfloat** N;
 GLfloat** NI;
 GLfloat** P;
 GLfloat** D;
 List* tl;
 int i,j;
 nelem++;
 if (nelem <= p) { n = 0; return; }
 n = nelem - 1;
 t = (GLfloat*)malloc(nelem*sizeof(GLfloat));
 for (i=0;i<=n;i++)  t[i] = (GLfloat)i/(GLfloat)n;
 m = n+p+1;
 if (knots) { free(knots); knots = NULL; }
 knots = (GLfloat*)malloc((m+1)*sizeof(GLfloat));
 for (i=0;i<=p;i++) knots[i] = 0.;
 for (j=1;j<=n-p;j++)
    {
	knots[j+p] = 0.;
	for (i=j;i<=j+p-1;i++) knots[j+p] += t[i];
	knots[j+p] /= (GLfloat)p;
    }
 for (i=m-p;i<=m;i++) knots[i] = 1.;
 D = (GLfloat**)malloc(nelem*sizeof(GLfloat*));
 for (i=0;i<nelem;i++) D[i] = (GLfloat*)malloc(3*sizeof(GLfloat));
 tl = get_list_tail(list);
 for (i=0;i<nelem;i++)
   {
    if (!tl) { printf("Error!, no %d list element!\n", i); exit(4); }
    D[i][0] = tl->x;
    D[i][1] = tl->y;
    D[i][2] = tl->z;
    tl = tl->prev;
   }
 N = (GLfloat**)malloc(nelem*sizeof(GLfloat*));
 for (i=0;i<nelem;i++) N[i] = (GLfloat*)malloc(nelem*sizeof(GLfloat));
 compute_n(N, t);
 NI = invert_matrix(N, n+1);
 P = matrix_mul(NI, D, n+1, n+1, n+1, 3);
 if (ctlpts) { free(ctlpts); ctlpts = NULL; }
 ctlpts = (GLfloat*)malloc(nelem*3*sizeof(GLfloat));
 if (ctlpts) { free(ctlpts); ctlpts = NULL; }
 ctlpts = (GLfloat*)malloc(nelem*3*sizeof(GLfloat));
 if (datpts) { free(datpts); datpts = NULL; }
 datpts = (GLfloat*)malloc(nelem*3*sizeof(GLfloat));
 for (i=0;i<=n;i++) 
   {
    ctlpts[3*i] = P[i][0];
    ctlpts[3*i+1] = P[i][1];
    ctlpts[3*i+2] = P[i][2];
    datpts[3*i]   = D[i][0];
    datpts[3*i+1] = D[i][1];
    datpts[3*i+2] = D[i][2];
   }
 for (i=0;i<nelem;i++) free(N[i]);
 free(N);
 for (i=0;i<nelem;i++) free(NI[i]);
 free(NI);
 for (i=0;i<nelem;i++) free(P[i]);
 free(P);
 for (i=0;i<nelem;i++) free(D[i]);
 free(D);
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

void Init(int lb, char** par)
{
 if (lb < 2) p = 2;
 else p = atoi(par[1]);
 if (p < 1) exit(2);
 n = -1;
 nelem = 0;
 m = 0;
 list = NULL;
 knots = NULL;
 ctlpts = NULL;
 curx = cury = curz = 0.;
 angX = 0.;
 angY = 0.;
 angZ = 0.;
 help();
}

void glInit()
{
 glClearColor(0.,0.,0.,0.);
 glShadeModel(GL_SMOOTH);
}

void rotatex(GLfloat** m, double ang)
{
 ang /= 180./PI;
 m[1][1] = cos(ang);
 m[2][1] = sin(ang);
 m[1][2] = -sin(ang);
 m[2][2] = cos(ang);
}


void rotatey(GLfloat** m, double ang)
{
 ang /= 180./PI;
 m[0][0] = cos(ang);
 m[2][0] = -sin(ang);
 m[0][2] = sin(ang);
 m[2][2] = cos(ang);
}


void rotatez(GLfloat** m, double ang)
{
 ang /= 180./PI;
 m[0][0] = cos(ang);
 m[1][0] = sin(ang);
 m[0][1] = -sin(ang);
 m[1][1] = cos(ang);
}

void matrix_mul_vector(GLfloat* dst, GLfloat** m, GLfloat* v, int len)
{
 int i,j;
 for (i=0;i<len;i++)
    {
     dst[i] = 0.0;
     for (j=0;j<len;j++) dst[i] += v[j] * m[i][j];
    }
}

void anti_world(GLfloat* cux, GLfloat* cuy, GLfloat* cuz)
{
 GLfloat **m1,**m2,**m3, **mt, **m, **im;
 GLfloat *x, *rx;
 GLint i;
 m1 = (GLfloat**)malloc(3*sizeof(GLfloat*));
 for (i=0;i<3;i++) m1[i] = (GLfloat*)malloc(3*sizeof(GLfloat));
 m2 = (GLfloat**)malloc(3*sizeof(GLfloat*));
 for (i=0;i<3;i++) m2[i] = (GLfloat*)malloc(3*sizeof(GLfloat));
 m3 = (GLfloat**)malloc(3*sizeof(GLfloat*));
 for (i=0;i<3;i++) m3[i] = (GLfloat*)malloc(3*sizeof(GLfloat));
 I_matrix(m1, 3);
 I_matrix(m2, 3);
 I_matrix(m3, 3);
 rotatex(m1, angX);
 rotatey(m2, angY);
 rotatez(m3, angZ);
 mt = matrix_mul(m1, m2, 3, 3, 3, 3);
 m  = matrix_mul(mt, m3, 3, 3, 3, 3);
 for (i=0;i<3;i++) { free(m1[i]); free(m2[i]); free(m3[i]); free(mt[i]); }
 free(m1);
 free(m2);
 free(m3);
 free(mt);
 x  = (GLfloat*)malloc(3*sizeof(GLfloat));
 rx = (GLfloat*)malloc(3*sizeof(GLfloat));
 x[0] = *cux;
 x[1] = *cuy;
 x[2] = *cuz;
 im = invert_matrix(m, 3);
 matrix_mul_vector(rx, im, x, 3);
 free(x);
 *cux = rx[0];
 *cuy = rx[1];
 *cuz = rx[2];
 free(rx);
 for (i=0;i<3;i++) { free(m[i]); free(im[i]); }
 free(m);
 free(im);
}

void mouse(int b, int s, int x, int y)
{
 curx = (x-wx/2.)*(800./wx);
 cury = (wy/2.-y)*(800./wy);
 curz = 0.;
 anti_world(&curx, &cury, &curz);
 if (s == 1 && b == 0) 
   {
    list_add(&list, curx, cury, curz); 
    update_curve();
   }
 glutPostRedisplay();
}



int main(int lb, char** par)
{
 wx = 800;
 wy = 600;
 Init(lb, par);
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(100, 100);
 glutCreateWindow(par[0]);
 glInit();
 glutDisplayFunc(render_scene);
 glutReshapeFunc(resize_scene);
 glutKeyboardFunc(keyboard);
 glutVisibilityFunc(visible);
 glutMouseFunc(mouse);
 glutMainLoop();
 return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
