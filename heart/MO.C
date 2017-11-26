#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define MATH_PI 3.1415926353
#define WS 768

unsigned char pix[WS*WS*3];

typedef struct _PStack
{
GLdouble x,y;
struct _PStack *next, *prev;
} PathStack;

GLint empty(PathStack* ps)
{
return (ps == NULL);
}

void push_path(PathStack** ps, GLdouble x, GLdouble y)
{
PathStack* tmp;
/* printf("PUSH %p\n", (void*)(*ps));*/
tmp = (PathStack*)malloc(sizeof(PathStack));
tmp->x = x;
tmp->y = y;
if (!(*ps)) { tmp->next = 0; tmp->prev = 0; *ps = tmp; }
else { tmp->next = *ps; (*ps)->prev = tmp; tmp->prev = 0; *ps = tmp; }
}

void set_color(GLint i, GLint j, GLint r, GLint g, GLint b)
{
if (i < 0 || i >= WS) return;
if (j < 0 || j >= WS) return;
pix[3*(WS*i+j)] = r;
pix[3*(WS*i+j)+1] = g;
pix[3*(WS*i+j)+2] = b;
}

void put_double(GLdouble x, GLdouble y, GLint r, GLint g, GLint b)
{
GLint i,j;
i = (GLint)((x + 1.5)*(WS/3));
j = (GLint)((y + 1.5)*(WS/3));
set_color(i, j, r, g, b);
}

void put_double_fat(GLdouble x, GLdouble y, GLint r, GLint g, GLint b)
{
GLint i,j;
i = (GLint)((x + 1.5)*(WS/3));
j = (GLint)((y + 1.5)*(WS/3));
set_color(i-1, j-1, r, g, b);
set_color(i-1, j, r, g, b);
set_color(i-1, j+1, r, g, b);
set_color(i, j-1, r, g, b);
set_color(i, j, r, g, b);
set_color(i, j+1, r, g, b); 
set_color(i+1, j-1, r, g, b);
set_color(i+1, j, r, g, b);
set_color(i+1, j+1, r, g, b);
}

void draw_circle()
{
GLdouble angle;
GLdouble step;
GLdouble x,y;
step = MATH_PI / 1800.;
for (angle = 0.; angle < 2*MATH_PI; angle += step)
{
x = sin(angle);
y = cos(angle);
//printf("%f,%f\n", x,y);
put_double(x, y, 0xff, 0, 0);
}
}

GLdouble distance(PathStack* a, PathStack* b)
{
if (!a || !b) return -1;
return sqrt((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
}

void atan_cast(GLdouble x, GLdouble y)
{
GLdouble a;
GLdouble nx, ny;
a = atan(fabs(x) / fabs(y));
if (x < 0. && y > 0.) a = MATH_PI - a;
if (x < 0. && y < 0.) a = MATH_PI + a;
if (x > 0. && y < 0.) a = 2.*MATH_PI - a;
nx = sin(a);
ny = cos(a);
put_double_fat(nx, ny, 0xff, 0, 0xff);
}

void draw_metric(PathStack* ps)
{
GLdouble eps;
PathStack *current, *prev;
if (!ps) return;
current = ps;
prev = current;
eps = 0.04;
while (current)
{
if (distance(current, prev) >= eps)
{
prev = current;
put_double_fat(current->x, current->y, 0, 0, 0xff);
atan_cast(current->x, current->y);
}
current = current->next;
}
}

void draw_rest()
{
GLdouble angle;
GLdouble step;
GLdouble x,y;
PathStack* ps;
ps = 0;
step = MATH_PI / 18000.;
for (angle = 0.; angle < 2*MATH_PI; angle += step)
{
x = sqrt(cos(2.*angle))*sin(angle);
y = sqrt(cos(2.*angle))*cos(angle);
//printf("%f,%f\n", x,y);
put_double(x, y, 0, 0xff, 0);
push_path(&ps, x, y);
}
draw_metric(ps);
}

void bla()
{
GLint i,j;
for (i=0;i<WS;i++)
for (j=0;j<WS;j++)
set_color(i, j, 0, 0, 0);
draw_circle();
draw_rest();
}

void anim(void)
{
glutPostRedisplay();
}


void visible(int vis)
{
if (vis == GLUT_VISIBLE) glutIdleFunc(anim);
else glutIdleFunc(NULL);
}

void writ_bmap()
{
FILE* plik;
plik = fopen("tex.bmp", "w");
if(!plik) return;

fclose(plik);
}

void keyboard(unsigned char key, int x, int y)
{
switch (key)
{
case 'q': exit(1); break;
//case 'w': write_bmap(); break;
}
}

void reshape(int w, int h)
{
glViewport(0, 0, (GLsizei) w, (GLsizei) h);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
glClear(GL_COLOR_BUFFER_BIT);
glLoadIdentity();
gluLookAt(0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
glRasterPos3d(3.33333, -3.33333, 0.);
glDrawPixels(WS, WS, GL_RGB, GL_UNSIGNED_BYTE, &pix);
glFlush();
glutSwapBuffers();
glutPostRedisplay();
}

int main(int argc, char** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE);
glutInitWindowSize(WS, WS);
glutInitWindowPosition(10, 10);
glutCreateWindow("Scene");
glutDisplayFunc(display);
glutReshapeFunc(reshape);
glutKeyboardFunc(keyboard);
bla();
glutMainLoop();
return 0;
}
