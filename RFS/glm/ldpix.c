#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/glut.h>

int GW, GH, fps;
time_t t1, t2;
unsigned char* pex = NULL;

void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
    case 27: case 'q': exit(0); break;
    default: break;
   }
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

void resize(int w, int h)
{
 glViewport(0, 0, (GLsizei)w, (GLsizei)h);
 glPixelZoom((GLdouble)w/(GLdouble)GW, (GLdouble)h/(GLdouble)GH);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1., 1., -1., 1., 1.5, 20.);
 glMatrixMode(GL_MODELVIEW);
}

void time_counter()
{
 char tstr[64];
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
    sprintf(tstr, "Random Fractal Surfaces: FPS: %d", fps/(int)(t2-t1));
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
}

void render(void)
{
 glViewport(0, 0, GW, GH);
 glClear(GL_COLOR_BUFFER_BIT);
 glLoadIdentity();
 gluLookAt(0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 glRasterPos3d(3.3, -3.3, 0.);
 glDrawPixels(GW, GH, GL_RGB, GL_UNSIGNED_BYTE, pex);
 time_counter();
 glFlush();
 glutSwapBuffers();
}

void Init(char* pname)
{
 FILE* ptr;
 int nel,i;
 if (!pname) exit(1);
 ptr = fopen(pname, "r");
 if (!ptr) { printf("cannot read: %s\n", pname); exit(1); }
 fread((void*)(&GW), sizeof(int), 1, ptr);
 fread((void*)(&GH), sizeof(int), 1, ptr);
 nel = GW*GH*3;
 pex = (unsigned char*)malloc(nel*sizeof(unsigned char));
 for (i=0;i<nel;i++) fread((void*)(&pex[i]), 1, 1, ptr);
 fclose(ptr);
}

int main(int lb, char** par)
{
 if (lb < 2) { printf("%s pixfile\n", par[0]); return 1; }
 Init(par[1]);
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(GW, GH);
 glutInitWindowPosition(10, 10);
 glutCreateWindow(par[0]);
 glutDisplayFunc(render);
 glutReshapeFunc(resize);
 glutKeyboardFunc(keyboard);
 glutVisibilityFunc(visible);
 glutMainLoop();
 return 0;
}

