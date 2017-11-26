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
int cskip, allCurve;

real xo, yo, zo, H;
real prevx, prevy, prevz, prevv, prevyp, prevzp, prevvp;
real newx, newy, newz, newv, newyp, newzp, newvp;
List* head;


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
	case 'i': ty += 0.05*scale; break;
	case 'k': ty -= 0.05*scale; break;
	case 'j': tx -= 0.05*scale; break;
	case 'l': tx += 0.05*scale; break;
	case 'o': tz += 0.05*scale; break;
	case 'm': tz -= 0.05*scale; break;
	case ' ': normals(); break;
   }
 if (tail_length < 5) tail_length = 5;
 if (tail_length > 20000) tail_length = 20000;
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

void render_object()
{
 glColor3d(.7, .5, 0.);
 glPushMatrix();
 glTranslated(xo, yo, zo);
 glutWireSphere(.1/scale, 8, 8);
 glPopMatrix();
 glBegin(GL_LINES);
  glVertex3d(0., 0., 0.);
  glVertex3d(xo, yo, zo);
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

void render_curve_tail()
{
 List* temp;
 int k;
 temp = head;
 k = 0;
 glColor3d(.3, .3, .3);
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
 render_object();
 render_curve_tail();
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

 *x1  = x0 + h;
 
 *y1  = y0 + h*yp0 + kk0;
 *yp1 = yp0 + kk1/h;
 
 *z1  = z0 + h*zp0 + ll0;
 *zp1 = zp0 + ll1/h;
 
 *v1  = v0 + h*vp0 + mm0;
 *vp1 = vp0 + mm1/h;

}

void do_runge_kutta_step(real h)
{
 runge_kutta_dsystem_single_step(
	    parser_func1, 
	    parser_func2, 
	    parser_func3,
	    prevx, 
	    prevy, prevyp, 
	    prevz, prevzp, 
	    prevv, prevvp, 
	    h, 
	    &newx, 
	    &newy, &newyp,
	    &newz, &newzp,
	    &newv, &newvp
	    );
 prevx = newx;
 prevy = newy;
 prevz = newz;
 prevv = newv;
 prevyp = newyp;
 prevzp = newzp;
 prevvp = newvp;
 xo = fpar3_f(FUNC_MAPX, newy, newz, newv);
 yo = fpar3_f(FUNC_MAPY, newy, newz, newv);
 zo = fpar3_f(FUNC_MAPZ, newy, newz, newv);
 list_add(&head, xo, yo, zo);
}

void runge_kutta_first_step(
	real x0,
	real y0, real yp0,
	real z0, real zp0,
	real v0, real vp0,
	real h)
{
 prevx  = x0;
 prevy  = y0;
 prevyp = yp0;
 prevz  = z0;
 prevzp = zp0;
 prevv  = v0;
 prevvp = vp0;
 do_runge_kutta_step(H);
}

void parser_ndsolver(real t0,
		real y0, real yp0,
		real z0, real zp0,
		real v0, real vp0,
		real h)
{
 H = h;
 head = NULL;
 runge_kutta_first_step(
	 t0, 	    /* time zero */
	 y0, yp0,        /* y(t0), y'(t0) */
	 z0, zp0,        /* z(t0), z'(t0) */
	 v0, vp0,        /* v(t0), v'(t0) */
	 h		    /* h: integral step */
	 );
 opengl_display();
}

void help()
{
 printf("parameters: func_def1 func_def2 func_def3\n");
 printf("pardef1 pardef2 pardef3 t0 y0 yp0 z0 zp0 v0 vp0 h\n");
 printf("Example: You want to solve 3 differential equations:\n");
 printf("y''=z'+y-t and z''=z'+y'-cos(z)-sin(y) and v''=-v+t\n");
 printf("y(t0)=1, z(t0)=0, v(t0)=-3, y'(t0)=-1, z'(t0)=2, v'(t0)=4\n");
 printf("in t=[0,inf], timestep: 0.01, then:\n");
 printf("Solver will give You {y,y',z,z',v,v'}(t) in [t0,inf] with step=h\n");
 printf("You can define parametrizationusing: y(t)=a, z(t)=b, v(t)=c\n");
 printf("on circle 'cos(a)*sin(b)', '-cos(b)', 'sin(a)*sin(b)'\n");
 printf("./nsolve 'zp-y-t' 'zp-yp-cos(z)-sin(y)' '-v+t' 'cos(a)*sin(b)'\n");
 printf("'-cos(b)' 'sin(a)*sin(b)' 0 1 -1 0 2 -3 4 0.01\n");
 printf("keys: wsadex(rotations), ikjlom(translations), 12(scale)\n");
 printf("SPACE(reset), 34(tail_length) q,ESC(quit)\n");
}

int main(int lb, char** par)
{
/* nsolve1();*/
/* nsolve2();*/
 if (lb < 15) { help(); return 1; }
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
	 atof(par[8]),	/*  y0 */
	 atof(par[9]),	/*  yp0 */
	 atof(par[10]),	/*  z0 */
	 atof(par[11]),	/*  zp0 */
	 atof(par[12]),	/*  v0 */
	 atof(par[13]),	/*  vp0 */
	 atof(par[14])	/*  h */
	     );
 return 0;
}

