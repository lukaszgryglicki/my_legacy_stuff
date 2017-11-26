#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#define MATH_PI 3.1415926353
#define WS 768

unsigned char pix[WS*WS*3];
GLdouble wx1, wy1, wx2, wy2, wx3, wy3;
time_t t1, t2;
GLint fps;

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

void draw_circle()
{
 GLdouble angle;
 GLdouble step;
 GLdouble x,y;
 GLdouble dx, dy;
 step = MATH_PI / 18000.;
 for (angle = -6*MATH_PI; angle <= 6*MATH_PI; angle += step)
   {
    x = sin(angle*wx1) + sin(angle*wx2) + sin(angle*wx3);
    y = cos(angle*wy1) + cos(angle*wy2) + cos(angle*wy3);
    //printf("%f,%f\n", x,y);
    put_double(x*.5, y*.5, 0xff, 0, 0);
   }
}

void clear()
{
 GLint i,j;
 for (i=0;i<WS;i++)
 for (j=0;j<WS;j++)
 set_color(i, j, 0, 0, 0);
}

void bla()
{
 time_t tm;
 wx1 = wx2 = wy1 = wy2 = wx3 = wy3 = 1.;
 time(&tm);
 srand((int)tm);
 clear();
 draw_circle();
}

void time_counter()
{
 char tstr[32];
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
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
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

void randomowe()
{
 wx1 += (GLdouble)((rand() % 1001) - 500) / 5000.;
 wx2 += (GLdouble)((rand() % 1001) - 500) / 5000.;
 wx3 += (GLdouble)((rand() % 1001) - 500) / 5000.;
 wy1 += (GLdouble)((rand() % 1001) - 500) / 5000.;
 wy2 += (GLdouble)((rand() % 1001) - 500) / 5000.;
 wy3 += (GLdouble)((rand() % 1001) - 500) / 5000.;
}

void keyboard(unsigned char key, int x, int y)
{
 clear();
switch (key)
{
case 'q': exit(1); break;
case '1': wx1 *= 1.02; break;
case '2': wx1 /= 1.02; break;
case '3': wx2 *= 1.02; break;
case '4': wx2 /= 1.02; break;
case '5': wx3 *= 1.02; break;
case '6': wx3 /= 1.02; break;
case '7': wy1 *= 1.02; break;
case '8': wy1 /= 1.02; break;
case '9': wy2 *= 1.02; break;
case '0': wy2 /= 1.02; break;
case '-': wy3 *= 1.02; break;
case '=': wy3 /= 1.02; break;
case 'r': randomowe(); break;
//case 'w': write_bmap(); break;
}
draw_circle();
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
time_counter();
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
