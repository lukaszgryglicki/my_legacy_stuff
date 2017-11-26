#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "GL/glut.h"				/* OpenGL*/

struct pdata
{
    long double ar, ai, b, cr, ci;
    float R,G,B;
};

struct csvdata
{
    pdata ***v;
    int r, i ,x;
};

csvdata* cd;

int render(char* fname)
{
    //0;0;0;2;0;-1.999;-9.72571;0;
    FILE* f;

    f = fopen(fname, "r");
    if (!f)
    {
	printf("Cannot read from: %s\n", fname);
	return false;
    }

    int r, i, x, n;
    int mr, mi, mx;
    long double ar, ai, b, cr, ci;

    mr = mi = mx = -1;

    while ((n=fscanf(f, "%d;%d;%d;%Lf;%Lf;%Lf;%Lf;%Lf;\n", &r, &i, &x, &ar, &ai, &b, &cr, &ci)) == 8)
    {
	if (r > mr) mr = r;
	if (i > mi) mi = i;
	if (x > mx) mx = x;
    }
    printf("Max indices: Real: %d Imag: %d X-Domain: %d\n", mr, mi, mx);
    fseek(f, 0, SEEK_SET);

    mr ++;
    mi ++;
    mx ++;

    if (!mr || !mi || !mx)
    {
	printf("Each index must be at least 1!\n");
	return false;
    }

    cd = new csvdata;
    cd->r = mr;
    cd->i = mi;
    cd->x = mx;

    cd->v = new pdata**[cd->r];
    for (r=0;r<cd->r;r++) 
    {
	cd->v[r] = new pdata*[cd->i];
	for (i=0;i<cd->i;i++) cd->v[r][i] = new pdata[cd->x];
    }

    while ((n=fscanf(f, "%d;%d;%d;%Lf;%Lf;%Lf;%Lf;%Lf;\n", &r, &i, &x, &ar, &ai, &b, &cr, &ci)) == 8)
    {
	cd->v[r][i][x].ar = ar; 
	cd->v[r][i][x].ai = ai; 
	cd->v[r][i][x].b  = b; 
	cd->v[r][i][x].cr = cr; 
	cd->v[r][i][x].ci = ci; 
    
	//printf("%d;%d;%d;%Lf;%Lf;%Lf;%Lf;%Lf;\n", r, i, x, ar, ai, b, cr, ci);
    }

    fclose(f);

    return true;
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

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.01, 200.0);
    glMatrixMode(GL_MODELVIEW);		
}

long double s_x, e_x, s_ar, e_ar, s_ai, e_ai, s_cr, e_cr, s_ci, e_ci;
double scalex, scaley, scalez, rotx, roty, rotz;
double trsx, trsy, trsz;
int rmode, cur_r, cur_i, cur_x;
float cut_av;

void Init();

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
	case 27 : case 'q': exit(0); break;
	case 'j': roty += 3.0; break;
	case 'l': roty -= 3.0; break;
	case 'i': rotx += 3.0; break;
	case 'k': rotx -= 3.0; break;
	case 'o': rotz += 3.0; break;
	case 'm': rotz -= 3.0; break;
	case 'w': scaley *= 1.1; break;
	case 's': scaley /= 1.1; break;
	case 'd': scalex *= 1.1; break;
	case 'a': scalex /= 1.1; break;
	case 'e': scalez *= 1.1; break;
	case 'x': scalez /= 1.1; break;
	case '1': scalex /= 1.1; scaley /= 1.1; scalez /= 1.1; break;
	case '2': scalex *= 1.1; scaley *= 1.1; scalez *= 1.1; break;
	case 't': trsy += 0.1; break;
	case 'g': trsy -= 0.1; break;
	case 'f': trsx -= 0.1; break;
	case 'h': trsx += 0.1; break;
	case 'y': trsz -= 0.1; break;
	case 'b': trsz += 0.1; break;
	case ' ': Init(); break;
	case 'p': glutFullScreen(); break;
	case '3': rmode ++; rmode %= 13; break;
	case '4': rmode --; if(rmode < 0) rmode = 12; break;
	case '5': cur_r ++; cur_r %= cd->r; break;
	case '6': cur_r --; if (cur_r < 0) cur_r = cd->r-1; break;
	case '7': cur_i ++; cur_i %= cd->i; break;
	case '8': cur_i --; if (cur_i < 0) cur_i = cd->i-1; break;
	case '9': cur_x ++; cur_x %= cd->x; break;
	case '0': cur_x --; if (cur_x < 0) cur_x = cd->x-1; break;
    }
}

void make_color(int r, int i, int x, int rr, int ii, int xx, pdata* pd)
{
    pd->R = cbrt((float)(r+1) / (float)rr);
    pd->G = cbrt((float)(i+1) / (float)ii);
    pd->B = cbrt((float)(x+1) / (float)xx);
}

char* get64(uint64_t i, char* str)
{
    int a, b, c;
    c = i & 0x1fffff;
    i >>= 21;
    b = i & 0x1fffff;
    i >>= 21;
    a = i;
    sprintf(str, "%d;%d;%d", a, b, c);
    return str;
}

void get64i(uint64_t i, int* a, int* b, int* c)
{
    *c = i & 0x1fffff;
    i >>= 21;
    *b = i & 0x1fffff;
    i >>= 21;
    *a = i;
}

uint64_t idx64(int a, int b, int c)
{
    //char s[256];
    uint64_t i = ((uint64_t)a << 42) + ((uint64_t)b << 21) + (uint64_t)c;
    //printf("New idx for %d %d %d --> %lld --> %s \n", a, b, c, i, get64(i, s));
    return i;
}

void Init()
{
  s_x = 1e300;
  e_x = 1e-300;
  s_ar = 1e300;
  e_ar = 1e-300;
  s_ai = 1e300;
  e_ai = 1e-300;
  s_cr = 1e300;
  e_cr = 1e-300;
  s_ci = 1e300;
  e_ci = 1e-300;

  printf("Cut AV: %f\n", cut_av);

  uint64_t s_cr_i, e_cr_i, s_ci_i, e_ci_i;
  s_cr_i = e_cr_i = s_ci_i = e_ci_i = 0;
  char s1[0x100], s2[0x100];
  //long double cut_v = cut_av;
  //long double cut_v = 0.;
  long double cut_v = 1e-300;

  int r,i,x;
  for (r=0;r<cd->r;r++)
  for (i=0;i<cd->i;i++)
  for (x=0;x<cd->x;x++)
  {
      if (cut_av >= 0.)
      {
	  if (cd->v[r][i][x].cr < -cut_av) cd->v[r][i][x].cr = -cut_v;
	  else if (cd->v[r][i][x].cr > cut_av) cd->v[r][i][x].cr = cut_v;
	  if (cd->v[r][i][x].ci < -cut_av) cd->v[r][i][x].ci = -cut_v;
	  else if (cd->v[r][i][x].ci > cut_av) cd->v[r][i][x].ci = cut_v;
      }

      if (cd->v[r][i][x].b < s_x) s_x = cd->v[r][i][x].b;
      if (cd->v[r][i][x].b > e_x) e_x = cd->v[r][i][x].b;
      if (cd->v[r][i][x].ar < s_ar) s_ar = cd->v[r][i][x].ar;
      if (cd->v[r][i][x].ar > e_ar) e_ar = cd->v[r][i][x].ar;
      if (cd->v[r][i][x].ai < s_ai) s_ai = cd->v[r][i][x].ai;
      if (cd->v[r][i][x].ai > e_ai) e_ai = cd->v[r][i][x].ai;

      if (cd->v[r][i][x].cr < s_cr) { s_cr = cd->v[r][i][x].cr; s_cr_i = idx64(r, i, x); }
      if (cd->v[r][i][x].cr > e_cr) { e_cr = cd->v[r][i][x].cr; e_cr_i = idx64(r, i, x); }
      if (cd->v[r][i][x].ci < s_ci) { s_ci = cd->v[r][i][x].ci; s_ci_i = idx64(r, i, x); }
      if (cd->v[r][i][x].ci > e_ci) { e_ci = cd->v[r][i][x].ci; e_ci_i = idx64(r, i, x); }
	  
      make_color(r, i, x, cd->r, cd->i, cd->x, &(cd->v[r][i][x]));
  }

  printf("X range:      [%Lf %Lf]\n", s_x, e_x);
  printf("A real range: [%Lf %Lf]\n", s_ar, e_ar);
  printf("A imag range: [%Lf %Lf]\n", s_ai, e_ai);
  printf("C real range: [%Lf %Lf] idx: [%s %s]\n", s_cr, e_cr, get64(s_cr_i, s1), get64(e_cr_i, s2));
  printf("C imag range: [%Lf %Lf] idx: [%s %s]\n", s_ci, e_ci, get64(s_ci_i, s1), get64(e_ci_i, s2));
  get64i(s_cr_i, &r, &i, &x);
  printf("Min C real is for %d;%d;%d: (%Lf, %Lf) ^^ %Lf = (%Lf, %Lf)\n", r, i, x, cd->v[r][i][x].ar, cd->v[r][i][x].ai, cd->v[r][i][x].b, cd->v[r][i][x].cr, cd->v[r][i][x].ci);
  get64i(e_cr_i, &r, &i, &x);
  printf("Max C real is for %d;%d;%d: (%Lf, %Lf) ^^ %Lf = (%Lf, %Lf)\n", r, i, x, cd->v[r][i][x].ar, cd->v[r][i][x].ai, cd->v[r][i][x].b, cd->v[r][i][x].cr, cd->v[r][i][x].ci);
  get64i(s_ci_i, &r, &i, &x);
  printf("Min C imag is for %d;%d;%d: (%Lf, %Lf) ^^ %Lf = (%Lf, %Lf)\n", r, i, x, cd->v[r][i][x].ar, cd->v[r][i][x].ai, cd->v[r][i][x].b, cd->v[r][i][x].cr, cd->v[r][i][x].ci);
  get64i(e_ci_i, &r, &i, &x);
  printf("Max C imag is for %d;%d;%d: (%Lf, %Lf) ^^ %Lf = (%Lf, %Lf)\n", r, i, x, cd->v[r][i][x].ar, cd->v[r][i][x].ai, cd->v[r][i][x].b, cd->v[r][i][x].cr, cd->v[r][i][x].ci);

  /*
  scalex = 2./(e_x-s_x);	
  scaley = 2./(e_cr-s_cr);
  scalez = 2./(e_ci-s_ci);
  //scalez = 1./s_ci;
  */
  scalex = scaley = scalez = 1.;
  trsx = trsy = trsz = 0.;
  rotx = roty = rotz = 0.;
/*  roty = 180;*/
  cur_r = cur_i = cur_x = 0;
  rmode = 12;
}

void set_indices(int* rS, int* rE, int* iS, int* iE, int* xS, int* xE, int r, int i, int x)
{
    *rS = r ? cur_r : 0;
    *iS = i ? cur_i : 0;
    *xS = x ? cur_x : 0;

    *rE = r ? (cur_r + 1) : cd->r;
    *iE = i ? (cur_i + 1) : cd->i;
    *xE = x ? (cur_x + 1) : cd->x;
}

bool cut_away(pdata* p)
{
    long double f1 = fabs(p->cr);
    long double f2 = fabs(p->ci);

    if (f1 > 0. && f1 < 1e-299) return false;
    if (f2 > 0. && f2 < 1e-299) return false;
    return true;
}

void display(void)
{		
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();	
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
  double z = -s_ci*1.1;
  if (z <= 0.) z = -2.;
  gluLookAt(0.,0.,z, 0.,0.,0.,0.,1.,0.);
  //gluLookAt(0.,0.,-2.0,0.,0.,0.,0.,1.,0.);

  glTranslated(trsx, trsy, trsz);
  glRotated(rotx, 1., 0., 0.);
  glRotated(roty, 0., 1., 0.);
  glRotated(rotz, 0., 1., 1.);
  glScaled(scalex, scaley, scalez);

  int rS, iS, xS, rE, iE, xE;

  if (rmode == 0) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 0, 0);	/* line on X */
  else if (rmode == 1) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 1, 0, 0);
  else if (rmode == 2) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 1, 0);
  else if (rmode == 3) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 1, 1, 0);
  else if (rmode == 4) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 0, 0);	/* line on CI */
  else if (rmode == 5) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 1, 0, 0);
  else if (rmode == 6) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 0, 1);
  else if (rmode == 7) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 1, 0, 1);
  else if (rmode == 8) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 0, 0);	/* line on CR */
  else if (rmode == 9) 	set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 1, 0);
  else if (rmode == 10) set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 0, 1);
  else if (rmode == 11) set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 1, 1);
  else 			set_indices(&rS, &rE, &iS, &iE, &xS, &xE, 0, 0, 0);

  char wtitle[0x100];
  int rm = rmode >> 2;

  if (rm == 0 || rm == 3)
  {
        sprintf(wtitle, "X lines mode: (%d,%d), RIX: (%d/%d,%d/%d,%d/%d)", rmode, rmode%4, cur_r, cd->r, cur_i, cd->i, cur_x, cd->x);
        if (rm == 3) sprintf(wtitle, "Hypersurfaces mode");
        glutSetWindowTitle(wtitle);
  	for (int r=rS;r<rE;r++)
  	for (int i=iS;i<iE;i++)
  	{
      		glBegin(GL_LINE_STRIP);
      		for (int x=xS;x<xE;x++)
      		{
		    	if (!cut_away(&cd->v[r][i][x])) 
			{
			    glEnd();
			    glBegin(GL_LINE_STRIP);
			    continue;
			}
	  		glColor3f(cd->v[r][i][x].R, cd->v[r][i][x].G, cd->v[r][i][x].B);
	  		glVertex3d(cd->v[r][i][x].b, cd->v[r][i][x].cr, cd->v[r][i][x].ci);
	  		//std::cout<<cd->v[r][i][x].b << " " << cd->v[r][i][x].cr << " "  << cd->v[r][i][x].ci << "\n";
      		}
      		glEnd();
  	}
  }
  else if (rm == 1 || rm == 3)
  {
        sprintf(wtitle, "I lines mode: (%d,%d), RIX: (%d/%d,%d/%d,%d/%d)", rmode, rmode%4, cur_r, cd->r, cur_i, cd->i, cur_x, cd->x);
        if (rm == 3) sprintf(wtitle, "Hypersurfaces mode");
        glutSetWindowTitle(wtitle);
  	for (int r=rS;r<rE;r++)
      	for (int x=xS;x<xE;x++)
  	{
      		glBegin(GL_LINE_STRIP);
  		for (int i=iS;i<iE;i++)
      		{
		    	if (!cut_away(&cd->v[r][i][x])) 
			{
			    glEnd();
			    glBegin(GL_LINE_STRIP);
			    continue;
			}
	  		glColor3f(cd->v[r][i][x].R, cd->v[r][i][x].G, cd->v[r][i][x].B);
	  		glVertex3d(cd->v[r][i][x].b, cd->v[r][i][x].cr, cd->v[r][i][x].ci);
	  		//std::cout<<cd->v[r][i][x].b << " " << cd->v[r][i][x].cr << " "  << cd->v[r][i][x].ci << "\n";
      		}
      		glEnd();
  	}
  }
  if (rm == 2 || rm == 3)
  {
        sprintf(wtitle, "R lines mode: (%d,%d), RIX: (%d/%d,%d/%d,%d/%d)", rmode, rmode%4, cur_r, cd->r, cur_i, cd->i, cur_x, cd->x);
        if (rm == 3) sprintf(wtitle, "Hypersurfaces mode");
        glutSetWindowTitle(wtitle);
      	for (int x=xS;x<xE;x++)
  	for (int i=iS;i<iE;i++)
  	{
      		glBegin(GL_LINE_STRIP);
  		for (int r=rS;r<rE;r++)
      		{
		    	if (!cut_away(&cd->v[r][i][x])) 
			{
			    glEnd();
			    glBegin(GL_LINE_STRIP);
			    continue;
			}
	  		glColor3f(cd->v[r][i][x].R, cd->v[r][i][x].G, cd->v[r][i][x].B);
	  		glVertex3d(cd->v[r][i][x].b, cd->v[r][i][x].cr, cd->v[r][i][x].ci);
	  		//std::cout<<cd->v[r][i][x].b << " " << cd->v[r][i][x].cr << " "  << cd->v[r][i][x].ci << "\n";
      		}
      		glEnd();
  	}
  }
			
  // Y axis
  //if (s_x < 0. && e_x > 0. && s_cr < 0. && e_cr > 0.)	
  { 
      glColor3f(1.,0.,0.);	
	
      glBegin(GL_LINES);
      	glVertex3d(0., s_cr, 0.);
	glVertex3d(0., e_cr, 0.);
      glEnd();
  }

  // X axis
  //if (s_x < 0. && e_x > 0. && s_cr < 0. && e_cr > 0.)	
  {
      glColor3f(0.,0.,1.);	
      glBegin(GL_LINES);
	   glVertex3d(s_x, 0., 0.);
	   glVertex3d(e_x, 0., 0.);
      glEnd();
  }

  // Z axis
  //if (s_cr < 0. && e_cr > 0. && s_ci < 0. && e_ci > 0.)	
  {
      glColor3f(0.,1.,0.);	
      glBegin(GL_LINES);
	   glVertex3d(0., 0., s_ci);
	   glVertex3d(0., 0., e_ci);
      glEnd();
  }
  
  glFlush();			
  glutSwapBuffers();	
}

int main(int lb, char** par)
{
    if (lb < 2) 
    {
	printf("usage: %s file.csv {generated by mpctet} [cut abs value]\n", par[0]);
	return 1;
    }
    if (lb > 2) cut_av = atof(par[2]);
    else cut_av = -1;
    if (render(par[1]))
    {
	Init();
	glutInit(&lb, par);	
  	glutInitDisplayMode(GLUT_DOUBLE);
  	glutInitWindowSize(600, 450);	
  	glutInitWindowPosition(10, 10);
  	glutCreateWindow("opgl");
  	glutDisplayFunc(display);
  	glutReshapeFunc(reshape);
  	glutKeyboardFunc(keyboard);	
  	glutVisibilityFunc(visible);
  	glutMainLoop();	
    }
}

