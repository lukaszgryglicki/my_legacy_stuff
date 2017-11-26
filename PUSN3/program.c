#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define PI 3.1415926F
/*#define DEBUG*/

typedef struct _Q
{
 GLdouble x,y,z,w;
} Quaternion;

typedef struct _E
{
 GLdouble y,p,r;
} EulerAngle;

typedef struct _Frame
{
 GLdouble x,y,z;
 GLdouble rx,ry,rz;
 Quaternion q;
 EulerAngle ea;
} Frame;

Frame A,B,W;
GLint wx,wy;
static GLfloat dR = .1;
static GLfloat dA = 3.;
static GLint interpolate = 0;
static GLint nI = 30;
static GLint cI = 0;
static GLint incr = 1;
static GLfloat wsx,wsy,wsz;

void get_matrix(GLdouble*, GLdouble, GLdouble, GLdouble);

#ifdef DEBUG

void print_matrix(GLdouble* m, char* n)
{
 int i,j;
 printf("---- %s ---- \n", n);
 for (i=0;i<4;i++)
   {
    for (j=0;j<4;j++) printf("%f\t", m[i+4*j]);
    printf("\n");
   }
}

#endif

void help()
{
 printf("frame A: qazwsxedc\n");
 printf("frame B: rfvtgbyhn\n");
 printf("world  : ujmik,ol.\n");
 printf("scales: 456789\n");
 printf("interpol opts: 123\n");
}

void get_euler_from_matrix(EulerAngle* e, GLdouble* m)
{
 GLdouble cosY, sinY, cosP, sinP, cosR, sinR;
 if (!e || !m) return;
 cosY = m[10];
 sinY = sqrt(1. - cosY * cosY);
 if (sinY != 0.)
  {
   cosP = -m[9] / sinY;
   sinP =  m[8] / sinY;
   cosR =  m[6] / sinY;
   sinR =  m[2] / sinY;
  }
 else cosP = sinP = cosR = sinR = 0.;
 if (sinY != 0 && m[0] * (cosP * cosR-cosY * sinP * sinR) < 0.)
   {
    sinY = -sinY;
    cosP = -m[9] / sinY;
    sinP =  m[8] / sinY;
    cosR =  m[6] / sinY;
    sinR =  m[2] / sinY;
    }
 e->y = atan2(sinY, cosY);
 e->p = atan2(sinP, cosP);
 e->r = atan2(sinR, cosR);
}

void get_matrix_from_euler(GLdouble* m, EulerAngle* e)
{
 GLdouble cosY, sinY, cosP, sinP, cosR, sinR;
 cosY = cosf(e->y);
 sinY = sinf(e->y);
 cosP = cosf(e->p);
 sinP = sinf(e->p);
 cosR = cosf(e->r);
 sinR = sinf(e->r);
 m[0]  = cosP * cosR - cosY * sinP * sinR;
 m[4]  = -cosP * sinR - cosY * sinP * cosR;
 m[8]  = sinY * sinP;
 m[1]  = sinP * cosR + cosY * cosP * sinR;
 m[5]  = -sinP * sinR + cosY * cosP * cosR;
 m[9]  = -sinY * cosP;
 m[2]  = sinY * sinR;
 m[6]  = sinY * cosR;
 m[10] = cosY;
 m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0;
 m[15] = 1.0;
}

void interpol_euler(EulerAngle* res, EulerAngle* from, EulerAngle* to, GLdouble i)
{
 if (!res || !from || !to) return;
 res->y = i * (to->y - from->y) + from->y;
 res->p = i * (to->p - from->p) + from->p;
 res->r = i * (to->r - from->r) + from->r;
}

void get_quaternion_from_matrix(Quaternion* q, GLdouble* m)
{
 GLdouble fD, Tr;
 GLint i,j,k;
 if (!q || !m) return;
 Tr = m[0] + m[5] + m[10] + m[15];
 if (Tr >= 1.0)
   {
    fD = 2.0*sqrt(Tr);
    q->w = fD / 4.0;
    q->x = (m[6] - m[9]) / fD;
    q->y = (m[8] - m[2]) / fD;
    q->z = (m[1] - m[4]) / fD;
   }
 else
   {
    if (m[0] > m[5]) i = 0;
    else i = 1;
    if (m[10] > m[5*i]) i = 2;
    j = (i+1) % 3;
    k = (j+1) % 3;
    fD = 2.0 * sqrt(m[5*i] - m[5*j] - m[5*k] + 1.0);
    switch (i)
      {
	      case 0: q->x = fD / 4.0; break;
	      case 1: q->y = fD / 4.0; break;
	      case 2: q->z = fD / 4.0; break;
/*	      case 3: q->w = fD / 4.0; break;*/
	      default: exit(2);
      }
    switch (j)
      {
	      case 0: q->x  = (m[j+4*i] + m[i+4*j]) / fD; break;
	      case 1: q->y  = (m[j+4*i] + m[i+4*j]) / fD; break;
	      case 2: q->z  = (m[j+4*i] + m[i+4*j]) / fD; break;
	      default: exit(2);
      }
    switch (k)
      {
	      case 0: q->x  = (m[k+4*i] + m[i+4*k]) / fD; break;
	      case 1: q->y  = (m[k+4*i] + m[i+4*k]) / fD; break;
	      case 2: q->z  = (m[k+4*i] + m[i+4*k]) / fD; break;
	      default: exit(2);
      }		
     q->w = (m[k+4*j] - m[j+4*k]) / fD;
   }
}

void get_matrix_from_quaternion(GLdouble* m, Quaternion* q)
{
 GLdouble norm,s;
 GLdouble xx,yy,zz,xy,xz,yz,wx,wy,wz;
 norm = q->x*q->x + q->y*q->y + q->z*q->z + q->w*q->w,
 s = (norm > 0) ? 2/norm : 0;	
 xx = q->x * q->x * s;
 yy = q->y * q->y * s;
 zz = q->z * q->z * s;	
 xy = q->x * q->y * s;
 xz = q->x * q->z * s;
 yz = q->y * q->z * s;
 wx = q->w * q->x * s;
 wy = q->w * q->y * s;
 wz = q->w * q->z * s;
 m[0]  = 1.0 - (yy + zz);
 m[1]  = xy + wz;
 m[2]  = xz - wy;
 m[4]  = xy - wz;
 m[5]  = 1.0 - (xx + zz);
 m[6]  = yz + wx;
 m[8]  = xz + wy;
 m[9]  = yz - wx;
 m[10] = 1.0 - (xx + yy);
 m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0;
 m[15] = 1.0;
}

void interpol_quaternion(Quaternion* res, Quaternion* from, Quaternion* to, GLdouble i)
{
 if (!res || !from || !to) return;
 res->x = i * (to->x - from->x) + from->x;
 res->y = i * (to->y - from->y) + from->y;
 res->z = i * (to->z - from->z) + from->z;
 res->w = i * (to->w - from->w) + from->w;
}

void set_frame(Frame* f, GLdouble x, GLdouble y, GLdouble z, GLdouble ax, GLdouble ay, GLdouble az)
{
 GLdouble m[16];
 if (!f) return;
 f->x  = x;
 f->y  = y;
 f->z  = z;
 f->rx = ax;
 f->ry = ay;
 f->rz = az;
 get_matrix(m, ax, ay, az);
 get_quaternion_from_matrix(&f->q, m);
 get_euler_from_matrix(&f->ea, m);
#ifdef DEBUG 
 printf("Q:%f\t%f\t%f\t%f\n", f->q.x, f->q.y, f->q.z, f->q.w);
 printf("E:%f\t%f\t%f\n", f->ea.y, f->ea.p, f->ea.r);
 printf("XYZ: %f\t%f\t%f\n", f->rx, f->ry, f->rz);
#endif
}

void move_frame(Frame* f, int x, int y, int z, int a, int b, int c)
{
 GLdouble m[16];
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
 get_matrix(m, f->rx, f->ry, f->rz);
 get_quaternion_from_matrix(&f->q, m);
 get_euler_from_matrix(&f->ea, m);
#ifdef DEBUG
 printf("Q:%f\t%f\t%f\t%f\n", f->q.x, f->q.y, f->q.z, f->q.w);
 printf("E:%f\t%f\t%f\n", f->ea.y, f->ea.p, f->ea.r);
 printf("XYZ: %f\t%f\t%f\n", f->rx, f->ry, f->rz);
#endif
}

void init(void) 
{
 glClearColor(0.0, 0.0, 0.0, 0.0);
 glShadeModel(GL_SMOOTH);
 /*set_frame(&A, 4., -2., 0.5, 308., -302., 142.);
 set_frame(&B, -4., 2., -0.5, -240., 10., 350.);*/
 set_frame(&A, 2., -1., 0.5, 0., 0., 0.);
 set_frame(&B, -2., 1., -0.5, 0., 0., 0.);
 set_frame(&W, 0., 0., 0., 0., 0., 0.);
 wsx = wsy = wsz = 1.;
}

void draw_int_q()
{
 GLint i;
 GLdouble x,y,z;
 GLdouble r1,r2,r3,g1,g2,g3,b1,b2,b3;
 GLdouble m[16];
 Quaternion interQ;
 for (i=1;i<nI;i++)
  {
   glPushMatrix();
   x = A.x + (B.x-A.x) * ((GLdouble)i / (GLdouble)nI);
   y = A.y + (B.y-A.y) * ((GLdouble)i / (GLdouble)nI);
   z = A.z + (B.z-A.z) * ((GLdouble)i / (GLdouble)nI);
   r1 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   r2 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   r3 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   g1 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   g2 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   g3 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b1 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b2 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b3 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   
   interpol_quaternion(&interQ, &A.q, &B.q, (GLdouble)i/(GLdouble)nI);
   get_matrix_from_quaternion(m, &interQ);
#ifdef DEBUG
   print_matrix(m, "Q");
#endif
   glTranslated(x, y, z);
   glMultMatrixd(m);
   if (i != cI) glScaled(0.6, 0.6, 0.6);
   else 
     { 
      glScaled(1.8, 1.8, 1.8);
      r1 *= 1.3;
      r2 *= 1.3;
      r3 *= 1.3;
      g1 *= 1.3;
      g2 *= 1.3;
      g3 *= 1.3;
      b1 *= 1.3;
      b2 *= 1.3;
      b3 *= 1.3;
     }
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

void draw_int_e()
{
 GLint i;
 GLdouble x,y,z;
 GLdouble r1,r2,r3,g1,g2,g3,b1,b2,b3;
 GLdouble m[16];
 EulerAngle interA;
 for (i=1;i<nI;i++)
  {
   glPushMatrix();
   x = A.x + (B.x-A.x) * ((GLdouble)i / (GLdouble)nI);
   y = A.y + (B.y-A.y) * ((GLdouble)i / (GLdouble)nI);
   z = A.z + (B.z-A.z) * ((GLdouble)i / (GLdouble)nI);
   r1 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   r2 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   r3 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   g1 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   g2 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   g3 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b1 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b2 = (GLdouble)(i)    / (1.3 * (GLdouble)nI);
   b3 = (GLdouble)(nI-i) / (1.3 * (GLdouble)nI);
   
   interpol_euler(&interA, &A.ea, &B.ea, (GLdouble)i/(GLdouble)nI);
   get_matrix_from_euler(m, &interA);
#ifdef DEBUG
   print_matrix(m, "E");
#endif
   glTranslated(x, y, z);
   glMultMatrixd(m);
   if (i != cI) glScaled(0.6, 0.6, 0.6);
   else 
     { 
      glScaled(1.8, 1.8, 1.8);
      r1 *= 1.3;
      r2 *= 1.3;
      r3 *= 1.3;
      g1 *= 1.3;
      g2 *= 1.3;
      g3 *= 1.3;
      b1 *= 1.3;
      b2 *= 1.3;
      b3 *= 1.3;
     }
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
 if (incr) cI ++;
 else cI --;
 if (cI >= nI - 1) { cI = nI -1; incr = ! incr; }
 if (cI <= 0)      { cI = 0; incr = ! incr; }
}

void rotatex(GLdouble* m, double ang)
{
 ang /= 180./PI;
 m[5]  =  cos(ang);
 m[6]  =  sin(ang);
 m[9]  = -sin(ang);
 m[10] =  cos(ang);
}


void rotatey(GLdouble* m, double ang)
{
 ang /= 180./PI;
 m[0]  =  cos(ang);
 m[2]  = -sin(ang);
 m[8]  =  sin(ang);
 m[10] =  cos(ang);
}


void rotatez(GLdouble* m, double ang)
{
 ang /= 180./PI;
 m[0] =  cos(ang);
 m[1] =  sin(ang);
 m[4] = -sin(ang);
 m[5] =  cos(ang);
}

void identity(GLdouble* m)
{
 m[0]  = 1.;
 m[1]  = 0.;
 m[2]  = 0.;
 m[3]  = 0.;
 m[4]  = 0.;
 m[5]  = 1.;
 m[6]  = 0.;
 m[7]  = 0.;
 m[8]  = 0.;
 m[9]  = 0.;
 m[10] = 1.;
 m[11] = 0.;
 m[12] = 0.;
 m[13] = 0.;
 m[14] = 0.;
 m[15] = 1.;
}

void mult_matrix(GLdouble* m, GLdouble* a, GLdouble* b)
{
 GLint i,j,k;
 for (i=0;i<4;i++)
 for (j=0;j<4;j++)
   {
    m[i+4*j] = 0.;
    for (k=0;k<4;k++) m[i+4*j] += a[i+4*k] * b[k+4*j];
   }
}

void get_matrix(GLdouble* m, GLdouble x, GLdouble y, GLdouble z)
{
 GLdouble mx[16], my[16], mz[16], t1[16];
 identity(mx);
 identity(my);
 identity(mz);
 rotatex(mx, x);
 rotatey(my, y);
 rotatez(mz, z);
 mult_matrix(t1, mx, my);
 mult_matrix(m, t1, mz);
}

void rotation(GLdouble x, GLdouble y, GLdouble z)
{
 GLdouble m[16];
 get_matrix(m, x, y, z);
 glMultMatrixd(m);
}


void display1(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_LINE_SMOOTH);
 glColor3f(1.0, 1.0, 1.0);
 glLoadIdentity();
 gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 glTranslated(W.x, W.y, W.z);
 rotation(W.rx, W.ry, W.rz);
 glScaled(wsx, wsy, wsz);
 glPushMatrix();
 glTranslated(A.x, A.y, A.z);
 rotation(A.rx, A.ry, A.rz);
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
 rotation(B.rx, B.ry, B.rz);
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
 if (interpolate) draw_int_e();
 glFlush();
 glutSwapBuffers();
 glutPostRedisplay();
}

void display2(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_LINE_SMOOTH);
 glColor3f(1.0, 1.0, 1.0);
 glLoadIdentity();
 gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 glTranslated(W.x, W.y, W.z);
 rotation(W.rx, W.ry, W.rz);
 glScaled(wsx, wsy, wsz);
 glPushMatrix();
 glTranslated(A.x, A.y, A.z);
 rotation(A.rx, A.ry, A.rz);
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
 rotation(B.rx, B.ry, B.rz);
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
 if (interpolate) draw_int_q();
 glFlush();
 glutSwapBuffers();
 glutPostRedisplay();
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
   glutDisplayFunc(display2); 
   glutReshapeFunc(reshape);
   glutVisibilityFunc(visible);
   glutKeyboardFunc(keyboard);
   
   init();
   help();
   glutMainLoop();
   return 0;
}
