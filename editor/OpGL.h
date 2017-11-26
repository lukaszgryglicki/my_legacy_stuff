#ifndef OPGL_VNIRUA7I4Y39T87GJ09H54Y60954W
#define OPGL_VNIRUA7I4Y39T87GJ09H54Y60954W

class OpGL
{
	HDC hdc;	//uchwyt kontekstu urzadzenia, na ktorym rysuje OpenGL
	HGLRC hrc;	//uchwyt kontekstu renderowania OpenGL

	void SetDCPixelFormat();	//ustawia format rysowania pikseli na kontekscie urzadzenia
								//(odpowiedni dla OpenGL z podwojnym buforem)
	void SetupRC();	//ustawia swiatla w scenie i parametry renderowania
public:
	OpGL(HWND hWnd);	//hWnd jest uchwytem okna, w ktorym rysuje OpenGL
	void CALLBACK ChangeSize(GLsizei w, GLsizei h);	//zmienia rozmiar obszaru, w ktorym rysuje OpenGL
	void CALLBACK RenderScene(Scene &scene);	//odrysowywuje zawartosc sceny
	~OpGL();
};

#endif
