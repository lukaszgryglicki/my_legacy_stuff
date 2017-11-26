/* Written by MorgothDBMA, morgothdbma@o2.pl, tel: +48693582014 */
/* Lukasz Gryglicki MiNI M1 CC */
/* License BSD */
#include "lib4d.h"
#include <GL/glut.h>
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
float angX, angY, angZ, obsD, tStep;
struct Matrix4D** batchMatrix;
struct Cursor4D cursor4d;
struct Mesh4D mesh[NMESH];
time_t t1,t2;
int fps;
float d;
/* configurable */
float precision = .1;
float mill_d = 10.;
float offset_dy = 1.4;
float offset_dx = .2;
float param_step = .04;
float matx = 150.;
float maty = 150.;
float matz = 30.;
float matmz = 50.;
char fname[32];
/* ends */
int npath;
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

typedef struct _Block
{
float** b;
float step;
float bx, by, bz;
int nx, ny;
} Block;
Block block;
typedef struct _PList
{
struct _PList* next;
struct _PList* prev;
float x,y,z;
} PList;

typedef struct _Mill
{
char mtype;
float d,r;
int center;
float h;
int nx,ny;
float** map;
float x,y,z;
PList* path;
} Mill;
Mill mill;

void help()
{
 printf("usage ./scene4d [scenefile]\n");
 printf("controls: translations: f/h/t/g/r/y\n");
 printf("                cursor: j/l/i/k/u/o\n");
 printf("              rotating: z/x c/v b/n\n");
 printf("     return to normals: e\n");
 printf("             quit,help: q,0\n");
 printf("      toggle selection: p\n");
 printf("     toggle bez points: m\n");
 printf("     toggle boo points: /\n");
 printf("       toggle curr obj: ,\n");
 printf("           lines count: a/s\n");
 printf("toggle lin/pts/triangs: d\n");
 printf(" force NURBS recompute: SPACE\n");
}

void init_block(Block* b, float bx, float by, float bz, float istep)
{
int i,j;
GLfloat step;
b->bx = bx;
b->by = by;
b->bz = bz;
b->step = istep;
b->nx = b->bx / b->step;
b->ny = b->by / b->step;
step = b->step;
b->b = (float**)malloc(b->nx*sizeof(float*));
for (i=0;i<b->nx;i++) b->b[i] = (float*)malloc(b->ny*sizeof(float));
for (i=0;i<b->nx;i++)
for (j=0;j<b->ny;j++) b->b[i][j] = b->bz;
printf("Block created (%dx%d).\n",b->nx,b->ny);
}

void init_mill(Mill* mil, float md, float mh)
{
 int i,j;
 float dI, dJ, dist;
if (!mil) exit(3);
mil->mtype = 'k';
mil->d = md;
mil->r = mil->d/2.;
mil->center = 1;
if (mh > 0) mil->h = mh;
else mil->h = mil->d * 3.;
mil->nx = mil->ny = mil->d/block.step;
mil->map = (float**)malloc(mil->nx*sizeof(float*));
for (i=0;i<mil->nx;i++) mil->map[i] = (float*)malloc(mil->ny*sizeof(float));
for (i=0;i<mil->nx;i++)
for (j=0;j<mil->ny;j++)
{
dI = i*block.step-mil->r;
dJ = j*block.step-mil->r;
dist = sqrt(dI*dI+dJ*dJ);
if (dist > mil->r) { mil->map[i][j] = 1e10; continue; } /* do not mill */
if (mil->mtype == 'f') { mil->map[i][j] = 0.; continue; }
dist = mil->r*mil->r-dI*dI-dJ*dJ;
if (dist < 0.) dist = 0.;
mil->map[i][j] = sqrt(dist);
if (!mil->center) mil->map[i][j] -= mil->r;
}
if (!mil->center) mil->h += mil->r;
printf("Mill(%f,%f) map (%dx%d)\n", mil->r, mil->h, mil->nx, mil->ny);
for (i=0;i<mil->nx;i++)
{
for (j=0;j<mil->ny;j++) printf("%f ", mil->map[i][j]);
printf("\n");
}
mil->path = 0;
/*while ((ok = mscanf(plik, &dx, &dy, &dz)) == 3)
plist_add(&mil->path, dx, dy, dz);
if (mil->path) while (mil->path->next) mil->path = mil->path->next;
fclose(plik);*/
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
 angX =  0.0;
 angY =  0.0;
 angZ =  0.0;
 d = 4.;
 obsD = 600.0;
 tStep = 2.00;
 construct_matrix(&perspectiveMatrix, 1.0,0.0,0.0,0.0,
	 			      0.0,1.0,0.0,0.0,
	 			      0.0,0.0,1.0,1.0/obsD,
	 			      0.0,0.0,0.0,1.0);
 construct_matrix(&scaleMatrix,       4.0,0.0,0.0,0.0,
	 			      0.0,4.0,0.0,0.0,
	 			      0.0,0.0,4.0,0.0,
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
   glVertex3d((GLfloat)rline.v[0].p[0], (GLfloat)rline.v[0].p[1], 0.);
   glVertex3d((GLfloat)rline.v[1].p[0], (GLfloat)rline.v[1].p[1], 0.);
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


void render_nurbs()
{
 int i,j,m;
 for (m=0;m<NMESH;m++)
 for (i=0;i<mesh[m].nNurbsX;i++)
 for (j=0;j<mesh[m].nNurbsY;j++) render_nurb(mesh[m].nurbs[i][j]);
}

void render_path()
{
 PList* path = mill.path;
 struct Line4D line;
 while (path->next)
   {
    construct_vector3(&line.v[0], path->z, path->x, path->y); 
    construct_vector3(&line.v[1], path->next->z, path->next->x, path->next->y); 
    render_line(&line);
    path = path->next;
   }
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
	case 'p':
		if (pmove)
		  {
		   bezier_from_deboor_idx_2da(mesh[cm].bez, mesh[cm].boor, mesh[cm].nBoorX, mesh[cm].nBoorY, si, sj);
                   update_nurbs_idx(si, sj, cm);
		   si = sj = -1;
		   pmove = 0;
		   break;
		  }
		if (!pmove)
		  {
		   get_point_nearesta(mesh[cm].boor, mesh[cm].nBoorX, mesh[cm].nBoorY,  &cursor4d, &si, &sj);
		   pmove = 1;
		  }
		break;
	case ' ': full_update_nurbs(); break;
	case ',': cm++; cm %= NMESH; si = sj = -1; break;
	case 'd': want_lines++; if (want_lines==3) want_lines = 0; break;
	case 'a': lpsegm/=2; if (lpsegm<3)  lpsegm = 3;  full_update_nurbs(); break;
	case 's': lpsegm*=2; if (lpsegm>24) lpsegm = 24; full_update_nurbs(); break;
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
 GLfloat scale;
 int i,j,m;
 int ii,jj;
 struct Line4D line;
 time_counter();
 scale = (GLfloat)wx + (GLfloat)wy;
 scale = 15. / scale;
 glClear(GL_COLOR_BUFFER_BIT);
 glEnable(GL_BLEND);
 glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
 glLoadIdentity();
 gluLookAt(0., 0., 5., 0. , 0., 0., 0., 1., 0.);
 glScaled(scale, scale, scale);
 glTranslated(-(GLfloat)(wx/2), - (GLfloat)(wy/2), 0.);
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
       glColor3d((GLfloat)curr_eye*0.75, (GLfloat)curr_eye*0.75, 1.);
       render_line(&line);
       glColor3d(1.,0.,0.);
      }
     if (m == cm && i == si && j == sj)
      {
       glColor3d((GLfloat)curr_eye*0.75, (GLfloat)curr_eye*0.75, 1.);
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
 glColor3d(0,0.5,1.);
 render_path();
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
       glColor3d((GLfloat)curr_eye*0.75, (GLfloat)curr_eye*0.75, 1.);
       render_line(&line);
       glColor3d(0.,1.,0.);
      }
     if (m == cm && i == si && j == sj)
      {
       glColor3d((GLfloat)curr_eye*0.75, (GLfloat)curr_eye*0.75, 1.);
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
 glColor3d(0,0.5,1.);
/* render_path();*/
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


void suszarka()
{
 int i;
 float scale, scalel;
 float dx,dy,dz;
 float dx1,dy1,dz1;
 float smill;
 smill = .68;
 mesh[0].nBoorX = 6;
 mesh[0].nBoorY = 9;
 mesh[0].nX = 3;
 mesh[0].nY = 6;
 mesh[0].boor = (struct Vector4D**)malloc(mesh[0].nBoorX*sizeof(struct Vector4D*));
 for (i=0;i<mesh[0].nBoorX;i++) mesh[0].boor[i] = (struct Vector4D*)malloc(mesh[0].nBoorY*sizeof(struct Vector4D));
 scale  = 14.*smill;
 scalel = 30.*smill;
 dx1 = 0.*smill;
 dy1 = 0.*smill;
 dz1 = 45.*smill;
 construct_vector3(&mesh[0].boor[0][0], 0.+dx1,     2.*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[0][1], -scale+dx1, 2.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[0][2], 0.+dx1    , 2.*scalel+dy1, scale+dz1);
 construct_vector3(&mesh[0].boor[0][3], scale+dx1,  2.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[0][4], 0.+dx1   ,  2.*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[0][5], -scale+dx1, 2.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[0][6], 0.+dx1    , 2.*scalel+dy1, scale+dz1);
 construct_vector3(&mesh[0].boor[0][7], scale+dx1,  2.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[0][8], 0.+dx1,     2.*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[1][0], 0.+dx1,     1.5*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[1][1], -scale+dx1, 1.5*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[1][2], 0.+dx1    , 1.5*scalel+dy1, scale+dz1);
 construct_vector3(&mesh[0].boor[1][3], scale+dx1,  1.5*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[1][4], 0.+dx1   ,  1.5*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[1][5], -scale+dx1, 1.5*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[1][6], 0.+dx1    , 1.5*scalel+dy1, scale+dz1);
 construct_vector3(&mesh[0].boor[1][7], scale+dx1,  1.5*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[1][8], 0.+dx1,     1.5*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[2][0], 0.+dx1,     0.*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[2][1], -scale+dx1, 0.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[2][2], 0.+dx1    , 0.*scalel+dy1, scale+dz1);
 construct_vector3(&mesh[0].boor[2][3], scale+dx1,  0.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[2][4], 0.+dx1   ,  0.*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[2][5], -scale+dx1, 0.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[2][6], 0.+dx1    , 0.*scalel+dy1, scale+dz1);
 construct_vector3(&mesh[0].boor[2][7], scale+dx1,  0.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[2][8], 0.+dx1,     0.*scalel+dy1, -scale+dz1);
 construct_vector3(&mesh[0].boor[3][0], -scale+dx1, -1.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[3][1], 0.+dx1    , -1.*scalel+dy1, scale+dz1);
 construct_vector3(&mesh[0].boor[3][2], scale+dx1,  -1.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[3][3], 0.+dx1,     -1.*scalel+dy1, -1.2*scale+dz1);
 construct_vector3(&mesh[0].boor[3][4], 0.+dx1,     -1.*scalel+dy1, -1.2*scale+dz1);
 construct_vector3(&mesh[0].boor[3][5], 0.+dx1    , -1.*scalel+dy1, -1.2*scale+dz1);
 construct_vector3(&mesh[0].boor[3][6], -scale+dx1, -1.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[3][7], 0.+dx1    , -1.*scalel+dy1, scale+dz1);
 construct_vector3(&mesh[0].boor[3][8], scale+dx1,  -1.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[4][0], -1.4*scale+dx1, -2.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[4][1], 0.+dx1    ,     -2.*scalel+dy1, 1.2*scale+dz1);
 construct_vector3(&mesh[0].boor[4][2], 1.4*scale+dx1,  -2.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[4][3], 0.+dx1   ,      -2.*scalel+dy1, -1.5*scale+dz1);
 construct_vector3(&mesh[0].boor[4][4], 0.+dx1   ,      -2.*scalel+dy1, -1.5*scale+dz1);
 construct_vector3(&mesh[0].boor[4][5], 0.+dx1    ,     -2.*scalel+dy1, -1.5*scale+dz1);
 construct_vector3(&mesh[0].boor[4][6], -1.4*scale+dx1, -2.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[4][7], 0.+dx1    ,     -2.*scalel+dy1, 1.2*scale+dz1);
 construct_vector3(&mesh[0].boor[4][8], 1.4*scale+dx1,  -2.*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[5][0], -.6*scale+dx1, -2.5*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[5][1], 0.+dx1    ,    -2.5*scalel+dy1, .5*scale+dz1);
 construct_vector3(&mesh[0].boor[5][2], .6*scale+dx1,  -2.5*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[5][3], 0.+dx1,        -2.5*scalel+dy1, -.5*scale+dz1);
 construct_vector3(&mesh[0].boor[5][4], 0.+dx1,        -2.5*scalel+dy1, -.5*scale+dz1);
 construct_vector3(&mesh[0].boor[5][5], 0.+dx1    ,    -2.5*scalel+dy1, -.5*scale+dz1);
 construct_vector3(&mesh[0].boor[5][6], -.6*scale+dx1, -2.5*scalel+dy1, 0.+dz1);
 construct_vector3(&mesh[0].boor[5][7], 0.+dx1    ,    -2.5*scalel+dy1, .5*scale+dz1);
 construct_vector3(&mesh[0].boor[5][8], .6*scale+dx1,  -2.5*scalel+dy1, 0.+dz1);
 bezier_from_deboor_2da(&mesh[0].bez, mesh[0].boor, &mesh[0].nBezX, &mesh[0].nBezY, mesh[0].nBoorX, mesh[0].nBoorY);
 mesh[1].nBoorX = 6;
 mesh[1].nBoorY = 7;
 mesh[1].nX = 3;
 mesh[1].nY = 4;
 mesh[1].boor = (struct Vector4D**)malloc(mesh[1].nBoorX*sizeof(struct Vector4D*));
 for (i=0;i<mesh[1].nBoorX;i++) mesh[1].boor[i] = (struct Vector4D*)malloc(mesh[1].nBoorY*sizeof(struct Vector4D));
 scale  = 22.*smill;
 scalel = 60*smill;
 dx = 0.;
 dy = 40.*smill;
 dz = 0.;
 construct_vector3(&mesh[1].boor[0][0], .8*dx+dx1       , .5*-scale+dy+dy1, -3.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[0][1], .8*-scale+dx+dx1, .5*dy+dy1       , -3.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[0][2], .8*dx+dx1       , .5*scale+dy+dy1 , -3.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[0][3], .8*scale+dx+dx1 , .5*dy+dy1       , -3.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[0][4], .8*dx+dx1       , .5*-scale+dy+dy1, -3.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[0][5], .8*-scale+dx+dx1, .5*dy+dy1       , -3.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[0][6], .8*dx+dx1       , .5*scale+dy+dy1 , -3.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[1][0], dx+dx1       , .5*-scale+dy+dy1, -2.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[1][1], -scale+dx+dx1, .5*dy+dy1    ,    -2.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[1][2], dx+dx1       , .5*scale+dy+dy1 , -2.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[1][3], scale+dx+dx1 , .5*dy+dy1    ,    -2.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[1][4], dx+dx1       , .5*-scale+dy+dy1, -2.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[1][5], -scale+dx+dx1, .5*dy+dy1    ,    -2.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[1][6], dx+dx1       , .5*scale+dy+dy1 , -2.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[2][0], .8*dx+dx1       , -scale+dy+dy1, -1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[2][1], .8*-scale+dx+dx1, dy+dy1    ,    -1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[2][2], .8*dx+dx1       , 1.5*scale+dy+dy1 , -1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[2][3], .8*scale+dx+dx1 , dy+dy1    ,    -1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[2][4], .8*dx+dx1       , -scale+dy+dy1, -1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[2][5], .8*-scale+dx+dx1, dy+dy1    ,    -1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[2][6], .8*dx+dx1       , 1.5*scale+dy+dy1 , -1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[3][0], 1.2*dx+dx1       , 2.*-scale+dy+dy1, 0.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[3][1], 1.2*-scale+dx+dx1, dy+dy1    ,    0.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[3][2], 1.2*dx+dx1       , scale+dy+dy1 , 0.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[3][3], 1.2*scale+dx+dx1 , dy+dy1    ,    0.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[3][4], 1.2*dx+dx1       , 2.*-scale+dy+dy1, 0.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[3][5], 1.2*-scale+dx+dx1, dy+dy1    ,    0.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[3][6], 1.2*dx+dx1       , scale+dy+dy1 , 0.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[4][0], .5*dx+dx1       , .3*-scale+dy+dy1, 1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[4][1], .3*-scale+dx+dx1, .3*dy+dy1    ,    1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[4][2], .5*dx+dx1       , .5*scale+dy+dy1 , 1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[4][3], .3*scale+dx+dx1 , .3*dy+dy1    ,    1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[4][4], .5*dx+dx1       , .5*-scale+dy+dy1, 1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[4][5], .3*-scale+dx+dx1, .3*dy+dy1    ,    1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[4][6], .5*dx+dx1       , .3*scale+dy+dy1 , 1.*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[5][0], .2*dx+dx1       , .4*-scale+dy+dy1, 1.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[5][1], .2*-scale+dx+dx1, .4*dy+dy1    ,    1.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[5][2], .2*dx+dx1       , .4*scale+dy+dy1 , 1.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[5][3], .2*scale+dx+dx1 , .4*dy+dy1    ,    1.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[5][4], .2*dx+dx1       , .4*-scale+dy+dy1, 1.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[5][5], .2*-scale+dx+dx1, .4*dy+dy1    ,    1.5*scalel+dz+dz1);
 construct_vector3(&mesh[1].boor[5][6], .2*dx+dx1       , .4*scale+dy+dy1 , 1.5*scalel+dz+dz1);
 bezier_from_deboor_2da(&mesh[1].bez, mesh[1].boor, &mesh[1].nBezX, &mesh[1].nBezY, mesh[1].nBoorX, mesh[1].nBoorY);
}

void plist_add(PList** pl, float x, float y, float z)
{
PList* tmp;
if (!pl) return;
tmp = (PList*)malloc(sizeof(PList));
tmp->x = x;
tmp->y = y;
tmp->z = z;
if (!(*pl)) { tmp->next = 0; tmp->prev = 0; *pl = tmp; }
else { tmp->next = *pl; (*pl)->prev = tmp; tmp->prev = 0; *pl = tmp; }
}

void mill_add_path(Mill* mil, FILE* file, float x, float y, float z)
{
 if (!file || !mil) return;
 plist_add(&mil->path, x, y, z-matz);
 fprintf(file, "X%fY%fZ%f\n", x,y,z);
 npath++;
}

void tesselator()
{
 float u,v; 
 int m;
 int i,j,i1,j1,nx,ny, dI, dJ,k;
/* int maxskip, lasti;*/
 struct Vector4D vec;
 float x,y,z;
 float minx,maxx,miny,maxy,minz,maxz;
 float hx,hy;
 float tmpd;
/* float prevz;*/
 FILE* plik;
 printf("Tesselator...\n");
 npath = 0;
 sprintf(fname, "tesselator.k%d", (int)mill_d);
 plik = fopen(fname, "w");
 if (!plik) { printf("cannot open for writing: %s\n", fname); exit(7); }
 dI = (int)(offset_dy/block.step);
 dJ = (int)(offset_dx/block.step);
/* maxskip = (int)(offset_dx/block.step);*/
/* fflush(stdout);*/
 hx = matx/2.;
 hy = maty/2.;
 minx = 1e9;
 miny = 1e9;
 minz = 1e9;
 maxx = -1e9;
 maxy = -1e9;
 maxz = -1e9;
 nx = mill.nx;
 ny = mill.ny;
 for (m=0;m<2;m++)
 for (u=0.;u<(float)mesh[m].nX;u+=param_step)
 {
  printf("pass%d, %3.2f%%...", m+1, (u/(float)mesh[m].nX)*100.);
  fflush(stdout);
 for (v=0.;v<(float)mesh[m].nY;v+=param_step)
 {
  nurb_computemv(&mesh[m], &vec, u, v);
  x = vec.p[1];
  y = vec.p[2];
  z = vec.p[0];
  x += hx;
  y += hy;
  i1 = (x-mill.r)/block.step;
  j1 = (y-mill.r)/block.step;
  if (z < 0.) continue;
  for (i=i1;i<i1+nx;i++)
  for (j=j1;j<j1+ny;j++)
    {
     if (mill.map[i-i1][j-j1] > 1e9) continue;
     if (i < 0 || i >= block.nx) continue;
     if (j < 0 || j >= block.ny) continue;
     tmpd = block.b[i][j];
     block.b[i][j] = z + matz + mill.map[i-i1][j-j1];
     if (block.b[i][j] < tmpd) block.b[i][j] = tmpd;
     if (block.b[i][j] > matmz) block.b[i][j] = matmz;
    }
  if (x < minx) minx = x;
  if (y < miny) miny = y;
  if (z < minz) minz = z;
  if (x > maxx) maxx = x;
  if (y > maxy) maxy = y;
  if (z > maxz) maxz = z;
 }
  printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
 }
 printf("(%f,%f,%f) -> (%f,%f,%f)\n", minx, miny, minz, maxx, maxy, maxz);
 printf("done.\n");
 printf("Path finding...");
 fflush(stdout);
 nx = block.nx;
 ny = block.ny;
 k = 0;
 mill_add_path(&mill, plik, -90., -90, 60.);
 mill_add_path(&mill, plik, -90., -90, 30.);
 for (i=0;i<nx;i+=dI)
 {
  k++;
  if (k%2) for (j=0;j<ny;j+=dJ) 
   {
     /*if (j==0)
     {
      prevz = block.b[i][j];
      lasti = 0;
     }
     if (j==0 || j-lasti > maxskip || abs(prevz-block.b[i][j]) > 1. )
       {
	mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);
	lasti = j;
	prevz = block.b[i][j];
       }*/
    /*if (block.b[i][j] > matz || j == 0 || j == ny-1) 
       {
	if (j != 0 && block.b[i][j-1] <= matz)
	  mill_add_path(&mill, plik, i*block.step-hx, (j-1)*block.step-hy, block.b[i][j-1]);
	mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);
	if (j != ny-1 && block.b[i][j+1] <= matz)
	  mill_add_path(&mill, plik, i*block.step-hx, (j+1)*block.step-hy, block.b[i][j+1]);
       }*/
     mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);  
   }
  else for (j=ny-1;j>=0;j-=dJ)  
   {
    /* if (j==ny-1)
     {
      prevz = block.b[i][j];
      lasti = 0;
     }
     if (j==ny-1 || lasti-j> maxskip || abs(prevz-block.b[i][j]) > 1. )
       {
	mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);
	lasti = j;
	prevz = block.b[i][j];
       }*/
    /*if (block.b[i][j] > matz || j == 0 || j == ny-1)
       {
	if (j != 0 &&  block.b[i][j-1] <= matz)
	  mill_add_path(&mill, plik, i*block.step-hx, (j-1)*block.step-hy, block.b[i][j]);
	mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);
	if (j != ny-1 && block.b[i][j+1] <= matz)
	  mill_add_path(&mill, plik, i*block.step-hx, (j+1)*block.step-hy, block.b[i][j]);
       }*/
     mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);  
   }
 }
 k=0;
 mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, 60.);
 mill_add_path(&mill, plik, -90., -90, 60.);
 mill_add_path(&mill, plik, -90., -90, 30.);
 for (j=0;j<ny;j+=dI)
 {
  k++;
  if (k%2) for (i=0;i<nx;i+=dJ) 
    {
     /*if (i==0)
     {
      prevz = block.b[i][j];
      lasti = 0;
     }
     if (i==0 || i-lasti > maxskip || abs(prevz-block.b[i][j]) > 1. )
       {
	mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);
	lasti = i;
	prevz = block.b[i][j];
       }*/
/*    if (block.b[i][j] > matz || i == 0 || i == nx-1) mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);*/
    /*if (block.b[i][j] > matz || i == 0 || i == nx-1) 
       {
	if (i != 0 && block.b[i-1][j] <= matz)
	  mill_add_path(&mill, plik, (i-1)*block.step-hx, j*block.step-hy, block.b[i-1][j]);
	mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);
	if (i != nx-1 && block.b[i+1][j] <= matz)
	  mill_add_path(&mill, plik, (i+1)*block.step-hx, j*block.step-hy, block.b[i+1][j]);
       }*/
     mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);  
    }
  else for (i=nx-1;i>=0;i-=dJ)  
    {
    /* if (i==nx-1)
     {
      prevz = block.b[i][j];
      lasti = 0;
     }
     if (i==nx-1 || lasti-i> maxskip || abs(prevz-block.b[i][j]) > 1. )
       {
	mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);
	lasti = i;
	prevz = block.b[i][j];
       }*/
    /*if (block.b[i][j] > matz || i == 0 || i == nx-1) 
       {
	if (i != 0 && block.b[i-1][j] <= matz)
	  mill_add_path(&mill, plik, (i-1)*block.step-hx, j*block.step-hy, block.b[i-1][j]);
	mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);
	if (i != nx-1 && block.b[i+1][j] <= matz)
	  mill_add_path(&mill, plik, (i+1)*block.step-hx, j*block.step-hy, block.b[i+1][j]);
       }*/
     mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, block.b[i][j]);  
    }
 }
 mill_add_path(&mill, plik, i*block.step-hx, j*block.step-hy, 60.);
 mill_add_path(&mill, plik, -90., -90, 60.);
 fclose(plik);
 printf("done %d paths.\n", npath);
}

void free_block(Block* bl)
{
 int i;
 for (i=0;i<block.nx;i++) free(bl->b[i]);
 free(bl->b);
 printf("Block freed.\n");
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
 t1 = t2 = (time_t)0;
 fps = 0;
 lpsegm = 6;
 time(&tm);
 srand((int)tm);
 setup_matrices();
 setup_batch();
 help();
 construct_cursor_default(&cursor4d, 1.0);
 suszarka();
 full_update_nurbs();
 init_block(&block, matx, matx, matz, precision);
 init_mill(&mill, mill_d, -1.);
 tesselator();
 free_block(&block);
 glClearColor(0.,0.,0.,0.);
 glShadeModel(GL_SMOOTH);
}


int main(int lb, char** par)
{
 wx = 800;
 wy = 600;
 if (lb !=6)
   { 
    printf("usage: %s preccision mill_D pathDX pathStep paramStep\n", par[0]);
    printf("using default values: .1 10. 2. .5 .04\n");
   }
 else 
  {
   precision  = atof(par[1]);
   mill_d     = atof(par[2]);
   offset_dx  = atof(par[3]);
   offset_dy  = atof(par[4]);
   param_step = atof(par[5]);
  }
 glutInit(&lb, par);
 glutInitDisplayMode(GLUT_DOUBLE);
 glutInitWindowSize(wx, wy);
 glutInitWindowPosition(10, 10);
 glutCreateWindow(par[0]);
 NurbsInit();
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
