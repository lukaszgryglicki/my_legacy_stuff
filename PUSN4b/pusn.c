/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#define PI  3.141592653F
#define TWOPI (2.*PI)
#define TOANG (180./PI)
#define NINT 128
#define NINTIV 1
#define GLuchar unsigned char
#define HERE __FILE__,__LINE__,__DATE__,__TIME__
/*#define DEBUG*/
GLint wx,wy;
GLint cx,cy;
GLdouble angX, angY, angZ, scaled;
time_t t1,t2;
GLint fps;
GLint auto_move;
GLdouble R3D3;
GLint idx, rising, explicit, moving;

typedef struct _Q
{
 GLdouble x,y,z,w;
} Quaternion;

typedef struct _T
{
 GLuint id;
 GLuint sizx,sizy;
 GLuchar *data;
} Texture;

typedef struct _P
{
 GLint solved;
 GLdouble a1,a2,a3,a4,a5;
 GLdouble l1,q2,l3,l4;
} PUMA;

typedef struct _F
{
 GLdouble** matrix;
 GLdouble rx, ry, rz;
 GLdouble dx,dy,dz;
 Quaternion q;
} Frame;
PUMA puma;
PUMA** sol;
PUMA** bsol;
Frame* ifrm;
Frame A, B;
Frame* currFrame;
Texture* tList;

void fatal(char* par)
{
 printf("Fatal error: %s\n", par?par:"(null)");
 exit(3);
}


void help()
{
 printf("TODO: szczegolne przypadki (brak rozwiazan i wtedy interpolacja\n");
 printf("po parametryzacji (zrobic funkcje z frame do frame)\n");
 printf("i mozna uzyc do interpolacji parametryzacji na calosci A-->B\n");
 printf("interpolacje obrotu zastapic quaternionami, a interpolacje\n");
 printf("obrotu w parametryzacji po najkrotszej drodze (patrz 1 i 359)\n");
 printf("usage: {basename}\n");
}


GLdouble* vector(GLint siz)
{
 if (siz <= 0) return NULL;
 return (GLdouble*)malloc(siz*sizeof(GLdouble));
}


GLdouble** matrix(GLint siz)
{
 GLdouble** mem;
 int i;
 if (siz <= 0) return NULL;
 mem = (GLdouble**)malloc(siz*sizeof(GLdouble*));
 for (i=0;i<siz;i++) mem[i] = vector(siz);
 return mem;
}


void copy_matrix(GLdouble** dst, GLdouble** src, GLint siz)
{
 int i,j;
 if (!src || !dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++) dst[i][j] = src[i][j];
}


void I_matrix(GLdouble** dst, GLint siz)
{
 int i,j;
 if (!dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++)
   {
    if (i == j) dst[i][j] = 1.;
    else dst[i][j] = 0.;
   }
}


void free_matrix(GLdouble** m, GLint siz)
{
 GLint i;
 if (siz <= 0) return;
 for (i=0;i<siz;i++) free(m[i]);
 free(m);
}


GLdouble** matrix_mul(GLdouble** m, GLdouble** n, GLint ma, GLint mb, GLint na, GLint nb)
{
 GLdouble** dst;
 register GLint k,j,i;
 if (ma <= 0 || mb  <= 0 || na <= 0 || nb <=0 || mb != na) return NULL;
 if (!m || !n) return NULL;
 dst = (GLdouble**)malloc(ma*sizeof(GLdouble*));
 if (!dst) return NULL;
 for (i=0;i<ma;i++) dst[i] = (GLdouble*)malloc(nb*sizeof(GLdouble));
 for (i=0;i<ma;i++)
 for (j=0;j<nb;j++)
    {
     dst[i][j] = 0.0;
     for (k=0;k<mb;k++) dst[i][j] += m[i][k] * n[k][j];
    }
 return dst;
}


void matrix_mul_vector(GLdouble* dst, GLdouble** m, GLdouble* v, GLint len)
{
 GLint i,j;
 for (i=0;i<len;i++)
    {
     dst[i] = 0.0;
     for (j=0;j<len;j++) dst[i] += v[j] * m[i][j];
    }
}


void init_puma(PUMA* p)
{
 p->l1 = 8.;
 p->q2 = 6.;
 p->l3 = 4.;
 p->l4 = 2.;
 p->a1 = p->a2 = p->a3 = p->a4 = 0.;
 p->solved = 1.;
}


void zero_puma(PUMA* p)
{
 p->l1 = 8.;
 p->q2 = 0.;
 p->l3 = 4.;
 p->l4 = 2.;
 p->a1 = p->a2 = p->a3 = p->a4 = 0.;
 p->solved = 1.;
}


void rotatex(GLdouble** m, GLdouble ang)
{
 ang /= 180./PI;
 m[1][1] = cos(ang);
 m[2][1] = sin(ang);
 m[1][2] = -sin(ang);
 m[2][2] = cos(ang);
}


void rotatey(GLdouble** m, GLdouble ang)
{
 ang /= 180./PI;
 m[0][0] = cos(ang);
 m[2][0] = -sin(ang);
 m[0][2] = sin(ang);
 m[2][2] = cos(ang);
}


void rotatez(GLdouble** m, GLdouble ang)
{
 ang /= 180./PI;
 m[0][0] = cos(ang);
 m[1][0] = sin(ang);
 m[0][1] = -sin(ang);
 m[1][1] = cos(ang);
}


void translatex(GLdouble** m, GLdouble arg)
{
 m[0][3] = arg;
}


void translatey(GLdouble** m, GLdouble arg)
{
 m[1][3] = arg;
}


void translatez(GLdouble** m, GLdouble arg)
{
 m[2][3] = arg;
}


void translate(GLdouble** m, GLdouble x, GLdouble y, GLdouble z)
{
 translatex(m, x);
 translatey(m, y);
 translatez(m, z);
}

void get_quaternion_from_matrix(Quaternion* q, GLdouble** m)
{
 GLdouble fD, Tr;
 GLint i,j,k;
 if (!q || !m) return;
 Tr = m[0][0] + m[1][1] + m[2][2] + m[3][3];
 if (Tr >= 1.0)
   {
    fD = 2.0*sqrt(Tr);
    q->w = fD / 4.0;
    q->x = (m[2][1] - m[1][2]) / fD;
    q->y = (m[0][2] - m[2][0]) / fD;
    q->z = (m[1][0] - m[0][1]) / fD;
   }
 else
   {
    if (m[0][0] > m[1][1]) i = 0;
    else i = 1;
    if (m[2][2] > m[i][i]) i = 2;
    j = (i+1) % 3;
    k = (j+1) % 3;
    fD = 2.0 * sqrt(m[i][i] - m[j][j] - m[k][k] + 1.0);
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
	      case 0: q->x  = (m[j][i] + m[i][j]) / fD; break;
	      case 1: q->y  = (m[j][i] + m[i][j]) / fD; break;
	      case 2: q->z  = (m[j][i] + m[i][j]) / fD; break;
	      default: exit(2);
      }
    switch (k)
      {
	      case 0: q->x  = (m[k][i] + m[i][k]) / fD; break;
	      case 1: q->y  = (m[k][i] + m[i][k]) / fD; break;
	      case 2: q->z  = (m[k][i] + m[i][k]) / fD; break;
	      default: exit(2);
      }		
     q->w = (m[k][j] - m[j][k]) / fD;
   }
}

void get_matrix_from_quaternion(GLdouble** m, Quaternion* q)
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
 m[0][0]  = 1.0 - (yy + zz);
 m[1][0]  = xy + wz;
 m[2][0]  = xz - wy;
 m[0][1]  = xy - wz;
 m[1][1]  = 1.0 - (xx + zz);
 m[2][1]  = yz + wx;
 m[0][2]  = xz + wy;
 m[1][2]  = yz - wx;
 m[2][2] = 1.0 - (xx + yy);
 m[3][0] = m[3][1] = m[3][2] = 0.0; 
 /*m[0][3] = m[1][3] = m[2][3] = 0.0;*/
 m[3][3] = 1.0;
}


void move_frameD_old(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
  GLdouble **m, **res;
  if (!f) return ;
  m = matrix(4);
  I_matrix(m, 4);
  translate(m, x, y, z);
  res = matrix_mul(f->matrix, m, 4, 4, 4, 4);
  copy_matrix(f->matrix, res, 4);
  free_matrix(res, 4);
  free_matrix(m, 4);
  f->dx += x;
  f->dy += y;
  f->dz += z;
  get_quaternion_from_matrix(&f->q, f->matrix);
/*  print_frame(f);*/
}

void move_frameD(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
  if (!f) return ;
  translate(f->matrix, x+f->dx, y+f->dy, z+f->dz);
  f->dx += x;
  f->dy += y;
  f->dz += z;
  get_quaternion_from_matrix(&f->q, f->matrix);
/*  print_frame(f);*/
}


void set_frameD(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
  if (!f) return ;
  I_matrix(f->matrix, 4);
  translate(f->matrix, x, y, z);
  f->dx = x;
  f->dy = y;
  f->dz = z;
  get_quaternion_from_matrix(&f->q, f->matrix);
/*  print_frame(f);*/
}


void make_angles(Frame* f)
{
 if (!f) return;
 while (f->rx < 0.) f->rx += 360.;
 while (f->ry < 0.) f->ry += 360.;
 while (f->rz < 0.) f->rz += 360.;
 while (f->rx >= 360.) f->rx -= 360.;
 while (f->ry >= 360.) f->ry -= 360.;
 while (f->rz >= 360.) f->rz -= 360.;
}


void rotate_frameR_old(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
 GLdouble **m1,**m2,**m3, **mt, **m, **res;
 if (!f) return;
 m1 = matrix(4);
 m2 = matrix(4);
 m3 = matrix(4);
 mt = matrix(4);
 I_matrix(m1, 4);
 I_matrix(m2, 4);
 I_matrix(m3, 4);
 rotatex(m1, x);
 rotatey(m2, y);
 rotatez(m3, z);
 mt = matrix_mul(m1, m2, 4, 4, 4, 4);
 m  = matrix_mul(mt, m3, 4, 4, 4, 4);
 res = matrix_mul(f->matrix, m, 4, 4, 4, 4);
 copy_matrix(f->matrix, res, 4);
 free_matrix(res, 4);
 free_matrix(m1, 4);
 free_matrix(m2, 4);
 free_matrix(m3, 4);
 free_matrix(mt, 4);
 free_matrix(m, 4);
 f->rx += x;
 f->ry += y;
 f->rz += z;
 make_angles(f);
  get_quaternion_from_matrix(&f->q, f->matrix);
/* print_frame(f);*/
}

void rotate_frameR(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
 GLdouble **m1,**m2,**m3, **mt, **m;
 if (!f) return;
 m1 = matrix(4);
 m2 = matrix(4);
 m3 = matrix(4);
 mt = matrix(4);
 I_matrix(m1, 4);
 I_matrix(m2, 4);
 I_matrix(m3, 4);
 rotatex(m1, x+f->rx);
 rotatey(m2, y+f->ry);
 rotatez(m3, z+f->rz);
 mt = matrix_mul(m1, m2, 4, 4, 4, 4);
 m  = matrix_mul(mt, m3, 4, 4, 4, 4);
 copy_matrix(f->matrix, m, 3);
 free_matrix(m1, 4);
 free_matrix(m2, 4);
 free_matrix(m3, 4);
 free_matrix(mt, 4);
 free_matrix(m, 4);
 f->rx += x;
 f->ry += y;
 f->rz += z;
 make_angles(f);
 get_quaternion_from_matrix(&f->q, f->matrix);
/* print_frame(f);*/
}


void set_frameR(Frame* f, GLdouble x, GLdouble y, GLdouble z)
{
 GLdouble **m1,**m2,**m3, **mt, **m;
 if (!f) return;
 m1 = matrix(4);
 m2 = matrix(4);
 m3 = matrix(4);
 mt = matrix(4);
 I_matrix(m1, 4);
 I_matrix(m2, 4);
 I_matrix(m3, 4);
 rotatex(m1, x);
 rotatey(m2, y);
 rotatez(m3, z);
 mt = matrix_mul(m1, m2, 4, 4, 4, 4);
 m  = matrix_mul(mt, m3, 4, 4, 4, 4);
 copy_matrix(f->matrix, m, 4);
 free_matrix(m1, 4);
 free_matrix(m2, 4);
 free_matrix(m3, 4);
 free_matrix(mt, 4);
 free_matrix(m, 4);
 f->rx = x;
 f->ry = y;
 f->rz = z;
 make_angles(f);
 get_quaternion_from_matrix(&f->q, f->matrix);
/* print_frame(f);*/
}


void set_frameRD(Frame* f, GLdouble rx, GLdouble ry, GLdouble rz, GLdouble dx, GLdouble dy, GLdouble dz)
{
 if (!f) return;
 set_frameR(f, rx, ry, rz);
 move_frameD(f, dx, dy, dz);
 get_quaternion_from_matrix(&f->q, f->matrix);
}


void set_frameDR(Frame* f, GLdouble dx, GLdouble dy, GLdouble dz, GLdouble rx, GLdouble ry, GLdouble rz)
{
 if (!f) return;
 set_frameD(f, dx, dy, dz);
 rotate_frameR(f, rx, ry, rz);
 get_quaternion_from_matrix(&f->q, f->matrix);
}


void init_frame(Frame* f)
{
 if (!f) return;
 f->matrix = matrix(4);
 I_matrix(f->matrix,4);
 f->dx = f->dy = f->dz = 0.;
 f->rx = f->ry = f->rz = 0.;
 get_quaternion_from_matrix(&f->q, f->matrix);
}


void free_frame(Frame* f)
{
 if (!f) return;
 free_matrix(f->matrix, 4);
}


void switch_current_frame()
{
 if (currFrame == &A) currFrame = &B;
 else currFrame = &A;
}


void check_puma(PUMA* p)
{
 if (!p) return;
 if (p->a1 < 0.)    p->a1 += 360.;
 if (p->a1 >= 360.) p->a1 -= 360.;
 if (p->a2 < 0.)    p->a2 += 360.;
 if (p->a2 >= 360.) p->a2 -= 360.;
 if (p->a3 < 0.)    p->a3 += 360.;
 if (p->a3 >= 360.) p->a3 -= 360.;
 if (p->a4 < 0.)    p->a4 += 360.;
 if (p->a4 >= 360.) p->a4 -= 360.;
 if (p->a5 < 0.)    p->a5 += 360.;
 if (p->a5 >= 360.) p->a5 -= 360.;
}


void update_angles(PUMA* p)
{
 if (!p) return;
 p->a1 *= 180./PI;
 p->a2 *= 180./PI;
 p->a3 *= 180./PI;
 p->a4 *= 180./PI;
 p->a5 *= 180./PI;
}


GLint equals(GLdouble a, GLdouble b, GLdouble eps)
{
 if (fabs(a-b) < eps) return 1;
 else return 0;
}


GLdouble angle_froma(GLdouble as, GLdouble ac, GLdouble eps, GLint* solved)
{
 GLdouble as2, ac2;
 if (as < 0.) as += TWOPI;
 as2 = PI - as;
 if (as2 < 0.) as2 += TWOPI;
 ac2 = TWOPI - ac;
 *solved = 1;
 if (equals(as,ac,eps)) return as;
 if (equals(as2,ac,eps)) return ac;
 if (equals(as,ac2,eps)) return as;
 if (equals(as2,ac2,eps)) return as2;
 printf("ASIN(%f,%f), ACOS(%f,%f), problem!!\n", as*TOANG, as2*TOANG, ac*TOANG, ac2*TOANG);
 *solved = 0;
 return -1.;
}


GLdouble masin(GLdouble a)
{
 if (a < -1.) a = -1.;
 if (a > 1.) a = 1.;
 return asin(a);
}


GLdouble macos(GLdouble a)
{
 if (a < -1.) a = -1.;
 if (a > 1.) a = 1.;
 return acos(a);
}


GLint solve_puma(PUMA* p, PUMA* s, Frame* f)
{
 GLdouble check;
 GLdouble ac, as, a23,eps;
 GLint i, solved;
 if (!p || !s) return 0;
 eps = 1e-2;
 for (i=0;i<8;i++) zero_puma(&s[i]);
 check = f->matrix[0][3] - p->l4 * f->matrix[0][0] ;
 if (check == 0.) 
   {
    for (i=0;i<8;i++) s[i].solved = 0;
    printf("failed!\n");
    return 0;
   }
 s[0].a1 = atan((f->matrix[1][3] - p->l4 * f->matrix[1][0]) / (f->matrix[0][3] - p->l4 * f->matrix[0][0]));
 if (s[0].a1 < 0.) s[0].a1 += TWOPI;
 s[1].a1 =  s[0].a1 + PI;
 if (s[1].a1 >= TWOPI) s[1].a1 -= TWOPI;
 s[2].a1 = s[0].a1;
 s[3].a1 = s[1].a1;
/* printf("A1(%f,%f,%f,%f)\n", s[0].a1*TOANG, s[1].a1*TOANG, s[2].a1*TOANG, s[3].a1*TOANG);*/
 /* two solutions here */
 s[0].a4 = masin(cos(s[0].a1) * f->matrix[1][0] - sin(s[0].a1) * f->matrix[0][0]);
 s[1].a4 = masin(cos(s[1].a1) * f->matrix[1][0] - sin(s[1].a1) * f->matrix[0][0]);
 if (s[0].a4 < 0.) s[0].a4 += TWOPI;
 if (s[1].a4 < 0.) s[1].a4 += TWOPI;
 /* four solutions here */
 s[2].a4 = PI - s[0].a4;
 s[3].a4 = PI - s[1].a4;
 if (s[2].a4 < 0.) s[2].a4 += TWOPI;
 if (s[3].a4 < 0.) s[3].a4 += TWOPI;
/* printf("A4(%f,%f,%f,%f)\n", s[0].a4*TOANG, s[1].a4*TOANG, s[2].a4*TOANG,s[3].a4*TOANG);*/
 for (i=0;i<4;i++)
   {
    check = cos(s[i].a4);
/*    printf("i = %d, check = %f, %f\n", i, check, masin(-1.));*/
    if (check == 0.) 
      {
       s[i].solved = 0;
       printf("unsoled1\n");
/*       continue;*/
      }
    check = (cos(s[i].a1) * f->matrix[1][1] - sin(s[i].a1) * f->matrix[0][1]) / cos(s[i].a4);
/*    printf("check is %f\n", check);*/
    ac = macos((cos(s[i].a1) * f->matrix[1][1] - sin(s[i].a1) * f->matrix[0][1]) / cos(s[i].a4));
    as = masin((sin(s[i].a1) * f->matrix[0][2] - cos(s[i].a1) * f->matrix[1][2]) / cos(s[i].a4));
/*    printf("c5 = %f, s5 = %f\n", ac, as);*/
    s[i].a5 = angle_froma(as, ac, eps, &solved);
    if (!solved) 
      {
       s[i].solved = 0;
/*       continue;*/
      }
   }
/* printf("A5(%f,%f,%f,%f)\n", s[0].a5*TOANG, s[1].a5*TOANG, s[2].a5*TOANG,s[3].a5*TOANG);*/
 for (i=0;i<4;i++)
   {
    if (!s[i].solved) 
      { 
       s[i+4].solved = 0; 
       printf("unsoled2\n");
/*       continue; */
      }
    s[i+4].a1 = s[i].a1;
    s[i+4].a4 = s[i].a4;
    s[i+4].a5 = s[i].a5;
   }
 for (i=0;i<4;i++)
   {
    if (!s[i].solved) 
      { 
       printf("unsoled3\n");
       s[i+4].solved = 0; 
/*       continue; */
      }
    check = cos(s[i].a4)*(f->matrix[0][3]-p->l4*f->matrix[0][0])-cos(s[i].a1)*p->l3*f->matrix[2][0];
    if (check == 0.) 
      {
       printf("unsoled4\n");
       s[i].solved = 0;
/*       continue;*/
      }
    /*check = cos(s[i].a1) * cos(s[i].a4) * (f->matrix[2][3] - p->l4 * f->matrix[2][0] - p->l1) + p->l3 * (f->matrix[0][0] + sin(s[i].a1) * sin(s[i].a4));
    printf("check is %f\n", check);
    check = p->l3 * (f->matrix[0][0] + sin(s[i].a1) * sin(s[i].a4));
    printf("check is %f\n", check);
    check = cos(s[i].a1) * cos(s[i].a4) * (f->matrix[2][3] - p->l4 * f->matrix[2][0] - p->l1);
    printf("check is %f\n", check);*/
    s[i].a2 = 0.;
    s[i].a2 = atan((cos(s[i].a1) * cos(s[i].a4) * (f->matrix[2][3] - p->l4 * f->matrix[2][0] - p->l1) + p->l3 * (f->matrix[0][0] + sin(s[i].a1) * sin(s[i].a4)))
	     /
             (-1.*(cos(s[i].a4) * (f->matrix[0][3] - p->l4 * f->matrix[0][0]) - cos(s[i].a1) * p->l3 * f->matrix[2][0])));
/*    printf("atan = %f, %f\n", s[i].a2, atan(2.));*/
    if (s[i].a2 < 0.) s[i].a2 += TWOPI;
    s[i+4].a2 =  s[i].a2 + PI;
    if (s[i+4].a2 >= TWOPI) s[i+4].a2 -= TWOPI;
   }
 /*printf("A2(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a2*TOANG, s[1].a2*TOANG, s[2].a2*TOANG,s[3].a2*TOANG, s[4].a2*TOANG, s[5].a2*TOANG, s[6].a2*TOANG,s[7].a2*TOANG);
 printf("A1(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a1*TOANG, s[1].a1*TOANG, s[2].a1*TOANG,s[3].a1*TOANG, s[4].a1*TOANG, s[5].a1*TOANG, s[6].a1*TOANG,s[7].a1*TOANG);
 printf("A4(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a4*TOANG, s[1].a4*TOANG, s[2].a4*TOANG,s[3].a4*TOANG, s[4].a4*TOANG, s[5].a4*TOANG, s[6].a4*TOANG,s[7].a4*TOANG);
 printf("A5(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a5*TOANG, s[1].a5*TOANG, s[2].a5*TOANG,s[3].a5*TOANG, s[4].a5*TOANG, s[5].a5*TOANG, s[6].a5*TOANG,s[7].a5*TOANG);*/
 /*printf("x = [%f,%f,%f]\n", f->matrix[0][0], f->matrix[1][0], f->matrix[2][0]);
 printf("y = [%f,%f,%f]\n", f->matrix[0][1], f->matrix[1][1], f->matrix[2][1]);
 printf("z = [%f,%f,%f]\n", f->matrix[0][2], f->matrix[1][2], f->matrix[2][2]);
 printf("p = [%f,%f,%f]\n", f->matrix[0][3], f->matrix[1][3], f->matrix[2][3]);*/
 for (i=0;i<8;i++)
 {
  if (!s[i].solved) 
    { 
     printf("unsoled5\n");
/*     continue; */
    }
  check = cos(s[i].a1)*cos(s[i].a2)*cos(s[i].a4);
  if (check == 0.) 
    {
     printf("unsoled6\n");
     s[i].solved = 0;
/*     continue;*/
    }
  s[i].q2 = (cos(s[i].a4) * (f->matrix[0][3]-p->l4*f->matrix[0][0]) - cos(s[i].a1) * p->l3 * f->matrix[2][0]) / ( cos(s[i].a1) * cos(s[i].a2) * cos(s[i].a4) );
 }
/* printf("Q2(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].q2, s[1].q2, s[2].q2,s[3].q2, s[4].q2, s[5].q2, s[6].q2,s[7].q2);*/
 eps = 0.02;
 for (i=0;i<8;i++)
   {
    if (!s[i].solved) 
      { 
       printf("unsoled7\n");
/*       continue; */
      }
    check = cos(s[i].a1) * cos(s[i].a4);
    if (check == 0.) 
      {
       printf("unsoled8\n");
       s[i].solved = 0;
/*       continue;*/
      }
    check = (f->matrix[0][0] + sin(s[i].a1) * sin(s[i].a4))/(cos(s[i].a1) * cos(s[i].a4));
/*    printf("check is %f\n", check);*/
    ac = macos((f->matrix[0][0] + sin(s[i].a1) * sin(s[i].a4))/(cos(s[i].a1) * cos(s[i].a4)));
    		/* FIXME was [2][0] is it Ok [0][0] ?? */
    as = masin(-f->matrix[2][0] / cos(s[i].a4));
    a23 = angle_froma(as, ac, eps, &solved);
    if (!solved)
      {
       s[i].solved = 0;
       printf("unsoled9\n");
/*       continue;*/
      }
    s[i].a3 = a23 - s[i].a2;
   }
 /*printf("Final:\n");
 printf("A1(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a1*TOANG, s[1].a1*TOANG, s[2].a1*TOANG,s[3].a1*TOANG, s[4].a1*TOANG, s[5].a1*TOANG, s[6].a1*TOANG,s[7].a1*TOANG);
 printf("A2(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a2*TOANG, s[1].a2*TOANG, s[2].a2*TOANG,s[3].a2*TOANG, s[4].a2*TOANG, s[5].a2*TOANG, s[6].a2*TOANG,s[7].a2*TOANG);
 printf("A3(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a3*TOANG, s[1].a3*TOANG, s[2].a3*TOANG,s[3].a3*TOANG, s[4].a3*TOANG, s[5].a3*TOANG, s[6].a3*TOANG,s[7].a3*TOANG);
 printf("A4(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a4*TOANG, s[1].a4*TOANG, s[2].a4*TOANG,s[3].a4*TOANG, s[4].a4*TOANG, s[5].a4*TOANG, s[6].a4*TOANG,s[7].a4*TOANG);
 printf("A5(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].a5*TOANG, s[1].a5*TOANG, s[2].a5*TOANG,s[3].a5*TOANG, s[4].a5*TOANG, s[5].a5*TOANG, s[6].a5*TOANG,s[7].a5*TOANG);
 printf("Q2(%f,%f,%f,%f,%f,%f,%f,%f)\n", s[0].q2, s[1].q2, s[2].q2,s[3].q2, s[4].q2, s[5].q2, s[6].q2,s[7].q2);*/
 for (i=0;i<8;i++)
   {
    update_angles(&s[i]);
    check_puma(&s[i]);
    }
/* printf("ok!\n");*/
 return 1;
}

void interpol_quaternion(Quaternion* res, Quaternion* from, Quaternion* to, GLdouble i)
{
 if (!res || !from || !to) return;
 res->x = i * (to->x - from->x) + from->x;
 res->y = i * (to->y - from->y) + from->y;
 res->z = i * (to->z - from->z) + from->z;
 res->w = i * (to->w - from->w) + from->w;
}


void interpolate_frame(Frame* dst, Frame* a, Frame* b, GLdouble i)
{
 if (!dst || !a || !b) return;
 interpol_quaternion(&dst->q, &a->q, &b->q, i); 
 get_matrix_from_quaternion(dst->matrix, &dst->q);
/* I_matrix(dst->matrix, 4);*/
 dst->dx = a->dx + (b->dx - a->dx) * i;
 dst->dy = a->dy + (b->dy - a->dy) * i;
 dst->dz = a->dz + (b->dz - a->dz) * i;
 dst->matrix[0][3] = dst->dx;
 dst->matrix[1][3] = dst->dy;
 dst->matrix[2][3] = dst->dz;
 /*dst->matrix[0][3] = 6.;
 dst->matrix[1][3] = 6.;
 dst->matrix[2][3] = 6.;*/
}

void init_arrays()
{
 GLint i;
 if (sol)
   {
    for (i=0;i<NINT;i++) { free(sol[i]); sol[i] = NULL; }
    free(sol);
    sol = NULL;
   }
 if (bsol) { free(bsol); bsol = NULL; }
 if (ifrm) 
   { 
    for (i=0;i<NINT;i++) free_frame(&ifrm[i]);
    free(ifrm); 
    ifrm = NULL; 
   }
 sol = (PUMA**)malloc(NINT*sizeof(PUMA*));
 for (i=0;i<NINT;i++) sol[i] = (PUMA*)malloc(8*sizeof(PUMA));
 bsol = (PUMA**)malloc(NINT*sizeof(PUMA*));
 ifrm = (Frame*)malloc(NINT*sizeof(Frame));
 for (i=0;i<NINT;i++) init_frame(&ifrm[i]);
}

GLdouble parametric_distance(PUMA* a, PUMA* b)
{
 if (!a || !b) return 1e10;
 return fabs(a->a1 - b->a1) +
        fabs(a->a2 - b->a2) +
        fabs(a->a3 - b->a3) +
        fabs(a->a4 - b->a4) +
        fabs(a->a5 - b->a5) +
        fabs(a->q2 - b->q2);
}

GLdouble interpolate_angle(GLdouble a, GLdouble b, GLdouble f)
{
 GLdouble ret;
/* printf("a = %f, b = %f, f = %f\n", a,b,f);*/
 if (fabs(a-b) <= 180.) ret = a + (b - a)*f;
 else
   {
       a += 180.;
       b += 180;
       if (a >= 360.) a -= 360.;
       if (b >= 360.) b -= 360.;
/*       printf("now: a = %f, b = %f, f = %f\n", a,b,f);*/
       ret = a + (b - a)*f;
       ret -= 180.;
       if (ret < 0.) ret += 360.;
/*      printf("ret = %f\n", ret);*/
   }
 return ret;
}

PUMA* interpol_parameters(PUMA* a, PUMA* b, GLdouble f)
{
 PUMA* r;
 if (!a || !b) return NULL;
 r = (PUMA*)malloc(sizeof(PUMA));
 /*printf("pa: %f %f %f %f %f (%f)\n", a->a1, a->a2, a->a3, a->a4, a->a5, f);
 printf("pb: %f %f %f %f %f (%f)\n", b->a1, b->a2, b->a3, b->a4, b->a5, f);*/
 r->a1 = interpolate_angle(a->a1, b->a1, f);
 r->a2 = interpolate_angle(a->a2, b->a2, f);
 r->a3 = interpolate_angle(a->a3, b->a3, f);
 r->a4 = interpolate_angle(a->a4, b->a4, f);
 r->a5 = interpolate_angle(a->a5, b->a5, f);
 r->q2 = a->q2 + (b->q2 - a->q2)*f;
 r->l1 = a->l1;
 r->l3 = a->l3;
 r->l4 = a->l4;
 r->solved = 1;
 printf("parameters interpolated!\n");
 return r;		/* FIXME: memory leak */
}

GLint interpolate_parametric(GLint cidx, GLint imi, GLint ima, PUMA** bsol, PUMA* prev, PUMA* nexta)
{
 GLint size, i, j, nsol, curidx;
 GLdouble dist, cdist;
 if (!bsol || !prev || !nexta) return -1;
 size = ima - imi;
 printf("parametric size = %d\n", size);
 dist = 1e10;
 nsol = 0;
 curidx = -1;
 if ((cdist = parametric_distance(prev, &nexta[cidx])) < 20.)
   {
    dist = cdist;
    curidx = cidx;
   }
 else
 {
 for (j=0;j<8;j++)
   {
    if (!nexta[j].solved) continue;
    nsol ++;
    cdist = parametric_distance(prev, &nexta[j]);
    if (cdist < dist) { dist = cdist; curidx = j; }
       printf("parametric: dist = %f, j = %d\n", dist, j);
   }
 }
 printf("parametric final dist = %f, curidx = %d\n", dist, curidx);
 if (curidx < 0 || !nsol) fatal("cannot solve parametric");
 for (i=imi;i<=ima;i++) bsol[i] = interpol_parameters(prev, &nexta[curidx], (GLdouble)(i-imi)/(GLdouble)size);
 printf("parametric new curidx after is: %d\n", curidx);
 return curidx;
}

void find_best_solution()
{
 GLint i,j;
 GLint curidx, previdx, nsol, imi, ima;
 GLdouble dist, cdist;
 GLdouble mdist;
 curidx = -1;
 previdx = 0;
 while (!sol[0][i].solved && i < 8) i++;
 if (i >= 8) fatal("cannot find initial solution!");
 bsol[0] = &sol[0][i];
 mdist = 10.;
 for (i=1;i<NINT;i++)
 {
/*	 printf("curidx = %d\n", curidx);*/
  dist = 1e10;
  nsol = 0;
  for (j=0;j<8;j++) 
      {
/*     printf("%d", sol[i][j].solved);*/
       if (!sol[i][j].solved) continue;
       nsol ++;
       cdist = parametric_distance(bsol[i-1], &sol[i][j]);
       if (cdist < dist) 
         { 
	  dist = cdist; 
	  previdx = curidx; 
	  curidx = j; 
	 }
       printf("dist = %f, j = %d\n", dist, j);
      }
  if (curidx < 0) fatal("cannot resolve curidx");
  printf("final dist = %f, final curidx = %d\n", dist, curidx);
  if (dist > mdist || !nsol) 
    {
     printf("dist = %f, i=%d, nsol=%d\n", dist, i, nsol);
     imi = i-4;
     if (imi < 0) imi = 0;
     ima = i+4;
     if (ima >= NINT) ima = NINT-1;
     previdx = curidx;
     curidx = interpolate_parametric(curidx, imi, ima, bsol, bsol[imi], sol[ima]);
     i = ima;
     continue;
    }
/*  printf("\n");*/
  bsol[i] = &sol[i][curidx];
 }
}

void make_frame_from_puma(Frame* f, PUMA* p)
{
 if (!f || !p) return;
 init_frame(f);
 rotate_frameR(f, 0., 0., p->a1);
 move_frameD(f, 0., 0., p->l1);
 rotate_frameR(f, 0., p->a2, 0.);
 move_frameD(f, p->q2, 0., 0.);
 rotate_frameR(f, 0., p->a3, 0.);
 move_frameD(f, 0., 0., -p->l3);
 rotate_frameR(f, 0., 0., p->a4);
 move_frameD(f, p->l4, 0., 0.);
 rotate_frameR(f, p->a5, 0., 0.);
 /*
  glRotated(p->a1, 0., 0., 1.);
  
  glTranslated(0., 0., p->l1);
  glRotated(p->a2, 0., 1., 0.);
  
  glTranslated(p->q2, 0., 0.);
  glRotated(p->a3, 0., 1., 0.);
  
  glTranslated(0., 0., -p->l3);
  glRotated(p->a4, 0., 0., 1.);
  
  glTranslated(p->l4, 0., 0.);
  glRotated(p->a5, 1., 0., 0.);
  */
}

void solve_engine_parametric()
{
 GLint i;
 init_arrays();
 solve_puma(&puma, sol[0], &A); 
 solve_puma(&puma, sol[NINT-1], &B); 
 while (!sol[0][i].solved && i < 8) i++;
 if (i >= 8) fatal("cannot find parametric initial solution!");
 bsol[0] = &sol[0][i];
 interpolate_parametric(i, 0, NINT-1, bsol, bsol[0], sol[NINT-1]);
 /*for (i=0;i<NINT;i++) make_frame_from_puma(&ifrm[i], bsol[i]);*/
 explicit = 0;
}



void solve_engine_analitic_quaternion()
{
 GLint i;
 init_arrays();
 get_quaternion_from_matrix(&A.q, A.matrix);
 get_quaternion_from_matrix(&B.q, B.matrix);
 /*printf("A> %f,%f,%f,%f (%f, %f, %f) (%f %f %f)\n", A.q.x, A.q.y, A.q.z, A.q.w, A.dx, A.dy, A.dz, A.matrix[0][3], A.matrix[1][3], A.matrix[2][3]);
 printf("B> %f,%f,%f,%f (%f, %f, %f) (%f %f %f)\n", B.q.x, B.q.y, B.q.z, B.q.w, B.dx, B.dy, B.dz, B.matrix[0][3], B.matrix[1][3], B.matrix[2][3]);*/
 for (i=0;i<NINT;i++)
   {
    interpolate_frame(&ifrm[i], &A, &B, (GLdouble)i / (GLdouble)NINT);
    solve_puma(&puma, sol[i], &ifrm[i]);
/*    printf("%d%d%d%d%d%d%d%d\n", sol[0].solved,  sol[1].solved,  sol[2].solved,  sol[3].solved,  sol[4].solved,  sol[5].solved,  sol[6].solved,  sol[7].solved);*/
   }
 find_best_solution();
 explicit = 1;
}

void next_idx()
{
 if (rising) idx++;
 else idx --;
 if (idx >= NINT) { idx = NINT-1; rising = !rising; }
 if (idx < 0) { idx = 0; rising = ! rising; }
/* printf("idx = %d\n", idx);*/
}


void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
        case '0': auto_move = ! auto_move; break;
	case '1': puma.a1 += 5.; check_puma(&puma); break;
	case '2': puma.a2 += 5.; check_puma(&puma); break;
	case '3': puma.a3 += 5.; check_puma(&puma); break;
	case '4': puma.a4 += 5.; check_puma(&puma); break;
	case '5': puma.a5 += 5.; check_puma(&puma); break;
	case '6': puma.q2 += .1; check_puma(&puma); break;
	case '7': puma.q2 -= .1; check_puma(&puma); break;
        case 'i': angX -= 10.; break;
        case 'k': angX += 10.; break;
        case 'j': angY += 10.; break;
        case 'l': angY -= 10.; break;
        case 'o': angZ += 10.; break;
        case 'm': angZ -= 10.; break;
	case '/': explicit = ! explicit; break;
	case '.': moving = ! moving; break;
	case ',': next_idx(); break;
	case 'p': switch_current_frame(); break;
	case 'r': move_frameD(currFrame, .1, 0., 0.); break;
	case 't': move_frameD(currFrame, -.1, 0., 0.); break;
	case 'f': move_frameD(currFrame, 0., .1, 0.); break;
	case 'g': move_frameD(currFrame, 0., -.1, 0.); break;
	case 'v': move_frameD(currFrame, 0., 0., .1); break;
	case 'b': move_frameD(currFrame, 0., 0., -.1); break;
	case 'w': rotate_frameR(currFrame, 5., 0., 0.); break;
	case 's': rotate_frameR(currFrame, -5., 0., 0.); break;
	case 'a': rotate_frameR(currFrame, 0., 5., 0.); break;
	case 'd': rotate_frameR(currFrame, 0., -5., 0.); break;
	case 'e': rotate_frameR(currFrame, 0., 0., 5.); break;
	case 'c': rotate_frameR(currFrame, 0., 0., -5.); break;
        case '-': scaled /= 1.3; break;
	case '=': case '+': scaled *= 1.3; break;
	case 'u': solve_engine_analitic_quaternion(); break;
	case 'y': solve_engine_parametric(); break;
	case' ': scaled = .7; angX = -60.; angZ = -80.; angY = 0.; auto_move = 0; break;
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
    sprintf(tstr, "GLScene FPS: %d",fps/(int)(t2-t1));
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }
}


void enable_light()
{
    GLfloat aL[] = { .2f, .2f, .4f, 1.f };
    GLfloat dL[] = { 1.f, 1.f, 0.7f, 1.f } ;
    GLfloat sL[] = { .5f, 0.f, 1.f, 1.f } ;
    GLfloat lP[] = { 0.f, 0.f, 0.f, 1.f };
    GLfloat mA[] = { 1.f, 1.f, 1.f, 1.f };
    GLfloat mD[] = { 1.f, 1.f, 1.f, 1.f };
    GLfloat mS[] = { 127.0 };
    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mD);
    glMaterialfv(GL_FRONT, GL_SHININESS, mS);
    glMaterialfv(GL_BACK, GL_AMBIENT, mA);
    glMaterialfv(GL_BACK, GL_DIFFUSE, mD);
    glMaterialfv(GL_BACK, GL_SHININESS, mS);
    glLightfv(GL_LIGHT0, GL_AMBIENT, aL);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dL);
    glLightfv(GL_LIGHT0, GL_SHININESS, sL);
    glLightfv(GL_LIGHT0, GL_POSITION, lP);
    glEnable(GL_LIGHT0);
}


void mglCubeR3(GLdouble a)
{
 glBegin(GL_QUADS);
   glNormal3d(-R3D3, R3D3, -R3D3);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, a, -a);
   glNormal3d(R3D3, R3D3, -R3D3);
   glTexCoord2d(1., 0.);
   glVertex3d(a, a, -a);
   glNormal3d(R3D3, -R3D3, -R3D3);
   glTexCoord2d(1., 1.);
   glVertex3d(a, -a, -a);
   glNormal3d(-R3D3, -R3D3, -R3D3);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, -a, -a);
   glNormal3d(-R3D3, R3D3, R3D3);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, a, a);
   glNormal3d(-R3D3, -R3D3, R3D3);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, -a, a);
   glNormal3d(R3D3, -R3D3, R3D3);
   glTexCoord2d(1., 1.);
   glVertex3d(a, -a, a);
   glNormal3d(R3D3, R3D3, R3D3);
   glTexCoord2d(1., 0.);
   glVertex3d(a, a, a);
   glNormal3d(R3D3, R3D3, -R3D3);
   glTexCoord2d(0., 0.);
   glVertex3d(a, a, -a);
   glNormal3d(R3D3, R3D3, R3D3);
   glTexCoord2d(1., 0.);
   glVertex3d(a, a, a);
   glNormal3d(R3D3, -R3D3, R3D3);
   glTexCoord2d(1., 1.);
   glVertex3d(a, -a, a);
   glNormal3d(R3D3, -R3D3, -R3D3);
   glTexCoord2d(0., 1.);
   glVertex3d(a, -a, -a);
   glNormal3d(-R3D3, R3D3, -R3D3);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, a, -a);
   glNormal3d(-R3D3, -R3D3, -R3D3);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, -a, -a);
   glNormal3d(-R3D3, -R3D3, R3D3);
   glTexCoord2d(1., 1.);
   glVertex3d(-a, -a, a);
   glNormal3d(-R3D3, R3D3, R3D3);
   glTexCoord2d(1., 0.);
   glVertex3d(-a, a, a);
   glNormal3d(-R3D3, R3D3, -R3D3);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, a, -a);
   glNormal3d(-R3D3, R3D3, R3D3);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, a, a);
   glNormal3d(R3D3, R3D3, R3D3);
   glTexCoord2d(1., 1.);
   glVertex3d(a, a, a);
   glNormal3d(R3D3, R3D3, -R3D3);
   glTexCoord2d(1., 0.);
   glVertex3d(a, a, -a);
   glNormal3d(-R3D3, -R3D3, -R3D3);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, -a, -a);
   glNormal3d(R3D3, -R3D3, -R3D3);
   glTexCoord2d(1., 0.);
   glVertex3d(a, -a, -a);
   glNormal3d(R3D3, -R3D3, R3D3);
   glTexCoord2d(1., 1.);
   glVertex3d(a, -a, a);
   glNormal3d(-R3D3, -R3D3, R3D3);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, -a, a);
 glEnd();
}


void mglCube(GLdouble a)
{
 glBegin(GL_QUADS);
   glNormal3d(0., 0., -1.);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, a, -a);
   glNormal3d(0., 0., -1.);
   glTexCoord2d(1., 0.);
   glVertex3d(a, a, -a);
   glNormal3d(0., 0., -1.);
   glTexCoord2d(1., 1.);
   glVertex3d(a, -a, -a);
   glNormal3d(0., 0., -1.);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, -a, -a);
   glNormal3d(0., 0., 1.);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, a, a);
   glNormal3d(0., 0., 1.);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, -a, a);
   glNormal3d(0., 0., 1.);
   glTexCoord2d(1., 1.);
   glVertex3d(a, -a, a);
   glNormal3d(0., 0., 1.);
   glTexCoord2d(1., 0.);
   glVertex3d(a, a, a);
   glNormal3d(1., 0., 0.);
   glTexCoord2d(0., 0.);
   glVertex3d(a, a, -a);
   glNormal3d(1., 0., 0.);
   glTexCoord2d(1., 0.);
   glVertex3d(a, a, a);
   glNormal3d(1., 0., 0.);
   glTexCoord2d(1., 1.);
   glVertex3d(a, -a, a);
   glNormal3d(1., 0., 0.);
   glTexCoord2d(0., 1.);
   glVertex3d(a, -a, -a);
   glNormal3d(-1., 0., 0.);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, a, -a);
   glNormal3d(-1., 0., 0.);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, -a, -a);
   glNormal3d(-1., 0., 0.);
   glTexCoord2d(1., 1.);
   glVertex3d(-a, -a, a);
   glNormal3d(-1., 0., 0.);
   glTexCoord2d(1., 0.);
   glVertex3d(-a, a, a);
   glNormal3d(0., 1., 0.);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, a, -a);
   glNormal3d(0., 1., 0.);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, a, a);
   glNormal3d(0., 1., 0.);
   glTexCoord2d(1., 1.);
   glVertex3d(a, a, a);
   glNormal3d(0., 1., 0.);
   glTexCoord2d(1., 0.);
   glVertex3d(a, a, -a);
   glNormal3d(0., -1., 0.);
   glTexCoord2d(0., 0.);
   glVertex3d(-a, -a, -a);
   glNormal3d(0., -1., 0.);
   glTexCoord2d(1., 0.);
   glVertex3d(a, -a, -a);
   glNormal3d(0., -1., 0.);
   glTexCoord2d(1., 1.);
   glVertex3d(a, -a, a);
   glNormal3d(0., -1., 0.);
   glTexCoord2d(0., 1.);
   glVertex3d(-a, -a, a);
 glEnd();
}


void mglConeZ(GLdouble r, GLdouble h)
{
 GLint i;
 GLdouble angle1, angle2;
 GLdouble tc1, tc2;
 GLdouble x1, x2, y1, y2;
 GLdouble x1n, x2n, y1n, y2n;
 glBegin(GL_QUADS);
   for (i=0;i<32;i++)
   {
    angle1 = ((GLdouble)(i)*TWOPI)/32.;
    angle2 = ((GLdouble)(i+1)*TWOPI)/32.;
    tc1 = ((GLdouble)(i))/32.;
    tc2 = ((GLdouble)(i+1))/32.;
    x1n = sin(angle1);
    x2n = sin(angle2);
    y1n = cos(angle1);
    y2n = cos(angle2);
    x1 = x1n * r;
    x2 = x2n * r;
    y1 = y1n * r;
    y2 = y2n * r;
    glNormal3d(x1n, y1n, 0.);
    glTexCoord2d(tc1, 0.);
    glVertex3d(x1, y1, h);
    glNormal3d(x2n, y2n, 0.);
    glTexCoord2d(tc2, 0.);
    glVertex3d(x2, y2, h);
    glNormal3d(x2n, y2n, 0.);
    glTexCoord2d(tc2, 1.);
    glVertex3d(x2, y2, 0.);
    glNormal3d(x1n, y1n, 0.);
    glTexCoord2d(tc1, 1.);
    glVertex3d(x1, y1, 0.);
   }
 glEnd();
}


void mglConeY(GLdouble r, GLdouble h)
{
 GLint i;
 GLdouble angle1, angle2;
 GLdouble tc1, tc2;
 GLdouble x1, x2, y1, y2;
 GLdouble x1n, x2n, y1n, y2n;
 glBegin(GL_QUADS);
   for (i=0;i<32;i++)
   {
    angle1 = ((GLdouble)(i)*TWOPI)/32.;
    angle2 = ((GLdouble)(i+1)*TWOPI)/32.;
    tc1 = ((GLdouble)(i))/32.;
    tc2 = ((GLdouble)(i+1))/32.;
    x1n = sin(angle1);
    x2n = sin(angle2);
    y1n = cos(angle1);
    y2n = cos(angle2);
    x1 = x1n * r;
    x2 = x2n * r;
    y1 = y1n * r;
    y2 = y2n * r;
    glNormal3d(x1n, 0., y1n);
    glTexCoord2d(tc1, 0.);
    glVertex3d(x1, h, y1);
    glNormal3d(x2n, 0., y2n);
    glTexCoord2d(tc2, 0.);
    glVertex3d(x2, h, y2);
    glNormal3d(x2n, 0., y2n);
    glTexCoord2d(tc2, 1.);
    glVertex3d(x2, 0., y2);
    glNormal3d(x1n, 0., y1n);
    glTexCoord2d(tc1, 1.);
    glVertex3d(x1, 0., y1);
   }
 glEnd();
}


void mglConeX(GLdouble r, GLdouble h)
{
 GLint i;
 GLdouble angle1, angle2;
 GLdouble tc1, tc2;
 GLdouble x1, x2, y1, y2;
 GLdouble x1n, x2n, y1n, y2n;
 glBegin(GL_QUADS);
   for (i=0;i<32;i++)
   {
    angle1 = ((GLdouble)(i)*TWOPI)/32.;
    angle2 = ((GLdouble)(i+1)*TWOPI)/32.;
    tc1 = ((GLdouble)(i))/32.;
    tc2 = ((GLdouble)(i+1))/32.;
    x1n = sin(angle1);
    x2n = sin(angle2);
    y1n = cos(angle1);
    y2n = cos(angle2);
    x1 = x1n * r;
    x2 = x2n * r;
    y1 = y1n * r;
    y2 = y2n * r;
    glNormal3d(0., y1n, x1n);
    glTexCoord2d(tc1, 0.);
    glVertex3d(h, y1, x1);
    glNormal3d(0., y2n, x2n);
    glTexCoord2d(tc2, 0.);
    glVertex3d(h, y2, x2);
    glNormal3d(0., y2n, x2n);
    glTexCoord2d(tc2, 1.);
    glVertex3d(0., y2, x2);
    glNormal3d(0., y1n, x1n);
    glTexCoord2d(tc1, 1.);
    glVertex3d(0., y1, x1);
   }
 glEnd();
}

void mglCone2Z(GLdouble r, GLdouble q, GLdouble h)
{
 GLint i;
 GLdouble angle1, angle2;
 GLdouble tc1, tc2;
 GLdouble x1, x2, y1, y2;
 GLdouble x1q, x2q, y1q, y2q;
 GLdouble x1n, x2n, y1n, y2n;
 glBegin(GL_QUADS);
   for (i=0;i<32;i++)
   {
    angle1 = ((GLdouble)(i)*TWOPI)/32.;
    angle2 = ((GLdouble)(i+1)*TWOPI)/32.;
    tc1 = ((GLdouble)(i))/32.;
    tc2 = ((GLdouble)(i+1))/32.;
    x1n = sin(angle1);
    x2n = sin(angle2);
    y1n = cos(angle1);
    y2n = cos(angle2);
    x1 = x1n * r;
    x2 = x2n * r;
    y1 = y1n * r;
    y2 = y2n * r;
    x1q = x1n * q;
    x2q = x2n * q;
    y1q = y1n * q;
    y2q = y2n * q;
    glNormal3d(x1n, y1n, 0.);
    glTexCoord2d(tc1, 0.);
    glVertex3d(x1, y1, h);
    glNormal3d(x2n, y2n, 0.);
    glTexCoord2d(tc2, 0.);
    glVertex3d(x2, y2, h);
    glNormal3d(x2n, y2n, 0.);
    glTexCoord2d(tc2, 1.);
    glVertex3d(x2q, y2q, 0.);
    glNormal3d(x1n, y1n, 0.);
    glTexCoord2d(tc1, 1.);
    glVertex3d(x1q, y1q, 0.);
   }
 glEnd();
}


void mglCone2Y(GLdouble r, GLdouble q, GLdouble h)
{
 GLint i;
 GLdouble angle1, angle2;
 GLdouble tc1, tc2;
 GLdouble x1, x2, y1, y2;
 GLdouble x1q, x2q, y1q, y2q;
 GLdouble x1n, x2n, y1n, y2n;
 glBegin(GL_QUADS);
   for (i=0;i<32;i++)
   {
    angle1 = ((GLdouble)(i)*TWOPI)/32.;
    angle2 = ((GLdouble)(i+1)*TWOPI)/32.;
    tc1 = ((GLdouble)(i))/32.;
    tc2 = ((GLdouble)(i+1))/32.;
    x1n = sin(angle1);
    x2n = sin(angle2);
    y1n = cos(angle1);
    y2n = cos(angle2);
    x1 = x1n * r;
    x2 = x2n * r;
    y1 = y1n * r;
    y2 = y2n * r;
    x1q = x1n * q;
    x2q = x2n * q;
    y1q = y1n * q;
    y2q = y2n * q;
    glNormal3d(x1n, 0., y1n);
    glTexCoord2d(tc1, 0.);
    glVertex3d(x1, h, y1);
    glNormal3d(x2n, 0., y2n);
    glTexCoord2d(tc2, 0.);
    glVertex3d(x2, h, y2);
    glNormal3d(x2n, 0., y2n);
    glTexCoord2d(tc2, 1.);
    glVertex3d(x2q, 0., y2q);
    glNormal3d(x1n, 0., y1n);
    glTexCoord2d(tc1, 1.);
    glVertex3d(x1q, 0., y1q);
   }
 glEnd();
}


void mglCone2X(GLdouble r, GLdouble q, GLdouble h)
{
 GLint i;
 GLdouble angle1, angle2;
 GLdouble tc1, tc2;
 GLdouble x1, x2, y1, y2;
 GLdouble x1q, x2q, y1q, y2q;
 GLdouble x1n, x2n, y1n, y2n;
 glBegin(GL_QUADS);
   for (i=0;i<32;i++)
   {
    angle1 = ((GLdouble)(i)*TWOPI)/32.;
    angle2 = ((GLdouble)(i+1)*TWOPI)/32.;
    tc1 = ((GLdouble)(i))/32.;
    tc2 = ((GLdouble)(i+1))/32.;
    x1n = sin(angle1);
    x2n = sin(angle2);
    y1n = cos(angle1);
    y2n = cos(angle2);
    x1 = x1n * r;
    x2 = x2n * r;
    y1 = y1n * r;
    y2 = y2n * r;
    x1q = x1n * q;
    x2q = x2n * q;
    y1q = y1n * q;
    y2q = y2n * q;
    glNormal3d(0., y1n, x1n);
    glTexCoord2d(tc1, 0.);
    glVertex3d(h, y1, x1);
    glNormal3d(0., y2n, x2n);
    glTexCoord2d(tc2, 0.);
    glVertex3d(h, y2, x2);
    glNormal3d(0., y2n, x2n);
    glTexCoord2d(tc2, 1.);
    glVertex3d(0., y2q, x2q);
    glNormal3d(0., y1n, x1n);
    glTexCoord2d(tc1, 1.);
    glVertex3d(0., y1q, x1q);
   }
 glEnd();
}

void render_frame_from_puma(PUMA* p)
{
 if (!p) return;
 glPushMatrix();
  glScaled(5., 5., 5.);
  glRotated(p->a1, 0., 0., 1.);
  
  glTranslated(0., 0., p->l1);
  glRotated(p->a2, 0., 1., 0.);
  
  glTranslated(p->q2, 0., 0.);
  glRotated(p->a3, 0., 1., 0.);
  
  glTranslated(0., 0., -p->l3);
  glRotated(p->a4, 0., 0., 1.);
  
  glTranslated(p->l4, 0., 0.);
  glRotated(p->a5, 1., 0., 0.);
  
  mglConeX(.12, 1.0);
  mglConeY(.1, .75);
  mglConeZ(.14, .5);
  
 glPopMatrix();
}


void render_puma(PUMA* p)
{
 if (!p) return;
 glPushMatrix();
  glPushMatrix();
  
  glScaled(15., 15., 2.);
  mglCubeR3(1.);
  
  glPopMatrix();
  glScaled(5., 5., 5.);
  glRotated(p->a1, 0., 0., 1.);
  mglCone2Z(2., 3., -.5*p->l1);
  mglCone2Z(.7, 1.5, p->l1);
  
  glTranslated(0., 0., p->l1);
  mglCubeR3(1.01);
  glRotated(p->a2, 0., 1., 0.);
  mglCone2X(.4, .8, p->q2);
  
  glTranslated(p->q2, 0., 0.);
  mglCubeR3(0.71);
  glRotated(p->a3, 0., 1., 0.);
  mglCone2Z(.25, .55, -p->l3);
  
  glTranslated(0., 0., -p->l3);
  mglCubeR3(.51);
  glRotated(p->a4, 0., 0., 1.);
  mglCone2X(.2, .35, p->l4);
  
  glTranslated(p->l4, 0., 0.);
  mglCubeR3(.31);
  glRotated(p->a5, 1., 0., 0.);
  mglCubeR3(.3);
  mglConeX(.1, 3.);
  mglConeY(.15, 2.5);
  mglConeZ(.2, 2.);
  mglConeX(.29, .5);
  mglConeY(.25, .7);
  mglConeZ(.21, .3);
  
 glPopMatrix();
}

void computeRotation(GLdouble* dst, GLdouble** m)
{
 GLint i,j;
 dst[15] = 1.;
 dst[3] = dst[7] = dst[11] = dst[12] = dst[13] = dst[14] = 0.;
 for (i=0;i<3;i++) for (j=0;j<3;j++) dst[i+4*j] = m[i][j];
}

void render_frame_skel(Frame* f)
{
 GLdouble m[16];
 if (!f) return;
 glPushMatrix();
   glScaled(5., 5., 5.);
   glTranslated(f->matrix[0][3], f->matrix[1][3], f->matrix[2][3]);
   computeRotation(m, f->matrix);
   glMultMatrixd(m);
   /*glRotated(f->rx, 1., 0., 0.);
   glRotated(f->ry, 0., 1., 0.);
   glRotated(f->rz, 0., 0., 1.);*/
  mglConeX(.12, 1.0);
  mglConeY(.1, .75);
  mglConeZ(.14, .5);
 glPopMatrix();
}

void render_frame(Frame* f)
{
 if (!f) return;
 glPushMatrix();
   glScaled(5., 5., 5.);
   glTranslated(f->matrix[0][3], f->matrix[1][3], f->matrix[2][3]);
   glRotated(f->rx, 1., 0., 0.);
   glRotated(f->ry, 0., 1., 0.);
   glRotated(f->rz, 0., 0., 1.);
  mglCubeR3(.3);
  mglConeX(.15, 2.0);
  mglConeY(.2, 1.5);
  mglConeZ(.25, 1.0);
  mglConeX(.29, .6);
  mglConeY(.25, .8);
  mglConeZ(.21, .4);
 glPopMatrix();
}


void render_frames()
{
 GLint i;
 render_frame(&A);
 render_frame(&B);
 if (explicit) 
   {
    for (i=0;i<NINT;i++) if (!(i % NINTIV)) render_frame_skel(&ifrm[i]);
   }
 else 
   {
    for (i=0;i<NINT;i++) if (!(i % NINTIV)) render_frame_from_puma(bsol[i]);
   }
}


void check_angles()
{
 if (angX >= 360.) angX -= 360.;
 if (angX < 0.)    angX += 360.;
 if (angY >= 360.) angY -= 360.;
 if (angY < 0.)    angY += 360.;
 if (angZ >= 360.) angZ -= 360.;
 if (angZ < 0.)    angZ += 360.;
}


void infos()
{
 /*printf("A: D(%f,%f,%f), R(%f,%f,%f)\n", A.dx, A.dy, A.dz, A.rx, A.ry, A.rz);
 printf("B: D(%f,%f,%f), R(%f,%f,%f)\n", B.dx, B.dy, B.dz, B.rx, B.ry, B.rz);*/
}


void render_scene(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 /*glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
 glLoadIdentity();
 gluLookAt(0., 0., 100., 0. , 0., 0., 0., 1., 0.);
 glScaled(scaled, scaled, scaled);
 check_angles();
 glRotated(angX, 1., 0. , 0.);
 glRotated(angY, 0., 1. , 0.);
 glRotated(angZ, 0., 0. , 1.);
 render_frames();
 render_puma(bsol[idx]);
 if (auto_move)
  {
   angX += 2.3;
   angY += 2.5;
   angZ += 2.7;
  }
 if (moving) next_idx();
 infos();
 time_counter();
 glFlush();
 glutSwapBuffers();
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


void dyna_tex_data(Texture* t)
{
 t->sizx = t->sizy = 2;
 t->data = (GLuchar*)malloc(0X10*sizeof(GLuchar));
 t->data[(0X0<<2)+0X0] = 0X9A;
 t->data[(0X0<<2)+0X1] = 0X54;
 t->data[(0X0<<2)+0X2] = 0X54;
 t->data[(0X0<<2)+0X3] = 0XAF;
 t->data[(0X1<<2)+0X0] = 0XAA;
 t->data[(0X1<<2)+0X1] = 0XAA;
 t->data[(0X1<<2)+0X2] = 0XAA;
 t->data[(0X1<<2)+0X3] = 0XAF;
 t->data[(0X2<<2)+0X0] = 0X66;
 t->data[(0X2<<2)+0X1] = 0X66;
 t->data[(0X2<<2)+0X2] = 0X66;
 t->data[(0X2<<2)+0X3] = 0XAF;
 t->data[(0X3<<2)+0X0] = 0X54;
 t->data[(0X3<<2)+0X1] = 0X54;
 t->data[(0X3<<2)+0X2] = 0X9A;
 t->data[(0X3<<2)+0X3] = 0XAF;
}


void init_texture(Texture* t, int i)
{
 char fname[128];
 if (!t) fatal("texture struct is null");
 glEnable(GL_TEXTURE_2D);
 glGenTextures(1, &t->id);
 glBindTexture(GL_TEXTURE_2D, t->id);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 sprintf(fname, "tex%d.dat", i);
 dyna_tex_data(t);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->sizx, t->sizy, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->data);
 glBindTexture(GL_TEXTURE_2D, t->id);
}


void init_textures_array()
{
 int i;
 tList = (Texture*)malloc(sizeof(Texture));
 for (i=0;i<1;i++) init_texture(&tList[i], i);
}


void Init()
{
 help();
 angX = -60.; angZ = -80.; angY = 0.; 
 scaled = 0.7;
 auto_move = 0;
 init_puma(&puma);
 init_frame(&A);
 init_frame(&B);
 /*set_frameDR(&A, 0., 6., 6., 3., 20., 300.);*/    /* problems */
 /*set_frameDR(&B, 6., 6., 6., 340., 330., 20.);*/  /* with it */
 /*set_frameDR(&A, 4., -6., 3., 3., 20., 300.);
 set_frameDR(&B, 8., 6., 9., 340., 330., 20.);*/
 set_frameDR(&A, 7.7, -8., 4.1, 58., 300., 345.);
 set_frameDR(&B, 6.6, 7.3, 10.6, 80., 330., 20.);
 /*set_frameDR(&A, 4., -6., 3., 0., 0., 0.);
 set_frameDR(&B, 8., 6., 9., 0., 0., 0.);*/
 R3D3 = sqrt(3.) / 3.;
 currFrame = &A;
 sol = NULL;
 bsol = NULL;
 ifrm = NULL;
 idx = 1;
 rising = 1;
 explicit = 1;
 moving = 1;
 solve_engine_analitic_quaternion();
}


void glInit()
{
 glClearColor(0.5,0.5,0.5,0.0);
 glShadeModel(GL_SMOOTH);
 glEnable(GL_DEPTH_TEST);
 enable_light();
}


int main(int lb, char** par)
{
 /*GLdouble ret;
 GLint i;*/
 wx = 800;
 wy = 600;
 
 /*for (i=0;i<=10;i++)
   {
    ret = interpolate_angle(90., 271., (GLdouble)i/10.);
    printf("RESULT: %f\n", ret);
   }
exit(1);*/
 
 Init();
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(100, 100);
 glutCreateWindow(par[0]);
 glutDisplayFunc(render_scene);
 glutReshapeFunc(resize_scene);
 glutKeyboardFunc(keyboard);
 glutVisibilityFunc(visible);
 glInit();
 init_textures_array();
 glutMainLoop();
 return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
