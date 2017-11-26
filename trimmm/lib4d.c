#include "lib4d.h"

/*int max(int u, int v)
{
 return (u > v) ? u : v;
}

int min(int u, int v)
{
 return (v > u) ? u : v;
}*/

unsigned long RGB16(int r, int g, int b)	/* return color on 16bit display */
{
 TRACE
 return ((r>>0x3)<<0xb)+((g>>0x2)<<0x5)+(b>>0x3);
}


unsigned long RGB24(int r, int g, int b)	/* return color on 24bit display */
{
 TRACE
 return (r<<0x10)+(g<<0x8)+b;
}


void print_vector(struct Vector4D* vec)		/* print vector */
{
 TRACE
 printf("%p = (%f,%f,%f,%f,%f)\n", (void*)vec, vec->p[0], vec->p[1], vec->p[2], vec->p[3],vec->w);
}


void print_matrix(struct Matrix4D* m)		/* print matrix */
{
 int i,j;
 TRACE
 printf(" ==== %p ====\n", (void*)m);
 for (i=0;i<DIM;i++)
   {
    for (j=0;j<DIM;j++) printf("%f\t", m->v[j].p[i]);
    printf("\n");
   }
}


void printf_vector(char* name, struct Vector4D* vec)	/* print vector with name */
{
 TRACE
 if (vec) printf("%s(%p) = (%f,%f,%f,%f,%f)\n", name, (void*)vec, vec->p[0], vec->p[1], vec->p[2], vec->p[3], vec->w);
 else printf("%s(null)\n", name);
}


void printf_point(char* name, struct Polygon4D* p)	/* print point with name */
{
 TRACE
 if (p) printf_vector(name, &p->p);
 else printf("%s(null)\n", name);
}


void printf_matrix(char* name, struct Matrix4D* m)	/* print matrix with name */
{
 int i,j;
 TRACE
 printf(" ==== %s(%p) ====\n", name, (void*)m);
 for (i=0;i<DIM;i++)
   {
    for (j=0;j<DIM;j++) printf("%f\t", m->v[j].p[i]);
    printf("\n");
   }
}


void copy_vector(struct Vector4D* dst, struct Vector4D* src)	/* copy vector */
{
 /*register int i;*/
 TRACE
/* for (i=0;i<DIM;i++) dst->p[i] = src->p[i];*/
 dst->p[0] = src->p[0];
 dst->p[1] = src->p[1];
 dst->p[2] = src->p[2];
 dst->p[3] = src->p[3];		/* for speed */
 dst->w = src->w;
}

			/* dst = M*v */

void matrix_mul_vector(struct Vector4D* dst, struct Matrix4D* m, struct Vector4D* v)
{
 register int j,i;
 TRACE
 for (i=0;i<DIM;i++)
    {
     dst->p[i] = 0.0;
     for (j=0;j<DIM;j++) dst->p[i] += v->p[j]*m->v[j].p[i];
    }
}

			/* dst = vT*M vector is transponed first */

void vector_mul_matrix(struct Vector4D* dst, struct Vector4D* v, struct Matrix4D* m)
{
 register int j,i;
 TRACE
 for (i=0;i<DIM;i++)
    {
     dst->p[i] = 0.0;
     for (j=0;j<DIM;j++) dst->p[i] += v->p[j]*m->v[i].p[j];
    }
}

			/* inner product of two vectors */

double vector_product(struct Vector4D* v1, struct Vector4D* v2)
{
 register int i;
 double reslt;
 TRACE
 reslt = 0.0;
 for (i=0;i<DIM;i++) reslt += v1->p[i] * v2->p[i];
 return reslt;
}


double vector_product_3d(struct Vector4D* v1, struct Vector4D* v2)
{
 register int i;
 double reslt;
 TRACE
 reslt = 0.0;
 for (i=0;i<DIM-1;i++) reslt += v1->p[i] * v2->p[i];
 return reslt;
}

			/* use vectors V as norm to normalize vector */

void normalize_vector_byv(struct Vector4D* v)
{
 double norm;
 register int i;
 TRACE
 norm = v->p[DIM-1];
 if (norm == 0.0)
   {
       printf("DIV0\n");
       exit(1);
   }
 for (i=0;i<DIM;i++) v->p[i] /= norm;
}


void average_vector(struct Vector4D* v1, struct Vector4D* v2)
{
 struct Vector4D temp;
 int i;
 TRACE
 for (i=0;i<DIM-1;i++)
  {
   temp.p[i] = (v1->p[i] + v2->p[i])/2.;
   v1->p[i] = v2->p[i] = temp.p[i];
  }
}

			/* construct vector from parameters */

void construct_vector(struct Vector4D* vec, double a1, double a2, double a3, double a4)
{
 TRACE
 vec->p[0] = a1;
 vec->p[1] = a2;
 vec->p[2] = a3;
 vec->p[3] = a4;
 vec->w    = 1.;
}


void construct_vector3(struct Vector4D* vec, double a1, double a2, double a3)
{
 TRACE
 vec->p[0] = a1;
 vec->p[1] = a2;
 vec->p[2] = a3;
 vec->p[3] = 1.;
 vec->w    = 1.;
}


void construct_vector5(struct Vector4D* vec, double a1, double a2, double a3, double a4, double a5)
{
 TRACE
 vec->p[0] = a1;
 vec->p[1] = a2;
 vec->p[2] = a3;
 vec->p[3] = a4;
 vec->w    = a5;
}

			/* construct matrix from 16 parameters */
void construct_matrix(struct Matrix4D* m,
	double a11, double a12, double a13, double a14,
	double a21, double a22, double a23, double a24,
	double a31, double a32, double a33, double a34,

	double a41, double a42, double a43, double a44)
{
 TRACE
 m->v[0].p[0] = a11;
 m->v[1].p[0] = a12;
 m->v[2].p[0] = a13;
 m->v[3].p[0] = a14;
 m->v[0].p[1] = a21;
 m->v[1].p[1] = a22;
 m->v[2].p[1] = a23;
 m->v[3].p[1] = a24;
 m->v[0].p[2] = a31;
 m->v[1].p[2] = a32;
 m->v[2].p[2] = a33;
 m->v[3].p[2] = a34;
 m->v[0].p[3] = a41;
 m->v[1].p[3] = a42;
 m->v[2].p[3] = a43;
 m->v[3].p[3] = a44;
 m->v[0].w = 1.;
 m->v[1].w = 1.;
 m->v[2].w = 1.;
 m->v[3].w = 1.;
}

		/* copy matrix */

void copy_matrix(struct Matrix4D* dst, struct Matrix4D* src)
{
 register int j,i;
 TRACE
 for (i=0;i<DIM;i++) for (j=0;j<DIM;j++) dst->v[i].p[j] = src->v[i].p[j];
}

		/* copy first 3x3 matrix to dest, REST of dest UNINITIALIZED! */
		/* [***-]  * - copied
		 * [***-]  - - uninitialized
		 * {***-]
		 * [----] */

void copy_3d_matrix(struct Matrix4D* dst, struct Matrix4D* src)
{
 register int j,i;
 TRACE
 for (i=0;i<DIM-1;i++) for (j=0;j<DIM;j++) dst->v[i].p[j] = src->v[i].p[j];
}

		/* dst = M*N brutal force N^3 */

void matrix_mul(struct Matrix4D* dst, struct Matrix4D* m, struct Matrix4D* n)
{
 register int k,j,i;
 TRACE
 for (i=0;i<DIM;i++)
 for (j=0;j<DIM;j++)
    {
     dst->v[i].p[j] = 0.0;
     for (k=0;k<DIM;k++) dst->v[i].p[j] += m->v[k].p[j] * n->v[i].p[k];
    }
}

		/* creates I4 matrix */

void identity_matrix(struct Matrix4D* dst)
{
 register int j,i;
 TRACE
 for (i=0;i<DIM;i++) for (j=0;j<DIM;j++)
     if (i==j) dst->v[i].p[j] = 1.0;
     else      dst->v[i].p[j] = 0.0;
}

		/* swaps rows in m, starting from idx if on diagonal is non-zero element */
		/* used when inverting matrix, to avoid division by 0 */

int try_swap(struct Matrix4D* m, int idx)
{
 register int x;
 TRACE
 for (x=idx;x<DIM;x++) if (m->v[idx].p[x]) return x;
 return -1;
}

		/* brutal force invert matrix, N^3 */
		/* can cause exit if uninvertable matrix */
		/* gauss-like algorithm */

void invert_matrix(struct Matrix4D* dst, struct Matrix4D* srcC)
{
 struct Matrix4D src;
 double div, pom;
 register int x,k;
 int i,swit;
 struct Vector4D vectmp;
 TRACE
 copy_matrix(&src, srcC);
 identity_matrix(dst);
 for (i=0;i<DIM;i++)
   {
    div = src.v[i].p[i];
    if (div == 0.0)
      {
       swit = try_swap(&src, i);
       if (swit < 0)
         {
	   printf("UNINVERTABLE_MATRIX\n");
	   print_matrix(srcC);
	   exit(1);
	 }
       for (x=0;x<DIM;x++) vectmp.p[x]       = src.v[x].p[i];
       for (x=0;x<DIM;x++) src.v[x].p[i]     = src.v[x].p[swit];
       for (x=0;x<DIM;x++) src.v[x].p[swit]  = vectmp.p[x];
       for (x=0;x<DIM;x++) vectmp.p[x]       = dst->v[x].p[i];
       for (x=0;x<DIM;x++) dst->v[x].p[i]    = dst->v[x].p[swit];
       for (x=0;x<DIM;x++) dst->v[x].p[swit] = vectmp.p[x];
       div = src.v[i].p[i];
      }
    for (x=0;x<DIM;x++)
      {
       src.v[x].p[i]  /= div;
       dst->v[x].p[i] /= div;
      }
    for (k=0;k<DIM;k++)
      {
       pom = src.v[i].p[k];
       if (k-i)
         {
          for (x=0;x<DIM;x++) src.v[x].p[k]  -= pom* src.v[x].p[i];
          for (x=0;x<DIM;x++) dst->v[x].p[k] -= pom* dst->v[x].p[i];
         }
      }
   }
}

		/* invert matrix using rotation special features
		 * [a11 a12 a13 0]     [a11 a21 a31 0]
		 * [a21 a22 a23 0] ==\ [a12 a22 a32 0]
		 * [a31 a32 a33 0] ==/ [a13 a23 a33 0]
		 * [0   0   0   1]     [0   0   0   1]
		 */

void invert_rot_matrix(struct Matrix4D* dst, struct Matrix4D* src)
{
 register int j,i;
 TRACE
 copy_matrix(dst, src);
 for (i=0;i<DIM-1;i++)
 for (j=0;j<DIM-1;j++)
 if (i!=j) dst->v[i].p[j] = src->v[j].p[i];
}

		/* invert matrix using world(rot+trans) special features
		 *  rotations-R,  translations-T, world is W=R*T, here W^(-1) ==>
		 * [a11 a12 a13 0]  [1 0 0 tx]     [a11 a21 a31 -tx]
		 * [a21 a22 a23 0]  [0 1 0 ty] ==\ [a12 a22 a32 -ty]
		 * [a31 a32 a33 0]  [0 0 1 tz] ==/ [a13 a23 a33 -tz]
		 * [0   0   0   1], [0 0 0 1 ]     [0   0   0   1  ]
		 */

void inverted_world_matrix(struct Matrix4D* dst, struct Matrix4D* Mv, struct Matrix4D* transM)
{
 register int j,i;
 TRACE
 copy_3d_matrix(dst, Mv);
 for (i=0;i<DIM-1;i++)
 for (j=0;j<DIM-1;j++)
 if (i!=j) dst->v[i].p[j] = Mv->v[j].p[i];
 dst->v[3].p[0] = -1.0*transM->v[3].p[0];
 dst->v[3].p[1] = -1.0*transM->v[3].p[1];
 dst->v[3].p[2] = -1.0*transM->v[3].p[2];
 dst->v[3].p[3] = 1.0;
}

		/* transpones matrix dst = srcT */

void transp_matrix(struct Matrix4D* dst, struct Matrix4D* src)
{
 register int j,i;
 TRACE
 copy_matrix(dst, src);
 for (i=0;i<DIM;i++)
 for (j=0;j<DIM;j++)
 if (i!=j) dst->v[i].p[j] = src->v[j].p[i];
}

		/* finds the nearest point of ellipse to the observer far away on -Z */
		/* uses W=Mv^(-1)T*Ed*Mv^(-1) matrix used as yTWy=0 (*) */
		/* we have b^2-4ac, if there are solutions then nearer is choosen and comp is set to 1 */
		/* else comp is set to 0 and result is not important */
		/* computes z for given x and y */
		/* parameters a,b,c are computed from (*) */

double double_product_z(struct Vector4D* vec, struct Matrix4D* m, double x, double y, int* comp)
{
 double a,b,c,d;
 TRACE
 a = m->v[2].p[2];
 b = m->v[0].p[2]*x+m->v[1].p[2]*y+m->v[2].p[0]*x+m->v[2].p[1]*y+m->v[2].p[3]+m->v[3].p[2];
 c = m->v[0].p[0]*x*x+m->v[0].p[1]*x*y+m->v[0].p[3]*x+m->v[1].p[0]*x*y+m->v[1].p[1]*y*y+m->v[1].p[3]*y+m->v[3].p[0]*x+m->v[3].p[1]*y+m->v[3].p[3];
 d = b*b-4*a*c;
 if (d<0.0)
   {
    *comp = 0;
    return 0.0;
   }
 *comp = 1;
 return (-b-sqrt(d))/(2*a);
}

		/* normalize vector length using its first 3 parameters */

void normalize_vector_3d(struct Vector4D* vec)
{
 double len;
 register int i;
 TRACE
 len = sqrt(vec->p[0]*vec->p[0]+vec->p[1]*vec->p[1]+vec->p[2]*vec->p[2]);
 if (len) for (i=0;i<DIM-1;i++) vec->p[i] /= len;
 else
   {
    for (i=0;i<DIM-1;i++) vec->p[i] = 0.0;
    vec->p[DIM-1] = 1.0;
    /* vec->w = 1.; */
    /* FIXME shouldn't we normalize weight here to 1.0 ?? */
    /* better not because some procedures normalizes vectors */
    /* and we don't want them to normalize weight too */
   }
}

		/* compute cos_light from observer on (0,0,-1) to ellipsoid on (x,y,z) */
		/* using known ellipsoid gradients for ellipsoid diag(a,b,c,-1) */

double ellipsoid_light_normal_from_z(double x,double y,double z, double a,double b,double c)
{
 struct Vector4D vec;
 struct Vector4D obsvec;
 TRACE
 construct_vector(&vec,x*a,y*b,z*c, 0.0);
 normalize_vector_3d(&vec);
 construct_vector(&obsvec, 0.0, 0.0, -1.0, 0.0);
 return vector_product(&vec, &obsvec);
}

		/* compute cos_light from observer on (xl,yl,zl) to ellipsoid on (x,y,z) */
		/* using known ellipsoid gradients for ellipsoid diag(a,b,c,-1) */

double ellipsoid_light(double x,double y,double z, double a,double b,double c, double xl, double yl, double zl)
{
 struct Vector4D vec;
 struct Vector4D obsvec;
 TRACE
 construct_vector(&vec,x*a,y*b,z*c, 0.0);
 normalize_vector_3d(&vec);
 construct_vector(&obsvec, xl, yl, zl, 0.0);
 normalize_vector_3d(&obsvec);
 return vector_product(&vec, &obsvec);
}

		/* construct default 3d cursor expanded each dimnesion to siz */

void construct_cursor4d_default(struct Cursor4D* cur, double siz)
{
 register int i,j,k;
 TRACE
 for (i=0;i<DIM-1;i++)
 for (j=0;j<2;j++)
 for (k=0;k<DIM-1;k++) cur->l[i].v[j].p[k] = 0.0;
 for (i=0;i<DIM-1;i++)
 for (j=0;j<2;j++) { cur->l[i].v[j].p[3] = 1.0; cur->l[i].v[j].w = 1.0; }
 for (i=0;i<DIM-1;i++)
   {
    cur->l[i].v[0].p[i] = -siz;
    cur->l[i].v[1].p[i] = siz;
   }
}


void world_translate(struct Matrix4D* m, double x, double y, double z)
{
 TRACE
 m->v[0].p[3] += x;
 m->v[1].p[3] += y;
 m->v[2].p[3] += z;
}


void world_scale(struct Matrix4D* m, double sx, double sy, double sz)
{
 TRACE
 m->v[0].p[0] *= sx;
 m->v[1].p[1] *= sy;
 m->v[2].p[2] *= sz;
}


void world_observer_distance(struct Matrix4D* m, double d)
{
 TRACE
 m->v[3].p[2] = 1.0/d;
}


void vector_rotatex(struct Vector4D* vec, double ang)
{
 struct Matrix4D m;
 struct Vector4D temp;
 TRACE
 identity_matrix(&m);
 copy_vector(&temp, vec);
 m.v[1].p[1] = cos(ang);
 m.v[1].p[2] = sin(ang);
 m.v[2].p[1] = -sin(ang);
 m.v[2].p[2] = cos(ang);
 vector_mul_matrix(vec, &temp, &m);
}


void vector_rotatey(struct Vector4D* vec, double ang)
{
 struct Matrix4D m;
 struct Vector4D temp;
 TRACE
 identity_matrix(&m);
 copy_vector(&temp, vec);
 m.v[0].p[0] = cos(ang);
 m.v[0].p[2] = -sin(ang);
 m.v[2].p[0] = sin(ang);
 m.v[2].p[2] = cos(ang);
 vector_mul_matrix(vec, &temp, &m);
}


void vector_rotatez(struct Vector4D* vec, double ang)
{
 struct Matrix4D m;
 struct Vector4D temp;
 TRACE
 identity_matrix(&m);
 copy_vector(&temp, vec);
 m.v[0].p[0] = cos(ang);
 m.v[0].p[1] = sin(ang);
 m.v[1].p[0] = -sin(ang);
 m.v[1].p[1] = cos(ang);
 vector_mul_matrix(vec, &temp, &m);
}


void world_rotate(struct Matrix4D* m, int dir, double ang)
{
 TRACE
 if (dir==0)
   {
    m->v[1].p[1] = cos(ang);
    m->v[1].p[2] = sin(ang);
    m->v[2].p[1] = -sin(ang);
    m->v[2].p[2] = cos(ang);
    return;
   }
 if (dir==1)
   {
    m->v[0].p[0] = cos(ang);
    m->v[0].p[2] = -sin(ang);
    m->v[2].p[0] = sin(ang);
    m->v[2].p[2] = cos(ang);
    return;
   }
 m->v[0].p[0] = cos(ang);
 m->v[0].p[1] = sin(ang);
 m->v[1].p[0] = -sin(ang);
 m->v[1].p[1] = cos(ang);
}


void vector_translatev(struct Vector4D* v, struct Vector4D* d)
{
 register int i;
 TRACE
 for (i=0;i<DIM-1;i++) v->p[i] += d->p[i];
}


void vector_translate(struct Vector4D* v, double dx, double dy, double dz)
{
 TRACE
 v->p[0] += dx;
 v->p[1] += dy;
 v->p[2] += dz;
}


void point_translatev(struct Polygon4D* point, struct Vector4D* d)
{
 TRACE
 vector_translatev(&point->p, d);
}


void point_translate(struct Polygon4D* point, double dx, double dy, double dz)
{
 TRACE
 vector_translate(&point->p, dx, dy, dz);
}


void cursor_translatev(struct Cursor4D* cursor, struct Vector4D* d)
{
 register int i,j;
 TRACE
 for (i=0;i<DIM-1;i++)
 for (j=0;j<2;j++) vector_translatev(&cursor->l[i].v[j], d);
}


void cursor_translate(struct Cursor4D* cursor, double dx, double dy, double dz)
{
 register int i,j;
 TRACE
 for (i=0;i<DIM-1;i++)
 for (j=0;j<2;j++) vector_translate(&cursor->l[i].v[j], dx, dy, dz);
}


void point_translatemv(struct Polygon4D* point, struct Matrix4D* invWorld, struct Vector4D* v)
{
 struct Vector4D temp;
 TRACE
 if (!invWorld) return;
 copy_vector(&temp, v);
 vector_mul_matrix(&temp, v, invWorld);
 point_translatev(point, &temp);
}


void point_translatem(struct Polygon4D* point, struct Matrix4D* invWorld, double dx, double dy, double dz)
{
 struct Vector4D d, temp;
 TRACE
 if (!invWorld) return;
 construct_vector(&d, dx, dy, dz, 1.0);
 copy_vector(&temp, &d);
 vector_mul_matrix(&d, &temp, invWorld);
 point_translatev(point, &d);
}


void point_translatevm(struct Vector4D* point, struct Matrix4D* invWorld, double dx, double dy, double dz)
{
 struct Vector4D d, temp;
 TRACE
 if (!invWorld) return;
 construct_vector(&d, dx, dy, dz, 1.0);
 copy_vector(&temp, &d);
 vector_mul_matrix(&d, &temp, invWorld);
 vector_translatev(point, &d);
}


void point_translateam(struct Vector4D** pt, int i, int j, struct Matrix4D* invWorld, double dx, double dy, double dz)
{
 struct Vector4D d, temp;
 TRACE
/* printf_vector("vec", &pt[i][j]);*/
 if (!pt) return;
 if (!invWorld) return;
 construct_vector(&d, dx, dy, dz, 1.0);
 copy_vector(&temp, &d);
 vector_mul_matrix(&d, &temp, invWorld);
 vector_translatev(&pt[i][j], &d);
}


void cursor_translatemv(struct Cursor4D* cursor, struct Matrix4D* invWorld, struct Vector4D* v)
{
 struct Vector4D temp;
 TRACE
 if (!invWorld) return;
 copy_vector(&temp, v);
 vector_mul_matrix(&temp, v, invWorld);
/* matrix_mul_vector(&temp, invWorld, v);*/
 cursor_translatev(cursor, &temp);
}


void cursor_translatem(struct Cursor4D* cursor, struct Matrix4D* invWorld, double dx, double dy, double dz)
{
 struct Vector4D d, temp;
 TRACE
 if (!invWorld) return;
 construct_vector(&d, dx, dy, dz, 1.0);
 copy_vector(&temp, &d);
 vector_mul_matrix(&d, &temp, invWorld);	/* FIXED should work if translations are in matrix but it don't */
 						/* but this is probably OK, because if world is translated */
 						/* and we're moving only then translation to MOVE vector is NOT */
 						/* needed, because we only want to know direction */
 						/* so invertedMv should be first tratened by clear_translations() */
 /*matrix_mul_vector(&d, invWorld, &temp);*/
 cursor_translatev(cursor, &d);
}


void get_cursor_posv(struct Cursor4D* cursor, struct Vector4D* v)
{
 TRACE
 v->p[0] = cursor->l[1].v[0].p[0];
 v->p[1] = cursor->l[0].v[0].p[1];
 v->p[2] = cursor->l[0].v[0].p[2];
 v->p[3] = 1.0;
 v->w    = 1.0;
}


void get_cursor_pos(struct Cursor4D* cursor, double *x, double *y, double *z)
{
 TRACE
 *x = cursor->l[1].v[0].p[0];
 *y = cursor->l[0].v[0].p[1];
 *z = cursor->l[0].v[0].p[2];
}


void negate_vector(struct Vector4D* v)
{
 register int i;
 TRACE
 for (i=0;i<DIM-1;i++) v->p[i] *= -1.0;
}


void clear_cursor(struct Cursor4D* cursor)
{
 struct Vector4D curvec;
 TRACE
 get_cursor_posv(cursor, &curvec);
 negate_vector(&curvec);
 cursor_translatev(cursor, &curvec);
}


void clear_translations(struct Matrix4D* m)
{
 register int i;
 TRACE
 for (i=0;i<DIM;i++) m->v[i].p[DIM-1] = 0.0;
}

struct VList* get_vlist_tail(struct VList* head)
{
 struct VList* temp;
 TRACE
 temp = head;
 while (head) { temp = head; head = head->next; }
 return temp;
}

struct VList* get_vlist_head(struct VList* tail)
{
 struct VList* temp;
 TRACE
 temp = tail;
 while (tail) { temp = tail; tail = tail->prev; }
 return temp;
}


struct Polygon4D* get_list_tail(struct Polygon4D* head)
{
 struct Polygon4D* temp;
 TRACE
 temp = head;
 while (head) { temp = head; head = head->next; }
 return temp;
}


struct Polygon4D* get_list_head(struct Polygon4D* tail)
{
 struct Polygon4D* temp;
 TRACE
 temp = tail;
 while (tail) { temp = tail; tail = tail->prev; }
 return temp;
}


void printf_list(char* name, struct Polygon4D* head)
{
 struct Polygon4D* t;
 TRACE
 printf(" *** list: %s ***\n", name);
 t = head;
 while (t)
   {
    printf("%10p %10p %10p (%3.3f, %3.3f, %3.3f, %3.3f, %3.3f)\n", (void*)t->prev, (void*)t, (void*)t->next, t->p.p[0], t->p.p[1], t->p.p[2], t->p.p[3], t->p.w);
    t = t->next;
   }
}


void list_add_pointp(struct Polygon4D** head, struct Polygon4D* pt)
{
 TRACE
 list_add_point(head, pt->p.p[0], pt->p.p[1], pt->p.p[2]);
}


void list_add_pointv(struct Polygon4D** head, struct Vector4D* v)
{
 TRACE
/* printf("-- %f, %f, %f, %f, %f --\n",v->p[0], v->p[1], v->p[2], v->p[3], v->w);*/
 list_add_point5(head, v->p[0], v->p[1], v->p[2], v->p[3], v->w);
}


void list_add_point(struct Polygon4D** head, double px, double py, double pz)
{
 struct Polygon4D* temp;
 TRACE
 if (*head == (struct Polygon4D*)(0))
   {
    *head = (struct Polygon4D*)(malloc(sizeof(struct Polygon4D)));
    if (!(*head)) { printf("malloc problem! list_add_point\n"); exit(1); }
    (*head)->p.p[0] = px;
    (*head)->p.p[1] = py;
    (*head)->p.p[2] = pz;
    (*head)->p.p[3] = 1.0;
    (*head)->p.w    = 1.0;
    (*head)->next = (struct Polygon4D*)(0);
    (*head)->prev = (struct Polygon4D*)(0);
/*    printf_list("list", *head);*/
    return;
   }
  temp = (struct Polygon4D*)(malloc(sizeof(struct Polygon4D)));
  if (!temp) { printf("malloc problem! list_add_point\n"); exit(1); }
  temp->p.p[0] = px;
  temp->p.p[1] = py;
  temp->p.p[2] = pz;
  temp->p.p[3] = 1.0;
  temp->p.w    = 1.0;
  temp->next = *head;
  temp->prev = NULL;
  (*head)->prev = temp;
  *head = temp;
/*  printf_list("list", *head);*/
}

void vlist_add(struct VList** head, void* data)
{
 struct VList* temp;
 TRACE
 if (*head == NULL)
   {
    *head = (struct VList*)(malloc(sizeof(struct VList)));
    if (!(*head)) { printf("malloc problem! vlist_add\n"); exit(1); }
    (*head)->data = data;
    (*head)->next = NULL;
    (*head)->prev = NULL;
    return;
   }
  temp = (struct VList*)(malloc(sizeof(struct VList)));
  if (!temp) { printf("malloc problem! vlist_add\n"); exit(1); }
  temp->data = data;
  temp->next = *head;
  temp->prev = NULL;
  (*head)->prev = temp;
  *head = temp;
}


void list_add_point5(struct Polygon4D** head, double px, double py, double pz, double pv, double pw)
{
 struct Polygon4D* temp;
 TRACE
 if (*head == (struct Polygon4D*)(0))
   {
    *head = (struct Polygon4D*)(malloc(sizeof(struct Polygon4D)));
    if (!(*head)) { printf("malloc problem! list_add_point\n"); exit(1); }
    (*head)->p.p[0] = px;
    (*head)->p.p[1] = py;
    (*head)->p.p[2] = pz;
    (*head)->p.p[3] = pv;
    (*head)->p.w    = pw;
    (*head)->next = (struct Polygon4D*)(0);
    (*head)->prev = (struct Polygon4D*)(0);
/*    printf_list("list", *head);*/
    return;
   }
  temp = (struct Polygon4D*)(malloc(sizeof(struct Polygon4D)));
  if (!temp) { printf("malloc problem! list_add_point\n"); exit(1); }
  temp->p.p[0] = px;
  temp->p.p[1] = py;
  temp->p.p[2] = pz;
  temp->p.p[3] = pv;
  temp->p.w    = pw;
  temp->next = *head;
  temp->prev = NULL;
  (*head)->prev = temp;
  *head = temp;
/*  printf_list("list", *head);*/
}


void list_add_point_nearest(struct Polygon4D** head, struct Polygon4D* nearest, double px, double py, double pz)
{
 struct Polygon4D* temp;
 TRACE
 if (*head == (struct Polygon4D*)(0))
   {
    *head = (struct Polygon4D*)(malloc(sizeof(struct Polygon4D)));
    if (!(*head)) { printf("malloc problem! list_add_point\n"); exit(1); }
    (*head)->p.p[0] = px;
    (*head)->p.p[1] = py;
    (*head)->p.p[2] = pz;
    (*head)->p.p[3] = 1.0;
    (*head)->p.w    = 1.0;
    (*head)->next = (struct Polygon4D*)(0);
    (*head)->prev = (struct Polygon4D*)(0);
    return;
   }
  temp = (struct Polygon4D*)(malloc(sizeof(struct Polygon4D)));
  if (!temp) { printf("malloc problem! list_add_point\n"); exit(1); }
  temp->p.p[0] = px;
  temp->p.p[1] = py;
  temp->p.p[2] = pz;
  temp->p.p[3] = 1.0;
  temp->p.w    = 1.0;
  if (nearest)
    {
     if (nearest->prev)
       {
        nearest->prev->next = temp;
	temp->prev = nearest->prev;
       }
     else
       {
	temp->prev = NULL;
	(*head) = temp;
       }
     nearest->prev = temp;
     temp->next = nearest;
    }
  else
  {
   printf("Error: head exist, no nearest element found!\n");
   exit(1);
  }
}


void list_add_pointc(struct Polygon4D** head, struct Cursor4D* cursor)
{
 double x,y,z;
 TRACE
 get_cursor_pos(cursor, &x, &y, &z);
 list_add_point(head, x, y, z);
}


void list_add_pointc_nearest(struct Polygon4D** head, struct Cursor4D* cursor)
{
 double x,y,z;
 struct Polygon4D* list;
 TRACE
 get_cursor_pos(cursor, &x, &y, &z);
 list = list_get_point_nearest(head, cursor);
 list_add_point_nearest(head, list, x, y, z);
}


double distance_to_cursor_fast(struct Cursor4D* cursor, struct Polygon4D* point)
{
 double x,y,z;
 TRACE
 get_cursor_pos(cursor, &x, &y, &z);
 return distance_to_cursord_fast(point, x, y, z);
}


double distance_to_cursord_fast(struct Polygon4D* point, double x, double y, double z)
{
 double sum;
 TRACE
 sum =  (point->p.p[0]-x)*(point->p.p[0]-x);
 sum += (point->p.p[1]-y)*(point->p.p[1]-y);
 sum += (point->p.p[2]-z)*(point->p.p[2]-z);
 return sum;
}


double distance_to_cursordv_fast(struct Vector4D* point, double x, double y, double z)
{
 double sum;
 TRACE
 sum =  (point->p[0]-x)*(point->p[0]-x);
 sum += (point->p[1]-y)*(point->p[1]-y);
 sum += (point->p[2]-z)*(point->p[2]-z);
 return sum;
}


double distance_to_cursor(struct Cursor4D* cursor, struct Polygon4D* point)
{
 double x,y,z;
 TRACE
 get_cursor_pos(cursor, &x, &y, &z);
 return distance_to_cursord(point, x, y, z);
}


double distance_to_cursord(struct Polygon4D* point, double x, double y, double z)
{
 double sum;
 TRACE
 sum =  (point->p.p[0]-x)*(point->p.p[0]-x);
 sum += (point->p.p[1]-y)*(point->p.p[1]-y);
 sum += (point->p.p[2]-z)*(point->p.p[2]-z);
 return sqrt(sum);
}


struct Polygon4D* list_get_point_nearest(struct Polygon4D** head, struct Cursor4D* cursor)
{
 double x,y,z,min,dis;
 struct Polygon4D* temp;
 struct Polygon4D* minptr;
 TRACE
 if ((*head) == (struct Polygon4D*)(0)) return (struct Polygon4D*)(0);
 minptr = (struct Polygon4D*)(0);
 get_cursor_pos(cursor, &x, &y, &z);
 temp = *head;
 min = 19e19;
 while (temp)
   {
    dis = distance_to_cursord_fast(temp, x, y, z);
    if (dis < min)
      {
       min = dis;
       minptr = temp;
      }
    temp = temp->next;
   }
 return minptr;
}


void list_delete_point_nearest(struct Polygon4D** head, struct Cursor4D* cursor)
{
 struct Polygon4D* temp;
 struct Polygon4D* minptr;
 TRACE
 minptr = list_get_point_nearest(head, cursor);
 if (minptr == (struct Polygon4D*)(0)) return;
 temp = *head;
 if (minptr == *head)
   {
    temp = *head;
    *head = temp->next;
    if (*head) (*head)->prev = NULL;
    temp->prev = temp->next = NULL;
    free(temp);
    return;
   }
 if (minptr == (struct Polygon4D*)(0)) return;
 while (temp)
   {
    if (temp == minptr)
      {
       if (temp->prev) temp->prev->next = temp->next;
       if (temp->next) temp->next->prev = temp->prev;
       temp->next = temp->prev = NULL;
       free(temp);
       return;
      }
    temp = temp->next;
   }
}


int list_save_to_file(struct Polygon4D* head, char* fn)
{
 FILE* fh;
 struct Polygon4D* temp;
 TRACE
 if (!fn) return 0;
 fh = fopen(fn, "w");
 if (!fh) return 0;
 temp = head;
 while (temp)
   {
    fprintf(fh, "%f,%f,%f,%f,%f\n", temp->p.p[0], temp->p.p[1], temp->p.p[2], temp->p.p[3],temp->p.w);
    temp = temp->next;
   }
 fclose(fh);
 return 1;
}


int list_tail_save_to_file(struct Polygon4D* tail, char* fn)
{
 FILE* fh;
 struct Polygon4D* temp;
 if (!fn) return 0;
 TRACE
 fh = fopen(fn, "w");
 if (!fh) return 0;
 temp = tail;
 while (temp)
   {
    fprintf(fh, "%f,%f,%f,%f,%f\n", temp->p.p[0], temp->p.p[1], temp->p.p[2], temp->p.p[3], temp->p.w);
    temp = temp->prev;
   }
 fclose(fh);
 return 1;
}


int list_load_from_file(struct Polygon4D** head, char* fn)
{
 FILE* fh;
 double x,y,z,v,w;
 int nread;
 TRACE
 if (!fn) return 0;
 fh = fopen(fn, "r");
 if (!fh) return 0;
 do
   {
    nread = fscanf(fh, "%lf,%lf,%lf,%lf,%lf\n", &x, &y, &z, &v, &w);
    list_add_point5(head, x, y, z, v, w);
   }
 while (nread == 5);
 fclose(fh);
 return 1;
}


void list_free(struct Polygon4D** head)
{
 struct Polygon4D* temp;
 TRACE
 while (*head)
   {
    temp = *head;
    *head = (*head)->next;
    free(temp);
   }
 *head = NULL;
}


void line_cursor_to_point(struct Line4D* line, struct Cursor4D* cursor, struct Polygon4D* point)
{
 TRACE
 get_cursor_posv(cursor, &line->v[0]);
 copy_vector(&line->v[1], &point->p);
}


void line_cursor_to_pointv(struct Line4D* line, struct Cursor4D* cursor, struct Vector4D* point)
{
 TRACE
 get_cursor_posv(cursor, &line->v[0]);
 copy_vector(&line->v[1], point);
}

	/* T[i,j](val), dim = dimnesion  off = offset = M */
double basis_bspline(double* T, double val, int dim, int off, int I, int J)

    /* will compute basis for given parameters */
{
 register double up;
 register double down;
 double factor1;
 double factor2;
 int Ioff, Joff;
 TRACE
 factor1 = factor2 = 0.0;
 Ioff = I+off;
 Joff = J+off;
 /*if (I>J)
   {
     printf("I more than J in BSPLINE basis");
     exit(1);
       return 0.0;
   }*/ /* not checking for speed! */
 if (dim == 0)
   {	/* middle of array (0..0 -> 1..1) */
       /* 0's and 1's added to create nodes beyond ti, for M dimnesion */
    if (val >= T[Ioff] && val <= T[Joff]) return 1.0;
    else return 0.0;
   }
 up = (val-T[Ioff]) * basis_bspline(T, val, dim-1, off, I, J-1);
 down = T[Joff-1] - T[Ioff];
 if (down==0.0) factor1 = 0.0;
 else factor1 = up/down;
 up = (T[Joff]-val) * basis_bspline(T, val, dim-1, off, I+1, J);
 down = T[Joff] - T[Ioff+1];
 if (down==0.0) factor2 += 0.0;
 else factor2 = up/down;
 return factor1+factor2;
}

			/* point_idx can be 0,1,2 means compute: X,Y,Z  */

double bspline_compute(struct BSpline* bspline, double t, int K, int point_xyz)
{
 int i;
 double up,down;
 double factor;
 int N,M;
 double b;
 TRACE
 N = bspline->N;	/* must be computed in spline */
 M = bspline->M;	/* this is probably 4, but algorithm uses general curve */
 up = 0.0;
 down = 0.0;
 for (i=0;i<=K;i++)
   {
    b = basis_bspline(bspline->T, t, M, M, i-M, i+1);		/* T must be computed */
    factor = bspline->W[i]*bspline->P[i].p[point_xyz]*b;	/* weights must be set, control points computed */
    up += factor;
    factor = bspline->W[i]*b;
    down += factor;
   }
 if (down==0.0) return 0.0;
 return up/down;
}


void vector_mul(struct Vector4D* v, struct Vector4D* a, struct Vector4D* b)
{
 TRACE
 if (!a || !b || !v) return;
 construct_vector3(v, a->p[1]*b->p[2]-a->p[2]*b->p[1], a->p[2]*b->p[0]-a->p[0]*b->p[2], a->p[0]*b->p[1]-a->p[1]*b->p[0]);
}


void nurb_computen(struct Nurb* nurb, double s, double t, int K1, int K2, struct Vector4D* n)
{
 int i,j,d;
 double up,down;
 double factor;
 int M1,M2,N1,N2;
 double b1,b2;
 struct Vector4D nx, ny;
 TRACE
 if (!n) return;
 M1 = nurb->M1;
 M2 = nurb->M2;
 N1 = nurb->N1;
 N2 = nurb->N2;
 n->p[3] = n->w = 1.;
 for (d=0;d<3;d++)
 {
  up = 0.0;
  down = 0.0;
  for (i=0;i<K1;i++)
   {
    for (j=0;j<K2;j++)
      {
       b1 = basis_bspline(nurb->S, s, M1-1, M1-1, i-M1, i+1);
       b2 = basis_bspline(nurb->T, t, M2-1, M2-1, j-M2, j+1);
       factor = (nurb->P[i+1][j].p[d]-nurb->P[i][j].p[d])*b1*b2;
       up += factor;
       factor = nurb->W[i][j]*b1*b2;
       down += factor;
      }
   }
  if (down==0.0) nx.p[d] = 0.;
  else nx.p[d] = up/down;
  up = 0.0;
  down = 0.0;
  for (i=0;i<K1;i++)
   {
    for (j=0;j<K2;j++)
      {
       b1 = basis_bspline(nurb->S, s, M1-1, M1-1, i-M1, i+1);
       b2 = basis_bspline(nurb->T, t, M2-1, M2-1, j-M2, j+1);
       factor = (nurb->P[i][j+1].p[d]-nurb->P[i][j].p[d])*b1*b2;
       up += factor;
       factor = nurb->W[i][j]*b1*b2;
       down += factor;
      }
   }
  if (down==0.0) ny.p[d] = 0.;
  else ny.p[d] = up/down;
 }
 vector_mul(n, &nx, &ny);
 normalize_vector_3d(n);
}


double nurb_compute(struct Nurb* nurb, double s, double t, int K1, int K2, int point_xyz)
{
 int i,j;
 double up,down;
 double factor;
 int M1,M2,N1,N2;
 double b1,b2;
 TRACE
 M1 = nurb->M1;
 M2 = nurb->M2;
 N1 = nurb->N1;
 N2 = nurb->N2;
 up = 0.0;
 down = 0.0;
 for (i=0;i<=K1;i++)
 {
 for (j=0;j<=K2;j++)
   {
    b1 = basis_bspline(nurb->S, s, M1, M1, i-M1, i+1);
    b2 = basis_bspline(nurb->T, t, M2, M2, j-M2, j+1);
    factor = nurb->W[i][j]*nurb->P[i][j].p[point_xyz]*b1*b2;
    up += factor;
    factor = nurb->W[i][j]*b1*b2;
    down += factor;
   }
 }
 if (down==0.0) return 0.0;
 return up/down;
}


void nurb_free(struct Nurb* nurb)
{
 int i;
 TRACE
 if (!nurb) return;
 free(nurb->S);
 free(nurb->T);
 nurb->S = 0;
 nurb->T = 0;
 for (i=0;i<=nurb->K1;i++) free(nurb->W[i]);
 free(nurb->W);
 nurb->W = 0;
 for (i=0;i<=nurb->K1;i++) free(nurb->P[i]);
 free(nurb->P);
 nurb->P = 0;
 for (i=0;i<=nurb->npointsx;i++) free(nurb->points[i]);
 free(nurb->points);
 nurb->points = 0;
 if (nurb->normals)
   {
    for (i=0;i<=nurb->npointsx;i++) free(nurb->normals[i]);
    free(nurb->normals);
    nurb->normals = 0;
   }
}


void nurb_setn(struct Nurb* nurb, int K1, int M1, int K2, int M2, int intervalx, int intervaly, struct Vector4D* points)
{
 register double xm,ym;
 register int N1, N2, A, B, C, i, j, U0, U1, V0, V1, x, y;
 double tstep, curt, stepx, stepy, t1, t2, t3;
 TRACE
 printf("warning: nurb_setn(): currently normal computing isn't checked. probably don't working\n");
 N1 = K1-M1+1;
 N2 = K2-M2+1;
 A = N2+2*M1;
 B = N1+2*M2;
 C = (K1+1)*(K2+1);
 nurb->M1  = M1;
 nurb->K1  = K1;
 nurb->N1  = N1;
 nurb->M2  = M2;
 nurb->K2  = K2;
 nurb->N2  = N2;
 nurb->A  = A;
 nurb->B  = B;
 nurb->C  = C;
 nurb->npointsx = intervalx+1;
 nurb->npointsy = intervaly+1;
 nurb->S = (double*)malloc((A+1)*sizeof(double));
 nurb->T = (double*)malloc((B+1)*sizeof(double));
 if (N1>1) tstep = 1.0/((double)N1);
 else      tstep = 1.0;
 curt = 0.0;
 for (i=0;i<=A;i++)
   {
     if (i<=M1) nurb->S[i] = 0.0;
     else if (i>=A-M1) nurb->S[i] = 1.0;
     else
       {
        curt += tstep;
	if (curt > 1.0) curt = 1.0;
	nurb->S[i] = curt;
       }
/*     printf("S[%d] = %f\n", i, nurb->S[i]);*/
   }
 if (N2>1) tstep = 1.0/((double)N2);
 else      tstep = 1.0;
 curt = 0.0;
 for (i=0;i<=B;i++)
   {
     if (i<=M2) nurb->T[i] = 0.0;
     else if (i>=B-M2) nurb->T[i] = 1.0;
     else
       {
        curt += tstep;
	if (curt > 1.0) curt = 1.0;
	nurb->T[i] = curt;
       }
/*     printf("T[%d] = %f\n", i, nurb->T[i]);*/
   }
 nurb->W = (double**)malloc((K1+1)*sizeof(double*));
 for (i=0;i<=K1;i++)
   {
    nurb->W[i] = (double*)malloc((K2+1)*sizeof(double));
/*    for (j=0;j<=K2;j++) nurb->W[i][j] = 1.;*/
    for (j=0;j<=K2;j++)
      {
	nurb->W[i][j] = points[(K2+1)*i+j].w;
/*	printf("%f ", nurb->W[i][j]);*/
      }
/*    printf("\n");*/
   }
/* printf("\n");*/
 nurb->P = (struct Vector4D**)malloc((K1+1)*sizeof(struct Vector4D*));
 for (i=0;i<=K1;i++)
   {
    nurb->P[i] = (struct Vector4D*)malloc((K2+1)*sizeof(struct Vector4D));
    for (j=0;j<=K2;j++)
      {
       copy_vector(&nurb->P[i][j], &points[(K2+1)*i+j]);
/*       printf("(%f %f %f) ", nurb->P[i][j].p[0], nurb->P[i][j].p[1], nurb->P[i][j].p[2]);*/
      }
/*    printf("\n");*/
   }
 nurb->U0 = U0 = 0.0;
 nurb->U1 = U1 = 1.0;
 nurb->V0 = V0 = 0.0;
 nurb->V1 = V1 = 1.0;
 nurb->points = (struct Vector4D**)malloc((nurb->npointsx+1)*sizeof(struct Vector4D*));
 for (i=0;i<=nurb->npointsx;i++) nurb->points[i] = (struct Vector4D*)malloc((nurb->npointsy+1)*sizeof(struct Vector4D));
 nurb->normals = (struct Vector4D**)malloc((nurb->npointsx+1)*sizeof(struct Vector4D*));
 for (i=0;i<=nurb->npointsx;i++) nurb->normals[i] = (struct Vector4D*)malloc((nurb->npointsy+1)*sizeof(struct Vector4D));
 x = 0;
 y = 0;
 stepx = (U1-U0)/(double)intervalx;
 stepy = (U1-U0)/(double)intervaly;
/* printf("step = %f\n", step);*/
 for (xm=U0;xm<=U1;xm+=stepx)
 {
  y = 0;
  for (ym=V0;ym<=V1;ym+=stepy)
   {
     t1 = nurb_compute(nurb, xm, ym, K1, K2, 0);
     t2 = nurb_compute(nurb, xm, ym, K1, K2, 1);
     t3 = nurb_compute(nurb, xm, ym, K1, K2, 2);
/*     printf("XYZ> %f,%f, %f\n",t1, t2, t3);*/
     nurb->points[x][y].p[0]  = t1;
     nurb->points[x][y].p[1]  = t2;
     nurb->points[x][y].p[2]  = t3;
     nurb->points[x][y].p[3]  = 1.0;
     nurb->points[x][y].w     = 1.0;
     nurb_computen(nurb, xm, ym, K1, K2, &nurb->normals[x][y]);
     y++;
    }
   x++;
  }
 nurb->npointsx = x-1;
 nurb->npointsy = y-1;
/* printf("%d %d\n", nurb->npointsx, nurb->npointsy);*/
}

/*t1 = nurb_compute(nurb, xm, ym, K1, K2, 0);*/
void nurb_computev(struct Nurb* nurb, struct Vector4D* vec, double xm, double ym)
{
 int i;
 for (i=0;i<DIM-1;i++) vec->p[i] = nurb_compute(nurb, xm, ym, nurb->K1, nurb->K2, i); 
 vec->p[DIM-1] = 1.;
}

void nurb_set(struct Nurb* nurb, int K1, int M1, int K2, int M2, int intervalx, int intervaly, struct Vector4D* points)
{
 register double xm,ym;
 register int N1, N2, A, B, C, i, j, U0, U1, V0, V1, x, y;
 double tstep, curt, stepx, stepy, t1, t2, t3;
 TRACE
 N1 = K1-M1+1;
 N2 = K2-M2+1;
 A = N2+2*M1;
 B = N1+2*M2;
 C = (K1+1)*(K2+1);
/* printf("K1=%d, K2=%d, N1=%d, N2=%d, M1=%d, M2=%d\n",K1,K2,N1,N2,M1,M2);*/
 nurb->M1  = M1;
 nurb->K1  = K1;
 nurb->N1  = N1;
 nurb->M2  = M2;
 nurb->K2  = K2;
 nurb->N2  = N2;
 nurb->A  = A;
 nurb->B  = B;
 nurb->C  = C;
 nurb->npointsx = intervalx+1;
 nurb->npointsy = intervaly+1;
 nurb->S = (double*)malloc((A+1)*sizeof(double));
 nurb->T = (double*)malloc((B+1)*sizeof(double));
 if (N1>1) tstep = 1.0/((double)N1);
 else      tstep = 1.0;
 curt = 0.0;
 for (i=0;i<=A;i++)
   {
     if (i<=M1) nurb->S[i] = 0.0;
     else if (i>=A-M1) nurb->S[i] = 1.0;
     else
       {
        curt += tstep;
	if (curt > 1.0) curt = 1.0;
	nurb->S[i] = curt;
       }
/*     printf("S[%d] = %f\n", i, nurb->S[i]);*/
   }
 if (N2>1) tstep = 1.0/((double)N2);
 else      tstep = 1.0;
 curt = 0.0;
 for (i=0;i<=B;i++)
   {
     if (i<=M2) nurb->T[i] = 0.0;
     else if (i>=B-M2) nurb->T[i] = 1.0;
     else
       {
        curt += tstep;
	if (curt > 1.0) curt = 1.0;
	nurb->T[i] = curt;
       }
/*     printf("T[%d] = %f\n", i, nurb->T[i]);*/
   }
 nurb->W = (double**)malloc((K1+1)*sizeof(double*));
 for (i=0;i<=K1;i++)
   {
    nurb->W[i] = (double*)malloc((K2+1)*sizeof(double));
/*    for (j=0;j<=K2;j++) nurb->W[i][j] = 1.;*/
    for (j=0;j<=K2;j++)
      {
	nurb->W[i][j] = points[(K2+1)*i+j].w;
/*	printf("%f ", nurb->W[i][j]);*/
      }
/*    printf("\n");*/
   }
/* printf("\n");*/
 nurb->P = (struct Vector4D**)malloc((K1+1)*sizeof(struct Vector4D*));
 for (i=0;i<=K1;i++)
   {
    nurb->P[i] = (struct Vector4D*)malloc((K2+1)*sizeof(struct Vector4D));
    for (j=0;j<=K2;j++)
      {
       copy_vector(&nurb->P[i][j], &points[(K2+1)*i+j]);
/*       printf("(%f %f %f) ", nurb->P[i][j].p[0], nurb->P[i][j].p[1], nurb->P[i][j].p[2]);*/
      }
/*    printf("\n");*/
   }
 nurb->U0 = U0 = 0.0;
 nurb->U1 = U1 = 1.0;
 nurb->V0 = V0 = 0.0;
 nurb->V1 = V1 = 1.0;
 nurb->points = (struct Vector4D**)malloc((nurb->npointsx+1)*sizeof(struct Vector4D*));
 for (i=0;i<=nurb->npointsx;i++) nurb->points[i] = (struct Vector4D*)malloc((nurb->npointsy+1)*sizeof(struct Vector4D));
 x = 0;
 y = 0;
 stepx = (U1-U0)/(double)intervalx;
 stepy = (U1-U0)/(double)intervaly;
/* printf("step = %f\n", step);*/
 for (xm=U0;xm<=U1;xm+=stepx)
 {
  y = 0;
  for (ym=V0;ym<=V1;ym+=stepy)
   {
     t1 = nurb_compute(nurb, xm, ym, K1, K2, 0);
     t2 = nurb_compute(nurb, xm, ym, K1, K2, 1);
     t3 = nurb_compute(nurb, xm, ym, K1, K2, 2);
/*     printf("XYZ> %f,%f, %f\n",t1, t2, t3);*/
     nurb->points[x][y].p[0] = t1;
     nurb->points[x][y].p[1] = t2;
     nurb->points[x][y].p[2] = t3;
     nurb->points[x][y].p[3] = 1.0;
     nurb->points[x][y].w    = 1.0;
     y++;
    }
   x++;
  }
 nurb->npointsx = x-1;
 nurb->npointsy = y-1;
 nurb->normals = NULL;
/* printf("%d %d\n", nurb->npointsx, nurb->npointsy);*/
}


void bspline_set(struct BSpline* bspline, int K, int M, int interval, struct Vector4D* points)
{
 register int N, A, i, U0, U1, x;
 double tstep, curt, step, t1, t2, t3, xm;
 TRACE
 N = K-M+1;
 A = N+2*M;
/* printf("K=%d, M=%d, N=%d, A=%d\n",K,M,N,A);*/
 bspline->M  = M;
 bspline->K  = K;
 bspline->N  = N;
 bspline->A  = A;
 bspline->npoints = interval+1;
 bspline->T = (double*)malloc((A+1)*sizeof(double));
 if (N>1) tstep = 1.0/((double)N);
 else     tstep = 1.0;
 curt = 0.0;
 for (i=0;i<=A;i++)
   {
     if (i<=M) bspline->T[i] = 0.0;
     else if (i>=A-M) bspline->T[i] = 1.0;
     else	/* values from 0 to 1 */
       {
        curt += tstep;
	if (curt > 1.0) curt = 1.0;
	bspline->T[i] = curt;
       }
/*     printf("T[%d] = %f\n", i, bspline->T[i]);*/
   }
 bspline->W = (double*)malloc((K+1)*sizeof(double));
 for (i=0;i<=K;i++)
   {
/*    bspline->W[i] = 1.0;*/
      bspline->W[i] = points[i].w;
/*    printf("W[%d] = %f\n", i, bspline->W[i]);*/
   }
 bspline->P = (struct Vector4D*)malloc((K+1)*sizeof(struct Vector4D));
 for (i=0;i<=K;i++)
      {
       copy_vector(&bspline->P[i], &points[i]);
/*       print_vector(&bspline->P[i]);*/
      }
 bspline->U0 = U0 = 0.0;
 bspline->U1 = U1 = 1.0;
 bspline->points = (struct Vector4D*)malloc(bspline->npoints*sizeof(struct Vector4D));
 x = 0;
 step = (U1-U0)/(double)interval;
/* printf("step = %f\n", step);*/
 for (xm=U0;xm<=U1;xm+=step)
   {
/*     if (xm>U1-eps) continue;*/
/*     if (xm<U0+eps) continue;*/
     t1 = bspline_compute(bspline, xm, K, 0);
     t2 = bspline_compute(bspline, xm, K, 1);
     t3 = bspline_compute(bspline, xm, K, 2);
/*     printf("XYZ> %f,%f, %f\n",t1, t2, t3);*/
     bspline->points[x].p[0] = t1;
     bspline->points[x].p[1] = t2;
     bspline->points[x].p[2] = t3;
     bspline->points[x].p[3] = 1.0;
     bspline->points[x].w    = 1.0;
     x++;
   }
/* printf("X = %d, npt = %d\n", x, bspline->npoints);*/
 bspline->npoints = x-1;
}


double bezier_compute(int n, int i, double t)
{
 TRACE
 if ( i<0 || i>n) return 0.0;
 if (i==0 && n==0) return 1.0;
 return (1-t)*bezier_compute(n-1, i, t) + t*bezier_compute(n-1, i-1, t);
}


double bcurve_compute(struct BezierCurve* bcurve, double t, int N, int point_xyz)
{
 double sum;
 int i;
 TRACE
 sum = 0.0;
 for (i=0;i<=N;i++) sum += bcurve->P[i].p[point_xyz] * bezier_compute(N, i, t);
 return sum;
}


void bcurve_set(struct BezierCurve* bcurve, int N, int interval, struct Vector4D* points)
{
 register int i, U0, U1, x;
 double tstep, curt, step, t1, t2, t3, xm;
 TRACE
 bcurve->N  = N;
 bcurve->npoints = interval+1;
 if (N>1) tstep = 1.0/((double)N);
 else     tstep = 1.0;
 curt = 0.0;
 bcurve->P = (struct Vector4D*)malloc((N+1)*sizeof(struct Vector4D));
 for (i=0;i<=N;i++)
   {
    copy_vector(&bcurve->P[i], &points[i]);
/*       print_vector(&bcurve->P[i]);*/
   }
 bcurve->U0 = U0 = 0.0;
 bcurve->U1 = U1 = 1.0;
 bcurve->points = (struct Vector4D*)malloc(bcurve->npoints*sizeof(struct Vector4D));
 x = 0;
 step = (U1-U0)/(double)interval;
 for (xm=U0;xm<=U1;xm+=step)
   {
     t1 = bcurve_compute(bcurve, xm, N, 0);
     t2 = bcurve_compute(bcurve, xm, N, 1);
     t3 = bcurve_compute(bcurve, xm, N, 2);
/*     printf("XYZ> %f,%f, %f\n",t1, t2, t3);*/
     bcurve->points[x].p[0] = t1;
     bcurve->points[x].p[1] = t2;
     bcurve->points[x].p[2] = t3;
     bcurve->points[x].p[3] = 1.0;
     bcurve->points[x].w    = 1.0;
     x++;
   }
/* printf("X = %d, npt = %d\n", x, bspline->npoints);*/
 bcurve->npoints = x-1;
}


void list_get_C0_dim_nomulti(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degree)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 register int count,i,j;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < 3) return;
 if (count < (degree+1)) return ;
 *nvec = (count-1)/degree;
 temp = get_list_tail(head);
 temp2 = temp;
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc(degree*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=degree;j++)
      {
       if (j == degree) temp2 = temp;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       temp = temp->prev;
      }
   }
}


void list_get_C0_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degree)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 register int count,i,j;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < 3) return;
 *nvec = (count-1)/degree;
/* *nvec = (count+1)/(degree+1);*/
 if (!(*nvec)) (*nvec)++;
 if (count % (degree+1)) (*nvec)++;
 temp = get_list_tail(head);
 temp2 = temp;
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc((degree+1)*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=degree;j++)
      {
       if (j == degree) temp2 = temp;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       if (temp->prev) temp = temp->prev;
      }
   }
}


void list_get_C0_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree, int mindim, int maxdim)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 register int count,i,j;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < mindim+1) return;
 *degree = count-1;
 if ((*degree) > maxdim) *degree = maxdim;
 *nvec = (count-1)/(*degree);
/* *nvec = (count+1)/((*degree)+1);*/
 if (!(*nvec)) (*nvec)++;
 temp = get_list_tail(head);
 temp2 = temp;
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc((*degree+1)*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=(*degree);j++)
      {
       if (j == (*degree)) temp2 = temp;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       temp = temp->prev;
      }
   }
}


void list_get_C0(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec)
{
 TRACE
 list_get_C0_dim(head, spvec, nvec, 3);
}


void list_get_splines_C0_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree)
{
 TRACE
 list_get_C0_freedim(head, spvec, nvec, degree, 3, 10);
}


void list_get_bcurves_C0_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree)
{
 TRACE
 list_get_C0_freedim(head, spvec, nvec, degree, 2, 12);
}


void list_get_C02(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec)
{
 TRACE
 list_get_C02_dim(head, spvec, nvec, 3, 3);
}


void list_get_C22(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec)
{
 TRACE
 list_get_C22_dim(head, spvec, nvec, 3, 3);
}

							/* scale is NOT yet supported for deBoor points using Graville abscissa */

void check_C2_constraints(struct Polygon4D* list, int degree, int scaled)
{
 struct Polygon4D* temp;
 struct Polygon4D* prev;
 struct Polygon4D* next;
 struct Polygon4D* curr;
 struct Polygon4D* c1pt;
 struct Polygon4D* c2pt;
 struct Polygon4D* ppt;
 struct Polygon4D* p2pt;
 int i;
 int j;
 double d1,d2;
 TRACE
 temp = list;
 i = 0;
 prev = NULL;
 next = NULL;
 curr = NULL;
 d1 = d2 = 0.;
 if (!temp) return;
 while (temp)
   {
    if (!(i % degree))
      {
       prev = curr;
       curr = next;
       next = temp;
       if (prev && curr && next)
         {
	  c1pt = curr->prev;
	  c2pt = c1pt->prev;
	  ppt  = curr->next;
	  p2pt = ppt->next;
	  if (scaled)
	    {
	     d1 = 0.;
	     d2 = 0.;
	     for (j=0;j<DIM-1;j++)
	       {
                d1 += (curr->p.p[j] - prev->p.p[j]) * (curr->p.p[j] - prev->p.p[j]);
		d2 += (next->p.p[j] - curr->p.p[j]) * (next->p.p[j] - curr->p.p[j]);
	       }
	    /* while ((d1 == 0. || d2 == 0.) && j < DIM)
	      {
	       d1 = curr->p.p[j] - prev->p.p[j];
	       d2 = next->p.p[j] - curr->p.p[j];
	       j++;
	      }*/
	     d1 = sqrt(d1);
	     d2 = sqrt(d2);
	     if (d1 == 0.) { d2 = 0.; d1 = 1.; }
	    }
	  for (j=0;j<DIM-1;j++)
	    {
	     if (d1 == 0.) { d1 = 1.; d2 = 0.; }
	     if (d1*d2 < 0.) d2 *= -1.;
	     if (scaled) c1pt->p.p[j] = curr->p.p[j] + (d2*(curr->p.p[j]-ppt->p.p[j]))/d1;
	     else c1pt->p.p[j] = curr->p.p[j] + curr->p.p[j]-ppt->p.p[j];
	     if (scaled) c2pt->p.p[j] = 2*c1pt->p.p[j]-curr->p.p[j]+(d2*(curr->p.p[j]-2*ppt->p.p[j]+p2pt->p.p[j]))/d1;
	     else c2pt->p.p[j] = 2*c1pt->p.p[j]-2*ppt->p.p[j]+p2pt->p.p[j];
	    }
         }
      }
    temp = temp->prev;
    i++;
   }
}

							/* scale is NOT yet supported for deBoor points using Graville abscissa */

void check_C1_constraints(struct Polygon4D* list, int degree, int scaled)
{
 struct Polygon4D* temp;
 struct Polygon4D* prev;
 struct Polygon4D* next;
 struct Polygon4D* curr;
 struct Polygon4D* c1pt;
 struct Polygon4D* ppt;
 int i;
 int j;
 double d1,d2;
 TRACE
 temp = list;
 i = 0;
 prev = NULL;
 next = NULL;
 curr = NULL;
 d1 = d2 = 0.;
 if (!temp) return;
 while (temp)
   {
    if (!(i % degree))
      {
       prev = curr;
       curr = next;
       next = temp;
       if (prev && curr && next)
         {
	  c1pt = curr->prev;
	  ppt  = curr->next;
	  if (scaled)
	    {
	     d1 = 0.;
	     d2 = 0.;
	     /*while ((d1 == 0. || d2 == 0.) && j < DIM)
	      {
	       d1 = curr->p.p[j] - prev->p.p[j];
	       d2 = next->p.p[j] - curr->p.p[j];
	       j++;
	      }
	     if (d1 != 0. && d2 != 0.)
	     if (d1*d2 < 0.) d2 *= -1.;*/
	     for (j=0;j<DIM-1;j++)
	       {
                d1 += (curr->p.p[j] - prev->p.p[j]) * (curr->p.p[j] - prev->p.p[j]);
		d2 += (next->p.p[j] - curr->p.p[j]) * (next->p.p[j] - curr->p.p[j]);
	       }
	     d1 = sqrt(d1);
	     d2 = sqrt(d2);
	     if (d1 == 0.) { d2 = 0.; d1 = 1.; }
	    }
	  for (j=0;j<DIM-1;j++)
	    {
	     if (scaled) c1pt->p.p[j] = curr->p.p[j] + (d2*(curr->p.p[j]-ppt->p.p[j]))/d1;
	     else c1pt->p.p[j] = curr->p.p[j] + curr->p.p[j]-ppt->p.p[j];
	    }
         }
      }
    temp = temp->prev;
    i++;
   }
}


void list_get_C1_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degree)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 struct Polygon4D* tail;
 register int count,i,j;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < 3) return;
 *nvec = (count-1)/degree;
 if (!(*nvec)) (*nvec)++;
/* *nvec = (count+1)/(degree+1);*/
 if (count % (degree+1)) (*nvec)++;
 temp = get_list_tail(head);
 tail = temp;
 temp2 = temp;
 check_C1_constraints(tail, degree, 1);
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc((degree+1)*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=degree;j++)
      {
       if (j == degree) temp2 = temp;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       if (temp->prev) temp = temp->prev;
      }
   }
}


void list_get_C1_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree, int mindim, int maxdim)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 struct Polygon4D* tail;
 register int count,i,j;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < mindim+1) return;
 *degree = count-1;
 if ((*degree) > maxdim) *degree = maxdim;
 *nvec = (count-1)/(*degree);
 /**nvec = (count+1)/((*degree)+1);*/
 if (!(*nvec)) (*nvec)++;
 temp = get_list_tail(head);
 tail = temp;
 temp2 = temp;
 check_C1_constraints(tail, *degree, 1);
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc((*degree+1)*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=(*degree);j++)
      {
       if (j == (*degree)) temp2 = temp;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       temp = temp->prev;
      }
   }
}


void list_get_C1(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec)
{
 TRACE
 list_get_C1_dim(head, spvec, nvec, 3);
}


void list_get_splines_C1_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree)
{
 TRACE
 list_get_C1_freedim(head, spvec, nvec, degree, 3, 10);
}


void list_get_bcurves_C1_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree)
{
 TRACE
 list_get_C1_freedim(head, spvec, nvec, degree, 2, 12);
}


void list_get_C2_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degree)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 struct Polygon4D* tail;
 register int count,i,j;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < 3) return;
 *nvec = (count-1)/degree;
/* if (count % (degree+1)) (*nvec)++;*/
 if (!(*nvec)) (*nvec)++;
/* printf("count = %d, nvec = %d\n", count, *nvec);*/
 temp = get_list_tail(head);
 tail = temp;
 temp2 = temp;
 check_C2_constraints(tail, degree, 0);
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc((degree+1)*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=degree;j++)
      {
       if (j == degree) temp2 = temp;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       if (temp->prev) temp = temp->prev;
      }
   }
}


void list_get_C2_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree, int mindim, int maxdim)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 struct Polygon4D* tail;
 register int count,i,j;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < mindim+1) return;
 *degree = count-1;
 if ((*degree) > maxdim) *degree = maxdim;
 *nvec = (count-1)/(*degree);
 if (!(*nvec)) (*nvec)++;
/* *nvec = (count+1)/((*degree)+1);*/
 temp = get_list_tail(head);
 tail = temp;
 temp2 = temp;
 check_C2_constraints(tail, *degree, 0);
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc((*degree+1)*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=(*degree);j++)
      {
       if (j == (*degree)) temp2 = temp;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       temp = temp->prev;
      }
   }
}


void list_get_C2(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec)
{
 TRACE
 list_get_C2_dim(head, spvec, nvec, 3);
}


void list_get_splines_C2_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree)
{
 TRACE
 list_get_C2_freedim(head, spvec, nvec, degree, 3, 10);
}


void list_get_bcurves_C2_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree)
{
 TRACE
 list_get_C2_freedim(head, spvec, nvec, degree, 2, 12);
}


void copy_spline(struct BSpline* dst, struct BSpline* src)
{
 register int i;
 TRACE
 dst->K  = src->K;
 dst->M  = src->M;
 dst->N  = src->N;
 dst->A  = src->A;
 dst->U0 = src->U0;
 dst->U1 = src->U1;
 dst->npoints = src->npoints;
 if (dst->points) free(dst->points);
 if (dst->npoints)
   {
    dst->points = (struct Vector4D*)malloc(dst->npoints*sizeof(struct Vector4D));
    for (i=0;i<dst->npoints;i++) copy_point(&dst->points[i], &src->points[i]);
   }
 else dst->points = NULL;
 if (dst->P) free(dst->P);
 if (dst->K)
   {
    dst->P = (struct Vector4D*)malloc((dst->K+1)*sizeof(struct Vector4D));
    for (i=0;i<=dst->K;i++)
      {
       copy_point(&(dst->P[i]), &(src->P[i]));
      }
   }
 else dst->P = NULL;
 if (dst->T) free(dst->T);
 if (dst->A)
  {
   dst->T = (double*)malloc((dst->A+1)*sizeof(double));
   for (i=0;i<=dst->A;i++)  dst->T[i] = src->T[i];
  }
 else dst->T = NULL;
 if (dst->W) free(dst->W);
 if (dst->K)
   {
    dst->W = (double*)malloc((dst->K+1)*sizeof(double));
    for (i=0;i<=dst->K;i++)  dst->W[i] = src->W[i];
   }
 else dst->W = NULL;
}


void zero_spline(struct BSpline* dst)
{
 TRACE
 dst->K  = 0;
 dst->M  = 0;
 dst->N  = 0;
 dst->A  = 0;
 dst->U0 = 0.0;
 dst->U1 = 1.0;
 dst->npoints = 0;
 dst->points = NULL;
 dst->P      = NULL;
 dst->T      = NULL;
 dst->W      = NULL;
}


void free_spline(struct BSpline* dst)
{
 TRACE
 if (dst->points) free(dst->points);
 if (dst->P) free(dst->P);
 if (dst->T) free(dst->T);
 if (dst->W) free(dst->W);
}


void copy_bezier(struct BezierCurve* dst, struct BezierCurve* src)
{
 register int i;
 TRACE
 dst->N  = src->N;
 dst->U0 = src->U0;
 dst->U1 = src->U1;
 dst->npoints = src->npoints;
 if (dst->points) free(dst->points);
 if (dst->npoints)
   {
    dst->points = (struct Vector4D*)malloc(dst->npoints*sizeof(struct Vector4D));
    for (i=0;i<dst->npoints;i++) copy_point(&dst->points[i], &src->points[i]);
   }
 else dst->points = NULL;
 if (dst->P) free(dst->P);
 if (dst->N)
   {
    dst->P = (struct Vector4D*)malloc((dst->N+1)*sizeof(struct Vector4D));
    for (i=0;i<=dst->N;i++)
      {
       copy_point(&(dst->P[i]), &(src->P[i]));
      }
   }
 else dst->P = NULL;
}


void zero_bezier(struct BezierCurve* dst)
{
 TRACE
 dst->N  = 0;
 dst->U0 = 0.0;
 dst->U1 = 1.0;
 dst->npoints = 0;
 dst->points = NULL;
 dst->P      = NULL;
}


void free_bezier(struct BezierCurve* dst)
{
 TRACE
 if (dst->points) free(dst->points);
 if (dst->P) free(dst->P);
}


void vector_subtract_3d(struct Vector4D* dst, struct Vector4D* vec)
{
 register int i;
 TRACE
 for (i=0;i<DIM-1;i++) dst->p[i] -= vec->p[i];
}


int idx_used(int idx, int* idx_tab, int nidx)
{
 int i;
 TRACE
 for (i=0;i<nidx;i++)
   {
    if (idx_tab[i] == idx) return 1;
    else if (idx_tab[i] == -1) return 0;
   }
 return 0;
}


void get_bezier_polygon4(struct Vector4D* control, int ncontrol, struct Vector4D** vertex)
{
 TRACE
 (*vertex) = (struct Vector4D*)malloc(ncontrol*sizeof(struct Vector4D));
 copy_vector(&((*vertex)[0]), &control[0]);
 copy_vector(&((*vertex)[1]), &control[1]);
 copy_vector(&((*vertex)[2]), &control[2]);
 copy_vector(&((*vertex)[3]), &control[3]);
}


void get_bezier_polygon(struct Vector4D* control, int ncontrol, struct Vector4D** vertex)
{
 int i,j,k,mi,mj,mk,minx;
 struct Vector4D u, v;
 double prod;
 double maxprod;
 struct Vector4D base;
 int* idx;
 TRACE
 printf("warning: get_bezier_polygon is marked broken!\n");
 (*vertex) = (struct Vector4D*)malloc(ncontrol*sizeof(struct Vector4D));
 idx = (int*)malloc(ncontrol*sizeof(int));
 for (i=0;i<ncontrol;i++) idx[i] = -1;
 maxprod = -9e19;
 minx    = 6e8;
 if (ncontrol == 3) for (i=0;i<3;i++) copy_vector(&((*vertex)[i]), &control[i]);
 if (ncontrol <= 3) return;
 mi = 0;
 for (i=0;i<ncontrol;i++) if (control[i].p[0] < minx)
   {
    minx = control[i].p[0];
    mi = i;
   }
 mj = mk = 0;
 copy_vector(&base, &control[mi]);
 for (j=0;j<ncontrol;j++)
 for (k=0;k<ncontrol;k++)
   {
    if (j == k || j == mi || k == mi) continue;
    copy_vector(&u, &control[j]);
    copy_vector(&v, &control[k]);
    vector_subtract_3d(&u, &base);
    vector_subtract_3d(&v, &base);
    normalize_vector_3d(&u);
    normalize_vector_3d(&v);
    prod = acos(vector_product_3d(&u, &v));
    if (prod > maxprod)
      {
       maxprod = prod;
       mj = j;
       mk = k;
      }
 }
 copy_vector(&base, &control[mi]);
 copy_vector(&((*vertex)[0]), &control[mj]);
 copy_vector(&((*vertex)[1]), &control[mi]);
 copy_vector(&((*vertex)[2]), &control[mk]);
 idx[0] = mj;
 idx[1] = mi;
 idx[2] = mk;
 if (ncontrol == 4)
   {
    i = 0;
    while (mi == i || mj == i || mk == i) i++;
    copy_vector(&((*vertex)[3]), &control[i]);
   }
 for (i=3;i<ncontrol;i++)
  {
   maxprod = -9e19;
   copy_vector(&base, &control[mj]);
   for (j=0;j<ncontrol;j++)
     {
      if (idx_used(j, idx, ncontrol)) continue;
      copy_vector(&u, &base);
      copy_vector(&v, &control[j]);
      vector_subtract_3d(&u, &base);
      vector_subtract_3d(&v, &base);
      normalize_vector_3d(&u);
      normalize_vector_3d(&v);
      prod = acos(vector_product_3d(&u, &v));
      if (prod > maxprod)
        {
         maxprod = prod;
         mj = j;
        }
     }
   copy_vector(&((*vertex)[i]), &control[mj]);
   }
 free(idx);
/* printf("MIN (%d, %d, %d) = %f\n", mi,mj,mk,maxprod);*/
}

	/* NOTE: You need to store old selected pointer value because this recreates bl-list, so pointer would change ! */

void deboor_from_bezier_scaled(struct BSpline* spline, int nspline, struct Polygon4D** bl)
{
 struct Polygon4D point;
 /*struct Polygon4D aux;
 struct Polygon4D aux2;*/
 struct Vector4D *curr, *prev, *next;
 int i,idx,j;
 double a;
 double d1,d2;
 TRACE
 if (!nspline) return;
 if (nspline<2)
   {
/*    deboor_from_bezier(spline, nspline, bl);*/
    return;
   }
 if (bl) list_free(bl);
 point.p.p[DIM-1] = 1.;
 point.p.w = 1.;
 /*for (i=0;i<DIM-1;i++) point.p.p[i] = spline[nspline-1].P[2].p[i] + (spline[nspline-1].P[2].p[i] - spline[nspline-1].P[1].p[i]);
 aux.p.p[DIM-1] = 1.;
 for (i=0;i<DIM-1;i++) aux.p.p[i] = spline[nspline-1].P[3].p[i] + (spline[nspline-1].P[3].p[i] - spline[nspline-1].P[2].p[i]);
 aux2.p.p[DIM-1] = 1.;
 for (i=0;i<DIM-1;i++) aux2.p.p[i] = aux.p.p[i] - 2*(point.p.p[i] - aux.p.p[i]);
 list_add_pointp(bl, &aux2);*/
 /*for (i=0;i<DIM-1;i++)
	point.p.p[i] = spline[nspline-1].P[2].p[i] +(spline[nspline-1].P[2].p[i] - spline[nspline-1].P[1].p[i]);
 list_add_pointp(bl, &point);*/
 for (idx=nspline-1;idx>0;idx--)
   {
    d1 = 0.;
    d2 = 0.;
    curr = &spline[idx].P[0];
    next = &spline[idx].P[3];
    prev = &spline[idx-1].P[0];
    for (j=0;j<DIM-1;j++)
      {
       d1 += (curr->p[j]-prev->p[j])*(curr->p[j]-prev->p[j]);
       d2 += (next->p[j]-curr->p[j])*(next->p[j]-curr->p[j]);
      }
    d1 = sqrt(d1);
    d2 = sqrt(d2);
    if (d2 == 0.) { d1 = 0.; d2 = 1.; }
    a = 1.;	/* FIXME compute real a here! */
    printf("warning: deboor_from_bezier_scaled: not yet finished.\n");
    printf("d1 = %f, d2 = %f, a = %f\n", d1, d2, a);
    for (i=0;i<DIM-1;i++)
	point.p.p[i] = spline[idx].P[1].p[i] - a*(spline[idx].P[2].p[i] - spline[idx].P[1].p[i]);
    list_add_pointp(bl, &point);
   }
 /*point.p.p[DIM-1] = 1.;
 point.p.w = 1.;
 for (i=0;i<DIM-1;i++) point.p.p[i] = spline[0].P[1].p[i] - (spline[0].P[2].p[i] - spline[0].P[1].p[i]);
 aux.p.p[DIM-1] = 1.;
 aux.p.w = 1.;
 for (i=0;i<DIM-1;i++) aux.p.p[i] = spline[0].P[0].p[i] - (spline[0].P[1].p[i] - spline[0].P[0].p[i]);
 aux2.p.p[DIM-1] = 1.;
 aux2.p.w = 1.;
 for (i=0;i<DIM-1;i++) aux2.p.p[i] = aux.p.p[i] - 2*(point.p.p[i] - aux.p.p[i]);
 list_add_pointp(bl, &point);
 list_add_pointp(bl, &aux2);*/
}


void deboor_from_bezier(struct BSpline* spline, int nspline, struct Polygon4D** bl)
{
 struct Polygon4D point;
 struct Polygon4D aux;
 struct Polygon4D aux2;
 int i,idx;
 TRACE
 if (!nspline) return;
 if (bl) list_free(bl);
 point.p.p[DIM-1] = 1.;
 for (i=0;i<DIM-1;i++) point.p.p[i] = spline[nspline-1].P[2].p[i] + (spline[nspline-1].P[2].p[i] - spline[nspline-1].P[1].p[i]);
 point.p.w = spline[nspline-1].P[2].w + (spline[nspline-1].P[2].w - spline[nspline-1].P[1].w);
 aux.p.p[DIM-1] = 1.;
 for (i=0;i<DIM-1;i++) aux.p.p[i] = spline[nspline-1].P[3].p[i] + (spline[nspline-1].P[3].p[i] - spline[nspline-1].P[2].p[i]);
 aux.p.w = spline[nspline-1].P[3].w + (spline[nspline-1].P[3].w - spline[nspline-1].P[2].w);
 aux2.p.p[DIM-1] = 1.;
 for (i=0;i<DIM-1;i++) aux2.p.p[i] = aux.p.p[i] - 2*(point.p.p[i] - aux.p.p[i]);
 aux2.p.w = aux.p.w - 2*(point.p.w - aux.p.w);
 list_add_pointp(bl, &aux2);
 for (idx=nspline-1;idx>=0;idx--)
   {
    for (i=0;i<DIM-1;i++) point.p.p[i] = spline[idx].P[2].p[i] + (spline[idx].P[2].p[i] - spline[idx].P[1].p[i]);
    point.p.w = spline[idx].P[2].w + (spline[idx].P[2].w - spline[idx].P[1].w);
    list_add_pointp(bl, &point);
   }
 point.p.p[DIM-1] = 1.;
 for (i=0;i<DIM-1;i++) point.p.p[i] = spline[0].P[1].p[i] - (spline[0].P[2].p[i] - spline[0].P[1].p[i]);
 point.p.w = spline[0].P[1].w - (spline[0].P[2].w - spline[0].P[1].w);
 aux.p.p[DIM-1] = 1.;
 for (i=0;i<DIM-1;i++) aux.p.p[i] = spline[0].P[0].p[i] - (spline[0].P[1].p[i] - spline[0].P[0].p[i]);
 aux.p.w = spline[0].P[0].w - (spline[0].P[1].w - spline[0].P[0].w);
 aux2.p.p[DIM-1] = 1.;
 for (i=0;i<DIM-1;i++) aux2.p.p[i] = aux.p.p[i] - 2*(point.p.p[i] - aux.p.p[i]);
 aux2.p.w = aux.p.w - 2*(point.p.w - aux.p.w);
 list_add_pointp(bl, &point);
 list_add_pointp(bl, &aux2);
}


void deboor_from_bezier_inv(struct BSpline* spline, int nspline, struct Polygon4D** bl)
{
 struct Polygon4D point;
 struct Polygon4D aux;
 struct Polygon4D aux2;
 int i,idx;
 TRACE
 if (!nspline) return;
 if (bl) list_free(bl);
 printf("warning: deboor_from_bezier_inv: doesn't recomute weights (and won't soon)\n");
 point.p.p[DIM-1] = 1.;
 point.p.w = 1.;
 for (i=0;i<DIM-1;i++) point.p.p[i] = spline[0].P[1].p[i] - (spline[0].P[2].p[i] - spline[0].P[1].p[i]);
 aux.p.p[DIM-1] = 1.;
 aux.p.w = 1.;
 for (i=0;i<DIM-1;i++) aux.p.p[i] = spline[0].P[0].p[i] - (spline[0].P[1].p[i] - spline[0].P[0].p[i]);
 aux2.p.p[DIM-1] = 1.;
 aux2.p.w = 1.;
 for (i=0;i<DIM-1;i++) aux2.p.p[i] = aux.p.p[i] - 2*(point.p.p[i] - aux.p.p[i]);
 list_add_pointp(bl, &aux2);
 list_add_pointp(bl, &point);
 for (idx=0;idx<nspline;idx++)
   {
    for (i=0;i<DIM-1;i++)
	point.p.p[i] = spline[idx].P[2].p[i] + (spline[idx].P[2].p[i] - spline[idx].P[1].p[i]);
    list_add_pointp(bl, &point);
   }
 point.p.p[DIM-1] = 1.;
 point.p.w = 1.;
 for (i=0;i<DIM-1;i++) point.p.p[i] = spline[nspline-1].P[2].p[i] + (spline[nspline-1].P[2].p[i] - spline[nspline-1].P[1].p[i]);
 aux.p.p[DIM-1] = 1.;
 aux.p.w = 1.;
 for (i=0;i<DIM-1;i++) aux.p.p[i] = spline[nspline-1].P[3].p[i] + (spline[nspline-1].P[3].p[i] - spline[nspline-1].P[2].p[i]);
 aux2.p.p[DIM-1] = 1.;
 aux2.p.w = 1.;
 for (i=0;i<DIM-1;i++) aux2.p.p[i] = aux.p.p[i] - 2*(point.p.p[i] - aux.p.p[i]);
 list_add_pointp(bl, &aux2);
}


void bezier_from_deboor_multi(struct Polygon4D** bez, struct Polygon4D* boor)
{
 int i;
 struct Polygon4D* temp;
 struct Polygon4D* next;
 struct Polygon4D* prev;
 struct Polygon4D point;
 TRACE
 if (!boor) return;
 printf("warning: bezier_from_deboor_multi: function is experimental\n");
 printf("call is identical to bezier_from_deboor, but generates two addictional beziers\n");
 printf("but this function won't be keep up-to-date\n");
 next = prev = NULL;
 temp = boor->next;
 if (!temp->next) return;
 list_free(bez);
 point.p.p[DIM-1] = 1.;
 point.p.w = 1.;
 while (temp->next)
   {
    next = temp->next;
    prev = temp->prev;
    for (i=0;i<DIM-1;i++) point.p.p[i] = (((prev->p.p[i]+2*temp->p.p[i])/3) + ((2*temp->p.p[i]+next->p.p[i])/3))/2;
    list_add_pointp(bez, &point);
    for (i=0;i<DIM-1;i++) point.p.p[i] = (2*temp->p.p[i]+next->p.p[i])/3;
    list_add_pointp(bez, &point);
    for (i=0;i<DIM-1;i++) point.p.p[i] = (temp->p.p[i]+2*next->p.p[i])/3;
    list_add_pointp(bez, &point);
    temp = temp->next;
   }
 for (i=0;i<DIM-1;i++) point.p.p[i] = (((prev->p.p[i]+2*temp->p.p[i])/3) + ((2*temp->p.p[i]+next->p.p[i])/3))/2;
 list_add_pointp(bez, &point);
}


void bezier_from_deboor(struct Polygon4D** bez, struct Polygon4D* boor)
{
 int i;
 struct Polygon4D* temp;
 struct Polygon4D* next;
 struct Polygon4D* prev;
 struct Polygon4D point;
 TRACE
 if (!boor) return;
 next = prev = NULL;
 temp = boor->next;
 if (!temp || !temp->next || !temp->next->next) return;
 list_free(bez);
 point.p.p[DIM-1] = 1.;
 while (temp->next)
   {
    next = temp->next;
    prev = temp->prev;
    for (i=0;i<DIM-1;i++) point.p.p[i] = (((prev->p.p[i]+2*temp->p.p[i])/3) + ((2*temp->p.p[i]+next->p.p[i])/3))/2;
    point.p.w = (((prev->p.w+2*temp->p.w)/3) + ((2*temp->p.w+next->p.w)/3))/2;
    list_add_pointp(bez, &point);
    if (temp->next->next)
      {
       for (i=0;i<DIM-1;i++) point.p.p[i] = (2*temp->p.p[i]+next->p.p[i])/3;
       point.p.w = (2*temp->p.w+next->p.w)/3;
       list_add_pointp(bez, &point);
       for (i=0;i<DIM-1;i++) point.p.p[i] = (temp->p.p[i]+2*next->p.p[i])/3;
       point.p.w = (temp->p.w+2*next->p.w)/3;
       list_add_pointp(bez, &point);
      }
    temp = temp->next;
   }
}


void bezier_from_deboor_scaled(struct Polygon4D** bez, struct Polygon4D* boor)
{
 int i;
 struct Polygon4D* temp;
 struct Polygon4D* next;
 struct Polygon4D* prev;
 struct Polygon4D point;
 TRACE
 if (!boor) return;
 next = prev = NULL;
 temp = boor->next;
 if (!temp || !temp->next || !temp->next->next) return;
 list_free(bez);
 printf("warning: bezier_from_deboor_scaled: currently SCALE isn't supported\n");
 printf("call is identical to bezier_from_deboor\n");
 printf("but this function won't be keep up-to-date\n");
 point.p.p[DIM-1] = 1.;
 point.p.w = 1.;
 while (temp->next)
   {
    next = temp->next;
    prev = temp->prev;
    for (i=0;i<DIM-1;i++) point.p.p[i] = (((prev->p.p[i]+2*temp->p.p[i])/3) + ((2*temp->p.p[i]+next->p.p[i])/3))/2;
    list_add_pointp(bez, &point);
    if (temp->next->next)
      {
       for (i=0;i<DIM-1;i++) point.p.p[i] = (2*temp->p.p[i]+next->p.p[i])/3;
       list_add_pointp(bez, &point);
       for (i=0;i<DIM-1;i++) point.p.p[i] = (temp->p.p[i]+2*next->p.p[i])/3;
       list_add_pointp(bez, &point);
      }
    temp = temp->next;
   }
}


void bezier_from_interpol(struct Polygon4D** bez, struct Polygon4D* interpol)
{
 struct Polygon4D* temp;
 struct BSpline bspline;
 int n,i,j;
 double *tk;
 double **N;
 double tstep, curt;
 TRACE
 printf("bezier_from_interpol() is currently marked broken!\n");
 return;
 if (!interpol) return;
 list_free(bez);
 temp = interpol;
 n = 0;
 while (temp) { n++; temp = temp->next; }
 printf("n = %d\n", n);
 tk = (double*)malloc(n*sizeof(double));
 for (i=0;i<n;i++) tk[i] = (double)(i+1)/(double)(n+1);
 for (i=0;i<n;i++) printf("tk[%d] = %f\n", i, tk[i]);
 N = (double**)malloc(n*sizeof(double*));
 for (i=0;i<n;i++) N[i] = (double*)malloc(n*sizeof(double));
 bspline.M  = 3;
 bspline.K  = 3;
 bspline.N  = 4;
 bspline.A  = 10;
 bspline.T = (double*)malloc((bspline.A+1)*sizeof(double));
 if (bspline.N>1) tstep = 1.0/((double)bspline.N);
 else     tstep = 1.0;
 curt = 0.0;
 for (i=0;i<=bspline.A;i++)
   {
     if (i<=bspline.M) bspline.T[i] = 0.0;
     else if (i>=bspline.A-bspline.M) bspline.T[i] = 1.0;
     else
       {
        curt += tstep;
	if (curt > 1.0) curt = 1.0;
	bspline.T[i] = curt;
       }
     printf("T[%d] = %f\n", i, bspline.T[i]);
   }
 for (i=0;i<n;i++)
 for (j=0;j<n;j++) N[i][j] = basis_bspline(bspline.T, tk[i], bspline.M, bspline.M, 3, j);
 for (i=0;i<n;i++)
   {
    for (j=0;j<n;j++) printf("%f ", N[i][j]);
    printf("\n");
   }
}


void list_get_C02_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degreex, int degreey)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 register int count,i,j;
 int degree;
 TRACE
 degree = ((degreex+1)*(degreey+1))-1;
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < 6) return;
 *nvec = (count-1)/degree;
/* *nvec = (count+1)/(degree+1);*/
 if (!(*nvec)) (*nvec)++;
 if (count % (degree+1)) (*nvec)++;
 temp = get_list_tail(head);
 temp2 = temp;
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc((degree+1)*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=degree;j++)
      {
       if (j == degree) temp2 = temp->next->next->next;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       if (temp->prev) temp = temp->prev;
      }
   }
}


void list_get_C02_nm(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int n, int m)
{
 struct Polygon4D* temp;
 int count,i,j,x;
 int npts;
 struct Vector4D* varray;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 npts = ((3*n)+1)*((3*m)+1);
 while (temp) { count++; temp = temp->next; }
/* printf("count = %d, npts=%d\n", count, npts);*/
/* printf("m = %d, n = %d\n", m, n);*/
 if (count < (npts-1)) return;
 varray = (struct Vector4D*)malloc((npts+1)*sizeof(struct Vector4D));
 *nvec = n*m;
 temp = get_list_tail(head);
 count = 0;
 while (temp)
   {
    copy_vector(&varray[count], &temp->p);
    count++;
    temp = temp->prev;
    if (count >= npts) break;
   }
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc(16*sizeof(struct Vector4D));
 for (i=0;i<n;i++)
 for (j=0;j<m;j++)
 for (x=0;x<16;x++)		/* m*i+j OR n*i+j FIXED */
       copy_vector(&((*spvec)[m*i+j][x]), &varray[(x%4)+((x/4)*((3*m)+1))+(j*3)+((3*((3*m)+1))*i)]);
 free(varray);
}


void list_get_C02_49(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec)
{
 struct Polygon4D* temp;
 int count,i,j,x;
 struct Vector4D* varray;
 TRACE
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count < 49) return;
 varray = (struct Vector4D*)malloc(50*sizeof(struct Vector4D));
 *nvec = 4;
 temp = get_list_tail(head);
 count = 0;
 while (temp)
   {
    copy_vector(&varray[count], &temp->p);
    count++;
    temp = temp->prev;
    if (count >= 49) break;
   }
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc(16*sizeof(struct Vector4D));
 for (i=0;i<2;i++)
 for (j=0;j<2;j++)
 for (x=0;x<16;x++)
       copy_vector(&((*spvec)[2*i+j][x]), &varray[ (x%4) + ((x/4)*7) + (i*3) + (21*j)]);
 free(varray);
}


void list_get_C22_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degreex, int degreey)
{
 struct Polygon4D* temp;
 struct Polygon4D* temp2;
 register int count,i,j;
 int degree;
 TRACE
 degree = ((degreex+1)*(degreey+1))-1;
 count = 0;
 temp = head;
 *nvec = 0;
 *spvec = NULL;
 while (temp) { count++; temp = temp->next; }
 if (count <= degree) return;
 *nvec = (count-1)/degree;
/* *nvec = (count+1)/(degree+1);*/
 if (!(*nvec)) (*nvec)++;
 if (count % (degree+1)) (*nvec)++;
 temp = get_list_tail(head);
 temp2 = temp;
 check_C22_constraints(temp, degree, 0);
 *spvec = (struct Vector4D**)malloc((*nvec)*sizeof(struct Vector4D*));
 for (i=0;i<(*nvec);i++) (*spvec)[i] = (struct Vector4D*)malloc((degree+1)*sizeof(struct Vector4D));
 for (i=0;i<(*nvec);i++)
   {
    temp = temp2;
    for (j=0;j<=degree;j++)
      {
       if (j == degree) temp2 = temp->next->next->next;
       copy_vector(&((*spvec)[i][j]), &temp->p);
       if (temp->prev) temp = temp->prev;
      }
   }
}


void check_C22_constraints(struct Polygon4D* list, int degree, int scaled)
{
 struct Polygon4D* temp;
 struct Polygon4D* prev;
 struct Polygon4D* next;
 struct Polygon4D* curr;
 struct Polygon4D* c1pt;
 struct Polygon4D* c2pt;
 struct Polygon4D* ppt;
 struct Polygon4D* p2pt;
 struct Polygon4D* supertemp;
 int i;
 int j;
 int s;
 double d1,d2;
 TRACE
 printf("warning: check_C22_constraints() currently broken!\n");
 return;
 supertemp = list;
 for (s=0;s<3;s++)
 {
  temp = supertemp;
  i = 0;
  prev = NULL;
  next = NULL;
  curr = NULL;
  d1 = d2 = 0.;
  if (!temp) return;
  while (temp)
   {
    if (!(i % degree))
      {
       prev = curr;
       curr = next;
       next = temp;
       if (prev && curr && next)
         {
	  c1pt = curr->prev->prev->prev->prev;
	  c2pt = c1pt->prev->prev->prev->prev;
	  ppt  = curr->next->next->next->next;
	  p2pt = ppt->next->next->next->next;
	  if (scaled)
	    {
	     d1 = 0.;
	     d2 = 0.;
	     for (j=0;j<DIM-1;j++)
	       {
                d1 += (curr->p.p[j] - prev->p.p[j]) * (curr->p.p[j] - prev->p.p[j]);
		d2 += (next->p.p[j] - curr->p.p[j]) * (next->p.p[j] - curr->p.p[j]);
	       }
	     d1 = sqrt(d1);
	     d2 = sqrt(d2);
	     if (d1 == 0.) { d2 = 0.; d1 = 1.; }
	    }
	  for (j=0;j<DIM-1;j++)
	    {
	     if (d1 == 0.) { d1 = 1.; d2 = 0.; }
	     if (d1*d2 < 0.) d2 *= -1.;
	     if (scaled) c1pt->p.p[j] = curr->p.p[j] + (d2*(curr->p.p[j]-ppt->p.p[j]))/d1;
	     else c1pt->p.p[j] = curr->p.p[j] + curr->p.p[j]-ppt->p.p[j];
	     if (scaled) c2pt->p.p[j] = 2*c1pt->p.p[j]-curr->p.p[j]+(d2*(curr->p.p[j]-2*ppt->p.p[j]+p2pt->p.p[j]))/d1;
	     else c2pt->p.p[j] = 2*c1pt->p.p[j]-2*ppt->p.p[j]+p2pt->p.p[j];
	    }
         }
      }
    temp = temp->prev->prev->prev->prev;
    i++;
   }
  supertemp = supertemp->prev;
 }
}


void create_interpolation(struct Polygon4D** intr_curve, struct Polygon4D* intr_points, int np)
{
 int count, npts;
 register int i,j;
 double uconst, wconst, ud, t;
 double* d;
 struct Vector4D A, B, C, D, T;
 struct Polygon4D* temp;
 struct Vector4D *p;
 struct Vector4D *c, *v;
 TRACE
 printf("warning: create interpolation skips weight evaluating interpol-curve\n");
 temp = intr_points;
 count = 0;
 while (temp) { count++; temp = temp->next; }
 if (count < 4) return;
 list_free(intr_curve);
 p = (struct Vector4D*)malloc(count*sizeof(struct Vector4D));
 temp = intr_points;
 npts = count;
 count = 0;
 while (temp)
   {
	copy_vector(&p[count], &temp->p);
	count++;
	temp = temp->next;
   }
 c = (struct Vector4D*)malloc(npts*sizeof(struct Vector4D));
 v = (struct Vector4D*)malloc(npts*sizeof(struct Vector4D));
 for (i=0;i<npts;i++)
   {
    c[i].p[0] = c[i].p[1] = c[i].p[2] = 0.;
    c[i].p[3] = 1.; c[i].w = 1.;
    v[i].p[0] = v[i].p[1] = v[i].p[2] = 0.;
    v[i].p[3] = 1.; c[i].w = 1.;
   }
 d = (double*)malloc(npts*sizeof(double));
 d[0] = d[npts-1] = 0.;
 for (i=1;i<npts-1;i++) d[i] = 2.;
 for (i=1;i<npts-1;++i)
   {
        v[i].p[0] = ((p[i+1].p[0] - p[i].p[0])-(p[i].p[0]-p[i-1].p[0]))/2.;
	v[i].p[1] = ((p[i+1].p[1] - p[i].p[1])-(p[i].p[1]-p[i-1].p[1]))/2.;
	v[i].p[2] = ((p[i+1].p[2] - p[i].p[2])-(p[i].p[2]-p[i-1].p[2]))/2.;
   }
 uconst = .5;
 wconst = .5;
 for (i=2;i<npts-1;++i)
   {
        ud = uconst/2.;
	v[i].p[0] -= v[i-1].p[0]*ud;
	v[i].p[1] -= v[i-1].p[1]*ud;
	v[i].p[2] -= v[i-1].p[2]*ud;
	d[i] -= wconst*ud;
   }
 copy_vector(&c[npts-2], &v[npts-2]);
 for (i=npts-3;i>=1;--i)
   {
        c[i].p[0] = (v[i].p[0]-wconst*c[i+1].p[0])/d[i];
	c[i].p[1] = (v[i].p[1]-wconst*c[i+1].p[1])/d[i];
	c[i].p[2] = (v[i].p[2]-wconst*c[i+1].p[2])/d[i];
   }
 A.p[3] = B.p[3] = C.p[3] = D.p[3] = 1.;
 A.w = B.w = C.w = D.w = 1.;
 for (i=1;i<npts-2;++i)
   {
        copy_vector(&A, &p[i]);
	B.p[0] = (p[i+1].p[0]-p[i].p[0])-(c[i+1].p[0]+2.*c[i].p[0]);
	B.p[1] = (p[i+1].p[1]-p[i].p[1])-(c[i+1].p[1]+2.*c[i].p[1]);
	B.p[2] = (p[i+1].p[2]-p[i].p[2])-(c[i+1].p[2]+2.*c[i].p[2]);
	C.p[0] = c[i].p[0]*3.;
	C.p[1] = c[i].p[1]*3.;
	C.p[2] = c[i].p[2]*3.;
	D.p[0] = (c[i+1].p[0]-c[i].p[0]);
	D.p[1] = (c[i+1].p[1]-c[i].p[1]);
	D.p[2] = (c[i+1].p[2]-c[i].p[2]);
	t = 0.;
	for (j=0;j<np;j++)
	  {
           T.p[0] = A.p[0]+B.p[0]*t+C.p[0]*t*t+D.p[0]*t*t*t;
	   T.p[1] = A.p[1]+B.p[1]*t+C.p[1]*t*t+D.p[1]*t*t*t;
	   T.p[2] = A.p[2]+B.p[2]*t+C.p[2]*t*t+D.p[2]*t*t*t;
	   t += 1./(double)np;
	   list_add_point(intr_curve, T.p[0], T.p[1], T.p[2]);
	  }
   }
 free(p);
 free(c);
 free(v);
 free(d);
}


void table_to_list(struct Polygon4D** list, struct Vector4D** bz, int x, int y)
{
 int i;
 register int j;
 TRACE
 if (!list || !bz) return;
 list_free(list);
 for (i=0;i<x;i++) for (j=0;j<y;j++)
 {
/*   print_vector(&bz[i][j]);*/
/*   list_add_point(list, bz[i][j].p[0], bz[i][j].p[1], bz[i][j].p[2]);*/
   list_add_pointv(list, &bz[i][j]);
 }
}


void list_to_table(struct Polygon4D* list, struct Vector4D** bz, int x, int y)
{
 struct Polygon4D* temp;
 int i;
 register int j;
 TRACE
 if (!list || !bz) return;
 temp = get_list_tail(list);
 i = j = 0;
 while (temp)
   {
    copy_vector(&bz[i][j], &temp->p);
    temp = temp->prev;
    j++;
    if (j == y) { i++; j = 0; }
    if (i == x) break;
   }
}


void update_bezier_2d_fast(struct Vector4D** bz, struct Vector4D** boo, int i, int j, int nox, int noy)
{
 register int bi,bj;
 int d;
 TRACE
 if (i == 0 || j==0 || i==nox-1 || j==noy-1 || !bz || !boo || nox<=0 || noy<=0)  return;
 bi = (i-1)*3+3;
 bj = (j-1)*3+3;
 for (d=0;d<DIM-1;d++)
     {
       bz[bi-2][bj-2].p[d] = (4.0*boo[i-1][j-1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j-1].p[d]+1.0*boo[i][j].p[d])/9.0;
       bz[bi-2][bj-1].p[d] = (2.0*boo[i-1][j-1].p[d]+4.0*boo[i-1][j].p[d]+1.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi-1][bj-2].p[d] = (2.0*boo[i-1][j-1].p[d]+1.0*boo[i-1][j].p[d]+4.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi-1][bj-1].p[d] = (1.0*boo[i-1][j-1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j-1].p[d]+4.0*boo[i][j].p[d])/9.0;
       bz[bi+2][bj+2].p[d] = (4.0*boo[i+1][j+1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j+1].p[d]+1.0*boo[i][j].p[d])/9.0;
       bz[bi+2][bj+1].p[d] = (2.0*boo[i+1][j+1].p[d]+4.0*boo[i+1][j].p[d]+1.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi+1][bj+2].p[d] = (2.0*boo[i+1][j+1].p[d]+1.0*boo[i+1][j].p[d]+4.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi+1][bj+1].p[d] = (1.0*boo[i+1][j+1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j+1].p[d]+4.0*boo[i][j].p[d])/9.0;
       bz[bi+2][bj-2].p[d] = (4.0*boo[i+1][j-1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j-1].p[d]+1.0*boo[i][j].p[d])/9.0;
       bz[bi+2][bj-1].p[d] = (2.0*boo[i+1][j-1].p[d]+4.0*boo[i+1][j].p[d]+1.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi+1][bj-2].p[d] = (2.0*boo[i+1][j-1].p[d]+1.0*boo[i+1][j].p[d]+4.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi+1][bj-1].p[d] = (1.0*boo[i+1][j-1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j-1].p[d]+4.0*boo[i][j].p[d])/9.0;
       bz[bi-2][bj+2].p[d] = (4.0*boo[i-1][j+1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j+1].p[d]+1.0*boo[i][j].p[d])/9.0;
       bz[bi-2][bj+1].p[d] = (2.0*boo[i-1][j+1].p[d]+4.0*boo[i-1][j].p[d]+1.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi-1][bj+2].p[d] = (2.0*boo[i-1][j+1].p[d]+1.0*boo[i-1][j].p[d]+4.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi-1][bj+1].p[d] = (1.0*boo[i-1][j+1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j+1].p[d]+4.0*boo[i][j].p[d])/9.0;
       bz[bi][bj+2].p[d]   = (bz[bi-1][bj+2].p[d]+bz[bi+1][bj+2].p[d])/2.0;
       bz[bi][bj+1].p[d]   = (bz[bi-1][bj+1].p[d]+bz[bi+1][bj+1].p[d])/2.0;
       bz[bi][bj-1].p[d]   = (bz[bi-1][bj-1].p[d]+bz[bi+1][bj-1].p[d])/2.0;
       bz[bi][bj-2].p[d]   = (bz[bi-1][bj-2].p[d]+bz[bi+1][bj-2].p[d])/2.0;
       bz[bi+2][bj].p[d]   = (bz[bi+2][bj-1].p[d]+bz[bi+2][bj+1].p[d])/2.0;
       bz[bi+1][bj].p[d]   = (bz[bi+1][bj-1].p[d]+bz[bi+1][bj+1].p[d])/2.0;
       bz[bi-1][bj].p[d]   = (bz[bi-1][bj-1].p[d]+bz[bi-1][bj+1].p[d])/2.0;
       bz[bi-2][bj].p[d]   = (bz[bi-2][bj-1].p[d]+bz[bi-2][bj+1].p[d])/2.0;
       bz[bi][bj].p[d]     = (bz[bi+1][bj+1].p[d]+bz[bi-1][bj-1].p[d])/2.0;
     }
}


void update_bezier_2d(struct Vector4D** bz, struct Vector4D** boo, int i, int j, int nox, int noy)
{
 register int bi,bj;
 int d;
 TRACE
 if (i == 0 || j==0 || i==nox-1 || j==noy-1 || !bz || !boo || nox<=0 || noy<=0)  return;
 bi = (i-1)*3+2;
 bj = (j-1)*3+2;
 for (d=0;d<DIM-1;d++)
     {
       bz[bi-2][bj-2].p[d] = (4.0*boo[i-1][j-1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j-1].p[d]+1.0*boo[i][j].p[d])/9.0;
       bz[bi-2][bj-1].p[d] = (2.0*boo[i-1][j-1].p[d]+4.0*boo[i-1][j].p[d]+1.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi-1][bj-2].p[d] = (2.0*boo[i-1][j-1].p[d]+1.0*boo[i-1][j].p[d]+4.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi-1][bj-1].p[d] = (1.0*boo[i-1][j-1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j-1].p[d]+4.0*boo[i][j].p[d])/9.0;
       bz[bi+2][bj+2].p[d] = (4.0*boo[i+1][j+1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j+1].p[d]+1.0*boo[i][j].p[d])/9.0;
       bz[bi+2][bj+1].p[d] = (2.0*boo[i+1][j+1].p[d]+4.0*boo[i+1][j].p[d]+1.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi+1][bj+2].p[d] = (2.0*boo[i+1][j+1].p[d]+1.0*boo[i+1][j].p[d]+4.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi+1][bj+1].p[d] = (1.0*boo[i+1][j+1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j+1].p[d]+4.0*boo[i][j].p[d])/9.0;
       bz[bi+2][bj-2].p[d] = (4.0*boo[i+1][j-1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j-1].p[d]+1.0*boo[i][j].p[d])/9.0;
       bz[bi+2][bj-1].p[d] = (2.0*boo[i+1][j-1].p[d]+4.0*boo[i+1][j].p[d]+1.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi+1][bj-2].p[d] = (2.0*boo[i+1][j-1].p[d]+1.0*boo[i+1][j].p[d]+4.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi+1][bj-1].p[d] = (1.0*boo[i+1][j-1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j-1].p[d]+4.0*boo[i][j].p[d])/9.0;
       bz[bi-2][bj+2].p[d] = (4.0*boo[i-1][j+1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j+1].p[d]+1.0*boo[i][j].p[d])/9.0;
       bz[bi-2][bj+1].p[d] = (2.0*boo[i-1][j+1].p[d]+4.0*boo[i-1][j].p[d]+1.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi-1][bj+2].p[d] = (2.0*boo[i-1][j+1].p[d]+1.0*boo[i-1][j].p[d]+4.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
       bz[bi-1][bj+1].p[d] = (1.0*boo[i-1][j+1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j+1].p[d]+4.0*boo[i][j].p[d])/9.0;
       bz[bi][bj+2].p[d]   = (bz[bi-1][bj+2].p[d]+bz[bi+1][bj+2].p[d])/2.0;
       bz[bi][bj+1].p[d]   = (bz[bi-1][bj+1].p[d]+bz[bi+1][bj+1].p[d])/2.0;
       bz[bi][bj-1].p[d]   = (bz[bi-1][bj-1].p[d]+bz[bi+1][bj-1].p[d])/2.0;
       bz[bi][bj-2].p[d]   = (bz[bi-1][bj-2].p[d]+bz[bi+1][bj-2].p[d])/2.0;
       bz[bi+2][bj].p[d]   = (bz[bi+2][bj-1].p[d]+bz[bi+2][bj+1].p[d])/2.0;
       bz[bi+1][bj].p[d]   = (bz[bi+1][bj-1].p[d]+bz[bi+1][bj+1].p[d])/2.0;
       bz[bi-1][bj].p[d]   = (bz[bi-1][bj-1].p[d]+bz[bi-1][bj+1].p[d])/2.0;
       bz[bi-2][bj].p[d]   = (bz[bi-2][bj-1].p[d]+bz[bi-2][bj+1].p[d])/2.0;
       bz[bi][bj].p[d]     = (bz[bi+1][bj+1].p[d]+bz[bi-1][bj-1].p[d])/2.0;
     }
  bz[bi-2][bj-2].w = (4.0*boo[i-1][j-1].w+2.0*boo[i-1][j].w+2.0*boo[i][j-1].w+1.0*boo[i][j].w)/9.0;
  bz[bi-2][bj-1].w = (2.0*boo[i-1][j-1].w+4.0*boo[i-1][j].w+1.0*boo[i][j-1].w+2.0*boo[i][j].w)/9.0;
  bz[bi-1][bj-2].w = (2.0*boo[i-1][j-1].w+1.0*boo[i-1][j].w+4.0*boo[i][j-1].w+2.0*boo[i][j].w)/9.0;
  bz[bi-1][bj-1].w = (1.0*boo[i-1][j-1].w+2.0*boo[i-1][j].w+2.0*boo[i][j-1].w+4.0*boo[i][j].w)/9.0;
  bz[bi+2][bj+2].w = (4.0*boo[i+1][j+1].w+2.0*boo[i+1][j].w+2.0*boo[i][j+1].w+1.0*boo[i][j].w)/9.0;
  bz[bi+2][bj+1].w = (2.0*boo[i+1][j+1].w+4.0*boo[i+1][j].w+1.0*boo[i][j+1].w+2.0*boo[i][j].w)/9.0;
  bz[bi+1][bj+2].w = (2.0*boo[i+1][j+1].w+1.0*boo[i+1][j].w+4.0*boo[i][j+1].w+2.0*boo[i][j].w)/9.0;
  bz[bi+1][bj+1].w = (1.0*boo[i+1][j+1].w+2.0*boo[i+1][j].w+2.0*boo[i][j+1].w+4.0*boo[i][j].w)/9.0;
  bz[bi+2][bj-2].w = (4.0*boo[i+1][j-1].w+2.0*boo[i+1][j].w+2.0*boo[i][j-1].w+1.0*boo[i][j].w)/9.0;
  bz[bi+2][bj-1].w = (2.0*boo[i+1][j-1].w+4.0*boo[i+1][j].w+1.0*boo[i][j-1].w+2.0*boo[i][j].w)/9.0;
  bz[bi+1][bj-2].w = (2.0*boo[i+1][j-1].w+1.0*boo[i+1][j].w+4.0*boo[i][j-1].w+2.0*boo[i][j].w)/9.0;
  bz[bi+1][bj-1].w = (1.0*boo[i+1][j-1].w+2.0*boo[i+1][j].w+2.0*boo[i][j-1].w+4.0*boo[i][j].w)/9.0;
  bz[bi-2][bj+2].w = (4.0*boo[i-1][j+1].w+2.0*boo[i-1][j].w+2.0*boo[i][j+1].w+1.0*boo[i][j].w)/9.0;
  bz[bi-2][bj+1].w = (2.0*boo[i-1][j+1].w+4.0*boo[i-1][j].w+1.0*boo[i][j+1].w+2.0*boo[i][j].w)/9.0;
  bz[bi-1][bj+2].w = (2.0*boo[i-1][j+1].w+1.0*boo[i-1][j].w+4.0*boo[i][j+1].w+2.0*boo[i][j].w)/9.0;
  bz[bi-1][bj+1].w = (1.0*boo[i-1][j+1].w+2.0*boo[i-1][j].w+2.0*boo[i][j+1].w+4.0*boo[i][j].w)/9.0;
  bz[bi][bj+2].w   = (bz[bi-1][bj+2].w+bz[bi+1][bj+2].w)/2.0;
  bz[bi][bj+1].w   = (bz[bi-1][bj+1].w+bz[bi+1][bj+1].w)/2.0;
  bz[bi][bj-1].w   = (bz[bi-1][bj-1].w+bz[bi+1][bj-1].w)/2.0;
  bz[bi][bj-2].w   = (bz[bi-1][bj-2].w+bz[bi+1][bj-2].w)/2.0;
  bz[bi+2][bj].w   = (bz[bi+2][bj-1].w+bz[bi+2][bj+1].w)/2.0;
  bz[bi+1][bj].w   = (bz[bi+1][bj-1].w+bz[bi+1][bj+1].w)/2.0;
  bz[bi-1][bj].w   = (bz[bi-1][bj-1].w+bz[bi-1][bj+1].w)/2.0;
  bz[bi-2][bj].w   = (bz[bi-2][bj-1].w+bz[bi-2][bj+1].w)/2.0;
  bz[bi][bj].w     = (bz[bi+1][bj+1].w+bz[bi-1][bj-1].w)/2.0;
}

		/*FIXME weight deBoor <-> Bezier transformations aren't ready for curves */
		/* but they are ready for Nurbs */

void bezier_from_deboor_2d(struct Polygon4D** list, struct Vector4D*** bz, struct Vector4D** boo, int* nzx, int* nzy, int nox, int noy)
{
 int x,y,i,j;
 TRACE
 if (nox <= 0 || noy <=0 || !boo) return;
 list_free(list);
 x = nox - 3;
 y = noy - 3;
 x = (3*x)+5;
 y = (3*y)+5;
 *nzx = x;
 *nzy = y;
 *bz = (struct Vector4D**)malloc(x*sizeof(struct Vector4D*));
 for (i=0;i<x;i++) (*bz)[i] = (struct Vector4D*)malloc(y*sizeof(struct Vector4D));
 for (i=0;i<nox;i++) for(j=0;j<noy;j++) update_bezier_2d(*bz, boo, i, j, nox, noy);
 			/* FIXME THIS doesnt't set p[DIM-1] = v for speed, so we must use list_add_point5() next */
/* for (i=2;i<x-2;i++) for(j=2;j<y-2;j++) list_add_point(list, (*bz)[i][j].p[0], (*bz)[i][j].p[1], (*bz)[i][j].p[2]);*/
 for (i=2;i<x-2;i++) for(j=2;j<y-2;j++) list_add_point5(list, (*bz)[i][j].p[0], (*bz)[i][j].p[1], (*bz)[i][j].p[2], 1., (*bz)[i][j].w);
}


void default_vec_array(struct Vector4D** va, int x, int y)
{
 register int i,j;
 TRACE
 if (!va) return;
 for (i=0;i<x;i++) for (j=0;j<y;j++)
   {
    va[i][j].p[0] = va[i][j].p[1] = va[i][j].p[2] = 0.;
    va[i][j].p[3] = va[i][j].w = 1.;
   }
}


void bezier_from_deboor_2da(struct Vector4D*** bz, struct Vector4D** boo, int* nzx, int* nzy, int nox, int noy)
{
 int x,y,i,j;
 TRACE
 if (nox <= 0 || noy <=0 || !boo) return;
 x = nox - 3;
 y = noy - 3;
 x = (3*x)+7;
 y = (3*y)+7;
 *nzx = x;
 *nzy = y;
 *bz = (struct Vector4D**)malloc(x*sizeof(struct Vector4D*));
 for (i=0;i<x;i++) (*bz)[i] = (struct Vector4D*)malloc(y*sizeof(struct Vector4D));
 for (i=0;i<x;i++) for(j=0;j<y;j++) (*bz)[i][j].p[3] = (*bz)[i][j].w = 1. ;
 					    /* HERE initially must compute foreach deBoor */
 					    /* then only every 4th */
 for (i=1;i<nox-1;i++) for(j=1;j<noy-1;j++) update_bezier_2d_fast(*bz, boo, i, j, nox, noy);
}


struct Vector4D* get_point_nearesta3(struct Vector4D*** arr, int C, int X, int Y, struct Cursor4D* cursor, int *sc, int* sx, int *sy)
{
 double x,y,z,min,dis;
 int i,j,c;
 struct Vector4D* minptr;
 TRACE
 if (arr == NULL) return NULL;
 minptr = NULL;
 get_cursor_pos(cursor, &x, &y, &z);
 min = 19e19;
 for (c=0;c<C;c++)
 for (i=0;i<X;i++)
 for (j=0;j<Y;j++)
   {
    dis = distance_to_cursordv_fast(&arr[c][i][j], x, y, z);
    if (dis < min)
      {
       min = dis;
       minptr = &arr[c][i][j];
       *sx = i;
       *sy = j;
       *sc = c;
      }
   }
 return minptr;
}


struct Vector4D* get_point_nearesta(struct Vector4D** arr, int X, int Y, struct Cursor4D* cursor, int* sx, int *sy)
{
 double x,y,z,min,dis;
 int i,j;
 struct Vector4D* minptr;
 TRACE
 if (arr == NULL) return NULL;
 minptr = NULL;
 get_cursor_pos(cursor, &x, &y, &z);
 min = 19e19;
 for (i=0;i<X;i++)
 for (j=0;j<Y;j++)
   {
    dis = distance_to_cursordv_fast(&arr[i][j], x, y, z);
    if (dis < min)
      {
       min = dis;
       minptr = &arr[i][j];
       *sx = i;
       *sy = j;
      }
   }
 return minptr;
}


void get_C2a_nm(struct Vector4D** bez, int nbx, int nby, struct Vector4D**** spl, int n, int m)
{
 int i,j;
 register int k;
 TRACE
 *spl = NULL;
 if (nbx < 10 || nby < 10 || !bez || n < 1 || m < 1) return;
 *spl = (struct Vector4D***)malloc(n*sizeof(struct Vector4D**));
 for (i=0;i<n;i++) (*spl)[i] = (struct Vector4D**)malloc(m*sizeof(struct Vector4D*));
 for (i=0;i<n;i++) for (j=0;j<m;j++) (*spl)[i][j] = (struct Vector4D*)malloc(0x10*sizeof(struct Vector4D));
 for (i=0;i<n;i++) 
 for (j=0;j<m;j++) 
 for (k=0;k<16;k++)
   {
/*    printf("i=%d, j=%d, k=%d [%d][%d]\n", i,j,k, ((3*i)+(k%4)+3), ((3*j)+(k/4)+3));*/
/*    printf_vector("v", &bez[(3*i)+(k%4)+3][(3*j)+(k/4)+3]);*/
    copy_vector(&((*spl)[i][j][k]), &bez[(3*i)+(k%4)+3][(3*j)+(k/4)+3]);
   }
}


void get_control_points(struct Vector4D** bez, int nbx, int nby, struct Vector4D** spl, int i, int j)
{
 register int k;
 TRACE
 *spl = NULL;
 if (nbx < 10 || nby < 10 || !bez) return;
 *spl = (struct Vector4D*)malloc(0x10*sizeof(struct Vector4D));
 for (k=0;k<16;k++) copy_vector(&((*spl)[k]), &bez[(3*i)+(k%4)+3][(3*j)+(k/4)+3]);
}


void get_2d2vec_gp(struct Vector4D** bez, struct Vector4D** aux, struct Vector4D** spl)
{
 register int i,j;
 TRACE
 if (!bez) return;
 *spl = NULL;
 *spl = (struct Vector4D*)malloc(0x10*sizeof(struct Vector4D));
 for (i=0;i<4;i++) for (j=0;j<4;j++)
   {
    if ((i == 1 || i == 2) && (j == 1 || j == 2))
      {
       aux[i-1][j-1].p[3] = 1.;
       aux[i-1][j-1].w    = 1.;
       copy_vector(&((*spl)[i*4+j]), &aux[i-1][j-1]);
/*       printf("aux copy: %d, %d\n", i, j);*/
/*       printf_vector("aux", &((*spl)[i*4+j]));*/
      }
    else
      {
       copy_vector(&((*spl)[i*4+j]), &bez[i][j]);
      }
   }
}


void get_2d2vec(struct Vector4D** bez, struct Vector4D** spl)
{
 register int i,j;
 TRACE
 if (!bez) return;
 *spl = NULL;
 *spl = (struct Vector4D*)malloc(0x10*sizeof(struct Vector4D));
 for (i=0;i<4;i++) for (j=0;j<4;j++) copy_vector(&((*spl)[i*4+j]), &bez[i][j]);
}


void bezier_from_deboor_idx_2da(struct Vector4D** bz, struct Vector4D** boo, int nox, int noy, int i, int j)
{
 register int bi,bj;
 int d;
 TRACE
 if (!bz || !boo || nox<=0 || noy<=0)  return;
 bi = (i-1)*3+3;
 bj = (j-1)*3+3;
 for (d=0;d<DIM-1;d++)
     {
       if (i>0 && j>0)
         {
          bz[bi-2][bj-2].p[d] = (4.0*boo[i-1][j-1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j-1].p[d]+1.0*boo[i][j].p[d])/9.0;
          bz[bi-2][bj-1].p[d] = (2.0*boo[i-1][j-1].p[d]+4.0*boo[i-1][j].p[d]+1.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
          bz[bi-1][bj-2].p[d] = (2.0*boo[i-1][j-1].p[d]+1.0*boo[i-1][j].p[d]+4.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
          bz[bi-1][bj-1].p[d] = (1.0*boo[i-1][j-1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j-1].p[d]+4.0*boo[i][j].p[d])/9.0;
	 }
       if (i<nox-1 && j<noy-1)
         {
          bz[bi+2][bj+2].p[d] = (4.0*boo[i+1][j+1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j+1].p[d]+1.0*boo[i][j].p[d])/9.0;
          bz[bi+2][bj+1].p[d] = (2.0*boo[i+1][j+1].p[d]+4.0*boo[i+1][j].p[d]+1.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
          bz[bi+1][bj+2].p[d] = (2.0*boo[i+1][j+1].p[d]+1.0*boo[i+1][j].p[d]+4.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
          bz[bi+1][bj+1].p[d] = (1.0*boo[i+1][j+1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j+1].p[d]+4.0*boo[i][j].p[d])/9.0;
	 }
       if (i<nox-1 && j>0)
         {
          bz[bi+2][bj-2].p[d] = (4.0*boo[i+1][j-1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j-1].p[d]+1.0*boo[i][j].p[d])/9.0;
          bz[bi+2][bj-1].p[d] = (2.0*boo[i+1][j-1].p[d]+4.0*boo[i+1][j].p[d]+1.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
          bz[bi+1][bj-2].p[d] = (2.0*boo[i+1][j-1].p[d]+1.0*boo[i+1][j].p[d]+4.0*boo[i][j-1].p[d]+2.0*boo[i][j].p[d])/9.0;
          bz[bi+1][bj-1].p[d] = (1.0*boo[i+1][j-1].p[d]+2.0*boo[i+1][j].p[d]+2.0*boo[i][j-1].p[d]+4.0*boo[i][j].p[d])/9.0;
	 }
       if (i>0 && j<noy-1)
         {
          bz[bi-2][bj+2].p[d] = (4.0*boo[i-1][j+1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j+1].p[d]+1.0*boo[i][j].p[d])/9.0;
          bz[bi-2][bj+1].p[d] = (2.0*boo[i-1][j+1].p[d]+4.0*boo[i-1][j].p[d]+1.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
          bz[bi-1][bj+2].p[d] = (2.0*boo[i-1][j+1].p[d]+1.0*boo[i-1][j].p[d]+4.0*boo[i][j+1].p[d]+2.0*boo[i][j].p[d])/9.0;
          bz[bi-1][bj+1].p[d] = (1.0*boo[i-1][j+1].p[d]+2.0*boo[i-1][j].p[d]+2.0*boo[i][j+1].p[d]+4.0*boo[i][j].p[d])/9.0;
	 }
       if (j<noy-1 && i>0 && i<nox-1)
         {
          bz[bi][bj+2].p[d]   = (bz[bi-1][bj+2].p[d]+bz[bi+1][bj+2].p[d])/2.0;
          bz[bi][bj+1].p[d]   = (bz[bi-1][bj+1].p[d]+bz[bi+1][bj+1].p[d])/2.0;
	 }
       if (j>0 && i>0 && i<nox-1)
         {
          bz[bi][bj-1].p[d]   = (bz[bi-1][bj-1].p[d]+bz[bi+1][bj-1].p[d])/2.0;
          bz[bi][bj-2].p[d]   = (bz[bi-1][bj-2].p[d]+bz[bi+1][bj-2].p[d])/2.0;
	 }
       if (i<nox-1 && j>0 && j<noy-1)
         {
          bz[bi+2][bj].p[d]   = (bz[bi+2][bj-1].p[d]+bz[bi+2][bj+1].p[d])/2.0;
          bz[bi+1][bj].p[d]   = (bz[bi+1][bj-1].p[d]+bz[bi+1][bj+1].p[d])/2.0;
	 }
       if (i>0 && j>0 && j<noy-1)
         {
          bz[bi-1][bj].p[d]   = (bz[bi-1][bj-1].p[d]+bz[bi-1][bj+1].p[d])/2.0;
          bz[bi-2][bj].p[d]   = (bz[bi-2][bj-1].p[d]+bz[bi-2][bj+1].p[d])/2.0;
	 }
       if (i>0 && j>0 && i<nox-1 && j<noy-1) bz[bi][bj].p[d] = (bz[bi+1][bj+1].p[d]+bz[bi-1][bj-1].p[d])/2.0;
       if (i>1 && j>0 && j<noy-1)
         {
          bz[bi-3][bj-2].p[d] = (bz[bi-4][bj-2].p[d]+bz[bi-2][bj-2].p[d])/2.0;
          bz[bi-3][bj-1].p[d] = (bz[bi-4][bj-1].p[d]+bz[bi-2][bj-1].p[d])/2.0;
          bz[bi-3][bj].p[d]   = (bz[bi-4][bj].p[d]+bz[bi-2][bj].p[d])/2.0;
          bz[bi-3][bj+1].p[d] = (bz[bi-4][bj+1].p[d]+bz[bi-2][bj+1].p[d])/2.0;
          bz[bi-3][bj+2].p[d] = (bz[bi-4][bj+2].p[d]+bz[bi-2][bj+2].p[d])/2.0;
	 }
       if (i<nox-2 && j>0 && j<noy-1)
         {
          bz[bi+3][bj-2].p[d] = (bz[bi+4][bj-2].p[d]+bz[bi+2][bj-2].p[d])/2.0;
          bz[bi+3][bj-1].p[d] = (bz[bi+4][bj-1].p[d]+bz[bi+2][bj-1].p[d])/2.0;
          bz[bi+3][bj].p[d]   = (bz[bi+4][bj].p[d]+bz[bi+2][bj].p[d])/2.0;
          bz[bi+3][bj+1].p[d] = (bz[bi+4][bj+1].p[d]+bz[bi+2][bj+1].p[d])/2.0;
          bz[bi+3][bj+2].p[d] = (bz[bi+4][bj+2].p[d]+bz[bi+2][bj+2].p[d])/2.0;
	 }
       if (j>1 && i>0 && i<nox-1)
         {
          bz[bi-2][bj-3].p[d] = (bz[bi-2][bj-4].p[d]+bz[bi-2][bj-2].p[d])/2.0;
          bz[bi-1][bj-3].p[d] = (bz[bi-1][bj-4].p[d]+bz[bi-1][bj-2].p[d])/2.0;
          bz[bi][bj-3].p[d]   = (bz[bi][bj-4].p[d]+bz[bi][bj-2].p[d])/2.0;
          bz[bi+1][bj-3].p[d] = (bz[bi+1][bj-4].p[d]+bz[bi+1][bj-2].p[d])/2.0;
          bz[bi+2][bj-3].p[d] = (bz[bi+2][bj-4].p[d]+bz[bi+2][bj-2].p[d])/2.0;
	 }
       if (j<noy-2 && i>0 && i<nox-1)
         {
          bz[bi-2][bj+3].p[d] = (bz[bi-2][bj+4].p[d]+bz[bi-2][bj+2].p[d])/2.0;
          bz[bi-1][bj+3].p[d] = (bz[bi-1][bj+4].p[d]+bz[bi-1][bj+2].p[d])/2.0;
          bz[bi][bj+3].p[d]   = (bz[bi][bj+4].p[d]+bz[bi][bj+2].p[d])/2.0;
          bz[bi+1][bj+3].p[d] = (bz[bi+1][bj+4].p[d]+bz[bi+1][bj+2].p[d])/2.0;
          bz[bi+2][bj+3].p[d] = (bz[bi+2][bj+4].p[d]+bz[bi+2][bj+2].p[d])/2.0;
	 }
       if (i<nox-2 && j<noy-2) bz[bi+3][bj+3].p[d] = (bz[bi+2][bj+2].p[d]+bz[bi+4][bj+4].p[d])/2.0;
       if (i>1 && j<noy-2)     bz[bi-3][bj+3].p[d] = (bz[bi-2][bj+2].p[d]+bz[bi-4][bj+4].p[d])/2.0;
       if (i<nox-2 && j>1)     bz[bi+3][bj-3].p[d] = (bz[bi+2][bj-2].p[d]+bz[bi+4][bj-4].p[d])/2.0;
       if (i>1 && j>1)         bz[bi-3][bj-3].p[d] = (bz[bi-2][bj-2].p[d]+bz[bi-4][bj-4].p[d])/2.0;
     }
}


void calculate_dcp(struct Vector4D** patch, double t, struct Vector4D** first, struct Vector4D** second)
{
 int i, j, k;
 struct Vector4D p[4];
 struct Vector4D q[4];
 struct Vector4D tempTable1[4][4];
 struct Vector4D tempTable2[4][4];
 TRACE
/* printf("calculate_dcp: move me to library!\n");*/
 k=0;
 for(k=0;k<4;k++)
  {
   for (i=0;i<4;i++) copy_vector(&p[i], &patch[k][i]);
   copy_vector(&q[0], &p[0]);
   for (j=1;j<4;j++)
     {
      for(i=0;i<4-j;i++)
	{
	 p[i].p[0] = (1-t)*p[i].p[0]+t*p[i+1].p[0];
	 p[i].p[1] = (1-t)*p[i].p[1]+t*p[i+1].p[1];
	 p[i].p[2] = (1-t)*p[i].p[2]+t*p[i+1].p[2];
	}
      copy_vector(&q[j], &p[0]);
    }
    for(i=0;i<4;i++)
     {
      copy_vector(&tempTable1[k][i], &q[i]);
      copy_vector(&tempTable2[k][i], &p[i]);
     }
  }
 for (i=0;i<4;i++)
 for (j=0;j<4;j++)
  {
    copy_vector(&first[i][j],  &tempTable1[i][j]);
    copy_vector(&second[i][j], &tempTable2[i][j]);
  }
}


int innormal_point(struct Vector4D* pt)
{
 if (pt->p[DIM-1] == 0. || pt->w == 0.) return 1;
 else return 0;
}


void calc_gp(struct Nurb** gnurbs, struct Vector4D*** bez, struct Vector4D**** bp1, struct Vector4D**** bp2, int lpsegm)
{
 struct Vector4D oldpts[3];
 struct Vector4D P0[3];
 struct Vector4D P1[3];
 struct Vector4D P2[3];
 struct Vector4D Q[3];
 struct Vector4D P;
 struct Vector4D*** tt1;
 struct Vector4D*** tt2;
 struct Vector4D tempVal, g0, g3;
 int i,j,k;
 char str[128];
 struct Vector4D* control_points;
 struct Vector4D*** dcp;
 TRACE
 if (*bp1 && *bp2)
 {
  for (i=0;i<3;i++)
  for (j=0;j<4;j++) free((*bp1)[i][j]);
  for (i=0;i<3;i++) free((*bp1)[i]);
  free(*bp1);
  for (i=0;i<3;i++)
  for (j=0;j<2;j++) free((*bp2)[i][j]);
  for (i=0;i<3;i++) free((*bp2)[i]);
  free(*bp2);
 }
 copy_vector(&oldpts[2], &bez[2][0][0]);
 copy_vector(&oldpts[0], &bez[1][0][3]);
 copy_vector(&oldpts[1], &bez[2][0][3]);
/* printf_vector("oldpts[0]", &oldpts[0]);*/
/* printf_vector("oldpts[1]", &oldpts[1]);*/
/* printf_vector("oldpts[2]", &oldpts[2]);*/
 tt1 = (struct Vector4D***)malloc(3*sizeof(struct Vector4D**));
 for (i=0;i<3;i++) tt1[i]  = (struct Vector4D**)malloc(4*sizeof(struct Vector4D*));
 for (i=0;i<3;i++)
 for (j=0;j<4;j++) tt1[i][j]  = (struct Vector4D*)malloc(4*sizeof(struct Vector4D));
 tt2 = (struct Vector4D***)malloc(3*sizeof(struct Vector4D**));
 for (i=0;i<3;i++) tt2[i]  = (struct Vector4D**)malloc(2*sizeof(struct Vector4D*));
 for (i=0;i<3;i++)
 for (j=0;j<2;j++) tt2[i][j]  = (struct Vector4D*)malloc(2*sizeof(struct Vector4D));
 dcp = (struct Vector4D***)malloc(6*sizeof(struct Vector4D**));
 for (i=0;i<6;i++) dcp[i]  = (struct Vector4D**)malloc(4*sizeof(struct Vector4D*));
 for (i=0;i<6;i++)
 for (j=0;j<4;j++) dcp[i][j]  = (struct Vector4D*)malloc(4*sizeof(struct Vector4D));
/* printf("FREE!!! dcp!!!\n");*/
 for (i=0;i<3;i++) calculate_dcp(bez[i], .5, dcp[i*2], dcp[i*2+1]);
 for (i=0;i<6;i++)
 for (j=0;j<4;j++)
 for (k=0;k<4;k++)
   {
    sprintf(str, "dcp[%d][%d][%d]", i, j, k);
/*    printf_vector(str, &dcp[i][j][k]);*/
/*    render_point_bez(&dcp[i][j][k]);*/
   }
 for (i=0;i<3;i++) copy_vector(&P0[i], &dcp[i*2][0][3]);
 for (i=0;i<3;i++)
   {
    sprintf(str, "P0[%d]", i);
/*    printf_vector(str, &P0[i]);*/
/*    render_point_bez(&P0[i]);*/
   }
 for (i=0;i<3;i++)
   {
    construct_vector3(&P1[i],(3*P0[i].p[0]-dcp[i*2][1][3].p[0])/2.,(3*P0[i].p[1]-dcp[i*2][1][3].p[1])/2.,(3*P0[i].p[2]-dcp[i*2][1][3].p[2])/2.);
/*    render_point_bez(&P1[i]);*/
   }
 for (i=0;i<3;i++)
   {
    construct_vector3(&Q[i],(3*P1[i].p[0]-P0[i].p[0])/2.,(3*P1[i].p[1]-P0[i].p[1])/2.,(3*P1[i].p[2]-P0[i].p[2])/2.);
/*    render_point_bez(&Q[i]);*/
   }
 construct_vector3(&P, (Q[0].p[0]+Q[1].p[0]+Q[2].p[0])/3.,(Q[0].p[1]+Q[1].p[1]+Q[2].p[1])/3.,(Q[0].p[2]+Q[1].p[2]+Q[2].p[2])/3.);
/* render_point_bez(&P);*/
 for (i=0;i<3;i++)
   {
    construct_vector3(&P2[i],(2*Q[i].p[0]+P.p[0])/3.,(2*Q[i].p[1]+P.p[1])/3.,(2*Q[i].p[2]+P.p[2])/3.);
/*    render_point_bez(&P2[i]);*/
   }
/* printf("FREE!!! dcp!!!\n");*/
 for(i=0;i<3;i++)
  {
    construct_vector3(&tt1[i][0][0], P0[i].p[0], P0[i].p[1], P0[i].p[2]);
    construct_vector3(&tt1[i][0][3], P.p[0], P.p[1], P.p[2]);
    construct_vector3(&tt1[i][3][3], P0[(i+1)%3].p[0], P0[(i+1)%3].p[1], P0[(i+1)%3].p[2]);
    construct_vector3(&tt1[i][3][0], oldpts[i].p[0], oldpts[i].p[1], oldpts[i].p[2]);
    construct_vector3(&tt1[i][1][0], dcp[i*2][0][2].p[0],dcp[i*2][0][2].p[1],dcp[i*2][0][2].p[2]);
    construct_vector3(&tt1[i][2][0], dcp[i*2][0][1].p[0],dcp[i*2][0][1].p[1],dcp[i*2][0][1].p[2]);
    construct_vector3(&tt1[i][3][1], dcp[(i+1)%3*2+1][0][2].p[0],dcp[(i+1)%3*2+1][0][2].p[1],dcp[(i+1)%3*2+1][0][2].p[2]);
    construct_vector3(&tt1[i][3][2], dcp[(i+1)%3*2+1][0][1].p[0],dcp[(i+1)%3*2+1][0][1].p[1],dcp[(i+1)%3*2+1][0][1].p[2]);
    construct_vector3(&tt1[i][0][1], P1[i].p[0],P1[i].p[1],P1[i].p[2]);
    construct_vector3(&tt1[i][0][2], P2[i].p[0],P2[i].p[1],P2[i].p[2]);
    construct_vector3(&tt1[i][1][3], P2[(i+1)%3].p[0],P2[(i+1)%3].p[1],P2[(i+1)%3].p[2]);
    construct_vector3(&tt1[i][2][3], P1[(i+1)%3].p[0],P1[(i+1)%3].p[1],P1[(i+1)%3].p[2]);
    construct_vector3(&g0, (tt1[i][0][1].p[0]-dcp[i*2][1][3].p[0]/2.)/2.,(tt1[i][0][1].p[1]-dcp[i*2][1][3].p[1]/2.)/2.,(tt1[i][0][1].p[2]-dcp[i*2][1][3].p[2]/2.)/2.);
    construct_vector3(&g3, (tt1[i][3][1].p[0]-dcp[i*2][1][0].p[0]/2.)/2.,(tt1[i][3][1].p[1]-dcp[i*2][1][0].p[1]/2.)/2.,(tt1[i][3][1].p[2]-dcp[i*2][1][0].p[2]/2.)/2.);
    tempVal.p[0] = tt1[i][1][0].p[0]+1./3.*g0.p[0]+2./3.*g3.p[0];
    tempVal.p[1] = tt1[i][1][0].p[1]+1./3.*g0.p[1]+2./3.*g3.p[1];	/* ? */
    tempVal.p[2] = tt1[i][1][0].p[2]+1./3.*g0.p[2]+2./3.*g3.p[2];
    copy_vector(&tt1[i][1][1], &tempVal);
    tempVal.p[0] = tt1[i][2][0].p[0]+2./3.*g0.p[0]+1./3.*g3.p[0];
    tempVal.p[1] = tt1[i][2][0].p[1]+2./3.*g0.p[1]+1./3.*g3.p[1];
    tempVal.p[2] = tt1[i][2][0].p[2]+2./3.*g0.p[2]+1./3.*g3.p[2];
    copy_vector(&tt1[i][2][1], &tempVal);
    g0.p[0] = (tt1[i][1][0].p[0]-dcp[(i+1)%3*2+1][1][3].p[0]/2.)/2.;
    g0.p[1] = (tt1[i][1][0].p[1]-dcp[(i+1)%3*2+1][1][3].p[1]/2.)/2.;
    g0.p[2] = (tt1[i][1][0].p[2]-dcp[(i+1)%3*2+1][1][3].p[2]/2.)/2.;
    g3.p[0] = (tt1[i][2][3].p[0]-dcp[(i+1)%3*2+1][1][0].p[0]/2.)/2.;
    g3.p[1] = (tt1[i][2][3].p[1]-dcp[(i+1)%3*2+1][1][0].p[1]/2.)/2.;
    g3.p[2] = (tt1[i][2][3].p[2]-dcp[(i+1)%3*2+1][1][0].p[2]/2.)/2.;
    tempVal.p[0] = tt1[i][3][1].p[0]+1./3.*g0.p[0]+2./3.*g3.p[0];
    tempVal.p[1] = tt1[i][3][1].p[1]+1./3.*g0.p[1]+2./3.*g3.p[1];
    tempVal.p[2] = tt1[i][3][1].p[2]+1./3.*g0.p[2]+2./3.*g3.p[2];
    copy_vector(&tt2[i][1][0], &tempVal);
    tempVal.p[0] = tt1[i][3][2].p[0]+2./3.*g0.p[0]+1./3.*g3.p[0];
    tempVal.p[1] = tt1[i][3][2].p[1]+2./3.*g0.p[1]+1./3.*g3.p[1];
    tempVal.p[2] = tt1[i][3][2].p[2]+2./3.*g0.p[2]+1./3.*g3.p[2];
    copy_vector(&tt2[i][1][1], &tempVal);
    g0.p[0] = (tt1[i][3][2].p[0]-dcp[(i+1)%3*2][0][2].p[0])/2.;
    g0.p[1] = (tt1[i][3][2].p[1]-dcp[(i+1)%3*2][0][2].p[1])/2.;
    g0.p[2] = (tt1[i][3][2].p[2]-dcp[(i+1)%3*2][0][2].p[2])/2.;
    g3.p[0] = (P2[i].p[0]-tt1[i][0][3].p[0])/2.;
    g3.p[1] = (P2[i].p[1]-tt1[i][0][3].p[1])/2.;
    g3.p[2] = (P2[i].p[2]-tt1[i][0][3].p[2])/2.;
    tempVal.p[0] = tt1[i][2][3].p[0]+1./3.*g0.p[0]+2./3.*g3.p[0];
    tempVal.p[1] = tt1[i][2][3].p[1]+1./3.*g0.p[1]+2./3.*g3.p[1];
    tempVal.p[2] = tt1[i][2][3].p[2]+1./3.*g0.p[2]+2./3.*g3.p[2];
    copy_vector(&tt1[i][2][2], &tempVal);
    tempVal.p[0] = tt1[i][1][3].p[0]+2./3.*g0.p[0]+1./3.*g3.p[0];
    tempVal.p[1] = tt1[i][1][3].p[1]+2./3.*g0.p[1]+1./3.*g3.p[1];
    tempVal.p[2] = tt1[i][1][3].p[2]+2./3.*g0.p[2]+1./3.*g3.p[2];
    copy_vector(&tt1[i][1][2], &tempVal);
    g0.p[0] = (P2[(i+1)%3].p[0]-tt1[i][0][3].p[0])/2.;
    g0.p[1] = (P2[(i+1)%3].p[1]-tt1[i][0][3].p[1])/2.;
    g0.p[2] = (P2[(i+1)%3].p[2]-tt1[i][0][3].p[2])/2.;
    g3.p[0] = (tt1[i][1][0].p[0]-dcp[i*2+1][0][1].p[0])/2.;
    g3.p[1] = (tt1[i][1][0].p[1]-dcp[i*2+1][0][1].p[1])/2.;
    g3.p[2] = (tt1[i][1][0].p[2]-dcp[i*2+1][0][1].p[2])/2.;
    tempVal.p[0] = tt1[i][0][2].p[0]+1./3.*g0.p[0]+2./3.*g3.p[0];
    tempVal.p[1] = tt1[i][0][2].p[1]+1./3.*g0.p[1]+2./3.*g3.p[1];
    tempVal.p[2] = tt1[i][0][2].p[2]+1./3.*g0.p[2]+2./3.*g3.p[2];
    copy_vector(&tt2[i][0][1], &tempVal);
    tempVal.p[0] = tt1[i][0][1].p[0]+2./3.*g0.p[0]+1./3.*g3.p[0];
    tempVal.p[1] = tt1[i][0][1].p[1]+2./3.*g0.p[1]+1./3.*g3.p[1];
    tempVal.p[2] = tt1[i][0][1].p[2]+2./3.*g0.p[2]+1./3.*g3.p[2];
    copy_vector(&tt2[i][0][0], &tempVal);
   }
 for (k=0;k<3;k++) for (i=0;i<4;i++) for (j=0;j<4;j++)
 {
  if (innormal_point(&tt1[k][i][j]))
    {
     sprintf(str, "tt1[%d][%d][%d]", k,i,j);
     tt1[k][i][j].p[DIM-1] = 1.;
/*     printf_vector(str, &tt1[k][i][j]);*/
    }
 }
 for (k=0;k<3;k++) for (i=0;i<2;i++) for (j=0;j<2;j++)
 {
  if (innormal_point(&tt2[k][i][j]))
    {
     sprintf(str, "tt2[%d][%d][%d]", k,i,j);
     tt2[k][i][j].p[DIM-1] = 1.;
/*     printf_vector(str, &tt2[k][i][j]);*/
    }
 }
 for (i=0;i<3;i++)
   {
    get_2d2vec_gp(tt1[i], tt2[i], &control_points);
    nurb_set(gnurbs[i], 3, 3, 3, 3, lpsegm, lpsegm, control_points);
    free(control_points);
   }
 /*for (i=0;i<3;i++)
 for (j=0;j<4;j++) free(tt1[i][j]);
 for (i=0;i<3;i++) free(tt1[i]);
 free(tt1);
 for (i=0;i<3;i++)
 for (j=0;j<2;j++) free(tt2[i][j]);
 for (i=0;i<3;i++) free(tt2[i]);
 free(tt2);*/
 *bp1 = tt1;
 *bp2 = tt2;
 for (i=0;i<6;i++)
 for (j=0;j<4;j++) free(dcp[i][j]);
 for (i=0;i<6;i++) free(dcp[i]);
 free(dcp);
}


int get_point_idx_a3(int c, int x, int y)
{
 TRACE
 return y+4*x+16*c;
}


struct Vector4D* get_point_a3(struct Vector4D*** bz, int idx)
{
 TRACE
 return &(bz[idx/16][(idx/4)%4][idx%4]);
}


void make_C1(struct Vector4D*** bz, int c, int x, int y)
{
 int id;
/* printf("(c,x,y) = (%d,%d,%d), id = %d\n", c,x,y,id);*/
 struct Vector4D *aPoint, *tPoint, *a2Point, *t2Point, tempPS;
 TRACE
 id = get_point_idx_a3(c, x, y);
 if (c < 0 || x < 0 || y < 0) return;
 copy_vector(&tempPS, &bz[c][x][y]);
/* printf_vector("tempPS", &tempPS);*/
 switch(id)
	{
	 case 0:
	        a2Point = get_point_a3(bz, 19);	/* drugi punkt oznaczajacy ten sam naroznik */
	        t2Point = get_point_a3(bz, 1);  /* punkt na trojkacie */
	        copy_vector(a2Point, &tempPS);  /* przesuniecie zdublowanego naroznika */
		aPoint = get_point_a3(bz, 23);  /* przesuwany punkt poza gp */
		aPoint->p[0] = 2*a2Point->p[0] - t2Point->p[0];
		aPoint->p[1] = 2*a2Point->p[1] - t2Point->p[1];
		aPoint->p[2] = 2*a2Point->p[2] - t2Point->p[2];
		break;
	 case 19:
	        a2Point = get_point_a3(bz, 0);
	        t2Point = get_point_a3(bz, 1);
	        copy_vector(a2Point, &tempPS);
		aPoint = get_point_a3(bz, 23);
		aPoint->p[0] = 2*a2Point->p[0] - t2Point->p[0];
		aPoint->p[1] = 2*a2Point->p[1] - t2Point->p[1];
		aPoint->p[2] = 2*a2Point->p[2] - t2Point->p[2];
		break;
	 case 3:
	        a2Point = get_point_a3(bz, 32);	/* drugi punkt oznaczajacy ten sam naroznik */
	        t2Point = get_point_a3(bz, 2);  /* punkt na trojkacie */
	        copy_vector(a2Point, &tempPS);  /* przesuniecie zdublowanego naroznika */
		aPoint = get_point_a3(bz, 36);  /* przesuwany punkt poza gp */
		aPoint->p[0] = 2*a2Point->p[0] - t2Point->p[0];
		aPoint->p[1] = 2*a2Point->p[1] - t2Point->p[1];
		aPoint->p[2] = 2*a2Point->p[2] - t2Point->p[2];
		break;
	 case 32:
	        a2Point = get_point_a3(bz, 3);	/* drugi punkt oznaczajacy ten sam naroznik */
	        t2Point = get_point_a3(bz, 2);  /* punkt na trojkacie */
	        copy_vector(a2Point, &tempPS);  /* przesuniecie zdublowanego naroznika */
		aPoint = get_point_a3(bz, 36);  /* przesuwany punkt poza gp */
		aPoint->p[0] = 2*a2Point->p[0] - t2Point->p[0];
		aPoint->p[1] = 2*a2Point->p[1] - t2Point->p[1];
		aPoint->p[2] = 2*a2Point->p[2] - t2Point->p[2];
		break;
	 case 16:
	        a2Point = get_point_a3(bz, 35);	/* drugi punkt oznaczajacy ten sam naroznik */
	        t2Point = get_point_a3(bz, 34);  /* punkt na trojkacie */
	        copy_vector(a2Point, &tempPS);  /* przesuniecie zdublowanego naroznika */
		aPoint = get_point_a3(bz, 20);  /* przesuwany punkt poza gp */
		aPoint->p[0] = 2*a2Point->p[0] - t2Point->p[0];
		aPoint->p[1] = 2*a2Point->p[1] - t2Point->p[1];
		aPoint->p[2] = 2*a2Point->p[2] - t2Point->p[2];
		break;
	 case 35:
	        a2Point = get_point_a3(bz, 16);	/* drugi punkt oznaczajacy ten sam naroznik */
	        t2Point = get_point_a3(bz, 34);  /* punkt na trojkacie */
	        copy_vector(a2Point, &tempPS);  /* przesuniecie zdublowanego naroznika */
		aPoint = get_point_a3(bz, 20);  /* przesuwany punkt poza gp */
		aPoint->p[0] = 2*a2Point->p[0] - t2Point->p[0];
		aPoint->p[1] = 2*a2Point->p[1] - t2Point->p[1];
		aPoint->p[2] = 2*a2Point->p[2] - t2Point->p[2];
		break;
	 case 1:
		aPoint = get_point_a3(bz, 23);	/* punkt do zmiany */
		tPoint = get_point_a3(bz, 19);	/* naroznik, read-only */
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	 case 2:
		aPoint = get_point_a3(bz, 36); /* punkt do zmiany */
		tPoint = get_point_a3(bz, 32); /* naroznik, read-only */
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	 case 33:
		aPoint = get_point_a3(bz, 7); /* punkt do zmiany */
		tPoint = get_point_a3(bz, 3); /* naroznik, read-only */
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	 case 34:
		aPoint = get_point_a3(bz, 20); /* punkt do zmiany */
		tPoint = get_point_a3(bz, 35); /* naroznik, read-only */
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	 case 17:
		aPoint = get_point_a3(bz, 39); /* punkt do zmiany */
		tPoint = get_point_a3(bz, 35); /* naroznik, read-only */
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	 case 18:
		aPoint = get_point_a3(bz, 4); /* punkt do zmiany */
		tPoint = get_point_a3(bz, 19); /* naroznik, read-only */
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	default: ;
	}
}


void make_C1_old(struct Vector4D*** bz, int c, int x, int y)
{
 int id;
/* printf("(c,x,y) = (%d,%d,%d), id = %d\n", c,x,y,id);*/
 struct Vector4D *aPoint, *tPoint, *a2Point, *t2Point, tempPS;
 TRACE
 id = get_point_idx_a3(c, x, y);
 copy_vector(&tempPS, &bz[c][x][y]);
/* printf_vector("tempPS", &tempPS);*/
 switch(id)
	{
	case 0:
		aPoint = get_point_a3(bz, 21);
		tPoint = get_point_a3(bz, 17);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 1:
		aPoint = get_point_a3(bz, 33);
		tPoint = get_point_a3(bz, 2);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 2:
		aPoint  = get_point_a3(bz, 6);
		a2Point = get_point_a3(bz, 33);
		tPoint  = get_point_a3(bz, 30);
		t2Point = get_point_a3(bz, 1);
		aPoint->p[0] = 2*tempPS.p[0] - tPoint->p[0];
		aPoint->p[1] = 2*tempPS.p[1] - tPoint->p[1];
		aPoint->p[2] = 2*tempPS.p[2] - tPoint->p[2];
		a2Point->p[0] = 2*tempPS.p[0] - t2Point->p[0];
		a2Point->p[1] = 2*tempPS.p[1] - t2Point->p[1];
		a2Point->p[2] = 2*tempPS.p[2] - t2Point->p[2];
		break;
	case 3:
		aPoint = get_point_a3(bz, 16);
		tPoint = get_point_a3(bz, 17);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 6:
		aPoint = get_point_a3(bz, 30);
		tPoint = get_point_a3(bz, 2);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 15:
		aPoint = get_point_a3(bz, 36);
		tPoint = get_point_a3(bz, 32);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 16:
		aPoint = get_point_a3(bz, 3);
		tPoint = get_point_a3(bz, 17);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 17:
		aPoint  = get_point_a3(bz, 3);
		a2Point = get_point_a3(bz, 21);
		tPoint  = get_point_a3(bz, 16);
		t2Point = get_point_a3(bz, 0);
		aPoint->p[0] = 2*tempPS.p[0] - tPoint->p[0];
		aPoint->p[1] = 2*tempPS.p[1] - tPoint->p[1];
		aPoint->p[2] = 2*tempPS.p[2] - tPoint->p[2];
		a2Point->p[0] = 2*tempPS.p[0] - t2Point->p[0];
		a2Point->p[1] = 2*tempPS.p[1] - t2Point->p[1];
		a2Point->p[2] = 2*tempPS.p[2] - t2Point->p[2];
		break;
	case 18:
		aPoint = get_point_a3(bz, 31);
		tPoint = get_point_a3(bz, 32);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 21:
		aPoint = get_point_a3(bz, 0);
		tPoint = get_point_a3(bz, 17);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 30:
		aPoint = get_point_a3(bz, 6);
		tPoint = get_point_a3(bz, 2);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 31:
		aPoint = get_point_a3(bz, 18);
		tPoint = get_point_a3(bz, 32);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 32:
		aPoint  = get_point_a3(bz, 18);
		a2Point = get_point_a3(bz, 36);
		tPoint  = get_point_a3(bz, 31);
		t2Point = get_point_a3(bz, 15);
		aPoint->p[0] = 2*tempPS.p[0] - tPoint->p[0];
		aPoint->p[1] = 2*tempPS.p[1] - tPoint->p[1];
		aPoint->p[2] = 2*tempPS.p[2] - tPoint->p[2];
		a2Point->p[0] = 2*tempPS.p[0] - t2Point->p[0];
		a2Point->p[1] = 2*tempPS.p[1] - t2Point->p[1];
		a2Point->p[2] = 2*tempPS.p[2] - t2Point->p[2];
		break;
	case 33:
		aPoint = get_point_a3(bz, 1);
		tPoint = get_point_a3(bz, 2);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	case 36:
		aPoint = get_point_a3(bz, 15);
		tPoint = get_point_a3(bz, 32);
		aPoint->p[0] = 2*tPoint->p[0] - tempPS.p[0];
		aPoint->p[1] = 2*tPoint->p[1] - tempPS.p[1];
		aPoint->p[2] = 2*tPoint->p[2] - tempPS.p[2];
		break;
	default: ;
	}
}


void correctC2_old(struct Vector4D*** bz, struct Vector4D*** points, struct Vector4D*** points2, int no)
{
	struct Vector4D tempVal, tempVal2, *aPoint, *bPoint, *tempaPoint, *tempbPoint;
	TRACE
	switch (no)
	{
	case 0:
		tempVal.p[0] = (points[0][2][1].p[0]+points2[0][1][0].p[0])/2.;
		tempVal.p[1] = (points[0][2][1].p[1]+points2[0][1][0].p[1])/2.;
		tempVal.p[2] = (points[0][2][1].p[2]+points2[0][1][0].p[2])/2.;
		tempVal2.p[0] = (points[0][2][0]).p[0]+(points[0][3][1]).p[0]-2*(points[0][3][0]).p[0];
		tempVal2.p[1] = (points[0][2][0]).p[1]+(points[0][3][1]).p[1]-2*(points[0][3][0]).p[1];
		tempVal2.p[2] = (points[0][2][0]).p[2]+(points[0][3][1]).p[2]-2*(points[0][3][0]).p[2];
		tempVal.p[0] += tempVal2.p[0];
		tempVal.p[1] += tempVal2.p[1];
		tempVal.p[2] += tempVal2.p[2];
		aPoint     = get_point_a3(bz, 4);
		tempaPoint = get_point_a3(bz, 0);
		bPoint     = get_point_a3(bz, 20);
		tempbPoint = get_point_a3(bz, 16);
		aPoint->p[0] = 2*tempaPoint->p[0] - tempVal.p[0];
		aPoint->p[1] = 2*tempaPoint->p[1] - tempVal.p[1];
		aPoint->p[2] = 2*tempaPoint->p[2] - tempVal.p[2];
		bPoint->p[0] = 2*tempbPoint->p[0] - tempVal.p[0];
		bPoint->p[1] = 2*tempbPoint->p[1] - tempVal.p[1];
		bPoint->p[2] = 2*tempbPoint->p[2] - tempVal.p[2];
		break;
	case 1:
		tempVal.p[0] = (points[1][2][1].p[0] + points2[1][1][0].p[0])/2.;
		tempVal.p[1] = (points[1][2][1].p[1] + points2[1][1][0].p[1])/2.;
		tempVal.p[2] = (points[1][2][1].p[2] + points2[1][1][0].p[2])/2.;
		tempVal2.p[0] = (points[1][2][0]).p[0] + (points[1][3][1]).p[0] - 2*(points[1][3][0]).p[0];
		tempVal2.p[1] = (points[1][2][0]).p[1] + (points[1][3][1]).p[1] - 2*(points[1][3][0]).p[1];
		tempVal2.p[2] = (points[1][2][0]).p[2] + (points[1][3][1]).p[2] - 2*(points[1][3][0]).p[2];
		tempVal.p[0] += tempVal2.p[0];
		tempVal.p[1] += tempVal2.p[1];
		tempVal.p[2] += tempVal2.p[2];
		aPoint     = get_point_a3(bz, 19);
		tempaPoint = get_point_a3(bz, 15);
		bPoint     = get_point_a3(bz, 35);
		tempbPoint = get_point_a3(bz, 31);
		aPoint->p[0] = 2*tempaPoint->p[0] - tempVal.p[0];
		aPoint->p[1] = 2*tempaPoint->p[1] - tempVal.p[1];
		aPoint->p[2] = 2*tempaPoint->p[2] - tempVal.p[2];
		bPoint->p[0] = 2*tempbPoint->p[0] - tempVal.p[0];
		bPoint->p[1] = 2*tempbPoint->p[1] - tempVal.p[1];
		bPoint->p[2] = 2*tempbPoint->p[2] - tempVal.p[2];
		break;
	case 2:
		tempVal.p[0] = (points[2][2][1].p[0] + points2[2][1][0].p[0])/2.;
		tempVal.p[1] = (points[2][2][1].p[1] + points2[2][1][0].p[1])/2.;
		tempVal.p[2] = (points[2][2][1].p[2] + points2[2][1][0].p[2])/2.;
		tempVal2.p[0] = (points[2][2][0]).p[0] + (points[2][3][1]).p[0] - 2*(points[2][3][0]).p[0];
		tempVal2.p[1] = (points[2][2][0]).p[1] + (points[2][3][1]).p[1] - 2*(points[2][3][0]).p[1];
		tempVal2.p[2] = (points[2][2][0]).p[2] + (points[2][3][1]).p[2] - 2*(points[2][3][0]).p[2];
		tempVal.p[0] += tempVal2.p[0];
		tempVal.p[1] += tempVal2.p[1];
		tempVal.p[2] += tempVal2.p[2];
		aPoint     = get_point_a3(bz, 34);
		tempaPoint = get_point_a3(bz, 30);
		bPoint     = get_point_a3(bz, 5);
		tempbPoint = get_point_a3(bz, 1);
		aPoint->p[0] = 2*tempaPoint->p[0] - tempVal.p[0];
		aPoint->p[1] = 2*tempaPoint->p[1] - tempVal.p[1];
		aPoint->p[2] = 2*tempaPoint->p[2] - tempVal.p[2];
		bPoint->p[0] = 2*tempbPoint->p[0] - tempVal.p[0];
		bPoint->p[1] = 2*tempbPoint->p[1] - tempVal.p[1];
		bPoint->p[2] = 2*tempbPoint->p[2] - tempVal.p[2];
		break;
	default: ;
	}
}


void correctC2(struct Vector4D*** bz, struct Vector4D*** points, struct Vector4D*** points2, int no)
{
	struct Vector4D tempVal, tempVal2, *aPoint, *bPoint, *tempaPoint, *tempbPoint;
	TRACE
	switch (no)
	{
	case 0:
		tempVal.p[0] = (points[0][2][1].p[0]+points2[0][1][0].p[0])/2.;
		tempVal.p[1] = (points[0][2][1].p[1]+points2[0][1][0].p[1])/2.;
		tempVal.p[2] = (points[0][2][1].p[2]+points2[0][1][0].p[2])/2.;
		tempVal2.p[0] = (points[0][2][0]).p[0]+(points[0][3][1]).p[0]-2*(points[0][3][0]).p[0];
		tempVal2.p[1] = (points[0][2][0]).p[1]+(points[0][3][1]).p[1]-2*(points[0][3][0]).p[1];
		tempVal2.p[2] = (points[0][2][0]).p[2]+(points[0][3][1]).p[2]-2*(points[0][3][0]).p[2];
		tempVal.p[0] += tempVal2.p[0];
		tempVal.p[1] += tempVal2.p[1];
		tempVal.p[2] += tempVal2.p[2];
		aPoint     = get_point_a3(bz, 4);
		tempaPoint = get_point_a3(bz, 0);
		bPoint     = get_point_a3(bz, 23);
		tempbPoint = get_point_a3(bz, 19);
		aPoint->p[0] = 2*tempaPoint->p[0] - tempVal.p[0];
		aPoint->p[1] = 2*tempaPoint->p[1] - tempVal.p[1];
		aPoint->p[2] = 2*tempaPoint->p[2] - tempVal.p[2];
		bPoint->p[0] = 2*tempbPoint->p[0] - tempVal.p[0];
		bPoint->p[1] = 2*tempbPoint->p[1] - tempVal.p[1];
		bPoint->p[2] = 2*tempbPoint->p[2] - tempVal.p[2];
		break;
	case 1:
		tempVal.p[0] = (points[1][2][1].p[0] + points2[1][1][0].p[0])/2.;
		tempVal.p[1] = (points[1][2][1].p[1] + points2[1][1][0].p[1])/2.;
		tempVal.p[2] = (points[1][2][1].p[2] + points2[1][1][0].p[2])/2.;
		tempVal2.p[0] = (points[1][2][0]).p[0] + (points[1][3][1]).p[0] - 2*(points[1][3][0]).p[0];
		tempVal2.p[1] = (points[1][2][0]).p[1] + (points[1][3][1]).p[1] - 2*(points[1][3][0]).p[1];
		tempVal2.p[2] = (points[1][2][0]).p[2] + (points[1][3][1]).p[2] - 2*(points[1][3][0]).p[2];
		tempVal.p[0] += tempVal2.p[0];
		tempVal.p[1] += tempVal2.p[1];
		tempVal.p[2] += tempVal2.p[2];
		aPoint     = get_point_a3(bz, 20);
		tempaPoint = get_point_a3(bz, 16);
		bPoint     = get_point_a3(bz, 39);
		tempbPoint = get_point_a3(bz, 35);
		aPoint->p[0] = 2*tempaPoint->p[0] - tempVal.p[0];
		aPoint->p[1] = 2*tempaPoint->p[1] - tempVal.p[1];
		aPoint->p[2] = 2*tempaPoint->p[2] - tempVal.p[2];
		bPoint->p[0] = 2*tempbPoint->p[0] - tempVal.p[0];
		bPoint->p[1] = 2*tempbPoint->p[1] - tempVal.p[1];
		bPoint->p[2] = 2*tempbPoint->p[2] - tempVal.p[2];
		break;
	case 2:
		tempVal.p[0] = (points[2][2][1].p[0] + points2[2][1][0].p[0])/2.;
		tempVal.p[1] = (points[2][2][1].p[1] + points2[2][1][0].p[1])/2.;
		tempVal.p[2] = (points[2][2][1].p[2] + points2[2][1][0].p[2])/2.;
		tempVal2.p[0] = (points[2][2][0]).p[0] + (points[2][3][1]).p[0] - 2*(points[2][3][0]).p[0];
		tempVal2.p[1] = (points[2][2][0]).p[1] + (points[2][3][1]).p[1] - 2*(points[2][3][0]).p[1];
		tempVal2.p[2] = (points[2][2][0]).p[2] + (points[2][3][1]).p[2] - 2*(points[2][3][0]).p[2];
		tempVal.p[0] += tempVal2.p[0];
		tempVal.p[1] += tempVal2.p[1];
		tempVal.p[2] += tempVal2.p[2];
		aPoint     = get_point_a3(bz, 36);
		tempaPoint = get_point_a3(bz, 32);
		bPoint     = get_point_a3(bz, 7);
		tempbPoint = get_point_a3(bz, 3);
		aPoint->p[0] = 2*tempaPoint->p[0] - tempVal.p[0];
		aPoint->p[1] = 2*tempaPoint->p[1] - tempVal.p[1];
		aPoint->p[2] = 2*tempaPoint->p[2] - tempVal.p[2];
		bPoint->p[0] = 2*tempbPoint->p[0] - tempVal.p[0];
		bPoint->p[1] = 2*tempbPoint->p[1] - tempVal.p[1];
		bPoint->p[2] = 2*tempbPoint->p[2] - tempVal.p[2];
		break;
	default: ;
	}
}


void make_C2(struct Vector4D*** bz, struct Vector4D*** bp1, struct Vector4D*** bp2, int c, int x, int y)
{
 int id;
 TRACE
 id = get_point_idx_a3(c, x, y);
 switch(id)
	{
	case 0:
	case 1:
	case 4:
	case 18:
	case 19:
	case 23:
		correctC2(bz, bp1, bp2, 0);
		break;
	case 16:
	case 17:
	case 20:
	case 34:
	case 35:
	case 39:
		correctC2(bz, bp1, bp2, 1);
		break;
	case 2:
	case 3:
	case 7:
	case 32:
	case 33:
	case 36:
		correctC2(bz, bp1, bp2, 2);
		break;
	default: ;
	}
}

void get_nurb_box(struct Nurb* n, struct Vector4D* vmin, struct Vector4D* vmax)
{
 int i,j,k;
 TRACE
 for (i=0;i<DIM-1;i++) vmax->p[i] = vmin->p[i] = n->P[0][0].p[i];
 for (i=0;i<4;i++)
 for (j=0;j<4;j++)
 for (k=0;k<DIM-1;k++)
   {
    if (n->P[i][j].p[k] > vmax->p[k])      vmax->p[k] = n->P[i][j].p[k];
    else if(n->P[i][j].p[k] < vmin->p[k])  vmin->p[k] = n->P[i][j].p[k];
   }
 vmin->p[DIM-1] = 1.;
 vmax->p[DIM-1] = 1.;
}

void get_nurb_pseudo_box(struct Nurb* n, struct Vector4D* vmin, struct Vector4D* vmax)
{
 int i,j,k;
 TRACE
 for (i=0;i<DIM-1;i++) vmax->p[i] = vmin->p[i] = n->points[0][0].p[i];
 for (i=0;i<n->npointsx;i++)
 for (j=0;j<n->npointsy;j++)
 for (k=0;k<DIM-1;k++)
   {
    if (n->points[i][j].p[k] > vmax->p[k])      vmax->p[k] = n->points[i][j].p[k];
    else if(n->points[i][j].p[k] < vmin->p[k])  vmin->p[k] = n->points[i][j].p[k];
   }
 vmin->p[DIM-1] = 1.;
 vmax->p[DIM-1] = 1.;
}

int box_intersect(struct Vector4D* min1, struct Vector4D* max1, struct Vector4D* min2, struct Vector4D* max2)
{
 struct Vector4D temp;		/* FIXME - looks like it works, but wasn't tested enough... */
 int i;
 TRACE
 i=0;
	construct_vector3(&temp, max2->p[0], max2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) return 1;
	construct_vector3(&temp, max2->p[0], max2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) return 1;
	construct_vector3(&temp, max2->p[0], min2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) return 1;
	construct_vector3(&temp, max2->p[0], min2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) return 1;
	construct_vector3(&temp, min2->p[0], max2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) return 1;
	construct_vector3(&temp, min2->p[0], max2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) return 1;
	construct_vector3(&temp, min2->p[0], min2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) return 1;
	construct_vector3(&temp, min2->p[0], min2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) return 1;
	construct_vector3(&temp, max2->p[0], max2->p[1], max2->p[2]);
	if( (temp.p[0] >= max1->p[0]) && (temp.p[1] >= min1->p[1]) &&(temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], max2->p[1], min2->p[2]);
	if( (temp.p[0] >= max1->p[0]) && (temp.p[1] >= min1->p[1]) &&(temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], min2->p[1], max2->p[2]);
	if( (temp.p[0] >= max1->p[0]) && (temp.p[1] >= min1->p[1]) &&(temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], min2->p[1], min2->p[2]);
	if( (temp.p[0] >= max1->p[0]) && (temp.p[1] >= min1->p[1]) &&(temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], max2->p[1], max2->p[2]);
	if( (temp.p[0] <= min1->p[0]) && (temp.p[1] >= min1->p[1]) &&(temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], max2->p[1], min2->p[2]);
	if( (temp.p[0] <= min1->p[0]) && (temp.p[1] >= min1->p[1]) &&(temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], min2->p[1], max2->p[2]);
	if( (temp.p[0] <= min1->p[0]) && (temp.p[1] >= min1->p[1]) &&(temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], min2->p[1], min2->p[2]);
	if( (temp.p[0] <= min1->p[0]) && (temp.p[1] >= min1->p[1]) &&(temp.p[1] <= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	if(i==8) return 1;
	i=0;
	construct_vector3(&temp, max2->p[0], max2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], max2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], min2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] <= min1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], min2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] <= min1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], max2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], max2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= max1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], min2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] <= min1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], min2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] <= min1->p[1]) && (temp.p[2] >= min1->p[2]) && (temp.p[2] <= max1->p[2])) ++i;
	if(i==8) return 1;
	i=0;
	construct_vector3(&temp, max2->p[0], max2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= max1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], max2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] <= min1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], min2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= max1->p[2])) ++i;
	construct_vector3(&temp, max2->p[0], min2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] <= min1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], max2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], max2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] <= min1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], min2->p[1], max2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] >= max1->p[2])) ++i;
	construct_vector3(&temp, min2->p[0], min2->p[1], min2->p[2]);
	if( (temp.p[0] >= min1->p[0]) && (temp.p[0] <= max1->p[0]) && (temp.p[1] >= min1->p[1]) && (temp.p[1] <= max1->p[1]) && (temp.p[2] <= min1->p[2])) ++i;
	if(i==8) return 1;
	return 0;
}

int nurb_intersection_box(struct Nurb* n1, struct Nurb* n2)
{
 struct Vector4D p1min, p1max, p2min, p2max;
 TRACE
 get_nurb_box(n1, &p1min, &p1max);
 get_nurb_box(n2, &p2min, &p2max);
 if (box_intersect(&p1min, &p1max, &p2min, &p2max)) return 1;
 else if (box_intersect(&p2min, &p2max, &p1min, &p1max)) return 1;
 else return 0;
}

int nurb_pseudo_intersection_box(struct Nurb* n1, struct Nurb* n2)
{
 struct Vector4D p1min, p1max, p2min, p2max;
 TRACE
 get_nurb_pseudo_box(n1, &p1min, &p1max);
 get_nurb_pseudo_box(n2, &p2min, &p2max);
 if (box_intersect(&p1min, &p1max, &p2min, &p2max)) return 1;
 else if (box_intersect(&p2min, &p2max, &p1min, &p1max)) return 1;
 else return 0;
}

double vector_distance(struct Vector4D* v1, struct Vector4D* v2)
{
 TRACE
 return sqrt((v2->p[0]-v1->p[0])*(v2->p[0]-v1->p[0])+(v2->p[1]-v1->p[1])*(v2->p[1]-v1->p[1])+(v2->p[2]-v1->p[2])*(v2->p[2]-v1->p[2]));
}

void nurb_computemv(struct Mesh4D* m, struct Vector4D* vec, double u, double v)
{
 int iu,iv;
 double tU, tV;
 TRACE
 iu = (int)u;
 iv = (int)v;
 tU = u - (double)iu;
 tV = v - (double)iv;
/* if (iu && u == (double)iu) tU = 0.;*/
/* if (iv && v == (double)iv) tV = 0.;*/
 if (iu < 0)      { iu = 0.; tU = 0.; }
 if (iv < 0)      { iv = 0.; tV = 0.; }
 if (iu >= m->nX) { iu = m->nX-1; tU = 1.; }
 if (iv >= m->nY) { iv = m->nY-1; tV = 1.; }
/* printf("Mesh(%f,%f) -> nurb[%d][%d](%f,%f)\n", u,v, iu,iv,tU,tV);*/
/* printf("computemv: %d %d, %f, %f\n", iu,iv,tU,tV);*/
/* struct Nurb* n = m->nurbs[iu][iv];*/
/* int i,j;*/
/* for (j=0;j<4;j++)*/
/* {*/
/*  for (i=0;i<4;i++) printf("%3.0f,%3.0f,%3.0f ", n->P[i][j].p[0], n->P[i][j].p[1],n->P[i][j].p[2]);*/
/*  printf("\n");*/
/* }*/
 nurb_computev(m->nurbs[iu][iv], vec, tV, tU);
}

int find_initial_point(struct TrimmedMesh* tm, struct Vector4D* p1, struct Vector4D* p2, struct Vector4D* x)
{
 int MAX_ITERATIONS = 25;
/* double EPSILON = 0.2*tm->bscale;*/
 double EPSILON = 1e1;
 int c = 0;
 double intersections;
 double cu, cv, cs, ct;
 double bu, bv, bs, bt;
 double au, av, as, at;
 double du, dv, ds, dt;
 double minu, minv, mins, mint, maxu, maxv, maxs, maxt;
 struct Vector4D cPSfirst, cPSsecond, tPSfirst, tPSsecond;
 double fu, fv, fs, ft;
 double cdist, tdist;
 int i1, j1, i2, j2;
 struct Mesh4D *n1, *n2;
 zero_vector(&cPSfirst);
 zero_vector(&cPSsecond);
 zero_vector(&tPSfirst);
 zero_vector(&tPSsecond);
/* printf("EPSILON=%f\n", EPSILON);*/
 TRACE
 n1 = tm->m1;
 n2 = tm->m2;
 intersections = 4;
 cu = 0; 
 cv = 0; 
 cs = 0; 
 ct = 0;
 au = n1->nX; 
 av = n1->nY; 
 as = n2->nX; 
 at = n2->nY;
/* printf("AA %f %f %f %f\n", au,av,as,at);*/
 minu = 0; 
 minv = 0; 
 mins = 0; 
 mint = 0;
 maxu = n1->nX;
 maxv = n1->nY;
 maxs = n2->nX;
 maxt = n2->nY;
 du = (maxu - minu) / intersections; 
 dv = (maxv - minv) / intersections; 
 ds = (maxs - mins) / intersections; 
 dt = (maxt - mint) / intersections;
/* printf("DD %f %f %f %f\n", du,dv,ds,dt);*/
/* printf("CC %f %f %f %f\n", cu,cv,cs,ct);*/
/* cPSfirst = surfaces.first->solveBSplineCurve(cu,cv); */
 nurb_computemv(n1, &cPSfirst, cu, cv);
/* printf_vector("cPSFirst", &cPSfirst);*/
 fu = cu; 
 fv = cv;
/* cPSsecond = surfaces.second->solveBSplineCurve(cs,ct); */
 nurb_computemv(n2, &cPSsecond, cs, ct);
/* printf_vector("cPSSecond", &cPSsecond);*/
 fs = cs; 
 ft = cv;
/* cdist = PointStruct::calculateDistance(cPSfirst,cPSsecond);	*/
 cdist = vector_distance(&cPSfirst, &cPSsecond);
/* printf("cdist = %f\n", cdist);*/
 /*nurb_computemv(n1, &cPSfirst, 3., 2.);
 printf_vector("11", &cPSfirst);
 nurb_computemv(n1, &cPSfirst, 0.3, 0.4);
 printf_vector("11", &cPSfirst);
 nurb_computemv(n1, &cPSfirst, 1.4, 1.3);
 printf_vector("11", &cPSfirst);
 nurb_computemv(n1, &cPSfirst, 2.5, 2.6);
 printf_vector("11", &cPSfirst);
 nurb_computemv(n1, &cPSfirst, 0., 2.9);
 printf_vector("11", &cPSfirst);
 exit(1);*/
 while (c<MAX_ITERATIONS)
  {
/*      printf("c = %d\n", c);*/
   ++c;
   bv = cv;
   for (i1=0;i1<=intersections;++i1)
     {
      bu = cu;
      for(j1=0;j1<=intersections;++j1)
	 {
/*	  tPSfirst = surfaces.first->solveBSplineCurve(bu,bv);*/
	  nurb_computemv(n1, &tPSfirst, bu, bv); 
	  bt = ct;
	  for (i2=0;i2<=intersections;++i2)
	    {
	     bs = cs;
	     for (j2=0;j2<=intersections;++j2)
	       {
/*		tPSsecond = surfaces.second->solveBSplineCurve(bs,bt);*/
		nurb_computemv(n2, &tPSsecond, bs, bt);
/*		tdist = PointStruct::calculateDistance(tPSfirst,tPSsecond);*/
		tdist = vector_distance(&tPSfirst, &tPSsecond);
		if (tdist<cdist)
		  {
/*		   printf("tdist = %f\n", tdist);*/
		   copy_vector(&cPSfirst, &tPSfirst); 
		   copy_vector(&cPSsecond, &tPSsecond);
		   cdist = tdist;
		   fu = bu;
		   fv = bv; 
		   fs = bs; 
		   ft = bt;				
		  }
		bs += ds;
	       }
	     bt += dt;
	    }
	    bu += du;
	   }
	   bv += dv;
	  }
/*      printf("cdist = %f\n", cdist);*/
	if (cdist<EPSILON) break;
	cu = max(minu, fu-du); 
	cv = max(minv, fv-dv); 
	cs = max(mins, fs-ds); 
	ct = max(mint, ft-dt);
	au = min(maxu, fu+du); 
	av = min(maxv, fv+dv); 
	as = min(maxs, fs+ds); 
	at = min(maxt, ft+dt);
	du = (au - cu) / intersections; 
	dv = (av - cv) / intersections; 
	ds = (as - cs) / intersections; 
	dt = (at - ct) / intersections;
      }
   if (c==MAX_ITERATIONS) return 0;
   else
     {
      copy_vector(p1, &cPSfirst);
      copy_vector(p2, &cPSsecond);
      printf("init_point> %f %f %f %f\n", fu,fv,fs,ft);
      construct_vector4(x, fu, fv, fs, ft);
/*      x.u = fu; x.v = fv; x.s = fs; x.t = ft;*/
      return 1;
     }
}

void nurb_compute_dumv(struct Mesh4D* m1, struct Vector4D* vec, double u, double v)	/* FIXME is it working OK */
{											/* shouldn't be vec normalized? */
 /*double tU = u-1e-10;
 struct Vector4D v1,v2;
 int i;
 TRACE
 vec->p[DIM-1] = 1.;
 if (tU >= 0.)
  {
      printf("positive!\n");
   nurb_computemv(m1, &v1, u, v);
   nurb_computemv(m1, &v2, tU, v);
   printf_vector("v1", &v1);
   printf_vector("v2", &v2);
   for (i=0;i<DIM-1;i++) vec->p[i] = (v1.p[i] - v2.p[i])/1e-10;
  }
 else
  {
   tU = u+1e-10;
   nurb_computemv(m1, &v1, u, v);
   nurb_computemv(m1, &v2, tU, v);
   printf_vector("v1", &v1);
   printf_vector("v2", &v2);
   for (i=0;i<DIM-1;i++) vec->p[i] = (v2.p[i] - v1.p[i])/1e-10;
  }*/
 double tempU;
 struct Vector4D fu, fu1;
 tempU = u-1e-6;
 if (u <= 1e-6) tempU = u+1e-6;
 nurb_computemv(m1, &fu, u, v);
 nurb_computemv(m1, &fu1, tempU, v);
/* printf_vector("fu", &fu);*/
/* printf_vector("fu1", &fu1);*/
 fu.p[3] = fu.w = 1.;
 fu1.p[3] = fu1.w = 1.;
 if (u <= 1e-6)
    {
     fu1.p[0] -= fu.p[0]; 
     fu1.p[1] -= fu.p[1]; 
     fu1.p[2] -= fu.p[2];
     fu1.p[0] /= 1e-6; 
     fu1.p[1] /= 1e-6; 
     fu1.p[2] /= 1e-6;
     fu1.p[0] = -fu1.p[0]; 
     fu1.p[1] = -fu1.p[1]; 
     fu1.p[2] = -fu1.p[2];
     copy_vector(vec, &fu1);
    }
 else 
    {
     fu.p[0] -= fu1.p[0]; 
     fu.p[1] -= fu1.p[1]; 
     fu.p[2] -= fu1.p[2];
     fu.p[0] /= 1e-6; 
     fu.p[1] /= 1e-6; 
     fu.p[2] /= 1e-6;
     copy_vector(vec, &fu);
    }
}

void nurb_compute_dvmv(struct Mesh4D* m1, struct Vector4D* vec, double u, double v)	/* FIXME same problem */
{
 /*double tV = v+1e-6;
 struct Vector4D v1,v2;
 int i;
 TRACE
 nurb_computemv(m1, &v1, u, v);
 nurb_computemv(m1, &v2, u, tV);
 for (i=0;i<DIM-1;i++) vec->p[i] = v2.p[i] - v1.p[i];
 vec->p[DIM-1] = 1.;
 normalize_vector_3d(vec);*/
 double tempU;
 struct Vector4D fu, fu1;
 tempU = u-1e-6;
 if (u <= 1e-6) tempU = u+1e-6;
 nurb_computemv(m1, &fu, v, u);
 nurb_computemv(m1, &fu1, v, tempU);
/* printf_vector("fu", &fu);*/
/* printf_vector("fu1", &fu1);*/
 fu.p[3] = fu.w = 1.;
 fu1.p[3] = fu1.w = 1.;
 if (u <= 1e-6)
    {
     fu1.p[0] -= fu.p[0]; 
     fu1.p[1] -= fu.p[1]; 
     fu1.p[2] -= fu.p[2];
     fu1.p[0] /= 1e-6; 
     fu1.p[1] /= 1e-6; 
     fu1.p[2] /= 1e-6;
     fu1.p[0] = -fu1.p[0]; 
     fu1.p[1] = -fu1.p[1]; 
     fu1.p[2] = -fu1.p[2];
     copy_vector(vec, &fu1);
    }
 else 
    {
     fu.p[0] -= fu1.p[0]; 
     fu.p[1] -= fu1.p[1]; 
     fu.p[2] -= fu1.p[2];
     fu.p[0] /= 1e-6; 
     fu.p[1] /= 1e-6; 
     fu.p[2] /= 1e-6;
     copy_vector(vec, &fu);
    }
}

double vector_length(struct Vector4D* v)
{
 TRACE
 return sqrt(v->p[0]*v->p[0]+v->p[1]*v->p[1]+v->p[2]*v->p[2]);
}

void root_finder_fp2(struct TrimmedMesh* tm, struct Matrix4D* retVal, struct Vector4D* x)	/* FIXME are vectors correctly stored */
{												/* in matrix, shouldn't they be transponed? */
/*	BSplinePatchTrimmedObject::vector4x4 retVal;*/
	struct Vector4D dpdu, dpdv, dqds, dqdt;
	TRACE
	nurb_compute_dumv(tm->m1, &dpdu, x->p[0], x->p[1]);
	nurb_compute_dvmv(tm->m1, &dpdv, x->p[0], x->p[1]);
	nurb_compute_dumv(tm->m2, &dqds, x->p[2], x->p[3]);
	nurb_compute_dvmv(tm->m2, &dqdt, x->p[2], x->p[3]);
/*	printf("tm->d = %f\n", tm->d);*/
/*	printf_vector("tm->t", &tm->t);*/
/*	printf_vector("dpdu", &dpdu);*/
/*	printf_vector("dpdv", &dpdv);*/
/*	printf_vector("dqds", &dqds);*/
/*	printf_vector("dqdt", &dqdt);*/
/*        printf_vector("f2p -->x", x);*/
	/*dpdu = dPdu2(x.u,x.v);
	dpdv = dPdv2(x.u,x.v);
	dqds = dQdu2(x.s,x.t);
	dqdt = dQdv2(x.s,x.t);*/
	retVal->v[0].p[0] = dpdu.p[0];
	retVal->v[0].p[1] = dpdv.p[0];
	retVal->v[0].p[2] = -dqds.p[0];
	retVal->v[0].p[3] = -dqdt.p[0];
	retVal->v[1].p[0] = dpdu.p[1];
	retVal->v[1].p[1] = dpdv.p[1];
	retVal->v[1].p[2] = -dqds.p[1];
	retVal->v[1].p[3] = -dqdt.p[1];
	retVal->v[2].p[0] = dpdu.p[2];
	retVal->v[2].p[1] = dpdv.p[2];
	retVal->v[2].p[2] = -dqds.p[2];
	retVal->v[2].p[3] = -dqdt.p[2];
	retVal->v[3].p[0] = dpdu.p[0]*tm->t.p[0] + dpdu.p[1]*tm->t.p[1] + dpdu.p[2]*tm->t.p[2];
	retVal->v[3].p[1] = dpdv.p[0]*tm->t.p[0] + dpdv.p[1]*tm->t.p[1] + dpdv.p[2]*tm->t.p[2];
	retVal->v[3].p[2] = 0;
	retVal->v[3].p[3] = 0;
	/*retVal->v[0].p[0] = dpdu.p[0];
	retVal->v[1].p[0] = dpdv.p[0];
	retVal->v[2].p[0] = -dqds.p[0];
	retVal->v[3].p[0] = -dqdt.p[0];
	retVal->v[0].p[1] = dpdu.p[1];
	retVal->v[1].p[1] = dpdv.p[1];
	retVal->v[2].p[1] = -dqds.p[1];
	retVal->v[3].p[1] = -dqdt.p[1];
	retVal->v[0].p[2] = dpdu.p[2];
	retVal->v[1].p[2] = dpdv.p[2];
	retVal->v[2].p[2] = -dqds.p[2];
	retVal->v[3].p[2] = -dqdt.p[2];
	retVal->v[0].p[3] = dpdu.p[0]*tm->t.p[0] + dpdu.p[1]*tm->t.p[1] + dpdu.p[2]*tm->t.p[2];
	retVal->v[1].p[3] = dpdv.p[0]*tm->t.p[0] + dpdv.p[1]*tm->t.p[1] + dpdv.p[2]*tm->t.p[2];
	retVal->v[2].p[3] = 0;
	retVal->v[3].p[3] = 0;*/
}

void root_finder_f2(struct TrimmedMesh* tm, struct Vector4D* retVal, struct Vector4D* x)
{
 struct Vector4D puv, qst;
 TRACE
 /*return surfaces.first->solveBSplineCurve(u,v);
 puv = root_finder_P2(x.p[0], x.p[1]);*/
 nurb_computemv(tm->m1, &puv, x->p[0], x->p[1]);
 nurb_computemv(tm->m2, &qst, x->p[2], x->p[3]);
/* printf_vector("f2 -->x", x);*/
/* qst = root_finder_Q2(x.p[2], x.p[3]);*/
 retVal->p[0] = puv.p[0] - qst.p[0];
 retVal->p[1] = puv.p[1] - qst.p[1];
 retVal->p[2] = puv.p[2] - qst.p[2];
/* printf_vector("tm->P0", &tm->P0);*/
 retVal->p[3] = (puv.p[0] - tm->P0.p[0])*tm->t.p[0] + (puv.p[1] - tm->P0.p[1])*tm->t.p[1] + (puv.p[2] - tm->P0.p[2])*tm->t.p[2] - tm->d;
/* printf_vector("f2 -->retVal", retVal);*/
}

int root_finder(struct TrimmedMesh* tm, struct Vector4D* xi)	/* FIXME: dont't work now, fix it! */
{								/* is: P0,d,t OK computed in tm? */
 struct Vector4D di, f2v;
 struct Matrix4D fp2v, ifp2v;
 double epsilon = 1e-10;
 double epsilon2 = 1e10;
 double dilen;
 int counter;
 int maxcount;
 struct Mesh4D *m1, *m2;
 TRACE
 m1 = tm->m1;
 m2 = tm->m2;
/* printf("called root finder!\n");*/
/* printf_vector("xi", xi);*/
/* printf("Bounds: <%d,%d> <%d,%d>\n", m1->nX, m1->nY, m2->nX, m2->nY);*/
 counter = 0;
 maxcount = 500;
 zero_vector(&di);
 zero_vector(&f2v);
/* printf_vector("xi", xi);*/
 do
  {
/*   printf("Check bound...\n");*/
   if (!(
	 (xi->p[0]<=(double)m1->nX) && 
	 (xi->p[0]>=0.) && 
	 (xi->p[1]<=(double)m1->nY) && 
	 (xi->p[1]>=0.) && 
	 (xi->p[2]<=(double)m2->nX) && 
	 (xi->p[2]>=0.) && 
	 (xi->p[3]<=(double)m2->nY) && 
	 (xi->p[3]>=0.)
       ))
     {
/*      printf("1:Root finder zwraca false (poza dziedzina)!\n");*/
      return 0;
     }
/*   printf("Bounds passed!\n");*/
   root_finder_f2(tm, &f2v, xi);
/*   printf_vector("f2v", &f2v);*/
   root_finder_fp2(tm, &fp2v, xi);
/*   printf_matrix("fp2v", &fp2v);*/
   invert_matrix(&ifp2v, &fp2v);
/*   printf_matrix("ifp2v", &ifp2v);*/
   /*matrix_mul_vector(&di, &ifp2v, &f2v);*/
   vector_mul_matrix(&di, &f2v, &ifp2v);
/*   printf_vector("di", &di);*/
/*   di = mnozenie(odwrotnosc((fprim2(xi))),f2(xi));*/
   xi->p[0] -= di.p[0];
   xi->p[1] -= di.p[1];
   xi->p[2] -= di.p[2];
   xi->p[3] -= di.p[3];
   counter++;
   dilen = sqrt(di.p[0]*di.p[0]+di.p[1]*di.p[1]+di.p[2]*di.p[2]+di.p[3]*di.p[3]);
  }
 while ((dilen>epsilon) && (dilen<epsilon2) && (counter<maxcount));
/* printf_vector("FINAL XI\n", xi);*/
 if (counter<maxcount) 
  { 
/*   printf("root finder ok iters: %d!\n", counter); */
   update_trimm_curves(tm, xi);
   return 1; 
  }
 printf("2:Root finder zwraca false!\n");
 if(!(
      (xi->p[0]<=m1->nX) 
      && (xi->p[0]>=0) 
      && (xi->p[1]<=m1->nY) 
      && (xi->p[1]>=0) 
      && (xi->p[2]<=m2->nX) 
      && (xi->p[2]>=0) 
      && (xi->p[3]<=m2->nY) 
      && (xi->p[3]>=0)
      ))
   {
    printf("3:Root finder zwraca false (poza dziedzina)!\n");
    return 0;
   }
 printf("4:Root finder zwraca false (poza dziedzina)!\n");
 return 0;
}

void update_trimm_curves(struct TrimmedMesh* tm, struct Vector4D* vec)
{
 struct Vector4D *p1, *p2, *p3;
 p1 = (struct Vector4D*)malloc(sizeof(struct Vector4D));
 p2 = (struct Vector4D*)malloc(sizeof(struct Vector4D));
 p3 = (struct Vector4D*)malloc(sizeof(struct Vector4D));
 p1->p[3] = p1->w = 1.;
 p2->p[3] = p2->w = 1.;
 nurb_computemv(tm->m1, p1, vec->p[0], vec->p[1]);
 nurb_computemv(tm->m2, p2, vec->p[2], vec->p[3]);
 copy_vector(p3, vec);
 vlist_add(&tm->tc1, (void*)p1);
 vlist_add(&tm->tc2, (void*)p2);
 vlist_add(&tm->tcuv, (void*)p3);
}

void zero_trimm_curves(struct TrimmedMesh* tm)
{
 tm->tc1 = tm->tc2 = tm->tcuv = NULL;
 tm->matrix1 = NULL;
 tm->points1 = NULL;
 tm->matrix2 = NULL;
 tm->points2 = NULL;
}

int root_finder_bla(struct TrimmedMesh* tm, struct Vector4D* xi)	/* FIXME: dont't work now, fix it! */
{
 double epsilon = 1;
 double epsilon1 = 1e-6;
 double epsilon2 = 1e6;
 double distance,mindistance;
 struct Vector4D p1,p2;
 struct Vector4D x2;
 struct Mesh4D *m1, *m2;
 int counter = 0;
 int maxcount = 400;
 m1 = tm->m1;
 m2 = tm->m2;
 nurb_computemv(tm->m1, &p1, xi->p[0], xi->p[1]);
 nurb_computemv(tm->m2, &p2, xi->p[2], xi->p[3]);
 distance = vector_distance(&p1, &p2);
 mindistance = distance;
/* printf("Distance = %f\n", distance);*/
/* printf_vector("tm->t", &tm->t);*/
 copy_vector(&x2, xi);
 update_trimm_curves(tm, xi);	/* FIXME repair root_finder */
 return 1;
 do
  {
/*   printf("Check bound...\n");*/
   if (!(
	 (x2.p[0]<=(double)m1->nX) && 
	 (x2.p[0]>=0.) && 
	 (x2.p[1]<=(double)m1->nY) && 
	 (x2.p[1]>=0.) && 
	 (x2.p[2]<=(double)m2->nX) && 
	 (x2.p[2]>=0.) && 
	 (x2.p[3]<=(double)m2->nY) && 
	 (x2.p[3]>=0.)
       ))
     {
      printf("1:Root finder zwraca false (poza dziedzina)!\n");
      return 0;
     }
   update_trimm_curves(tm, xi);
   x2.p[0] += epsilon * tm->du;
   x2.p[1] += epsilon * tm->dv;
   x2.p[2] += epsilon * tm->ds;
   x2.p[3] += epsilon * tm->dt;
   nurb_computemv(tm->m1, &p1, x2.p[0], x2.p[1]);
   nurb_computemv(tm->m2, &p2, x2.p[2], x2.p[3]);
   distance = vector_distance(&p1, &p2);
   if (distance < mindistance) mindistance = distance;
   printf("Distance = %f\n", distance);
   counter++;
  }
 while ((distance>epsilon1) && (distance<epsilon2) && (counter<maxcount));
/* printf_vector("TRIMM", &x2);*/
 return 1;
 if (counter<maxcount) 
   { 
    copy_vector(xi, &x2);
    printf("root finder ok!\n"); 
    return 1; 
   }
 printf("4:Root finder zwraca false (poza dziedzina)!\n");
 return 0;
}

void zero_vector(struct Vector4D* vec)
{
 int i;
 for (i=0;i<DIM-1;i++) vec->p[i] = 0.;
 vec->p[DIM-1] = 1.;
 vec->w = 1.;
}

void set_param_vector(struct TrimmedMesh* tm, double du, double dv, double ds, double dt, double factor)
{
 tm->du = du * factor;
 tm->dv = dv * factor;
 tm->ds = ds * factor;
 tm->dt = dt * factor;
}

int find_inter_curve(struct TrimmedMesh* tm)	/* FIXME: a lot of work needed */
{
 struct Vector4D p1,p2,x,x0;
/* BSplinePatchTrimmedObject::vector4 x, x0;*/
/* std::pair<PointStruct*,PointStruct*> p;*/
/* std::pair<std::list<PointStruct*>, std::list<PointStruct*> > trimCurveTemp;*/
 struct Vector4D np, nq, td, Pu, Pv, Qs, Qt;
 double tlen;
 double du, dv, ds, dt;
 double W;
 int i;
 struct Mesh4D *m1, *m2;
/* return;*/
 TRACE
 m1 = tm->m1;
 m2 = tm->m2;
 tm->d = 1e0;
 zero_vector(&p1);
 zero_vector(&p2);
 zero_vector(&x);
 zero_vector(&x0);
 zero_vector(&np);
 zero_vector(&nq);
 zero_vector(&td);
 zero_vector(&Pu);
 zero_vector(&Pv);
 zero_vector(&Qs);
 zero_vector(&Qt);
 /*nurb_computemv(m2, &p1, 0. , 0. ); printf_vector("nurb(0. ,0. )", &p1);
 nurb_computemv(m2, &p1, 0. ,  .5); printf_vector("nurb(0. , .5)", &p1);
 nurb_computemv(m2, &p1, 0. , 1. ); printf_vector("nurb(0. ,1. )", &p1);
 nurb_computemv(m2, &p1, 0. , 1.5); printf_vector("nurb(0. ,1.5)", &p1);
 nurb_computemv(m2, &p1, 0. , 2. ); printf_vector("nurb(0. ,2. )", &p1);
 nurb_computemv(m2, &p1,  .5, 0. ); printf_vector("nurb( .5,0. )", &p1);
 nurb_computemv(m2, &p1,  .5,  .5); printf_vector("nurb( .5, .5)", &p1);
 nurb_computemv(m2, &p1,  .5, 1. ); printf_vector("nurb( .5,1. )", &p1);
 nurb_computemv(m2, &p1,  .5, 1.5); printf_vector("nurb( .5,1.5)", &p1);
 nurb_computemv(m2, &p1,  .5, 2. ); printf_vector("nurb( .5,2. )", &p1);
 nurb_computemv(m2, &p1, 1. , 0. ); printf_vector("nurb(1. ,0. )", &p1);
 nurb_computemv(m2, &p1, 1. ,  .5); printf_vector("nurb(1. , .5)", &p1);
 nurb_computemv(m2, &p1, 1. , 1. ); printf_vector("nurb(1. ,1. )", &p1);
 nurb_computemv(m2, &p1, 1. , 1.5); printf_vector("nurb(1. ,1.5)", &p1);
 nurb_computemv(m2, &p1, 1. , 2. ); printf_vector("nurb(1. ,2. )", &p1);
 nurb_computemv(m2, &p1, 1.5, 0. ); printf_vector("nurb(1.5,0. )", &p1);
 nurb_computemv(m2, &p1, 1.5,  .5); printf_vector("nurb(1.5, .5)", &p1);
 nurb_computemv(m2, &p1, 1.5, 1. ); printf_vector("nurb(1.5,1. )", &p1);
 nurb_computemv(m2, &p1, 1.5, 1.5); printf_vector("nurb(1.5,1.5)", &p1);
 nurb_computemv(m2, &p1, 1.5, 2. ); printf_vector("nurb(1.5,2. )", &p1);
 nurb_computemv(m2, &p1, 2. , 0. ); printf_vector("nurb(2. ,0. )", &p1);
 nurb_computemv(m2, &p1, 2. ,  .5); printf_vector("nurb(2. , .5)", &p1);
 nurb_computemv(m2, &p1, 2. , 1. ); printf_vector("nurb(2. ,1. )", &p1);
 nurb_computemv(m2, &p1, 2. , 1.5); printf_vector("nurb(2. ,1.5)", &p1);
 nurb_computemv(m2, &p1, 2. , 2. ); printf_vector("nurb(2. ,2. )", &p1);*/
 /* X will be used with 4 coords */
 if (!find_initial_point(tm, &p1, &p2, &x)) { printf("Failed to find initial point!\n"); return 0; }
/* printf_vector("p1", &p1);*/
/* printf_vector("p2", &p2);*/
/* printf_vector("x", &x);*/
/* exit(1);*/
 /* good starts here! */
 copy_vector(&x0, &x);
 nurb_computemv(m1, &Pu, x.p[0], x.p[1]);
 nurb_computemv(m2, &Pv, x.p[2], x.p[3]);
/* printf_vector("NURB1V", &Pu);*/
/* printf_vector("NURB2V", &Pv);*/
/* printf("distance: %f\n", vector_distance(&Pu, &Pv));*/
 nurb_compute_dumv(m1, &Pu, x.p[0], x.p[1]);
 nurb_compute_dvmv(m1, &Pv, x.p[0], x.p[1]);
 nurb_compute_dumv(m2, &Qs, x.p[2], x.p[3]);
 nurb_compute_dvmv(m2, &Qt, x.p[2], x.p[3]);
/* printf_vector("Pu", &Pu);*/
/* printf_vector("Pv", &Pv);*/
/* printf_vector("Qs", &Qs);*/
/* printf_vector("Qt", &Qt);*/
 /*Pu = surfaces.first->du(x.p[0],x.p[1]); 
 Pv = surfaces.first->dv(x.p[0],x.p[1]);
 Qs = surfaces.second->du(x.p[2],x.p[3]); 
 Qt = surfaces.second->dv(x.p[2],x.p[3]);
 np = PointStruct::crossProduct(Pu, Pv);
 nq = PointStruct::crossProduct(Qs, Qt);
 t = PointStruct::crossProduct(np,nq);*/
 vector_mul(&np, &Pu, &Pv);
 vector_mul(&nq, &Qs, &Qt);
 vector_mul(&tm->t, &np, &nq);
/* printf_vector("np", &np);*/
/* printf_vector("nq", &nq);*/
/* printf_vector("t", &tm->t);*/
 tlen = vector_length(&tm->t);
/* printf("tlen = %f\n", tlen);*/
 if (tlen == 0)
   {
    printf("error 1: vector has length  0\n");
  /*  while (!trimCurveTemp.first.empty())
     {
      addPoint(trimCurveTemp.second.back());
      trimCurve.first.addPoint(trimCurveTemp.first.back());
      trimCurve.second.addPoint(trimCurveTemp.second.back());
      trimCurveTemp.first.pop_back();
      trimCurveTemp.second.pop_back();
     } */
    return 0;
  }
  for (i=0;i<DIM-1;i++) 
    {
     tm->t.p[i] /= tlen;
     td.p[i] = tm->t.p[i] * tm->d;  
    }
/*  printf_vector("td", &td);*/
  {
   W = Pu.p[0]*Pv.p[1]-Pv.p[0]*Pu.p[1];
/*   printf("W1 = %f\n", W);*/
   if (W!=0)
     {
      du = td.p[0] * Pv.p[1] - Pv.p[0] * td.p[1]; 
      du /= W;
      dv = Pu.p[0] * td.p[1] - td.p[0] * Pu.p[1]; 
      dv /= W;
     } 
   else
     {
      W = Pu.p[1] * Pv.p[2] - Pv.p[1] * Pu.p[2];
      if (W!=0)
	 {
	  du = td.p[1] * Pv.p[2] - Pv.p[1] * td.p[2]; 
	  du /= W;
	  dv = Pu.p[1] * td.p[2] - td.p[1] * Pu.p[2]; 
	  dv /= W;
	 } 
      else
	{
	 du = 0; 
	 dv = 0;
	}
      }
   W = Qs.p[0] * Qt.p[1] - Qt.p[0] * Qs.p[1];
/*   printf("W2 = %f\n", W);*/
   if (W!=0)
     {
      ds = td.p[0] * Qt.p[1] - Qt.p[0] * td.p[1]; 
      ds /= W;
      dt = Qs.p[0] * td.p[1] - td.p[0] * Qs.p[1]; 
      dt /= W;
     } 
   else
     {
      W = Qs.p[1] * Qt.p[2] - Qt.p[1] * Qs.p[2];
      if (W!=0)
	{
	 ds = td.p[1] * Qt.p[2] - Qt.p[1] * td.p[2]; ds /= W;
	 dt = Qs.p[1] * td.p[2] - td.p[1] * Qs.p[2]; dt /= W;
	} 
      else
	{
	 ds = 0; 
	 dt = 0;
	}
      }
   }
/*  printf("du,dv,ds,dt = %f,%f,%f,%f\n", du,dv,ds,dt);*/
  set_param_vector(tm, du, dv, ds, dt, 1.);
  x.p[0] += tm->du; 
  x.p[1] += tm->dv; 
  x.p[2] += tm->ds; 
  x.p[3] += tm->dt;
  if ((x.p[0]>m1->nX) || (x.p[1]>m1->nY) || (x.p[2]>m2->nX) || (x.p[3]>m2->nY) || (x.p[0]<0) || (x.p[1]<0) || (x.p[2]<0) || (x.p[3]<0))
    {
     printf("error 2: first patch beyond domain\n");
/*     printf_vector("x", &x);*/
     tm->d = -tm->d;
     td.p[0] = tm->t.p[0] * tm->d;  
     td.p[1] = tm->t.p[1] * tm->d; 
     td.p[2] = tm->t.p[2] * tm->d;
       {
	W = Pu.p[0] * Pv.p[1] - Pv.p[0] * Pu.p[1];
	if (W!=0)
	   {
	    du = td.p[0] * Pv.p[1] - Pv.p[0] * td.p[1]; du /= W;
	    dv = Pu.p[0] * td.p[1] - td.p[0] * Pu.p[1]; dv /= W;
	   } 
	else
	   {
	    W = Pu.p[1] * Pv.p[2] - Pv.p[1] * Pu.p[2];
	    if (W!=0)
	       {
		du = td.p[1] * Pv.p[2] - Pv.p[1] * td.p[2]; 
		du /= W;
		dv = Pu.p[1] * td.p[2] - td.p[1] * Pu.p[2]; 
		dv /= W;
	       } 
	    else
	       {
		du = 0; 
		dv = 0;
	       }
	    }
	 W = Qs.p[0] * Qt.p[1] - Qt.p[0] * Qs.p[1];
	 if (W!=0)
	   {
	    ds = td.p[0] * Qt.p[1] - Qt.p[0] * td.p[1]; 
	    ds /= W;
	    dt = Qs.p[0] * td.p[1] - td.p[0] * Qs.p[1]; 
	    dt /= W;
	   } 
	 else
	   {
	    W = Qs.p[1] * Qt.p[2] - Qt.p[1] * Qs.p[2];
	    if (W!=0)
	      {
	       ds = td.p[1] * Qt.p[2] - Qt.p[1] * td.p[2]; 
	       ds /= W;
	       dt = Qs.p[1] * td.p[2] - td.p[1] * Qs.p[2]; 
	       dt /= W;
	      } 
	    else
	      {
	       ds = 0; 
	       dt = 0;
	      }
	    }
         }
/*        printf("du,dv,ds,dt = %f,%f,%f,%f\n", du,dv,ds,dt);*/
  set_param_vector(tm, du, dv, ds, dt, 1.);
  x.p[0] += tm->du; 
  x.p[1] += tm->dv; 
  x.p[2] += tm->ds; 
  x.p[3] += tm->dt;
	if ((x.p[0]>m1->nX) || (x.p[1]>m1->nY) || (x.p[2]>m2->nX) || (x.p[3]>m2->nY) || (x.p[0]<0) || (x.p[1]<0) || (x.p[2]<0) || (x.p[3]<0))
	  {
	   printf("error 3: second patch beyond domain\n");
	   return 0;
	  }
       }
    nurb_computemv(m1, &p1, x.p[0], x.p[1]);
    nurb_computemv(m2, &p2, x.p[2], x.p[3]);
/*    printf_vector("4dv", &x);*/
/*    printf_vector("p1", &p1);*/
/*    printf_vector("p2", &p2);*/
/*    p.first = new PointStruct(surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[0], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[1], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[2]);*/
/*    p.second = new PointStruct(surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[0], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[1], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[2]);*/
    tm->P0.p[0] = p1.p[0];
    tm->P0.p[1] = p1.p[1];
    tm->P0.p[2] = p1.p[2];
/*    printf_vector("<P0 for root_finder>", &tm->P0);*/
/*    printf_vector("<x for root_finder>", &x);*/
    for (i=0;i<=1;i++)
      {
       while (root_finder(tm, &x))
	 {
	  if((x.p[0]>m1->nX) || (x.p[1]>m1->nY) || (x.p[2]>m2->nX) || (x.p[3]>m2->nY) || (x.p[0]<0) || (x.p[1]<0) || (x.p[2]<0) || (x.p[3]<0))
	    {
	     printf("error 4: root_finder leads to nowwhere\n");
	     break;
	    }
        nurb_computemv(m1, &p1, x.p[0], x.p[1]);
        nurb_computemv(m2, &p2, x.p[2], x.p[3]);
/*	printf("distance = %f\n", vector_distance(&p1, &p2));*/
/*	p.first = new PointStruct(surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[0], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[1], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[2]);*/
/*	p.second = new PointStruct(surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[0], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[1], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[2]);*/
	if (i==0)
	   {
	    /*trimCurveTemp.first.push_back(p.first);
	    trimCurveTemp.second.push_back(p.second);
	    uvs.first.push_back(std::pair<double, double>(x.p[0],x.p[1]));
	    uvs.second.push_back(std::pair<double, double>(x.p[2],x.p[3]));*/
	   } 
	else
	   {
	    /*trimCurveTemp.first.push_front(p.first);
	    trimCurveTemp.second.push_front(p.second);
	    uvs.first.push_back(std::pair<double, double>(x.p[0],x.p[1]));
	    uvs.second.push_back(std::pair<double, double>(x.p[2],x.p[3]));*/
	   }
        nurb_compute_dumv(m1, &Pu, x.p[0], x.p[1]);
        nurb_compute_dvmv(m1, &Pv, x.p[0], x.p[1]);
        nurb_compute_dumv(m2, &Qs, x.p[2], x.p[3]);
        nurb_compute_dvmv(m2, &Qt, x.p[2], x.p[3]);
        vector_mul(&np, &Pu, &Pv);
        vector_mul(&nq, &Qs, &Qt);
        vector_mul(&tm->t, &np, &nq);
        tlen = vector_length(&tm->t);
	/*Pu = surfaces.first->du(x.p[0],x.p[1]); Pv = surfaces.first->dv(x.p[0],x.p[1]);
	Qs = surfaces.second->du(x.p[2],x.p[3]); Qt = surfaces.second->dv(x.p[2],x.p[3]);
	np = PointStruct::crossProduct(Pu, Pv);
	nq = PointStruct::crossProduct(Qs, Qt);
	t = PointStruct::crossProduct(np,nq);
	tlen = t.len();*/
	if (tlen == 0)
	  {
	   printf("error 5: vector has length 0\n");
	   /*while (!trimCurveTemp.first.empty())
	     {
	      addPoint(trimCurveTemp.first.back());
	      trimCurve.first.addPoint(trimCurveTemp.first.back());
	      trimCurve.second.addPoint(trimCurveTemp.second.back());
	      trimCurveTemp.first.pop_back();
	      trimCurveTemp.second.pop_back();
	     }*/
	   return 0;
	  }
	tm->t.p[0] /= tlen; 
	tm->t.p[1] /= tlen; 
	tm->t.p[2] /= tlen;			
	td.p[0] = tm->t.p[0] * tm->d;  
	td.p[1] = tm->t.p[1] * tm->d; 
	td.p[2] = tm->t.p[2] * tm->d;
	  {
	   W = Pu.p[0] * Pv.p[1] - Pv.p[0] * Pu.p[1];
	   if(W!=0)
	      {
	       du = td.p[0] * Pv.p[1] - Pv.p[0] * td.p[1]; 
	       du /= W;
	       dv = Pu.p[0] * td.p[1] - td.p[0] * Pu.p[1]; 
	       dv /= W;
	      } 
	   else
	      {
	       W = Pu.p[1] * Pv.p[2] - Pv.p[1] * Pu.p[2];
	       if (W!=0)
		  {
		   du = td.p[1] * Pv.p[2] - Pv.p[1] * td.p[2]; 
		   du /= W;
		   dv = Pu.p[1] * td.p[2] - td.p[1] * Pu.p[2]; 
		   dv /= W;
		  } 
	       else
		  {
		   du = 0; 
		   dv = 0;
		  }
	       }
	     W = Qs.p[0] * Qt.p[1] - Qt.p[0] * Qs.p[1];
	     if (W!=0)
		{
		 ds = td.p[0] * Qt.p[1] - Qt.p[0] * td.p[1]; 
		 ds /= W;
		 dt = Qs.p[0] * td.p[1] - td.p[0] * Qs.p[1]; 
		 dt /= W;
		} 
	     else
		{
		 W = Qs.p[1] * Qt.p[2] - Qt.p[1] * Qs.p[2];
		 if (W!=0)
		   {
		    ds = td.p[1] * Qt.p[2] - Qt.p[1] * td.p[2]; 
		    ds /= W;
		    dt = Qs.p[1] * td.p[2] - td.p[1] * Qs.p[2]; 
		    dt /= W;
		   } 
		 else
		   {
		    ds = 0; 
		    dt = 0;
		   }
		}
	}
  set_param_vector(tm, du, dv, ds, dt, 1.);
  x.p[0] += tm->du; 
  x.p[1] += tm->dv; 
  x.p[2] += tm->ds; 
  x.p[3] += tm->dt;
       if ((x.p[0]>m1->nX) || (x.p[1]>m1->nY) || (x.p[2]>m2->nX) || (x.p[3]>m2->nY) || (x.p[0]<0) || (x.p[1]<0) || (x.p[2]<0) || (x.p[3]<0))
	 {
/*	  printf_vector("correction x", &x);*/
	  printf("error 6: beyond domain\n");
	  break;
	 }
       nurb_computemv(m1, &p1, x.p[0], x.p[1]);
       nurb_computemv(m2, &p2, x.p[2], x.p[3]);
/*       p.first = new PointStruct(surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[0], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[1], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[2]);*/
/*       p.second = new PointStruct(surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[0], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[1], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[2]);*/
       tm->P0.p[0] = p1.p[0];
       tm->P0.p[1] = p1.p[1];
       tm->P0.p[2] = p1.p[2];
/*       if (trimCurveTemp.first.size() > 10000) break;*/
      }
   copy_vector(&x, &x0);
   tm->d = -tm->d;
   nurb_computemv(m1, &p1, x.p[0], x.p[1]);
   nurb_computemv(m2, &p2, x.p[2], x.p[3]);
/*   p.first = new PointStruct(surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[0], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[1], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[2]);*/
/*   p.second = new PointStruct(surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[0], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[1], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[2]);*/
/*   trimCurveTemp.first.push_back(p.first);*/
/*   trimCurveTemp.second.push_back(p.second);*/
    nurb_compute_dumv(m1, &Pu, x.p[0], x.p[1]);
    nurb_compute_dvmv(m1, &Pv, x.p[0], x.p[1]);
    nurb_compute_dumv(m2, &Qs, x.p[2], x.p[3]);
    nurb_compute_dvmv(m2, &Qt, x.p[2], x.p[3]);
    vector_mul(&np, &Pu, &Pv);
    vector_mul(&nq, &Qs, &Qt);
    vector_mul(&tm->t, &np, &nq);
    tlen = vector_length(&tm->t);
    /*Pu = surfaces.first->du(x.p[0],x.p[1]); Pv = surfaces.first->dv(x.p[0],x.p[1]);
    Qs = surfaces.second->du(x.p[2],x.p[3]); Qt = surfaces.second->dv(x.p[2],x.p[3]);
    np = PointStruct::crossProduct(Pu, Pv);
    nq = PointStruct::crossProduct(Qs, Qt);
    t = PointStruct::crossProduct(np,nq);
    tlen = t.len();*/
    if (tlen == 0)
      {
       printf("error 7:vector has length 0\n");
       /*while (!trimCurveTemp.first.empty())
	  {
	   addPoint(trimCurveTemp.first.back());
	   trimCurve.first.addPoint(trimCurveTemp.first.back());
	   trimCurve.second.addPoint(trimCurveTemp.second.back());
	   trimCurveTemp.first.pop_back();
	   trimCurveTemp.second.pop_back();
	  }*/
       return 0;
      }
    tm->t.p[0] /= tlen; 
    tm->t.p[1] /= tlen; 
    tm->t.p[2] /= tlen;
    td.p[0] = tm->t.p[0] * tm->d;  
    td.p[1] = tm->t.p[1] * tm->d; 
    td.p[2] = tm->t.p[2] * tm->d;
      {
       W = Pu.p[0] * Pv.p[1] - Pv.p[0] * Pu.p[1];
       if (W!=0)
	 {
	  du = td.p[0] * Pv.p[1] - Pv.p[0] * td.p[1]; 
	  du /= W;
	  dv = Pu.p[0] * td.p[1] - td.p[0] * Pu.p[1]; 
	  dv /= W;
	 } 
       else
	 {
	  W = Pu.p[1] * Pv.p[2] - Pv.p[1] * Pu.p[2];
	  if (W!=0)
	    {
	     du = td.p[1] * Pv.p[2] - Pv.p[1] * td.p[2]; 
	     du /= W;
	     dv = Pu.p[1] * td.p[2] - td.p[1] * Pu.p[2]; 
	     dv /= W;
	    } 
	  else
	    {
	     du = 0; 
	     dv = 0;
	    }
	}
      W = Qs.p[0] * Qt.p[1] - Qt.p[0] * Qs.p[1];
      if (W!=0)
	 {
	  ds = td.p[0] * Qt.p[1] - Qt.p[0] * td.p[1]; 
	  ds /= W;
	  dt = Qs.p[0] * td.p[1] - td.p[0] * Qs.p[1]; 
	  dt /= W;
	 } 
      else
	 {
	  W = Qs.p[1] * Qt.p[2] - Qt.p[1] * Qs.p[2];
	  if (W!=0)
	    {
	     ds = td.p[1] * Qt.p[2] - Qt.p[1] * td.p[2]; 
	     ds /= W;
	     dt = Qs.p[1] * td.p[2] - td.p[1] * Qs.p[2]; 
	     dt /= W;
	    } 
	  else
	    {
	     ds = 0; 
	     dt = 0;
	    }
	 }
      }
/*  printf_vector("x", &x);*/
  set_param_vector(tm, du, dv, ds, dt, 1.);
/*  printf("TM-> (%f,%f,%f,%f)", du,dv,ds,dt);*/
  x.p[0] += tm->du; 
  x.p[1] += tm->dv; 
  x.p[2] += tm->ds; 
  x.p[3] += tm->dt;
/*  if (x.p[0] < 0.) x.p[0] += m1->nX;
  if (x.p[1] < 0.) x.p[1] += m1->nY;
  if (x.p[2] < 0.) x.p[2] += m2->nX;
  if (x.p[3] < 0.) x.p[3] += m2->nY;*/
/*  printf_vector("x", &x);*/
    if ((x.p[0]>m1->nX) || (x.p[1]>m1->nY) || (x.p[2]>m2->nX) || (x.p[3]>m2->nY) || (x.p[0]<0) || (x.p[1]<0) || (x.p[2]<0) || (x.p[3]<0))
       {
/*	printf_vector("correction x", &x);*/
	printf("error 8: patch beyond domain\n");
	break;
       }
       nurb_computemv(m1, &p1, x.p[0], x.p[1]);
       nurb_computemv(m2, &p2, x.p[2], x.p[3]);
/*       p.first = new PointStruct(surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[0], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[1], surfaces.first->solveBSplineCurve(x.p[0],x.p[1]).p[2]);*/
/*       p.second = new PointStruct(surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[0], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[1], surfaces.second->solveBSplineCurve(x.p[2],x.p[3]).p[2]);*/
       tm->P0.p[0] = p1.p[0];
       tm->P0.p[1] = p1.p[1];
       tm->P0.p[2] = p1.p[2];
      }
  /*while (!trimCurveTemp.first.empty())
    {
     addPoint(trimCurveTemp.first.back());
     trimCurve.first.addPoint(trimCurveTemp.first.back());
     trimCurve.second.addPoint(trimCurveTemp.second.back());
     trimCurveTemp.first.pop_back();
     trimCurveTemp.second.pop_back();
    }*/
  printf("FINISHED SEARCHING.\n");
  return 1;
}


int trimm(struct TrimmedMesh* tm)		/* FIXME unfinished */
{
 /*int i1,j1,i2,j2;
 struct VList *head;
 struct NurbPair* tmpair;
 struct Mesh4D *m1, *m2;
 m1 = tm->m1;
 m2 = tm->m2;
 TRACE
 head = NULL;
 for (i1=0;i1<m1->nX;i1++)
 for (j1=0;j1<m1->nY;j1++)
 for (i2=0;i2<m2->nX;i2++)
 for (j2=0;j2<m2->nY;j2++)
   {
    tmpair = (struct NurbPair*)malloc(sizeof(struct NurbPair));
    tmpair->n1 = m1->nurbs[i1][j1];
    tmpair->n2 = m2->nurbs[i2][j2];
    if (nurb_intersection_box(tmpair->n1, tmpair->n2)) 
       if (nurb_pseudo_intersection_box(tmpair->n1, tmpair->n2)) vlist_add(&head, (void*)tmpair);
   }*/
 zero_trimm_curves(tm);
 return find_inter_curve(tm);
}

void map_curve_to_matrix(struct TrimmedMesh* tm, int idx, struct VList* curve, unsigned char** m, int size, int nx, int ny)
{
 struct VList* tmp;
 struct Vector4D* cv;
 struct Vector4D V0, V1;
 struct Mesh4D* mesh;
 struct Vector4D** pts;
 double u,v;
 int i,j;
 int blank;
 tmp = curve;
 if (!tmp) return;
 while (tmp)
   {
    cv = (struct Vector4D*)tmp->data;
/*    printf_vector("cv", cv);*/
    if (idx == 0) { pts = tm->points1; mesh = tm->m1; u = cv->p[0]; v = cv->p[1]; }
    else          { pts = tm->points2; mesh = tm->m2; u = cv->p[2]; v = cv->p[3]; }
    i = (int)(((double)u*(double)size)/(double)nx);
    j = (int)(((double)v*(double)size)/(double)ny);
    m[i][j] = 1;
    nurb_computemv(mesh, &V0, u, v);
    nurb_computemv(mesh, &V0, u+.1, v+.1);
/*    copy_vector(&V1, &pts[i][j]);*/
    printf("%f, %f -> %d, %d for (%d,%d,%d) ===> %f\n", u,v,i,j,size,nx,ny,vector_distance(&V0, &V1));
    tmp = tmp->next;
   }
 /*printf("matrix before\n");
 for (i=0;i<size;i++)
   {
    for (j=0;j<size;j++) printf("%d", m[i][j]);
    printf("\n");
   }
 printf("\n");*/
 for (i=0;i<size;i++)
   {
    blank = 0;
    for (j=0;j<size;j++)
     {
      if ((j != size-1 && m[i][j] == 1 && !m[i][j+1])) blank = !blank;
      if (blank) m[i][j] = 2;
     }
    if (blank) m[i][size-1] = 2;
   }
 /*printf("matrix affter\n");
 for (i=0;i<size;i++)
   {
    for (j=0;j<size;j++) printf("%d", m[i][j]);
    printf("\n");
   }
 printf("\n");*/
}

void print_curve_distance(struct TrimmedMesh* tm)
{
 struct VList *c1, *c2, *uv;
 struct Vector4D *v1, *v2, *vp;
 struct Vector4D vp1, vp2;
 double distanceUV;
 double distancePT;
 c1 = tm->tc1;
 c2 = tm->tc2;
 uv = tm->tcuv;
 distanceUV = 0.;
 distancePT = 0.;
 while (c1 && c2 && uv)
   {
    v1 = (struct Vector4D*)c1->data;
    v2 = (struct Vector4D*)c2->data;
    vp = (struct Vector4D*)uv->data;
    distancePT += vector_distance(v1, v2);
    nurb_computemv(tm->m1, &vp1, vp->p[0], vp->p[1]);
    nurb_computemv(tm->m2, &vp2, vp->p[2], vp->p[3]);
    distanceUV += vector_distance(&vp1, &vp2);
    c1 = c1->next;
    c2 = c2->next;
    uv = uv->next;
   }
 printf("distancdUV = %f, distancePT = %f\n", distanceUV, distancePT);
}

void gen_trimm_matrices_with_matrix(struct TrimmedMesh* tm, int size)
{
 int i,j;
 double u1,v1,u2,v2;
 if (!tm) return;
 tm->points1 = (struct Vector4D**)malloc(size*sizeof(struct Vector4D*));
 for (i=0;i<size;i++) tm->points1[i] = (struct Vector4D*)malloc(size*sizeof(struct Vector4D));
 tm->matrix1 = (unsigned char**)malloc(size*sizeof(unsigned char*));
 for (i=0;i<size;i++) tm->matrix1[i] = (unsigned char*)malloc(size*sizeof(unsigned char));
 tm->points2 = (struct Vector4D**)malloc(size*sizeof(struct Vector4D*));
 for (i=0;i<size;i++) tm->points2[i] = (struct Vector4D*)malloc(size*sizeof(struct Vector4D));
 tm->matrix2 = (unsigned char**)malloc(size*sizeof(unsigned char*));
 for (i=0;i<size;i++) tm->matrix2[i] = (unsigned char*)malloc(size*sizeof(unsigned char));
 for (i=0;i<size;i++)
 for (j=0;j<size;j++)
   {
    u1 = (double)i/(double)tm->m1->nX;
    u2 = (double)i/(double)tm->m2->nX;
    v1 = (double)j/(double)tm->m1->nY;
    v2 = (double)j/(double)tm->m2->nY;
/*    printf("%f,%f : %f,%f\n",u1,v1,u2,v2);*/
    nurb_computemv(tm->m1, &tm->points1[i][j], u1, v1);
    nurb_computemv(tm->m2, &tm->points2[i][j], u2, v2);
    tm->matrix1[i][j] = tm->matrix2[i][j] = 0;
   }
 tm->npx1 = tm->npx2 = tm->npy1 = tm->npy2 = size;
 map_curve_to_matrix(tm, 0, tm->tcuv, tm->matrix1, size, tm->m1->nX, tm->m1->nY); 
 map_curve_to_matrix(tm, 1, tm->tcuv, tm->matrix2, size, tm->m2->nX, tm->m2->nY); 
 for (i=0;i<size;i++)
   {
    if (tm->matrix1[i][size-1] == 2)
    { 
     j = size-1;
     while (tm->matrix1[i][j] == 2) { tm->matrix1[i][j] = 0; j--; }
    }
   }
 /*printf("matrix affter all\n");
 for (i=0;i<size;i++)
   {
    for (j=0;j<size;j++) printf("%d", tm->matrix1[i][j]);
    printf("\n");
   }
 printf("\n");*/
 print_curve_distance(tm);
}

void gen_trimm_matrices(struct TrimmedMesh* tm, int sizex1, int sizey1, int sizex2, int sizey2)
{
 int i,j;
 double u1,v1,u2,v2;
 if (!tm) return;
 tm->points1 = (struct Vector4D**)malloc(sizex1*sizeof(struct Vector4D*));
 for (i=0;i<sizex1;i++) tm->points1[i] = (struct Vector4D*)malloc(sizey1*sizeof(struct Vector4D));
 tm->points2 = (struct Vector4D**)malloc(sizex2*sizeof(struct Vector4D*));
 for (i=0;i<sizex2;i++) tm->points2[i] = (struct Vector4D*)malloc(sizey2*sizeof(struct Vector4D));
 for (i=0;i<sizex1;i++)
 for (j=0;j<sizey1;j++)
   {
    u1 = (double)i/(double)tm->m1->nX;
    v1 = (double)j/(double)tm->m1->nY;
/*    printf("%f,%f : %f,%f\n",u1,v1,u2,v2);*/
    nurb_computemv(tm->m1, &tm->points1[i][j], u1, v1);
   }
 for (i=0;i<sizex2;i++)
 for (j=0;j<sizey2;j++)
   {
    u2 = (double)i/(double)tm->m2->nX;
    v2 = (double)j/(double)tm->m2->nY;
/*    printf("%f,%f : %f,%f\n",u1,v1,u2,v2);*/
    nurb_computemv(tm->m2, &tm->points2[i][j], u2, v2);
   }
 tm->npx1 = sizex1;
 tm->npx2 = sizex2; 
 tm->npy1 = sizey1;
 tm->npy2 = sizey2;
 print_curve_distance(tm);
}

