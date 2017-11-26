/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include "lib4d.h"
#include "GL/glut.h"
#include <time.h>
/* #define PI 3.1415926535F */
#define DIM 4
#define NMESH 2
struct Matrix4D perspectiveMatrix;
struct Matrix4D scaleMatrix;
struct Matrix4D shiftMatrix;
struct Matrix4D translationMatrix;
struct Matrix4D rotXMatrix;
struct Matrix4D rotYMatrix;
struct Matrix4D rotZMatrix;
struct Matrix4D worldMatrix;
struct Matrix4D invertedMv;
double angX, angY, angZ, obsD, tStep;
struct Matrix4D** batchMatrix;
struct Cursor4D cursor4d;
struct Mesh4D mesh[NMESH];
struct TrimmedMesh tmesh;
struct Vector4D* arrU;
struct Vector4D* arrV;
struct Vector4D* arrS;
struct Vector4D* arrT;
time_t t1,t2;
int fps;
double d,gs;
int wx,wy;
int cx,cy;
int curr_eye;
int r;
int want_bez;
int want_boo;
int si,sj;
int cm;
int pmove;
int lpsegm;
int want_lines;
int nel;
int want_in2;
int want_in1;

void help()
{
 printf("usage ./scene4d [scenefile]\n");
 printf("controls: translations: f/h/t/g/r/y\n");
 printf("                cursor: j/l/i/k/u/o\n");
 printf("              rotating: z/x c/v b/n\n");
 printf("     return to normals: e\n");
 printf("             quit,help: q,0\n");
 printf("     toggle bez points: m\n");
 printf("     toggle boo points: /\n");
 printf("           change trim: a/s\n");
 printf("toggle lin/pts/triangs: d\n");
}


void compute_vector(struct Vector4D* vec)
{
 struct Vector4D temp;
 copy_vector(&temp, vec);
 vector_mul_matrix(vec, &temp, &worldMatrix);
 normalize_vector_byv(vec);
 copy_vector(&temp, vec);
 vector_mul_matrix(vec, &temp, &shiftMatrix);
}


void setup_matrices()
{
 
 angX =  1.0;
 angY =  -1.0;
 angZ =  0.5;
 d = 4.;
 obsD = 6000.0;
 tStep = 2.00;
 gs = 0.4;
 construct_matrix(&perspectiveMatrix, 1.0,0.0,0.0,0.0,
	 			      0.0,1.0,0.0,0.0,
	 			      0.0,0.0,1.0,1.0/obsD,
	 			      0.0,0.0,0.0,1.0);
 construct_matrix(&scaleMatrix,       gs ,0.0,0.0,0.0,
	 			      0.0,gs ,0.0,0.0,
	 			      0.0,0.0,gs ,0.0,
	 			      0.0,0.0,0.0,1.0);
 construct_matrix(&shiftMatrix,       1.0,0.0,0.0,0.0,
	 			      0.0,1.0,0.0,0.0,
	 			      0.0,0.0,1.0,0.0,
	 			      wx/2,wy/2,0.0,1.0);
 construct_matrix(&translationMatrix, 1.0,0.0,0.0,0.0,
	 			      0.0,1.0,0.0,0.0,
	 			      0.0,0.0,1.0,0.0,
	 			      0.0,0.0,0.0,1.0);
 construct_matrix(&rotXMatrix,        1.0,0.0,0.0,0.0,
	 			      0.0,cos(angX),-sin(angX),0.0,
	 			      0.0,sin(angX),cos(angX),0.0,
	 			      0.0,0.0,0.0,1.0);
 construct_matrix(&rotYMatrix,        cos(angY),0.0,sin(angY),0.0,
	 			      0.0,1.0,0.0,0.0,
	 			      -sin(angY),0.0,cos(angY),0.0,
	 			      0.0,0.0,0.0,1.0);
 construct_matrix(&rotZMatrix,        cos(angZ),-sin(angZ),0.0,0.0,
	 			      sin(angZ),cos(angZ),0.0,0.0,
	 			      0.0,0.0,1.0,0.0,
	 			      0.0,0.0,0.0,1.0);
}


void setup_batch()
{
 batchMatrix = (struct Matrix4D**)malloc(7*sizeof(struct Matrix4D*));
 if (!batchMatrix)
   {
    printf("Error: cannot allocate batchMatrix\n");
    exit(1);
   }
 batchMatrix[0] = &rotXMatrix;
 batchMatrix[1] = &rotYMatrix;
 batchMatrix[2] = &rotZMatrix;
 batchMatrix[3] = &scaleMatrix;
 batchMatrix[4] = &translationMatrix;
 batchMatrix[5] = &perspectiveMatrix;
 batchMatrix[6] = &shiftMatrix;
}


void nurbs_free()
{
 int i, j, m;
 for (m=0;m<NMESH;m++)
   {
    for (i=0;i<mesh[m].nNurbsX;i++)
    for (j=0;j<mesh[m].nNurbsY;j++) nurb_free(mesh[m].nurbs[i][j]);
    for (i=0;i<mesh[m].nNurbsX;i++) free(mesh[m].nurbs[i]);
    free(mesh[m].nurbs);
    mesh[m].nurbs = NULL;
   }
}


void full_update_nurbs()
{
 int i,j,m;
 struct Vector4D*** control_points;
 nurbs_free();
 for (m=0;m<NMESH;m++)
  {
   mesh[m].nNurbsX = mesh[m].nNurbsY = 0;
   mesh[m].nurbs = NULL;
   control_points = NULL;
   get_C2a_nm(mesh[m].bez, mesh[m].nBezX, mesh[m].nBezY, &control_points, mesh[m].nX, mesh[m].nY);
   mesh[m].nNurbsX = mesh[m].nX;
   mesh[m].nNurbsY = mesh[m].nY;
   if (mesh[m].nNurbsX <= 0 || mesh[m].nNurbsY <= 0) continue ;
   mesh[m].nurbs = (struct Nurb***)malloc(mesh[m].nNurbsX*sizeof(struct Nurb**));
   for (i=0;i<mesh[m].nNurbsX;i++) mesh[m].nurbs[i] = (struct Nurb**)malloc(mesh[m].nNurbsY*sizeof(struct Nurb*));
   for (i=0;i<mesh[m].nNurbsX;i++)
   for (j=0;j<mesh[m].nNurbsY;j++) mesh[m].nurbs[i][j] = (struct Nurb*)malloc(sizeof(struct Nurb));
   for (i=0;i<mesh[m].nNurbsX;i++)
   for (j=0;j<mesh[m].nNurbsY;j++) nurb_set(mesh[m].nurbs[i][j], 3, 3, 3, 3, lpsegm, lpsegm, control_points[i][j]);
   for (i=0;i<mesh[m].nNurbsX;i++)
   for (j=0;j<mesh[m].nNurbsY;j++) free(control_points[i][j]);
   for (i=0;i<mesh[m].nNurbsX;i++) free(control_points[i]);
   free(control_points);
   control_points = NULL;
  }
}


void update_nurbs_idx(int x, int y, int m)
{
 struct Vector4D* cp;
 int fromx,fromy,tox,toy,i,j;
 if (m<0 || m >= NMESH || x<0 || y<0 || x>=mesh[m].nBoorX || y>=mesh[m].nBoorY) return;
 fromx = x-3;
 fromy = y-3;
 tox   = x;
 toy   = y;
 if (fromx < 0) fromx = 0;
 if (fromy < 0) fromy = 0;
 if (tox >= mesh[m].nNurbsX) tox = mesh[m].nNurbsX-1;
 if (toy >= mesh[m].nNurbsY) toy = mesh[m].nNurbsY-1;
 for (i=fromx;i<=tox;i++)
 for (j=fromy;j<=toy;j++)
   {
    cp = NULL;
    get_control_points(mesh[m].bez, mesh[m].nBezX, mesh[m].nBezY, &cp, i, j);
    nurb_free(mesh[m].nurbs[i][j]);
    nurb_set(mesh[m].nurbs[i][j], 3, 3, 3, 3, lpsegm, lpsegm, cp);
    free(cp);
   }
}


void render_line(struct Line4D* line)
{
 struct Line4D rline;
 copy_vector(&rline.v[0], &line->v[0]);
 copy_vector(&rline.v[1], &line->v[1]);
 compute_vector(&rline.v[0]);
 compute_vector(&rline.v[1]);
 if (rline.v[0].p[2]<-obsD || rline.v[1].p[2]<-obsD) return;
 if (rline.v[0].p[3]!=1.0 || rline.v[1].p[3]!=1.0) return;
 if (rline.v[0].p[0] < -200.0 || rline.v[1].p[0] > wx+200.0) return;
 if (rline.v[0].p[1] < -200.0 || rline.v[1].p[1] > wy+200.0) return;
 glBegin(want_lines ? GL_LINES : GL_POINTS);
   glVertex3d((GLdouble)rline.v[0].p[0], (GLdouble)rline.v[0].p[1], 0.);
   glVertex3d((GLdouble)rline.v[1].p[0], (GLdouble)rline.v[1].p[1], 0.);
 glEnd();
}


void render_point(struct Vector4D* pt)
{
 struct Vector4D rpt;
 copy_vector(&rpt, pt);
 compute_vector(&rpt);
 if (rpt.p[2]<-obsD) return;
 if (rpt.p[3]!=1.0) return;
 if (rpt.p[0] < -200.0 || rpt.p[0] > wx+200.0) return;
 glBegin(want_lines ? GL_LINES : GL_POINTS);
   glVertex3d(rpt.p[0]-5, rpt.p[1]  , 0.);
   glVertex3d(rpt.p[0],   rpt.p[1]+5, 0.);
   glVertex3d(rpt.p[0],   rpt.p[1]+5, 0.);
   glVertex3d(rpt.p[0]+5, rpt.p[1],   0.);
   glVertex3d(rpt.p[0]+5, rpt.p[1]  , 0.);
   glVertex3d(rpt.p[0],   rpt.p[1]-5, 0.);
   glVertex3d(rpt.p[0],   rpt.p[1]-5, 0.);
   glVertex3d(rpt.p[0]-5, rpt.p[1]  , 0.);
 glEnd();
}

void render_point_small(struct Vector4D* pt)
{
 struct Vector4D rpt;
 copy_vector(&rpt, pt);
 compute_vector(&rpt);
 if (rpt.p[2]<-obsD) return;
 if (rpt.p[3]!=1.0) return;
 if (rpt.p[0] < -200.0 || rpt.p[0] > wx+200.0) return;
 glBegin(want_lines ? GL_LINES : GL_POINTS);
   glVertex3d(rpt.p[0]-2, rpt.p[1]  , 0.);
   glVertex3d(rpt.p[0],   rpt.p[1]+2, 0.);
   glVertex3d(rpt.p[0],   rpt.p[1]+2, 0.);
   glVertex3d(rpt.p[0]+2, rpt.p[1],   0.);
   glVertex3d(rpt.p[0]+2, rpt.p[1]  , 0.);
   glVertex3d(rpt.p[0],   rpt.p[1]-2, 0.);
   glVertex3d(rpt.p[0],   rpt.p[1]-2, 0.);
   glVertex3d(rpt.p[0]-2, rpt.p[1]  , 0.);
 glEnd();
}


void render_point_bez(struct Vector4D* pt)
{
 struct Vector4D rpt;
 copy_vector(&rpt, pt);
 compute_vector(&rpt);
 if (rpt.p[2]<-obsD) return;
 if (rpt.p[3]!=1.0) return;
 if (rpt.p[0] < -200.0 || rpt.p[0] > wx+200.0) return;
 glBegin(want_lines ? GL_LINES : GL_POINTS);
   glVertex3d(rpt.p[0]-2,   rpt.p[1]-2, 0.);
   glVertex3d(rpt.p[0]+2,   rpt.p[1]+2, 0.);
   glVertex3d(rpt.p[0]-2,   rpt.p[1]+2, 0.);
   glVertex3d(rpt.p[0]+2,   rpt.p[1]-2, 0.);
 glEnd();
}


void cursor_2d_pos(struct Line4D* line)
{
 struct Line4D rline;
 copy_vector(&rline.v[0], &line->v[0]);
 copy_vector(&rline.v[1], &line->v[1]);
 compute_vector(&rline.v[0]);
 compute_vector(&rline.v[1]);
 cx = (int)((rline.v[0].p[0] + rline.v[1].p[0]) / 2.0);
 cy = (int)((rline.v[0].p[1] + rline.v[1].p[1]) / 2.0);
}


void render_cursor(struct Cursor4D* cur)
{
 render_line(&cur->l[0]);
 render_line(&cur->l[1]);
 render_line(&cur->l[2]);
 cursor_2d_pos(&cur->l[0]);
}


void render_nurb(struct Nurb* nurb)
{
 struct Line4D line;
 int i,j;
 if (nurb)
   {
    for (i=0;i<nurb->npointsx;i++)
    for (j=0;j<nurb->npointsy;j++)
     {
      copy_vector(&line.v[0], &nurb->points[i][j]);
      copy_vector(&line.v[1], &nurb->points[i+1][j]);
      render_line(&line);
      copy_vector(&line.v[0], &nurb->points[i][j]);
      copy_vector(&line.v[1], &nurb->points[i][j+1]);
      render_line(&line);
      if (want_lines == 2)
        {
         copy_vector(&line.v[0], &nurb->points[i][j]);
         copy_vector(&line.v[1], &nurb->points[i+1][j+1]);
         render_line(&line);
	}
     }
   }
}

void render_mesh(struct Mesh4D* mesh)
{
 double i,j;
 double nx,ny;
 struct Vector4D vec;
 nx = (double)mesh->nX;
 ny = (double)mesh->nY;
 for (i=0.;i<nx;i+=0.25)
 for (j=0.;j<ny;j+=0.25)
   {
    nurb_computemv(mesh, &vec, i, j);
    render_point(&vec);
   }
}

void render_trim_curve(struct VList* curve)
{
 struct VList* tmp;
 struct Vector4D* vec;
 tmp = curve;
/* if (tmp) printf("rendering trim curve!\n");*/
 while (tmp)
   {
    vec = (struct Vector4D*)tmp->data;
    render_point_small(vec);
    tmp = tmp->next;
   }
}


double find_t(double s, double diff)
{
 int i,mini;
 double distance;
 double mindistance;
/* printf("find_t called for s=%f, diff=%f\n", s,diff);*/
 mindistance = diff;
 mini = -1;
 for (i=0;i<nel;i++)
   {
    distance = fabs(s - arrS[i].p[2]);
    if (distance < mindistance) 
      {
       mindistance = distance; 
       mini = i; 
      }
   }
 if (mini >=0) { /*printf("mindistance = %f\n", arrS[mini].p[2]);*/ return arrS[mini].p[3]; }
 else { /*printf("not found t\n");*/ return -1.; }
}

double find_v(double u, double diff)
{
 int i,mini;
 double distance;
 double mindistance;
/* printf("find_v called for u=%f, diff=%f\n", u,diff);*/
 mindistance = diff;
 mini = -1;
 for (i=0;i<nel;i++)
   {
    distance = fabs(u - arrU[i].p[0]);
    if (distance < mindistance) 
      {
       mindistance = distance; 
       mini = i; 
      }
   }
 if (mini >=0) { /*printf("mindistance = %f\n", arrU[mini].p[0]);*/ return arrU[mini].p[1]; }
 else { /*printf("not found v\n");*/ return -1.; }
}

double find_vi(double u, double diff, int* idx)
{
 int i,mini;
 double distance;
 double mindistance;
/* printf("find_v called for u=%f, diff=%f\n", u,diff);*/
 mindistance = diff;
 mini = -1;
 for (i=0;i<nel;i++)
   {
    distance = fabs(u - arrU[i].p[0]);
    if (distance < mindistance) 
      {
       mindistance = distance; 
       mini = i; 
      }
   }
 *idx = mini;
 if (mini >=0) { /*printf("mindistance = %f\n", arrU[mini].p[0]);*/ return arrU[mini].p[1]; }
 else { /*printf("not found v\n");*/ return -1.; }
}

void find_vmm(double u, double diff, double uval, int vidx, double* minv, double* maxv)
{
 int idx;
 *maxv = *minv = -1.;
 if (vidx == -1) return;
 idx = vidx;
 *maxv = arrU[vidx].p[1];
 *minv = arrU[vidx].p[1];
 vidx++;
 while (fabs(arrU[vidx].p[0] - uval) < 0.01)
 {
/*  printf("id-->: %f,%f\n", arrU[vidx].p[0], arrU[vidx].p[1]);*/
  if (arrU[vidx].p[1] < (*minv)) *minv = arrU[vidx].p[1];
  if (arrU[vidx].p[1] > (*maxv)) *maxv = arrU[vidx].p[1];
  vidx++;
 }
 vidx = idx-1;
 while (fabs(arrU[vidx].p[0] - uval) < 0.01)
 {
/*  printf("id-->: %f,%f\n", arrU[vidx].p[0], arrU[vidx].p[1]);*/
  if (arrU[vidx].p[1] < (*minv)) *minv = arrU[vidx].p[1];
  if (arrU[vidx].p[1] > (*maxv)) *maxv = arrU[vidx].p[1];
  vidx--;
 }
}

void render_trimm_curves(struct TrimmedMesh* tm)
{
 struct Line4D line;
 int i,j,vidx;
 double u,v,s,t;
 double u2,v2,s2,t2;
 double maxv,maxt,maxv2,maxt2,Maxv,Maxv2;
 struct Vector4D vec;
 if (!want_in1)
 {
 for (i=2;i<tm->npx1-3;i++)
   {
    u = (double)i/(double)tm->m1->nX;
    maxv = find_vi(u, .5*((double)tm->m1->nX/(double)tm->npx1), &vidx);
    find_vmm(u, .5*((double)tm->m1->nX/(double)tm->npx1), u, vidx, &maxv, &Maxv);
    
    u2 = (double)(i+1)/(double)tm->m1->nX;
    maxv2 = find_vi(u2, .5*((double)tm->m1->nX/(double)tm->npx1), &vidx);
    find_vmm(u2, .5*((double)tm->m1->nX/(double)tm->npx1), u2, vidx, &maxv2, &Maxv2);
    for (j=0;j<tm->npy1-1;j++)
      {
       v = (double)j/(double)tm->m1->nY;
       v2 = (double)(j+1)/(double)tm->m1->nY;
       if ( maxv >= 0. && Maxv >= 0. && v > maxv && v < Maxv) 
         {
          v = maxv;
	  nurb_computemv(tm->m1, &vec, u, v);
	  render_point_small(&vec);
	  continue;
         }
       if ( maxv2 >= 0. && Maxv2 >= 0. && v2 > maxv2 && v2 < Maxv2) 
         {
          v2 = maxv2;
	  nurb_computemv(tm->m1, &vec, u2, v2);
	  render_point_small(&vec);
	  continue;
         }
       copy_vector(&line.v[0], &tm->points1[i][j]);
       copy_vector(&line.v[1], &tm->points1[i+1][j]);
       render_line(&line);
       copy_vector(&line.v[0], &tm->points1[i][j+1]);
       copy_vector(&line.v[1], &tm->points1[i][j]);
       render_line(&line);
      }
   }
 }
 else
 {
 for (i=2;i<tm->npx1-3;i++)
   {
    u = (double)i/(double)tm->m1->nX;
    maxv = find_vi(u, .5*((double)tm->m1->nX/(double)tm->npx1), &vidx);
    find_vmm(u, .5*((double)tm->m1->nX/(double)tm->npx1), u, vidx, &maxv, &Maxv);
    
    u2 = (double)(i+1)/(double)tm->m1->nX;
    maxv2 = find_vi(u2, .5*((double)tm->m1->nX/(double)tm->npx1), &vidx);
    find_vmm(u2, .5*((double)tm->m1->nX/(double)tm->npx1), u2, vidx, &maxv2, &Maxv2);
    if (maxv < 0. || Maxv < 0.) continue;
    for (j=0;j<tm->npy1;j++)
      {
       v = (double)j/(double)tm->m1->nY;
       v2 = (double)(j+1)/(double)tm->m1->nY;
       if (v < maxv || v > Maxv) 
         {
          v = maxv;
	  nurb_computemv(tm->m1, &vec, u, v);
	  render_point_small(&vec);
	  continue;
         }
       if ( maxv2 >= 0. && Maxv2 >= 0. && v2 > maxv2 && v2 < Maxv2) 
         {
          v2 = maxv2;
	  nurb_computemv(tm->m1, &vec, u2, v2);
	  render_point_small(&vec);
	  continue;
         }
       copy_vector(&line.v[0], &tm->points1[i][j]);
       copy_vector(&line.v[1], &tm->points1[i+1][j]);
       render_line(&line);
       copy_vector(&line.v[0], &tm->points1[i][j+1]);
       copy_vector(&line.v[1], &tm->points1[i][j]);
       render_line(&line);
      }
   }
 }
 if (want_in2)
 {
 for (i=0;i<tm->npx2-1;i++)
   {
    s  = (double)i/(double)tm->m2->nX;
    s2 = (double)(i+1)/(double)tm->m2->nX;
    maxt  = find_t(s,  (double)tm->m2->nX/(double)tm->npx2);
    maxt2 = find_t(s2, (double)tm->m2->nX/(double)tm->npx2);
    if (maxt < 0.) continue;
    for (j=0;j<tm->npy2-1;j++)
      {
       t =  (double)j/(double)tm->m2->nY;
       t2 = (double)(j+1)/(double)tm->m2->nY;
       if ( maxt >= 0. && t > maxt) 
         {
          t = maxt;
	  nurb_computemv(tm->m2, &vec, s, t);
	  render_point_small(&vec);
	  break;
         }
       if ( maxt2 >= 0. && t2 > maxt2) 
         {
          t2 = maxt2;
	  nurb_computemv(tm->m2, &vec, s2, t2);
	  render_point_small(&vec);
	  break;
         }
       copy_vector(&line.v[0], &tm->points2[i][j]);
       copy_vector(&line.v[1], &tm->points2[i+1][j]);
       render_line(&line);
       copy_vector(&line.v[0], &tm->points2[i][j+1]);
       copy_vector(&line.v[1], &tm->points2[i][j]);
       render_line(&line);
      }
   }
 }
 else
 {
 for (i=0;i<tm->npx2-1;i++)
   {
    s  = (double)i/(double)tm->m2->nX;
    s2 = (double)(i+1)/(double)tm->m2->nX;
    maxt  = find_t(s,  (double)tm->m2->nX/(double)tm->npx2);
    maxt2 = find_t(s2, (double)tm->m2->nX/(double)tm->npx2);
    if (maxt < 0.) continue;
    for (j=tm->npy2-2;j>=0;j--)
      {
       t =  (double)j/(double)tm->m2->nY;
       t2 = (double)(j+1)/(double)tm->m2->nY;
       if ( maxt >= 0. && t < maxt) 
         {
          t = maxt;
	  nurb_computemv(tm->m2, &vec, s, t);
	  render_point_small(&vec);
	  break;
         }
       if ( maxt2 >= 0. && t2 < maxt2) 
         {
          t2 = maxt2;
	  nurb_computemv(tm->m2, &vec, s2, t2);
	  render_point_small(&vec);
	  break;
         }
       copy_vector(&line.v[0], &tm->points2[i][j]);
       copy_vector(&line.v[1], &tm->points2[i+1][j]);
       render_line(&line);
       copy_vector(&line.v[0], &tm->points2[i][j+1]);
       copy_vector(&line.v[1], &tm->points2[i][j]);
       render_line(&line);
      }
   }
 }
 /*else
 {
 for (i=0;i<tm->npx2;i++)
   {
    s = (double)i/(double)tm->m2->nX;
    maxt = find_t(s, (double)tm->m2->nX/(double)tm->npx2);
    if (maxt < 0.) continue;
    for (j=tm->npy2-1;j>=0;j--)
      {
       t = (double)j/(double)tm->m2->nY;
       if ( maxt >= 0. && t < maxt) 
         {
          t = maxt;
	  nurb_computemv(tm->m2, &vec, s, t);
	  render_point_small(&vec);
	  break;
         }
       render_point_small(&tm->points2[i][j]);
      }
   }
 }*/
 /*if (!want_in1)
 {
 for (i=2;i<tm->npx1-2;i++)
   {
    u = (double)i/(double)tm->m1->nX;
    maxv = find_vi(u, .5*((double)tm->m1->nX/(double)tm->npx1), &vidx);
    find_vmm(u, .5*((double)tm->m1->nX/(double)tm->npx1), u, vidx, &maxv, &Maxv);
    for (j=0;j<tm->npy1;j++)
      {
       v = (double)j/(double)tm->m1->nY;
       if ( maxv >= 0. && Maxv >= 0. && v > maxv && v < Maxv) 
         {
          v = maxv;
	  nurb_computemv(tm->m1, &vec, u, v);
	  render_point_small(&vec);
	  continue;
         }
       render_point_small(&tm->points1[i][j]);
      }
   }
 }
 else
 {
 for (i=2;i<tm->npx1-2;i++)
   {
    u = (double)i/(double)tm->m1->nX;
    maxv = find_vi(u, .5*((double)tm->m1->nX/(double)tm->npx1), &vidx);
    find_vmm(u, .5*((double)tm->m1->nX/(double)tm->npx1), u, vidx, &maxv, &Maxv);
    if (maxv < 0. || Maxv < 0.) continue;
    for (j=0;j<tm->npy1;j++)
      {
       v = (double)j/(double)tm->m1->nY;
       if (v < maxv || v > Maxv) 
         {
          v = maxv;
	  nurb_computemv(tm->m1, &vec, u, v);
	  render_point_small(&vec);
	  continue;
         }
       render_point_small(&tm->points1[i][j]);
      }
   }
 }*/
 if (tm->tc1) render_trim_curve(tm->tc1);
 if (tm->tc2) render_trim_curve(tm->tc2);
}

void render_nurbs_old()
{
 int m;
 for (m=0;m<NMESH;m++) render_mesh(&mesh[m]);
}

void render_nurbs()
{
 /*int i,j,m;
 for (m=0;m<NMESH;m++)
 for (i=0;i<mesh[m].nNurbsX;i++)
 for (j=0;j<mesh[m].nNurbsY;j++) render_nurb(mesh[m].nurbs[i][j]);*/
 render_trimm_curves(&tmesh);
}


void update_cursorpos()
{
 char* window_name;
 double x,y,z;
 get_cursor_pos(&cursor4d, &x, &y, &z);
 window_name = (char*)malloc(0x100);
 sprintf(window_name, "Cursor (%f,%f,%f) --> (%d,%d)", x,y,z,cx,cy);
 glutSetWindowTitle(window_name);
 free(window_name);
}


void clean_up()
{
 int i,m;
 nurbs_free();
 for (m=0;m<NMESH;m++)
   {
    if (mesh[m].boor) for (i=0;i<mesh[m].nBoorX;i++) { free(mesh[m].boor[i]); mesh[m].boor[i] = NULL; }
    free(mesh[m].boor);
    mesh[m].boor = NULL;
    if (mesh[m].bez) for (i=0;i<mesh[m].nBezX;i++) { free(mesh[m].bez[i]); mesh[m].bez[i] = NULL; }
    free(mesh[m].bez);
    mesh[m].bez = NULL;
   }
}


void keyboard(unsigned char key, int x, int y)
{
 y = x = 0;
 switch (key)
   {
    case 27: case 'q': clean_up(); exit(0); break;
	case '0': help(); break;
	case 'd': want_lines++; if (want_lines==3) want_lines = 0; break;
	case 'a': want_in1 = ! want_in1; break;
	case 's': want_in2 = ! want_in2; break;
	case 'j':
		if (!pmove)    cursor_translatem(&cursor4d, &invertedMv, -5*tStep, 0.0, 0.0);
		else           point_translateam(mesh[cm].boor, si, sj, &invertedMv, -5*tStep, 0.0, 0.0);
	    break;
	case 'l':
		if (!pmove)    cursor_translatem(&cursor4d, &invertedMv, 5*tStep, 0.0, 0.0);
		else           point_translateam(mesh[cm].boor, si, sj, &invertedMv, 5*tStep, 0.0, 0.0);
        break;
	case 'k':
		if (!pmove)    cursor_translatem(&cursor4d, &invertedMv, 0.0, -5*tStep, 0.0);
		else           point_translateam(mesh[cm].boor, si, sj, &invertedMv, 0.0, -5*tStep, 0.0);
		break;
	case 'i':
		if (!pmove)    cursor_translatem(&cursor4d, &invertedMv, 0.0, 5*tStep, 0.0);
		else           point_translateam(mesh[cm].boor, si, sj, &invertedMv, 0.0, 5*tStep, 0.0);
		break;
	case 'u':
		if (!pmove)    cursor_translatem(&cursor4d, &invertedMv, 0.0, 0.0, -5*tStep);
		else           point_translateam(mesh[cm].boor, si, sj, &invertedMv, 0.0, 0.0, -5*tStep);
		break;
	case 'o':
		if (!pmove)    cursor_translatem(&cursor4d, &invertedMv, 0.0, 0.0, 5*tStep);
		else           point_translateam(mesh[cm].boor, si, sj, &invertedMv, 0.0, 0.0, 5*tStep);
		break;
	case 'g':
		world_translate(&translationMatrix, 0.0, -tStep, 0.0);
		break;
	case 't':
		world_translate(&translationMatrix, 0.0, tStep, 0.0);
		break;
	case 'f':
		world_translate(&translationMatrix, -tStep, 0.0, 0.0);
		break;
	case 'h':
		world_translate(&translationMatrix, tStep, 0.0, 0.0);
		break;
	case 'r':
		world_translate(&translationMatrix, 0.0, 0.0, -tStep);
		break;
	case 'y':
		world_translate(&translationMatrix, 0.0, 0.0, tStep);
		break;
	case 'e':
		clear_cursor(&cursor4d);
		setup_matrices();
		break;
	case 'z':
		angX -= (PI*0.05);
		if (angX<-2.0*PI) angX += 2.0*PI;
		world_rotate(&rotXMatrix, 0, angX);
		break;
	case 'x':
		angX += (PI*0.05);
		if (angX>2.0*PI) angX -= 2.0*PI;
		world_rotate(&rotXMatrix, 0, angX);
		break;
	case 'c':
		angY -= (PI*0.05);
		if (angY<-2.0*PI) angY += 2.0*PI;
		world_rotate(&rotYMatrix, 1, angY);
		break;
	case 'v':
		angY += (PI*0.05);
		if (angY>2.0*PI) angY -= 2.0*PI;
		world_rotate(&rotYMatrix, 1, angY);
		break;
	case 'b':
		angZ -= (PI*0.05);
		if (angZ<-2.0*PI) angZ += 2.0*PI;
		world_rotate(&rotZMatrix, 2, angZ);
		break;
	case 'n':
		angZ += (PI*0.05);
		if (angZ>2.0*PI) angZ -= 2.0*PI;
		world_rotate(&rotZMatrix, 2, angZ);
		break;
	case 'm':
		want_bez = ! want_bez;
		break;
	case '/':
		want_boo = ! want_boo;
		break;
   }
 update_cursorpos();
}


void resize_scene(int w, int h)
{
 wx = w;
 wy = h;
 construct_matrix(&shiftMatrix,1.0,0.0,0.0,0.0,
	 			           0.0,1.0,0.0,0.0,
	 			           0.0,0.0,1.0,0.0,
	 			           wx/2,wy/2,0.0,1.0);
 glViewport(0, 0, (GLsizei)wx, (GLsizei)wy);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glFrustum(-1., 1., -1., 1., 1.5, 20.);
 glMatrixMode(GL_MODELVIEW);
}


void recompute_world_matrix_R()
{
 int i=0;
 struct Matrix4D temp, temp2;
 struct Matrix4D mv;
 translationMatrix.v[0].p[3] += d*tStep;
 identity_matrix(&temp);
 for (i=0;i<6;i++)
   {
    if (i==5) copy_matrix(&mv, &temp);
    matrix_mul(&temp2, &temp, batchMatrix[i]);
    copy_matrix(&temp, &temp2);
   }
 copy_matrix(&worldMatrix, &temp);
 invert_matrix(&invertedMv, &mv);
 clear_translations(&invertedMv);
}


void recompute_world_matrix_L()
{
 int i=0;
 struct Matrix4D temp, temp2;
 struct Matrix4D mv;
 translationMatrix.v[0].p[3] -= (d*tStep)/2.0;
 identity_matrix(&temp);
 for (i=0;i<6;i++)
   {
    if (i==5) copy_matrix(&mv, &temp);
    matrix_mul(&temp2, &temp, batchMatrix[i]);
    copy_matrix(&temp, &temp2);
   }
 copy_matrix(&worldMatrix, &temp);
 invert_matrix(&invertedMv, &mv);
 clear_translations(&invertedMv);
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


void render_scene(void)
{
 GLdouble scale;
 int i,j,m;
 int ii,jj;
 struct Line4D line;
 time_counter();
 scale = (GLdouble)wx + (GLdouble)wy;
 scale = 15. / scale;
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_BLEND);
 glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
 glLoadIdentity();
 gluLookAt(0., 0., 5., 0. , 0., 0., 0., 1., 0.);
 glScaled(scale, scale, scale);
 glTranslated(-(GLdouble)(wx/2), - (GLdouble)(wy/2), 0.);
 recompute_world_matrix_L();
 curr_eye = 0;
 glColor3d(1.,0.,0.);
 render_cursor(&cursor4d);
 for (m=0;m<NMESH;m++)
 {
  get_point_nearesta(mesh[m].boor, mesh[m].nBoorX, mesh[m].nBoorY,  &cursor4d, &ii, &jj);
  if (want_boo)
  {
   for (i=0;i<mesh[m].nBoorX;i++)
   for (j=0;j<mesh[m].nBoorY;j++)
    {
     render_point(&mesh[m].boor[i][j]);
     line_cursor_to_pointv(&line, &cursor4d, &mesh[m].boor[i][j]);
     if (m == cm && i == ii && j == jj)
      {
       glColor3d((GLdouble)curr_eye*0.75, (GLdouble)curr_eye*0.75, 1.);
       render_line(&line);
       glColor3d(1.,0.,0.);
      }
     if (m == cm && i == si && j == sj)
      {
       glColor3d((GLdouble)curr_eye*0.75, (GLdouble)curr_eye*0.75, 1.);
       render_line(&line);
       glColor3d(1.,0.,0.);
      }
   }
 }
 if (want_bez)
   {
    for (i=3;i<mesh[m].nBezX-3;i++)
    for (j=3;j<mesh[m].nBezY-3;j++) render_point_bez(&mesh[m].bez[i][j]);
   }
 }
 render_nurbs();
 recompute_world_matrix_R();
 curr_eye = 1;
 glColor3d(0.,1.,0.);
 recompute_world_matrix_L();
 render_cursor(&cursor4d);
 for (m=0;m<NMESH;m++)
 {
  get_point_nearesta(mesh[m].boor, mesh[m].nBoorX, mesh[m].nBoorY,  &cursor4d, &ii, &jj);
  if (want_boo)
  {
   for (i=0;i<mesh[m].nBoorX;i++)
   for (j=0;j<mesh[m].nBoorY;j++)
    {
     render_point(&mesh[m].boor[i][j]);
     line_cursor_to_pointv(&line, &cursor4d, &mesh[m].boor[i][j]);
     if (m == cm && i == ii && j == jj)
      {
       glColor3d((GLdouble)curr_eye*0.75, (GLdouble)curr_eye*0.75, 1.);
       render_line(&line);
       glColor3d(0.,1.,0.);
      }
     if (m == cm && i == si && j == sj)
      {
       glColor3d((GLdouble)curr_eye*0.75, (GLdouble)curr_eye*0.75, 1.);
       render_line(&line);
       glColor3d(0.,1.,0.);
      }
   }
 }
 if (want_bez)
   {
    for (i=3;i<mesh[m].nBezX-3;i++)
    for (j=3;j<mesh[m].nBezY-3;j++) render_point_bez(&mesh[m].bez[i][j]);
   }
 }
 render_nurbs();
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

void bz_see(struct Vector4D*** b)
{
 int i,j;
 for (i=3;i<7;i++)
  {
   for (j=3;j<7;j++) printf("(%2.0f,%2.0f,%2.0f) ", (*b)[i][j].p[0], (*b)[i][j].p[1], (*b)[i][j].p[2]);
   printf("\n");
  }
}

double func_b_orig(double sc, int obj, int cord, int I, int J)
{
 double s;
 s = -405.*sc;
 if (obj == 0)
   {
    if (cord == 0) return s+sc*(90.*(double)I);
    if (cord == 1) return s+sc*(90.*(double)J);
    if (cord == 2) return sc*(30.*sin(.01*(s+sc*(90.*(double)I)))+30.*sin(.01*(s+sc*(90.*(double)J))));
   }
 if (obj == 1)
   {
    if (cord == 0) return s+sc*(90.*(double)I);
    if (cord == 2) return s+sc*(90.*(double)J);
    if (cord == 1) return sc*(30.*sin(.01*(s+sc*(90.*(double)I)))+30.*sin(.01*(s+sc*(90.*(double)J))));
   }
 return -1.;
}

double func_b(double sc, int obj, int cord, int I, int J)
{
 if (obj == 0)
   {
    if (cord == 0) 
      {
       if (I == 1 || I == 5 || I == 9) return sc*(2.+(double)J/7.);
       if (I == 0 || I == 4 || I == 8) return sc*(-2.-(double)J/7.);
       if (I == 2 || I == 6) return sc;
       if (I == 3 || I == 7) return -sc;
      }
    if (cord == 1)
      {
       if (I == 1 || I == 5 || I == 9) return -sc*(2.-(double)J/10);
       if (I == 0 || I == 4 || I == 8) return -sc*(2.-(double)J/10);
       if (I == 2 || I == 6) return sc*((double)J/5);
       if (I == 3 || I == 7) return sc*((double)J/5);
      }
    if (cord == 2) return sc*(double)((J-3));
   }
 if (obj == 1)
   {
    if (cord == 0) 
      {
/*       if ((I == 0  || I == 3 || I == 6 || I == 9) && (J == 9 || J == 8 || J ==7 || J == 6)) return 0.;*/
       if (I == 1 || I == 5 || I == 9) return sc*2.;
       if (I == 0 || I == 4 || I == 8) return sc*-2.;
       if (I == 2 || I == 6) return 0.;
       if (I == 3 || I == 7) return 0.;
      }
    if (cord == 2)
      {
/*       if ((I == 0  || I == 3 || I == 6 || I == 9) && (J == 9 || J == 8 || J ==7 || J == 6)) return 0.;*/
       if (I == 1 || I == 5 || I == 9) return sc*2.;
       if (I == 0 || I == 4 || I == 8) return sc*2.;
       if (I == 2 || I == 6) return 0.;
       if (I == 3 || I == 7) return 0.;
      }
    if (cord == 1) return -sc*(double)(J);
   }
 return -1.;
}

void suszarka5()
{
 int i,j;
 int nx,ny;
 double startX, startY;
 double scl;
 
 mesh[0].nBoorX = 10;
 mesh[0].nBoorY = 10;
 mesh[0].nBezX = 28;
 mesh[0].nBezY = 28;
 mesh[0].nX = 7;
 mesh[0].nY = 7;
 mesh[0].nNurbsX = 0;
 mesh[0].nNurbsY = 0;
 
 scl = 90.;
 mesh[0].boor = (struct Vector4D**)malloc(10*sizeof(struct Vector4D*));
 for (i=0;i<10;i++) mesh[0].boor[i] = (struct Vector4D*)malloc(10*sizeof(struct Vector4D));
 mesh[0].nurbs = NULL;
 
 startX = scl*-405.;
 startY = scl*-405.;
 for (i=0;i<10;i++)
 for (j=0;j<10;j++)
     {
      mesh[0].boor[i][j].p[0] = func_b(scl, 0, 0, i, j);
      mesh[0].boor[i][j].p[1] = func_b(scl, 0, 1, i, j);
      mesh[0].boor[i][j].p[2] = func_b(scl, 0, 2, i, j);
/*      mesh[0].boor[i][j].p[0] = scl*(startX + 90.*(double)i);
      mesh[0].boor[i][j].p[1] = scl*(startY + 90.*(double)j);
      mesh[0].boor[i][j].p[2] = scl*(3. + 30.*sin(.01*(mesh[0].boor[i][j].p[0])) + 30.*sin(.01*(mesh[0].boor[i][j].p[1])));*/
     }
 bezier_from_deboor_2da(&mesh[0].bez, mesh[0].boor, &nx, &ny, 10, 10);

 mesh[1].nBoorX = 10;
 mesh[1].nBoorY = 10;
 mesh[1].nBezX = 28;
 mesh[1].nBezY = 28;
 mesh[1].nX = 7;
 mesh[1].nY = 7;
 mesh[1].nNurbsX = 0;
 mesh[1].nNurbsY = 0;
 
 scl = 70.;
 mesh[1].boor = (struct Vector4D**)malloc(10*sizeof(struct Vector4D*));
 for (i=0;i<10;i++) mesh[1].boor[i] = (struct Vector4D*)malloc(10*sizeof(struct Vector4D));
 mesh[1].nurbs = NULL;
 
 startX = scl*-405.;
 startY = scl*-405.;
 for (i=0;i<10;i++)
 for (j=0;j<10;j++)
     {
/*      mesh[1].boor[i][j].p[0] = scl*(startX + 90.*(double)i);
      mesh[1].boor[i][j].p[2] = scl*(startY + 90.*(double)j);
      mesh[1].boor[i][j].p[1] = scl*(3. + 30.*sin(.01*(mesh[1].boor[i][j].p[0])) + 30.*sin(.01*(mesh[1].boor[i][j].p[2])));*/
      mesh[1].boor[i][j].p[0] = func_b(scl, 1, 0, i, j);
      mesh[1].boor[i][j].p[1] = func_b(scl, 1, 1, i, j);
      mesh[1].boor[i][j].p[2] = func_b(scl, 1, 2, i, j);
     }
 bezier_from_deboor_2da(&mesh[1].bez, mesh[1].boor, &nx, &ny, 10, 10);
}


void suszarka4()
{
 int i,j;
 int nx,ny;
 double startX, startY;
 double scl;
 
 mesh[0].nBoorX = 6;
 mesh[0].nBoorY = 6;
 mesh[0].nBezX = 16;
 mesh[0].nBezY = 16;
 mesh[0].nX = 3;
 mesh[0].nY = 3;
 mesh[0].nNurbsX = 0;
 mesh[0].nNurbsY = 0;
 
 scl = 1.;
 mesh[0].boor = (struct Vector4D**)malloc(6*sizeof(struct Vector4D*));
 for (i=0;i<6;i++) mesh[0].boor[i] = (struct Vector4D*)malloc(6*sizeof(struct Vector4D));
 mesh[0].nurbs = NULL;
 
 startX = scl*-225.;
 startY = scl*-225.;
 for (i=0;i<6;i++)
 for (j=0;j<6;j++)
     {
      mesh[0].boor[i][j].p[0] = scl*(startX + 90.*(double)i);
      mesh[0].boor[i][j].p[1] = scl*(startY + 90.*(double)j);
      mesh[0].boor[i][j].p[2] = scl*(3. + 30.*sin(.01*(mesh[0].boor[i][j].p[0])) + 30.*sin(.01*(mesh[0].boor[i][j].p[1])));
     }
 bezier_from_deboor_2da(&mesh[0].bez, mesh[0].boor, &nx, &ny, 6, 6);

 mesh[1].nBoorX = 6;
 mesh[1].nBoorY = 6;
 mesh[1].nBezX = 16;
 mesh[1].nBezY = 16;
 mesh[1].nX = 3;
 mesh[1].nY = 3;
 mesh[1].nNurbsX = 0;
 mesh[1].nNurbsY = 0;
 
 scl = 1.;
 mesh[1].boor = (struct Vector4D**)malloc(6*sizeof(struct Vector4D*));
 for (i=0;i<6;i++) mesh[1].boor[i] = (struct Vector4D*)malloc(6*sizeof(struct Vector4D));
 mesh[1].nurbs = NULL;
 
 startX = scl*-225.;
 startY = scl*-225.;
 for (i=0;i<6;i++)
 for (j=0;j<6;j++)
     {
      mesh[1].boor[i][j].p[0] = scl*(startX + 90.*(double)i);
      mesh[1].boor[i][j].p[2] = scl*(startY + 90.*(double)j);
      mesh[1].boor[i][j].p[1] = scl*(3. + 30.*sin(.01*(mesh[1].boor[i][j].p[0])) + 30.*sin(.01*(mesh[1].boor[i][j].p[2])));
     }
 bezier_from_deboor_2da(&mesh[1].bez, mesh[1].boor, &nx, &ny, 6, 6);
}

void suszarka()
{
 int i,j,m,n;
 int nx,ny;
 double startX, startY;
 double scl;
 struct Vector4D **bz, **boo;
 struct Vector4D ***control_points;
 mesh[0].nBoorX = 6;
 mesh[0].nBoorY = 6;
 mesh[0].nBezX = 16;
 mesh[0].nBezY = 16;
 mesh[0].nX = 3;
 mesh[0].nY = 3;
 mesh[0].nNurbsX = 3;
 mesh[0].nNurbsY = 3;
 scl = 1.;
 boo = (struct Vector4D**)malloc(4*sizeof(struct Vector4D*));
 for (i=0;i<4;i++) boo[i] = (struct Vector4D*)malloc(4*sizeof(struct Vector4D));
 mesh[0].nurbs = (struct Nurb***)malloc(mesh[0].nNurbsX*sizeof(struct Nurb**));
 for (i=0;i<mesh[0].nNurbsX;i++) mesh[0].nurbs[i] = (struct Nurb**)malloc(mesh[0].nNurbsY*sizeof(struct Nurb*));
 for (i=0;i<mesh[0].nNurbsX;i++)
 for (j=0;j<mesh[0].nNurbsY;j++) mesh[0].nurbs[i][j] = (struct Nurb*)malloc(sizeof(struct Nurb));
 for (m=0;m<3;m++)
 for (n=0;n<3;n++)
  {
   startX = scl*(90.*(double)m-225.);
   startY = scl*(90.*(double)n-225.);
   for (i=0;i<4;i++)
   for (j=0;j<4;j++)
     {
      boo[i][j].p[0] = scl*(startX + 90.*(double)i);
      boo[i][j].p[1] = scl*(startY + 90.*(double)j);
/*      boo[i][j].p[2] = 30.*sin(.01*(boo[i][j].p[0]+boo[i][j].p[1]));*/
      boo[i][j].p[2] = scl*(3. + 30.*sin(.01*(boo[i][j].p[0])) + 30.*sin(.01*(boo[i][j].p[1])));
/*      boo[i][j].p[2] = 0.;*/
     }
   bezier_from_deboor_2da(&bz, boo, &nx, &ny, 4, 4);
   printf("bez(%d,%d)\n", m,n);
   bz_see(&bz);
   control_points = NULL;
   get_C2a_nm(bz, nx, ny, &control_points, 1, 1);
   mesh[0].nNurbsX = mesh[0].nX;
   mesh[0].nNurbsY = mesh[0].nY;
   nurb_set(mesh[0].nurbs[m][n], 3, 3, 3, 3, lpsegm, lpsegm, control_points[0][0]);
   control_points = NULL;
  }
 mesh[1].nBoorX = 6;
 mesh[1].nBoorY = 6;
 mesh[1].nBezX = 16;
 mesh[1].nBezY = 16;
 mesh[1].nX = 3;
 mesh[1].nY = 3;
 mesh[1].nNurbsX = 3;
 mesh[1].nNurbsY = 3;
 
 /*boo = (struct Vector4D**)malloc(4*sizeof(struct Vector4D*));
 for (i=0;i<10;i++) boo[i] = (struct Vector4D*)malloc(4*sizeof(struct Vector4D));*/
 
 mesh[1].nurbs = (struct Nurb***)malloc(mesh[1].nNurbsX*sizeof(struct Nurb**));
 for (i=0;i<mesh[1].nNurbsX;i++) mesh[1].nurbs[i] = (struct Nurb**)malloc(mesh[1].nNurbsY*sizeof(struct Nurb*));
 for (i=0;i<mesh[1].nNurbsX;i++)
 for (j=0;j<mesh[1].nNurbsY;j++) mesh[1].nurbs[i][j] = (struct Nurb*)malloc(sizeof(struct Nurb));
 for (m=0;m<3;m++)
 for (n=0;n<3;n++)
  {
   startX = scl*(90.*(double)m-225.);
   startY = scl*(90.*(double)n-225.);
   for (i=0;i<4;i++)
   for (j=0;j<4;j++)
     {
      boo[i][j].p[0] = scl*(startX + 90.*(double)i);
      boo[i][j].p[2] = scl*(startY + 90.*(double)j);
/*      boo[i][j].p[1] = 30.*sin(.01*(boo[i][j].p[0]+boo[i][j].p[2]));*/
/*      boo[i][j].p[1] = 30.*sin(.01*(boo[i][j].p[0]));*/
      boo[i][j].p[1] = scl*(3. + 30.*sin(.01*(boo[i][j].p[0])) + 30.*sin(.01*(boo[i][j].p[2])));
/*      boo[i][j].p[1] = 0.;*/
     }
   bezier_from_deboor_2da(&bz, boo, &nx, &ny, 4, 4);
   printf("bez(%d,%d)\n", m,n);
   bz_see(&bz);
 printf("done1!\n");
   control_points = NULL;
   get_C2a_nm(bz, nx, ny, &control_points, 1, 1);
 printf("done2! %p\n", (void*)mesh[1].nurbs[m][n]);
   nurb_set(mesh[1].nurbs[m][n], 3, 3, 3, 3, lpsegm, lpsegm, control_points[0][0]);
 printf("done3!\n");
   control_points = NULL;
  }
 printf("done4!\n");
}

void suszarka2()
{
 int i,j,m,n;
 int nx,ny;
 FILE* plik;
 struct Vector4D **bz, **boo;
 struct Vector4D ***control_points;
 plik = fopen("punkty", "r");
 mesh[0].nBoorX = 6;
 mesh[0].nBoorY = 6;
 mesh[0].nBezX = 16;
 mesh[0].nBezY = 16;
 mesh[0].nX = 3;
 mesh[0].nY = 3;
 mesh[0].nNurbsX = 3;
 mesh[0].nNurbsY = 3;
 /*mesh[0].boor = (struct Vector4D**)malloc(mesh[0].nBoorX*sizeof(struct Vector4D*));
 for (i=0;i<mesh[0].nBoorX;i++) mesh[0].boor[i] = (struct Vector4D*)malloc(mesh[0].nBoorY*sizeof(struct Vector4D));*/
 /*bz = (struct Vector4D**)malloc(10*sizeof(struct Vector4D*));
 for (i=0;i<10;i++) bz[i] = (struct Vector4D*)malloc(10*sizeof(struct Vector4D));*/
 boo = (struct Vector4D**)malloc(4*sizeof(struct Vector4D*));
 for (i=0;i<10;i++) boo[i] = (struct Vector4D*)malloc(4*sizeof(struct Vector4D));
 fscanf(plik,"punkty:\n");
   mesh[0].nurbs = (struct Nurb***)malloc(mesh[0].nNurbsX*sizeof(struct Nurb**));
   for (i=0;i<mesh[0].nNurbsX;i++) mesh[0].nurbs[i] = (struct Nurb**)malloc(mesh[0].nNurbsY*sizeof(struct Nurb*));
   for (i=0;i<mesh[0].nNurbsX;i++)
   for (j=0;j<mesh[0].nNurbsY;j++) mesh[0].nurbs[i][j] = (struct Nurb*)malloc(sizeof(struct Nurb));
 for (m=0;m<3;m++)
 for (n=0;n<3;n++)
  {
   for (i=0;i<4;i++)
   for (j=0;j<4;j++)
     {
      fscanf(plik, "(%lf,%lf,%lf)\n", &boo[i][j].p[0], &boo[i][j].p[1], &boo[i][j].p[2]);
     }
   bezier_from_deboor_2da(&bz, boo, &nx, &ny, 4, 4);
   printf("bez(%d,%d)\n", m,n);
   bz_see(&bz);
   for (i=0;i<10;i++)
   {
   for (j=0;j<10;j++)
   {
    if (bz[i][j].p[0] == 0.) printf("0");
    else printf("X");
   }
   printf("\n");
   }
   printf("nx = %d, ny = %d\n", nx, ny);
   control_points = NULL;
   get_C2a_nm(bz, nx, ny, &control_points, 1, 1);
   printf("control_points: %p\n", (void*)control_points);
   printf("control_points: %p\n", (void*)control_points[0][0]);
   mesh[0].nNurbsX = mesh[0].nX;
   mesh[0].nNurbsY = mesh[0].nY;
   nurb_set(mesh[0].nurbs[m][n], 3, 3, 3, 3, lpsegm, lpsegm, control_points[0][0]);
   control_points = NULL;
  }
 mesh[1].nBoorX = 6;
 mesh[1].nBoorY = 6;
 mesh[1].nBezX = 16;
 mesh[1].nBezY = 16;
 mesh[1].nX = 3;
 mesh[1].nY = 3;
 /*mesh[0].boor = (struct Vector4D**)malloc(mesh[0].nBoorX*sizeof(struct Vector4D*));
 for (i=0;i<mesh[0].nBoorX;i++) mesh[0].boor[i] = (struct Vector4D*)malloc(mesh[0].nBoorY*sizeof(struct Vector4D));*/
 fscanf(plik,"punkty:\n");
   mesh[1].nNurbsX = mesh[1].nX;
   mesh[1].nNurbsY = mesh[1].nY;
   mesh[1].nurbs = (struct Nurb***)malloc(mesh[1].nNurbsX*sizeof(struct Nurb**));
   for (i=0;i<mesh[1].nNurbsX;i++) mesh[1].nurbs[i] = (struct Nurb**)malloc(mesh[1].nNurbsY*sizeof(struct Nurb*));
   for (i=0;i<mesh[1].nNurbsX;i++)
   for (j=0;j<mesh[1].nNurbsY;j++) mesh[1].nurbs[i][j] = (struct Nurb*)malloc(sizeof(struct Nurb));
 for (m=0;m<3;m++)
 for (n=0;n<3;n++)
  {
   for (i=0;i<4;i++)
   for (j=0;j<4;j++)
     {
      fscanf(plik, "(%lf,%lf,%lf)\n", &boo[i][j].p[0], &boo[i][j].p[1], &boo[i][j].p[2]);
     }
   bezier_from_deboor_2da(&bz, boo, &nx, &ny, 4, 4);
   printf("bez(%d,%d)\n", m,n);
   bz_see(&bz);
   control_points = NULL;
   get_C2a_nm(bz, nx, ny, &control_points, 1, 1);
   nurb_set(mesh[1].nurbs[m][n], 3, 3, 3, 3, lpsegm, lpsegm, control_points[0][0]);
   control_points = NULL;
  }
 fclose(plik);
}

void suszarka3()
{
 int i;
 double scale, scalel;
 double dx,dy,dz;
 double bscale;
 bscale = 100.;
 mesh[0].nBoorX = 6;
 mesh[0].nBoorY = 9;
 mesh[0].nX = 3;
 mesh[0].nY = 6;
 mesh[0].boor = (struct Vector4D**)malloc(mesh[0].nBoorX*sizeof(struct Vector4D*));
 for (i=0;i<mesh[0].nBoorX;i++) mesh[0].boor[i] = (struct Vector4D*)malloc(mesh[0].nBoorY*sizeof(struct Vector4D));
 scale  = 1.4*bscale;
 scalel = 3.*bscale;
 construct_vector3(&mesh[0].boor[0][0], 0.,     2.*scalel, -scale);
 construct_vector3(&mesh[0].boor[0][1], -scale, 2.*scalel, 0.);
 construct_vector3(&mesh[0].boor[0][2], 0.    , 2.*scalel, scale);
 construct_vector3(&mesh[0].boor[0][3], scale,  2.*scalel, 0.);
 construct_vector3(&mesh[0].boor[0][4], 0.   ,  2.*scalel, -scale);
 construct_vector3(&mesh[0].boor[0][5], -scale, 2.*scalel, 0.);
 construct_vector3(&mesh[0].boor[0][6], 0.    , 2.*scalel, scale);
 construct_vector3(&mesh[0].boor[0][7], scale,  2.*scalel, 0.);
 construct_vector3(&mesh[0].boor[0][8], 0.,     2.*scalel, -scale);
 construct_vector3(&mesh[0].boor[1][0], 0.,     1.5*scalel, -scale);
 construct_vector3(&mesh[0].boor[1][1], -scale, 1.5*scalel, 0.);
 construct_vector3(&mesh[0].boor[1][2], 0.    , 1.5*scalel, scale);
 construct_vector3(&mesh[0].boor[1][3], scale,  1.5*scalel, 0.);
 construct_vector3(&mesh[0].boor[1][4], 0.   ,  1.5*scalel, -scale);
 construct_vector3(&mesh[0].boor[1][5], -scale, 1.5*scalel, 0.);
 construct_vector3(&mesh[0].boor[1][6], 0.    , 1.5*scalel, scale);
 construct_vector3(&mesh[0].boor[1][7], scale,  1.5*scalel, 0.);
 construct_vector3(&mesh[0].boor[1][8], 0.,     1.5*scalel, -scale);
 construct_vector3(&mesh[0].boor[2][0], 0.,     0.*scalel, -scale);
 construct_vector3(&mesh[0].boor[2][1], -scale, 0.*scalel, 0.);
 construct_vector3(&mesh[0].boor[2][2], 0.    , 0.*scalel, scale);
 construct_vector3(&mesh[0].boor[2][3], scale,  0.*scalel, 0.);
 construct_vector3(&mesh[0].boor[2][4], 0.   ,  0.*scalel, -scale);
 construct_vector3(&mesh[0].boor[2][5], -scale, 0.*scalel, 0.);
 construct_vector3(&mesh[0].boor[2][6], 0.    , 0.*scalel, scale);
 construct_vector3(&mesh[0].boor[2][7], scale,  0.*scalel, 0.);
 construct_vector3(&mesh[0].boor[2][8], 0.,     0.*scalel, -scale);
 construct_vector3(&mesh[0].boor[3][0], -scale, -1.*scalel, 0.);
 construct_vector3(&mesh[0].boor[3][1], 0.    , -1.*scalel, scale);
 construct_vector3(&mesh[0].boor[3][2], scale,  -1.*scalel, 0.);
 construct_vector3(&mesh[0].boor[3][3], 0.,     -1.*scalel, -1.2*scale);
 construct_vector3(&mesh[0].boor[3][4], 0.,     -1.*scalel, -1.2*scale);
 construct_vector3(&mesh[0].boor[3][5], 0.    , -1.*scalel, -1.2*scale);
 construct_vector3(&mesh[0].boor[3][6], -scale, -1.*scalel, 0.);
 construct_vector3(&mesh[0].boor[3][7], 0.    , -1.*scalel, scale);
 construct_vector3(&mesh[0].boor[3][8], scale,  -1.*scalel, 0.);
 construct_vector3(&mesh[0].boor[4][0], -1.4*scale, -2.*scalel, 0.);
 construct_vector3(&mesh[0].boor[4][1], 0.    ,     -2.*scalel, 1.2*scale);
 construct_vector3(&mesh[0].boor[4][2], 1.4*scale,  -2.*scalel, 0.);
 construct_vector3(&mesh[0].boor[4][3], 0.   ,      -2.*scalel, -1.5*scale);
 construct_vector3(&mesh[0].boor[4][4], 0.   ,      -2.*scalel, -1.5*scale);
 construct_vector3(&mesh[0].boor[4][5], 0.    ,     -2.*scalel, -1.5*scale);
 construct_vector3(&mesh[0].boor[4][6], -1.4*scale, -2.*scalel, 0.);
 construct_vector3(&mesh[0].boor[4][7], 0.    ,     -2.*scalel, 1.2*scale);
 construct_vector3(&mesh[0].boor[4][8], 1.4*scale,  -2.*scalel, 0.);
 construct_vector3(&mesh[0].boor[5][0], -.6*scale, -2.5*scalel, 0.);
 construct_vector3(&mesh[0].boor[5][1], 0.    ,    -2.5*scalel, .5*scale);
 construct_vector3(&mesh[0].boor[5][2], .6*scale,  -2.5*scalel, 0.);
 construct_vector3(&mesh[0].boor[5][3], 0.,        -2.5*scalel, -.5*scale);
 construct_vector3(&mesh[0].boor[5][4], 0.,        -2.5*scalel, -.5*scale);
 construct_vector3(&mesh[0].boor[5][5], 0.    ,    -2.5*scalel, -.5*scale);
 construct_vector3(&mesh[0].boor[5][6], -.6*scale, -2.5*scalel, 0.);
 construct_vector3(&mesh[0].boor[5][7], 0.    ,    -2.5*scalel, .5*scale);
 construct_vector3(&mesh[0].boor[5][8], .6*scale,  -2.5*scalel, 0.);
 bezier_from_deboor_2da(&mesh[0].bez, mesh[0].boor, &mesh[0].nBezX, &mesh[0].nBezY, mesh[0].nBoorX, mesh[0].nBoorY);
 mesh[1].nBoorX = 6;
 mesh[1].nBoorY = 7;
 mesh[1].nX = 3;
 mesh[1].nY = 4;
 mesh[1].boor = (struct Vector4D**)malloc(mesh[1].nBoorX*sizeof(struct Vector4D*));
 for (i=0;i<mesh[1].nBoorX;i++) mesh[1].boor[i] = (struct Vector4D*)malloc(mesh[1].nBoorY*sizeof(struct Vector4D));
 scale  = 2.2*bscale;
 scalel = 6.*bscale;
 dx = 0.;
 dy = 4.*bscale;
 dz = 0.;
 construct_vector3(&mesh[1].boor[0][0], .8*dx       , .5*-scale+dy, -3.*scalel+dz);
 construct_vector3(&mesh[1].boor[0][1], .8*-scale+dx, .5*dy       , -3.*scalel+dz);
 construct_vector3(&mesh[1].boor[0][2], .8*dx       , .5*scale+dy , -3.*scalel+dz);
 construct_vector3(&mesh[1].boor[0][3], .8*scale+dx , .5*dy       , -3.*scalel+dz);
 construct_vector3(&mesh[1].boor[0][4], .8*dx       , .5*-scale+dy, -3.*scalel+dz);
 construct_vector3(&mesh[1].boor[0][5], .8*-scale+dx, .5*dy       , -3.*scalel+dz);
 construct_vector3(&mesh[1].boor[0][6], .8*dx       , .5*scale+dy , -3.*scalel+dz);
 construct_vector3(&mesh[1].boor[1][0], dx       , .5*-scale+dy, -2.5*scalel+dz);
 construct_vector3(&mesh[1].boor[1][1], -scale+dx, .5*dy    ,    -2.5*scalel+dz);
 construct_vector3(&mesh[1].boor[1][2], dx       , .5*scale+dy , -2.5*scalel+dz);
 construct_vector3(&mesh[1].boor[1][3], scale+dx , .5*dy    ,    -2.5*scalel+dz);
 construct_vector3(&mesh[1].boor[1][4], dx       , .5*-scale+dy, -2.5*scalel+dz);
 construct_vector3(&mesh[1].boor[1][5], -scale+dx, .5*dy    ,    -2.5*scalel+dz);
 construct_vector3(&mesh[1].boor[1][6], dx       , .5*scale+dy , -2.5*scalel+dz);
 construct_vector3(&mesh[1].boor[2][0], .8*dx       , -scale+dy, -1.*scalel+dz);
 construct_vector3(&mesh[1].boor[2][1], .8*-scale+dx, dy    ,    -1.*scalel+dz);
 construct_vector3(&mesh[1].boor[2][2], .8*dx       , 1.5*scale+dy , -1.*scalel+dz);
 construct_vector3(&mesh[1].boor[2][3], .8*scale+dx , dy    ,    -1.*scalel+dz);
 construct_vector3(&mesh[1].boor[2][4], .8*dx       , -scale+dy, -1.*scalel+dz);
 construct_vector3(&mesh[1].boor[2][5], .8*-scale+dx, dy    ,    -1.*scalel+dz);
 construct_vector3(&mesh[1].boor[2][6], .8*dx       , 1.5*scale+dy , -1.*scalel+dz);
 construct_vector3(&mesh[1].boor[3][0], 1.2*dx       , 2.*-scale+dy, 0.*scalel+dz);
 construct_vector3(&mesh[1].boor[3][1], 1.2*-scale+dx, dy    ,    0.*scalel+dz);
 construct_vector3(&mesh[1].boor[3][2], 1.2*dx       , scale+dy , 0.*scalel+dz);
 construct_vector3(&mesh[1].boor[3][3], 1.2*scale+dx , dy    ,    0.*scalel+dz);
 construct_vector3(&mesh[1].boor[3][4], 1.2*dx       , 2.*-scale+dy, 0.*scalel+dz);
 construct_vector3(&mesh[1].boor[3][5], 1.2*-scale+dx, dy    ,    0.*scalel+dz);
 construct_vector3(&mesh[1].boor[3][6], 1.2*dx       , scale+dy , 0.*scalel+dz);
 construct_vector3(&mesh[1].boor[4][0], .5*dx       , .3*-scale+dy, 1.*scalel+dz);
 construct_vector3(&mesh[1].boor[4][1], .3*-scale+dx, .3*dy    ,    1.*scalel+dz);
 construct_vector3(&mesh[1].boor[4][2], .5*dx       , .5*scale+dy , 1.*scalel+dz);
 construct_vector3(&mesh[1].boor[4][3], .3*scale+dx , .3*dy    ,    1.*scalel+dz);
 construct_vector3(&mesh[1].boor[4][4], .5*dx       , .5*-scale+dy, 1.*scalel+dz);
 construct_vector3(&mesh[1].boor[4][5], .3*-scale+dx, .3*dy    ,    1.*scalel+dz);
 construct_vector3(&mesh[1].boor[4][6], .5*dx       , .3*scale+dy , 1.*scalel+dz);
 construct_vector3(&mesh[1].boor[5][0], .2*dx       , .4*-scale+dy, 1.5*scalel+dz);
 construct_vector3(&mesh[1].boor[5][1], .2*-scale+dx, .4*dy    ,    1.5*scalel+dz);
 construct_vector3(&mesh[1].boor[5][2], .2*dx       , .4*scale+dy , 1.5*scalel+dz);
 construct_vector3(&mesh[1].boor[5][3], .2*scale+dx , .4*dy    ,    1.5*scalel+dz);
 construct_vector3(&mesh[1].boor[5][4], .2*dx       , .4*-scale+dy, 1.5*scalel+dz);
 construct_vector3(&mesh[1].boor[5][5], .2*-scale+dx, .4*dy    ,    1.5*scalel+dz);
 construct_vector3(&mesh[1].boor[5][6], .2*dx       , .4*scale+dy , 1.5*scalel+dz);
 bezier_from_deboor_2da(&mesh[1].bez, mesh[1].boor, &mesh[1].nBezX, &mesh[1].nBezY, mesh[1].nBoorX, mesh[1].nBoorY);
 /*int k,j;
 for (i=0;i<NMESH;i++)
   { 
    for (j=0;j<mesh[i].nBezX;j++)
    for (k=0;k<mesh[i].nBezY;k++)
           {
	    printf("mesh%d(%d,%d) -> ",i,j,k);
	    printf_vector("v", &mesh[i].bez[j][k]);
	   }
   }*/
/* exit(1);*/
 tmesh.bscale = bscale;
}

void gen_trimm_array(struct TrimmedMesh* tm)
{
 int i,j,idx;
 struct VList* vl;
 struct Vector4D tmp;
 double min;
 vl = tm->tcuv;
 i = 0;
 while (vl) { i++; vl = vl->next; }
/* printf("Num_elems: %d\n", i);*/
 arrU = (struct Vector4D*)malloc(i*sizeof(struct Vector4D));
 vl = tm->tcuv;
 i = 0;
 while (vl) { copy_vector(&arrU[i], (struct Vector*)vl->data); i++; vl = vl->next; }
 arrV = (struct Vector4D*)malloc(i*sizeof(struct Vector4D));
 vl = tm->tcuv;
 i = 0;
 while (vl) { copy_vector(&arrV[i], (struct Vector*)vl->data); i++; vl = vl->next; }
 arrS = (struct Vector4D*)malloc(i*sizeof(struct Vector4D));
 vl = tm->tcuv;
 i = 0;
 while (vl) { copy_vector(&arrS[i], (struct Vector*)vl->data); i++; vl = vl->next; }
 arrT = (struct Vector4D*)malloc(i*sizeof(struct Vector4D));
 vl = tm->tcuv;
 i = 0;
 while (vl) { copy_vector(&arrT[i], (struct Vector*)vl->data); i++; vl = vl->next; }
 nel = i;
 for (i=0;i<nel;i++) 
  {
   min = arrU[i].p[0];
   idx = i;
   for (j=i;j<nel;j++) { if (arrU[j].p[0] < arrU[idx].p[0]) idx = j; }
   copy_vector(&tmp, &arrU[idx]);
   copy_vector(&arrU[idx], &arrU[i]);
   copy_vector(&arrU[i], &tmp);
  }
 for (i=0;i<nel;i++) 
  {
   min = arrV[i].p[1];
   idx = i;
   for (j=i;j<nel;j++) { if (arrV[j].p[1] < arrV[idx].p[1]) idx = j; }
   copy_vector(&tmp, &arrV[idx]);
   copy_vector(&arrV[idx], &arrV[i]);
   copy_vector(&arrV[i], &tmp);
  }
 for (i=0;i<nel;i++) 
  {
   min = arrS[i].p[2];
   idx = i;
   for (j=i;j<nel;j++) { if (arrS[j].p[2] < arrS[idx].p[2]) idx = j; }
   copy_vector(&tmp, &arrS[idx]);
   copy_vector(&arrS[idx], &arrS[i]);
   copy_vector(&arrS[i], &tmp);
  }
 for (i=0;i<nel;i++) 
  {
   min = arrT[i].p[3];
   idx = i;
   for (j=i;j<nel;j++) { if (arrT[j].p[3] < arrT[idx].p[3]) idx = j; }
   copy_vector(&tmp, &arrT[idx]);
   copy_vector(&arrT[idx], &arrT[i]);
   copy_vector(&arrT[i], &tmp);
  }
/* for (i=0;i<nel;i++) printf_vector("sorted?", &arrT[i]);*/
}


void NurbsInit()
{
 time_t tm;
 int m;
 for (m=0;m<NMESH;m++)
   {
    mesh[m].nNurbsX = mesh[m].nNurbsY = 0;
    mesh[m].nurbs = NULL;
    mesh[m].bez = mesh[m].boor = NULL;
   }
 cm = 0;
 si = sj = -1;
 pmove = 0;
 want_bez = 0;
 want_boo = 0;
 want_lines= 2;
 want_in1 = want_in2 = 0;
 t1 = t2 = (time_t)0;
 fps = 0;
 lpsegm = 6;
 time(&tm);
 srand((int)tm);
 setup_matrices();
 setup_batch();
 help();
 construct_cursor_default(&cursor4d, 1.0);
 suszarka5();
 full_update_nurbs();
 tmesh.m1 = &mesh[0];
 tmesh.m2 = &mesh[1];
 if (trimm(&tmesh)) 
   {
    gen_trimm_matrices(&tmesh, lpsegm*24, lpsegm*24, lpsegm*8, lpsegm*8);
    gen_trimm_array(&tmesh);
   }
 glClearColor(0.,0.,0.,0.);
 glShadeModel(GL_SMOOTH);
}


int main(int lb, char** par)
{
 wx = 800;
 wy = 600;
 NurbsInit();
 printf("FINISHED TRIMMING\n");
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(10, 10);
 glutCreateWindow(par[0]);
 glutDisplayFunc(render_scene);
 glutReshapeFunc(resize_scene);
 glutKeyboardFunc(keyboard);
 glutVisibilityFunc(visible);
 glutMainLoop();
 return 0;
}

/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
