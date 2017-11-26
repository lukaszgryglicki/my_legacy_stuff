// PixelShaderLight.cpp : Defines the entry point for the application.
#include "stdafx.h"
#include "PixelShaderLight.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL/glut.h"		// Dodatkowe bajery OpenGL typu kule, torrusy itp.
#include <Cg/cg.h>			// Biblioteka Cg (obs³uga shaderów pixeli i vertexów)
#include <Cg/cgGL.h>		// Obs³uga Cg w OpenGL

#include "matrix.h"			// Przekszta³cenia kamery

// Struktura do odczytu textury w formacie BMP z pliku "texture.bmp"
// Jest to po prostu fizyczny nag³owek plików BMP systemu windows
typedef struct _BMPTag		
{
 char ident[2];				// BM
 int fsize;
 int dummy;
 int offset;
 int dummy2;
 int bm_x;					// wymiary x i y, textury musz¹ mieæ wymiary bêd¹ce potêgami 2, np.128x128 czy 512x256 itp
 int bm_y;					// inaczej ani opengl ani directx nie wczyta takich textur do pamiêci karty graficznej
 short planes;
 short bpp;					// uproszczona obs³uga, tylko 24bitowe pliki BMP
 int compress;				// skompresowane te¿ nie s¹ obs³ugiwane
 int nbytes;
 int no_matter[4];
} BMPTag;


static CGcontext cg_context;					// konteks Cg
static CGprofile v_s_profile, p_s_profile;		// profile shaderów odpowiednio v_s (vertexshader) i p_s (pixelshader)
static CGprogram v_s_program, p_s_program;		// programy w/w profili
// parametry shaderów
// shader wierzcho³ków (vertex), - macierze przekszta³ceñ œwiata w celu obliczenia po³o¿enia (przekszta³cenia i rzutowanie)
static CGparameter v_s_view_matrix, v_s_proj_matrix, v_s_timer;
// parametry shadera pixeli (oœwietlenie liczone dla ka¿dego pixela), g³ównie parametry oœwietlenia
static CGparameter p_s_global_ambient, p_s_light_color, p_s_light_pos;
static CGparameter p_s_eye_pos, p_s_emission, p_s_ambient, p_s_diffuse, p_s_specular, p_s_shine;

static float light_angle = -0.4;		// k¹ty obrotów œwiat³a, kr¹¿y sobie ono w ob³êdny sposób po sferze o=(0,0,0), r=3
static float light_angle2 = -0.7; 
static float global_ambient[3] = { 0.1, 0.1, 0.1 };  /* Dim */
static float light_color[3] = { 0.95, 0.95, 0.95 };  /* White */

// zmienne globalne programu
float angX, angY, angZ;			// obrót kamer wokól osi OX, OY, OZ
float disX, disY, disZ;			// przesuniêcie kamery z punktu (0,0,0) w przestrzeni 3D
float zoom;						// zoom kamery

//Zmienne u¿ywane do obliczenia iloœci klatek na sekundê
time_t t1,t2;
int fps;

// kolejne bity obrazu tekstury (format RGB), ka¿dy kolejny bajt to nasycenie R lub G lub B 0-255
unsigned char* bits;
// ID textury w GPU
GLuint tid;

// Rozmiar textury
long bmx, bmy;
// Czy przypisano texturê (aby zapewniæ ¿e przypisanie odbywa siê tylko raz na pocz¹tku)
long tex_bind = 0;

float timer = 0.0;

// Wstawia domyœlne wartoœci do struktury nag³ówka BMP
void init_bmp(BMPTag* b)
{
 int i;
 if (!b) return;
 b->ident[0]='B';
 b->ident[1]='M';
 b->fsize=0;
 b->dummy=0;
 b->offset=sizeof(BMPTag);
 b->bm_x=b->bm_y=0x20;
 b->dummy2=40;
 b->planes=0;
 b->bpp=0x18;
 b->planes=1;
 b->compress=0;
 b->nbytes=3*32*32;
 for (i=0;i<4;i++) b->no_matter[i]=0;
}

// Wczytujê bitmape do zmiennej 'bits' z pliku podanego jako parametr
int load_bmp(char* fn)
{
 FILE* plik;
 int i;
 char b,m;
 BMPTag bm_handle;
 plik = fopen(fn, "rb");		// otwiera plik w trybie binarnym
 if (!plik) return 0;
 init_bmp(&bm_handle);
 i = fscanf(plik,"%c%c",&b,&m);	// sprawdzanie czy to plik BMP
 if (i != 2) return 0;
 if (b != 'B' || m != 'M') return 0;
 fread(&bm_handle.fsize,4,1,plik);	// odczyt nag³ówka BMP
 fread(&bm_handle.dummy,4,1,plik);
 fread(&bm_handle.offset,4,1,plik);
 fread(&bm_handle.dummy2,4,1,plik);
 fread(&bm_handle.bm_x,4,1,plik);
 fread(&bm_handle.bm_y,4,1,plik);
 fread(&bm_handle.planes,2,1,plik);
 fread(&bm_handle.bpp,2,1,plik);
 if (bm_handle.bpp != 24) return 0;	// czy 24 bit
 fseek(plik,bm_handle.offset,SEEK_SET);	// przeniesienie siê do pocz¹tku pixeli obrazu
 // alokacja pixeli bitmapy
 bits = (unsigned char*)malloc(3*bm_handle.bm_y*bm_handle.bm_x*sizeof(unsigned char));
 bmx = bm_handle.bm_x;	// rozmiar obrazu
 bmy = bm_handle.bm_y;
 fread(bits, 3*bmx*bmy, sizeof(unsigned char), plik);	// odczyt wszystkich pixeli
 printf("Image read (%dx%d).\n", bmx, bmy);
 fclose(plik);	// zamykanie pliku
 return 1;
}

// sprawdza czy nie nast¹pi³ b³¹d biblioteki Cg
static void check_error(const char *descr)
{
  CGerror error;
  char errstr[512];
  const char *string = cgGetLastErrorString(&error);

  if (error != CG_NO_ERROR) 
  {
    sprintf(errstr, "ShaderLighting: %s: %s\n", descr, string);
	MessageBox(NULL, errstr, "Cg Error", MB_OK);
    if (error == CG_COMPILER_ERROR) 
	{
      sprintf(errstr, "%s\n", cgGetLastListing(cg_context));
	  MessageBox(NULL, errstr, "Cg Compile Error", MB_OK);
    }
    exit(1);
  }
}

// Zapowiedzi f-cji obs³ugi zdarzeñ biblioteki GLUT
static void reshape(int width, int height);
static void display(void);
static void keyboard(unsigned char c, int x, int y);
static void idle(void);

// Okienko z klawiszologi¹
void Help()
{
	MessageBox(NULL,	"Sterowanie kamer¹:\n"
						"Obroty:\n"
						"\ti/k - oœ OX\n"
						"\tj/l - oœ OY\n"
						"\to/m - oœ OZ\n"
						"Przemieszczanie:\n"
						"\tw/s - kierunek przód/ty³\n"
						"\ta/d - kierunek lewo/prawo\n"
						"\te/x - kierunek góra/dó³\n"
						"Zoom: 0.8x-5x:\n"
						"\t1 - przybliz (zwiêksz zoom)\n"
						"\t2 - oddal (zmniejsz zoom)\n"
						"Inne:\n"
						"\tq/esc - zakoñcz\n"
						"\tf - pe³ny ekran\n"
						"\tSPACJA - pocz¹tkowe wartoœci"
						,"Informacja", MB_OK);
}

// Wpisanie jakichœ domyslnych wartoœci
void Init()
{
	angX = angY = angZ = 0.;
	disX = disY = 0.;
	disZ = 0.;
	zoom = 1.f;
	//Próba odczytu textury
	if (!load_bmp("texture.bmp")) bits = NULL;
}

// tutaj aplikacja zaczyna dzia³anie
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
  char* name;
  int lb;
  lb = 1;							// iloœæ argumentów z linii poleceñ (w aplikacji wndows nie u¿ywane, sztucznie ustawione na 1)
  name = new char[20];				// jedyny parametr nazwa programu: OpenGL camera
  strcpy(name, "PixelShared Lighting");
  glutInitWindowSize(600, 600);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  // MULTI_SAMLE musi byæ u¿yte bo rózne dane s¹ trzymane w jednostkach texturowania
  // jednostka pierwsza: tex0 (textura)
  // jednostka druga przechowuje vektor pozycji wierzcho³ka (bo POSITION niedostêpne w pixelshaderach)
  // jednostka trzecia przechowuje normalne
  // Wiêc do PS trafia na wejœcie interpolowane: tex0(textura), tex1(po³o¿enie akt. pixela w przestrzeni)
  // oraz tex2( œrednia normalna do tego pixela w przestrzeni) - wszystko co potrzebne do obliczenia œwiat³a
  glutInit(&lb, &name);

  Init();
  glutCreateWindow("ShaderLighting");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  glClearColor(0.1, 0.1, 0.1, 0);  /* Kolor t³a: szary */
  glEnable(GL_DEPTH_TEST);         /* Test g³êbi, wykluczanie zas³oniêtych pixeli z przetwarzania */

  cg_context = cgCreateContext();	// stworz kontekst Cg
  check_error("creating context");
  cgGLSetDebugMode(CG_FALSE);		// wy³¹cz debugowanie
  cgSetParameterSettingMode(cg_context, CG_DEFERRED_PARAMETER_SETTING);	// zezwol na zmiany parametrów w trakcie renderingu

  v_s_profile = cgGLGetLatestProfile(CG_GL_VERTEX);	//znajdŸ najnowszy profil wierzcho³ków obs³ugiwany przez aktualn¹ kartê graficzn¹
  cgGLSetOptimalOptions(v_s_profile);				// i ustaw lub sprawdŸ b³êdy
  check_error("selecting vertex profile");

  // zadaj program karcie, program VertexShader.cg(plik), f-cja main
  // program ten bêdzie wykonany zawsze gdy GPU przetwarza wierzcho³ek (glVertex*)
  v_s_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "VertexShader.cg", v_s_profile, "main", NULL);
  check_error("creating vertex program from file");
  cgGLLoadProgram(v_s_program);
  check_error("loading vertex program");

  // Pobierz uchwyty do parametrów wewnêtrznych programu Cg
  v_s_view_matrix = cgGetNamedParameter(v_s_program, "matrixView");
  check_error("could not get modelView parameter");

  v_s_proj_matrix = cgGetNamedParameter(v_s_program, "matrixProj");
  check_error("could not get modelProj parameter");

  v_s_timer = cgGetNamedParameter(v_s_program, "timer");
  check_error("could not get timer parameter");

  // j.w. - znajdŸ najnowszy profil pixel-shadera i aktywuj go
  p_s_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
  cgGLSetOptimalOptions(p_s_profile);
  check_error("selecting fragment profile");

  // zadaj program karcie, program PixelShader.cg(plik), f-cja main
  // program ten bêdzie wykonany zawsze gdy GPU przetwarza pixel (czyli przy ostatecznym rysowaniu bufora na ekranie)
  // programy z PS wykonuj¹ siê czêœciej: bo np. 1024x768 = oko³o 770,000 na klatkê, czyli przy 100FPS mamy 77 milionów
  // razy na sekundê!
  // program VS wykonuje siê tyle razy ile jest wierzcho³ków na scenie, czyli na ogó³ duuuu¿o mniej, w tym przyk³adzie 
  // jest ich maksymalnie kilka tysiêcy
  // jednak obliczanie oœwiuetlenia dla ka¿dego pixela ma sens (bo ka¿dy ma dok³adnie takie oœwietlenie jakie jest obliczone
  // jeœli byœmy to zrobilki na poziomie wierzcho³ków to tylko wierzcho³ki miua³yby obliczone oœwietlenie a wszêdzie 
  // indziej by³aby œrednia z wierzcho³ków danego trójk¹ta, czyli np. moznaby pomin¹æ rozb³yski itp.
  p_s_program = cgCreateProgramFromFile(cg_context, CG_SOURCE, "PixelShader.cg", p_s_profile, "main", NULL);

  check_error("creating fragment program from string");
  cgGLLoadProgram(p_s_program);
  check_error("loading fragment program");

  // Zadaj parametry programu fragmentów (pixeli)
  // globalne rozproszone œwiat³o
  p_s_global_ambient = cgGetNamedParameter(p_s_program, "globalAmbient");
  check_error("could not get globalAmbient parameter");

  p_s_light_color = cgGetNamedParameter(p_s_program, "lightColor");
  check_error("could not get lightColor parameter");

  p_s_light_pos = cgGetNamedParameter(p_s_program, "lightPosition");
  check_error("could not get lightPosition parameter");

  p_s_eye_pos = cgGetNamedParameter(p_s_program, "eyePosition");
  check_error("could not get eyePosition parameter");

  // parametry œwiat³a kolejno: emisja, otoczenia, rozproszone, odbicia, wspó³czynnik rozbicia
  p_s_emission = cgGetNamedParameter(p_s_program, "emission");
  check_error("could not get emission parameter");

  p_s_ambient = cgGetNamedParameter(p_s_program, "ambient");
  check_error("could not get ambient parameter");

  p_s_diffuse = cgGetNamedParameter(p_s_program, "diffuse");
  check_error("could not get diffuse parameter");

  p_s_specular = cgGetNamedParameter(p_s_program, "specular");
  check_error("could not get apecular parameter");

  p_s_shine = cgGetNamedParameter(p_s_program, "shininess");
  check_error("could not get shininess parameter");


  // Ustaw sta³e parametry œwiat³a - tylko raz
  cgSetParameter3fv(p_s_global_ambient, global_ambient);
  cgSetParameter3fv(p_s_light_color, light_color);

  glutMainLoop();
  return 0;
}


// Zmiana rozmiaru okna
static void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);	/* widok od (0,0) do (w,h) */
  glMatrixMode(GL_PROJECTION);				// tryb macierzy rzutowania
  glLoadIdentity();
  glFrustum(-1., 1., -1., 1., zoom, 2000.);	// prespektywa (œciety graniastos³up) x: -1->1, y: -1->1, z: 1->2000
  glMatrixMode(GL_MODELVIEW);				// tryb widoku sceny
}

// Ustaw "chropowaty" materia³
static void set_material_sharp(void)
{
  const float sharpEmissive[3] = {0.0,  0.0,  0.0},
              sharpAmbient[3]  = {0.33, 0.22, 0.03},
              sharpDiffuse[3]  = {0.78, 0.57, 0.11},
              sharpSpecular[3] = {0.99, 0.91, 0.81},
              sharpShininess = 27.8;

  // Ustawia parametry materia³u w pixel-shaderze
  cgSetParameter3fv(p_s_emission, sharpEmissive);
  check_error("setting emission parameter");
  cgSetParameter3fv(p_s_ambient, sharpAmbient);
  check_error("setting ambient parameter");
  cgSetParameter3fv(p_s_diffuse, sharpDiffuse);
  check_error("setting diffuse parameter");
  cgSetParameter3fv(p_s_specular, sharpSpecular);
  check_error("setting specular parameter");
  cgSetParameter1f(p_s_shine, sharpShininess);
  check_error("setting shininess parameter");
}


// Ustaw materia³ pod³o¿a
static void set_material_ground(void)
{
  const float groundEmissive[3] = {0.05,  0.05,  0.05},
              groundAmbient[3]  = {0.22, 0.11, 0.05},
              groundDiffuse[3]  = {0.36, 0.89, 0.32},
              groundSpecular[3] = {0.12, 0.27, 0.99},
              groundShininess = 13.1;

  cgSetParameter3fv(p_s_emission, groundEmissive);
  check_error("setting emission parameter");
  cgSetParameter3fv(p_s_ambient, groundAmbient);
  check_error("setting ambient parameter");
  cgSetParameter3fv(p_s_diffuse, groundDiffuse);
  check_error("setting diffuse parameter");
  cgSetParameter3fv(p_s_specular, groundSpecular);
  check_error("setting specular parameter");
  cgSetParameter1f(p_s_shine, groundShininess);
  check_error("setting shininess parameter");
}

// Ustaw materia³: "plastik"
static void set_material_plastic(void)
{
  const float redPlasticEmissive[3] = {0.0,  0.0,  0.0},
              redPlasticAmbient[3]  = {0.0, 0.0, 0.0},
              redPlasticDiffuse[3]  = {0.5, 0.0, 0.0},
              redPlasticSpecular[3] = {0.7, 0.6, 0.6},
              redPlasticShininess = 32.0;

  cgSetParameter3fv(p_s_emission, redPlasticEmissive);
  check_error("setting emission parameter");
  cgSetParameter3fv(p_s_ambient, redPlasticAmbient);
  check_error("setting ambient parameter");
  cgSetParameter3fv(p_s_diffuse, redPlasticDiffuse);
  check_error("setting diffuse parameter");
  cgSetParameter3fv(p_s_specular, redPlasticSpecular);
  check_error("setting specular parameter");
  cgSetParameter1f(p_s_shine, redPlasticShininess);
  check_error("setting shininess parameter");
}

// Ustaw materia³ œwiec¹cego œwiat³a (widoczne jako ma³a kulka)
static void set_emmission_light(void)
{
  const float zero[3] = {0.0,  0.0,  0.0};

  cgSetParameter3fv(p_s_emission, light_color);
  check_error("setting Ke parameter");
  cgSetParameter3fv(p_s_ambient, zero);
  check_error("setting Ka parameter");
  cgSetParameter3fv(p_s_diffuse, zero);
  check_error("setting Kd parameter");
  cgSetParameter3fv(p_s_specular, zero);
  check_error("setting Ks parameter");
  cgSetParameter1f(p_s_shine, 0);
  check_error("setting shininess parameter");
}

// Oblicza FPS: zlicza wykonane klatki do czasu a¿ aktualna sekunda siê zmieni
void time_counter()
{
 char tstr[0x100];
 if (t1 == (time_t)0)	// pocz¹tkjowa sytuacja
   {
    time(&t1);
    time(&t2);
    return;
   }
 fps++;
 time(&t2);
 if (t2 > t1)			// czy od ostatniej klatki zmieni³a siê sekunda?
   {
	   sprintf(tstr, "ShaderLighting, FPS: %d", 
		   fps/(int)(t2-t1));
    t1 = t2;
    glutSetWindowTitle(tstr);
    fps = 0;
   }

   timer += 0.01;
}

// Jeœli odczytano texturê z pliku to ustaw j¹ w GPU
void BindTex()
{
 if (tex_bind) return;
 if (bits)
	{
		glEnable(GL_TEXTURE_2D);			// w³¹cz texturowanie 2D
		glGenTextures(1, &tid);				// wygeneruj ID textury
		glBindTexture(GL_TEXTURE_2D, tid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// sposób interpolacji texeli
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// interpolacja liniowa - trochê wolniejsze od GL_NEAREST
		// Wczytaj texturê do pamiêci GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmx, bmy, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);
		glBindTexture(GL_TEXTURE_2D, tid);	// w³¹cz texturowanie (ustaw aktualn¹ texturê)
		tex_bind = 1;						// zapisz flagê ¿e texturê ustawiono
	}
}


// G³owna f-cja wyœwietlaj¹ca - wywo³ywana w ka¿dej klatce
static void display(void)
{
  /* World-space positions for light and eye. */
  const float eyePosition[4] = { disX, disY, disZ, 1. };		// pozycja oka
  const float lightPosition[4] = 
  { 3.*sin(light_angle), 3.*sin(light_angle2), 3.*cos(light_angle)*cos(light_angle2), 1 };
  //pozycja œwiat³a ( kr¹czy po sferze o=(0,0,0) i r=3
  float viewMatrix[16], projMatrix[16];	// macierze odpowiednio widoku i rzutowania pobierane ze stanu maszyny openGL

  glMatrixMode(GL_PROJECTION);				// tryb macierzy rzutowania
  glLoadIdentity();
  glFrustum(-1., 1., -1., 1., zoom, 2000.);	// u¿ywaj zoom-u
  glMatrixMode(GL_MODELVIEW);				// tryb widoku sceny
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();											// wyczyœæ macierz przekszta³ceñ
  gluLookAt(0., 0., 5., 0., 0., 0., 0., 1., 0.);			// rzut kamery:
  glRotatef(angX, 1., 0. , 0.);							// obróæ swiat o aktualny obrót
  glRotatef(angY, 0., 1. , 0.);
  glRotatef(angZ, 0., 0. , 1.);
  glTranslatef(disX, disY, disZ);						// przesuñ na miejsce gdzie jest obserwator
  BindTex();

 
  // jak to siê ustawi na 0 to mo¿na zobaczyæ ile shader robi w programie ;-)
  static int use_shader = 1;

  if (use_shader)
  {
	cgGLBindProgram(v_s_program);				// przypisz program VS
	check_error("binding vertex program");

	cgGLEnableProfile(v_s_profile);				// uruchom program	VS
	check_error("enabling vertex profile");

	cgGLBindProgram(p_s_program);
	check_error("binding fragment program");	// przypisz program PS

	cgGLEnableProfile(p_s_profile);
	check_error("enabling fragment profile");	// uruchom program PS

	// pozycje oka i œwiat³a
	cgSetParameter3f(p_s_eye_pos, 0,0,10);
	cgSetParameter3f(p_s_light_pos, lightPosition[0],lightPosition[1], lightPosition[2]);
	cgSetParameter1f(v_s_timer, timer);
  }
 
  glPushMatrix();
	// pozycja i orientacja obiektu
    glTranslatef(-2., 0., 0.);
	glScalef(0.7, 0.7, 0.7);
	glRotatef(-50., 1., 0., 0.);
	glRotatef(20., 0., 1., 0.);

	if (use_shader)
	{
		// materia³ "chropowaty" (dla PS)
		set_material_sharp();
		// ustawienie macierzy widoku i rzutu dla VS
		glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
		glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
		cgSetMatrixParameterfr(v_s_view_matrix, viewMatrix);
		cgSetMatrixParameterfr(v_s_proj_matrix, projMatrix);
		// Uaktualniej zmienne programu
		cgUpdateProgramParameters(v_s_program);
		cgUpdateProgramParameters(p_s_program);
	}

	// Fili¿anka
	glutSolidTeapot(1.0);
  glPopMatrix();

  
  glPushMatrix();
	glTranslatef(2., 0., 0.);
	glScalef(0.2, 0.2, 0.2);
	glRotatef(-30, .5, .6, .7);
	if (use_shader)
	{
		// materia³ "plastik" dla PS
		set_material_plastic();
		// ustawienie macierzy widoku i rzutu dla VS
		glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
		glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
		cgSetMatrixParameterfr(v_s_view_matrix, viewMatrix);
		cgSetMatrixParameterfr(v_s_proj_matrix, projMatrix);
		cgUpdateProgramParameters(v_s_program);
		cgUpdateProgramParameters(p_s_program);
	}
	// Doœæ dok³adny torrus
	glutSolidTorus(3., 6., 32, 32);
  glPopMatrix();
  


  glPushMatrix();
	glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
	if (use_shader)
	{
		// materia³ "œwiat³o" dla PS
		set_emmission_light();
		// ustawienie macierzy widoku i rzutu dla VS
		glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
		glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
		cgSetMatrixParameterfr(v_s_view_matrix, viewMatrix);
		cgSetMatrixParameterfr(v_s_proj_matrix, projMatrix);
		cgUpdateProgramParameters(v_s_program);
		cgUpdateProgramParameters(p_s_program);  
	}
	//Sfera ma³o dok³adna (bo to i tak œwiat³o bia³ego koloru)
	glutSolidSphere(.1, 8, 8);
  glPopMatrix(); 


  if (use_shader)
  {
	// materia³ "pod³o¿e" dla PS
	set_material_ground();
	// ustawienie macierzy widoku i rzutu dla VS
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
	glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
	cgSetMatrixParameterfr(v_s_view_matrix, viewMatrix);
	cgSetMatrixParameterfr(v_s_proj_matrix, projMatrix);
	cgUpdateProgramParameters(v_s_program);
	cgUpdateProgramParameters(p_s_program);  
  }

  // Pod³o¿e -> czworobok texturowany w PS
  glBegin(GL_QUADS);
	glNormal3f(0., 1., 0.);
	glTexCoord2f(0., 0.);
	glVertex3f(-8., -3.5, -8.);
	glTexCoord2f(1., 0.);
	glVertex3f(8., -3.5, -8.);
	glTexCoord2f(1., 1.);
	glVertex3f(8., -3.5, 8.);
	glTexCoord2f(0., 1.);
	glVertex3f(-8., -3.5, 8.);
  glEnd();

  
  if (use_shader)
  {
    // Wy³¹czanie shaderów
	cgGLDisableProfile(v_s_profile);
	check_error("disabling vertex profile");

	cgGLDisableProfile(p_s_profile);
	check_error("disabling fragment profile");
  }
  

  // Oblicz FPS
  time_counter();
  // Zamieñ bufory (podwójne buforowanie aby zapobiegaæ miganiu obrazu)
  glutSwapBuffers();
}

// Przesuwaj œwiat³o i odnawiaj klatkê animacji
static void idle(void)
{
  light_angle += 0.0081851; 
  light_angle2 -= 0.0063543; 
  if (light_angle > 2*PI) 
  {
    light_angle -= 2*PI;
  }
  if (light_angle2 < 0.) 
  {
    light_angle2 += 2*PI;
  }
  glutPostRedisplay();
}


// Reakcja na klawiaturê, opis w f-cji Help(0 (Klawiszologia)
void keyboard(unsigned char key, int x, int y)
{
 switch (key)
   {
		case 27:  /* Esc key */
		case 'q':
			cgDestroyProgram(v_s_program);
			cgDestroyContext(cg_context);
			exit(0);
			break;
        case 'i': matrix_rotate(-2., 0., 0.); break;	// Rotacje aktualnej sceny
        case 'k': matrix_rotate(2., 0., 0.);  break;
        case 'j': matrix_rotate(0., -2., 0.); break;
        case 'l': matrix_rotate(0., 2., 0.);  break;
        case 'o': matrix_rotate(0., 0., 2.);  break;
        case 'm': matrix_rotate(0., 0., -2.); break;
        case 'w': matrix_translate(0., 0., .1); break;	// translacje kamery (trzeba obliczyæ jaki kierunek to
        case 's': matrix_translate(0., 0., -.1);  break;	// w przód, w ty³, w bok, do góry itp.
        case 'a': matrix_translate(-.1, 0., 0.); break;	// patrz operacje przekszta³ceñ w matrix.cpp i matrix.h
        case 'd': matrix_translate(.1, 0., 0.);  break;
        case 'e': matrix_translate(0., -.1, 0.); break;
        case 'x': matrix_translate(0., .1, 0.);  break;
		case '1': zoom *= 1.03; break;
		case '2': zoom /= 1.03; break;
		case 'h': Help(); break;
		case 'f': glutFullScreen(); break;
		case ' ': angY = angZ = angX = 0.; disX = disY = 0.; disZ = 0.; zoom = 1.; break;	// wartoœci startowe
   }
   if (zoom < 0.8f) zoom = 0.8f;
   if (zoom > 5.0f) zoom = 5.0f;
}

