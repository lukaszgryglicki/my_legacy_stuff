#include "xmwnd.h"
//Projekt Indywidualny, Lukasz Gryglicki    ==> kod
//			Lukasz bainski      ==> projekt
//			Malgorzata Antosik  ==> konspekt
//PLIK XMWND.H jest moja wlasna implementacja
//okien, przyciskow, suwakow, kolorow, zdarzen
//czytaj: biblioteka obiektowa jak MFC :-)
//ma okolo 180kb kodu zrodlowego i jest WYPASIONA
//zalacza: mran.h <moja implementacja losowania na UNIX>
//	   mh.h <zbior roznych drobnych funkcji>
//	   mgraph.h <funkcje rysujace przyciski, ramki itp>
//	   asmqsort.s <implementacja QSort z mediana i Insertion
//	   dla malych przedzialow w UNIX AT&T assemblerze, autor: MorgothDBMA
//	   kompilacja:
//	   zalaczony jest plik ,,Makefile'' sugeruje wpisac w linii polecen
//	   make
//	   Wymagania programu:
//	   	1) FreeBSD UNIX z Gcc 3.x.y lub 2.9x.y <lepiej 3.x>
//	   gdyz 2.9x.y moze nie znalezc pewnych plikow *.h
//	   program NIE_BYL_TESTOWNY na Linux'ie w szczegolnosci
//	   wstawki assemblerowe uzywaja Konwencji BSD przy wolaniu Kernel
//	   STACK: PUSH,POP INT $0X80, na Linux'ie moze NIE dzialac !!!
//	   Linux ma gdzie indziej pewne pliki *.h i inne nazwy
//	   funkcji terminala np. tssetattr <moze sie nie kompilowac>
//	   Program NA _ 100% _ nie skompiluje sie w WIndows i Visual C++
//	   	2)XFree86, nie wiem czy konieczne jest 4.x.y <ja mam>
//	   byc moze zadziala na 3.x.y. Program uzywa wbudowanej glebi kolorow
//	   uzyj #define do definicji (zbedne zakomentuj np)
//	   //#define BIT24DISP
//	   #define BIT16DISP <w pliku xmwnd.h>
//	   	3) Jakikolwiek manager okien nawet TWM
//		4) myszka, klawiatura, RAM (zalecane >=16MB)
//	   aby debugowac aplikacje wpisz:
//	   make debug && ./projectd
//	   aby po prostu uruchomic
//	   make && ./project
//	   	DZIEKUJE tworcy malloc-debugera: ElectricFence 2.0.2
//	   	PROGRAM BAAARDZO POMOGL w wykrywaniu bledow new-delete itp.
#define HLT	mgetch();
#define ATOM	write(1,"a\n",2);
#define TTEXT    0
#define TDATE    1
#define TFLOAT   2
#define TLIST	 3
#define TUNKNOWN 4
#define READOK   0
#define READBAD  1
#define READEOL  2
#define READEOF  3
static bool fopened = false;	//is file opened
static bool screated= false;    //are headers created
static char* fn=NULL;           //filename
static bool scol = false;       //set column separator
static bool sdec = false;	//set floating point separator
static bool stex = false;	//set text separator
static bool sdat = false;	//set DATE separator
static bool fdat = false;	//format of date is set
static bool bprior=false;	//are priorites set
static bool bgrp = false;	//used group
static bool bintv= false;	//used intervals
static bool bsrt = false;	//used sort
static bool borient = false;	//is report vertical
static bool baltn= false;	//used alternative names for columns
char* tscol = NULL;		//table of col separators
char* tstex = NULL;		//table of text separators
char* tsdec = NULL;		//table of FP separators
char* tsdat = NULL;		//table of DATE separators
static int cols = -1;		//number of columns
static int rows = -1;		//number of rows
static int* gtypes = NULL;	//data types in columns
static int* prior  = NULL;	//priorities of given column
static int* pri    = NULL;	//pri[0] = k, means that first is k-column, pri[1]=b
				//b is second etc.
static int* grp    = NULL;	//stores group info
				//0-no, 1-group,2-interval, see below
static int* interv = NULL;	//stores interval lengths by columns
static double* tm  = NULL;	//stores start values of intervals...
static int* srt    = NULL;	//stores sort info
static char** altn = NULL;	//alternative names for columns
static char* title = NULL;	//title of report
static bool back = false;	//going back in creator
static int step = 0;		//current creator step
static int ncols = -1;		//number of seleted (to report) columns
typedef unsigned char BYTE;	//BYTE type
static int month[]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //months lenghts

typedef struct SEPS		//there are separators <names are significant>
{
 SEPS();
 char scol;
 char sdec;
 char stex;
 char sdat;
 BYTE fdat;
};

SEPS :: SEPS()			//init
{
 scol = ' ';
 sdec = ' ';
 stex = ' ';
 sdat = ' ';
 fdat = 0;
 bytecopy(tscol, "#,;", 3);
 bytecopy(tstex, "\"'", 2);
 bytecopy(tsdec, ",.", 2);
 bytecopy(tsdat, "-_", 2);
}

typedef struct SEPS* PSEPS;
static PSEPS seps = NULL;		//our separators here
typedef struct List			//List abstraction <used EVERYWHERE>
{					//I think it WORKS...
 void* data;
 List* next;
 List* prev;
 List* p;
 int t;
};
typedef List* pList;
typedef List** squareList;		//ha, see THIS!
pList hheader = NULL;			//data header
static squareList ppd = NULL;		//MAIN_DBASE_DATA

typedef struct TText			//stores text info
{
 TText();
 ~TText();
 char* data;
 char* text();
 int read(FILE*);
};

char* TText :: text()
{
 return data;
}


int TText :: read(FILE* p)		//reads from file to FSTREAM buffer
{
 if (!p) return READBAD;
 int zn;
 int fp = ftell(p);
 zn = fgetc(p);
 if (zn != seps->stex) { fseek(p, fp, SEEK_SET); return READBAD; }
 int i=0;
 int sret = READOK;
 while ((zn=fgetc(p))!=seps->stex) i++;
 zn = fgetc(p);
 if (zn==EOF)  sret = READEOF;
 if (zn=='\n') sret = READEOL;
 data = new char[i+2];
 fseek(p, fp+1, SEEK_SET);
 i=0;
 while ((zn=fgetc(p))!=seps->stex) { data[i] = zn; i++; }
 data[i] = '\0';
 //printf("RD_TEXT=%s, SRET=%d\n", text(), sret);
 return sret;
}

typedef TText* pText;

typedef struct TDate			//stores DATE info
{
 TDate();
 ~TDate();
 unsigned long data;
 int read(FILE*);
 int analyze(char*);
 char* text();
 int y();
 int m();
 int d();
};
typedef TDate* pDate;

int TDate :: analyze(char* par)		//parses read string
{
 if (!par) return 0;
 int len = strlen(par);
 if (len!=10) return 0;
 char pa[5];
 char pb[3];
 char pc[3];
 int a,b,c;
 if (!seps->fdat)
   {
    if (par[4] != seps->sdat || par[7] != seps->sdat) return 0;
    bytecopy(pa, par, 4, 0); pa[4] = 0;
    bytecopy(pb, par, 2, 5); pb[2] = 0;
    bytecopy(pc, par, 2, 8); pc[2] = 0;
   }
 else
   {
    if (par[2] != seps->sdat || par[5] != seps->sdat) return 0;
    bytecopy(pa, par, 4, 6); pa[4] = 0;
    bytecopy(pb, par, 2, 3); pb[2] = 0;
    bytecopy(pc, par, 2, 0); pc[2] = 0;
   }
 len = sscanf(pa, "%d", &a); if (len<1) return 0;
 len = sscanf(pb, "%d", &b); if (len<1) return 0;
 len = sscanf(pc, "%d", &c); if (len<1) return 0;
 if (a<0 || a>3000) return 0;
 if (b<1 || b>12) return 0;
 if (c<1) return 0;
 if (b!=2 && c>month[b]) return 0;
 if (b==2 && a%4 && c>28) return 0;
 if (b==2 && !(a%4) && c>29) return 0;
 data = 10000*a + 100*b + c;
 return 1;
}


char* ul2pcDate(unsigned long arg)		//makes char* from ulong
{
 char* txt = new char[12];
 sprintf(txt,"%04d-%02d-%02d", (int)(arg/10000), (int)((arg%10000)/100), (int)(arg%100));
 return txt;
}


char* TDate :: text()
{
 char* txt = new char[12];
 sprintf(txt,"%04d-%02d-%02d", y(), m(), d());
 return txt;
}


int TDate :: y()		//operators <not really> Y,M,D
{
 return data/10000;
}


int TDate :: m()
{
 return (data%10000)/100;
}


int TDate :: d()
{
 return data%100;
}


int TDate :: read(FILE* p)		//reads from file
{
 if (!p) return READBAD;
 int zn;
 int fp = ftell(p);
 int i=0;
 int sret = READOK;
 do { zn=fgetc(p); i++; }  while (zn!=seps->scol && zn!=EOF && zn!='\n');
 if (zn==EOF)  sret = READEOF;
 if (zn=='\n') sret = READEOL;
 char* tmp = new char[i+1];
 fseek(p, fp, SEEK_SET);
 int x = 0;
 while (x<i-1) { tmp[x] = fgetc(p); x++; }
 tmp[x] = '\0';
 int ok = analyze(tmp);
 delete [] tmp;
 if (!ok) return READBAD;
 //printf("RD_DATE=%s, SRET=%d\n", text(), sret);
 return sret;
}


typedef struct TFloat			//struct numeric
{
 TFloat();
 ~TFloat();
 double data;
 int read(FILE*);
 int analyze(char*);
 char* text();
};
typedef TFloat* pFloat;

char* TFloat :: text()
{
 char* txt = new char[12];
 sprintf(txt,"%9.4f", data);
 return txt;
}


int TFloat :: analyze(char* par)	//parse read string
{
 if (!par) return 0;
 if (strlen(par)<1) return 0;
 int len = strlen(par);
 int dot=0;
 for (int i=0;i<len;i++) if ((par[i]>'9' || par[i]<'0') && par[i]!='-' && par[i]!='+')
   {
    if (par[i]=='e' || par[i]=='E') continue;
    if (dot) return 0;
    if (par[i]!=seps->sdec) return 0;
    par[i] = '.';
    dot = 1;
   }
 double arg;
 int ki = sscanf(par,"%lf", &arg);
 if (ki<1) return 0;
 data = arg;
 return 1;
}


int TFloat :: read(FILE* p)		//read from file
{
 if (!p) return READBAD;
 int zn;
 int fp = ftell(p);
 int i=0;
 int sret = READOK;
 do { zn=fgetc(p); i++; } while (zn!=seps->scol && zn!=EOF && zn!='\n');
 if (zn==EOF)  sret = READEOF;
 if (zn=='\n') sret = READEOL;
 char* tmp = new char[i+2];
 fseek(p, fp, SEEK_SET);
 int x = 0;
 while (x<i-1) { tmp[x] = fgetc(p); x++; }
 tmp[x] = '\0';
 //printf("TmpFloat=%s\n", tmp);
 int ok = analyze(tmp);
 delete [] tmp;
 if (!ok) return READBAD;
 //printf("RD_FLOAT=%s, SRET=%d\n", text(), sret);
 return sret;
}


TText :: ~TText()
{
 if (data) delete [] data;
 data = NULL;
}


TDate :: ~TDate()
{
 data = 19000101;
}


TFloat :: ~TFloat()
{
 data = 0.00;
}


TText :: TText()
{
 data = NULL;
}


TDate :: TDate()
{
 data = 20000101;
}


TFloat :: TFloat()
{
 data = 0.00;
}


void init_vars()		//STD setup
{
 tscol = new char[3];
 tstex = new char[2];
 tsdec = new char[2];
 tsdat = new char[2];
 seps = new SEPS;
 grp = NULL;
 interv = NULL;
 if (!seps) panic("cannot alocate separators variables");
 srt = NULL;
 grp = NULL;
 altn = NULL;
 title = NULL;
 bgrp = borient = bsrt = false;
}


void free_square_list()			//free memory
{
 //printf("Freeying SquareList\n");
 for (int i=0;i<cols;i++)
   {
    if (ppd[i])
      {
       while (ppd[i]->next) { ppd[i] = ppd[i]->next; delete ppd[i]->prev; }
       delete ppd[i];
       ppd[i] = NULL;
      }
   }
 ppd = NULL;
 //printf("Succeded\n");
}

void delete_vars()			//global DESTRUCTOR, do not use
{					//while program isn't exiting ;-)
 if (tscol)  delete [] tscol;
 if (tstex)  delete [] tstex;
 if (tsdec)  delete [] tsdec;
 if (tsdat)  delete [] tsdat;
 if (seps)   delete seps;
 if (gtypes) delete [] gtypes;
 if (fn)     delete [] fn;
 if (ppd) free_square_list();
 if (prior) delete [] prior;
 if (pri) delete [] pri;
 if (grp) delete [] grp;
 if (interv) delete [] interv;
 if (srt) delete [] srt;
 for (int i=0;i<ncols;i++)
    {
     if (altn[i])
       {
	delete [] altn[i];
	altn[i] = NULL;
       }
    }
 if (altn) delete [] altn;
 altn = NULL;
 if (title) delete [] title;
}


bool DisplayExt(char* nname, char* rrege)		//this extension Display?
{
 tout("DisplayExt");
#ifdef DBG
 char dstr[LONG_STR];
 if (nname) sprintf(dstr,"DisplayExt(%p:%s)",nname,nname);
 else sprintf(dstr,"DisplayExt((char*)NULL)");
 tout(dstr);
#endif
 if (!nname) return false;
 if (!rrege) return true;
 char name[STRING];
 char rege[STRING];
 strcpy(name, nname);
 strcpy(rege, rrege);
 downcase(rege);
 downcase(name);
 if (strstr(name, rege)) return true;
 else return false;
}


char** ReadFolderToTable(int& n, char* kat, char* rege=NULL)	//read directory and return text lines
{
 tout("ReadFolderToTable");
 if (!kat) return NULL;
#ifdef DBG
 char dstr[LONG_STR];
 sprintf(dstr,"ReadFolderToTable(%d,%p:%s) num, directory_name",n,kat,kat);
 tout(dstr);
#endif
 int num = 0;
 DIR* dp;
 struct dirent* wpis;
 struct stat statbufor;
 if ((dp = opendir(kat)) == NULL) return NULL;
 chdir(kat);
 while ((wpis = readdir(dp)) != NULL)
        {
         stat(wpis->d_name, &statbufor);
         if ((S_ISDIR(statbufor.st_mode)) ||  (DisplayExt(wpis->d_name, rege))) num++;
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
 if ((dp = opendir(kat)) == NULL) return NULL;
 chdir(kat);
 while ((wpis = readdir(dp)) != NULL)
        {
         stat(wpis->d_name, &statbufor);
         if ((S_ISDIR(statbufor.st_mode))  || (DisplayExt(wpis->d_name, rege)))
	   {
	    strcpy(tmp[num], wpis->d_name);
	    if (S_ISDIR(statbufor.st_mode)) strcat(tmp[num], "/");
	    num++;
	   }
        }
 chdir("..");
 closedir(dp);
 n = num;
#ifdef DBG
  for (int i=0;i<n;i++)
    {
     sprintf(dstr,"tmp[%d]=%p:%s",i,tmp[i],tmp[i]);
     tout(dstr);
    }
#endif
 return tmp;
}


char* ReadFileToBuffer(Wnd& wnd, char* name)		//Read file into char*
{
 if (!name) return NULL;
 if (name[strlen(name)-1]=='/') return NULL;
 FILE* plik = fopen(name,"r");
 if (!plik)
   {
    wnd.CreateMBoxChild("Nie moge odczytac pliku!");
    return NULL;
   }
 fseek(plik, 0, SEEK_END);
 int flen = ftell(plik);
 if (flen<=1) return NULL;
 fseek(plik, 0, SEEK_SET);
 char* buf = new char[flen+10];
 int zn;
 int i=0;
 while ((zn = fgetc(plik))!=EOF) { buf[i] = zn; i++; }
 buf[i] = '\0';
 fclose(plik);
 return buf;
}


char* f2pc(double arg)			//makes char* from double
{
 char* rt = new char[12];
 sprintf(rt,"%9.5f", arg);
 return rt;
}

char* RetText(pList arg, int type)	//Ret Text from list, use type to determine
{					//type of conversion
 if (!arg) return NULL;
 char* rt;
 if (type!=TTEXT) rt = new char[12];
 else rt = new char[strlen((char*)arg->data)+2];
 switch (type)
  {
   case TTEXT:
	   strcpy(rt, (char*)arg->data);
	   break;
   case TDATE:
	   sprintf(rt,"%s", ul2pcDate(*(unsigned long*)arg->data));		//isn't it beautiful?
	   break;
   case TFLOAT:
	   sprintf(rt,"%s", f2pc(*(double*)arg->data));
	   break;
   case TLIST:
	   sprintf(rt,"pList:%p", arg->data);
	   break;
   default: sprintf(rt, "{UNKNOWN}");
  }
 return rt;
}

char* RetText(void* arg, int type)			//see above, this takes VOID* no pList
{							//only difference
 if (!arg) return NULL;
 char* rt;
 if (type!=TTEXT) rt = new char[12];
 else rt = new char[strlen((char*)arg)+2];
 switch (type)
  {
   case TTEXT:
	   strcpy(rt, (char*)arg);
	   break;
   case TDATE:
	   sprintf(rt,"%s", ul2pcDate(*(unsigned long*)arg));
	   break;
   case TFLOAT:
	   sprintf(rt,"%s", f2pc(*(double*)arg));
	   break;
   case TLIST:
	   sprintf(rt,"pList:%p", arg);
	   break;
   default: sprintf(rt, "{UNKNOWN}");
  }
 return rt;
}

char* ReadListToBuffer(Wnd& wnd)		//Make char* from List pointer
{						//Used to show entire DBase
 int len = 50*(cols+1)*(rows+1)+0x180;
 //printf("len=%d cols=%d, rows=%d\n", len, cols, rows);
 int j = 0;
 char* rt = new char[len+2];
 sprintf(rt,"Wczytana Baza Danych:\n");
 char* buf = new char[((cols>8)?cols:8)*60];
 char* buf2 = new char[((cols>8)?cols:8)*60];
 if (!buf || !buf2) return NULL;
 int ll=0;
 for (int i=0;i<cols;i++)
   {
    //printf("i=%d\n", i);
    sprintf(buf2,"KOLUMNA [%-3d]", i);
    if (gtypes[i]==TTEXT) { ll+=40; sprintf(buf,"%-40s", buf2); }
    else { ll+=18; sprintf(buf,"%-18s", buf2); }
    strcat(rt, buf);
   }
 strcat(rt,"\n");
 for (int x=0;x<ll;x++) buf[x] = '=';
 buf[ll] = '\0';
 strcat(rt, buf);
 strcat(rt,"\n");
 pList tmp = hheader;
 int z=0;
 while (tmp)
   {
    if (gtypes[z]==TTEXT) sprintf(buf,"%-40s\t", (char*)tmp->data);
    else sprintf(buf,"%-18s\t", (char*)tmp->data);
    strcat(rt, buf);
    tmp = tmp->next;
    z++;
   }
 strcat(rt,"\n");
 for (int x=0;x<ll;x++) buf[x] = '=';
 buf[ll] = '\0';
 strcat(rt,"\n");
 strcat(rt, buf);
 strcat(rt,"\n");
 for (int i=0;i<rows;i++)
 {
  //printf("i=%d\n", i);
  tmp = ppd[i];
  j=0;
  while (tmp)
    {
     //printf("j=%d, ", j);
     if (gtypes[j]==TTEXT) sprintf(buf,"%-40s\t", RetText(tmp, gtypes[j]));
     else sprintf(buf,"%-18s\t", RetText(tmp, gtypes[j]));
     strcat(rt, buf);
     tmp = tmp->next;
     j++;
    }
  //printf("\n");
  //printf("j=%d\n", j);
  strcat(rt,"\n");
 }
 for (int x=0;x<ll;x++) buf[x] = '=';
 buf[ll] = '\0';
 strcat(rt,"\n");
 strcat(rt, buf);
 strcat(rt,"\n");
 if (buf) delete [] buf;
 if (buf2) delete [] buf2;
 return rt;
}

void SaveWndClipToFile(Wnd& wnd, void* arg)		//Save window Text to FIle
{							//Used to Save Report
 if (!wnd.HaveClipArea()) { wnd.CreateMBoxChild("Okno nie posiada tekstu!"); return ;}
 ClipArea* ca = wnd.RetClipArea();
 int lin = ca->RetCharsY();
 char** tex = ca->RetClipText();
 if (lin<0 || !tex) { wnd.CreateMBoxChild("Blad danych w ClipArea"); return; }
 char* buf = wnd.CreateWriteBox("Podaj nazwe pliku: ");
 if (!buf || !strcmp(buf, "")) { wnd.CreateMBoxChild("Nazwa pliku nie moze byc rowna NULL"); return; }
 char* fn = new char[strlen(buf)+20];
 strcpy(fn,"./data/");
 strcat(fn,buf);
 strcat(fn,".R");
 FILE* plik = fopen(fn, "r");
 if (plik)
   {
    fclose(plik);
    int ok = wnd.CreateYesNoChild("Plik istnieje, zastapic?");
    if (!ok) return;
   }
 plik = fopen(fn, "w");
 if (!plik) { wnd.CreateMBoxChild("Nie mozna utworzyc pliku"); return; }
 for (int i=0;i<lin;i++) fprintf(plik,"%s\n", tex[i]);
 fclose(plik);
 delete [] buf;
 delete [] fn;
}

void DisplayAll(Wnd& wnd, void* ptr)		//This shows entire DBASE when used
{						//By clicking the button
 if (!ppd)
   {
    wnd.CreateMBoxChild("Nie utworzono jeszcze tablicy glow list");
    return ;
   }
 char* fbuf = ReadListToBuffer(wnd);
 if (!fbuf)
   {
    wnd.CreateMBoxChild("ReadListToBuffer zwrocilo NULL!");
    return ;
   }
 Wnd* w = new Wnd(wnd.RetDspStr(), 500, 500);
 if (!w) panic("memory exception in creating win_child!");
 w->WinDefaults("Podglad struktury danych");
 w->DeclareResponseTable(MEDI_RESPONSES);
 w->SelectExposeFunc(StdExposeFunc);
 w->SelectConfigureFunc(DenyResizeFunc);
 w->SelectKeyPressFunc(KeyPressClipControl);
 w->SelectKeyReleaseFunc(eXtendedKeyRelease);
 w->SelectButtonPressFunc(eXtendedButtonPress);
 w->SelectButtonReleaseFunc(eXtendedButtonRelease);
 w->PrepareWndText(fbuf);
 w->EnableText(0);
 w->EnableFrames(0);
 w->MakeClipFromWindowText(16, 16, 484, 454);
 w->SetButtonsNumber(2);
 w->CreateButton(0, "Zamknij",390 , 470, StdOKFunc, true, 100);
 w->CreateButton(1, "Zapisz" ,280 , 470, SaveWndClipToFile, false, 100);
 w->DefaultResponse();
 if (w) delete w;
 wnd.Invalidate();
 delete [] fbuf;
}


void PreviewFunc(Wnd& wnd, void* ptr)		//Preview of File in the window
{
 char* arg = (char*)ptr;
 if (!ptr)
   {
    wnd.CreateMBoxChild("Blad wewnetrzny:\nRzutowanie char* na void* zawiodlo!");
    return ;
   }
 char* fbuf = ReadFileToBuffer(wnd, arg);
 if (!fbuf)
   {
    wnd.CreateMBoxChild("ReadFileToBuffer zwrocilo NULL!");
    return ;
   }
 Wnd* w = new Wnd(wnd.RetDspStr(), 500, 500);
 if (!w) panic("memory exception in creating win_child!");
 w->WinDefaults("Podglad pliku");
 w->DeclareResponseTable(MEDI_RESPONSES);
 w->SelectExposeFunc(StdExposeFunc);
 w->SelectConfigureFunc(DenyResizeFunc);
 w->SelectKeyPressFunc(KeyPressClipControl);
 w->SelectKeyReleaseFunc(eXtendedKeyRelease);
 w->SelectButtonPressFunc(eXtendedButtonPress);
 w->SelectButtonReleaseFunc(eXtendedButtonRelease);
 w->PrepareWndText(fbuf);
 w->EnableText(0);
 w->EnableFrames(0);
 w->MakeClipFromWindowText(16, 16, 484, 454);
 w->SetButtonsNumber(1);
 w->CreateButton(0, "Zamknij",390 , 470, StdOKFunc, true, 100);
 w->DefaultResponse();
 if (w) delete w;
 wnd.Invalidate();
 delete [] fbuf;
}


void just_print(pList arg)  //DEBUG & only for this
{
 int i=0;
 while (arg)
   {
    if (gtypes[i]==TTEXT)  printf("<<(%p)<< this(%p)this >>(%p)>> txt[%d]=%s\n", arg->prev, arg, arg->next, i, (char*)arg->data);
    if (gtypes[i]==TDATE)  printf("<<(%p)<< this(%p)this >>(%p)>> dat[%d]=%s\n",arg->prev, arg, arg->next, i, ul2pcDate(*(unsigned long*)arg->data));
    if (gtypes[i]==TFLOAT) printf("<<(%p)<< this(%p)this >>(%p)>> flo[%d]=%f\n", arg->prev, arg, arg->next, i, *(double*)arg->data);
    i++;
    arg = arg->next;
   }
}


void empty_list(pList& h)				//Destroy given list
{
 if (h)
  {
   while (h->next) { h = h->next; delete h->prev; }
   delete h;
   h = NULL;
  }
}

int init_spec(pList h, void* el, int type)		//Insert Specific Element into list of
{							//void*, use conversion
 if (!h) return 0;
 if (type==TLIST)				//this creates square list (list of lists :-) )
   {
    pList ptr = (pList)el;
    if (!ptr) return 0;
    h->data = (void*)(new List);
    pList tmp = (pList)(h->data);
    (*tmp).next = ((pList)(el))->next;		//creates squareList <List**>
    (*tmp).prev = ((pList)(el))->prev;
    (*tmp).data = ((pList)(el))->data;
    (*tmp).p    = ((pList)(el))->p;
    (*tmp).t    = ((pList)(el))->t;
   }
 if (type==TTEXT)
   {
    pText ptr = (pText)el;
    int len = strlen(ptr->data);
    h->data =(void*) (new char[len+2]);		//data just cast to char* but done by STRCPY
    if (!h->data) return 0;			//do not write addres, because when list
    strcpy((char*)h->data, ptr->data);		//deleted data deleted too, we don't want it
    //printf("COPIED=%s\n", (char*)h->data);
   }
 if (type==TDATE)
   {
    pDate ptr = (pDate)el;
    if (!ptr) return 0;
    h->data = (void*)(new unsigned long);
    if (!h->data) return 0;
    unsigned long* tmp = (unsigned long*)(h->data);	//char* ul2pcDate(ulong)  makes %s from %u date
    *tmp = ((pDate)(el))->data;				//write to memory address <dereference>
   }
 if (type==TFLOAT)
   {
    pFloat ptr = (pFloat)el;
    if (!ptr) return 0;
    h->data = (void*)(new double);
    if (!h->data) return 0;
    double* tmp = (double*)(h->data);		//double size is 8 and void* is 4 so needed
    *tmp = ((pFloat)(el))->data;		//new double* and PTR, just neccesary casts :-)
   }
 return 1;		//ret 1 means ok=1, ret 0 means FAILED
}

pList add_to_last(pList& h, void* e, int ty, pList pr=NULL)	//main list func, add element
{								//to list tail, and init it
 //printf("ADDTOLIST(type=%d)\n", ty);	//DEBUG
 if (!h)
   {
    h = new List;
    if (!h) return NULL;
    h->p = pr;
    h->t = ty;
    h->next = h->prev = NULL;
    int ok = init_spec(h, e, ty);		//makes main work
    if (!ok) return NULL;
    //printf("OK\n");
    return h;
   }
 pList tmp = new List;
 if (!tmp) return NULL;
 tmp->p = pr;
 tmp->t = ty;
 tmp->next = NULL;
 tmp->prev = h;
 h->next = tmp;
 int ok = init_spec(tmp, e, ty);		//the same
 if (!ok) return NULL;
    //printf("OK\n");
 return tmp;
}


pList RetPointer(pList wsk, int num)	//return num-th element of list wsk
{
// pList tmp = wsk;
 for (int i=0;i<num;i++)
   {
    if (wsk) wsk = wsk->next;
    else return NULL;
   }
 return wsk;
}

pList RetByPriority(squareList sl, int row, int col) //ret col,row element in squareList
{						     //but traverse using addictional <priority> ptrs
 if (row<0 || col<0) return NULL;
 pList tmp = RetPointer(sl[row], pri[0]);
 for (int i=0;i<col;i++)
   {
    if (tmp) tmp = tmp->p;
    else return NULL;
   }
 return tmp;
}


int read_headers(pList& h, char* fn)		//read headers, and return OK
{
 if (!fn) return 0;
 FILE* p = fopen(fn, "r");
 if (!p) return 0;
 pText t;
 int code;
 int z;
 pList last = h;
 int read=0;
 do
   {
    t = new TText;
    code = t->read(p);
    if (code!=READBAD)
      {
       z=fgetc(p);
       if (z!=seps->scol && z!=EOF && z!='\n') code=READBAD;
       last = add_to_last(last, (void*)t, TTEXT);
       if (!h) h = last;
       if (!last) { trace("memory warning"); return 0; }	//no mem
       read++;
      }
    delete t;
   }
 while (code==READOK);
 fclose(p);
 /*while (arg)
   {
    printf("*TEXT[%d]=%s <<(%p)<< this(%p)this >>(%p)>>\n", i, (char*)arg->data, arg->prev, arg, arg->next);
    i++;
    arg = arg->next;
   }*/
 return read;
}

inline unsigned int SI22LI(int a, int b)	// A XXXX, B YYYY, A=>XX, B=>YY, ret XXYY
{						//pack to <0x10000 INT int ONE int
 return (unsigned int)((a<<16)+b);
}


void SetTypeCol(Wnd& w, void* ptr)		//set type of column <numer LOWORD, type=HIWORD>
{
 int type = ((unsigned int)ptr)/(1<<16)-1;
 int num  = ((unsigned int)ptr)%(1<<16)-1;	//here
 w.DoQuit();
 w.Invalidate();
 if (((unsigned int)ptr)<(1<<16)) { back = true; return; }
 gtypes[num] = type;
}


void WndSetColType(Wnd& wnd, pList ptr, int num)	//Set col type dialog, displayed num_cols times
{
 tout("SetFDatFunc");
 Wnd* wybor = new Wnd(wnd.RetDspStr(), 550, 125);
 if (!wybor) panic("memory exception in creating win_child!");
 char buuf[0x600];
 sprintf(buuf,"Podaj typ %d[%s] kolumny:", num, (char*)ptr->data);
 wybor->WinDefaults(buuf);
 wybor->SetButtonsNumber(4);
 wybor->CreateButton(0, "Liczba" ,105  , 5, SetTypeCol, true , 340, 25, (void*)SI22LI(TFLOAT+1,num+1));
 wybor->CreateButton(1, "Tekst"  ,105 , 35, SetTypeCol, false, 340, 25, (void*)SI22LI(TTEXT+1,num+1));
 wybor->CreateButton(2, "Data"   ,105  ,65, SetTypeCol, false, 340, 25, (void*)SI22LI(TDATE+1,num+1)); //MAKEINT
 wybor->CreateButton(3, "Wstecz" ,105  ,95, SetTypeCol, false ,340, 25, (void*)1);
 wybor->SetParent(&wnd);
 wybor->DefaultResponse();
 wnd.Invalidate();
 if (wybor) delete wybor;
}


void UpdateSeps(Wnd& w)
{
 char buf[0x100];
 if (!seps) return;
 sprintf(buf,"Wybierz Separatory:\nAktualne: %c %c %c %c %d", seps->scol, seps->stex, seps->sdec, seps->sdat, seps->fdat);
 w.BlackWings();
 w.PrepareWndText(buf);
 w.Invalidate();
}


void ReadFile(Wnd& wnd, void* ptr)	//but if no seps setup, return ERR
{
 if (!scol || !stex || !sdec || !sdat || !fdat)
 {
  wnd.CreateMBoxChild("Ktorys z separatow nie zostal okreslony\nLub nie okreslono formatu daty\nNie wiem jak wczytac ten plik!");
  return ;
 }
 empty_list(hheader);
 int rd = read_headers(hheader, (char*)ptr);
 if (!rd) { wnd.CreateMBoxChild("Odczyt naglowka pliku nie powiodl sie!"); return ; }
 prior = new int[rd];
 if (!prior) panic("table_of_prior cannot be created, inside SeqRead");
 for (int i=0;i<rd;i++) prior[i] = 0;
// while (hheader) printf(">>>LIST <<(%p)<< this(%p)this >>(%p)>>\n%s\n", hheader->prev, hheader, hheader->next,(char*)(hheader->data)); hheader=hheader->next;
 gtypes = new int[rd];
 pList wsk = hheader;
 for (int i=0;i<rd;i++)
   {
    back = false;
    WndSetColType(wnd, wsk, i);
    if (wsk) wsk = wsk->next;
    if (back)
      {
       wsk = wsk->prev;
       if (wsk->prev) wsk = wsk->prev;
       i-=2;
       if (i<-1) i=-1;
       back = false;
      }
   }
 screated = true;
 cols = rd;
 wnd.DoQuit();
 wnd.Invalidate();
}

void SetScolFunc(Wnd& w, void* ptr)		//set Col separator <all seps made by funcs>
{						//see below
 char buf[150];
 char small[10];
 char tabok[3];
 sprintf(buf,"Wybierz jeden z separatorow kolumn:\n");
 int k=0;
 for (int i=0;i<3;i++)
   {
    if (seps->stex != tscol[i] && seps->sdec != tscol[i] && seps->sdat != tscol[i])
       {
        sprintf(small, " %c ", tscol[i]);
	strcat(buf, small);
	tabok[k] = tscol[i];
	k++;
       }
   }
 tabok[k] = 0;
 if (k==0) strcat(buf,"{brak separatorow}");
 char* ret = w.CreateWriteBox(buf);
 if (!strcmp(ret, "")) return ;
 char sepo = 0;
 sscanf(ret, "%c", &sepo);
 bool ok = false;
 for (int i=0;i<k;i++) if (sepo == tabok[i]) ok = true;
 if (!ok) w.CreateMBoxChild("Niepoprawny separator!\nSproboj jeszcze raz!");
 else { seps->scol = sepo; scol = true; }
 UpdateSeps(w);
}


void SetSdecFunc(Wnd& w, void* ptr)
{
 char buf[150];
 char small[10];
 char tabok[3];
 sprintf(buf,"Wybierz jeden ze znacznikow dziesietnych:\n");
 int k=0;
 for (int i=0;i<2;i++)
   {
    if (seps->scol != tsdec[i] && seps->stex != tsdec[i] && seps->sdat != tsdec[i])
       {
        sprintf(small, " %c ", tsdec[i]);
	strcat(buf, small);
	tabok[k] = tsdec[i];
	k++;
       }
   }
 tabok[k] = 0;
 if (k==0) strcat(buf,"{brak separatorow}");
 char* ret = w.CreateWriteBox(buf);
 if (!strcmp(ret, "")) return ;
 char sepo = 0;
 sscanf(ret, "%c", &sepo);
 bool ok = false;
 for (int i=0;i<k;i++) if (sepo == tabok[i]) ok = true;
 if (!ok) w.CreateMBoxChild("Niepoprawny separator!\nSproboj jeszcze raz!");
 else { seps->sdec = sepo; sdec = true; }
 UpdateSeps(w);
}


void SetStexFunc(Wnd& w, void* ptr)
{
 char buf[150];
 char small[10];
 char tabok[3];
 sprintf(buf,"Wybierz jeden z separatorow tekstu:\n");
 int k=0;
 for (int i=0;i<2;i++)
   {
    if (seps->scol != tstex[i] && seps->sdec != tstex[i] && seps->sdat != tstex[i])
       {
        sprintf(small, " %c ", tstex[i]);
	strcat(buf, small);
	tabok[k] = tstex[i];
	k++;
       }
   }
 tabok[k] = 0;
 if (k==0) strcat(buf,"{brak separatorow}");
 char* ret = w.CreateWriteBox(buf);
 if (!strcmp(ret, "")) return ;
 char sepo = 0;
 sscanf(ret, "%c", &sepo);
 bool ok = false;
 for (int i=0;i<k;i++) if (sepo == tabok[i]) ok = true;
 if (!ok) w.CreateMBoxChild("Niepoprawny separator!\nSproboj jeszcze raz!");
 else { seps->stex = sepo; stex = true; }
 UpdateSeps(w);
}


void SetSdatFunc(Wnd& w, void* ptr)
{
 char buf[150];
 char small[10];
 char tabok[3];
 sprintf(buf,"Wybierz jeden z separatorow date:\n");
 int k=0;
 for (int i=0;i<2;i++)
   {
    if (seps->scol != tsdat[i] && seps->sdec != tsdat[i] && seps->stex != tsdat[i])
       {
        sprintf(small, " %c ", tsdat[i]);
	strcat(buf, small);
	tabok[k] = tsdat[i];
	k++;
       }
   }
 tabok[k] = 0;
 if (k==0) strcat(buf,"{brak separatorow}");
 char* ret = w.CreateWriteBox(buf);
 if (!strcmp(ret, "")) return ;
 char sepo = 0;
 sscanf(ret, "%c", &sepo);
 bool ok = false;
 for (int i=0;i<k;i++) if (sepo == tabok[i]) ok = true;
 if (!ok) w.CreateMBoxChild("Niepoprawny separator!\nSproboj jeszcze raz!");
 else { seps->sdat = sepo; sdat = true; }
 UpdateSeps(w);
}


void SetDF(Wnd& w, void* ptr)		//Set DateFormat
{
 int tmp = (int)ptr;
 seps->fdat = (BYTE)tmp;
 fdat = true;
 w.DoQuit();
 w.Invalidate();
}


void SetOrient(Wnd& w, void* ptr)	//Set Report Orientation ,COLUMNS, TABLES
{
 bool tmp = (bool)ptr;
 borient = tmp;
}


void SetFDatFunc(Wnd& wnd, void* ptr)	//Calls SetFD with arg ==> void* <== format
{
 tout("SetFDatFunc");
 Wnd* wybor = new Wnd(wnd.RetDspStr(), 200, 65);
 if (!wybor) panic("memory exception in creating win_child!");
 wybor->WinDefaults("Format daty");
 wybor->SetButtonsNumber(2);
 wybor->CreateButton(0, "RRRR-MM-DD" ,5  , 5, SetDF, true, 190, 25, (void*)0);
 wybor->CreateButton(1, "DD-MM-RRRR" ,5 , 35, SetDF, false, 190, 25, (void*)1);
 wybor->SetParent(&wnd);
 wybor->DefaultResponse();
 wnd.Invalidate();
 if (wybor) delete wybor;
 UpdateSeps(wnd);
}

void DefaultSeps(Wnd& w, void* arg)	//This wasn't in project, but manually set
{					//seps in every debug is a real HELL
 seps->fdat = 0;			//so I did this ;-)
 fdat = true;
 seps->sdat = '-';
 sdat = true;
 seps->stex = '\"';
 stex = true;
 seps->sdec = '.';
 sdec = true;
 seps->scol = ',';
 scol = true;
 UpdateSeps(w);
}


int MyReadFile(Wnd& wnd, char* name)	//Read DBase Options...
{
 tout("MyReadFile");
 Wnd* w = new Wnd(wnd.RetDspStr(), 200, 340);
 if (!w) panic("memory exception in creating win_child!");
 w->WinDefaults("Opcje pliku danych");
 w->DeclareResponseTable(MEDI_RESPONSES);
 w->SelectExposeFunc(StdExposeFunc);
 w->SelectConfigureFunc(DenyResizeFunc);
 w->SelectKeyPressFunc(eXtendedKeyPressQ);
 w->SelectKeyReleaseFunc(eXtendedKeyRelease);
 w->SelectButtonPressFunc(eXtendedButtonPress);
 w->SelectButtonReleaseFunc(eXtendedButtonRelease);
 w->PrepareWndText("Wybierz separatory:");
 w->StdTextWrite();
 w->SetButtonsNumber(9);
 w->CreateButton(0, "Podglad pliku",50 , 60, PreviewFunc, true, 100, 25, (void*)name);
 w->CreateButton(1, "Sep. kolumn",50 , 90, SetScolFunc, false, 100);
 w->CreateButton(2, "Sep. tekstu",50 , 120, SetStexFunc, false, 100);
 w->CreateButton(3, "Zn dziesietny",50 , 150, SetSdecFunc, false, 100);
 w->CreateButton(4, "Sep. daty",50 , 180, SetSdatFunc, false, 100);
 w->CreateButton(5, "Form. daty",50 , 210, SetFDatFunc, false, 100);
 w->CreateButton(6, "OK, wczytaj",50 , 240, ReadFile, false, 100, 25, (void*)name);
 w->CreateButton(7, "Zamknij",50 , 270, StdOKFunc, false, 100);
 w->CreateButton(8, "Domyslne Separatory",20 , 300, DefaultSeps, false, 160);
 w->DefaultResponse();
 if (w) delete w;
 wnd.Invalidate();
 return 1;
}

void OpenDataFile(Wnd& wnd, void* ptr)		//Open Dialog <browses Entire FS>
{						//watch up permissions to read_dir!
 tout("OpenDataFile");
 if (fopened)
 {
  int ok = wnd.CreateYesNoChild("Juz jest wczytany plik.\nChcesz wczytac inny?");
  if (!ok) return;
 }
 tout("OpenDataBase");
 int num = 0;
 char cwd[4096];
 char path[4096];
 getcwd(cwd, 4000);
 strcpy(path,cwd);
 strcat(path,"/data/");
 chdir(path);
 char** table = NULL;
 char* selected = NULL;
 char buff[512];
 bool first = true;
 Button **btn;
 while (true)
   {
   if (table)
     {
      for (int i=0;i<num;i++) if (table[i]) delete [] table[i];
      if (table) delete [] table;
     }
   //getcwd(cwd, 4096);
   table = ReadFolderToTable(num, path, ".DAT");
   if (first && num<=2) { wnd.CreateMBoxChild("Brak plikow danych w domyslnym folderze!\nLub Access Denied"); return; }
   first=false;
   if (!table)
     {
      wnd.CreateMBoxChild("Brak danych w katalogu\nLub Access Denied");
      return ;
     }
   if (!num)
     {
      wnd.CreateMBoxChild("Brak plikow w folderze: ./data\nLub Access Denied");
      return ;
     }
   FSort(table, num);
   if (selected) delete [] selected;
   sprintf(buff,"%s, plik:", path);
   btn = new Button*[1];
   btn[0] = new Button;
   btn[0]->SetBtnPos(575,460);
   btn[0]->SetBtnSize(100,25);
   btn[0]->SetDefault(0);
   btn[0]->SetBtnName("Zamknij");
   btn[0]->SetBtnFunc(StdOKFunc);
   selected = wnd.CreateExCheckBoxVert(buff, table, num, 48, 700, 500, 1, btn);
   if (!selected) { chdir(cwd); return; }
   if (selected[strlen(selected)-1] == '/')
     {
      strcat(path, selected);
      chdir(path);
      getcwd(path, 4096);
      strcat(path, "/");
     }
   else break;
   }
 chdir(cwd);
 Button* btmp;
 if (wnd.validBtn(0))
  {
   btmp = wnd.RetBtn(0);
   btmp->SetDefault(0);
  }
 if (wnd.validBtn(1))
  {
   btmp = wnd.RetBtn(1);
   btmp->SetDefault(1);
  }
// char* tmp = new char[strlen(selected)+20];
 strcat(path, selected);
 int oki = MyReadFile(wnd, path);
 if (!oki)
   {
    wnd.CreateMBoxChild("Plik istnieje, ale ma niepoprawny format!");
    return;
   }
 if (fn) delete [] fn;
 fn = new char[strlen(path)+20];
 strcpy(fn, path);
 fopened = true;
 if (table)
   {
    for (int i=0;i<num;i++) if (table[i]) delete [] table[i];
    if (table) delete [] table;
   }
 wnd.Invalidate();
}

void InitFire(Wnd& wnd, void* arg)		//When I had problems with SORT !!!
{						//I wanted to write something
 (wnd.RetScroll(0))->SetScrlPos(0X8A);		//So I wrote Autor BAnner ;-)
 (wnd.RetScroll(1))->SetScrlPos(0X0C);
 (wnd.RetScroll(2))->SetScrlPos(0XE4);		//Sets Default two RGB values
 (wnd.RetScroll(3))->SetScrlPos(0XA8);
 (wnd.RetScroll(4))->SetScrlPos(0XF2);
 (wnd.RetScroll(5))->SetScrlPos(0X3F);
}

void MyColor(Wnd& wnd, void* arg)		//When scrollBar THUMBS, called
{						//to setup Fire Colors ;-)
 int r1 = (wnd.RetScroll(0))->RetPos();
 int g1 = (wnd.RetScroll(1))->RetPos();
 int b1 = (wnd.RetScroll(2))->RetPos();
 int r2 = (wnd.RetScroll(3))->RetPos();
 int g2 = (wnd.RetScroll(4))->RetPos();
 int b2 = (wnd.RetScroll(5))->RetPos();
 int **cola = new int*[120];
 for (int i=0;i<120;i++) cola[i] = new int[220];
 int norm = -280;
 int r,g,b;
 Display* d = wnd.RetDisp();
 Window w = wnd.RetWin();
 GC gc = wnd.RetGC();
 for (int i=0;i<120;i++)
   {
    norm += 3;
    for (int j=0;j<220;j++)
      {
       r = (j*r1 + (220-j)*r2)/220;
       g = (j*g1 + (220-j)*g2)/220;
       b = (j*b1 + (220-j)*b2)/220;
       r += norm;
       g += norm;
       b += norm;
       if (r<0)   r=0;
       if (r>255) r=255;
       if (g<0)   g=0;
       if (g>255) g=255;
       if (b<0)   b=0;
       if (b>255) b=255;
       cola[i][j] = RGB(r,g,b);
       XSetForeground(d,gc, cola[i][j]);
       XDrawPoint(d,w,gc, 15+j, 90+i);
      }
   }
 //wnd.Invalidate();
 wnd.StdTextWrite();
}


void AutorFunc(Wnd& wnd, void* ptr)			//Autor Banner! ;-))
{
 tout("AboutFunc");
 Wnd* wybor = new Wnd(wnd.RetDspStr(), 250, 400);
 if (!wybor) panic("memory exception in creating win_child!");
 wybor->WinDefaults("Autor: Morgoth DBMA");
 wybor->DeclareResponseTable(MOST_RESPONSES);
 wybor->SelectExposeFunc(PalExposeFunc);
 wybor->SelectExposeXFunc(MyColor);
 wybor->SelectConfigureFunc(PalDenyResizeFunc);
 wybor->SelectButtonPressFunc(PalButtonPress);
 wybor->SelectKeyPressFunc(eXtendedKeyPress);
 wybor->SelectButtonReleaseFunc(PalButtonRelease);
 wybor->SelectKeyReleaseFunc(PalKeyRelease);
 wybor->SelectMotionNotifyFunc(PalMotionNotifyFunc);
 wybor->SelectKeyPressFunc(eXtendedKeyPressQ);
 wybor->SelectConfigureFunc(NoResizeDrawWFunc);
 wybor->SetPaletteInfo(false);
 wybor->PrepareWndText("Projekt Indywidualny:\nGenerator Raportow\nLukasz Gryglicki M1 (MorgothDBMA)\nNa podstawie projektu\nLukasza Bainskiego");
 wybor->EnableFrames(0);
 wybor->SetScrlNumber(6);
 wybor->CreateScrl(0, 20,  250, MyColor, true, 128, 0xff, (void*)1);
 wybor->CreateScrl(1, 40,  250, MyColor, false, 128, 0xff, (void*)2);
 wybor->CreateScrl(2, 60,  250, MyColor, false, 128, 0xff, (void*)3);
 wybor->CreateScrl(3, 190,  250, MyColor, false, 128, 0xff, (void*)4);
 wybor->CreateScrl(4, 210,  250, MyColor, false, 128, 0xff, (void*)5);
 wybor->CreateScrl(5, 230,  250, MyColor, false, 128, 0xff, (void*)6);
 wybor->SetButtonsNumber(1);
 wybor->CreateButton(0, "OK", 105, 220, StdOKFunc, true, 40, 18);
 InitFire(*wybor, NULL);
 wybor->DefaultResponse();
 if (wybor) delete wybor;
 wnd.Invalidate();
}

void Suicide(Wnd& w, void* ptr)		//this Kills All WIndows and Terminates Application
{					//of course when called by Parent Window
 w.DoQuit();
 w.Invalidate();
}


void GrandFinale(Wnd& w, void* ptr)	//asks Parent WIndow to COMMITE SUICIDE
{
 int yes = w.CreateYesNoChild("Czy na pewno chcesz zakonczyc?");
 if (!yes) return ;
 w.ParentCallFunc(Suicide);		//bye!
 w.DoQuit();
 w.Invalidate();
 step = -1;				//leave creator
}


int testEOF(FILE* p)		//parser wants to know if there are only ' ' or \n until EOF
{
 if (!p) return 1;
 int pos = ftell(p);
 int zn;
 again:
 zn = fgetc(p);
 if (zn==EOF) return 1;
 if (zn== ' ' || zn=='\n' || zn=='\t') goto again;
 if (zn==EOF) return 1;
 else { fseek(p, pos, SEEK_SET); return 0; }
}

int ScanLine(FILE* p, pList& hdr, int& rcd)	//Scanes Line from DataFile
{						//rcd tell how much cols read
 int code;
 int z;
 rcd = -1;
 pText  pT;
 pFloat pF;
 pDate  pD;
 pList last = hdr;
 int r=0;
 for (int i=0;i<cols;i++)
 {
  if (testEOF(p)) return READEOF;
  if (gtypes[i]==TTEXT)  { pT = new TText;  code = pT->read(p); }
  if (gtypes[i]==TDATE)  { pD = new TDate;  code = pD->read(p); }
  if (gtypes[i]==TFLOAT) { pF = new TFloat; code = pF->read(p); }
  rcd=i;
  //printf("<%d>code=%d\n", rcd, code);
  if (code != READBAD)
    {
     z = fgetc(p);
     if (z!=seps->scol && z!=EOF && z!='\n') code=READBAD;
     if (gtypes[i]==TTEXT)  last = add_to_last(last, (void*)pT, TTEXT);
     if (gtypes[i]==TDATE)  last = add_to_last(last, (void*)pD, TDATE);
     if (gtypes[i]==TFLOAT) last = add_to_last(last, (void*)pF, TFLOAT);
     if (!hdr) hdr = last;
     if (!last) { trace("memory warning"); return 0; }
     if (gtypes[i]==TTEXT)  delete pT;
     if (gtypes[i]==TDATE)  delete pD;
     if (gtypes[i]==TFLOAT) delete pF;
     r++;
    }
  else return READBAD;
  //printf("now READED=");
  //just_print(hdr);
  //printf("internal-code={%d}\n", code);
 }
 return READOK;
}


void just_print_sq(pList arg)  //DEBUG & only DEBUG
{
 int i=0;
 while (arg)
   {
    printf("<<(%p)<< this(%p)this >>(%p)>> list[%d]=%p\n", arg->prev, arg, arg->next, i, arg->data);
    just_print((pList)arg->data);
    i++;
    arg = arg->next;
   }
}


int SeqRead(Wnd& w)		//Sequentially Read File <line by line>
{
 if (cols<1) return 0;
 int code;
 int zn;
 FILE* p = fopen(fn, "r");
 if (!p) return 0;
 do zn=fgetc(p); while (zn!=EOF && zn!='\n');	//skip header line, already read if here
 if (zn==EOF) return 0;
 int i=0;
 pList super_head = NULL;		//temorary list of lists
 pList ll = super_head;			//next lists will be added to super_head
 pList tmpL;
 char buf[0x80];
 int row=0;
 do
   {
    //printf("NEXT_SCLINE=%d\n", i);
    tmpL = NULL;
    code = ScanLine(p, tmpL, row);
    //just_print(tmpL);
    if (code==READBAD)
      {
       char buuf[256];
       sprintf(buuf,"Blad w ScanLine, linia [%d] zmienna [%d]\nmoze byc konieczna zmiana\npewnych separatorow", i, row);
       w.CreateMBoxChild(buuf);
       return 0;
      }
    if (tmpL) { ll = add_to_last(ll, (void*)tmpL, TLIST); i++; }	//here add list to list
    if (!super_head) super_head = ll;
    if (!ll)
      {
       trace("memory warning");
       sprintf(buf,"Przeczytalem tylko %d linii", i);
       w.CreateMBoxChild(buf);
      }
   }
 while (code!=READEOF);
 //just_print_sq(super_head);
 rows = i;
 //printf("ROWS=%d, COLS=%d\n", rows, cols);
 fclose(p);
 ppd = new pList[rows+1];				//now create array of lists
 if (!ppd) panic("cannot create SquareList - Area");
 for (int i=0;i<rows;i++)
   {
    ppd[i] = (pList)super_head->data;
    if (super_head->next) { super_head = super_head->next; delete super_head->prev; }
    else delete super_head;		//free super_head
   }
 //printf("SquareList Created!\n");
 return rows;
}


void SelectAll(Wnd& w, void* ptr)	//Select all columns to report
{
 for (int i=0;i<cols;i++) prior[i] = i+1;
 pList tmp = hheader;
 char texti[512];
 for (int i=0;i<cols;i++)
  {
   sprintf(texti,"[%d]%s", prior[i], (char*)tmp->data);
   w.SetButtonName(i, texti);
   tmp = tmp->next;
  }
 w.Invalidate();
}


void SetColPriorityFunc(Wnd& w, void* arg)		//Set Column Priority
{
 char buf[0x100];
 char bnam[0x100];
 int p;
 sscanf(w.RetSelectedButtonName(), "[%d]%[a-zA-Z0-9 \t]", &p, bnam);
 sprintf(buf,"Aktualny priorytet\nkolumny %s\nto %d, zmienic na:", bnam, prior[w.RetPressed()]);
 char* ans = w.CreateLWriteBox(buf);
 if (!strcmp(ans, "")) return ;
 p=-1;
 sscanf(ans, "%d", &p);
 if (p<0) { w.CreateMBoxChild("Priorytet jest liczba nieujemna!"); return; }
 prior[w.RetPressed()] = p;
 Button* btn = w.RetBtn(w.RetPressed());
 sprintf(buf,"[%d]",p);
 strcat(buf,bnam);
 btn->SetBtnName(buf);
}


void SetColSortFunc(Wnd& w, void* arg)			//Set Column Sort Mode
{
 char buf[0x100];
 char bnam[0x100];
 int p;
 sscanf(w.RetSelectedButtonName(), "[%d]%[a-zA-Z0-9 \t]", &p, bnam);
 sprintf(buf,"Aktualne sortowanie\nkolumny %s\nto %d, zmienic na:", bnam, srt[w.RetPressed()]);
 char* ans = w.CreateLWriteBox(buf);
 if (!strcmp(ans, "")) return ;
 int num = w.RetPressed();
 p=-1;
 sscanf(ans, "%d", &p);
 if (p!=0 && p!=-1 && p!=1) { w.CreateMBoxChild("Sortowanie, wybierz: -1,0 lub 1"); return; }
 if (p==0 && grp[num]>0) { w.CreateMBoxChild("Nie mozna anulowac sortowania\nna kolumnie z grupowaniem"); return; }
 for (int i=0;i<num;i++) if (srt[i]==0) { w.CreateMBoxChild("Nie mozna ustawic sortowania po kolumnie\nJezeli kolumna o wyzszym priorytecie\nnie podlega sortowaniu"); return ; }
 int back=srt[num];
 srt[num] = p;
 int started = 0;
 for (int i=ncols-1;i>=0;i--)
   {
    if (srt[i]>0) started=1;
    if (started && !srt[i]) { w.CreateMBoxChild("Proba ustawienia niewlasciwego sortowania"); srt[num] = back; return ; }
   }
 Button* btn = w.RetBtn(w.RetPressed());
 sprintf(buf,"[%d]",p);
 strcat(buf,bnam);
 btn->SetBtnName(buf);
}


void SetColGroupFunc(Wnd& w, void* arg)			//Set Column Group mode
{
 char buf[0x100];
 //char bnam[0x100];
 int p  = (int)arg;
 pList tmp = hheader;
 sprintf(buf,"Aktualne grupowanie\nkolumny (%d)%s\nto %d, 0-brak,1-grupow,2-interwal\nPodaj nowe:", pri[p], (char*)(RetPointer(tmp, pri[p]))->data, grp[p]);
 char* ans = w.CreateLWriteBox(buf);
 if (!strcmp(ans, "")) return ;
 int r=-1;
 sscanf(ans, "%d", &r);
 if (r!=0 && r!=1 && r!=2) { w.CreateMBoxChild("Podaj liczbe 0, 1 lub 2!"); return; }
 if (r==2 && gtypes[pri[p]]!=TFLOAT) { w.CreateMBoxChild("Interwal moze byc uzyty tylko dla\ndanych liczbowych!"); return; }
 for (int i=0;i<p;i++) if (grp[i]<1) { w.CreateMBoxChild("Nie mozna ustawic grupowania po kulumnie\njezeli istnieja kolumny o\nwyzszych priorytetach bez ustawionego grupowania"); return ; }
 int started=0;
 int back=grp[p];
 grp[p] = r;
 for (int i=ncols-1;i>=0;i--)
   {
    if (grp[i]>0) started=1;
    if (started && !grp[i]) {w.CreateMBoxChild("Proba ustawienia niewlasciwego grupowania"); grp[p] = back; return ; }
   }
 if (srt && !srt[p]) srt[p] = 1;
 if (r==2)
   {
    r=-1;
    ans = w.CreateWriteBox("Podaj dlugosc interwalu (przedzialu):");
    if (!strcmp(ans, "")) return ;
    sscanf(ans, "%d", &r);
    interv[p] = r;
    if (r<=0) { w.CreateMBoxChild("Podaj liczbe wieksza od 0"); return; }
   }
 else interv[p] = 0;
 Button* btn = w.RetBtn(p);
 sprintf(buf,"[%d<%d]",grp[p], interv[p]);
 strcat(buf, (char*)(RetPointer(tmp, pri[p])->data));
 btn->SetBtnName(buf);
}


void SetColAltNameFunc(Wnd& w, void* arg)		//set alternate name for column
{
 int p = (int)arg;
 char buf[0x100];
 //char bnam[0x100];
 pList tmp = hheader;
 sprintf(buf,"Aktualna kolumna %s\nma alias %s, zmienic na:", (char*)(RetPointer(tmp, pri[p]))->data, altn[p]);
 char* ans = w.CreateLWriteBox(buf);
 if (!strcmp(ans, "")) return ;
 if (!ans) return ;
 if (altn[p]) { delete [] altn[p]; altn[p] = NULL; }
 altn[p] = new char[strlen(ans)+2];
 if (!altn[p]) return ;
 strcpy(altn[p], ans);
 Button* btn = w.RetBtn(p);
 sprintf(buf,"[%s]%s",altn[p], (char*)(RetPointer(tmp, pri[p]))->data);
 btn->SetBtnName(buf);
}


void SetupColBtns(Wnd& w, void* arg)		//Setup Buttons on window
{
 int x=10;
 int y=28;
 pList tmp = hheader;
 char texti[512];
 for (int i=0;i<cols;i++)
  {
   sprintf(texti,"[%d]%s", prior[i], (char*)tmp->data);
   w.CreateButton(i, texti, x, y, SetColPriorityFunc, false, 115, 26);
   y+= 28;
   if (y>=308) { y=28; x+= 120; }
   tmp = tmp->next;
  }
}


void SetupColAltBtns(Wnd& w, void* arg)		//see above, these for Alternames
{
 int x=10;
 int y=56;
 pList tmp = RetPointer(hheader, pri[0]);
 char texti[512];
 for (int i=0;i<ncols;i++)
  {
   sprintf(texti,"[%s]%s", altn[i], (char*)tmp->data);
   w.CreateButton(i, texti, x, y, SetColAltNameFunc, false, 170, 26, (void*)i);
   y+= 28;
   if (y>=308) { y=28; x+= 170; }
   tmp = tmp->p;
  }
}


void SetupColSortBtns(Wnd& w, void* arg)	//sort btns...
{
 int x=10;
 int y=28;
 pList tmp = RetPointer(hheader, pri[0]);
 char texti[512];
 for (int i=0;i<ncols;i++)
  {
   sprintf(texti,"[%d]%s", srt[i], (char*)tmp->data);
   w.CreateButton(i, texti, x, y, SetColSortFunc, false, 115, 26);
   y+= 28;
   if (y>=308) { y=28; x+= 120; }
   tmp = tmp->p;
  }
}


void SetupColSelBtns(Wnd& w, void* arg)		//which cols select...
{
 int x=10;
 int y=56;
 pList tmp = RetPointer(hheader, pri[0]);
 char texti[512];
 for (int i=0;i<ncols;i++)
  {
   sprintf(texti,"[%d:<%d]%s", grp[i], interv[i], (char*)tmp->data);
   w.CreateButton(i, texti, x, y, SetColGroupFunc, false, 115, 26, (void*)i);
   y+= 28;
   if (y>=308) { y=56; x+= 120; }
   tmp = tmp->p;
  }
}

int CheckPriority(Wnd&, void*);			//some forwards
int CheckSort(Wnd&, void*);
int CheckGroup(Wnd&, void*);
int CheckAltNames(Wnd&, void*);

int CompareSimpleEqual(void* a, void* b, int type)	//Are they just Equal or Not
{
 if (type==TTEXT) return !strcmp((char*)a, (char*)b);
 else if (type==TFLOAT) return (*(double*)a)==(*(double*)b);
 else if (type==TDATE) return (*(unsigned long*)a)==(*(unsigned long*)b);
 else return 0;
}

int CompareSimple(void* a, void* b, int type)		//Are A MORE_THAN B
{							//And not Equal, sort must be stable
 if (type==TTEXT) return StrMore((char*)a, (char*)b);
 else if (type==TFLOAT) return (*(double*)a)>(*(double*)b);
 else if (type==TDATE) return (*(unsigned long*)a)>(*(unsigned long*)b);
 else return 0;
}


int InsideArea(double a, double b, double len, double start)	//is A & B inside INTERVAL AREA
{
 //printf("Insidearea(%f,%f,%f,%f)\n",a,b,len,start);
 if (a>=start && a<=start+len && b>=start && b<=start+len) return 1;
 else return 0;
}

int InsArea(double a, double b, double len, double min)	
{					
 double aa = a - min;
 double bb = b - min;
 int ai = (int)aa /(int)len;
 int bi = (int)bb /(int)len;		//get INT COMPARISON <numbers are AREAS>
 int r = ai-bi;				//JNE WRT " " else WRT dane
 //printf("a=%f, b=%f, min=%f, len=%f, aa=%f, bb=%f, ai=%d, bi=%d, r=%d\n", a,b,min,len,aa,bb,ai,bi,r);
 return !r;
}

			//This function is really hard, description:
			//Need prev elements <columns> because in sort & group
			//we want to know about relation between previous elements
			//when equal continue sort or when the are in
			//interval area continue, else compare rets 0
			//works as break, because I'm using insertion_sort
			//short slices are sorted very fast by insertion
			//a,b are compared values
			//direction tells about sort (or group, setting up group
			//set sort to 1) direction DESC or ASC
			//typep <type_of_previous> we wanna know how to compare those
			//type <type of current> to know how to compare them
			//intv if non-zero means that interval is used
			//use it to compare in areas, when non-zero from must be set
			//and means from where area starts, reference is used because
			//when valkues are growing, area may change (increase by intv)

int Compare(pList prva, pList prvb, void* a, void* b, int direction, int typep, int type, double intv, double& from)
{
 //if (prva && prvb) if (!CompareSimpleEqual(prva->data, prvb->data, typep)) return 0;
 //if (direction) return CompareSimple(a,b,type);
 //else return !CompareSimple(a,b,type);
 if (typep!=TFLOAT && intv) { trace("Interwal dla nie-liczbowej kolumny"); return 0; }
 /*printf(" COMPARE(%s,%s,%s,%s,%d,%d,%d) \n",
 ((prva)?(RetText(prva, typep)):"(null)"),
 ((prvb)?(RetText(prvb, typep)):"(null)"),
 RetText(a, type),
 RetText(b, type),
 direction,
 typep ,type
 );*/
 if (prva && prvb)
   {
    //printf("PREVIOUS ARE NOT NULL, NOW ARE THEY EQUAL");
    int r;
    if (intv)
      {
       //printf("intv=%f, from=%f\n", intv, from);
       //if (!InsideArea(*(double*)prva->data, *(double*)prvb->data, intv, from))	//if not
       if (!InsArea(*(double*)prva->data, *(double*)prvb->data, intv, from))	//if not
         {						//in the same area, increase area and RET
	  from+=intv;
	  return 0;
	 }
       //return 0;
      }
    else		//no INTERVALS
      {
       //if (!CompareSimpleEqual(prva->data, prvb->data, typep)) return 0;
       r = CompareSimpleEqual(prva->data, prvb->data, typep);		//are prev EQUAL
       //printf(", EQ=%d\n", r);
      }
    if (!r) return 0;					//no, END sorting
   }
 if (direction==1)					//ASC
   {
    //printf("ASC_SORT ");
    return  CompareSimple(a,b,type);		//our comparison
    //printf("CompareSimple ret=%d\n", r);
   }
 else if (direction==-1)
   {
    //printf("DESC_SORT ");
    return !CompareSimple(a,b,type);		//inverted comparison
    //printf("CompareSimple ret=%d\n", r);
   }
 return 0;
}

			//THIS IS VERY IMPORTANT
//Algorytm w projekcie nie przewidzial sytuacji:
//jezeli sortujemy (z grupowaniem) po np 6 kolumnach
//i przypadkiem mamy w kroku 5-tej kolumny: np
//+++++++++++++++++++++++++++++++
//| 50.00000     | |uuuu          | |2002-02-02    | |2001-08-17    | |dddd          | |120.00000     |
//|100.00000     | |aaaa          | |2010-09-19    | |2000-01-01    | |cccc          | |200.00000     |
//|100.00000     | |aaaa          | |2010-10-10    | |2000-01-01    | |eeee          | |250.00000     |
//|100.00000     | |aaaa          | |2010-10-10    | |2000-02-03    | |dddd          | |300.00000     |
//|150.00000     | |rrrr          | |2010-09-19    | |2000-01-01    | |dddd          | |150.00000     |
//|150.00000     | |zzzz          | |1999-09-09    | |2000-01-01    | |uuuu          | |250.00000     |
//|200.00000     | |ffff          | |2009-09-09    | |2003-03-03    | |eeee          | |150.00000     |
//|200.00000     | |gggg          | |2000-12-12    | |2003-02-02    | |cccc          | |120.00000     |
//+++++++++++++++++++++++++++++++
//zauwazmy ze sortowanie i grupowanie 4 i 5-go wiersza ustawilo przypadkiem w piatej kolumnie
//obok siebie wartosci "dddd", sortowanie po nastepnej kolumnie (6) porowna poprzednie wartosci
//tj "dddd" z "dddd" stwierdzi, ze sa takie same i w zwiazku z tym zamieni wiersze 4 i 5
//psujac kolejnosc juz w pierwszej kolumnie danych (moj algorytm sortowania nie rozwiazuje tej
//sytuacji, poniewaz mam narzucony algorytm z projektu), wynik bedzie nastepujacy:
//+++++++++++++++++++++++++++++++
//| 50.00000     | |uuuu          | |2002-02-02    | |2001-08-17    | |dddd          | |120.00000     |
//|100.00000     | |aaaa          | |2010-09-19    | |2000-01-01    | |cccc          | |200.00000     |
//|100.00000     | |aaaa          | |2010-10-10    | |2000-01-01    | |eeee          | |250.00000     |
//|150.00000     | |rrrr          | |2010-09-19    | |2000-01-01    | |dddd          | |150.00000     |
//|100.00000     | |aaaa          | |2010-10-10    | |2000-02-03    | |dddd          | |300.00000     |
//|150.00000     | |zzzz          | |1999-09-09    | |2000-01-01    | |uuuu          | |250.00000     |
//|200.00000     | |ffff          | |2009-09-09    | |2003-03-03    | |eeee          | |150.00000     |
//|200.00000     | |gggg          | |2000-12-12    | |2003-02-02    | |cccc          | |120.00000     |
//+++++++++++++++++++++++++++++++
// i nic na to nie poradze :-(
// 			//TO NIE MOJ BLAD, JA TYLKO PISZE ALGORYTM
double SetupTMin(squareList sl, double len, double min, int col, int row)	//compute
{							//minimum value for AREA_INTERVAL
 if (!sl) return 0.0;
 double cur = *(double*)(RetByPriority(sl, row, col)->data);	//in given CELL
 double ret = min;
 while (ret<cur) ret+=len;
 return (ret-len);
}

void SortByColumn(squareList& sl, int col)	//Sort given Column
{						//to sort all, we need sort in loop all columns
 //printf("sort_by_column, col=%d\n", col);
 if (!sl) return;
 void* v;
 pList tmp;
 pList prev;
 pList tj;
 pList prvL;
 int gp = pri[col];
 int gpp = pri[((col-1)>=0)?(col-1):0];		//not go beyond the table, DEBUG ERROR WAS HERE
 double iv = 0;
 double cmin;
 double tmin;
 if (col>0 && grp[col-1]==2)		//if interval
   {
    if (gtypes[gpp]!=TFLOAT) { trace("Cannot use interval for non-number column"); return ; }
    iv = (double)interv[col-1];
    cmin = *(double*)((RetByPriority(sl, 0, col-1))->data);	//get minimum this is first
    //printf("cmin=%f\n", cmin);				//data because already sorted
   }								//because it is col-1
 tmin=cmin;
 //printf("gp=%d, gpp=%d\n", gp, gpp);
 int j;
 for (int i=1;i<rows;i++)					//first loop of insertion
  {
   //printf("<col=%d, i=%d>\n", col, i);
   tmp  = sl[i];
   prev = RetByPriority(sl, i, col-1);
   //printf("tmp=%p, prev=%p\n", tmp, prev);
   v = (RetByPriority(sl, i, col))->data;			//fatal line 1808 <corrected>
   j = i-1;
   tj = RetByPriority(sl, j, col);
   prvL = RetByPriority(sl, j, col-1);
   if (iv) tmin = SetupTMin(sl, iv, cmin, col-1, i);		//need current minimum, because while-loop
   else tmin=cmin;						//changes it
   								//if no intervals just setup anything
   //printf("tmin[%d]=%f\n", col, tmin);
   //printf("tj=%p, prvL=%p, v=%p\n", tj, prvL, v);
   tm[col] = tmin;
   while (j>=0 && Compare(prev, prvL, tj->data, v, srt[col], gtypes[gpp], gtypes[gp], iv, tmin))
     {						//main comparison !!!!! :-) WORKS AT LAST !!! :-)
      //printf("\t<j=%d> SWAP [j+1]=[j]", j);
      sl[j+1] = sl[j];
      j--;
      tj = RetByPriority(sl, j, col);
      prvL = RetByPriority(sl, j, col-1);
     }
   //printf("tmp=%p, j+1=%d\n", tmp, j+1);
   sl[j+1] = tmp;
   //   printf("atmin[%d]=%f\n", col, tmin);
   //   printf("%s\n", RetText(tmp, gtypes[pri[col]]));
  }
 //printf("sort_by_column, col=%d ENDF\n", col);
}

/*void SortByColumn(squareList& sl, int col)		//OLD VERSION, WHEN FATAL ERRORS
{							//TO RECOVER AND HARD-DEBUG-TEST!
 if (!sl) return;
 void* min;
 int index;
 pList tmp;
 pList prev;
 pList tj;
 int gp = pri[col];
 int gpp = pri[((col-1)>=0)?(col-1):0];
 for (int i=0;i<rows;i++)
  {
   printf("<col=%d, i=%d>\n", col, i);
   tmp  = RetByPriority(sl, i, col);
   prev = RetByPriority(sl, i, col-1);
   min = tmp->data;
   index = i;
   for (int j=i+1;j<rows;j++)
     {
      printf("\t<j=%d>", j);
      tj = (RetByPriority(sl, j, col));
      if (Compare(prev,RetByPriority(sl, j, col-1), tj->data, min, srt[col], gtypes[gpp], gtypes[gp]))
        {
	 printf(" _swap_ ");
	 min = tj->data;
	 index = j;
        }
      ln();
     }
   tj = sl[i];
   sl[i] = sl[index];
   sl[index] = tj;
   //   printf("%s\n", RetText(tmp, gtypes[pri[col]]));
  }
}*/
void DisplaySl(squareList l)		//only for DEBUG, when Display to Window not done
{					//use it to see (on console) what really happened ;-)
 printf("DisplaySl\n");
 if (!l) { printf("NULL!\n"); return ; }
 printf("+++++++++++++++++++++++++++++++\n");
 pList tmp;
 for (int i=0;i<rows;i++)
   {
    tmp = RetByPriority(l, i, 0);
    for (int j=0;j<ncols;j++)
      {
       printf("|%-14s| ", RetText(tmp, gtypes[pri[j]]));
       tmp = tmp->p;
      }
    ln();
   }
 printf("+++++++++++++++++++++++++++++++\n");
}

//a,b to compare
//min current area starts, min+len ENDS
//buf <there write>
void DispatchWrite(double a, double b, double min, double len, char*& buf)	//write it to
{						//to string or not <DISPATCH INTERVAL WRITE>
 double aa = a - min;
 double bb = b - min;
 int ai = (int)aa /(int)len;
 int bi = (int)bb /(int)len;		//get INT COMPARISON <numbers are AREAS>
 int r = ai-bi;				//JNE WRT " " else WRT dane
 //printf("a=%f, b=%f, min=%f, len=%f, aa=%f, bb=%f, ai=%d, bi=%d, r=%d\n", a,b,min,len,aa,bb,ai,bi,r);
 if (!r) { sprintf(buf,"%-18s ", " "); return; }
 else
   {
    sprintf(buf,"[%-4.2f-%-4.2f]", min+(bi*len), min+((bi+1)*len));
    return ;
   }
}


inline void StdCheck()			//we don't want VERTICAL with GROUP <sensless>
{
 if (bgrp && grp) borient=false;
}

int CreatePCharFromSquareList(char*& rt, squareList sl)  //Creates Window Text from temporary SL
{							 //When SL means Sorted, Groupped, Intervalled
 char* buf = new char[50*((ncols>8)?ncols:8)];				 //array of lists
 char* buf2 = new char[50*((ncols>8)?ncols:8)];				 //then this object is deleted!
 if (!buf || !buf2) { trace("no memory for buf or buf2"); return 0; }
 pList tmp=NULL;
 pList tmph=NULL;
 if (!rt) return 0;
 if (title) sprintf(buf,"\t\t**** RAPORT %s ****\n", title);
 else sprintf(buf,"\t\t **** RAPORT NIENAZWANY ****\n");
 strcpy(rt, buf);
 StdCheck();
 if (borient==1)					//VERTICAL <COLUMNS>
  {
   for (int x=0;x<80;x++) buf[x] = '=';
   buf[80] = '\0';
   strcat(rt, buf);
   strcat(rt,"\n");
   for (int i=0;i<rows;i++)
     {
      tmph = RetPointer(hheader, pri[0]);
      tmp  = RetPointer(sl[i], pri[0]);
      for (int j=0;j<ncols;j++)
         {
          if (!baltn) sprintf(buf,"%-35s || ", (char*)tmph->data);
	  else sprintf(buf,"%-35s || ", altn[j]);
          strcat(rt, buf);
          tmph = tmph->p;
          if (gtypes[pri[j]]==TTEXT) sprintf(buf,"%-35s|\n", RetText(tmp,gtypes[pri[j]]));  //fatal line 1935
          else sprintf(buf,"%-35s|\n", RetText(tmp, gtypes[pri[j]]));
          strcat(rt, buf);
          tmp = tmp->p;
         }
      for (int x=0;x<80;x++) buf[x] = '-';
      buf[80] = '\0';
      strcat(rt, buf);
      strcat(rt,"\n");
     }
    for (int x=0;x<80;x++) buf[x] = '=';
    buf[80] = '\0';
    strcat(rt,"\n");
    strcat(rt, buf);
    strcat(rt,"\n");
    return 1;				//ENDS VERTICAL, RET 1 means OK
   }
  for (int x=0;x<100;x++) buf[x] = '*';	//TABULAR,
  buf[100] = '\0';
  strcat(rt, buf);
  strcat(rt,"\n");
  int ll=0;
  for (int i=0;i<ncols;i++)		//WHICH COLUMN, and compute row_length
   {
    sprintf(buf2,"KOLUMNA [%-3d]", pri[i]);
    if (gtypes[pri[i]]==TTEXT) { ll+=40; sprintf(buf,"%-40s ", buf2); }
    else { ll+=18; sprintf(buf,"%-18s ", buf2); }
    strcat(rt, buf);
   }
  strcat(rt,"\n");
  for (int x=0;x<ll;x++) buf[x] = '=';
  buf[ll] = '\0';
  strcat(rt, buf);
  strcat(rt,"\n");
  tmp = RetPointer(hheader,  pri[0]);
  for (int z=0;z<ncols;z++)		//headers
   {
    if (!baltn)			//normal
     {
      if (gtypes[pri[z]]==TTEXT) sprintf(buf,"%-40s ", (char*)(tmp->data));
      else sprintf(buf,"%-18s ", (char*)tmp->data);
     }
    else			//alternative
     {
      if (gtypes[pri[z]]==TTEXT) sprintf(buf,"%-40s ", altn[z]);
      else sprintf(buf,"%-18s ", altn[z]);
     }
    tmp = tmp->p;
    strcat(rt, buf);
   }
  strcat(rt,"\n");
  for (int x=0;x<ll;x++) buf[x] = '=';
  buf[ll] = '\0';
  strcat(rt,"\n");
  strcat(rt, buf);
  strcat(rt,"\n");
  pList prev = NULL;
  pList tmpp=NULL;
  int k=0;
  double* minc = new double[ncols];
  //double* tmin = new double[ncols];
  if (!minc) { trace("no memory for double* minc"); return 0; }	//table of INTERVAL MINIMUMS
  								//THEY'RE FIRST in column
								//because they're alredy sorted
  for (int i=0;i<rows;i++)				//for all rows
    {
     tmp = RetPointer(sl[i], pri[0]);
     tmpp = tmp;
     for (int j=0;j<ncols;j++)
       {
        if (!grp[j])		//no group
	  {
           if (gtypes[pri[j]]==TTEXT) sprintf(buf,"%-40s ", RetText(tmp,gtypes[pri[j]]));
           else sprintf(buf,"%-18s ", RetText(tmp, gtypes[pri[j]]));
	  }
	else if (grp[j]==1)	//group
	  {
           if (!prev || !CompareSimpleEqual(prev->data, tmp->data, gtypes[pri[j]])) //difference or first
	     {
              if (gtypes[pri[j]]==TTEXT) sprintf(buf,"%-40s ", RetText(tmp,gtypes[pri[j]]));
              else sprintf(buf,"%-18s ", RetText(tmp, gtypes[pri[j]]));
	     }
	   else			//equal write " "
	     {
              if (gtypes[pri[j]]==TTEXT) sprintf(buf,"%-40s ", " ");
              else sprintf(buf,"%-18s ", " ");
	     }
	  }
	else			//INTERVALS
	  {			//fatal, cannot interval non-number
	   if (gtypes[pri[j]]!=TFLOAT) { trace("used interval for non-numeric column, aborted"); return 0; }
           if (!i)
	     {
	      //minc[j] = *(double*)(RetByPriority(sl, 0, j)->data);	//when i=0, get MINIMUM AREA for j
	      if (srt[j]==1) minc[j] = tm[j];
	      else minc[j] = *(double*)(RetByPriority(sl, 0, j)->data);
	      //printf("minc[%d]=%f\n", j, minc[j]);
              //sprintf(buf,"%-18s ", RetText(tmp, gtypes[pri[j]]));
              sprintf(buf2,"[%-4.2f-%-4.2f]", minc[j], minc[j]+(double)interv[j]); //WRITE THIS
	      k = strlen(buf2);
	      strcpy(buf, buf2);
	      for (int u=k;u<19;u++) buf[u] = ' ';	//UP TO %-18s + one SPACE
	      buf[19]=0;
	     }
           else
	     {
		    	//this writes if neccessary (not inside interval area), and increases area range
	      DispatchWrite(*(double*)prev->data, *(double*)tmp->data, minc[j], (double)interv[j], buf2);
	      k = strlen(buf2);
	      strcpy(buf, buf2);
	      for (int u=k;u<19;u++) buf[u] = ' ';	//up to 19
	      buf[19]=0;
	     }
	  }
	strcat(rt, buf);
	tmp = tmp->p;
	if (prev) prev = prev->p;
       }
     strcat(rt, "\n");
     prev = tmpp;
    }
  for (int x=0;x<ll;x++) buf[x] = '=';
  buf[ll] = '\0';
  strcat(rt,"\n");
  strcat(rt, buf);
  strcat(rt,"\n");
 if (minc) { delete [] minc; minc=NULL; }		//dea(d)(l)lock variables
 if (buf)  { delete [] buf; buf=NULL; }
 if (buf2) { delete [] buf2; buf2=NULL; }
 return 1;
}

char* TransformDataTo_ppc(Wnd& w)	//Makes preparations for SHOW REPORT
{					//calls SORT, GROUP, INTERV & CREATES PCHAR to
 int ok=1;				//WRITE INTO ClipArea of WIndow
 int len=0;
 //int j=0;
 int ll=0;
 char* rt = NULL;
 char* buf;
 char* buf2;
 if (!buf || !buf2) return NULL;
 pList tmp;
 squareList sl=NULL;
 //squareList prv=NULL;
 StdCheck();
 int st = step;
 if (step)
  {
   buf = new char[60*((ncols>8)?ncols:8)];
   buf2= new char[60*((ncols>8)?ncols:8)];
   sl = new pList[rows];
   if (!sl) { trace("sl:Transform__pc error!"); return NULL; }
   for (int i=0;i<rows;i++) sl[i] = ppd[i];
   //prv = new pList[rows];
   //if (!prv) { trace("prv:Transform__pc error!"); return NULL; }
   //for (int i=0;i<rows;i++) prv[i] = NULL;
   if (step==1) ok = CheckGroup(w, NULL);
   else if (step==2) ok = CheckSort(w, NULL);
   else if (step==4) ok = CheckAltNames(w, NULL);
   step = st;
   if (!ok) { trace("Check: Transform__pc: error"); delete [] sl; return NULL; }
   len = 50*(ncols+1)*(rows+1)+0x180;
   if (borient) len *= 2;  //pniewaz wszystkie nazwy kolumn beda wyswietlana 2x
   rt = new char[len+2];
   //rt = new char[1024*1024];
   if (!rt) { trace("Transform__pc: rt_error"); delete[] sl; return NULL; }
   tm = new double[ncols];
   if (!tm) { trace("Transform__pc: tm_error"); delete[] sl; delete[] rt; return NULL; }
   for (int i=0;i<ncols;i++) tm[i]=0.0;
   //code here!
   int srt_num=0;
   while (srt_num<ncols && srt[srt_num]) srt_num++;
   int grp_num=0;
   while (grp_num<ncols && srt[grp_num]) grp_num++;
   for (int col=0;col<srt_num;col++)   //every column
     {
      //printf("NOW_SORORTING_COL_NUM ====> %d <====\n", col);
      SortByColumn(sl, col);
      //DisplaySl(sl);
     }
   //printf("LAST_STEP\n");		//LOTS of DEBUG HERE
   //DisplaySl(sl);
   //printf("PPD\n");
   //DisplaySl(ppd);
   int ok = CreatePCharFromSquareList(rt, sl);
   if (!ok) { w.CreateMBoxChild("Blad przy przetwarzaniu\nlisty do char*"); return NULL; }
   //FIXME erase this!
   //strcpy(rt, "Hello World!\n");
   delete [] sl;
   delete [] buf;
   delete [] buf2;
   delete [] tm;
   //panic("TransformDataTo__ppc {unimplemented event step>0}");
  }
 else
 {
  buf = new char[60*((ncols>8)?ncols:8)];
  buf2= new char[60*((ncols>8)?ncols:8)];
  ok = CheckPriority(w, NULL);
  step=0;
  if (!ok) return NULL;
  len = 50*(ncols+1)*(rows+1)+0x180;
  rt = new char[len+2];
  if (step==0) sprintf(rt," ==> Raport utworzony z kolumn w/g priorytetu:\n");
  else sprintf(rt," ==> Raport tabelaryczny\n");
  ll=0;
  for (int i=0;i<ncols;i++)
   {
    sprintf(buf2,"KOLUMNA [%-3d]", pri[i]);
    if (gtypes[pri[i]]==TTEXT) { ll+=40; sprintf(buf,"%-40s ", buf2); }
    else { ll+=18; sprintf(buf,"%-18s ", buf2); }
    strcat(rt, buf);
   }
  strcat(rt,"\n");
  for (int x=0;x<ll;x++) buf[x] = '=';
  buf[ll] = '\0';
  strcat(rt, buf);
  strcat(rt,"\n");
  tmp = hheader;
  tmp = RetPointer(tmp,  pri[0]);
  for (int z=0;z<ncols;z++)
   {
    if (gtypes[pri[z]]==TTEXT) sprintf(buf,"%-40s ", (char*)(tmp->data));
    else sprintf(buf,"%-18s ", (char*)tmp->data);
    tmp = tmp->p;
    strcat(rt, buf);
   }
  strcat(rt,"\n");
  for (int x=0;x<ll;x++) buf[x] = '=';
  buf[ll] = '\0';
  strcat(rt,"\n");
  strcat(rt, buf);
  strcat(rt,"\n");
  for (int i=0;i<rows;i++)
   {
    tmp = RetPointer(ppd[i], pri[0]);
    //j++;
    for (int j=0;j<ncols;j++)
      {
       if (gtypes[pri[j]]==TTEXT) sprintf(buf,"%-40s ", RetText(tmp,gtypes[pri[j]]));
       else sprintf(buf,"%-18s ", RetText(tmp, gtypes[pri[j]]));
       strcat(rt, buf);
       tmp = tmp->p;
      }
    //printf("j=%d\n", j);
    strcat(rt,"\n");
   }
  for (int x=0;x<ll;x++) buf[x] = '=';
  buf[ll] = '\0';
  strcat(rt,"\n");
  strcat(rt, buf);
  strcat(rt,"\n");
  delete [] buf;
  delete [] buf2;
 }
 return rt;
}


void StepShowFunc(Wnd& wnd, void* arg)		//Make next step in the creator
{
 if (!ppd)
   {
    wnd.CreateMBoxChild("Nie utworzono jeszcze tanlicy glow list");
    return ;
   }
 char* fbuf = TransformDataTo_ppc(wnd);
 if (!fbuf)
   {
    wnd.CreateMBoxChild("TransformDataTo_ppc zwrocilo NULL!");
    return ;
   }
 Wnd* w = new Wnd(wnd.RetDspStr(), 500, 500);
 if (!w) panic("memory exception in creating win_child!");
 w->WinDefaults("Baza Danych: Raport");
 w->DeclareResponseTable(MEDI_RESPONSES);
 w->SelectExposeFunc(StdExposeFunc);
 w->SelectConfigureFunc(DenyResizeFunc);
 w->SelectKeyPressFunc(KeyPressClipControl);
 w->SelectKeyReleaseFunc(eXtendedKeyRelease);
 w->SelectButtonPressFunc(eXtendedButtonPress);
 w->SelectButtonReleaseFunc(eXtendedButtonRelease);
 w->PrepareWndText(fbuf);
 w->EnableText(0);
 w->EnableFrames(0);
 w->MakeClipFromWindowText(16, 16, 484, 454);
 w->SetButtonsNumber(2);
 w->CreateButton(0, "Zamknij",390 , 470, StdOKFunc, true, 100);
 w->CreateButton(1, "Zapisz" ,280 , 470, SaveWndClipToFile, false, 100);
 w->DefaultResponse();
 if (w) delete w;
 wnd.Invalidate();
 delete [] fbuf;
}


pList TraverseList(pList tmp, int a)		//traverse list back and next by a
{
 if (!a) return tmp;
 //printf("tmp=%p, a=%d, <<%p >>%p\n", tmp, a, tmp->prev, tmp->next);
 pList wsk = tmp;
 if (a>0)
   {
    for (int i=0;i<a;i++)
      {
       if (wsk) wsk = wsk->next;
       else return NULL;
      }
   }
 else
   {
    for (int i=0;i<-a;i++)
      {
       if (wsk) wsk = wsk->prev;
       else return NULL;
      }
   }
 //printf("wsk=%p\n", wsk);
 return wsk;
}


void SetReportTitle(Wnd& w, void* arg)		//Read Function Name
{
 char* ans = w.CreateWriteBox("Podaj tytul raportu:");
 if (!ans || !strcmp(ans, "")) { w.CreateMBoxChild("Nazwa nie moze byc rowna NULL!"); return; }
 if (title) delete [] title;
 title = NULL;
 title = new char[strlen(ans)+2];
 if (!title) { w.CreateMBoxChild("Brak pamieci RAM"); return ; }
 strcpy(title, ans);
 char bufi[0x200];
 if (title) sprintf(bufi,"Wybierz aliasy dla kolumn oraz tytul raportu:\nAktualny: %s", title);
 else sprintf(bufi,"Wybierz aliasy dla kolumn oraz tytul raportu:\nAktualny: {NULL}");
 w.BlackWings();
 w.PrepareWndText(bufi);
 w.Invalidate();
}

int CheckPriority(Wnd& wnd, void* arg)	//Are colum priority OK
{					//And setup Program Variables
 bprior=false;
 int num=0;
 int cur;
 int err=0;
 for (int i=0;i<cols;i++)
   {
    if (!prior[i]) continue;
    else cur = prior[i];
    for (int j=i+1;j<cols;j++)
      {
       if (prior[j]==cur) { err=1; break; }
      }
    if (err) break;
    num++;
   }
 if (err) { wnd.CreateMBoxChild("Priorytety kolumn nie moga\nsie powtarzac\n"
		 "wybierz 0, aby nie uzywac kolumny\nlub kolejno 1,2,3 dla uzywanych"); return 0; }
 if (num==0) { wnd.CreateMBoxChild("Zaznacz conajmniej jedna kolumne\njako priorytet > 0"); return 0; }
 ncols = num;
 if (pri) delete [] pri;
 pri = NULL;
 pri = new int[ncols];
 for (int i=0;i<ncols;i++) pri[i] = -1;
 if (!pri) { trace("new pri in CheckPriority"); return 0; }
 for (int i=0;i<cols;i++)
   {
    if (prior[i]) pri[prior[i]-1] = i;
   }
 for (int i=0;i<ncols;i++)  if (pri[i]==-1)
   {
    wnd.CreateMBoxChild("Nie moga wystepowac przerwy w priorytetach\njezeli np zdefiniowano priorytet 1,3\nto priorytet 2 tez musi wystapic!");
    ncols = -1;
    return 0;
   }
 //for (int i=0;i<ncols;i++) printf("pri[%d]=%d\n", i, pri[i]);
/* pList tmp = hheader;
 for (int i=0;i<ncols-1;i++)
   {
    tmp = RetPointer(hheader, pri[i]);			//get actual using header
    tmp->p = RetPointer(hheader, pri[i+1]);		//connect with next
   }
 (RetPointer(hheader, pri[ncols-1]))->p = NULL;*/
 pList tmp = RetPointer(hheader, pri[0]);		//makes all Priority connections
 if (!tmp) return 0;					//by list addictional pointer ,,p''
 int trv;
 for (int i=0;i<ncols-1;i++)
   {
    trv = pri[i+1] - pri[i];
    //printf("trv=%d\n", trv);
    tmp->p = TraverseList(tmp, trv);
    //printf("tmp->data=%s\n", (char*)tmp->data);
    tmp = tmp->p;
   }
 (RetPointer(hheader, pri[ncols-1]))->p = NULL;		//last should point to NULL
 for (int i=0;i<rows;i++)				//can go by this pointer only
   {							//one direction, cannot go back
    tmp = RetPointer(ppd[i], pri[0]);			//but it is not needed
    if (!tmp) return 0;
    for (int j=0;j<ncols-1;j++)
      {
       trv = pri[j+1] - pri[j];
       tmp->p = TraverseList(tmp, trv);
       tmp = tmp->p;
      }
    (RetPointer(ppd[i], pri[ncols-1]))->p = NULL;
   // printf("\n");
   }
/* tmp = RetPointer(ppd[0], pri[0]);
 int z = 0;
 while (tmp)
  {
   //printf("%s\n", (char*)tmp->data);  //for hheader
   //printf("%s\n", RetText(tmp, gtypes[pri[z]])); //for ppd[i]
   tmp = tmp->p;
   z++;
  }*/
 if (grp) { delete [] grp; grp = NULL; }			//RESET many variables
 grp = new int[ncols];
 if (interv) {delete [] interv; interv = NULL;}
 interv = new int[ncols];
 if (srt) { delete [] srt; srt = NULL; }
 srt = new int[ncols];
 if (!grp) return 0;
 if (!interv) return 0;
 if (!srt) return 0;
 for (int i=0;i<ncols;i++) { grp[i] = 0; interv[i] = 0; srt[i] = 0;}
 altn = new char*[ncols];
 if (!altn) return 0;
 tmp = RetPointer(hheader, pri[0]);
 for (int i=0;i<ncols;i++)
   {
    altn[i] = new char[strlen((char*)tmp->data)+3];
    if (!altn[i]) return 0;
    strcpy(altn[i], (char*)tmp->data);			//GET ALTNames = CURnames
    tmp = tmp->p;
   }
 step++;
 bprior = true;
 return 1;
}


int CheckGroup(Wnd& w, void* arg)			//Is Grouping OK?
{
 for (int i=0;i<ncols;i++) { if (grp[i]>0) srt[i] = 1; }
 bgrp = false;
 bintv = 0;
 if (grp[0]>0) bgrp = true;
 for (int i=0;i<ncols;i++)
   {
    if (grp[i]==2 && gtypes[pri[i]]==TFLOAT) { bintv=true;  step++; return 1; }
    if (grp[i]==2 && gtypes[pri[i]]!=TFLOAT) { bgrp =false; return 0; }
   }
 step++;
 return 1;
}


int CheckSort(Wnd& w, void* arg)			//What about the Sort?
{
 bsrt = false;
 step++;
 for (int i=0;i<ncols;i++) if (srt[i]) { bsrt=true;  return 1; }
 return 1;
}


int CheckAltNames(Wnd& w, void* arg)			//no comment
{
 baltn = false;
 if (!altn) return 0;
 for (int i=0;i<ncols;i++) if (!altn[i]) return 0;
 step++;
 baltn = true;
 return 1;
}

void StepNextFunc(Wnd& w, void* arg)		//Meke one step in creator
{						//if Check Are OK
 int ok = 0;
 if (step==0) ok = CheckPriority(w, NULL);
 else if (step==1) ok = CheckGroup(w, NULL);
 else if (step==2) ok = CheckSort(w, NULL);
 else if (step==3) { ok = 1; step++; }	//orientacja
 else if (step==4) ok = CheckAltNames(w, NULL);
 //printf("step=%d, ok=%d\n", step, ok);
 if (!ok) return;
 w.DoQuit();
 w.Invalidate();
}

void StepPrevFunc(Wnd& w, void* arg)		//Go Prev <Quit SImple>
{						//because all sort-like-stuff
 step--;					//is done just-in-time
 if (step<0) step = -1;				//when Showing Report
 w.DoQuit();					//orginal data is not changed
 w.Invalidate();
}


void GenerateFunc(Wnd& wnd, void* ptr)		//main program dialog when pressed 'dalej'
{
 tout("GenerateFunc");
 if (!fopened)
   {
    wnd.CreateMBoxChild("Aby rozpoczac generowanie raportu\nnalezy wczytac plik .DAT!");
    return ;
   }
 if (!screated)
   {
    wnd.CreateMBoxChild("Okresl separatory i format daty\naby aplikacja mogla przetworzyc\nplik .DAT\nUzyj opcji Otworz");
    return ;
   }
 int ok = SeqRead(wnd);
 if (!ok)
   {
    wnd.CreateMBoxChild("Nie udalo sie odczytac danych!");
    return ;
   }
 Wnd* w;
 char bufi[0x200];
 step = 0;
 while (step>=0)
 {
 switch (step)
   {
    case 0:						//case n, means creator_step n+1
      w = new Wnd(wnd.RetDspStr(), 500, 430);
      if (!w) panic("memory exception in creating win_child!");
      w->WinDefaults("Krok 1: Priorytety kolumn do raportu");
      w->DeclareResponseTable(MEDI_RESPONSES);
      w->SelectExposeFunc(StdExposeFunc);
      w->SelectConfigureFunc(DenyResizeFunc);
      w->SelectKeyPressFunc(eXtendedKeyPressQ);
      w->SelectKeyReleaseFunc(eXtendedKeyRelease);
      w->SelectButtonPressFunc(eXtendedButtonPress);
      w->SelectButtonReleaseFunc(eXtendedButtonRelease);
      w->SetParent(&wnd);
      w->PrepareWndText("Wybierz kolumny:");
      w->StdTextWrite();
      w->SetButtonsNumber(6+cols);
      SetupColBtns(*w, NULL);
      w->SetParent(&wnd);
      w->CreateButton(cols+0, "Dalej",300 , 330, StepNextFunc, true, 100);
      w->CreateButton(cols+1, "Wstecz",100 , 330, StepPrevFunc, false, 100);
      w->CreateButton(cols+2, "Pokaz",300 , 360, StepShowFunc, false, 100);
      w->CreateButton(cols+3, "Zakoncz",100 , 360, GrandFinale, false, 100);
      w->CreateButton(cols+4, "Cala baza",100 , 390, DisplayAll, false, 100);
      w->CreateButton(cols+5, "Wszystkie",300 , 390, SelectAll, false, 100);
      w->DefaultResponse();
      if (w) delete w;
      wnd.Invalidate();
      break;
    case 1:
      w = new Wnd(wnd.RetDspStr(), 500, 430);
      if (!w) panic("memory exception in creating win_child!");
      w->WinDefaults("Krok 2: Opcje grupowania kolumn");
      w->DeclareResponseTable(MEDI_RESPONSES);
      w->SelectExposeFunc(StdExposeFunc);
      w->SelectConfigureFunc(DenyResizeFunc);
      w->SelectKeyPressFunc(eXtendedKeyPressQ);
      w->SelectKeyReleaseFunc(eXtendedKeyRelease);
      w->SelectButtonPressFunc(eXtendedButtonPress);
      w->SelectButtonReleaseFunc(eXtendedButtonRelease);
      w->SetParent(&wnd);
      w->PrepareWndText("Format: [0<0], pierwsza liczba: 0 brak grupow, 1 grupow, 2 interwal\ndruga liczba (jezeli pierwsza=2) dlugosc interwalu\nWybierz kolumny:");
      w->StdTextWrite();
      w->SetButtonsNumber(5+ncols);
      SetupColSelBtns(*w, NULL);
      w->SetParent(&wnd);
      w->CreateButton(ncols+0, "Dalej",300 , 320, StepNextFunc, true, 100);
      w->CreateButton(ncols+1, "Wstecz",100 , 320, StepPrevFunc, false, 100);
      w->CreateButton(ncols+2, "Pokaz",300 , 360, StepShowFunc, false, 100);
      w->CreateButton(ncols+3, "Zakoncz",100 , 360, GrandFinale, false, 100);
      w->CreateButton(ncols+4, "Cala baza",200 , 390, DisplayAll, false, 100);
      w->DefaultResponse();
      if (w) delete w;
      wnd.Invalidate();
      break;
    case 2:
      w = new Wnd(wnd.RetDspStr(), 500, 430);
      if (!w) panic("memory exception in creating win_child!");
      w->WinDefaults("Krok 3: Opcje sortowania kolumn");
      w->DeclareResponseTable(MEDI_RESPONSES);
      w->SelectExposeFunc(StdExposeFunc);
      w->SelectConfigureFunc(DenyResizeFunc);
      w->SelectKeyPressFunc(eXtendedKeyPressQ);
      w->SelectKeyReleaseFunc(eXtendedKeyRelease);
      w->SelectButtonPressFunc(eXtendedButtonPress);
      w->SelectButtonReleaseFunc(eXtendedButtonRelease);
      w->SetParent(&wnd);
      w->PrepareWndText("Wybierz sortowania: -1-malejace, 0-brak, 1-rosnace:");
      w->StdTextWrite();
      w->SetButtonsNumber(5+ncols);
      SetupColSortBtns(*w, NULL);
      w->SetParent(&wnd);
      w->CreateButton(ncols+0, "Dalej",300 , 320, StepNextFunc, true, 100);
      w->CreateButton(ncols+1, "Wstecz",100 , 320, StepPrevFunc, false, 100);
      w->CreateButton(ncols+2, "Pokaz",300 , 360, StepShowFunc, false, 100);
      w->CreateButton(ncols+3, "Zakoncz",100 , 360, GrandFinale, false, 100);
      w->CreateButton(ncols+4, "Cala baza",200 , 390, DisplayAll, false, 100);
      w->DefaultResponse();
      if (w) delete w;
      wnd.Invalidate();
      break;
      case 3:
      borient = false;
      w = new Wnd(wnd.RetDspStr(), 300, 150);
      if (!w) panic("memory exception in creating win_child!");
      w->WinDefaults("Krok 4: Uklad raportu");
      if (!bgrp)
        {
         w->SetButtonsNumber(7);
         w->CreateButton(5, "Tabelkowy" ,25  , 30, SetOrient, false, 100, 25, (void*)false);
         w->CreateButton(6, "Kolumnowy" ,175 , 30, SetOrient, false, 100, 25, (void*)true);
        }
      else
	{
         w->PrepareWndText("Grupowania zaznaczone, niedostepna opcja ukladu\nNacisnij Dalej aby kontynuowac");
         w->StdTextWrite();
	 w->SetButtonsNumber(5);
	}
      w->CreateButton(0, "Dalej",175 , 60, StepNextFunc, true, 100);
      w->CreateButton(1, "Wstecz",25 , 60, StepPrevFunc, false, 100);
      w->CreateButton(2, "Pokaz",175 , 90, StepShowFunc, false, 100);
      w->CreateButton(3, "Zakoncz",25 , 90, GrandFinale, false, 100);
      w->CreateButton(4, "Cala baza",100 , 120, DisplayAll, false, 100);
      w->SetParent(&wnd);
      w->DefaultResponse();
      if (w) delete w;
      wnd.Invalidate();
      break;
    case 4:
      w = new Wnd(wnd.RetDspStr(), 500, 430);
      if (!w) panic("memory exception in creating win_child!");
      w->WinDefaults("Krok 5: Alternatywne nazwy kolumn");
      w->DeclareResponseTable(MEDI_RESPONSES);
      w->SelectExposeFunc(StdExposeFunc);
      w->SelectConfigureFunc(DenyResizeFunc);
      w->SelectKeyPressFunc(eXtendedKeyPressQ);
      w->SelectKeyReleaseFunc(eXtendedKeyRelease);
      w->SelectButtonPressFunc(eXtendedButtonPress);
      w->SelectButtonReleaseFunc(eXtendedButtonRelease);
      w->SetParent(&wnd);
      if (title) sprintf(bufi,"Wybierz aliasy dla kolumn oraz tytul raportu:\nAktualny: %s", title);
      else sprintf(bufi,"Wybierz aliasy dla kolumn oraz tytul raportu:\nAktualny: {NULL}");
      w->PrepareWndText(bufi);
      w->StdTextWrite();
      w->SetButtonsNumber(6+ncols);
      SetupColAltBtns(*w, NULL);
      w->SetParent(&wnd);
      w->CreateButton(ncols+0, "ZAKONCZ",300 , 320, StepNextFunc, true, 100);
      w->CreateButton(ncols+1, "Wstecz",100 , 320, StepPrevFunc, false, 100);
      w->CreateButton(ncols+2, "Raport",300 , 360, StepShowFunc, false, 100);
      w->CreateButton(ncols+3, "Zakoncz",100 , 360, GrandFinale, false, 100);
      w->CreateButton(ncols+4, "Cala baza",100 , 390, DisplayAll, false, 100);
      w->CreateButton(ncols+5, "Tytul",300 , 390, SetReportTitle, false, 100);
      w->DefaultResponse();
      if (w) delete w;
      wnd.Invalidate();
      break;
     default:
      if (step>0)
        {
	 StepShowFunc(wnd, NULL);
	 wnd.CreateMBoxChild("Gratuluje ukonczenia raportu\nten program mozna uzywac na\nlicencji GNU OpenSource"); step = -1;
	}
   }
 }
}


void DontDoThis(Wnd& w, void* p)		//ShutDown Application?
{
 int yes = w.CreateYesNoChild("Czy na pewno chcesz zakonczyc?");
 if (!yes) return ;
 StdOKFunc(w,p);
}


int main(int lb, char** par)			//As everybody knows, thes C++ programs starts...
{
 tout("Thread started!");
 printf("Projekt Indywidualny; Lukasz Gryglicki (MorgothDBMA)\n");
 printf("Kompilacja: plik: [%s]; linia: [%d]; data: [%s %s]\n", __FILE__, __LINE__, __DATE__, __TIME__);
 if (lb>=2 && (par[1][1]=='h' || par[1][0]=='h')) { system("cat ./README | more"); exit(0); }
 printf("SetUp Graphic Engine...\n");
 fopened = false;
 init_random();
 init_vars();
 Dsp* main_dsp = new Dsp;
 if (!main_dsp) panic("memory_exception_in_new",__LINE__,__FILE__);
 Wnd* wnd = new Wnd(main_dsp, 400, 200);
 if (!wnd) panic("memory_exception_in_new",__LINE__,__FILE__);
 wnd->CreateGC();
 wnd->SetFont("*-clean-*-10-*");
 wnd->SetCaption("Projekt indywidualny - Lukasz Gryglicki");
 wnd->DeclareResponseTable(MEDI_RESPONSES);
 wnd->SelectExposeFunc(StdExposeFunc);
 wnd->SelectConfigureFunc(DenyResizeFunc);
 wnd->SelectKeyPressFunc(eXtendedKeyPressQ);
 wnd->SelectKeyReleaseFunc(eXtendedKeyReleaseND);
 wnd->SelectButtonPressFunc(eXtendedButtonPress);
 wnd->SelectButtonReleaseFunc(eXtendedButtonReleaseND);
 wnd->PrepareWndText("Nacisnij przycisk Otworz aby wczytac dane z pliku\nakceptowane sa pliki .DAT\nNacisniecie 'Q' zamyka okno");
 wnd->StdTextWrite();
 wnd->SetButtonsNumber(4);
 wnd->CreateButton(0, "Otworz",60 , 100, OpenDataFile, true, 100);
 wnd->CreateButton(1, "Dalej",240 , 100, GenerateFunc, false, 100);
 wnd->CreateButton(2, "Autor",60 , 160, AutorFunc, false, 100);
 wnd->CreateButton(3, "Koniec",240 , 160, DontDoThis, false, 100);
 wnd->DefaultResponse();
 if (wnd) delete wnd;
 if (main_dsp) delete main_dsp;
 delete_vars();
 kill_random();
 printf("Rei Ayonnami\n");
 tout("Thread stopped");
 return 0;
}

//EOF
