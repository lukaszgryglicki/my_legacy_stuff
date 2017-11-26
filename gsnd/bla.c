#define WIN32_LEAN_AND_MEAN				// "odchudza" aplikacjê Windows
#include <windows.h>					// standardowy plik nag³ówkowy Windows 
#include <winuser.h>          // Windows constants
#include <gl/gl.h>						// standardowy plik nag³ówkowy OpenGL 
#include <gl/glu.h>						// funkcje pomocnicze OpenGL 
#include <stdlib.h>           // generator liczb pseudolosowych
#include <stdio.h>
#include "stereo.h"


#define WND_CLASS_NAME  "OpenGL Window Class"
#define HERE __FILE__,__LINE__,__DATE__,__TIME__
#define GLuchar unsigned char


typedef struct _V
{
 GLdouble x,y,z;
 GLdouble nx,ny,nz;
 GLdouble tx,ty;
} Vertex;

typedef struct _P
{
 GLint n;
 Vertex* v;
 GLint flag;
} Poly;

typedef struct _T
{
 GLuint id;
 GLuint sizx,sizy;
 GLuchar *data;
} Texture;

HDC       g_hdc;		  // globalny kontekst urz¹dzenia
HGLRC     g_hrc;                  // globalny kontekst tworzenia grafiki
BOOL      g_isFullscreen = TRUE;  // prze³¹cza tryb pe³noekranowy i okienkowy
BOOL      g_isActive = TRUE;      // false jeœli okno zwiniête do ikony
HWND      g_hwnd = NULL;          // uchwyt okna  
HINSTANCE g_hInstance;            // instancja aplikacji
GLdouble eye;

// wymiary okna
GLuint g_width, g_height;
// zmienne s³u¿¹ce przechowywaniu danych uzyskanych od OpenGL
GLfloat g_pointSize = 1.0;
GLfloat g_pointSizeStep;
GLfloat g_pointSizeMinMax[2];
GLfloat g_lineWidth = 1.0;
GLfloat g_lineWidthStep;
GLfloat g_lineWidthMinMax[2];

GLint nPoly;
GLint nVert;
GLint nTex;
GLint maxPolys;
GLint glob_alloc;
Vertex* vList;
Poly* pList;
Texture* tList;
GLfloat angX, angY, angZ, scalef;
GLint want_tex, want_light;
GLint auto_move;
GLfloat obsD;

/******************************** Prototypy ********************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SetupWindow(char *title, int width, int height, int bits, bool isFullscreen);
BOOL InitializeScene();
GLvoid ResizeScene(GLsizei width, GLsizei height);
BOOL DisplayScene();
BOOL KillWindow();

void fatal(char* par)
{
 printf("Fatal error: %s\n", par?par:"(null)");
 exit(3);
}

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

void blank_polys(Poly* list, GLint n)
{
 GLint i;
 if (!list) return;
 for (i=0;i<n;i++) list[i].v = NULL;
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

void help()
{

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
 want_tex = 1;
 want_light = 1;
 auto_move = 1;
 obsD = 200.;
 eye = 5.;
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

void simplest_dyna(Texture* t)
{
 t->sizx = t->sizy = 1;
 t->data = (GLuchar*)malloc(4);
 t->data[0] = 125;
 t->data[1] = 125;
 t->data[2] = 125;
 t->data[3] = 125;
}

void dyna_tex_data(Texture* t)
{
 t->sizx = t->sizy = 4;
 t->data = (GLuchar*)mm_malloc(HERE,0X80*sizeof(GLuchar));

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

// printf("(%d x %d) --> %p [%x]\n", t->sizx, t->sizy, t->data, t->data[63]);
 
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  MSG   msg;       // komunikat
  BOOL  isDone;    // znacznik zakoñczenia pracy aplikacji
 
  Init(); 
  LoadTriangles8("bla.uli");
  init_textures_file("tex0.dat");

  SetupWindow("OpenGL", 1024, 768, 32, TRUE);

  isDone = FALSE;

  while (!isDone)
  {
    if(PeekMessage(&msg, g_hwnd, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
      {
        isDone = TRUE;
      }
      else
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }

    DisplayScene();
    SwapBuffers(g_hdc);
  }

  KillWindow();

  return msg.wParam;
} // WinMain()


/****************************************************************************
 WndProc

 procedura okienkowa
*****************************************************************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static BOOL   antialias = false;    // znacznik antialiasingu
  static GLenum polyMode  = GL_FILL;  // wybór trybu rysowania wielok¹tów
  static BOOL   stipple   = false;    // znacznik wype³nienia  wielok¹tów

  switch(message)
  {
  case WM_ACTIVATE:  // okno jest zwijane do ikony b¹dŸ rozwijane
    {
      if (!HIWORD(wParam))
      {
        // okno zosta³o rozwiniête
        g_isActive = TRUE;
      }
      else
      {
        // okno zosta³o zwiniête do ikony
        g_isActive=FALSE;
      }

      return 0;
    }
  case WM_SYSCOMMAND:  // wygaszacz ekranu b¹dŸ tryb oszczêdny monitora
    {
      switch (wParam)
			{
				case SC_SCREENSAVE:     // uruchamiany jest wygaszacz ekranu 
				case SC_MONITORPOWER:   // monitor prze³¹cza siê w tryb oszczêdny
          // zwrócenie wartoœci 0 zapobiega obu akcjom
  				return 0;
        default:
          break;
      }
    } break;
  case WM_CLOSE:    // okno jest zamykane
    {
      // wstawia komunikat WM_QUIT do kolejki
      PostQuitMessage(0);
      return 0;
    }
  case WM_SIZE:
    {
      // aktualizuje szerokoœæ i wysokoœæ sceny
      ResizeScene(LOWORD(lParam), HIWORD(lParam));
      return 0;
    }
  case WM_CHAR:
    {
      if (wParam >= 'a' && wParam <= 'z') wParam -= 0x20;
      switch (wParam)
      {
      case VK_ESCAPE:
      case 'Q':
        {
          // wstawia komunikat WM_QUIT do kolejki
          PostQuitMessage(0);
          return 0;
        }
      case 'A':   // prze³¹cza antialiasing
        {
          antialias = !antialias;

          // w³¹cza antialiasing
          if (antialias)
          {
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);
            glEnable(GL_BLEND);
          }
          // wy³¹cza antialiasing
          else
          {
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
            glDisable(GL_BLEND);
          }
          return 0;
        }
      case 'P':   // prze³¹cza tryby rysowania wielok¹tów
        {
          // przechodzi do nastêpnego trybu
          polyMode++;

          // zapêtla prze³¹czanie trybów
          if (polyMode > GL_FILL)
            polyMode = GL_POINT;

          // wybiera nowy tryb rysowania wielok¹tów
          glPolygonMode(GL_FRONT_AND_BACK, polyMode);
        }
      case '=':   // zwiêksza rozmiary
        {
          // zwiêksza rozmiar punktów (jeœli jest mniejszy od maksymalnego)
          if (g_pointSize < g_pointSizeMinMax[1])
            g_pointSize += g_pointSizeStep;

          glPointSize(g_pointSize);

          // zwiêksza szerokoœæ odcinków (jeœli jest mniejsza od maksymalnej)
          if (g_lineWidth < g_lineWidthMinMax[1])
            g_lineWidth += g_lineWidthStep;

          glLineWidth(g_lineWidth);

          return 0;
        }
      case 'T':
	{
	want_tex = ! want_tex;
	return 0;
	}
      case 'L':
	{
	want_light = !want_light;
	return 0;
	}
      case 'W':
	{
	 obsD /= 1.05;
	return 0;
	}
      case 'S':
	{
	 obsD *= 1.05;
	return 0;
	}

      case '-':   // zmniejsza rozmiary
        {
          // zmniejsza rozmiar punktów (jeœli jest wiêkszy od minimalnego)
          if (g_pointSize > g_pointSizeMinMax[0])
            g_pointSize -= g_pointSizeStep;

          glPointSize(g_pointSize);

          // zmniejsza szerokoœæ odcinków (jeœli jest wiêksza od minimalnej)          
          if (g_lineWidth > g_lineWidthMinMax[0])
            g_lineWidth -= g_lineWidthStep;

          glLineWidth(g_lineWidth);

          return 0;
        }
      default:
        break;
      };
    } break;

  default:
    break;
  }

  return (DefWindowProc(hwnd, message, wParam, lParam));
} // WndProc()


BOOL SetupWindow(char *title, int width, int height, int bits, bool isFullscreen)
{
  g_isFullscreen = isFullscreen;

  g_hInstance	= GetModuleHandle(NULL);

  WNDCLASSEX  wc;    // klasa okna

  wc.cbSize         = sizeof(WNDCLASSEX);
  wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc    = WndProc;
  wc.cbClsExtra     = 0;
  wc.cbWndExtra     = 0;
  wc.hInstance      = g_hInstance;
  wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);	// domyœlna ikona
  wc.hIconSm        = LoadIcon(NULL, IDI_WINLOGO);		  // logo windows 
  wc.hCursor        = LoadCursor(NULL, IDC_ARROW);		  // strza³ka (kursor domyœlny)
  wc.hbrBackground  = NULL; //(HBRUSH) GetStockObject(BLACK_BRUSH);   // t³o w kolorze czarnym
  wc.lpszMenuName   = NULL;     // bez menu
  wc.lpszClassName  = WND_CLASS_NAME;
  
  if (!RegisterClassEx(&wc))
  {
    MessageBox(NULL,"Nie mo¿na zarejestrowaæ klasy okna", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;							// zwraca wartoœæ FALSE
  }

  if (g_isFullscreen)
  {
    // wype³nia strukturê opisuj¹c¹ tryb wyœwietlania
    DEVMODE screenSettings;
    memset(&screenSettings,0,sizeof(screenSettings));

    screenSettings.dmSize       = sizeof(screenSettings);	
    screenSettings.dmPelsWidth  = width;			// szerokoœæ ekranu
    screenSettings.dmPelsHeight = height;			// wysokoœæ ekranu
    screenSettings.dmBitsPerPel = bits;				// bitów na piksel
    screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // próbuje prze³aczyæ kartê na zdefiniowany wy¿ej tryb graficzny
    if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
      if (MessageBox(NULL, "Prze³¹czenie do wybranego trybu graficznego nie jest mo¿liwe\n"
                           "Czy przejœæ do tryby okinkowego?",
                           "Programowanie gier w OpenGL",
                           MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
      {
        g_isFullscreen = FALSE;	
      }
      else
			{
				return FALSE;
			}
    }
  }

	DWORD dwExStyle;
	DWORD dwStyle;

	if (g_isFullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;	// okno bez ramki i paska tytu³u
		ShowCursor(FALSE);            // ukrywa kursor
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

  RECT  windowRect;
  windowRect.left = 0;
  windowRect.right = (LONG) width;
  windowRect.top = 0;
  windowRect.bottom = (LONG) height;


  AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

  g_hwnd = CreateWindowEx(dwExStyle,          // styl rozszerzony
                          WND_CLASS_NAME,     // nazwa klasy
                          title,              // nazwa aplikacji
                          dwStyle |           // styl okna
                          WS_CLIPCHILDREN |   // wymagane przez Opengl
                          WS_CLIPSIBLINGS,    
                          0, 0,               // wspó³rzêdne x,y coordinate
                          windowRect.right - windowRect.left, // szerokoœæ
                          windowRect.bottom - windowRect.top, // wysokoœæ
                          NULL,               // uchwyt okna nadrzêdnego 
                          NULL,               // uchwyt menu
                          g_hInstance,        // instancja aplikacji
                          NULL);              // bez dodatkowych parametrów

  if (!g_hwnd)
	{
    
		KillWindow();
		MessageBox(NULL, "Nie mo¿na utworzyæ okna", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(g_hdc = GetDC(g_hwnd)))
	{

		KillWindow();
		MessageBox(NULL,"Nie mo¿na utworzyæ kontekstu urz¹dzenia", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

  // konfiguruje format pikseli
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),	// rozmiar struktury
    1,                              // domyœlna wersja
    PFD_DRAW_TO_WINDOW |            // okno grafiki OpenGL
    PFD_SUPPORT_OPENGL |            
    PFD_DOUBLEBUFFER |               // podwójne buforowanie
    PFD_STEREO,
    PFD_TYPE_RGBA,                  // tryb kolorów RGBA 
    bits,                           // 32-bitowy opis kolorów
    0, 0, 0, 0, 0, 0,               // ignoruje bity koloru, bez palety
    0,                              // bez bufora alfa
    0,                              // ignoruje bit przesuniêcia
    0,                              // bez bufora akumulacji
    0, 0, 0, 0,                     // ignoruje bity akumulacji
    16,                             // 16-bitowy bufor z
    0,                              // bez bufora powielania
    0,                              // bez bufora pomocniczego
    PFD_MAIN_PLANE,                 // g³ówny plan rysowania
    0,                              // zarezerwowane
    0, 0, 0 };                      // ignoruje maski warst
      
	GLuint  pixelFormat;

	// wybiera najodpowiedniejszy format pikseli
	if (!(pixelFormat = ChoosePixelFormat(g_hdc, &pfd)))
	{
    
		KillWindow();
		MessageBox(NULL, "Nie mo¿na dopasowaæ formatu pikseli", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	// wybiera format pikseli dla kontekstu urz¹dzenia
  if(!SetPixelFormat(g_hdc, pixelFormat,&pfd))
	{
    
		KillWindow();
		MessageBox(NULL, "Nie mo¿na wybraæ formatu pikseli", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
  }

  // tworzy kontekst tworzenia grafiki OpenGL
  if (!(g_hrc = wglCreateContext(g_hdc)))
	{
    		KillWindow();
		MessageBox(NULL, "Nie mo¿na utworzyæ kontekstu grafiki OpenGL", "B³¹d",MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

  // aktywuje kontekst tworzenia grafiki
  if(!wglMakeCurrent(g_hdc, g_hrc))
	{
    
		KillWindow();
		MessageBox(NULL,"Nie mo¿na aktywowaæ kontekstu grafiki OpenGL", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

  PFNGLQUERYSTEREOEXTPROC pfnQueryStereo;
  PFNGLDISABLESTEREOEXTPROC pfnDisableStereo;
  
  GLboolean sS = 0;
  LPCSTR str = (const char*)glGetString(GL_EXTENSIONS);
  glGetBooleanv(GL_STEREO, &sS);

  //if (!sS) exit(1);
  
  

  // aktywuje okno
  ShowWindow(g_hwnd, SW_SHOW);
	SetForegroundWindow(g_hwnd);
	SetFocus(g_hwnd);

  // zmienia rozmiary sceny
	ResizeScene(width, height);

  // wykonuje jednorazow¹ inicjacjê
  if (!InitializeScene())
	{
    		KillWindow();
		MessageBox(NULL, "Inicjacja nie powiod³a siê", "B³¹d", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

  return TRUE;
} // SetupWindow()


/****************************************************************************
 InitializeScene()

 Jednorazowa inicjacja parametrów sceny.
*****************************************************************************/
BOOL InitializeScene()
{
  // cieniowanie Gouraud
  glClearColor(0.2,0.2,0.2,0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // informuje OpenGL, ¿e powinien u¿ywac najlepszego algorytmu antialiasingu
  glHint(GL_POINT_SMOOTH, GL_NICEST);
  glHint(GL_LINE_SMOOTH, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH, GL_NICEST);

  // pobiera najmniejsz¹ i najwiêksz¹ wielkoœæ punktu
  // oraz krok jej zmiany
  glGetFloatv(GL_POINT_SIZE_RANGE, g_pointSizeMinMax);
  glGetFloatv(GL_POINT_SIZE_GRANULARITY, &g_pointSizeStep);

  // pobiera najmniejsz¹ i najwiêksz¹ szerokoœæ odcinka
  // oraz krok jej zmiany
  return TRUE;
} // InitializeScene()


GLvoid ResizeScene(GLsizei width, GLsizei height)
{
  /*glViewport(100, 100, width-100, height-100);

  g_width = width;
  g_height = height;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, width, 0, height);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();*/
  g_width = width;
  g_height = height;
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1., 1., -1., 1., 1.5, 2000.);
  glMatrixMode(GL_MODELVIEW);
} // ResizeScene()

void render_polys(GLdouble rr, GLdouble gg, GLdouble bb)
{
 GLint i,j;
 //want_tex = want_light = 0;
 //printf("dupa: %d %d\n", want_tex, want_light);
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
      glColor3d( ((j%3)==0)?1.:0., ((j%3)==1)?1.:0., ((j%3)==2)?1.:0.);
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

void disable_light()
{
   glDisable(GL_LIGHT0);
   glDisable(GL_LIGHT1);
   glDisable(GL_LIGHTING);
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


BOOL DisplayScene()
{
  //glClear(GL_COLOR_BUFFER_BIT);
 //glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (want_tex)
    {
     glEnable(GL_BLEND);
     glBindTexture(GL_TEXTURE_2D, tList[0].id);
     /* glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);*/
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
  if (want_light) enable_light();
  else disable_light();
   glEnable(GL_DEPTH_TEST);  //fix
   //glEnable(GL_CULL_FACE);   //fix
  /*glPolygonMode(GL_FRONT, GL_LINE);
  glPolygonMode(GL_BACK, GL_LINE);*/
   
  glLoadIdentity();
  gluLookAt(-eye, 0., obsD, -eye , 0., 0., 0., 1., 0.);
  glScaled(scalef, scalef, scalef);
  glRotatef(angX, 1., 0. , 0.);
  glRotatef(angY, 0., 1. , 0.);
  glRotatef(angZ, 0., 0. , 1.);
  render_polys(1., 0., 0.);
  
  glLoadIdentity();
  gluLookAt(eye, 0., obsD, eye , 0., 0., 0., 1., 0.);
  glScaled(scalef, scalef, scalef);
  glRotatef(angX, 1., 0. , 0.);
  glRotatef(angY, 0., 1. , 0.);
  glRotatef(angZ, 0., 0. , 1.);
  render_polys(0., 1., 0.);
  
  if (auto_move)
   {
    angX += 2.3;
    angY += 2.5;
    angZ += 2.7;
   }
  glFlush();
} // DisplayScene()


BOOL KillWindow()
{
  if (g_isFullscreen)
  {
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(TRUE);
  }

  if (g_hrc)
  {
    if (!wglMakeCurrent(NULL,NULL))
    {
      MessageBox(NULL, "Nie mo¿na zwolniæ kontekstu grafiki", "B³¹d", MB_OK | MB_ICONINFORMATION);
    }

    if (!wglDeleteContext(g_hrc))
    {
      MessageBox(NULL, "Nie mo¿na usun¹æ kontekstu grafiki", "B³¹d", MB_OK | MB_ICONINFORMATION);
    }

    g_hrc = NULL;
  }

  if (g_hdc && !ReleaseDC(g_hwnd, g_hdc))
  {
    MessageBox(NULL, "Nie mo¿na zwolniæ kontekstu urz¹dzenia", "B³¹d", MB_OK | MB_ICONINFORMATION);
    g_hdc = NULL;
  }

  if (g_hwnd && !DestroyWindow(g_hwnd))
  {
    MessageBox(NULL, "Nie mo¿na usun¹æ okna", "B³¹d", MB_OK | MB_ICONINFORMATION);
    g_hwnd = NULL;
  }

  if (!UnregisterClass(WND_CLASS_NAME, g_hInstance))
  {
    MessageBox(NULL, "Nie mo¿na wyrejestrowaæ klasy okna", "B³¹d", MB_OK | MB_ICONINFORMATION);
    g_hInstance = NULL;
  }

  return TRUE;
} // KillWindow()


