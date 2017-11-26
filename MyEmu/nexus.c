#include <xmwnd.h>

static char cmdline[0x200];

bool DisplayROM(char* nname)
{
 char name[STRING];
 strcpy(name, nname);
 downcase(name);
 int len = strlen(name);
 if (len<4) return false;
 if ((name[len-1]=='s')&&(name[len-2]=='e')&&(name[len-3]=='n')&&(name[len-4]=='.')) return true;
 else return false;
}

char** ReadFolderToTable(int& n, char* kat)
{
 int num = 0;
 DIR* dp;
 struct dirent* wpis;
 struct stat statbufor;
 if ((dp = opendir(kat)) == NULL) return NULL;
 chdir(kat);
 while ((wpis = readdir(dp)) != NULL)
        {
         stat(wpis->d_name, &statbufor);
         if ((!S_ISDIR(statbufor.st_mode))&&(DisplayROM(wpis->d_name))) num++;
        }
 chdir("..");
 closedir(dp);
 if (!num)
   {
    n = 0;
    return NULL;
   }
 char** tmp;
 tmp = new char*[num+1];
 if (!tmp) panic("memory_exception_in_new: tmp");
 for (int i=0;i<num;i++)
   {
    tmp[i] = new char[TINY_TXT];
    if (!tmp[i]) panic("memory_exception_in_new: tmp[]");
   }
 num = 0;
 if ((dp = opendir(kat))==NULL) return NULL;
 chdir(kat);
 while ((wpis = readdir(dp)) != NULL)
        {
         stat(wpis->d_name, &statbufor);
         if ((!S_ISDIR(statbufor.st_mode))  && (DisplayROM(wpis->d_name)))
	   {
	    strcpy(tmp[num], wpis->d_name);
	    num++;
	   }
        }
 chdir("..");
 closedir(dp);
 n = num;
 return tmp;
}

void vFPS(Wnd& w, void* vSync)
{
 int sync = (int)vSync;
 w.DoQuit();
 w.Invalidate();
 if (sync<5) { strcat(cmdline," 125 "); return; }
 char cat[100];
 sprintf(cat," %d ", sync);
 strcat(cmdline, cat);
}

void vWin(Wnd& w, void* vSync)
{
 int sync = (int)vSync;
 w.DoQuit();
 w.Invalidate();
 char cat[100];
 sprintf(cat," %d ", sync);
 strcat(cmdline, cat);
}

void SelectFPSFunc(Wnd& wnd, void* ptr)
{
 Wnd* wybor = new Wnd(wnd.RetDspStr(), 170, 400);
 if (!wybor) panic("memory exception in creating win_child!");
 wybor->WinDefaults("Klatek na sekunde:");
 wybor->SetButtonsNumber(13);
 wybor->CreateButton(0, "5  FPS" ,35  , 5, vFPS, false, 100, 25,  (void*)(5));
 wybor->CreateButton(1, "10 FPS" ,35 , 35, vFPS, false, 100, 25,  (void*)(10));
 wybor->CreateButton(2, "12 FPS" ,35 , 65, vFPS, false, 100, 25,  (void*)(12));
 wybor->CreateButton(3, "15 FPS" ,35 , 95, vFPS, false, 100, 25,  (void*)(15));
 wybor->CreateButton(4, "18 FPS" ,35 , 125, vFPS, false, 100, 25, (void*)(18));
 wybor->CreateButton(5, "20 FPS" ,35 , 155, vFPS, false, 100, 25, (void*)(20));
 wybor->CreateButton(6, "22 FPS" ,35 , 185, vFPS, true, 100, 25,  (void*)(22));
 wybor->CreateButton(7, "25 FPS" ,35 , 215, vFPS, false, 100, 25, (void*)(25));
 wybor->CreateButton(8, "30 FPS" ,35 , 245, vFPS, false, 100, 25, (void*)(30));
 wybor->CreateButton(9, "35 FPS" ,35 , 275, vFPS, false, 100, 25, (void*)(35));
 wybor->CreateButton(10, "40 FPS" ,35 , 305, vFPS, false, 100, 25, (void*)(40));
 wybor->CreateButton(11, "50 FPS" ,35 , 335, vFPS, false, 100, 25, (void*)(50));
 wybor->CreateButton(12, "MAX CPU" ,35 , 365, vFPS, false, 100, 25, (void*)(1));
 wybor->DefaultResponse();
 wnd.Invalidate();
}

void SelectWinSize(Wnd& wnd, void* ptr)
{
 Wnd* wybor = new Wnd(wnd.RetDspStr(), 170, 400);
 if (!wybor) panic("memory exception in creating win_child!");
 wybor->WinDefaults("Wielkosc okna:");
 wybor->SetButtonsNumber(5);
 wybor->CreateButton(0, "Normalne"    ,35  , 5,  vWin, false, 100, 25,  (void*)(1));
 wybor->CreateButton(1, "  X 2"       ,35 , 35,  vWin, false, 100, 25,  (void*)(2));
 wybor->CreateButton(2, "  X 3"       ,35 , 65,  vWin, true, 100, 25,   (void*)(3));
 wybor->CreateButton(3, "  X 4"       ,35 , 95,  vWin, false, 100, 25,  (void*)(4));
 wybor->CreateButton(4, "Pelny Ekran" ,35 , 125, vWin, false, 100, 25,  (void*)(10));
 wybor->DefaultResponse();
 wnd.Invalidate();
}

void OpenROMFunc(Wnd& wnd, void* arg)
{
 strcpy(cmdline,"/data/bin/bsd/pegasus ");
 int err = chdir("/data/pkgs/NES/roms");
 if (err) { wnd.CreateMBoxChild("Nie moge zmienic katalogu na \n/data/pkgs/NES/roms"); return ; }
 tout("PDFunc");
 int num = 0;
 char** table = ReadFolderToTable(num, "/data/pkgs/NES/roms");
 if (!table)
   {
    wnd.CreateMBoxChild("Brak ROM'ow");
    return;
   }
 if (!num)
   {
    wnd.CreateMBoxChild("Brak plikow w folderze: /data/pkgs/NES/roms");
    return ;
   }
 FSort(table, num);
 char* selected = wnd.CreateCheckBoxVert("Wybierz postac:", table, num);
 for (int i=0;i<num;i++) if (table[i]) delete [] table[i];
 if (table ) delete [] table;
 strcat(cmdline,"'");
 strcat(cmdline,selected);
 strcat(cmdline,"'");
 SelectFPSFunc(wnd, NULL);
 SelectWinSize(wnd, NULL);
 printf("CMD: %s\n", cmdline);
 system(cmdline);
 strcpy(cmdline,"/data/bin/bsd/pegasus ");
}

int main(int lb, char** par)
{
  Dsp* main_dsp = new Dsp;
  if (!main_dsp) panic("memory_exception_in_new",__LINE__,__FILE__);
  Wnd* wnd = new Wnd(main_dsp, 200, 200);
  if (!wnd) panic("memory_exception_in_new",__LINE__,__FILE__);
  wnd->CreateGC();
  wnd->SetFont("*-helvetica-*-12-*");
  wnd->SetCaption("Pegasus");
  wnd->DeclareResponseTable(MEDI_RESPONSES);
  wnd->SelectExposeFunc(StdExposeFunc);
  wnd->SelectConfigureFunc(DenyResizeFunc);
  wnd->SelectKeyPressFunc(eXtendedKeyPressQ);
  wnd->SelectKeyReleaseFunc(eXtendedKeyRelease);
  wnd->SelectButtonPressFunc(eXtendedButtonPress);
  wnd->SelectButtonReleaseFunc(eXtendedButtonRelease);
  wnd->PrepareWndText("");
  wnd->StdTextWrite();
  wnd->SetButtonsNumber(1);
  wnd->CreateButton(0, "Otworz ROM",10 , 10, OpenROMFunc, true, 180, 180);
  wnd->DefaultResponse();
  if (wnd) delete wnd;
  if (main_dsp) delete main_dsp;
}

