/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#define PI 3.1415926F
GLfloat angX, angY, angZ, obsD, tStep;	
int wx,wy;		
int cx,cy;			
GLfloat angX, angY, angZ;
GLfloat** D;
GLfloat** P;
GLfloat** N;
GLfloat** NI;
int n;
int p;
int s;
int m;
GLfloat* t;
GLfloat* knot;
GLfloat* ctlpts;

GLUnurbs* theNurb;

void help()
{
 printf("usage ./program [file]\n");
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
/* printf("spline(t=%f, dim=%d (%d,%d)[%f,%f]\n", val, dim, Ioff, Joff, knot[Ioff], knot[Joff]);*/
 if (I>J)
   {
     printf("I more than J in BSPLINE basis");
     exit(1);
     return 0.0;
   }
 if (dim == 0)
   {
    if (val >= knot[Ioff] && val <= knot[Joff]) return 1.0;
    else return 0.0;
   }
 up = (val-knot[Ioff]) * basis_bspline(val, dim-1, off, I, J-1);
 down = knot[Joff-1] - knot[Ioff];
 if (down==0.0) factor1 = 0.0;
 else factor1 = up/down;
 up = (knot[Joff]-val) * basis_bspline(val, dim-1, off, I+1, J);
 down = knot[Joff] - knot[Ioff+1];
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
 for (i=0;i<=p;i++)
   {
    b = bsp(t, p, i);
    factor = P[i][xyz]*b;
    up += factor;
    down += b;
   }
 if (down==0.0) return 0.0;
 return up/down;
}

void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
   }
}

void resize_scene(int w, int h)
{
 wx = w;
 wy = h;
 glViewport(0, 0, (GLsizei)wx, (GLsizei)wy);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1., 1., -1., 1., 1.5, 20.);
 glMatrixMode(GL_MODELVIEW);
}

void render_scene(void)
{
 GLint i;
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_BLEND);
 glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
 glLoadIdentity();
 gluLookAt(0., 0., 4., 0. , 0., 0., 0., 1., 0.);
 glRotated(angX, 1., 0., 0.);
 glRotated(angY, 0., 1., 0.);
 glRotated(angZ, 0., 0., 1.);
 glTranslated(-1., -1., 0.);
 glScaled(.1, .1, .1);
 glColor3d(1., 0., 0.);
 glBegin(GL_LINE_STRIP);
 for (i=0;i<=n;i++) glVertex3d(D[i][0], D[i][1], D[i][2]);
 glEnd();
 glColor3d(0., 1., 0.);
 glBegin(GL_LINE_STRIP);
 for (i=0;i<=n;i++) 
   {
    glVertex3d(P[i][0], P[i][1], P[i][2]);
    ctlpts[3*i] = P[i][0];
    ctlpts[3*i+1] = P[i][1];
    ctlpts[3*i+2] = P[i][2];
   }
 glEnd();
 glColor3d(0., 0., 1.);
 glBegin(GL_LINE_STRIP);
 glEnd();
 gluBeginCurve(theNurb);
 gluNurbsCurve(theNurb, m+1, knot, 3, ctlpts, p+1, GL_MAP1_VERTEX_3);
 gluEndCurve(theNurb);
 glFlush();
 glutSwapBuffers();
 angX += .3;
 angY += .5;
 angZ += .7;
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

GLfloat** matrix(int siz)
{
 GLfloat** mem;
 int i;
 if (siz <= 0) return NULL;
 mem = (GLfloat**)malloc(siz*sizeof(GLfloat*));
 for (i=0;i<siz;i++) mem[i] = vector(siz);
 return mem;
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

void print_vector(GLfloat* v, int siz, char* name)
{
 int j;
 if (name) printf("%s(%p) {", name, (void*)v);
 else printf("NONAME(%p) {\n", (void*)v);
 for (j=0;j<siz;j++) printf("%f\t", v[j]);
 printf("}\n");
}

void print_matrix(GLfloat** m, int siz, char* name)
{
 int i,j;
 if (name) printf(" ==== %s(%p) ====\n", name, (void*)m);
 else printf(" ==== NONAME(%p) ====\n", (void*)m);
 for (i=0;i<siz;i++)
   {
    for (j=0;j<siz;j++) printf("%f\t", m[i][j]);
    printf("\n");
   }
}

void print_matrix2(GLfloat** m, int siz1, int siz2, char* name)
{
 int i,j;
 if (name) printf(" ==== %s(%p) ====\n", name, (void*)m);
 else printf(" ==== NONAME(%p) ====\n", (void*)m);
 for (i=0;i<siz1;i++)
   {
    for (j=0;j<siz2;j++) printf("%f\t", m[i][j]);
    printf("\n");
   }
}


void read_curve_data()
{
    int i,j;
    scanf("%d,%d,%d\n", &n, &p, &s);
    if (n < 1 || n > 128) exit(2);
    if (p < 1 || n > 12) exit(3);
    if (s < 1 || s > 16) exit(4);
    D = matrix2(n+1, s);
    t = (GLfloat*)malloc((n+1)*sizeof(GLfloat));
    for (i=0;i<=n;i++) 
    {
	scanf("(");
	for (j=0;j<s-1;j++) scanf("%f,", &D[i][j]);
	scanf("%f)\n", &D[i][s-1]);
/*	printf("%f,%f,%f\n", D[i][0], D[i][1], D[i][2]);*/
    }
    for (i=0;i<=n;i++) 
    {
	t[i] = (GLfloat)i/(GLfloat)n;
    }
    m = n+p+1;
    knot = (GLfloat*)malloc((m+1)*sizeof(GLfloat));
    for (i=0;i<=p;i++) knot[i] = 0.;
    for (j=1;j<=n-p;j++)
    {
	knot[j+p] = 0.;
	for (i=j;i<=j+p-1;i++) 
	{
	    knot[j+p] += t[i];
	}
	knot[j+p] /= (GLfloat)p;
    }
    for (i=m-p;i<=m;i++) knot[i] = 1.;
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
	   print_matrix(srcC, dim, "src");
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

GLfloat** matrix_mul_old(GLfloat** m, GLfloat** n, int A, int B, int C)
{
 GLfloat** dst;
 register int k,j,i;
 dst = matrix2(A, C);
 if (!m || !n || !dst) return NULL;
 /*print_matrix2(n, B, C, "B");
 print_matrix(m, A, "A");*/
 for (i=0;i<C;i++)
 for (j=0;j<B;j++)
    {
     dst[j][i] = 0.0;
     for (k=0;k<A;k++) dst[j][i] += m[j][k] * n[k][i];
    }
 return dst;
}

void compute_n()
{
 int i,j;
 for (i=0;i<=n;i++)
  for (j=0;j<=n;j++) 
    N[i][j] = bsp(t[i], p, j);
}

GLfloat** matrix_mul(GLfloat** m, GLfloat** n, int ma, int mb, int na, int nb)
{
 GLfloat** dst;
 register int k,j,i;
 int I,J,K;
 if (ma <= 0 || mb  <= 0 || na <= 0 || nb <=0 || nb != ma) return NULL;
 if (!m || !n) return NULL;
 I = na;
 J = mb;
 K = ma;
 /*printf("MUL:\n");
 print_matrix2(m, ma, mb, "m");
 print_matrix2(n, na, nb, "n");
 printf("MUL ENDS.\n");*/
/* printf("I = %d, J = %d, K = %d\n", I,J,K);*/
 dst = matrix2(I, J);
 if (!dst) return NULL;
 for (i=0;i<I;i++)
 for (j=0;j<J;j++)
    {
     dst[i][j] = 0.0;
     for (k=0;k<K;k++) dst[i][j] += n[i][k] * m[k][j];
    }
 return dst;
}

GLfloat** transpone_matrix(GLfloat** m, int x, int y)
{
 GLfloat** mt;
 int i,j;
 if (x <= 0 || y <= 0 || !m) return NULL;
 mt = matrix2(y,x);
 if (!mt) return NULL;
 for (i=0;i<x;i++)
 for (j=0;j<y;j++) mt[j][i] = m[i][j];
 return mt;
}

void free_matrix(GLfloat** m, int siz)
{
 int i;
 if (siz <= 0) return;
 for (i=0;i<siz;i++) free(m[i]);
 free(m);
}

void Init()
{
 GLfloat** I; 
 GLfloat** D2;
 GLfloat** DT;
 GLfloat** PT;
 GLfloat** D2T;
 GLfloat** NIT;
 float precision;
 read_curve_data();
 printf("n=%d, p=%d, m=%d\n", n, p, m);
 print_vector(t, n+1, "t");
 print_vector(knot, m+1, "knot");
 print_matrix2(D, n+1, s, "D");

 DT = transpone_matrix(D, n+1, s);
 print_matrix2(DT, s, n+1, "DT");
 

 N = matrix(n+1);
 compute_n();
 print_matrix(N, n+1, "N");
 
 NI = invert_matrix(N, n+1);
 print_matrix(NI, n+1, "NI");
 
 NIT = transpone_matrix(NI, n+1, n+1);
 print_matrix2(NIT, n+1, n+1, "NIT");

 
/* P = matrix_mul(D, NIT, n+1, s, n+1, n+1); */
/* P = matrix_mul(D, NI, n+1, s, n+1, n+1); */
 PT = matrix_mul(NIT, DT, n+1, n+1, s, n+1);
 P  = transpone_matrix(PT, s, n+1);
/* P = matrix_mul_old(NI, D, n+1, n+1, s); */
/* P = matrix_mul(D, NI, n+1, s, n+1);*/
/* print_matrix2(PT, s, n+1, "PT");*/
 print_matrix2(P, n+1, s, "P");
 
 /* tests */
 
 I = matrix_mul(N, NI, n+1, n+1, n+1, n+1);
 print_matrix(I, n+1, "I");

 PT = transpone_matrix(P, n+1, s);
 D2T = matrix_mul(N, PT, n+1, n+1, s, n+1);
 D2 = transpone_matrix(D2T, s, n+1);
 print_matrix2(D2, n+1, s, "D2");

 free_matrix(NI, n+1);
 free_matrix(NIT, n+1);
 free_matrix(N, n+1);
 free_matrix(D2, n+1);
 free_matrix(D2T, s);
 free_matrix(DT, s);
 free_matrix(PT, s);
 
 precision = 10.;

 theNurb = gluNewNurbsRenderer();
 gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, precision);
 gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

 ctlpts = (GLfloat*)malloc(3*(n+1)*sizeof(GLfloat));
 

}

void glInit()
{
 angX = angY = angZ = 0.;
 help();
 glClearColor(0.,0.,0.,0.);
 glShadeModel(GL_SMOOTH);
}



int main(int lb, char** par)
{
 wx = 800;
 wy = 600;
 Init();
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
 glutMainLoop();
 return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
