#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

typedef struct _Frame
{
 GLdouble x,y,z;
 GLdouble rx,ry,rz;
} Frame;

Frame A,B,W;
GLint wx,wy;
static GLfloat dR = .1;
static GLfloat dA = 3.;
static GLint interpolate = 0;
static GLint nI = 30;
static GLfloat wsx,wsy,wsz;


void help()
{
 printf("frame A: qazwsxedc\n");
 printf("frame B: rfvtgbyhn\n");
 printf("world  : ujmik,ol.\n");
 printf("scales: 456789\n");
 printf("interpol opts: 123\n");
}

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

void move_frame(Frame* f, int x, int y, int z, int a, int b, int c)
{
 if (!f) return;
 if (x) f->x  += x*dR;
 if (y) f->y  += y*dR;
 if (z) f->z  += z*dR;
 if (a) f->rx += a*dA;
 if (b) f->ry += b*dA;
 if (c) f->rz += c*dA;
 if (f->rx < 0.) f->rx += 360.;
 if (f->ry < 0.) f->ry += 360.;
 if (f->rz < 0.) f->rz += 360.;
 if (f->rx >= 360.) f->rx -= 360.;
 if (f->ry >= 360.) f->ry -= 360.;
 if (f->rz >= 360.) f->rz -= 360.;
}

void init(void) 
{
 glClearColor(0.0, 0.0, 0.0, 0.0);
 glShadeModel(GL_SMOOTH);
 set_frame(&A, 2., -1., 0.5, 30., 0., -20.);
 set_frame(&B, -2., 1., -0.5, 0., 30., 20.);
 set_frame(&W, 0., 0., 0., 0., 0., 0.);
 wsx = wsy = wsz = 1.;
}

void draw_int()
{
 GLint i;
 GLdouble x,y,z,a,b,c;
 GLdouble r1,r2,r3,g1,g2,g3,b1,b2,b3;
 for (i=1;i<nI;i++)
  {
   glPushMatrix();
   x = A.x + (B.x-A.x) * ((GLdouble)i / (GLdouble)nI);
   y = A.y + (B.y-A.y) * ((GLdouble)i / (GLdouble)nI);
   z = A.z + (B.z-A.z) * ((GLdouble)i / (GLdouble)nI);
   a = A.rx + (B.rx-A.rx) * ((GLdouble)i / (GLdouble)nI);
   b = A.ry + (B.ry-A.ry) * ((GLdouble)i / (GLdouble)nI);
   c = A.rz + (B.rz-A.rz) * ((GLdouble)i / (GLdouble)nI);
   r1 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   r2 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   r3 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   g1 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   g2 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   g3 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b1 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b2 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b3 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   glTranslated(x, y, z);
   glRotated(a, 1., 0., 0.);
   glRotated(b, 0., 1., 0.);
   glRotated(c, 0., 0., 1.);
   glScaled(0.7, 0.7, 0.7);
   glBegin(GL_LINES);
     glColor3d(r1,g1,b1);
     glVertex3d(0., 0., 0.);
     glVertex3d(1., 0., 0.);
     glColor3d(r2,g2,b2);
     glVertex3d(0., 0., 0.);
     glVertex3d(0., 1., 0.);
     glColor3d(r3,g3,b3);
     glVertex3d(0., 0., 0.);
     glVertex3d(0., 0., 1.);
   glEnd();
   glPopMatrix();
  }
}


void display1(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glColor3f(1.0, 1.0, 1.0);
 glLoadIdentity();
 gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 glTranslated(W.x, W.y, W.z);
 glRotated(W.rx, 1., 0., 0.);
 glRotated(W.ry, 0., 1., 0.);
 glRotated(W.rz, 0., 0., 1.);
 glScaled(wsx, wsy, wsz);
 glPushMatrix();
 glTranslated(A.x, A.y, A.z);
 glRotated(A.rx, 1., 0., 0.);
 glRotated(A.ry, 0., 1., 0.);
 glRotated(A.rz, 0., 0., 1.);
 glBegin(GL_LINES);
   glColor3d(1., 0., 0.);
   glVertex3d(0., 0., 0.);
   glVertex3d(1., 0., 0.);
   glColor3d(0., 1., 0.);
   glVertex3d(0., 0., 0.);
   glVertex3d(0., 1., 0.);
   glColor3d(0., 0., 1.);
   glVertex3d(0., 0., 0.);
   glVertex3d(0., 0., 1.);
 glEnd();
 glPopMatrix();
 glPushMatrix();
 glTranslated(B.x, B.y, B.z);
 glRotated(B.rx, 1., 0., 0.);
 glRotated(B.ry, 0., 1., 0.);
 glRotated(B.rz, 0., 0., 1.);
 glBegin(GL_LINES);
   glColor3d(0., 1., 1.);
   glVertex3d(0., 0., 0.);
   glVertex3d(1., 0., 0.);
   glColor3d(1., 0., 1.);
   glVertex3d(0., 0., 0.);
   glVertex3d(0., 1., 0.);
   glColor3d(1., 1., 0.);
   glVertex3d(0., 0., 0.);
   glVertex3d(0., 0., 1.);
 glEnd();
 glPopMatrix();
 if (interpolate) draw_int();
 glFlush();
 glutSwapBuffers();
 glutPostRedisplay();
}

void display2(void)
{
 display1();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27: exit(0); break;
      case 'd': move_frame(&A, 1,0,0,0,0,0); break;
      case 'a': move_frame(&A, -1,0,0,0,0,0); break;
      case 'w': move_frame(&A, 0,1,0,0,0,0); break;
      case 's': move_frame(&A, 0,-1,0,0,0,0); break;
      case 'x': move_frame(&A, 0,0,1,0,0,0); break;
      case 'e': move_frame(&A, 0,0,-1,0,0,0); break;
      case 'q': move_frame(&A, 0,0,0,1,0,0); break;
      case 'z': move_frame(&A, 0,0,0,0,1,0); break;
      case 'c': move_frame(&A, 0,0,0,0,0,1); break;
		/**/
      case 'h': move_frame(&B, 1,0,0,0,0,0); break;
      case 'f': move_frame(&B, -1,0,0,0,0,0); break;
      case 't': move_frame(&B, 0,1,0,0,0,0); break;
      case 'g': move_frame(&B, 0,-1,0,0,0,0); break;
      case 'b': move_frame(&B, 0,0,1,0,0,0); break;
      case 'y': move_frame(&B, 0,0,-1,0,0,0); break;
      case 'r': move_frame(&B, 0,0,0,1,0,0); break;
      case 'v': move_frame(&B, 0,0,0,0,1,0); break;
      case 'n': move_frame(&B, 0,0,0,0,0,1); break;
		
      case 'l': move_frame(&W,1,0,0,0,0,0); break;
      case 'j': move_frame(&W,-1,0,0,0,0,0); break;
      case 'i': move_frame(&W,0,1,0,0,0,0); break;
      case 'k': move_frame(&W,0,-1,0,0,0,0); break;
      case ',': move_frame(&W,0,0,1,0,0,0); break;
      case 'o': move_frame(&W,0,0,-1,0,0,0); break;
      case 'u': move_frame(&W,0,0,0,1,0,0); break;
      case 'm': move_frame(&W,0,0,0,0,1,0); break;
      case '.': move_frame(&W,0,0,0,0,0,1); break;

      case '1': interpolate = ! interpolate; break;
      case '2': nI=(GLint)((GLdouble)nI/1.4); if (nI < 3) nI = 3; break;
      case '3': nI=(GLint)((GLdouble)nI*1.4); if (nI > 1000) nI = 1000; break;

      case '4': wsx *= 1.4; if (wsx > 16.) wsx = 16.; break;
      case '6': wsy *= 1.4; if (wsy > 16.) wsy = 16.; break;
      case '8': wsz *= 1.4; if (wsz > 16.) wsz = 16.; break;
      case '5': wsx /= 1.4; if (wsx < 0.07) wsx = 0.07; break;
      case '7': wsy /= 1.4; if (wsy < 0.07) wsy = 0.07; break;
      case '9': wsz /= 1.4; if (wsz < 0.07) wsz = 0.07; break;
   }
}

void anim(void)
{
 glutPostRedisplay();
}

void visible(int vis)
{
  if (vis == GLUT_VISIBLE)    glutIdleFunc(anim);
  else    glutIdleFunc(NULL);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE);
   
   glutInitWindowSize (480, 480); 
   glutInitWindowPosition (10, 100);
   glutCreateWindow("E");
   glutDisplayFunc(display1); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   
   glutInitWindowSize (480, 480); 
   glutInitWindowPosition (500, 100);
   glutCreateWindow("Q");
   glutDisplayFunc(display1); 
   glutReshapeFunc(reshape);
   glutVisibilityFunc(visible);
   glutKeyboardFunc(keyboard);
   
   init();
   help();
   glutMainLoop();
   return 0;
}
