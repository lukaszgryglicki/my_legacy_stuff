#include <windows.h>

extern TCHAR path[256];

//otwiera plik wybrany z okna std, sciezka ->path
//nie pobrano nazwy ->2
//nie otwarto pliku ->1
//sukces            ->0
//-------------------------------------------------------------------
int OtworzPlik(HWND hWnd)
{
	static OPENFILENAME ofn;
	static TCHAR Filter[]=TEXT("plik tekstowy (*.txt)\0*.txt\0") \
						  TEXT("wszystkie pliki (*.*)\0*.*\0\0");
	static TCHAR plik2[256];

	_stprintf(plik2, "\0");
	ofn.lStructSize=sizeof(OPENFILENAME); 
	ofn.hwndOwner=hWnd; 
	ofn.lpstrFilter=Filter; 
	ofn.lpstrFile=plik2; 
	ofn.nMaxFile=MAX_PATH; 
	ofn.lpstrFileTitle=path; 
	ofn.nMaxFileTitle=MAX_PATH; 
	ofn.Flags=OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;

	if(GetSaveFileName(&ofn))
	{
		strcpy(path,ofn.lpstrFile);
		if (plik=fopen(path,"r")) return 0;
		else return 1;
	}
	else return 2;
}
//-------------------------------------------------------------------