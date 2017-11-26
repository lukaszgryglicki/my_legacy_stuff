/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PI 3.1415926F	/* math constant */
#define DIM 4
/*#define DEBUG*/
#define invert_world_matrix inverted_world_matrix
#define copy_point copy_vector
#define construct_cursor_default construct_cursor4d_default
#define list_get_bcurves_C0 list_get_C0
#define list_get_bcurves_C0_dim list_get_C0_dim
#define list_get_splines_C0 list_get_C0
#define list_get_splines_C0_dim list_get_C0_dim
#define list_get_bcurves_C1 list_get_C1
#define list_get_bcurves_C1_dim list_get_C1_dim
#define list_get_splines_C1 list_get_C1
#define list_get_splines_C1_dim list_get_C1_dim
#define list_get_bcurves_C2 list_get_C2
#define list_get_bcurves_C2_dim list_get_C2_dim
#define list_get_splines_C2 list_get_C2
#define list_get_splines_C2_dim list_get_C2_dim
#define list_get_nurbs_C0   list_get_C02
#define list_get_nurbs_C2   list_get_C22
#define construct_vector4 construct_vector
#define eps 1e-3
#define Point4D Vector4D
#ifdef DEBUG
#define TRACE printf("file: %s line: %d\n", __FILE__, __LINE__);
#else
#define TRACE
#endif
struct Vector4D		/* stores vector points */
{
 double p[DIM];
 double w;
};
struct Matrix4D		/* stores 4 vectors */
{
 struct Vector4D v[DIM];
};
struct Line4D
{
 struct Vector4D v[2];
};
struct Cursor4D
{
 struct Line4D l[3];
};
struct Polygon4D
{
 struct Polygon4D* next;
 struct Polygon4D* prev;
 struct Vector4D p;
};
struct BSpline
{
 int K;				/* wezly ?			 	3 */
 int M;				/* stopien krzywej bazowej 		3 */
 int N;				/* computed: N = K-M+1 			1 */
 int A;				/* A = N+2*M 				7 */
 double* T;			/* T(-M) ... T(M+N), T[A+1] 		T(-3) ... T(4) .. T(8) */
 double* W;			/* Wagi W[K+1] */
 struct Vector4D* P;		/* Punkty kontr  P[K+1].p[0-3] */
 double U0, U1;			/* Przedz parametryzacji */
 struct Vector4D* points;	/* wyliczone punkty */
 int   npoints;			/* ilosc punktow */
};
struct Nurb
{
 int K1, K2;
 int M1, M2;
 int N1, N2;
 int A,B,C;
 double* T;
 double* S;
 double** W;
 struct Vector4D** P;
 double U0, U1;	
 double V0, V1;
 struct Vector4D** points;
 struct Vector4D** normals;
 int   npointsx;
 int   npointsy;
};
struct BezierCurve
{
 int N;
 struct Vector4D* P;		/* Punkty kontr  P[N+1].p[0-3] */
 double U0, U1;			/* Przedz parametryzacji */
 struct Vector4D* points;	/* wyliczone punkty */
 int   npoints;			/* ilosc punktow */
};
struct Mesh4D
{
 struct Vector4D** bez;
 struct Vector4D** boor;
 struct Nurb*** nurbs;
 int nBoorX, nBoorY, nBezX, nBezY;
 int nNurbsX, nNurbsY;
 int nX, nY;
};
struct VList
{
 struct VList *next, *prev;
 void* data;
};
struct NurbPair
{
 struct Nurb *n1, *n2;
};
struct TrimmedMesh
{
 struct Mesh4D *m1, *m2;
 struct Vector4D P0, t;
 struct VList *tc1, *tc2, *tcuv;
 double d,du,dv,ds,dt,bscale;
 struct Vector4D** points1;
 struct Vector4D** points2;
 unsigned char** matrix1;
 unsigned char** matrix2;
 int npx1, npy1, npx2, npy2;
};
int mmin(int u, int v);
int mmax(int u, int v);
unsigned long RGB16(int r, int g, int b);	/* return ulong color on 16bit display */
unsigned long RGB24(int r, int g, int b);	/* return ulong color on 24bit display */
void print_vector(struct Vector4D* vec);	/* prints out given vector */
void print_matrix(struct Matrix4D* m);	/* prints out matrix */
void printf_vector(char* name, struct Vector4D* vec);	/* prints out given vector */
void printf_point(char* name, struct Polygon4D* p);	/* print point with name */
void printf_matrix(char* name, struct Matrix4D* m);	/* prints out matrix */
void copy_vector(struct Vector4D* dst, struct Vector4D* src); /* copies from src to dst */
void matrix_mul_vector(struct Vector4D* dst, struct Matrix4D* m, struct Vector4D* v); /*dst = M*v */
void vector_mul_matrix(struct Vector4D* dst, struct Vector4D* v, struct Matrix4D* m); /* dst = vT*M */
void normalize_vector_byv(struct Vector4D* v);  /* normalizes using v (x,y,z/=v) */
void normalize_vector_3d(struct Vector4D* vec); /* normalizes by 3D<length, v untouched */
double vector_product(struct Vector4D* v1, struct Vector4D* v2); /* returns v1 x v2 */
double vector_product_3d(struct Vector4D* v1, struct Vector4D* v2); /* returns v1 x v2 */
void construct_vector(struct Vector4D* vec, double a1, double a2, double a3, double a4); /* constructs vector */
void construct_vector3(struct Vector4D* vec, double a1, double a2, double a3);
void construct_vector5(struct Vector4D* vec, double a1, double a2, double a3, double a4, double a5);
void copy_matrix(struct Matrix4D* dst, struct Matrix4D* src); /* copies dst <- src */
void copy_3d_matrix(struct Matrix4D* dst, struct Matrix4D* src); /* copies first 3x3 matrix from src to dst */
void matrix_mul(struct Matrix4D* dst, struct Matrix4D* m, struct Matrix4D* n); /* multiplies matrices: dst = m*n */
void identity_matrix(struct Matrix4D* dst); /* dst = I4, loads identity matrix */
void construct_matrix(struct Matrix4D* m,   /* constructs matrix from 16 double values */
	double a11, double a12, double a13, double a14, 
	double a21, double a22, double a23, double a24, 
	double a31, double a32, double a33, double a34, 
	double a41, double a42, double a43, double a44);
void invert_matrix(struct Matrix4D* dst, struct Matrix4D* src); /* inverts matrix using brutal gauss dst = src^(-1) */
								/* can raise exception UNINVERTABLE (exit(1)) */
void invert_rot_matrix(struct Matrix4D* dst, struct Matrix4D* srcC);
void inverted_world_matrix(struct Matrix4D* dst, struct Matrix4D* Mv, struct Matrix4D* transM); /* inverts world matrix Mv, using its properties */
void transp_matrix(struct Matrix4D* dst, struct Matrix4D* src); /* transpones: dst = srcT */
int try_swap(struct Matrix4D* m, int idx); /* used internally by invert_matrix read comments in lib4d.c */
double double_product_z(struct Vector4D* vec, struct Matrix4D* a, double x, double y, int* c); /* comments in lib4d.c */
double ellipsoid_light_normal_from_z(double x,double y,double z, double a,double b,double c);  /* computes lighting from (0,0,1,1)*/
double ellipsoid_light(double x,double y,double z, double a,double b,double c, double xl, double yl, double zl); /* computes lighting from (xl,yl,zl,1.0) */
void construct_cursor4d_default(struct Cursor4D* cur, double siz);	/* creates cursor; read in lib4d.c */
void world_translate(struct Matrix4D* m, double x, double y, double z); /* translates matrix by (x,y,z) */
void world_scale(struct Matrix4D* m, double sx, double sy, double sz);  /* scales matrix by (sx,sy,sz) */
void world_rotate(struct Matrix4D* m, int dir, double ang);             /* rotates matrix by ang using X-axis (dir=0) or Y-axis (dir=1) or Z-axis (else) */
void world_observer_distance(struct Matrix4D* m, double d);		/* setups perspective matrix */
void vector_translatev(struct Vector4D* v, struct Vector4D* d);		/* translate vector/point by d */
void vector_translate(struct Vector4D* v, double dx, double dy, double dz); /* same using double values, functions with postfix "d" uses double values */
									    /* "v" vectors/points, "m" matrices if none written then doubles used typically*/
void vector_rotatex(struct Vector4D* vec, double ang);
void vector_rotatey(struct Vector4D* vec, double ang);
void vector_rotatez(struct Vector4D* vec, double ang);
void average_vector(struct Vector4D* v1, struct Vector4D* v2);
void cursor_translatev(struct Cursor4D* cursor, struct Vector4D* d);	    /* translate cursor by d */
void cursor_translate(struct Cursor4D* cursor, double dx, double dy, double dz);
void cursor_translatemv(struct Cursor4D* cursor, struct Matrix4D* invWorld, struct Vector4D* v); /* translate cursor with Mv^(-1), to guarantee cursor is moving */
												 /* not using world transformation, just like from observer's view point */
void cursor_translatem(struct Cursor4D* cursor, struct Matrix4D* invWorld, double dx, double dy, double dz);
void get_cursor_posv(struct Cursor4D* cursor, struct Vector4D* v);	/* returns cursor position (center) to doubles/vector */
void get_cursor_pos(struct Cursor4D* cursor, double *x, double *y, double *z);
void clear_translations(struct Matrix4D* m);				/* clears translation part of the matrix */
void clear_cursor(struct Cursor4D* cursor);				/* moves cursor to (0,0,0) */
void negate_vector(struct Vector4D* v);					/* negation: v = -v */
void list_add_point(struct Polygon4D** head, double px, double py, double pz);	/* adds point to Point-list (may be used as polygon) */
void list_add_point5(struct Polygon4D** head, double px, double py, double pz, double pv, double pw);
void list_add_pointp(struct Polygon4D** head, struct Polygon4D* pt);	/* adds point to Point-list (may be used as polygon) */
void list_add_pointv(struct Polygon4D** head, struct Vector4D* v);
void list_add_point_nearest(struct Polygon4D** head, struct Polygon4D* nearest, double px, double py, double pz);	/* adds point to Point-list (may be used as polygon) next to the nearest found */
void list_add_pointc(struct Polygon4D** head, struct Cursor4D* cursor);		/* adds point on cursor position */
void list_add_pointc_nearest(struct Polygon4D** head, struct Cursor4D* cursor);		/* adds point on cursor position next to the nearest found */
void list_delete_point_nearest(struct Polygon4D** head, struct Cursor4D* cursor);		/* deletes nearest from cursor point */
struct Polygon4D* get_list_tail(struct Polygon4D* head);					/* returns list's tail */
struct Polygon4D* get_list_head(struct Polygon4D* tail);					/* returns list's head */
struct Polygon4D* list_get_point_nearest(struct Polygon4D** head, struct Cursor4D* cursor);	/* returns pointer to nearest to cursor point on list */
double distance_to_cursor_fast(struct Cursor4D* cursor, struct Polygon4D* point);		/* returns distances to cursor from point of three doubles */
double distance_to_cursord_fast(struct Polygon4D* point, double x, double y, double z);		/* fast means that no sqrt is computed, so result is bad */
double distance_to_cursor(struct Cursor4D* cursor, struct Polygon4D* point);			/* but enough to compare two results: sqrt(x) < sqrt(y) <=> x < y */
double distance_to_cursord(struct Polygon4D* point, double x, double y, double z);
void point_translatev(struct Polygon4D* point, struct Vector4D* d);				/* translate point by vector or doubles */
void point_translate(struct Polygon4D* point, double dx, double dy, double dz);
void point_translatemv(struct Polygon4D* point, struct Matrix4D* invWorld, struct Vector4D* v);
void point_translatem(struct Polygon4D* point, struct Matrix4D* invWorld, double dx, double dy, double dz);
void point_translatevm(struct Vector4D* point, struct Matrix4D* invWorld, double dx, double dy, double dz);
void line_cursor_to_point(struct Line4D* line, struct Cursor4D* cursor, struct Polygon4D* point);	/* returns line connectiong cursor with point */
void list_get_splines_C0_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree);
void list_get_bcurves_C0_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree);
void list_get_C0_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree, int mindim, int maxdim);
void list_get_C0_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degree);
void list_get_C0_dim_nomulti(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degree); /* do not copy control points to satisfy degree */
void list_get_C0(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec);
void list_get_splines_C1_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree);
void list_get_bcurves_C1_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree);
void list_get_C1_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree, int mindim, int maxdim);
void list_get_C1_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degree);
void list_get_C1(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec);
void list_get_splines_C2_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree);
void list_get_bcurves_C2_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree);
void list_get_C2_freedim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int* degree, int mindim, int maxdim);
void list_get_C2_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degree);
void list_get_C2(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec);
void bspline_set(struct BSpline* bspline, int K, int M, int interval, struct Vector4D* points); /* computes bspline */
double bspline_compute(struct BSpline* bspline, double t, int K, int point_xyz);		/* computes bspline for given t */
double basis_bspline(double* T, double val, int dim, int off, int I, int J);	/* will compute basis for given parameters */
void zero_vector(struct Vector4D* vec);
void copy_spline(struct BSpline* dst, struct BSpline* src);			/* copies entire spline with computed points used by Visualizer*/
void zero_spline(struct BSpline* spline);					/* generates zero spline */
void free_spline(struct BSpline* spline);					/* frees spline memory */
void bcurve_set(struct BezierCurve* bcurve, int N, int interval, struct Vector4D* points);	/* computes bezier curve points */
double bcurve_compute(struct BezierCurve* bcurve, double t, int N, int point_xyz);	/* computes bezier curve for given t */
double bezier_compute(int n, int i, double t);						/* computes bezier B(n,i)(t) */
int list_save_to_file(struct Polygon4D* head, char* fn);				/* loads point list from file */
int list_tail_save_to_file(struct Polygon4D* tail, char* fn);
int list_load_from_file(struct Polygon4D** head, char* fn);				/* saves point list to file */
void list_free(struct Polygon4D** head);						/* frees list */
void copy_bezier(struct BezierCurve* dst, struct BezierCurve* src);			/* copies entire bezier with computed points used by Visualizer*/
void zero_bezier(struct BezierCurve* spline);					/* generates zero bezier */
void free_bezier(struct BezierCurve* spline);					/* frees bezier memory */
void printf_list(char* name, struct Polygon4D* head);
void get_bezier_polygon(struct Vector4D* control, int ncontrol, struct Vector4D** vertex);
void get_bezier_polygon4(struct Vector4D* control, int ncontrol, struct Vector4D** vertex);
void vector_subtract_3d(struct Vector4D* dst, struct Vector4D* vec);
int idx_used(int idx, int* idx_tab, int nidx);
void check_C1_constraints(struct Polygon4D* head, int degree, int scaled); /* set scaled to 1 if You want to use deltas */
void check_C2_constraints(struct Polygon4D* head, int degree, int scaled); /* set scaled to 1 if You want to use deltas */
void check_C22_constraints(struct Polygon4D* head, int degree, int scaled); /* set scaled to 1 if You want to use deltas */
void deboor_from_bezier(struct BSpline* spline, int nspline, struct Polygon4D** bl);
void deboor_from_bezier_scaled(struct BSpline* spline, int nspline, struct Polygon4D** bl);
void deboor_from_bezier_inv(struct BSpline* spline, int nspline, struct Polygon4D** bl);
void bezier_from_deboor_multi(struct Polygon4D** bez, struct Polygon4D* boor);
void bezier_from_deboor(struct Polygon4D** bez, struct Polygon4D* boor);
void bezier_from_deboor_scaled(struct Polygon4D** bez, struct Polygon4D* boor);
void bezier_from_interpol(struct Polygon4D** bez, struct Polygon4D* interpol);
void nurb_set(struct Nurb* nurb, int K1, int M1, int K2, int M2, int intervalx, int intervaly, struct Vector4D* points);
void nurb_setn(struct Nurb* nurb, int K1, int M1, int K2, int M2, int intervalx, int intervaly, struct Vector4D* points);
double nurb_compute(struct Nurb* nurb, double s, double t, int K1, int K2, int point_xyz);
void nurb_computen(struct Nurb* nurb, double s, double t, int K1, int K2, struct Vector4D* n);
void list_get_C02_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degreex, int degreey);
void list_get_C22(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec);
void list_get_C22_dim(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int degreex, int degreey);
void list_get_C02(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec);
void list_get_C02_49(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec);
void list_get_C02_nm(struct Polygon4D* head, struct Vector4D*** spvec, int* nvec, int n, int m);
void create_interpolation(struct Polygon4D** intr_curve, struct Polygon4D* intr_points, int np);
void table_to_list(struct Polygon4D** list, struct Vector4D** bz, int x, int y);
void list_to_table(struct Polygon4D* list, struct Vector4D** bz, int x, int y);
void update_bezier_2d(struct Vector4D** bz, struct Vector4D** boo, int i, int j, int nox, int noy);
void bezier_from_deboor_2d(struct Polygon4D** list, struct Vector4D*** bz, struct Vector4D** boo, int* nzx, int* nzy, int nox, int noy);
void default_vec_array(struct Vector4D** va, int x, int y);
void nurb_free(struct Nurb* nurb);
void bezier_from_deboor_2da(struct Vector4D*** bz, struct Vector4D** boo, int* nzx, int* nzy, int nox, int noy);
struct Vector4D* get_point_nearesta(struct Vector4D** arr, int x, int y, struct Cursor4D* cursor, int* sx, int *sy);
double distance_to_cursordv_fast(struct Vector4D* point, double x, double y, double z);
void line_cursor_to_pointv(struct Line4D* line, struct Cursor4D* cursor, struct Vector4D* point);
void get_C2a_nm(struct Vector4D** bez, int nbx, int nby, struct Vector4D**** spl, int n, int m);
void point_translateam(struct Vector4D** pt, int i, int j, struct Matrix4D* invWorld, double dx, double dy, double dz);
void bezier_from_deboor_idx_2da(struct Vector4D** bz, struct Vector4D** boo, int nbx, int nby, int I, int J);
void get_control_points(struct Vector4D** bez, int nbx, int nby, struct Vector4D** spl, int i, int j);
void vector_mul(struct Vector4D* v, struct Vector4D* a, struct Vector4D* b);
void get_2d2vec(struct Vector4D** bez, struct Vector4D** spl);
void get_2d2vec_gp(struct Vector4D** bez, struct Vector4D** aux, struct Vector4D** spl);
void calculate_dcp(struct Vector4D** patch, double t, struct Vector4D** first, struct Vector4D** second);
void calc_gp(struct Nurb** gnurbs, struct Vector4D*** bez, struct Vector4D**** bp1, struct Vector4D**** bp2, int lpsegm);
struct Vector4D* get_point_nearesta3(struct Vector4D*** arr, int C, int X, int Y, struct Cursor4D* cursor, int *sc, int* sx, int *sy);
int get_point_idx_a3(int c, int x, int y);
struct Vector4D* get_point_a3(struct Vector4D*** bz, int idx);
void correctC2(struct Vector4D*** bz, struct Vector4D*** points, struct Vector4D*** points2, int no);
void make_C2(struct Vector4D*** bz, struct Vector4D*** bp1, struct Vector4D*** bp2, int c, int x, int y);
void make_C1(struct Vector4D*** bz, int c, int x, int y);
int innormal_point(struct Vector4D* pt);
struct VList* get_vlist_head(struct VList* tail);
struct VList* get_vlist_tail(struct VList* head);
int trimm(struct TrimmedMesh* tm);
/*void gen_trimm_matrices(struct TrimmedMesh* tm, int size);*/
void gen_trimm_matrices(struct TrimmedMesh* tm, int sizex1, int sizey1, int sizex2, int sizey2);
int nurb_intersection_box(struct Nurb* n1, struct Nurb* n2);
void set_param_vector(struct TrimmedMesh* tm, double du, double dv, double ds, double dt, double factor);
int nurb_pseudo_intersection_box(struct Nurb* n1, struct Nurb* n2);
void get_nurb_box(struct Nurb* n, struct Vector4D* vmin, struct Vector4D* vmax);
void get_nurb_pseudo_box(struct Nurb* n, struct Vector4D* vmin, struct Vector4D* vmax);
int box_intersect(struct Vector4D* min1, struct Vector4D* max1, struct Vector4D* min2, struct Vector4D* max2);
void vlist_add(struct VList** head, void* data);
int find_inter_curve(struct TrimmedMesh* tm);
void nurb_computev(struct Nurb* nurb, struct Vector4D* vec, double xm, double ym);
void nurb_computemv(struct Mesh4D* m, struct Vector4D* vec, double u, double v);
double vector_distance(struct Vector4D* v1, struct Vector4D* v2);
int find_initial_point(struct TrimmedMesh* tm, struct Vector4D* p1, struct Vector4D* p2, struct Vector4D* x);
void nurb_compute_dumv(struct Mesh4D* m1, struct Vector4D* vec, double u, double v);
void nurb_compute_dvmv(struct Mesh4D* m1, struct Vector4D* vec, double u, double v);
double vector_length(struct Vector4D* v);
int root_finder(struct TrimmedMesh* tm, struct Vector4D* xi);
void root_finder_f2(struct TrimmedMesh* tm, struct Vector4D* retVal, struct Vector4D* x);
void root_finder_fp2(struct TrimmedMesh* tm, struct Matrix4D* retVal, struct Vector4D* x);
void set_param_vector(struct TrimmedMesh* tm, double du, double dv, double ds, double dt, double factor);
void update_trimm_curves(struct TrimmedMesh* tm, struct Vector4D* vec);
void zero_trimm_curves(struct TrimmedMesh* tm);
void map_curve_to_matrix(struct TrimmedMesh* tm, int idx, struct VList* curve, unsigned char** m, int size, int nx, int ny);
void print_curve_distance(struct TrimmedMesh* tm);
void gen_trimm_matrices_with_matrix(struct TrimmedMesh* tm, int size);
