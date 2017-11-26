#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include "odeparser.h"

#define real long double

typedef struct _List
{
 struct _List *next, *prev;
 real xo, yo, zo;
} List;

double angX, angY, angZ, obsD, tStep, scale;	
double tx, ty, tz;
int wx,wy;		
int cx,cy;
int tail_length;
int follow, sel;

real *xo, *yo, *zo, H;
int rank, nEqu, nMap;
real prevx, *prevy, *prevyp;
real newx, *newy, *newyp, *curr_ypp;
List** head;


void list_add(List** head, real xx, real yy, real zz)
{
 List* temp;
 if (*head == NULL)
   {
    *head = (List*)(malloc(sizeof(List)));
    (*head)->xo = xx;
    (*head)->yo = yy;
    (*head)->zo = zz;
    (*head)->next = NULL;
    (*head)->prev = NULL;
    return;
   }
  temp = (List*)(malloc(sizeof(List)));
  temp->xo = xx;
  temp->yo = yy;
  temp->zo = zz;
  temp->next = *head;
  temp->prev = NULL;
  (*head)->prev = temp;
  *head = temp;
}

void list_tail_free(List** head)
{
 List *temp, *prev;
 if (!head || !*head) return;
 if ((*head)->prev) (*head)->prev->next = NULL;
 temp = *head;
 while (temp)
   {
    prev = temp;
    temp = temp->next;
    prev->next = NULL;
    prev->prev = NULL;
    free(prev);
   }
 *head = NULL;
}

void normals()
{
 scale = 1.;
 angX = angZ = tx = ty = tz = 0.;
 angY = 0.;
}

void keyboard(unsigned char key, int x, int y)
{
 int i;
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
	case '3': tail_length = (int)((float)tail_length/1.2); break;
	case '4': tail_length = (int)((float)tail_length*1.2); break;
	case '5': H /= 1.1; break;
	case '6': H *= 1.1; break;
	case '7': sel--; if (sel < 0)     sel = nMap-1; break;
	case '8': sel++; if (sel >= nMap) sel = 0;      break;
	case 'i': ty += 0.05*scale; break;
	case 'k': ty -= 0.05*scale; break;
	case 'j': tx -= 0.05*scale; break;
	case 'l': tx += 0.05*scale; break;
	case 'o': tz += 0.05*scale; break;
	case 'm': tz -= 0.05*scale; break;
	case ' ': normals(); break;
	case 'f': follow = !follow; break;
   }
 if (tail_length < 5) tail_length = 5;
 if (tail_length > 20000) tail_length = 20000;
 
 printf("ang(%3.0f,%3.0f,%3.0f), trans(%3.2f,%3.2f,%3.2f)\n", angX, angY, angZ, tx, ty, tz);
 printf("tail=%d, nEqu=%d, rank=%d, H=%Lf\n", tail_length, nEqu, rank, H);
 for (i=0;i<nMap;i++)
  {
   printf("Map%d=[%Lf,%Lf,%Lf]\n", i, xo[i], yo[i], zo[i]);
  }
 printf("Integral:\n");
 printf("t=%Lf\n", newx);
 for (i=0;i<nEqu;i++)
 {
  if (rank == 2) printf("y%d=%Lf\ty%d'=%Lf\ty%d\"=%Lf\n", 
	  i, newy[i], i, newyp[i], i, curr_ypp[i]);
  if (rank == 1) printf("y%d=%Lf\ty%d'=%Lf\n", i, newy[i], i, newyp[i]);
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

void render_object(int num)
{
 double r,g,b;
 r = g = b = 0.7;
 switch (num % 7)
   {
    case 6: r = 0.0; g = 0.0; b = 0.7; break;
    case 5: r = 0.0; g = 0.7; b = 0.0; break;
    case 4: r = 0.0; g = 0.7; b = 0.7; break;
    case 3: r = 0.7; g = 0.0; b = 0.0; break;
    case 2: r = 0.7; g = 0.0; b = 0.7; break;
    case 1: r = 0.7; g = 0.7; b = 0.0; break;
    case 0: r = 0.7; g = 0.7; b = 0.7; break;
   }
 glColor3d(r, g, b);
 glPushMatrix();
 glTranslated(xo[num], yo[num], zo[num]);
 glutWireSphere(.1/scale, 4, 4);
 glPopMatrix();
 /*glBegin(GL_LINES);
  glVertex3d(0., 0., 0.);
  glVertex3d(xo, yo, zo);
 glEnd();*/
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

void render_curve_tail(int num)
{
 double r,g,b;
 List* temp;
 int k;
 temp = head[num];
 k = 0;
 r = g = b = 0.4;
 switch (num % 7)
   {
    case 6: r = 0.0; g = 0.0; b = 0.4; break;
    case 5: r = 0.0; g = 0.4; b = 0.0; break;
    case 4: r = 0.0; g = 0.4; b = 0.4; break;
    case 3: r = 0.4; g = 0.0; b = 0.0; break;
    case 2: r = 0.4; g = 0.0; b = 0.4; break;
    case 1: r = 0.4; g = 0.4; b = 0.0; break;
    case 0: r = 0.4; g = 0.4; b = 0.4; break;
   }
 glColor3d(r, g, b);
 glBegin(GL_LINE_STRIP);
   while  (temp)
     {
      glVertex3d(temp->xo, temp->yo, temp->zo);
      temp = temp->next;
      k++;
      if (k >= tail_length) list_tail_free(&temp);
     }
 glEnd();
}

void do_runge_kutta_step(real);

void render_scene(void)
{
 int i;
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_BLEND);
 glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
 glLoadIdentity();
 gluLookAt(0., 0., 10., 0. , 0., 0., 0., 1., 0.);
 glTranslated(tx, ty, tz);
 glRotated(angX, 1., 0., 0.);
 glRotated(angY, 0., 1., 0.);
 glRotated(angZ, 0., 0., 1.);
 glScaled(scale, scale, scale);
 if (follow) glTranslated(-xo[sel], -yo[sel], -zo[sel]);
 for (i=0;i<nMap;i++)
   {
    render_object(i);
    render_curve_tail(i);
   }
 render_zero();
 glFlush();
 glutSwapBuffers();
 do_runge_kutta_step(H);
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
 follow = 0;
 sel = 0;
 angX = angZ = tx = ty = tz = 0.;
 angY = 0.;
 tail_length = 250;
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


void runge_kutta_dsystem_single_step(
	int rank, int nequ,
	real  x0, 
	real*  y0, real*  yp0, 
	real h,
	real* x1, 
	real* y1, real* yp1)
{
 int i;
 real *k1,*k2,*k3,*k4;
 real *kk0, *kk1;
 real *yytab, *yyptab;
 real h2;
 
 h2 = h/2.L;
 if (rank == 1)
 {
 k1 = (real*)malloc(nequ*sizeof(real));
 k2 = (real*)malloc(nequ*sizeof(real));
 k3 = (real*)malloc(nequ*sizeof(real));
 k4 = (real*)malloc(nequ*sizeof(real));
 kk0 = (real*)malloc(nequ*sizeof(real));
 kk1 = NULL;
 yytab  = (real*)malloc(nequ*sizeof(real));
 yyptab = NULL;
 
 for (i=0;i<nequ;i++) yytab[i]  = y0[i];
 for (i=0;i<nequ;i++) k1[i] = h * fpar_f(i, x0, yytab, NULL, NULL);
 
 for (i=0;i<nequ;i++) 
   {
    yytab[i]  = y0[i] + k1[i]/2.L;
   }
 for (i=0;i<nequ;i++) k2[i] = h * fpar_f(i, x0+h2, yytab, NULL, NULL);
 
 for (i=0;i<nequ;i++) 
   {
    yytab[i]  = y0[i] + k2[i]/2.L;
   }
 for (i=0;i<nequ;i++) k3[i] = h * fpar_f(i, x0+h2, yytab, NULL, NULL);
 
 for (i=0;i<nequ;i++) 
   {
    yytab[i]  = y0[i] + k3[i];
   }
 for (i=0;i<nequ;i++) k4[i] = h * fpar_f(i, x0+h, yytab, NULL, NULL);

 for (i=0;i<nequ;i++)
   {
    kk0[i] = (k1[i]+2.L*k2[i]+2.L*k3[i]+k4[i])/6.L;
   }
 
 *x1  = x0 + h;

 for (i=0;i<nequ;i++)
   {
    y1[i]  = y0[i] + kk0[i];
   }
 free(k1);
 free(k2);
 free(k3);
 free(k4);
 free(kk0);
 free(yytab);
 }
 
 if (rank == 2)
 {
 k1 = (real*)malloc(nequ*sizeof(real));
 k2 = (real*)malloc(nequ*sizeof(real));
 k3 = (real*)malloc(nequ*sizeof(real));
 k4 = (real*)malloc(nequ*sizeof(real));
 kk0 = (real*)malloc(nequ*sizeof(real));
 kk1 = (real*)malloc(nequ*sizeof(real));
 yytab  = (real*)malloc(nequ*sizeof(real));
 yyptab = (real*)malloc(nequ*sizeof(real));
 
 for (i=0;i<nequ;i++) 
   {
    yytab[i]  = y0[i];
    yyptab[i] = yp0[i];
   }
 for (i=0;i<nequ;i++) k1[i] = h * h2 * fpar_f(i, x0, yytab, yyptab, NULL);
 
 for (i=0;i<nequ;i++) 
   {
    yytab[i]  = y0[i] + h2*yp0[i] + k1[i]/4.L;
    yyptab[i] = yp0[i] + k1[i]/h;
   }
 for (i=0;i<nequ;i++) k2[i] = h * h2 * fpar_f(i, x0+h2, yytab, yyptab, NULL);
 
 for (i=0;i<nequ;i++) 
   {
/*    yytab[i]  = y0[i] + h2*yp0[i] + k1[i]/4.L;*/
    yyptab[i] = yp0[i] + k2[i]/h;
   }
 for (i=0;i<nequ;i++) k3[i] = h * h2 * fpar_f(i, x0+h2, yytab, yyptab, NULL);
 
 for (i=0;i<nequ;i++) 
   {
    yytab[i]  = y0[i] + h*yp0[i] + k3[i];
    yyptab[i] = yp0[i] + (2.L*k3[i])/h;
   }
 for (i=0;i<nequ;i++) k4[i] = h * h2 * fpar_f(i, x0+h, yytab, yyptab, NULL);

 for (i=0;i<nequ;i++)
   {
    kk0[i] = (k1[i]+k2[i]+k3[i])/3.L;
    kk1[i] = (k1[i]+(2.L*k2[i])+(2.L*k3[i])+k4[i])/3.L;
   }
 
 *x1  = x0 + h;

 for (i=0;i<nequ;i++)
   {
    y1[i]  = y0[i] + h*yp0[i] + kk0[i];
    yp1[i] = yp0[i] + kk1[i]/h;
   }
 free(k1);
 free(k2);
 free(k3);
 free(k4);
 free(kk0);
 free(kk1);
 free(yytab);
 free(yyptab);
 }
}

void do_runge_kutta_step(real h)
{
 int i;
 runge_kutta_dsystem_single_step(
	    rank, nEqu,
	    prevx, 
	    prevy, prevyp, 
	    h, 
	    &newx, 
	    newy, newyp
	    );
 prevx = newx;
 for (i=0;i<nEqu;i++) 
   {
    prevy[i]  = newy[i];
   }
 if (rank == 2)
 {
 for (i=0;i<nEqu;i++) 
   {
    prevyp[i]  = newyp[i];
   }
 }

 if (rank == 2)
  {
   for (i=0;i<nEqu;i++) curr_ypp[i] = fpar_f(i, newx, newy, newyp, NULL);
   for (i=0;i<nMap;i++)
     {
      xo[i] = fpar_f(nEqu+3*i, newx, newy, newyp, curr_ypp);
      yo[i] = fpar_f(nEqu+3*i+1, newx, newy, newyp, curr_ypp);
      zo[i] = fpar_f(nEqu+3*i+2, newx, newy, newyp, curr_ypp);
     }
  }
 if (rank == 1)
  {
   for (i=0;i<nEqu;i++) newyp[i] = fpar_f(i, newx, newy, NULL, NULL);
   for (i=0;i<nMap;i++)
     {
      xo[i] = fpar_f(nEqu+3*i, newx, newy, newyp, NULL);
      yo[i] = fpar_f(nEqu+3*i+1, newx, newy, newyp, NULL);
      zo[i] = fpar_f(nEqu+3*i+2, newx, newy, newyp, NULL);
     }
  }
 
 for (i=0;i<nMap;i++) list_add(&head[i], xo[i], yo[i], zo[i]);
}

void runge_kutta_first_step(
	int r, int ne,
	real x0,
	real* y0, real* yp0,
	real h)
{
 int i;
 nEqu = ne;
 rank = r;
 prevy  = (real*)malloc(nEqu*sizeof(real));
 newy   = (real*)malloc(nEqu*sizeof(real));
 newyp  = (real*)malloc(nEqu*sizeof(real));
 prevyp = (real*)malloc(nEqu*sizeof(real));
 
 if (rank == 2)
  {
   curr_ypp  = (real*)malloc(nEqu*sizeof(real));
  }
 else
  {
   curr_ypp = NULL;
  }

 prevx = x0;
 for (i=0;i<nEqu;i++)
   {
    prevy[i]  = y0[i];
   }
 if (rank == 2)
 {
 for (i=0;i<nEqu;i++)
   {
    prevyp[i]  = yp0[i];
   }
 }
 do_runge_kutta_step(H);
}

void parser_ndsolver(char* fn)
{
 FILE* fp;
 int ne,nm,r,i,j,k;
 int d1,d2;
 char buff[4096];
 real t0;
 real *y0, *yp0;
 fp = fopen(fn, "r");
 if (!fp) { printf("Cannot open: %s\n", fn); return; }
 ne = -1;
 fscanf(fp, "Number of equations: %d\n", &ne);
 if (ne <= 0) { printf("bad number of equations: %d\n", ne); return; }
 nm = -1;
 fscanf(fp, "Number of mappings: %d\n", &nm);
 if (nm <= 0) { printf("bad number of mappings: %d\n", nm); return; }
 r = -1;
 fscanf(fp, "Rank of equations system: %d\n", &r);
 if (r != 1 && r!= 2) 
   { printf("bad rank of equations system: %d\n", r); return; }
 H = -1.L;
 fscanf(fp, "Integral step: %Lf\n", &H);
 if (H <= 0.L) 
   { printf("bad integral step: %Lf\n", H); return; }
 fscanf(fp, "Start conditions:\n");
 fscanf(fp,"t0=%Lf\n", &t0);
 y0  = (real*)malloc(ne*sizeof(real));
 if (r == 2) yp0 = (real*)malloc(ne*sizeof(real));
 else yp0 = NULL;
/* printf("t0 = %Lf\n", t0);*/
 for (i=0;i<ne;i++)
   {
    d1 = d2 = -1;
    if (r == 2)
     {
      fscanf(fp, "y%d(t0)=%Lf, y%d'(t0)=%Lf\n", &d1, &y0[i], &d2, &yp0[i]);
/*    printf("d1=%d, d2=%d, y=%Lf, y'=%Lf\n", d1, d2, y0[i], yp0[i]);*/
      if (d1 != i+1 || d2 != i+1) { printf("Bad start conditions at: %d\n", i); return; }
     }
    else
     {
      fscanf(fp, "y%d(t0)=%Lf\n", &d1, &y0[i]);
      if (d1 != i+1) { printf("Bad start conditions at: %d\n", i); return; }
     }
   }
 fscanf(fp, "Differential equations system:\n");
 fpar_init(ne+3*nm);
 for (i=0;i<ne;i++)
   {
/*       printf("i = %d\n", i);*/
/*    pos = ftell(fp); */
    k = 0;
    while ((buff[k] = fgetc(fp)) != '\n' && buff[k] != EOF) k++;
    buff[k] = '\n';
    buff[k+1] = 0;
    k = -1;
    if (r == 2) sscanf(buff, "y%d\"=", &k);
    else sscanf(buff, "y%d'=", &k);
    if (k != i+1) { printf("bad equations order, at %d\n", i); return; }
    k = 0;
    while (buff[k] != '=' && k < strlen(buff)) k++;
    k++;
/*    printf("Buff = '%s'\n", buff+k);*/
    fpar_function(i, ne, buff+k);
    if (!fpar_ok(i)) { printf("Function not accepted.\n"); return; }
   }
 fscanf(fp, "Mapping functions:\n");
/* printf("Mappers...\n");*/
 for (j=0;j<nm;j++)
 {
 for (i=0;i<3;i++)
   {
    k = 0;
    while ((buff[k] = fgetc(fp)) != '\n' && buff[k] != EOF) k++;
    buff[k] = '\n';
    buff[k+1] = 0;
    k = 0;
    switch (i)
      {
	  case 0: sscanf(buff, "X%d=", &k); if (k != j+1) k = 0; break;
	  case 1: sscanf(buff, "Y%d=", &k); if (k != j+1) k = 0; break;
	  case 2: sscanf(buff, "Z%d=", &k); if (k != j+1) k = 0; break;
      }
    if (k == 0) { printf("Bad %d mapping order for %dth screen coord\n", j, i+1); return; }
    k = 0;
    while (buff[k] != '=' && k < strlen(buff)) k++;
    k++;
/*    printf("Buff = '%s'\n", buff+k);*/
    fpar_function(3*j+i+ne, ne, buff+k);
    if (!fpar_ok(3*j+i+ne)) return;
   }
 }

 nMap = nm;
 
 xo = (real*)malloc(nMap*sizeof(real));
 yo = (real*)malloc(nMap*sizeof(real));
 zo = (real*)malloc(nMap*sizeof(real));
 head = (List**)malloc(nMap*sizeof(List*));
 for (i=0;i<nMap;i++) head[i] = NULL;
 fclose(fp);
 runge_kutta_first_step(r, ne, t0, y0, yp0, H);
 free(y0);
 free(yp0);
 opengl_display();
}

void help()
{
 printf("parameters: filename\n\texample:dsystem.txt\n");
 printf("keys: wsadex ikjlom 12345678  f q SPACE\n");
}

int main(int lb, char** par)
{
 if (lb < 2) { help(); return 1; }
 parser_ndsolver(par[1]);
 return 0;
}

