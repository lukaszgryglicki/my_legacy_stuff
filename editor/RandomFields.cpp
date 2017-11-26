// RandomFields.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Texture.h"
#include "RandomFields.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int px, py;
GLdouble angX, angY, angZ, zdis, fdim,mi;
char filename[MAX_PATH];
char datafile[MAX_PATH];
FILE* plik;
Texture* texture;
int nTex;
int gl_current_type;
int want_random;
int nums;
int t_enabled;
int l_enabled;
int b_enabled;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ConfigProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	SolveProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	AboutProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RANDOMFIELDS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_RANDOMFIELDS);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_RANDOMFIELDS);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;	//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_RANDOMFIELDS;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      100, 100, 800, 600,/*CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,*/ NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

Scene *scene = NULL;

/*double exampleField[4][4] = {
	{0.2, 0.5, 0.1, 0.3},
	{0.1, 0.4, 0.3, 0.1},
	{0.3, 0.8, 0.7, 0.4},
	{0.2, 0.4, 0.2, 0.1}
};*/

void initFile()
{
 //plik = fopen(filename, "w");
	plik = NULL;
 //if (!plik) MessageBox(NULL, "Nie moge zapisac do pliku!\nSprobuj innego pliku lub katalogu", "Blad zapisu", 0);
}

void createRandScene()
{
 scene = new RandField();
 double **field = new double*[px];
 double gauss_var = 7./sqrt((double)(px+py));
 for (int i=0; i<px; i++)
	{
	 field[i] = new double[py];
	 for(int j=0; j<py; j++) //field[i][j] = (((double)rand())*2.0)/(double)RAND_MAX - 8.0;
		 field[i][j] = want_random ? ((double)gaussian()*gauss_var*mi) : 0.;
	}
 ((RandField*)scene)->SetField(field, px, py, -10.0, 10.0, -10.0, 10.0);
 for(int i=0; i<px; i++) delete[] field[i];
 delete[] field;
 field = NULL;
}



void initTextures()
{
 char textureDir[256];
 char fn[256];
 int i;
 FILE* texf;
 int done;
 strcpy(textureDir, "textures");
 done = 0;
 texture = NULL;
 texf = NULL;
 nTex = 0;
 i = 0;
 do
   {
    i++;
    sprintf(fn, "%s\\%d.bmp", textureDir, i);
	texf = fopen(fn, "r");
	if (!texf) { done = 1; i--; }
	else fclose(texf);
   }
 while (!done);
 texture = (Texture*)malloc((i+1)*sizeof(Texture));
 nTex = i;
 for (i=0;i<nTex;i++) 
  {
   sprintf(fn, "%s\\%d.bmp", textureDir, i+1);
   if (!create_texture(&texture[i], fn)) exit(1);
  }
}


void initDefaultScene()
{
 px = py = 4;
 strcpy(filename, "mapa.ccm");
 strcpy(datafile, "dane4x4.txt");
 initFile();
 angY = angZ = 0.;
 zdis = -20.;
 angX = 30.;
 fdim = 2.3;
 nums = 2;
 mi = .33;
 want_random = 1;
 t_enabled = 1;
 l_enabled = 1;
 b_enabled = 1;
 gl_current_type = GL_TRIANGLES;
 initTextures();
 createRandScene();
 //((RandField*)scene)->GenerateRandomField(2.3, 1);
}


void reinitScene()
{
 delete scene;
 createRandScene();
}

void loadScene()
{
 FILE* in;
 in = fopen(datafile, "r");
 if (!in) { MessageBox(NULL, datafile, "Nie moge otworzyc pliku", 0); return; }
 //////
 int tx, ty;
 double ff;
 char buff[128];
 tx = ty = -1;
 ff = 0.;
 fscanf(in, "%d,%d,%lf\n", &tx, &ty, &ff);
 if (tx < 4 || tx > 40 || ty < 4 || ty > 40 || ff < 1. || ff > 3.)
   {
	sprintf(buff, "Zle parametry sceny: <%d,%d,%f>", tx, ty,ff);
    MessageBox(NULL, buff, "Blad", 0);
	fclose(in);
	return;
   }
 px = tx;
 py = ty;
 fdim = ff;
 delete scene;
 scene = new RandField();
 double **field = new double*[px];
 double gauss_var = 3./sqrt((double)(px+py));
 for (int i=0; i<px; i++)
	{
	 field[i] = new double[py];
	 for(int j=0; j<py; j++) 
	   {
		tx = fscanf(in, "%lf,", &field[i][j]); 
		if (tx != 1)
		  {
			  sprintf(buff, "Blad wczytywania elementu siatki: (%d,%d)", i,j);
			  MessageBox(NULL, buff, "Naprawa w toku...", 0);
			  fclose(in);
			  reinitScene();
			  return;
		  }
	   }
	}
 ((RandField*)scene)->SetField(field, px, py, -10.0, 10.0, -10.0, 10.0);
 for(int i=0; i<px; i++) delete[] field[i];
 delete [] field;
 field = NULL;
 fclose(in);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static OpGL *opgl;

	switch (message) 
	{
	case WM_CREATE:
		initDefaultScene();
		opgl = new OpGL(hWnd);
		((RandField*)scene)->GenerateRandomField(fdim, nums);
		break;
	case WM_SIZE:
		opgl->ChangeSize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_DOWN: 
				angX += 3.; 
				if (angX >= 360.) angX -= 360.; 
				break;
			case VK_UP: 
				angX -= 3.; 
				if (angX < 0.) angX += 360.; 
				break;
			case VK_RIGHT: 
				angY += 3.; 
				if (angY >= 360.) angY -= 360.; 
				break;
			case VK_LEFT: 
				angY -= 3.; 
				if (angY < 0.) angY += 360.; 
				break;
			case VK_PRIOR: 
				angZ += 3.; 
				if (angZ >= 360.) angZ -= 360.; 
				break;
			case VK_END: 
				zdis *= 1.03; 
				if (zdis <= -50.) zdis = -50.; 
				break;
			case VK_HOME: 
				zdis /= 1.03; 
				if (zdis >= -4.) zdis = -4.; 
				break;
			case VK_NEXT: 
				angZ -= 3.; 
				if (angZ < 0.) angZ += 360.; 
				break;
			case VK_SPACE:
				if (gl_current_type == GL_TRIANGLES) gl_current_type = GL_LINES;
				else gl_current_type = GL_TRIANGLES;
				InvalidateRect(hWnd, NULL, 1);
				break;
			case VK_ESCAPE:
			case 219:
				((RandField*)scene)->RaiseWater(-0.02);
				break;
			case 191:
				if (t_enabled) glDisable(GL_TEXTURE_2D);
				else glEnable(GL_TEXTURE_2D); 
				t_enabled = ! t_enabled;
				break;
			case 190:
				if (l_enabled) glDisable(GL_LIGHTING);
				else glEnable(GL_LIGHTING); 
				l_enabled = ! l_enabled;
				break;
			case 188:
				if (b_enabled) glDisable(GL_BLEND);
				else glEnable(GL_BLEND); 
				b_enabled = ! b_enabled;
				break;
			case 221:
				((RandField*)scene)->RaiseWater(0.02);
				break;
			case 87:
				((RandField*)scene)->Selected(0, 1);
				break;
			case 83:
				((RandField*)scene)->Selected(0, -1);
				break;
			case 65:
				((RandField*)scene)->Selected(1, 0);
				break;
			case 68:
				((RandField*)scene)->Selected(-1, 0);
				break;
			case 81:
				DestroyWindow(hWnd);
				break;
			case 186:
				((RandField*)scene)->Raise(0.07);
				break;
			case 222:
				((RandField*)scene)->Raise(-0.07);
				break;
			case 57:
				((RandField*)scene)->BigRaise(0.1);
				break;
			case 48:
				((RandField*)scene)->BigRaise(-0.1);
				break;
			case 220:
				((RandField*)scene)->NextGround(1);
				break;
			case 189:
				((RandField*)scene)->NextTex(1);
				break;
			case 187:
				((RandField*)scene)->NextTex(-1);
				break;
			case 13:
				((RandField*)scene)->GenerateRandomField(fdim, nums);
				break;
			default:
				char b[100];
				sprintf(b,"%d", wParam);
				MessageBox(NULL,b,b,0);
				break;
		}
		InvalidateRect(hWnd, NULL, 1);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_SOLVE:
			//DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX2, hWnd, (DLGPROC)SolveProc);
			((RandField*)scene)->GenerateRandomField(fdim, nums);
			InvalidateRect(hWnd, NULL, 1);
			break;
		case IDM_PARAMS:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)ConfigProc);
			InvalidateRect(hWnd, NULL, 1);
			//((RandField*)scene)->GenerateRandomField(2.3, 1);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX1, hWnd, (DLGPROC)AboutProc);
			//((RandField*)scene)->GenerateRandomField(2.3, 1);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		opgl->RenderScene(*scene);
		ValidateRect(hWnd, NULL);
		break;
	case WM_DESTROY:
		delete scene;
		delete opgl;
		if (plik) fclose(plik);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void openFile(HWND hWnd, char* fn)
{
 static OPENFILENAME ofn;
 static TCHAR Filter[]=TEXT("plik tekstowy (*.txt)\0*.txt\0") \
						  TEXT("wszystkie pliki (*.*)\0*.*\0\0");
 static TCHAR plikn[MAX_PATH+1];
 static TCHAR path[MAX_PATH+1];
 _stprintf(plikn, "\0");
 ofn.lStructSize=sizeof(OPENFILENAME); 
 ofn.hwndOwner=hWnd; 
 ofn.lpstrFilter=Filter; 
 ofn.lpstrFile=plikn; 
 ofn.nMaxFile=MAX_PATH; 
 ofn.lpstrFileTitle=path; 
 ofn.nMaxFileTitle=MAX_PATH; 
 ofn.Flags=OFN_HIDEREADONLY;
 if (GetOpenFileName(&ofn)) strcpy(fn,ofn.lpstrFile);
}


void saveFile(HWND hWnd, char* fn)
{
 static OPENFILENAME ofn;
 static TCHAR Filter[]=TEXT("Mapa CAD-illac (*.ccm)\0*.ccm\0") \
						  TEXT("wszystkie pliki (*.*)\0*.*\0\0");
 static TCHAR plikn[MAX_PATH+1];
 static TCHAR path[MAX_PATH+1];
 _stprintf(plikn, "\0");
 ofn.lStructSize=sizeof(OPENFILENAME); 
 ofn.hwndOwner=hWnd; 
 ofn.lpstrFilter=Filter; 
 ofn.lpstrFile=plikn; 
 ofn.nMaxFile=MAX_PATH; 
 ofn.lpstrFileTitle=path; 
 ofn.nMaxFileTitle=MAX_PATH; 
 ofn.Flags=OFN_HIDEREADONLY;
 if (GetSaveFileName(&ofn)) strcpy(fn,ofn.lpstrFile);
 FILE* map;
 map = fopen(fn, "wb");
 if (!map) { MessageBox(NULL, "Cannot write map", "Cannot write map", 0); return; }
 ((RandField*)scene)->SaveCCM(map);
 fclose(map);
}

// Message handler for about box.
LRESULT CALLBACK ConfigProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
 static char buff[256];
 static bool loaded;
 static int tmp;
 static double ff;
	switch (message)
	{
	case WM_INITDIALOG:
		loaded = false;
		sscanf(buff, "%d", &tmp);
		sprintf(buff, "%d", px);
		SendDlgItemMessage(hDlg,IDC_EDIT1,WM_SETTEXT,FALSE,(LPARAM)buff);
		sprintf(buff, "%d", py);
		SendDlgItemMessage(hDlg,IDC_EDIT2,WM_SETTEXT,FALSE,(LPARAM)buff);
		sprintf(buff, "%f", fdim);
		SendDlgItemMessage(hDlg,IDC_EDIT5,WM_SETTEXT,FALSE,(LPARAM)buff);
		sprintf(buff, "%s", filename);
		SendDlgItemMessage(hDlg,IDC_EDIT3,WM_SETTEXT,FALSE,(LPARAM)buff);
		sprintf(buff, "%s", datafile);
		SendDlgItemMessage(hDlg,IDC_EDIT4,WM_SETTEXT,FALSE,(LPARAM)buff);
		return TRUE;

	case WM_COMMAND:
		//if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		switch (LOWORD(wParam))
		{
		case ID_OPENFILE:
			sprintf(buff, "%s", filename);
			SendDlgItemMessage(hDlg,IDC_EDIT3,WM_SETTEXT,FALSE,(LPARAM)buff);
			saveFile(hDlg, filename);
			sprintf(buff, "%s", filename);
			SendDlgItemMessage(hDlg,IDC_EDIT3,WM_SETTEXT,FALSE,(LPARAM)buff);
			initFile();
			break;
		case ID_OPENFILE2:
			sprintf(buff, "%s", datafile);
			SendDlgItemMessage(hDlg,IDC_EDIT4,WM_SETTEXT,FALSE,(LPARAM)buff);
			openFile(hDlg, datafile);
			sprintf(buff, "%s", datafile);
			SendDlgItemMessage(hDlg,IDC_EDIT4,WM_SETTEXT,FALSE,(LPARAM)buff);
			loaded = true;
			loadScene();
			sprintf(buff, "%d", px);
			SendDlgItemMessage(hDlg,IDC_EDIT1,WM_SETTEXT,FALSE,(LPARAM)buff);
			sprintf(buff, "%d", py);
			SendDlgItemMessage(hDlg,IDC_EDIT2,WM_SETTEXT,FALSE,(LPARAM)buff);
			sprintf(buff, "%f", fdim);
			SendDlgItemMessage(hDlg,IDC_EDIT5,WM_SETTEXT,FALSE,(LPARAM)buff);
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDOK_PAR:
			 sprintf(buff, "%f", fdim);
			 SendDlgItemMessage(hDlg,IDC_EDIT1,EM_GETLINE,0,(LPARAM)buff);
			 tmp = 4;
			 sscanf(buff, "%d", &tmp);
			 //sprintf(buff, "%d", IDC_EDIT2);
			 //MessageBox(NULL,buff,buff,0);
			 if (tmp < 4 || tmp > 40)
			   {
			    MessageBox(NULL, "Parametry musza byc\nz przedzialu <4-40>", "Bladne parametry", 0);
				return FALSE;
			   }
		     px = tmp;
			 SendDlgItemMessage(hDlg,IDC_EDIT2,EM_GETLINE,0,(LPARAM)buff);
			 tmp = 4;
			 sscanf(buff, "%d", &tmp);
			 //MessageBox(NULL,buff,buff,0);
			 if (tmp < 4 || tmp > 40)
			   {
			    MessageBox(NULL, "Parametry musza byc\nz przedzialu <4-40>", "Bladne parametry", 0);
				return FALSE;
			   }
		     py = tmp;
			 SendDlgItemMessage(hDlg,IDC_EDIT5,EM_GETLINE,0,(LPARAM)buff);
			 ff = 2.3;
			 sscanf(buff, "%lf", &ff);
			 //MessageBox(NULL,buff,buff,0);
			 if (ff < 1. || ff > 3.)
			   {
			    MessageBox(NULL, "Dim musi byc\nz przedzialu <1-3>", "Bladne parametry", 0);
				return FALSE;
			   }
		     fdim = ff;
			 EndDialog(hDlg, LOWORD(wParam));
			 //recreate scene!
			 if (!loaded) reinitScene();
			 return TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
	    return TRUE;
		//break;
	}
	return FALSE;
}

LRESULT CALLBACK SolveProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char buff[256];
	int tmp;
	double tmpd;
	switch (message)
	{
	case WM_INITDIALOG:
		if (nums <= 0) nums = 3;
		sprintf(buff, "%d", nums);
		SendDlgItemMessage(hDlg,IDC_NUMS,WM_SETTEXT,FALSE,(LPARAM)buff);
		sprintf(buff, "%f", mi);
		SendDlgItemMessage(hDlg,IDC_MI,WM_SETTEXT,FALSE,(LPARAM)buff);
		if (want_random) SendDlgItemMessage(hDlg,IDC_NUMS2,WM_SETTEXT,FALSE,(LPARAM)("tak"));
		else SendDlgItemMessage(hDlg,IDC_NUMS2,WM_SETTEXT,FALSE,(LPARAM)("nie"));
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		 {
		  case ID_RAND:
			  want_random = !want_random;
			  if (want_random) SendDlgItemMessage(hDlg,IDC_NUMS2,WM_SETTEXT,FALSE,(LPARAM)("tak"));
			  else SendDlgItemMessage(hDlg,IDC_NUMS2,WM_SETTEXT,FALSE,(LPARAM)("nie"));
			  break;
		  case ID_ALG:
			 SendDlgItemMessage(hDlg,IDC_NUMS,EM_GETLINE,0,(LPARAM)buff);
			 tmp = 1;
			 sscanf(buff, "%d", &tmp);
			 if (tmp < 1 || tmp > 8)
			   {
				MessageBox(NULL, "Ilosc krokow musi byc z przedzialu <1,8>", "Blad", 0);
				return FALSE;
			   }
		     nums = tmp;
			 SendDlgItemMessage(hDlg,IDC_MI,EM_GETLINE,0,(LPARAM)buff);
			 tmpd = 1;
			 sscanf(buff, "%lf", &tmpd);
			 if (tmpd < 0. || tmpd > 4.)
			   {
				MessageBox(NULL, "Mnoznik losowosci musi byc z przedzialu <0,4>", "Blad", 0);
				return FALSE;
			   }
		      mi = tmpd;
			  EndDialog(hDlg, LOWORD(wParam));
			  return TRUE;
		  //EndDialog(hDlg, LOWORD(wParam));
		 }
		break;
	case WM_CLOSE:
		nums = 0;
		EndDialog(hDlg, LOWORD(wParam));
	    return TRUE;
	}
	return FALSE;
}


LRESULT CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK_AB || LOWORD(wParam) == IDCANCEL) 
		{
			 EndDialog(hDlg, LOWORD(wParam));
			 return TRUE;
		}
		break;
	}
	return FALSE;
}
