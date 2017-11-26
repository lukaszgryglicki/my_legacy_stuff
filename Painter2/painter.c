/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#define PI  3.1415926F
#define PI2 6.2831853F
#define GLuchar unsigned char
#define HERE __FILE__,__LINE__,__DATE__,__TIME__

/*#define DEBUG*/
GLint wx,wy;
GLint cx,cy;
GLfloat angX, angY, angZ, scalef;
time_t t1,t2;
GLint fps;
GLint want_tex;
GLint auto_move;
GLint want_light;
GLint si;

typedef struct _V
{
 GLdouble x,y,z;
 GLdouble nx,ny,nz;
 GLdouble tx,ty;
} Vertex;

typedef struct _L
{
 GLdouble dx,dy,dz;
 GLdouble px,py,pz;
 Vertex *va, *vb;
 GLdouble A,B,C;
} Line;

typedef struct _P
{
 GLint n;
 Vertex* v;
 GLint flag;
} Poly;

typedef struct _S
{
 GLdouble A,B,C,D;
} Surface;

typedef struct _T
{
 GLuint id;
 GLuint sizx,sizy;
 GLuchar *data;
} Texture;


GLint nPoly;
GLint nPoly_buf;
GLint nVert;
GLint nTex;
GLint maxPolys;
Vertex* vList;
Poly* pList;
Poly* pList_buf;
Texture* tList;
GLint nt1, nt1p, nt2, nt2p, nt3, nt3p, nt4, nt4p, nt5, nt5p;
GLint glob_alloc;

void* mm_malloc(char* file, int line, char* date, char* time, size_t nbytes)
{
 void* addr;
 addr = malloc(nbytes);
 glob_alloc += nbytes;
/* printf("malloc called from %s, line %d, compiled: %s %s, wants %d bytes, pointer is %p, global: %d\n", file, line, date, time, (int)nbytes, addr, glob_alloc);*/
/* printf("%d\n", line);*/
/* fflush(stdout);*/
 return addr;
}

void mm_free(char* file, int line, char* date, char* time, void** restr_ptr)
{
/* printf("free called from %s, line %d, compiled: %s %s, on %p pointer, global: %d\n", file, line, date, time, restr_ptr, glob_alloc);*/
/* printf("%d\n", line);*/
/* fflush(stdout);*/
 if (*restr_ptr) free(*restr_ptr);
 *restr_ptr = NULL;
}

void fatal(char* par)
{
 printf("Fatal error: %s\n", par?par:"(null)");
 exit(3);
}

void print_polygon(Poly*);


GLint poly_closer_zmin(GLint pI, GLdouble zmin)
{
 Poly* p;
 GLint i;
 p = &pList[pI];
 for (i=0;i<p->n;i++) if (p->v[i].z < zmin) return 1;
 return 0;
}

GLdouble poly_zmin(GLint pI)
{
 Poly* p;
 GLint i;
 GLdouble min = 1e10;
 p = &pList[pI];
 for (i=0;i<p->n;i++) if (p->v[i].z < min) min = p->v[i].z;
 return min;
}

GLdouble poly_z(Poly* p)
{
 GLint i;
 GLdouble min = 1e10;
 for (i=0;i<p->n;i++) if (p->v[i].z < min) min = p->v[i].z;
 return min;
}


GLdouble poly_zmax(GLint pI)
{
 Poly* p;
 GLint i;
 GLdouble max = -1e10;
 p = &pList[pI];
 for (i=0;i<p->n;i++) if (p->v[i].z > max) max = p->v[i].z;
 return max;
}

GLdouble poly_ymin(GLint pI)
{
 Poly* p;
 GLint i;
 GLdouble min = 1e10;
 p = &pList[pI];
 for (i=0;i<p->n;i++) if (p->v[i].y < min) min = p->v[i].y;
 return min;
}

GLdouble poly_ymax(GLint pI)
{
 Poly* p;
 GLint i;
 GLdouble max = -1e10;
 p = &pList[pI];
 for (i=0;i<p->n;i++) if (p->v[i].y > max) max = p->v[i].y;
 return max;
}

GLdouble poly_xmin(GLint pI)
{
 Poly* p;
 GLint i;
 GLdouble min = 1e10;
 p = &pList[pI];
 for (i=0;i<p->n;i++) if (p->v[i].x < min) min = p->v[i].x;
 return min;
}

GLdouble poly_xmax(GLint pI)
{
 Poly* p;
 GLint i;
 GLdouble max = -1e10;
 p = &pList[pI];
 for (i=0;i<p->n;i++) if (p->v[i].x > max) max = p->v[i].x;
 return max;
}

void copy_polygon(Poly* a, Poly* b)
{
 GLint i;
 a->n = b->n;
 a->v = (Vertex*)mm_malloc(HERE,b->n*sizeof(Vertex));
 for (i=0;i<a->n;i++) memcpy(&a->v[i], &b->v[i], sizeof(Vertex));
 a->flag = b->flag;
}

void free_polygon(Poly* a)
{
 if (!a) return;
 mm_free(HERE,(void*)&a->v);
}

void free_polygon_list(Poly* list, GLint n)
{
 GLint i;
 if (!list) return;
 for (i=0;i<n;i++) free_polygon(&list[i]);
}

void copy_from_buffer()
{
 GLint i;
 nPoly = nPoly_buf;
 free_polygon_list(pList, nPoly);
 for (i=0;i<nPoly;i++) copy_polygon(&pList[i], &pList_buf[i]);
}

void poly_swap(GLint I, GLint J)
{
 Poly temp;
 Poly *a, *b;
 a = &pList[I];
 b = &pList[J];
 copy_polygon(&temp, a);
 mm_free(HERE,(void*)&a->v);
 copy_polygon(a, b);
 mm_free(HERE,(void*)&b->v);
 copy_polygon(b, &temp);
 mm_free(HERE,(void*)&temp.v);
}

void sort_by_zmin()
{
 GLint i,j;
 GLint minidx;
 GLdouble zmin;
 for (i=0;i<nPoly;i++)
   {
    minidx = i;
    zmin = poly_zmin(minidx);
    for (j=i+1;j<nPoly;j++) 
      {
       if (poly_closer_zmin(j, zmin)) 
         {
	  minidx = j;
	  zmin = poly_zmin(minidx);
	 }
      }
    if (minidx != i) poly_swap(minidx, i);
   }
/* for (i=0;i<nPoly;i++) printf("%f\n", poly_zmin(i));*/
}


GLint nearly_equal(GLdouble x, GLdouble y, GLdouble eps)
{
 if (fabs(x-y) < eps) return 1;
 else return 0;
}

GLint nearly_equal_spec(GLdouble x, GLdouble y, GLdouble eps)
{
 if (fabs(x-y) < eps) return 1;
 else if (x >= 1e11 || y >= 1e11) return 0;
 else return 0;
}

void zero_stats()
{
 nt1 = nt2 = nt3 = nt4 = nt5 = 0;
 nt1p = nt2p = nt3p = nt4p = nt5p = 0;
}

void calculate_normal_poly(Poly* p, GLdouble* x, GLdouble* y, GLdouble* z)
{
 GLdouble x0,y0,z0,nx,ny,nz,xu,yu,zu,xv,yv,zv,len;
 x0 = p->v[1].x;
 y0 = p->v[1].y;
 z0 = p->v[1].z;
 xu = p->v[0].x;
 yu = p->v[0].y;
 zu = p->v[0].z;
 xv = p->v[2].x;
 yv = p->v[2].y;
 zv = p->v[2].z;
 xu -= x0;
 yu -= y0;
 zu -= z0;
 xv -= x0;
 yv -= y0;
 zv -= z0;
 nx = yu*zv - zu*yv;
 ny = zu*xv - xu*zv;
 nz = xu*yv - yu*xv;
 len = sqrt(nx*nx + ny*ny + nz*nz);
 *x = -nx/len;			/* FIXME with '-' or '+' ?? */
 *y = -ny/len;			/* CHYBA lepiej - */
 *z = -nz/len;
/* printf("normal is: %f %f %f\n", *x, *y, *z);*/

}

void calculate_normal(GLint I, GLdouble* x, GLdouble* y, GLdouble* z)
{
 Poly* p;
 p = &pList[I];
 calculate_normal_poly(p, x, y, z);
}

/* 0 -no inter, 1 - point, 2 - line */
GLint get_intersection(Line* l1, Line* l2, GLdouble* x, GLdouble* y)	/* FIXME: FLOATING POINT COMPARISONS */
{
 GLdouble a,b,c,d,e,f;
 a = l1->A;
 b = l1->B;
 e = -l1->C;
 c = l2->A;
 d = l2->B;
 f = -l2->C;
 if (nearly_equal(a, 0., 1e-9) && nearly_equal(b, 0., 1e-9)) return 0; /* dx=dy=0, degraded line */
 if (nearly_equal(c, 0., 1e-9) && nearly_equal(d, 0., 1e-9)) return 0; /* same */
 if (nearly_equal(b, 0., 1e-9)) /* then line a is line x = k */
   {
    *x = e/a;
    if (nearly_equal(d, 0., 1e-9)) /* if line b is also x = l */
      {					/* then check k == l */
       if (nearly_equal(c*(e/a), d, 1e-9))
         {
          *x = l1->px;		/* intersection is a line, any point (start too) matches */
          *y = l1->py;
	  return 2;
         }
       else return 0; /* k != l parallel to oY */
      }
    *y = (f-(c*(e/a)))/d;
    return 1;
   }
 if (nearly_equal(d, 0., 1e-9)) /* then line a is line x = k */
   {
    *x = f/c;			/* needn't check b == 0 checked above */
    *y = (e-(a*(f/c)))/b;
    return 1;
   }
 if (nearly_equal((a*d-c*b), 0., 1e-9))	/* y=ax+b parallel */
   {
    if (nearly_equal(e/b, f/d, 1e-9)) /* parallel and the same */
      {
       *x = l1->px;		/* intersection is a line, any point (start too) matches */
       *y = l1->py;
       return 2;
      }
    else return 0;
   }
 *x = (e*d-f*b)/(a*d-c*b);	/* most common case */
 *y = (e-a*(*x))/b;
 return 1;
}

Line* make_line(Vertex* a, Vertex* b)
{
 Line* l;
 if (!a || !b) return NULL;
 l = (Line*)mm_malloc(HERE,sizeof(Line));
 l->px = a->x;
 l->py = a->y;
 l->pz = a->z;
 l->dx = b->x - a->x;
 l->dy = b->y - a->y;
 l->dz = b->z - a->z;
 l->va = a;
 l->vb = b;
 if (nearly_equal(l->dx, 0., 1e-9) && nearly_equal(l->dy, 0., 1e-9))
  {
   l->A = l->B = l->C = 0.;
  }
 else if (l->dx != 0)
   {
    l->A = l->dy / l->dx;
    l->B = -1.;
    l->C = l->py - l->A*l->px;
   }
 else if (l->dy != 0)
   {
    l->A = -1.;
    l->B = l->dx / l->dy;
    l->C = l->px - l->B*l->py;
   }
 return l;
}

GLint point_in_line(GLdouble x, GLdouble y, GLdouble z, Line* l)	/* FIXME: is this function OK */
{
 GLdouble t1,t2,t3;
 t1 = t2 = t3 = 1e12;
 if (l->dx != 0) t1 = (x - l->px) / l->dx;
 if (l->dy != 0) t2 = (y - l->py) / l->dy;
 if (l->dz != 0) t3 = (z - l->pz) / l->dz;
 if (t1 < 1e10 && t2 < 1e10 && t3 < 1e10)
   {
    if ( nearly_equal(t1,t2,1e-8) && nearly_equal(t2,t3,1e-8) && nearly_equal(t1,t3,1e-8)) 
      {
/*       printf("point in line1\n");*/
       return 1;
      }
/*    printf("point not in line1\n");*/
    return 0;
   }
 else if (t1 >= 1e10 && t2 < 1e10 && t3 < 1e10)
   {
    if (nearly_equal(x, l->px, 1e-8) && nearly_equal(t2,t3, 1e-8))
      {
/*       printf("point in line2\n");*/
       return 1;
      }
/*    printf("point not in line2\n");*/
    return 0;
   }
 else if (t1 < 1e10 && t2 >= 1e10 && t3 < 1e10)
   {
    if (nearly_equal(y, l->py, 1e-8) && nearly_equal(t1,t3, 1e-8))
      {
/*       printf("point in line3\n");*/
       return 1;
      }
/*    printf("point not in line3\n");*/
    return 0;
   }
 else if (t1 < 1e10 && t2 < 1e10 && t3 >= 1e10)
   {
    if (nearly_equal(z, l->pz, 1e-8) && nearly_equal(t1,t2, 1e-8))
      {
/*       printf("point in line4\n");*/
       return 1;
      }
/*    printf("point not in line4\n");*/
    return 0;
   }
 else if (t1 >= 1e10 && t2 >= 1e10 && t3 < 1e10)
   {
    if (nearly_equal(x, l->px, 1e-8) && nearly_equal(y, l->py, 1e-8))
      {
/*       printf("point in line5\n");*/
       return 1;
      }
/*    printf("point not in line5\n");*/
    return 0;
   }
 else if (t1 >= 1e10 && t2 < 1e10 && t3 >= 1e10)
   {
    if (nearly_equal(x, l->px, 1e-8) && nearly_equal(z, l->pz, 1e-8))
      {
/*       printf("point in line6\n");*/
       return 1;
      }
/*    printf("point not in line6\n");*/
    return 0;
   }
 else if (t1 < 1e10 && t2 >= 1e10 && t3 >= 1e10)
   {
    if (nearly_equal(z, l->pz, 1e-8) && nearly_equal(y, l->py, 1e-8))
      {
/*       printf("point in line7\n");*/
       return 1;
      }
/*    printf("point not in line7\n");*/
    return 0;
   }
 else printf("undefined error!\n");
 return 0;
}

GLint check_direction(Line* a)
{
 if (nearly_equal(a->dx, 0., 1e-10) && nearly_equal(a->dy, 0., 1e-10) && nearly_equal(a->dz, 0., 1e-10)) return 0.;
 return 1;
}

GLint check_parallel(Line* a, Line* b)
{
 GLdouble d1,d2,d3;
 d1 = d2 = d3 = 1e12;
 if (a->dx != 0) d1 = b->dx / a->dx;
 if (a->dy != 0) d2 = b->dy / a->dy;
 if (a->dz != 0) d3 = b->dz / a->dz;
/* printf("d1,d2,d3=(%f,%f,%f)\n",d1,d2,d3);*/
 if (nearly_equal_spec(d1,d2,1e-8) && nearly_equal_spec(d2,d3,1e-8) && nearly_equal_spec(d1,d3,1e-8))
   {
/*    printf("parallel!, checking start point of A inside line B\n");*/
    return point_in_line(a->px, a->py, a->pz, b);
   }
 return 0;
}
		/* FIXME: it is OK? */
void compute_intersection(Line* a, Line* b, GLdouble* x, GLdouble* y, GLdouble* z)
{
 GLdouble t1,t2,t3;
 t1 = t2 = t3 = 1e12;
 if ((b->dx - a->dx) != 0) t1 = (a->px - b->px) / (b->dx - a->dx);
 if ((b->dy - a->dy) != 0) t2 = (a->py - b->py) / (b->dy - a->dy);
 if ((b->dz - a->dz) != 0) t3 = (a->pz - b->pz) / (b->dz - a->dz);
 if (t1 >= 1e10 && t2 < 1e10) t1 = t2;
 if (t1 >= 1e10 && t3 < 1e10) t1 = t3;
 if (t2 >= 1e10 && t1 < 1e10) t2 = t1;
 if (t2 >= 1e10 && t3 < 1e10) t2 = t3;
 if (t3 >= 1e10 && t1 < 1e10) t3 = t1;
 if (t3 >= 1e10 && t2 < 1e10) t3 = t2;
/* printf("not parallel and tvec is (%f,%f,%f)\n", t1, t2, t3);*/
 if (t1 < 1e11) *x = b->dx * t1 + b->px; else *x = b->px;
 if (t2 < 1e11) *y = b->dy * t2 + b->py; else *y = b->py;
 if (t3 < 1e11) *z = b->dz * t3 + b->pz; else *z = b->pz;
}

GLint check_edge(Line* a, GLdouble x, GLdouble y, GLdouble z)
{
 GLdouble minx,miny,minz;
 GLdouble maxx,maxy,maxz;
 minx = (a->va->x < a->vb->x)?a->va->x:a->vb->x;
 miny = (a->va->y < a->vb->y)?a->va->y:a->vb->y;
 minz = (a->va->z < a->vb->z)?a->va->z:a->vb->z;
 maxx = (a->va->x > a->vb->x)?a->va->x:a->vb->x;
 maxy = (a->va->y > a->vb->y)?a->va->y:a->vb->y;
 maxz = (a->va->z > a->vb->z)?a->va->z:a->vb->z;
/* printf(" (%f,%f,%f)\n", x,y,z);*/
 if (x < minx || x > maxx) return 0;		/* FIXME: should be used almost_higher */
 if (y < miny || y > maxy) return 0;
 if (z < minz || z > maxz) return 0;
/* printf("intersects line at: %f,%f,%f\n", x,y,z);*/
 return 1;
}

GLint almost_lower(GLdouble a, GLdouble val, GLdouble eps)
{
 if (nearly_equal(a, val, eps)) return 1;
 return a <= val;
}

GLint almost_higher(GLdouble a, GLdouble val, GLdouble eps)
{
 if (nearly_equal(a, val, eps)) return 1;
 return a >= val;
}

GLint check_edge2(Line* a, GLdouble x, GLdouble y)
{
 GLdouble minx,maxx;
 minx = (a->va->x < a->vb->x)?a->va->x:a->vb->x;
 maxx = (a->va->x > a->vb->x)?a->va->x:a->vb->x;
/* printf("check edge (%f,%f) (x(%f,%f)\n", x,y,minx,maxx); */
 if (nearly_equal(minx, maxx, 1e-4) && (nearly_equal(x, maxx, 1e-4) || nearly_equal(x, minx, 1e-4))) return 1;
 if (almost_lower(x, minx, 1e-4) || almost_higher(x, maxx, 1e-4)) return 0;		/* FIXME: discards intersections on vertexes */
 						/* FLOATING POINT COMPARISONS */
/* printf("intersects line at: %f,%f,%f\n", x,y,z);*/
 return 1;
}

GLint line_intersect(Line* a, Line* b)		/* FIXME: is it OK? */
{
 GLdouble x,y,z;
 
/* printf("A: %f %f %f %f %f %f\n", a->px, a->py, a->pz, a->dx, a->dy, a->dz);*/
/* printf("B: %f %f %f %f %f %f\n", b->px, b->py, b->pz, b->dx, b->dy, b->dz;*/
 
 if (!check_direction(a)) { printf("0 is not a correct direction A!\n"); return 0; }
 if (!check_direction(b)) { printf("0 is not a correct direction B!\n"); return 0; }
 
 if (check_parallel(a,b)) return 1;

 compute_intersection(a, b, &x, &y, &z);

/* printf("INTERSECTION=(%f,%f,%f)\n", x,y,z);*/

 if (!point_in_line(x,y,z, a) || !point_in_line(x,y,z, b)) 
  { 
/*   printf("this is not intersection in 3D!\n"); */
   return 0; 
  }
/* printf("We have edge to check!!!\n");*/
 if (check_edge(a, x,y,z) && check_edge(b,x,y,z)) 
  {
/*   printf("EDGES intersect!\n");*/
   return 1;
  }
 else return 0;
}

GLdouble compute_angle(Vertex* a, Vertex* b, Vertex* c)
{
 GLdouble x1,x2,y1,y2,z1,z2,len;
 x1 = b->x - a->x;
 x2 = c->x - a->x;
 y1 = b->y - a->y;
 y2 = c->y - a->y;
 z1 = b->z - a->z;
 z2 = c->z - a->z;
 len = sqrt(x1*x1+y1*y1+z1*z1);
 x1 /= len;
 y1 /= len;
 z1 /= len;
 len = sqrt(x2*x2+y2*y2+z2*z2);
 x2 /= len;
 y2 /= len;
 z2 /= len;
 return acos(x1*x2+y1*y2+z1*z2);
}

GLdouble compute_angle_2d(Vertex* a, Vertex* b, Vertex* c)
{
 GLdouble x1,x2,y1,y2,len;
 x1 = b->x - a->x;
 x2 = c->x - a->x;
 y1 = b->y - a->y;
 y2 = c->y - a->y;
 len = sqrt(x1*x1+y1*y1);
 if (nearly_equal(len, 0., 1e-8)) return 4.;
 x1 /= len;
 y1 /= len;
 len = sqrt(x2*x2+y2*y2);
 if (nearly_equal(len, 0., 1e-8)) return 4.;
 x2 /= len;
 y2 /= len;
 return (x1*x2+y1*y2);
}

GLdouble vertex_distance(Vertex* a, Vertex* b)
{
 return sqrt((b->x-a->x)*(b->x-a->x)+(b->y-a->y)*(b->y-a->y)+(b->z-a->z)*(b->z-a->z));
}

GLint vertex_in_polygon(Vertex* w, Poly* p)
{
 GLdouble kat;
 GLdouble dist;
 GLdouble nx,ny,nz;
 GLint n,i,j;
 Vertex W;
 n = p->n;
 kat = 0.;
 dist = 0.;
 for (i=0;i<n;i++) dist += vertex_distance(w, &p->v[i]);
 dist /= n;
 calculate_normal_poly(p, &nx, &ny, &nz);
 W.x = w->x + dist*nx;
 W.y = w->y + dist*ny;
 W.z = w->z + dist*nz;
/* printf("distance = %f\n", dist);*/
 for (i=0;i<n;i++)
   {
    j = (i+1==n)?0:i+1;
/*    printf("(i,j) = (%d,%d)\n", i,j);*/
    kat += compute_angle(&W, &p->v[i], &p->v[j]);
   }
/* printf("kat = %f\n", kat);*/
 if (kat > 3.5)		/* FIXME: is this test OK? */ 
   {
/*    printf("vertex is inside polygon!\n");*/
    return 1;
   }
 else return 0;
}

GLint vertex_in_polygon_2d(Vertex* w, Poly* p)
{
 GLdouble kat;
 GLint n,i,j;
 n = p->n;
 kat = 0.;
 for (i=0;i<n;i++)
   {
    j = (i+1==n)?0:i+1;
/*    printf("(i,j) = (%d,%d)\n", i,j);*/
    kat += compute_angle_2d(w, &p->v[i], &p->v[j]);
   }
 /* printf("kat = %f\n", kat); */
 if (kat < -1.)		/* FIXME: is this test OK? */ 
   {
/*    printf("vertex is inside polygon!\n");*/
    return 1;
   }
 else return 0;
}

GLint line_intersect_in_xyplane_old(Line* a, Line* b)
{
 GLdouble t;
 GLdouble x,y,z;
 GLdouble PX = a->px - b->px;
 GLdouble PY = a->py - b->py;
 GLdouble tmp = a->dy * b->dx - a->dx * b->dy;    
/* printf("A: %f %f %f %f %f %f\n", a->px, a->py, a->pz, a->dx, a->dy, a->dz);*/
/* printf("B: %f %f %f %f %f %f\n", b->px, b->py, b->pz, b->dx, b->dy, b->dz);*/
/* printf("tmp = %f\n", tmp);*/
 if (tmp != 0)
   {
     t = (PX * a->dy - PY * a->dx) / tmp;
/*     printf("t = %f\n", t);*/
     x = b->dx * t + b->px;
     y = b->dy * t + b->py;
     z = b->dz * t + b->pz;
   /*  printf("We have intersection at: (%f,%f,%f) checking edge constraints...\n", x,y,z); */
     if (check_edge(a, x,y,z) && check_edge(b,x,y,z)) 
       {
/*        printf("EDGES intersect in XYplane!\n");*/
        return 1;
       }
     return 0;
   }
 else return 0; 
}

GLint check_parallel_2d(Line* a, Line* b)
{
 GLdouble d1,d2;
 if (nearly_equal(a->dx, 0., 1e-8) && nearly_equal(b->dx, 0., 1e-8)) return 1;
 if (nearly_equal(a->dy, 0., 1e-8) && nearly_equal(b->dy, 0., 1e-8)) return 1;
 d1 = d2 = 1e12;
 if (a->dx != 0) d1 = b->dx / a->dx;
 if (a->dy != 0) d2 = b->dy / a->dy;
/* printf("d1,d2=(%f,%f)\n",d1,d2); */
 if (nearly_equal_spec(d1,d2,1e-8))
   {
/*    printf("parallel!, checking start point of A inside line B\n");*/
    return point_in_line(a->px, a->py, a->pz, b);
   }
 return 0;
}

GLint line_intersect_in_xyplane(Line* a, Line* b)
{
 GLdouble x,y;
 GLint intr;
 if (nearly_equal(a->dx, 0., 1e-8) && nearly_equal(a->dy, 0., 1e-8)) return 0; /* line A casted to point in XYplane */
 if (nearly_equal(b->dx, 0., 1e-8) && nearly_equal(b->dy, 0., 1e-8)) return 0; /* line B casted to point in XYplane */
 intr = get_intersection(a, b, &x, &y);
/* printf("INTERSECT=%d, X=%f, Y=%f\n", intr,x,y); */
 if (intr == 0) return 0;
 if (intr == 2) return 1;
 if (check_edge2(a, x,y) && check_edge2(b,x,y)) 
   {
/*    printf("EDGES intersect in XYplane!\n");*/
    return 1;
   }
 return 0;
}

GLint test1(GLint pI, GLint pJ)
{
 GLdouble ymaxi, ymaxj,ymini,yminj;
/* printf("test1(%d,%d)\n", pI, pJ); */
 ymini = poly_ymin(pI);
 yminj = poly_ymin(pJ);
 ymaxi = poly_ymax(pI);
 ymaxj = poly_ymax(pJ);
/* printf("Y: (%f,%f) - (%f,%f)\n", ymini,ymaxi,yminj,ymaxj); */
 if (almost_higher(ymini, ymaxj, 1e-4) || almost_higher(yminj, ymaxi, 1e-4)) { nt1++; nt1p++; return 1; }
 else { nt1++; return 0; }
}

GLint test2(GLint pI, GLint pJ)
{
 GLdouble xmaxi, xmaxj,xmini,xminj;
/* printf("test2(%d,%d)\n", pI, pJ); */
 xmini = poly_xmin(pI);
 xminj = poly_xmin(pJ);
 xmaxi = poly_xmax(pI);
 xmaxj = poly_xmax(pJ);
/* printf("X: (%f,%f) - (%f,%f)\n", xmini,xmaxi,xminj,xmaxj);*/
 if (almost_higher(xmini, xmaxj, 1e-4) || almost_higher(xminj, xmaxi, 1e-4)) { nt2++; nt2p++; return 1; }
 else { nt2++; return 0; } 
}

GLint test3(GLint pI, GLint pJ)
{
 GLdouble nx,ny,nz;
 GLdouble wx,wy,wz;
 GLdouble res;
 GLint i;
 Poly* p;
 Vertex* vj;
/* printf("test3(%d,%d)\n", pI, pJ);*/
 p = &pList[pI];
 vj = &(pList[pJ].v[0]);
 calculate_normal(pJ, &nx, &ny, &nz);
 for (i=0;i<p->n;i++)
   {
    wx = p->v[i].x - vj->x;
    wy = p->v[i].y - vj->y;
    wz = p->v[i].z - vj->z;
    res = nx*wx + ny*wy + nz*wz;
/*    printf("result3: %1.10f\n", res);*/
    if (res > 1e-6) { nt3++; return 0; }
   }
 nt3++;
 nt3p++;
 return 1;
}

GLint test4(GLint pI, GLint pJ)
{
 GLdouble nx,ny,nz;
 GLdouble wx,wy,wz;
 GLdouble res;
 GLint j;
 Poly* p;
 Vertex* vi;
/* printf("test4(%d,%d)\n", pI, pJ);*/
 p = &pList[pJ];
 vi = &(pList[pI].v[0]);
 calculate_normal(pI, &nx, &ny, &nz);
 for (j=0;j<p->n;j++)
   {
    wx = p->v[j].x - vi->x;
    wy = p->v[j].y - vi->y;
    wz = p->v[j].z - vi->z;
    res = nx*wx + ny*wy + nz*wz;
/*    printf("result4: %1.10f\n", res);*/
    if (res < -1e-6) { nt4++; return 0; }
   }
 nt4++;
 nt4p++;
 return 1;
}


GLint test5(GLint pI, GLint pJ)
{
 Poly *a, *b;
 Line *la, *lb;
 GLint na, nb, i, j;
 Vertex *vi, *vj;
/* printf("test5(%d,%d)\n", pI, pJ);*/
 a = &pList[pI];
 b = &pList[pJ];
 na = a->n;
 nb = b->n;
/* printf("STARTING TEST5... (%d,%d)\n", na, nb);*/
 for (i=0;i<na;i++)
   {
    la = make_line(&a->v[i], &a->v[(i+1==na)?0:i+1]);
/*    printf(" (%d/%d) LA: %f %f %f %f %f %f\n", i, na, la->px, la->py, la->pz, la->dx, la->dy, la->dz);*/
    for (j=0;j<nb;j++)
      {
       lb = make_line(&b->v[j], &b->v[(j+1==nb)?0:j+1]);
/*       printf("(%d,%d) LB: %f %f %f %f %f %f\n", j, nb, lb->px, lb->py, lb->pz, lb->dx, lb->dy, lb->dz);*/
/*       printf("checking intersection edge indexes (%d,%d)\n", i, j);*/
       if (line_intersect_in_xyplane(la, lb)) 
        { 
/*	 printf("now we have intersection!\n");*/
	 nt5++; 
	 mm_free(HERE,(void*)&la); 
	 mm_free(HERE,(void*)&lb); 
	 return 0;
	}
       mm_free(HERE,(void*)&lb);
      }
     mm_free(HERE,(void*)&la);
    }
/* printf("no edges were intersected!\n");*/
 vi = &pList[pI].v[0];
 vj = &pList[pJ].v[0];
 if (vertex_in_polygon_2d(vj, a)) { nt5++;  return 0; }
 if (vertex_in_polygon_2d(vi, b)) { nt5++;  return 0; }
/* printf("test5 passed!\n");*/
 nt5p++;
 nt5++;
 return 1;
}

GLint flag_list(GLint i)
{
 return pList[i].flag;
}

void set_flag(GLint i, GLint v)
{
 pList[i].flag = v;
}

Surface* find_surface_from_points(Vertex* a, Vertex* b, Vertex* c)
{
 Surface *ret;
 GLdouble x1,x2,y1,y2,z1,z2,len;
 GLdouble nx,ny,nz;
 ret = (Surface*)mm_malloc(HERE,sizeof(Surface));
 x1 = b->x - a->x;
 x2 = c->x - a->x;
 y1 = b->y - a->y;
 y2 = c->y - a->y;
 z1 = b->z - a->z;
 z2 = c->z - a->z;
 nx = y1*z2 - z1*y2;
 ny = z1*x2 - x1*z2;
 nz = x1*y2 - y1*x2;
 len = sqrt(nx*nx + ny*ny + nz*nz);
 nx /= len;
 ny /= len;
 nz /= len;
 ret->A = nx;
 ret->B = ny;
 ret->C = nz;
 ret->D = -ret->A*a->x - ret->B*a->y - ret->C*a->z;
 return ret;
}

Surface* make_polygon_surface(Poly* s)
{
 Vertex *a,*b,*c;
 a = &s->v[1];
 b = &s->v[0];
 c = &s->v[2];
 return find_surface_from_points(a, b, c);
}

Vertex* point_intersect_line_surface(Line* l, Surface* s)
{
 Vertex *ret;
 GLdouble t, tmp;
 ret = NULL;
 tmp = ( s->A*l->dx +  s->B*l->dy + s->C*l->dz);
 if (!nearly_equal(tmp, 0., 1e-8))
   {	
    ret = (Vertex*)mm_malloc(HERE,sizeof(Vertex));
    t = - ( s->A*l->px + s->B*l->py + s->C*l->pz + s->D) / tmp;
    ret->x = l->dx * t + l->px;
    ret->y = l->dy * t + l->py;
    ret->z = l->dz * t + l->pz;
   }
 return ret;
}

void interpolate_vertex(Vertex* t, Vertex* a, Vertex* b)
{
 /*t->x =  (a->x  + b->x)  / 2.;
 t->y =  (a->y  + b->y)  / 2.;
 t->z =  (a->z  + b->z)  / 2.;*/
 t->nx = (a->nx + b->nx) / 2.;
 t->ny = (a->ny + b->ny) / 2.;
 t->nz = (a->nz + b->nz) / 2.;
 t->tx = (a->tx + b->tx) / 2.;
 t->ty = (a->ty + b->ty) / 2.;
}

Poly* split_polygon(GLint toSplit, GLint splitting)
{
 Poly *t, *s;
 Line *la;
 GLint na, nb, i, j;
 GLint intr[2];
 Vertex *vert;
 Vertex ver[2];
 Surface *su;
 Poly* i1;
 Poly* i2;
 t = &pList[toSplit];
 s = &pList[splitting];
 na = t->n;
 nb = s->n;
 intr[0] = -1;
 intr[1] = -1;
 j = 0;
/* printf("SPLIT!!!!!!!!!!!!!!!!!!!!!!!!\n");*/
/* printf("Polygon to split:\n");*/
/* print_polygon(t);*/
/* printf("Splitting polygon:\n");*/
/* print_polygon(s);*/
 su = make_polygon_surface(s);
/* printf("SURFACE: %f,%f,%f,%f\n", su->A, su->B, su->C, su->D);*/
 for (i=0;i<na;i++)
   {
    la = make_line(&t->v[i], &t->v[(i+1==na)?0:i+1]);
/*    printf("to split edge: %f,%f,%f,%f,%f,%f\n", la->px, la->py, la->pz, la->dx, la->dy, la->dz);*/
    vert = point_intersect_line_surface(la, su);
    if (!vert)
      {
       mm_free(HERE,(void*)&la);
       continue;
      }
/*    printf("intersection with surface: %f,%f,%f\n", vert->x, vert->y, vert->z);*/
    if (check_edge(la, vert->x, vert->y, vert->z)) 
      {
       if (j == 2) { mm_free(HERE,(void*)&vert); break; }
/*       printf("Belongs to the edge!\n");*/
       memcpy(&ver[j], vert, sizeof(Vertex));
       intr[j++] = i;
      }
    mm_free(HERE,(void*)&la);
    mm_free(HERE,(void*)&vert);
/*    mm_free(HERE,(void*)la);*/
   }
 mm_free(HERE,(void*)&su);
 if (intr[0] < 0 || intr[1] < 0) return NULL;
 if (intr[0] > intr[1])
  {
   j = intr[0];
   intr[0] = intr[1];
   intr[1] = j;
  }
 j = 0;
 interpolate_vertex(&ver[0], &t->v[intr[0]], &t->v[(intr[0]+1==na)?0:(intr[0]+1)]);
 interpolate_vertex(&ver[1], &t->v[intr[1]], &t->v[(intr[1]+1==na)?0:(intr[1]+1)]);
 for (i=0;i<=na;i++)
   {
    if (i == intr[0]) j++;
    if (i > intr[0] && i <= intr[1]) j++;
    if (i == intr[1]+1) j++;
   }
 i1 = (Poly*)mm_malloc(HERE,sizeof(Poly));
 i2 = (Poly*)mm_malloc(HERE,sizeof(Poly));
 i1->n = j;
 i1->flag = 0;
 i1->v = (Vertex*)mm_malloc(HERE,j*sizeof(Vertex));
 j = 0;
 for (i=0;i<=na;i++)
   {
    if (i == intr[0]) 
      {
       memcpy(&i1->v[j], &ver[0], sizeof(Vertex));
       j++;
      }
    if (i > intr[0] && i <= intr[1])
      {
       memcpy(&i1->v[j], &t->v[i], sizeof(Vertex));
       j++;
      }
    if (i == intr[1]+1)
      {
       memcpy(&i1->v[j], &ver[1], sizeof(Vertex));
       j++;
      }
   }
/* print_polygon(i1);*/
 j = 0;
 for (i=0;i<na;i++)
   {
    if (i <= intr[0]) j++;
    if (i == intr[0]+1) j++;
    if (i == intr[1]) j++;
    if (i > intr[1]) j++;
   }
 i2->n = j;
 i2->flag = 0;
 i2->v = (Vertex*)mm_malloc(HERE,j*sizeof(Vertex));
 j = 0;
 for (i=0;i<na;i++)
   {
    if (i <= intr[0]) 
      {
       memcpy(&i2->v[j], &t->v[i], sizeof(Vertex));
       j++;
      }
    if (i == intr[0]+1)
      {
       memcpy(&i2->v[j], &ver[0], sizeof(Vertex));
       j++;
      }
    if (i == intr[1])
      {
       memcpy(&i2->v[j], &ver[1], sizeof(Vertex));
       j++;
      }
    if (i > intr[1]) 
      {
       memcpy(&i2->v[j], &t->v[i], sizeof(Vertex));
       j++;
      }
   }
/* print_polygon(i2);*/
/* printf("intr {%d, %d}, %d\n", intr[0], intr[1], j);*/
 if (poly_z(i1) > poly_z(i2))
  {
   mm_free(HERE,(void*) &t->v);
   copy_polygon(t, i1);
   mm_free(HERE,(void*)&i1->v);
   mm_free(HERE,(void*)&i1);
   return i2;
  }
 else
  {
   mm_free(HERE,(void*) &t->v);
   copy_polygon(t, i2);
   mm_free(HERE,(void*)&i2->v);
   mm_free(HERE,(void*)&i2);
   return i1;
  }
}

void blank_polys(Poly* list, GLint n)
{
 GLint i;
 if (!list) return;
 for (i=0;i<n;i++) list[i].v = NULL;
}

void insert_polygon(Poly* p, int idx)
{
 GLint i;
 for (i=nPoly-1;i>=idx;i--) 
  {
   if (pList[i+1].v) mm_free(HERE,(void*) &pList[i+1].v);	/* FIXME: pointer specific error may occur here */
   copy_polygon(&pList[i+1], &pList[i]);
  }
 if (pList[idx].v) mm_free(HERE,(void*) &pList[idx].v);
 copy_polygon(&pList[idx], p);
 nPoly++;
}

void rotatex(GLfloat** m, double ang)
{
 ang /= 180./PI;
 m[1][1] = cos(ang);
 m[2][1] = sin(ang);
 m[1][2] = -sin(ang);
 m[2][2] = cos(ang);
}


void rotatey(GLfloat** m, double ang)
{
 ang /= 180./PI;
 m[0][0] = cos(ang);
 m[2][0] = -sin(ang);
 m[0][2] = sin(ang);
 m[2][2] = cos(ang);
}


void rotatez(GLfloat** m, double ang)
{
 ang /= 180./PI;
 m[0][0] = cos(ang);
 m[1][0] = sin(ang);
 m[0][1] = -sin(ang);
 m[1][1] = cos(ang);
}

void matrix_mul_vector(GLfloat* dst, GLfloat** m, GLfloat* v, int len)
{
 int i,j;
 for (i=0;i<len;i++)
    {
     dst[i] = 0.0;
     for (j=0;j<len;j++) dst[i] += v[j] * m[i][j];
    }
}

GLfloat** matrix_mul(GLfloat** m, GLfloat** n, int ma, int mb, int na, int nb)
{
 GLfloat** dst;
 register int k,j,i;
 if (ma <= 0 || mb  <= 0 || na <= 0 || nb <=0 || mb != na) return NULL;
 if (!m || !n) return NULL;
 dst = (GLfloat**)mm_malloc(HERE,ma*sizeof(GLfloat*));
 if (!dst) return NULL;
 for (i=0;i<ma;i++) dst[i] = (GLfloat*)mm_malloc(HERE,nb*sizeof(GLfloat));
 
 for (i=0;i<ma;i++)
 for (j=0;j<nb;j++)
    {
     dst[i][j] = 0.0;
     for (k=0;k<mb;k++) dst[i][j] += m[i][k] * n[k][j];
    }
 return dst;
}

void I_matrix(GLfloat** dst, int siz)
{
 int i,j;
 if (!dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++) 
   {
    if (i == j) dst[i][j] = 1.;
    else dst[i][j] = 0.;
   }
}

void rotations()
{
 GLfloat **m1,**m2,**m3, **mt, **m;
 GLfloat *x, *rx;
 GLint i,j;
 m1 = (GLfloat**)mm_malloc(HERE,3*sizeof(GLfloat*));
 for (i=0;i<3;i++) m1[i] = (GLfloat*)mm_malloc(HERE,3*sizeof(GLfloat));
 m2 = (GLfloat**)mm_malloc(HERE,3*sizeof(GLfloat*));
 for (i=0;i<3;i++) m2[i] = (GLfloat*)mm_malloc(HERE,3*sizeof(GLfloat));
 m3 = (GLfloat**)mm_malloc(HERE,3*sizeof(GLfloat*));
 for (i=0;i<3;i++) m3[i] = (GLfloat*)mm_malloc(HERE,3*sizeof(GLfloat));
 I_matrix(m1, 3);
 I_matrix(m2, 3);
 I_matrix(m3, 3);
 rotatex(m1, angX);
 rotatey(m2, angY);
 rotatez(m3, angZ);
 mt = matrix_mul(m1, m2, 3, 3, 3, 3);
 m  = matrix_mul(mt, m3, 3, 3, 3, 3);
 for (i=0;i<3;i++) { mm_free(HERE,(void*)&m1[i]); mm_free(HERE,(void*)&m2[i]); mm_free(HERE,(void*)&m3[i]); mm_free(HERE,(void*)&mt[i]); }
 mm_free(HERE,(void*)&m1);
 mm_free(HERE,(void*)&m2);
 mm_free(HERE,(void*)&m3);
 mm_free(HERE,(void*)&mt);
 x  = (GLfloat*)mm_malloc(HERE,3*sizeof(GLfloat));
 rx = (GLfloat*)mm_malloc(HERE,3*sizeof(GLfloat));
 for (i=0;i<nPoly;i++)
   {
    for (j=0;j<pList[i].n;j++)
      {
       x[0] = pList[i].v[j].x;
       x[1] = pList[i].v[j].y;
       x[2] = pList[i].v[j].z;
       matrix_mul_vector(rx, m, x, 3);
       pList[i].v[j].x = rx[0];
       pList[i].v[j].y = rx[1];
       pList[i].v[j].z = rx[2];
       x[0] = pList[i].v[j].nx;
       x[1] = pList[i].v[j].ny;
       x[2] = pList[i].v[j].nz;
       matrix_mul_vector(rx, m, x, 3);
       pList[i].v[j].nx = rx[0];
       pList[i].v[j].ny = rx[1];
       pList[i].v[j].nz = rx[2];
      }
   }
 mm_free(HERE,(void*)&x);
 mm_free(HERE,(void*)&rx);
 for (i=0;i<3;i++) mm_free(HERE,(void*)&m[i]);
 mm_free(HERE,(void*)&m);
}


void painter_sort()
{
 GLint i,j;
 GLdouble zmax,zmin;
/* GLdouble zmaxj,zminj;*/
 Poly* ip;
 zero_stats();
 copy_from_buffer();
 rotations();
 sort_by_zmin();
 for (i=0;i<nPoly;i++)
   {
    zmax = poly_zmax(i);
    zmin = poly_zmin(i);
/*    printf("i=%d <%f,%f>\n", i, zmin, zmax);*/
    for (j=i+1;j < nPoly && poly_zmin(j) <= zmax;j++)
    {
/*     zmaxj = poly_zmax(j);*/
/*     zminj = poly_zmin(j);*/
/*     printf("\ti=%d <%f,%f>\n", j, zminj, zmaxj);*/

     if (test1(i, j)) continue;
     if (test2(i, j)) continue;
     if (test3(i, j)) continue;
     if (test4(i, j)) continue;
     if (test5(i, j)) continue;

/*     printf("trying test3 and test4 in swapped order flags(%d,%d)!\n", flag_list(i), flag_list(j));*/

     if (!(flag_list(i) && flag_list(j))  && (test3(j,i) || test4(j,i)) )
       {
/*	printf("polygon swapping...\n");*/
        poly_swap(i, j);
	set_flag(i, 1);
	set_flag(j, 1);
	continue;
       }
/*     printf("DOING SPLIT!\n");*/
     ip = split_polygon(i, j);
     if (!ip) continue;
     insert_polygon(ip, j+1);
     set_flag(i, 0);
     set_flag(j, 0);
     set_flag(j+1, 0);
     set_flag(j+2, 0);
    }
   }
 /*printf("TEST1 passed: %d/%d\n", nt1p, nt1);
 printf("TEST2 passed: %d/%d\n", nt2p, nt2);
 printf("TEST3 passed: %d/%d\n", nt3p, nt3);
 printf("TEST4 passed: %d/%d\n", nt4p, nt4);
 printf("TEST5 passed: %d/%d\n", nt5p, nt5);*/
}

void help()
{
 printf("usage: {basename} 3|7|8 0|1 0|1 file_in [texfile]\n");
 printf("3,7,8 stands for triangle file type\nfirst 0-1 (texture), second 0,1 (light)\n");
 printf("texfile can be nonexisting (or not specified) then texture is dynamicaly generated\n");
 printf("keys:\n");
 printf("1: auto move toogle\n");
 printf("9-0: change alpha channel cycle\n");
 printf("7-8: change alpha channel fade\n");
}

void alfa(GLint diff, GLint cycle)
{
 Texture* t;
 GLint x,y,a,i;
 t = &tList[0];
 x = t->sizx;
 y = t->sizy;
 for (i=0;i<x*y;i++) 
   {
    a = t->data[4*i+3];
    a += diff;
    if (a <= -1 && cycle) a = 0xFF;
    if (a <= -1 && !cycle) a = 0;
    if (a >= 0x100 && cycle) a = 0x0;
    if (a >= 0x100 && !cycle) a = 0xFF;
    t->data[4*i+3] = a;
   }
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->sizx, t->sizy, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->data);
 glBindTexture(GL_TEXTURE_2D, t->id);
}

void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
        case 27: case 'q':  exit(0); break;
        case '1': auto_move = ! auto_move; break;
	case '9': alfa(-5, 1); break;
	case '0': alfa(5, 1); break;
	case '7': alfa(-1, 0); break;
	case '8': alfa(1, 0); break;
        case 'i': angX -= 10.; painter_sort(); break;
        case 'k': angX += 10.; painter_sort();  break;
        case 'j': angY += 10.; painter_sort();  break;
        case 'l': angY -= 10.; painter_sort();  break;
        case 'o': angZ += 10.; painter_sort();  break;
        case 'm': angZ -= 10.; painter_sort();  break;
        case '-': scalef /= 1.3; break;
	case '=': case '+': scalef *= 1.3; break;
	case 'u': si--; if (si == 0) si = nPoly-1; printf("si = %d\n", si); break;
	case 'p': si++; if (si == nPoly) si = 0; printf("si = %d\n", si); break;
	case' ': scalef = 1.; angY = angZ = angX = 0.; auto_move = 0; break;
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

void render_polys()
{
 GLint i,j;
 if (want_tex && want_light)
 {
  for (i=0;i<nPoly;i++)
/* for (i=nPoly-1;i>=0;i--)*/
   {
    glBegin(GL_POLYGON);
    for (j=0;j<pList[i].n;j++) 
     {
      glTexCoord2d(pList[i].v[j].tx, pList[i].v[j].ty);
      glNormal3d(pList[i].v[j].nx, pList[i].v[j].ny, pList[i].v[j].nz);
      glVertex3d(pList[i].v[j].x, pList[i].v[j].y, pList[i].v[j].z);
     }
    glEnd();
   }
 }
 else if (want_tex && !want_light)
 {
 for (i=0;i<nPoly;i++)
/* for (i=nPoly-1;i>=0;i--)*/
   {
    glBegin(GL_POLYGON);
    for (j=0;j<pList[i].n;j++) 
     {
      glTexCoord2d(pList[i].v[j].tx, pList[i].v[j].ty);
      glVertex3d(pList[i].v[j].x, pList[i].v[j].y, pList[i].v[j].z);
     }
    glEnd();
   }
 }
 else if (!want_tex && !want_light)
 {
 for (i=0;i<nPoly;i++)
/*  for (i=nPoly-1;i>=0;i--)*/
   {
    /*if (i == si) glColor4d(0., 1., 0., 0.8);
    else glColor4d(1., 0., 0., 0.4);*/
    glBegin(GL_POLYGON);
    for (j=0;j<pList[i].n;j++) 
     {
      if (i == si) glColor3d( ((j%3)==0)?0.:1., ((j%3)==1)?0.:1., ((j%3)==2)?0.:1.);
      else glColor3d( ((j%3)==0)?1.:0., ((j%3)==1)?1.:0., ((j%3)==2)?1.:0.);
      glVertex3d(pList[i].v[j].x, pList[i].v[j].y, pList[i].v[j].z);
     }
    glEnd();
   }
 }
 else if (!want_tex && want_light)
 {
  for (i=0;i<nPoly;i++)
/* for (i=nPoly-1;i>=0;i--)*/
   {
    glBegin(GL_POLYGON);
    for (j=0;j<pList[i].n;j++) 
     {
      glNormal3d(pList[i].v[j].nx, pList[i].v[j].ny, pList[i].v[j].nz);
      glVertex3d(pList[i].v[j].x, pList[i].v[j].y, pList[i].v[j].z);
     }
    glEnd();
   }
 }
}

void test()
{
 glBegin(GL_POLYGON);
   glTexCoord2d(0., 0.);
   glVertex3d(-10, 10, -1);
   
   glTexCoord2d(1., 0.);
   glVertex3d(10, 10, -2);
   
   glTexCoord2d(1., 1.);
   glVertex3d(10, -10, -1);
   
   glTexCoord2d(0., 1.);
   glVertex3d(-10, -10, 0);
 glEnd();
}

void enable_light()
{
   GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 127.0 };
   GLfloat mat_diffuse2[] = { 0.6, 0.6, 0.6, 1.0 };
   GLfloat mat_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess2[] = { 127.0 };
   GLfloat lightPos0[] = { 0., 0., -10., 0.};
   GLfloat lightPos1[] = { 0., 0., 10., 0.};
   GLfloat lightColor[] = { 1.0f, 1.0f, 0.7f, 0.7 };
   GLfloat lightColor1[] = { 0.7f, 1.0f, 1.0f, 0.7 };
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse2);
   glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular2);
   glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess2);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
   glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor1);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
   glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
}


void render_scene(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
/* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
 if (want_tex)
   {
    glEnable(GL_BLEND);
    /* glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);*/
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }
 if (want_light) enable_light();
/* glEnable(GL_DEPTH_TEST);*/
/* glEnable(GL_CULL_FACE);*/
 /*glPolygonMode(GL_FRONT, GL_LINE);
 glPolygonMode(GL_BACK, GL_LINE);*/
 glLoadIdentity();
 gluLookAt(0., 0., 100., 0. , 0., 0., 0., 1., 0.);
 glScaled(scalef, scalef, scalef);
 /*glRotatef(angX, 1., 0. , 0.);
 glRotatef(angY, 0., 1. , 0.);
 glRotatef(angZ, 0., 0. , 1.);*/
/* test();*/
 render_polys();
 if (auto_move)
  {
   painter_sort();
   angX += 2.3;
   angY += 2.5;
   angZ += 2.7;
  }
 time_counter();
 glFlush();
 glutSwapBuffers();
 glClear(GL_DEPTH_BUFFER_BIT);
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

GLint load_tex_data(Texture* t, char* fn)
{
 FILE* plik;
 GLint n,nDat,sizx,sizy;
 if (!t) fatal("texture is null");
 plik = fopen(fn , "r");
 if (!plik) { printf("texture file don't exists, using dynamic...\n"); return 0; }
 n = fread((void*)(&sizx), sizeof(int), 1, plik);
 if (n != 1) fatal("cannot read x from file");
 n = fread((void*)(&sizy), sizeof(int), 1, plik);
 if (n != 1) fatal("cannot read y from file");
 t->sizx = sizx;
 t->sizy = sizy;
 nDat = 4 * t->sizx * t->sizy * sizeof(GLuchar);
/* printf("nDat = %d, %d\n", nDat, sizeof(GLuchar));*/
 t->data = (GLuchar*)mm_malloc(HERE,nDat);
 n = fread((void*)t->data, nDat, 1, plik);
 if (n != 1) fatal("cannot read entire texture data from file");
 n = fread((void*)(&nDat), sizeof(int), 1, plik);
 if (nDat != 0XDEADBEEF) fatal("bad magic in texture");
 if (n != 1) fatal("cannot read x from file");
 fclose(plik);
 return 1;
}

void dyna_tex_data(Texture* t)
{
 t->sizx = t->sizy = 4;
 t->data = (GLuchar*)mm_malloc(HERE,0X40*sizeof(GLuchar));

 t->data[(0X0<<2)+0X0] = 0X00;
 t->data[(0X0<<2)+0X1] = 0X00;
 t->data[(0X0<<2)+0X2] = 0X00;
 t->data[(0X0<<2)+0X3] = 0X40;

 t->data[(0X1<<2)+0X0] = 0X00;
 t->data[(0X1<<2)+0X1] = 0X00;
 t->data[(0X1<<2)+0X2] = 0XF1;
 t->data[(0X1<<2)+0X3] = 0X2B;

 t->data[(0X2<<2)+0X0] = 0X00;
 t->data[(0X2<<2)+0X1] = 0XF1;
 t->data[(0X2<<2)+0X2] = 0X00;
 t->data[(0X2<<2)+0X3] = 0X9A;

 t->data[(0X3<<2)+0X0] = 0X00;
 t->data[(0X3<<2)+0X1] = 0XF1;
 t->data[(0X3<<2)+0X2] = 0XF1;
 t->data[(0X3<<2)+0X3] = 0X80;

 t->data[(0X4<<2)+0X0] = 0X00;
 t->data[(0X4<<2)+0X1] = 0X00;
 t->data[(0X4<<2)+0X2] = 0X71;
 t->data[(0X4<<2)+0X3] = 0X53;

 t->data[(0X5<<2)+0X0] = 0X40;
 t->data[(0X5<<2)+0X1] = 0X40;
 t->data[(0X5<<2)+0X2] = 0X40;
 t->data[(0X5<<2)+0X3] = 0XCB;

 t->data[(0X6<<2)+0X0] = 0X00;
 t->data[(0X6<<2)+0X1] = 0X71;
 t->data[(0X6<<2)+0X2] = 0X71;
 t->data[(0X6<<2)+0X3] = 0X34;

 t->data[(0X7<<2)+0X0] = 0X00;
 t->data[(0X7<<2)+0X1] = 0X71;
 t->data[(0X7<<2)+0X2] = 0X00;
 t->data[(0X7<<2)+0X3] = 0X66;

 t->data[(0X8<<2)+0X0] = 0X40;
 t->data[(0X8<<2)+0X1] = 0XE1;
 t->data[(0X8<<2)+0X2] = 0X00;
 t->data[(0X8<<2)+0X3] = 0X66;

 t->data[(0X9<<2)+0X0] = 0X40;
 t->data[(0X9<<2)+0X1] = 0X71;
 t->data[(0X9<<2)+0X2] = 0X71;
 t->data[(0X9<<2)+0X3] = 0X1A;

 t->data[(0XA<<2)+0X0] = 0X20;
 t->data[(0XA<<2)+0X1] = 0X40;
 t->data[(0XA<<2)+0X2] = 0X60;
 t->data[(0XA<<2)+0X3] = 0XE6;

 t->data[(0XB<<2)+0X0] = 0X00;
 t->data[(0XB<<2)+0X1] = 0X40;
 t->data[(0XB<<2)+0X2] = 0X31;
 t->data[(0XB<<2)+0X3] = 0X53;

 t->data[(0XC<<2)+0X0] = 0XF1;
 t->data[(0XC<<2)+0X1] = 0X00;
 t->data[(0XC<<2)+0X2] = 0X00;
 t->data[(0XC<<2)+0X3] = 0X81;

 t->data[(0XD<<2)+0X0] = 0XF1;
 t->data[(0XD<<2)+0X1] = 0X00;
 t->data[(0XD<<2)+0X2] = 0XF1;
 t->data[(0XD<<2)+0X3] = 0X66;

 t->data[(0XE<<2)+0X0] = 0XF1;
 t->data[(0XE<<2)+0X1] = 0XF1;
 t->data[(0XE<<2)+0X2] = 0X00;
 t->data[(0XE<<2)+0X3] = 0XD5;

 t->data[(0XF<<2)+0X0] = 0XF1;
 t->data[(0XF<<2)+0X1] = 0XF1;
 t->data[(0XF<<2)+0X2] = 0XF1;
 t->data[(0XF<<2)+0X3] = 0XC0;
 
}

void init_texture_file(Texture* t, char* fname)
{
 if (!t) fatal("texture struct is null");
 glEnable(GL_TEXTURE_2D);
 glGenTextures(1, &t->id);
 glBindTexture(GL_TEXTURE_2D, t->id);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 if (!fname || !load_tex_data(t, fname)) dyna_tex_data(t);
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->sizx, t->sizy, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->data);
 glBindTexture(GL_TEXTURE_2D, t->id);
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
/* dyna_tex_data(t);*/
 load_tex_data(t, fname);
 /*printf("%d %d %p\n", t->sizx, t->sizy, t->data);*/
 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->sizx, t->sizy, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->data);
 glBindTexture(GL_TEXTURE_2D, t->id);
}

void init_textures_array()
{
 int i;
 nTex = 1;
 tList = (Texture*)mm_malloc(HERE,nTex*sizeof(Texture));
 for (i=0;i<nTex;i++) init_texture(&tList[i], i);
}

void init_textures_file(char* file)
{
 nTex = 1;
 tList = (Texture*)mm_malloc(HERE,nTex*sizeof(Texture));
 init_texture_file(&tList[0], file);
}

void Init()
{
 help();
 angY = angZ = 0.;
 angX = 0.;
 scalef = 1.;
 nPoly = 0;
 nVert = 0;
 nTex = 0;
 glob_alloc = 0;
 vList = NULL;
 pList = NULL;
 tList = NULL;
 si = 0;
}


void glInit()
{
 glClearColor(0.5,0.5,0.5,0.0);
 glShadeModel(GL_SMOOTH);
/* glEnable(GL_LINES_SMOOTH);*/
 glEnable(GL_DEPTH_TEST);
}

void print_vertex(Vertex* v)
{
 printf("Vertex(%p) -- v(%f,%f,%f) n(%f,%f,%f) t(%f,%f)\n",(void*)v,v->x,v->y,v->z,v->nx,v->ny,v->nz,v->tx,v->ty);
}

void print_polygon(Poly* p)
{
 GLint i;
 if (!p) { printf("Polygon: (null)\n"); return; }
 printf("Polygon (%p) - %d vertexes\n", (void*)p, p->n);
 for (i=0;i<p->n;i++) print_vertex(&p->v[i]);
 printf("---\n");
}

void read_vertex9_from_file(Vertex* v, FILE* plik)
{
 GLint n,i;
 if (!plik) fatal("file not open");
 if (!v) fatal("vertex is null");
 n = fscanf(plik, "%d %lf %lf %lf %lf %lf %lf %lf %lf\n", &i,&v->x,&v->y,&v->z,&v->nx,&v->ny,&v->nz,&v->tx,&v->ty);
 if (n != 9) fatal("vertex cannot be read from file");
}

void read_vertex8_from_file(Vertex* v, FILE* plik)
{
 GLint n;
 if (!plik) fatal("file not open");
 if (!v) fatal("vertex is null");
 n = fscanf(plik, "%lf %lf %lf %lf %lf %lf %lf %lf\n", &v->x,&v->y,&v->z,&v->nx,&v->ny,&v->nz,&v->tx,&v->ty);
 if (n != 8) fatal("vertex cannot be read from file");
}

void read_vertex3_from_file(Vertex* v, FILE* plik)
{
 GLint n;
 if (!plik) fatal("file not open");
 if (!v) fatal("vertex is null");
 n = fscanf(plik, "%lf %lf %lf\n", &v->x,&v->y,&v->z);
 if (n != 3) fatal("vertex cannot be read from file");
 v->nx = v->ny = v->nz = v->tx = v->ty = 0.;
}

void read_triangle_from_file(Poly* t, FILE* plik)
{
 GLint n,i1,i2,i3;
 if (!plik) fatal("file not open");
 if (!t) fatal("polygon is null");
 n = fscanf(plik, "%d %d %d\n", &i1, &i2, &i3);
 if (n != 3) fatal("triangle cannot be read from file");
 if (i1 < 1 || i1 > nVert) fatal("i1 out of range");
 if (i2 < 1 || i2 > nVert) fatal("i2 out of range");
 if (i3 < 1 || i3 > nVert) fatal("i3 out of range");
 t->n = 3;
 t->v = (Vertex*)mm_malloc(HERE,3*sizeof(Vertex));
 memcpy(&t->v[0], &vList[i1-1], sizeof(Vertex));
 memcpy(&t->v[1], &vList[i2-1], sizeof(Vertex));
 memcpy(&t->v[2], &vList[i3-1], sizeof(Vertex));
/* print_polygon(t);*/
}

void read_triangle_shift_from_file(Poly* t, FILE* plik)
{
 GLint n,i1,i2,i3;
 if (!plik) fatal("file not open");
 if (!t) fatal("polygon is null");
 n = fscanf(plik, "%d %d %d\n", &i1, &i2, &i3);
 if (n != 3) fatal("triangle cannot be read from file");
 if (i1 < 0 || i1 >= nVert) fatal("i1 out of range");
 if (i2 < 0 || i2 >= nVert) fatal("i2 out of range");
 if (i3 < 0 || i3 >= nVert) fatal("i3 out of range");
 t->n = 3;
 t->v = (Vertex*)mm_malloc(HERE,3*sizeof(Vertex));
 memcpy(&t->v[0], &vList[i1], sizeof(Vertex));
 memcpy(&t->v[1], &vList[i2], sizeof(Vertex));
 memcpy(&t->v[2], &vList[i3], sizeof(Vertex));
/* print_polygon(t);*/
}

void LoadTriangles9(char* fn)
{
 FILE* plik;
 GLint nread, i;
 plik = fopen(fn, "r");
 if (!plik) fatal("error opening file");
 
 nread = fscanf(plik, "%d\n", &nVert);
 if (nread != 1) fatal("unknown vertex count");
 if (nVert <= 3) fatal("not enough vertexes");
 vList = (Vertex*)mm_malloc(HERE,nVert*sizeof(Vertex));
 for (i=0;i<nVert;i++) read_vertex9_from_file(&vList[i], plik);
 
 nread = fscanf(plik, "%d\n", &nPoly);
 if (nread != 1) fatal("unknown triangles count");
 if (nPoly <= 1) fatal("not enough triangles");
 pList = (Poly*)mm_malloc(HERE,10*(nPoly+4)*sizeof(Poly));
 maxPolys = 10*(nPoly+4);
 blank_polys(pList, maxPolys);
 for (i=0;i<nPoly;i++) read_triangle_from_file(&pList[i], plik);
 for (i=0;i<maxPolys;i++) pList[i].flag = 0;
 
 fclose(plik);
/* printf("nPoly = %d\n", nPoly);*/
}

void LoadTriangles8(char* fn)
{
 FILE* plik;
 GLint nread, i;
 plik = fopen(fn, "r");
 if (!plik) fatal("error opening file");
 
 nread = fscanf(plik, "%d\n", &nVert);
 if (nread != 1) fatal("unknown vertex count");
 if (nVert <= 3) fatal("not enough vertexes");
 vList = (Vertex*)mm_malloc(HERE,nVert*sizeof(Vertex));
 for (i=0;i<nVert;i++) read_vertex8_from_file(&vList[i], plik);
 
 nread = fscanf(plik, "%d\n", &nPoly);
 if (nread != 1) fatal("unknown triangles count");
 if (nPoly <= 1) fatal("not enough triangles");
 pList = (Poly*)mm_malloc(HERE,10*(nPoly+4)*sizeof(Poly));
 maxPolys = 10*(nPoly+4);
 blank_polys(pList, maxPolys);
 for (i=0;i<nPoly;i++) read_triangle_from_file(&pList[i], plik);
 for (i=0;i<maxPolys;i++) pList[i].flag = 0;
 
 fclose(plik);
/* printf("nPoly = %d\n", nPoly);*/
}

void LoadTriangles3(char* fn)
{
 FILE* plik;
 GLint nread, i;
 plik = fopen(fn, "r");
 if (!plik) fatal("error opening file");
 
 nread = fscanf(plik, "%d\n", &nVert);
 if (nread != 1) fatal("unknown vertex count");
 if (nVert <= 3) fatal("not enough vertexes");
 vList = (Vertex*)mm_malloc(HERE,nVert*sizeof(Vertex));
 for (i=0;i<nVert;i++) read_vertex3_from_file(&vList[i], plik);
 
 nread = fscanf(plik, "%d\n", &nPoly);
 if (nread != 1) fatal("unknown triangles count");
 if (nPoly <= 1) fatal("not enough triangles");
 pList = (Poly*)mm_malloc(HERE,10*(nPoly+4)*sizeof(Poly));
 maxPolys = 10*(nPoly+4);
 blank_polys(pList, maxPolys);
 for (i=0;i<nPoly;i++) read_triangle_shift_from_file(&pList[i], plik);
 for (i=0;i<maxPolys;i++) pList[i].flag = 0;
 
 fclose(plik);
/* printf("nPoly = %d\n", nPoly);*/
}

void test_intersect()
{
 Line a,b;
/* a.px = 0.;
 a.py = 10.;
 a.pz = 10.;
 b.px = 10.;
 b.py = 0.;
 b.pz = 20.;
 a.dx = 2.;
 a.dy = 0.;
 a.dz = 0.;
 b.dx = 0.;
 b.dy = 1.;
 b.dz = -1.;*/
 a.px = 0.;
 a.py = 0.;
 a.pz = 15.;
 b.px = 0.;
 b.py = 0.;
 b.pz = 0.;
 a.dx = 30.;
 a.dy = 30.;
 a.dz = 0.;
 b.dx = 30.;
 b.dy = 30.;
 b.dz = 30.;
 line_intersect(&a, &b);
 exit(1);
}

void test_inside()
{
 Vertex w;
 Poly p;
 w.x = 0.1;
 w.y = 0.1;
 w.z = 25.;
 p.n = 4;
 p.v = (Vertex*)mm_malloc(HERE,4*sizeof(Vertex));
 p.v[0].x = -10; p.v[0].y = -10; p.v[0].z = 0;
 p.v[1].x = -10; p.v[1].y =  10; p.v[1].z = 0;
 p.v[2].x =  10; p.v[2].y =  10; p.v[2].z = 0;
 p.v[3].x =  10; p.v[3].y = -10; p.v[3].z = 0;
 vertex_in_polygon(&w, &p);
 exit(1);
}

void copy_to_buffer()
{
 GLint i;
 pList_buf = (Poly*)mm_malloc(HERE,maxPolys*sizeof(Poly));
 for (i=0;i<nPoly;i++) copy_polygon(&pList_buf[i], &pList[i]);
 nPoly_buf = nPoly;
}

void test_test1()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = 0.;
 
 pList[0].v[1].x = 20.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 0.;
 
 pList[0].v[2].x = 0.;
 pList[0].v[2].y = 20.;
 pList[0].v[2].z = 0.;
 
 pList[1].v[0].x = 0.;
 pList[1].v[0].y = 30.;
 pList[1].v[0].z = 0.;
 
 pList[1].v[1].x = 20.;
 pList[1].v[1].y = 30.;
 pList[1].v[1].z = 0.;
 
 pList[1].v[2].x = 0.;
 pList[1].v[2].y = 50.;
 pList[1].v[2].z = 0.;
 test1(0,1);
 printf("after test1: %d/%d\n", nt1p,nt1);
}

void test_test2()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = 0.;
 
 pList[0].v[1].x = 20.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 0.;
 
 pList[0].v[2].x = 0.;
 pList[0].v[2].y = 20.;
 pList[0].v[2].z = 0.;
 
 pList[1].v[0].x = 30.;
 pList[1].v[0].y = 0.;
 pList[1].v[0].z = 0.;
 
 pList[1].v[1].x = 50.;
 pList[1].v[1].y = 0.;
 pList[1].v[1].z = 0.;
 
 pList[1].v[2].x = 30.;
 pList[1].v[2].y = 20.;
 pList[1].v[2].z = 0.;
 test2(0,1);
 printf("after test2: %d/%d\n", nt2p,nt2);
}

void test_test3()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = -40.;
 
 pList[0].v[1].x = 30.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = -20.;
 
 pList[0].v[2].x = 15.;
 pList[0].v[2].y = 20.;
 pList[0].v[2].z = -30.;
 
 pList[1].v[0].x = 10.;
 pList[1].v[0].y = 0.;
 pList[1].v[0].z = 0.;
 
 pList[1].v[1].x = 50.;
 pList[1].v[1].y = 0.;
 pList[1].v[1].z = -20.;
 
 pList[1].v[2].x = 30.;
 pList[1].v[2].y = 40.;
 pList[1].v[2].z = -10.;
 test3(0,1);
 printf("after test3: %d/%d\n", nt3p,nt3);
}

void test_test3_notused()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = -60.;
 
 pList[0].v[1].x = 30.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = -40.;
 
 pList[0].v[2].x = 15.;
 pList[0].v[2].y = 20.;
 pList[0].v[2].z = -50.;
 
 pList[1].v[0].x = 10.;
 pList[1].v[0].y = 0.;
 pList[1].v[0].z = 0.;
 
 pList[1].v[1].x = 50.;
 pList[1].v[1].y = 0.;
 pList[1].v[1].z = -20.;
 
 pList[1].v[2].x = 30.;
 pList[1].v[2].y = 40.;
 pList[1].v[2].z = -10.;
 test3(0,1);
 printf("after test3: %d/%d\n", nt3p,nt3);
}

void test_test3_fail()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = -25.;
 
 pList[0].v[1].x = 30.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = -5.;
 
 pList[0].v[2].x = 15.;
 pList[0].v[2].y = 20.;
 pList[0].v[2].z = -15.;
 
 pList[1].v[0].x = 10.;
 pList[1].v[0].y = 0.;
 pList[1].v[0].z = 0.;
 
 pList[1].v[1].x = 50.;
 pList[1].v[1].y = 0.;
 pList[1].v[1].z = -20.;
 
 pList[1].v[2].x = 30.;
 pList[1].v[2].y = 40.;
 pList[1].v[2].z = -10.;
 test3(0,1);
 printf("after test3: %d/%d\n", nt3p,nt3);
}

void test_test4()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = -20.;
 
 pList[0].v[1].x = 40.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 0.;
 
 pList[0].v[2].x = 20.;
 pList[0].v[2].y = 20.;
 pList[0].v[2].z = -10.;
 
 pList[1].v[0].x = -10.;
 pList[1].v[0].y = 0.;
 pList[1].v[0].z = -10.;
 
 pList[1].v[1].x = 10.;
 pList[1].v[1].y = 0.;
 pList[1].v[1].z = 10.;
 
 pList[1].v[2].x = 0.;
 pList[1].v[2].y = 40.;
 pList[1].v[2].z = 0.;
 test4(0,1);
 printf("after test4: %d/%d\n", nt4p,nt4);
}

void test_test4_fail()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = -20.;
 
 pList[0].v[1].x = 40.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 0.;
 
 pList[0].v[2].x = 20.;
 pList[0].v[2].y = 20.;
 pList[0].v[2].z = -10.;
 
 pList[1].v[0].x = -10.;
 pList[1].v[0].y = 0.;
 pList[1].v[0].z = -30.;
 
 pList[1].v[1].x = 20.;
 pList[1].v[1].y = 0.;
 pList[1].v[1].z = -10.;
 
 pList[1].v[2].x = -5.;
 pList[1].v[2].y = 40.;
 pList[1].v[2].z = -20.;
 test4(0,1);
 printf("after test4: %d/%d\n", nt4p,nt4);
}

void test_test5_lines_intersect()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = 0.;
 
 pList[0].v[1].x = 40.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 0.;
 
 pList[0].v[2].x = 20.;
 pList[0].v[2].y = 40.;
 pList[0].v[2].z = 0.;
 
 pList[1].v[0].x = 0.;
 pList[1].v[0].y = 40.;
 pList[1].v[0].z = 0.;
 
 pList[1].v[1].x = 20.;
 pList[1].v[1].y = 0.;
 pList[1].v[1].z = 0.;
 
 pList[1].v[2].x = 40.;
 pList[1].v[2].y = 40.;
 pList[1].v[2].z = 0.;
 test5(0,1);
 printf("after test5: %d/%d\n", nt5p,nt5);
 exit(1);
}

void test_test5_edges_intersect()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = 0.;
 
 pList[0].v[1].x = 40.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 0.;
 
 pList[0].v[2].x = 20.;
 pList[0].v[2].y = 40.;
 pList[0].v[2].z = 0.;
 
 pList[1].v[0].x = 0.;
 pList[1].v[0].y = 40.;
 pList[1].v[0].z = 0.;
 
 pList[1].v[1].x = 20.;
 pList[1].v[1].y = 0.;
 pList[1].v[1].z = 0.;
 
 pList[1].v[2].x = 40.;
 pList[1].v[2].y = 40.;
 pList[1].v[2].z = 0.;
 test5(0,1);
 printf("after test5: %d/%d\n", nt5p,nt5);
 exit(1);
}

void test_test5_small_and_big()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = 10.;
 
 pList[0].v[1].x = 80.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 10.;
 
 pList[0].v[2].x = 40.;
 pList[0].v[2].y = 80.;
 pList[0].v[2].z = -10.;
 
 pList[1].v[0].x = 60.;
 pList[1].v[0].y = 60.;
 pList[1].v[0].z = -20.;
 
 pList[1].v[1].x = 70.;
 pList[1].v[1].y = 40.;
 pList[1].v[1].z = -20.;
 
 pList[1].v[2].x = 70.;
 pList[1].v[2].y = 60.;
 pList[1].v[2].z = 20.;
 test5(0,1);
 printf("after test5: %d/%d\n", nt5p,nt5);
}

void test_test5_split_simple()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = 0.;
 
 pList[0].v[1].x = 60.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 0.;
 
 pList[0].v[2].x = 0.;
 pList[0].v[2].y = 60.;
 pList[0].v[2].z = 0.;
 
 pList[1].v[0].x = 10.;
 pList[1].v[0].y = 10.;
 pList[1].v[0].z = 10.;
 
 pList[1].v[1].x = 50.;
 pList[1].v[1].y = 10.;
 pList[1].v[1].z = -10.;
 
 pList[1].v[2].x = 10.;
 pList[1].v[2].y = 50.;
 pList[1].v[2].z = -10.;
 test5(0,1);
 printf("after test5: %d/%d\n", nt5p,nt5);
}

void test_test5_inside()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = -20.;
 pList[0].v[0].y = -20.;
 pList[0].v[0].z = 20.;
 
 pList[0].v[1].x = 20.;
 pList[0].v[1].y = -20.;
 pList[0].v[1].z = 20.;
 
 pList[0].v[2].x = 0.;
 pList[0].v[2].y = 30.;
 pList[0].v[2].z = -10.;
 
 pList[1].v[0].x = -40.;
 pList[1].v[0].y = -40.;
 pList[1].v[0].z = -30.;
 
 pList[1].v[1].x = 40.;
 pList[1].v[1].y = -40.;
 pList[1].v[1].z = -30.;
 
 pList[1].v[2].x = 0.;
 pList[1].v[2].y = 60.;
 pList[1].v[2].z = 20.;
 test5(0,1);
 printf("after test5: %d/%d\n", nt5p,nt5);
}


void test_line_intersect()
{
 Vertex a,b,c,d;
 GLdouble x,y;
 Line *l1, *l2;
 a.x = 0.;
 a.y = 0.;
 a.z = 2.;
 
 b.x = 1.;
 b.y = 0.;
 b.z = 5.;
 
 d.x = 0.;
 d.y = 0.;
 d.z = 4.;
 
 c.x = 0.;
 c.y = 1.;
 c.z = 3.;
 
 l1 = make_line(&a, &b);
 printf("%f,%f,%f\n",l1->A, l1->B, l1->C);
 l2 = make_line(&c, &d);
 printf("%f,%f,%f\n",l2->A, l2->B, l2->C);
 if (get_intersection(l1, l2, &x, &y)) printf("intersection: %f,%f\n", x, y);
 else printf("no intersection!\n");
 mm_free(HERE,(void*)&l1);
 mm_free(HERE,(void*)&l2);
}

void test_edge_intersect()
{
 Vertex a,b,c,d;
 GLint res;
 Line *l1, *l2;
 a.x = 2.;
 a.y = 3.;
 a.z = 0.;
 
 b.x = 4.;
 b.y = 6.;
 b.z = 0.;
 
 d.x = 6.;
 d.y = 4.;
 d.z = 0.;
 
 c.x = 3.;
 c.y = 2.;
 c.z = 0.;
 
 l1 = make_line(&a, &b);
 printf("%f,%f,%f\n",l1->A, l1->B, l1->C);
 l2 = make_line(&c, &d);
 printf("%f,%f,%f\n",l2->A, l2->B, l2->C);

 res = line_intersect_in_xyplane(l1, l2);
 printf("HAVE INTERSECT: %d\n", res);

 mm_free(HERE,(void*)&l1);
 mm_free(HERE,(void*)&l2);
}

void test_vertex_in_poly()
{
 Vertex w;
 Poly p;
 GLint res;
 
 w.x = 0.;
 w.y = 0.;
 w.z = 20.;
 
 p.n = 3;
 p.v = mm_malloc(HERE,3*sizeof(Vertex));
 
 p.v[0].x = 0.;
 p.v[0].y = 0.;
 p.v[0].z = 10.;
 
 p.v[1].x = 10.;
 p.v[1].y = 0.;
 p.v[1].z = 40.;
 
 p.v[2].x = 10.;
 p.v[2].y = 10.;
 p.v[2].z = -30.;

 res = vertex_in_polygon_2d(&w, &p);
 printf("Result is: %d\n", res);

 exit(1);
}

void test_swap_polygon()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = 0.;
 pList[0].v[0].y = 0.;
 pList[0].v[0].z = 0.;
 
 pList[0].v[1].x = 0.;
 pList[0].v[1].y = 0.;
 pList[0].v[1].z = 0.;
 
 pList[0].v[2].x = 0.;
 pList[0].v[2].y = 0.;
 pList[0].v[2].z = 0.;
 
 pList[1].v[0].x = 10.;
 pList[1].v[0].y = 10.;
 pList[1].v[0].z = 10.;
 
 pList[1].v[1].x = 10.;
 pList[1].v[1].y = 10.;
 pList[1].v[1].z = 10.;
 
 pList[1].v[2].x = 10.;
 pList[1].v[2].y = 10.;
 pList[1].v[2].z = 10.;

 printf("polygons:\n");

 print_polygon(&pList[0]);
 print_polygon(&pList[1]);

 poly_swap(0, 1);
 printf("swapped polygons:\n");
 
 print_polygon(&pList[0]);
 print_polygon(&pList[1]);

 exit(1);
}


void test_common_edges()
{
 maxPolys = 10*(nPoly+4);
 pList = (Poly*)mm_malloc(HERE,100*sizeof(Poly));
 blank_polys(pList, 150);
 nPoly = 2;
 pList[0].n = 3;
 pList[1].n = 3;
 pList[0].v = mm_malloc(HERE,3*sizeof(Vertex));
 pList[1].v = mm_malloc(HERE,3*sizeof(Vertex));
 
 pList[0].v[0].x = -30.;
 pList[0].v[0].y = -15.;
 pList[0].v[0].z = -15.;
 
 pList[0].v[1].x = -10.;
 pList[0].v[1].y = -5.;
 pList[0].v[1].z = -5.;
 
 pList[0].v[2].x = -20.;
 pList[0].v[2].y = -5.;
 pList[0].v[2].z = -15.;
 
 pList[1].v[0].x = -30;
 pList[1].v[0].y = -15.;
 pList[1].v[0].z = -15.;
 
 pList[1].v[1].x = -20.;
 pList[1].v[1].y = -15.;
 pList[1].v[1].z = -5;
 
 pList[1].v[2].x = -10.;
 pList[1].v[2].y = -5.;
 pList[1].v[2].z = -5.;
 test5(0,1);
 printf("after test5: %d/%d\n", nt5p,nt5);
}



int main(int lb, char** par)
{
 wx = 800;
 wy = 600;
/* test_intersect();*/
/* test_inside();*/
 Init();
 if (lb < 5) fatal("bad parameters count");
 if (par[1][0] == '9') LoadTriangles9(par[4]);
 if (par[1][0] == '8') LoadTriangles8(par[4]);
 if (par[1][0] == '3') LoadTriangles3(par[4]);
 if (par[2][0] == '1') want_tex = 1;
 if (par[2][0] == '0') want_tex = 0;
 if (par[3][0] == '1') want_light = 1;
 if (par[3][0] == '0') want_light = 0;
/* test_test1();*/
/* test_test2();*/
/* test_test3();*/
/* test_test3_fail();*/
/* test_test3_notused();*/
/* test_test4(); */
/* test_test4_fail();*/
/* test_line_intersect();*/
/* test_edge_intersect();*/
/* test_test5_edges_intersect();*/
/* test_vertex_in_poly();*/
/* test_test5_small_and_big();*/
/* test_swap_polygon();*/
/* test_test5_inside();*/
/* test_test5_split_simple();*/
/* test_common_edges();*/
 copy_to_buffer();
 painter_sort();
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
 if (want_tex) 
   {
    if (lb >=6) init_textures_file(par[5]);
    else init_textures_file(NULL);
   }
 glutMainLoop();
 return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
