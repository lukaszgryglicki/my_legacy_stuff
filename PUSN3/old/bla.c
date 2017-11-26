#include <stdlib.h>
#include <GL/glut.h>

typedef struct _Frame
{
 GLdouble x,y,z;
 GLdouble rx,ry,rz;
} Frame;

Frame A,B;
GLint wx,wy;

void set_frame(Frame* f, GLdouble x, GLdouble y, GLdouble z, GLdouble ax, GLdouble ay, GLdouble az)
{
 if (!f) return;
 f->x  = x;
 f->y  = y;
 f->z  = z;
 f->rx = ax;
 f->ry = ay;
 f->rz = az;
}

void init(void) 
{
 glClearColor(0.0, 0.0, 0.0, 0.0);
 glShadeModel(GL_SMOOTH);
 set_frame(&A, 0., 0., 0., 0., 0., 0.);
 set_frame(&B, 0., 0., 0., 40., 50., 60.);
}

void display1(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glLoadIdentity();
 gluLookAt(0., 0., 5., 0. , 0., 0., 0., 1., 0.);
 /*glPushMatrix();
 glRotated(A.rx, 1., 0., 0.);
 glRotated(A.ry, 0., 1., 0.);
 glRotated(A.rz, 0., 0., 1.);*/
 glBegin(GL_TRIANGLES);
   glColor3d(1., 0., 0.);
   glVertex3d(A.x, A.y, A.z);
   glVertex3d(A.x+10., A.y, A.z);
   glColor3d(0., 1., 0.);
   glVertex3d(A.x, A.y, A.z);
   glVertex3d(A.x, A.y+1., A.z);
   glColor3d(0., 0., 1.);
   glVertex3d(A.x, A.y, A.z);
   glVertex3d(A.x, A.y, A.z+1.);
 glEnd();
 /*glPopMatrix();*/
 glFlush();
 glutSwapBuffers();
}

void display2(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glFlush();
 glutSwapBuffers();
}

void reshape(int w, int h)
{
 wx = w;
 wy = h;
 glViewport(0, 0, (GLsizei)wx, (GLsizei)wy);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1., 1., -1., 1., 1., 20.);
 glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) 
     {
      case 27: case 'q': exit(0); break;
     }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(370, 370); 
   glutInitWindowPosition(10, 10);
   glutCreateWindow("E");
   glutDisplayFunc(display1); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   
   /*glutInitWindowSize(370, 370); 
   glutInitWindowPosition(400, 100);
   glutCreateWindow("Q");
   glutDisplayFunc(display2); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);*/
   init();
   glutMainLoop();
   return 0;
}
