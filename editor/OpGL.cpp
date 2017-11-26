#include "stdafx.h"
#include "Texture.h"

extern GLdouble angX, angY, angZ, zdis;
unsigned int* tids;
extern nTex;
extern Texture* texture;

void OpGL::SetDCPixelFormat()
{
	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, PFD_MAIN_PLANE,
		0, 0, 0, 0};
	nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);
}
void bind_textures()
{
 int i;
 //char tmp[1000];
 tids = new unsigned int[nTex];
 glEnable(GL_TEXTURE_2D);
 glGenTextures(nTex, tids);
 for (i=0;i<nTex;i++)
  {
   if (!texture[i].pixels)
     {
		 exit(1);
/*      tids[i] = 0; */
/*	 printf("skipping texture: %d tid -- %d\n", i, tids[i]);*/
      continue;
     }
   glBindTexture(GL_TEXTURE_2D, tids[i]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   /*sprintf(tmp, "tids[%d] = %d, x=%d, y=%d, mem=%p\n", i, tids[i], texture[i].x, texture[i].y, texture[i].pixels);
   MessageBox(0, tmp, tmp, 0);*/
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture[i].x, texture[i].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture[i].pixels);
  }
}


void OpGL::SetupRC()
{
	GLfloat ambientLight[] = {0.05f, 0.1f, 0.1f, 1.0f};
	GLfloat diffuseLight[] = {0.9f, 0.7f, 0.3f, 1.0f};
	GLfloat lightPos[] = {0.0f, 0.0f, 1.0f, 0.0f};

	GLfloat ambientLight2[] = {0.1f, 0.1f, 0.05f, 1.0f};
	GLfloat diffuseLight2[] = {0.3f, 0.7f, 0.9f, 1.0f};
	GLfloat lightPos2[] = {0.0f, 0.0f, -1.0f, 0.0f};

	GLfloat ambientLight3[] = {0.1f, 0.05f, 0.1f, 1.0f};
	GLfloat diffuseLight3[] = {0.4f, 0.9f, 0.4f, 1.0f};
	GLfloat lightPos3[] = {1.0f, 0.0f, 0.0f, 0.0f};

    GLfloat ambientLight4[] = {0.05f, 0.1f, 0.05f, 1.0f};
	GLfloat diffuseLight4[] = {0.8f, 0.3f, 0.8f, 1.0f};
	GLfloat lightPos4[] = {0.0f, -1.0f, 0.0f, 0.0f};
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight2);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight3);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos3);
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight4);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight4);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPos4);
	glEnable(GL_LIGHT3);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	bind_textures();
}


OpGL::OpGL(HWND hWnd)
{
	hdc = GetDC(hWnd);
	SetDCPixelFormat();
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);
	SetupRC();
}

void World()
{
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslated(0.0f, 0.0f, zdis);
 glRotated(angX, 1.0f, 0.0f, 0.0f);
 glRotated(angY, 0.0f, 1.0f, 0.0f);
 glRotated(angZ, 0.0f, 0.0f, 1.0f);
}


void CALLBACK OpGL::ChangeSize(GLsizei w, GLsizei h)
{
	if(!h) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, (GLdouble)w/(GLdouble)h, 1.0, 1000.0);
	World();
}


void CALLBACK OpGL::RenderScene(Scene &scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	World();
	scene.Draw();
	glFlush();
	SwapBuffers(hdc);
}

OpGL::~OpGL()
{
	wglMakeCurrent(hdc, NULL);
	wglDeleteContext(hrc);
}
