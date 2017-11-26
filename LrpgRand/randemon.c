#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <mran.h>
#include <termios.h>
#define OK 0;
#define ATTRIB        11
#define S_ATTRIB      11
#define MAX_ATT       16
#define STRING        45
#define COMMON        2
#define MAX_TYPE      150
#define TRUE          1
#define FALSE         0
#define MAX_SKI       350
#define MULTIPLE      0
#define ATTRIBUTE     1
#define MOD           2
#define DIFF          3
#define CATEG         4
#define MAGIC         5
#define MAX_CATEG     10
#define MAX_LEVELS    25
#define MAX_EKW       60
#define MAX_EKW_P     30
#define DANE_POSTACI  15
#define IMIE          0
#define NAZWISKO      1
#define RASA          2
#define PROFESJA      3
#define CHARAKTER     4
#define OCZY          5
#define WLOSY         6
#define CERA          7
#define WZROST        8
#define WAGA          9
#define PLEC          10
#define WIEK           11
#define MAX_UM        80
#define ALL           2
#define DIR_EXEC      0777
#define MAX_LEV_S     25
#define MAX_SPELL     40
#define SHSTR         40
#define MEDSTR        40
#define USER          -1
#define ASK	      -2
#define COST          0x200
#define REQ           0x201
#define MAX_SP        125
#define BIG           200
#define SECRET        0xfade
#define HIDDEN        0xfada
#define IL_LIT 'z'-'a'+1

static int CLEAR = 45;
FILE* lg;

struct DATA
{
 DATA();
 ~DATA();
 int ilosc[ATTRIB];
 int mod[COMMON][MAX_TYPE][MAX_ATT];
 char name[COMMON][MAX_TYPE][STRING];
 int value_att[MAX_ATT];
 char name_att[MAX_ATT][STRING];
 char s_name[MAX_SKI][STRING];
 int s_mod[MAX_SKI][S_ATTRIB];
 char c_name[MAX_CATEG][STRING];
 int cost[MAX_LEVELS];
 int pd_cost[MAX_LEVELS];
 int sv;
 int st;
 int p_mod[MAX_ATT];
 int lr;
 int mm;
 char dbname[STRING];
 int ilosc_ekw;
 char ekw[MAX_EKW][STRING];
 int c_ekw[MAX_EKW];
 int def_cash;

 int ilosc_poziomow;
 int ilosc_czarow[MAX_LEV_S];
 int spell_cost[MAX_LEV_S][MAX_SPELL];
 char req_magic[MAX_LEV_S][MAX_SPELL][SHSTR];
 char zaklecie[MAX_LEV_S][MAX_SPELL][MEDSTR];
};



DATA :: DATA()
{
 ilosc_poziomow = 0;
 for (int i=0;i<MAX_LEV_S;i++)
        {
         ilosc_czarow[i] = 0;
         for (int j=0;j<MAX_SPELL;j++)
                {
                 spell_cost[i][j] =(i+1) *(i+2) + j;
                 strcpy(req_magic[i][j],"RENEGACJA");
                 strcpy(zaklecie[i][j],"SOME_SPELL");
                }
        }
 ilosc[0] = 13;
 ilosc[1] = 10;
 ilosc[2] = 20;
 ilosc[3] = 150;
 ilosc[4] = 6;
 ilosc[5] = 15;
 ilosc[6] = 15;
 ilosc_ekw = 10;
 sv = 0;
 st = 1;
 lr = 0;
 mm = 0;
 strcpy(dbname,"DataBase/default");
 for (int i=0;i<MAX_ATT;i++)
        {
         value_att[i] = 10;
         p_mod[i]     = 0;
         strcpy(name_att[i],"DEFAULT_ATTR");
        }
 for (int i=0;i<MAX_TYPE;i++)
        {
         strcpy(name[0][i],"DEFAULT_RACE");
         strcpy(name[1][i],"DEFAULT_PROF");
         for (int j=0;j<MAX_ATT;j++)
                {
                 mod[0][i][j] = 0;
                 mod[1][i][j] = 0;
                }
        }
 for (int i=0;i<MAX_SKI;i++)
        {
         strcpy(s_name[i],"DEFAULT_SKILL");
         s_mod[i][MULTIPLE] = 3;
         s_mod[i][ATTRIBUTE] = 0;
         s_mod[i][MOD] = 0;
         s_mod[i][DIFF] = 1;
         s_mod[i][CATEG] = 0;
         s_mod[i][MAGIC] = 0;
        }
 for (int i=0;i<MAX_CATEG;i++)
        {
         strcpy(c_name[i],"DEFAULT_CATEG");
        }
 for (int i=0;i<MAX_LEVELS;i++) {cost[i] = 2*i+1;pd_cost[i] = cost[i];}
 for (int i=0;i<MAX_EKW;i++) {strcpy(ekw[i],"DEFAULT_ITEM"); c_ekw[i] = 40;}
 def_cash = 400;
 fprintf(lg,"stworzylem strukture DATA\n");
}
DATA::~DATA(){fprintf(lg,"zniszczylem strukture DATA\n");}
struct RPG
{
 RPG();
 ~RPG();
 int ilosc_danych;
 int ilosc_cech;
 int sv;
 int st;
 int cecha[MAX_ATT];
 char n_cechy[MAX_ATT][STRING];
 char dane[DANE_POSTACI][STRING];
 int il_um;
 char um[MAX_UM][STRING];
 int pozum[MAX_UM];
 int tesum[MAX_UM];
 int trum[MAX_UM];
 int ceum[MAX_UM];
 int mulum[MAX_UM];
 int magum[MAX_UM];
 int il_ekw;
 int PD;
 int p_c[MAX_ATT];
 int p_u[MAX_UM];
 char ekw[MAX_EKW_P][STRING];
 int rl;
 int stMM;
 int numMM;
 int IsDead;
 int lev;
 int lev_p;
 char pname[STRING];
 int cash;

 int il_spell;
 int sp;
 int spt;
 int max_lev;
 char spell[MAX_SP][MEDSTR];
 int spell_cost[MAX_SP];
 int spell_level[MAX_SP];
 int mage;
};
RPG :: RPG()
{
 ilosc_danych = 12;
 ilosc_cech = 13;
 strcpy(pname,"Characters/default");
 sv = 0;
 st = 1;
 mage = 0;
 cash = 100;
 il_um = 0;
 il_ekw = 0;
 PD = 0;
 rl = 0;
 lev = 0;
 lev_p = 0;
 IsDead = 0;
 sp = 10;
 spt = 2;
 il_spell = 0;
 max_lev = 0;
 for (int i=0;i<MAX_SP;i++)
        {
         strcpy(spell[i],"SOME_SPELL");
         spell_cost[i] = 5;
         spell_level[i] = 1;
        }
 for (int i=0;i<DANE_POSTACI;i++)
        {
         strcpy(dane[i],"SOME_DATA");
        }
 for (int i=0;i<MAX_ATT;i++)
        {
         cecha[i] = 10;
         p_c[i] = 0;
         strcpy(n_cechy[i],"SOME_DATA");
        }
 stMM = 10;
 for (int i=0;i<MAX_UM;i++)
        {
         strcpy(um[i],"SOME_SKILL");
         tesum[i] = 0;
         pozum[i] = 0;
         trum[i]  = 1;
         mulum[i] = 3;
         ceum[i]  = 0;
         p_u[i]   = 0;
         magum[i] = 0;
        }
 for (int i=0;i<MAX_EKW_P;i++)
        {
         strcpy(ekw[i],"SOME_EQUIP");
        }
 fprintf(lg,"stworzylem strukture RPG\n");
}
RPG::~RPG(){fprintf(lg,"Zniszczylem strukture RPG\n");}
inline void ln(){cout<<endl;}
void lns(int par){for (int i=0;i<par;i++) ln();}

inline char losuj_litere()
{
 return 'a' + random(IL_LIT);
}
char losuj_spolgl()
{
 int los = random(18);
 if (los==0) return 'b';
 if (los==1) return 'c';
 if (los==2) return 'd';
 if (los==3) return 'f';
 if (los==4) return 'g';
 if (los==5) return 'h';
 if (los==6) return 'i';
 if (los==7) return 'j';
 if (los==8) return 'k';
 if (los==9) return 'l';
 if (los==10) return 'm';
 if (los==11) return 'n';
 if (los==12) return 'p';
 if (los==13) return 'r';
 if (los==14) return 's';
 if (los==15) return 't';
 if (los==16) return 'w';
 if (los==17) return 'z';
}
void down(char& zn)
{
 if ((zn >= 'A') && (zn <= 'Z')) zn += 0x20;
}

int spolgl(char p)
{
 down(p);
 if ((p == 'b') ||(p == 'c') ||(p == 'd') ||(p == 'f') ||(p == 'g') ||(p == 'h') ||(p == 'j') ||(p == 'k') ||(p == 'l') ||(p == 'm') ||(p == 'n') ||(p == 'p') ||(p == 'q') ||(p == 'r') ||(p == 's') ||(p == 't') ||(p == 'v') ||(p == 'w') ||(p == 'x') ||(p == 'z')) return 1;
 return 0;
}
int samogl(char p)
{
 down(p);
 if ((p == 'a') || (p == 'e') || (p == 'i') || (p == 'o') || (p == 'u') || (p == 'y')) return 1;
 return 0;
}
char losuj_samogl()
{
 int los = random(6);
 if (los==0) return 'a';
 if (los==1) return 'e';
 if (los==2) return 'i';
 if (los==3) return 'o';
 if (los==4) return 'u';
 if (los==5) return 'y';
}
void takestring(char* imie, int def = 0, char* w1 = NULL, char* w2 = NULL, char* w3 = NULL)
{
 int len = (random(4)+1)*(random(5)+1)+2;
 int tmp = 0;
 imie[0] = losuj_litere();
 imie[0] -= 0x20;
 if (spolgl(imie[0])) imie[1] = losuj_samogl();
 else imie[1] = losuj_spolgl();
 for (int i=2;i<len;i++)
        {
        if (samogl(imie[i-1]) && samogl(imie[i-2])) imie[i] = losuj_spolgl();
        else if (spolgl(imie[i-1]) && samogl(imie[i-1]))
                {
                 tmp = random(10);
                 if (tmp<=6) imie[i] = losuj_spolgl();
                 else imie[i] = losuj_samogl();
                }
        else if (samogl(imie[i-1]) && spolgl(imie[i-1]))
                {
                 tmp = random(10);
                 if (tmp<=4) imie[i] = losuj_spolgl();
                 else imie[i] = losuj_samogl();
                }
        else imie[i] = losuj_samogl();
        }
 imie[len] = '\0';
 if (def)
        {
         int ran;
         if (def == 1) strcpy(imie, w1);
         else if (def == 2)
                {
                 ran = random(2);
                 if (ran) strcpy (imie, w1);
                 else strcpy(imie, w2);
                }
         else if (def == 3)
                {
                 ran = random(3);
                 if (!ran) strcpy (imie, w1);
                 else if (ran == 1) strcpy(imie, w2);
                 else strcpy(imie, w3);
                }
         else
                {
                 ran = random(def);
                 if (!ran) strcpy(imie, w1);
                }
        }
 cout<<"\n\n\nLOSOWANIE ZWROCILO:\t"<<imie<<endl<<endl;
}

void takeint(int* par, int def = 0, int minus = 0)
{
 if (def) *par = random(def);
 else * par = random(random(100));
 if (minus)
        {
         int ran = random(2);
         if (ran) *par *= (-1);
        }
 cout<<"\n\n\nLOSOWANIE ZWROCILO:\t"<<(*par)<<endl<<endl;
}

void downcase(char* str)
{
 for (int i=0;i<(int)strlen(str);i++) if ((str[i] >= 'A') && (str[i] <= 'Z')) str[i] += (char)0x20;
}
int getch(int clean = 0, char z1 = 't', char z2 = 'n', char z3 = 'k', char z4 = 'a')
{
 int ret;
 if (!clean) ret= losuj_litere();
 else
        {
         int ran = random(clean)+1;
         if (ran == 1) ret= z1;
         else if (ran == 2) ret= z2;
         else if (ran == 3) ret= z3;
         else if (ran == 4) ret= z4;
         else ret= losuj_litere();
        }
 cout<<"\n\n\nLOSOWANIE ZWROCILO:\t"<<(char)ret<<endl<<endl;
 return ret;
}

int ncont()
{
 fprintf(lg,"jezeli uzytkownik kontynuowal od tej chwili\nto bledy wystapia prawie na pewno!\n");
 cout<<"kontynuacja jest niebezpieczna nacisnij X aby kontynuowac [X] ";
 int zn = getch(1,'x');
 ln();
 if (zn == 'X') return 0;
 else return 1;
}

void secretake(int& lb)
{
 char dat[STRING];
 takestring(dat, 1,"123");
 sscanf(dat,"%d",&lb);
}
void takechar(char* zn, int clean = 0, char z1 = 't', char z2 = 'n', char z3 = 'k', char z4 = 'a')
{
 *zn = getch(clean, z1, z2, z3, z4);
 ln();
}
void AboutDisplay(char* name, int& pos)
{
 fprintf(lg,"znalazlem %s\n", name);
 char nname[STRING];
 strcpy(nname, name);
 downcase(name);
 if ((strcmp(name,"info") == 0) || (strcmp(name,"instalacja") == 0) || (strcmp(name,"backup_file") == 0)) return;
 int len = strlen(name);
 if ((name[0] == '_') || (name[0] == '-')) return;
 if (len <= 4)
 	{
    if (pos < 3){printf("%-17s ",nname);pos++;}
    else {printf("%-17s\n",nname); pos = 0;}
    return;
   }
 if ((strcmp(name,"info") == 0) || (strcmp(name,"instalacja") == 0) || (strcmp(name,"backup_file") == 0)) return;
 for (int i=0;i<len;i++) if ((name[i] == '.') || (name[i] == ' ')) return;
 if ((name[len-1] == 'n') && (name[len-2] == 'i') && (name[len-3] == 'b') && (name[len-4] == '_')) return;
 else if ((name[len-1] == 'g') && (name[len-2] == 'o') && (name[len-3] == 'l') && (name[len-4] == '_')) return;
 else if ((name[len-1] == 'f') && (name[len-2] == 'n') && (name[len-3] == 'i') && (name[len-4] == '_')) return;
 else if ((name[0] == 'k') && (name[1] == 'o') && (name[2] == 'p') && (name[3] == 'i')) return;
 else
 	{
    if (pos < 3){printf("%-17s ",nname);pos++;}
    else {printf("%-17s\n",nname); pos = 0;}
   }
}

void AboutDisplayChar(char* name, int& pos)
{
 fprintf(lg,"znalazlem %s\n", name);
 char nname[STRING];
 strcpy(nname, name);
 downcase(name);
 int len = strlen(name);
 if ((name[0] == '_') || (name[0] == '-')) return;
 if (len <= 4)
 	{
    if (pos < 2){printf("%-25s ",nname);pos++;}
    else {printf("%-25s\n",nname); pos = 0;}
    return;
   }
 if ((name[len-1] == 'n') && (name[len-2] == 'i') && (name[len-3] == 'b') && (name[len-4] == '_')) return;
 else if ((name[len-1] == 'g') && (name[len-2] == 'o') && (name[len-3] == 'l') && (name[len-4] == '_')) return;
 else if ((name[len-1] == 'f') && (name[len-2] == 'n') && (name[len-3] == 'i') && (name[len-4] == '_')) return;
 else if ((name[len-1] == 'l') && (name[len-2] == 'm') && (name[len-3] == 't') && (name[len-4] == 'h')) return;
 else if ((name[0] == 'k') && (name[1] == 'o') && (name[2] == 'p') && (name[3] == 'i')) return;
 else
 	{
    if (pos < 2){printf("%-25s ",nname);pos++;}
    else {printf("%-25s\n",nname); pos = 0;}
   }
}
void DecompressData(char* data)
{
 for (int i=0;i<STRING;i++) if (data[i] == '_') data[i] = ' ';
}
void CompressData(char* data)
{
 for (int i=0;i<STRING;i++) if (data[i] == ' ') data[i] = '_';
}

void wait()
{
 getch(1,'s');
}

void clear(int, int, int&);

void DisplayBases()
{
 fprintf(lg,"wyswietlam bazy danych\n");
 char kat[STRING];
 strcpy(kat,"./DataBase");
 DIR* dp;
 int pos = 0;
 int i = 0;
 int skip = 0;
 struct dirent* wpis;
 struct stat statbufor;
 if ((dp = opendir(kat)) == NULL) {int nc = ncont(); if (nc)return;}
 chdir(kat);
 cout<<"bazy danych:\n\n";
 while ((wpis = readdir(dp)) != NULL)
        {
         stat(wpis->d_name, &statbufor);
         if (!S_ISDIR(statbufor.st_mode))
                {
                 i++;
                 AboutDisplay(wpis->d_name, pos);
                 clear(i, 3*CLEAR, skip);
                }
        }
 chdir("..");
 closedir(dp);
 lns(2);
}

void DisplayCharacters()
{
 fprintf(lg,"wyswietlam postacie\n");
 char kat[STRING];
 int skip = 0;
 strcpy(kat,"./Characters");
 DIR* dp;
 int pos = 0;
 int i = 0;
 struct dirent* wpis;
 struct stat statbufor;
 if ((dp = opendir(kat)) == NULL) {int nc = ncont(); if (nc)return;}
 chdir(kat);
 cout<<"postacie:\n\n";
 while ((wpis = readdir(dp)) != NULL)
        {
         stat(wpis->d_name, &statbufor);
         if (!S_ISDIR(statbufor.st_mode))
                {
                 i++;
                 AboutDisplayChar(wpis->d_name, pos);
                 clear(i, 3*CLEAR, skip);
                }
        }
 chdir("..");
 closedir(dp);
 lns(2);
}
void Protection(int check, DATA* db, FILE* plik)
{
 fprintf(lg,"sprawdzam poprawnosc\n");
 if (!check)
        {
        fprintf(lg,"-- error -- bledny odczyt bazy\n");
         lns(6);
         cout<<"baza zostala blednie wczytana.\n";
         cout<<"kontynuacja moze ja uszkosdzic\n";
         cout<<"administrator cos o tym wie...\n";
         lns(4);
         char zn;
         cout<<"kontynuowac? ";
         takechar(&zn, 1, 'n');
         if (zn != 't') {delete db;exit( -1 );fclose(plik);}
        }
}
void Load(DATA* db,FILE* plik)
{
 fprintf(lg,"odczyt bazy...\n");
 cout<<"wczytywanie.\n";
 int check;
 check = fscanf(plik,"%d\n",&db->ilosc[0]);
 Protection(check,db,plik);
 for (int i=0;i<db->ilosc[0];i++)
        {
         fscanf(plik,"%d %s\n",&db->value_att[i],db->name_att[i]);
         DecompressData(db->name_att[i]);
        }
 fscanf(plik,"%d\n",&db->ilosc[1]);
 check = fscanf(plik,"%d\n",&db->ilosc[2]);
 for (int i=0;i<db->ilosc[1];i++)
        {
         fscanf(plik,"%s ",db->name[0][i]);
         DecompressData(db->name[0][i]);
         for (int j=0;j<db->ilosc[0];j++)
                {
                 fscanf(plik,"%d ",&db->mod[0][i][j]);
                }
         fscanf(plik,"\n");
        }
 for (int i=0;i<db->ilosc[2];i++)
        {
         fscanf(plik,"%s ",db->name[1][i]);
         DecompressData(db->name[1][i]);
         for (int j=0;j<db->ilosc[0];j++)
                {
                 fscanf(plik,"%d ",&db->mod[1][i][j]);
                }
         fscanf(plik,"\n");
        }
 check = fscanf(plik,"%d\n",&db->ilosc[3]);
 Protection(check,db,plik);
 for (int i=0;i<db->ilosc[3];i++)
        {
         fscanf(plik,"%s ",db->s_name[i]);
         DecompressData(db->s_name[i]);
         fscanf(plik,"%d %d %d %d %d\n",&db->s_mod[i][0],&db->s_mod[i][1],&db->s_mod[i][2],&db->s_mod[i][3],&db->s_mod[i][4]);
        }
 fscanf(plik,"%d\n",&db->ilosc[4]);
 for (int i=0;i<db->ilosc[4];i++)
        {
         fscanf(plik,"%s\n",db->c_name[i]);
         DecompressData(db->c_name[i]);
        }
 fscanf(plik,"%d\n",&db->ilosc[5]);
 for (int i=0;i<db->ilosc[5];i++)
        {
         fscanf(plik,"%d\n",&db->cost[i]);
        }
 fscanf(plik,"%d\n",&db->sv);
 fscanf(plik,"%d\n",&db->st);
 fscanf(plik,"%d\n",&db->ilosc[6]);
 for (int i=0;i<db->ilosc[6];i++)
        {
         fscanf(plik,"%d\n",&db->pd_cost[i]);
        }
 for (int i=0;i<db->ilosc[0];i++)
        {
         fscanf(plik,"%d\n",&db->p_mod[i]);
        }
 fscanf(plik,"%d\n",&db->lr);
 for (int i=0;i<db->ilosc[3];i++)
        {
         fscanf(plik,"%d\n",&db->s_mod[i][MAGIC]);
        }
 fscanf(plik,"%d\n",&db->mm);
 fscanf(plik,"%s\n",db->dbname);
 DecompressData(db->dbname);
 fscanf(plik,"%d\n",&db->def_cash);
 fscanf(plik,"%d\n",&db->ilosc_ekw);
 for (int i=0;i<db->ilosc_ekw;i++)
 	{
    fscanf(plik,"%s\n",db->ekw[i]);
    DecompressData(db->ekw[i]);
    fscanf(plik,"%d\n",&db->c_ekw[i]);
   }
  check = fscanf(plik,"%d\n",&db->ilosc_poziomow);
  Protection(check,db,plik);
  for (int i=0;i<db->ilosc_poziomow;i++)
        {
         fscanf(plik,"%d\n",&db->ilosc_czarow[i]);
         for (int j=0;j<db->ilosc_czarow[i];j++)
                {
                 fscanf(plik,"%d\n",&db->spell_cost[i][j]);
                 fscanf(plik,"%s\n",db->req_magic[i][j]);
                 DecompressData(db->req_magic[i][j]);
                 fscanf(plik,"%s\n",db->zaklecie[i][j]);
                 DecompressData(db->zaklecie[i][j]);
                }
        }
}
void Uncompress(DATA* db)
{
 fprintf(lg,"dekompresja bazy\n");
 for (int i=0;i<db->ilosc[0];i++)
        {
         DecompressData(db->name_att[i]);
        }
 for (int i=0;i<db->ilosc[1];i++)
        {
         DecompressData(db->name[0][i]);
        }
 for (int i=0;i<db->ilosc[2];i++)
        {
         DecompressData(db->name[1][i]);
        }
 for (int i=0;i<db->ilosc[3];i++)
        {
         DecompressData(db->s_name[i]);
        }
 for (int i=0;i<db->ilosc[4];i++)
        {
         DecompressData(db->c_name[i]);
        }
 for (int i=0;i<db->ilosc_ekw;i++)
        {
         DecompressData(db->ekw[i]);
        }
 for (int i=0;i<db->ilosc_poziomow;i++)
        {
         for (int j=0;j<db->ilosc_czarow[i];j++)
                {
                 DecompressData(db->req_magic[i][j]);
                 DecompressData(db->zaklecie[i][j]);
                }
        }
 DecompressData(db->dbname);
}

void Save(DATA* db, FILE* plik)
{
 fprintf(lg,"zapisuje baze\n");
 cout<<"zapisywanie.\n";
 fprintf(plik,"%d\n",db->ilosc[0]);
 for (int i=0;i<db->ilosc[0];i++)
        {
         CompressData(db->name_att[i]);
         fprintf(plik,"%d %s\n",db->value_att[i],db->name_att[i]);
        }
 fprintf(plik,"%d\n",db->ilosc[1]);
 fprintf(plik,"%d\n",db->ilosc[2]);
 for (int i=0;i<db->ilosc[1];i++)
        {
         CompressData(db->name[0][i]);
         fprintf(plik,"%s ",db->name[0][i]);
         for (int j=0;j<db->ilosc[0];j++)
                {
                 fprintf(plik,"%d ",db->mod[0][i][j]);
                }
         fprintf(plik,"\n");
        }
 for (int i=0;i<db->ilosc[2];i++)
        {
         CompressData(db->name[1][i]);
         fprintf(plik,"%s ",db->name[1][i]);
         for (int j=0;j<db->ilosc[0];j++)
                {
                 fprintf(plik,"%d ",db->mod[1][i][j]);
                }
         fprintf(plik,"\n");
        }
 fprintf(plik,"%d\n",db->ilosc[3]);
 for (int i=0;i<db->ilosc[3];i++)
        {
         CompressData(db->s_name[i]);
         fprintf(plik,"%s ",db->s_name[i]);
         fprintf(plik,"%d %d %d %d %d\n",db->s_mod[i][0],db->s_mod[i][1],db->s_mod[i][2],db->s_mod[i][3],db->s_mod[i][4]);
        }
 fprintf(plik,"%d\n",db->ilosc[4]);
 for (int i=0;i<db->ilosc[4];i++)
        {
         CompressData(db->c_name[i]);
         fprintf(plik,"%s\n",db->c_name[i]);
        }
 fprintf(plik,"%d\n",db->ilosc[5]);
 for (int i=0;i<db->ilosc[5];i++)
        {
         fprintf(plik,"%d\n",db->cost[i]);
        }
 fprintf(plik,"%d\n",db->sv);
 fprintf(plik,"%d\n",db->st);
fprintf(plik,"%d\n",db->ilosc[6]);
 for (int i=0;i<db->ilosc[6];i++)
        {
         fprintf(plik,"%d\n",db->pd_cost[i]);
        }
 for (int i=0;i<db->ilosc[0];i++)
        {
         fprintf(plik,"%d\n",db->p_mod[i]);
        }
 fprintf(plik,"%d\n",db->lr);
  for (int i=0;i<db->ilosc[3];i++)
        {
         fprintf(plik,"%d\n",db->s_mod[i][MAGIC]);
        }
 fprintf(plik,"%d\n",db->mm);
 CompressData(db->dbname);
 fprintf(plik,"%s\n",db->dbname);
 fprintf(plik,"%d\n",db->def_cash);
 fprintf(plik,"%d\n",db->ilosc_ekw);
 for (int i=0;i<db->ilosc_ekw;i++)
 	{
    CompressData(db->ekw[i]);
    fprintf(plik,"%s\n",db->ekw[i]);
    fprintf(plik,"%d\n",db->c_ekw[i]);
   }
  fprintf(plik,"%d\n",db->ilosc_poziomow);
  for (int i=0;i<db->ilosc_poziomow;i++)
        {
         fprintf(plik,"%d\n",db->ilosc_czarow[i]);
         for (int j=0;j<db->ilosc_czarow[i];j++)
                {
                 fprintf(plik,"%d\n",db->spell_cost[i][j]);
                 CompressData(db->req_magic[i][j]);
                 fprintf(plik,"%s\n",db->req_magic[i][j]);
                 CompressData(db->zaklecie[i][j]);
                 fprintf(plik,"%s\n",db->zaklecie[i][j]);
                }
        }
 Uncompress(db);
}
void BaseToHTML(DATA* db, FILE* plik, char* name)
{
 fprintf(lg,"zapisuje baze w HTML\n");
 fprintf(plik,"<html>\n");
 fprintf(plik,"<head>\n");
 fprintf(plik,"<title>\n");
 fprintf(plik,"Data Base %s\n",name);
 fprintf(plik,"</title>\n");
 fprintf(plik,"</head>\n");
 fprintf(plik,"<body>\n");
 fprintf(plik,"<center><big>Data Base %s</center></big>\n",name);
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Number of attributes %d</center>\n",db->ilosc[0]);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Number\n");
 fprintf(plik,"<td>Attribute\n");
 fprintf(plik,"<td>Start Value\n");
 for (int i=0;i<db->ilosc[0];i++)
        {
         fprintf(plik,"<tr>\n");
         fprintf(plik,"<td>%d\n",i+1);
         fprintf(plik,"<td>%s\n",db->name_att[i]);
         fprintf(plik,"<td>%d\n",db->value_att[i]);
        }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Number of races %d</center>\n",db->ilosc[1]);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Number\n");
 fprintf(plik,"<td>Race\n");
 for (int i=0;i<db->ilosc[0];i++)
        {
         fprintf(plik,"<td>%c%c%c\n",db->name_att[i][0],db->name_att[i][1],db->name_att[i][2]);
        }
 for (int i=0;i<db->ilosc[1];i++)
        {
         fprintf(plik,"<tr>\n");
         fprintf(plik,"<td>%d\n",i+1);
         fprintf(plik,"<td>%s\n",db->name[0][i]);
         for (int j=0;j<db->ilosc[0];j++)
                {
                 fprintf(plik,"<td>%d\n",db->mod[0][i][j]);
                }
        }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Number of professies %d</center>\n",db->ilosc[2]);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Number\n");
 fprintf(plik,"<td>Profession\n");
 for (int i=0;i<db->ilosc[0];i++)
        {
         fprintf(plik,"<td>%c%c%c\n",db->name_att[i][0],db->name_att[i][1],db->name_att[i][2]);
        }
 for (int i=0;i<db->ilosc[2];i++)
        {
         fprintf(plik,"<tr>\n");
         fprintf(plik,"<td>%d\n",i+1);
         fprintf(plik,"<td>%s\n",db->name[1][i]);
         for (int j=0;j<db->ilosc[0];j++)
                {
                 fprintf(plik,"<td>%d\n",db->mod[1][i][j]);
                }
        }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Number of skills %d</center>\n",db->ilosc[3]);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Number\n");
 fprintf(plik,"<td>Skil\n");
 fprintf(plik,"<td>Multiple\n");
 fprintf(plik,"<td>Num.Attrib\n");
 fprintf(plik,"<td>Attrib\n");
 fprintf(plik,"<td>Mod\n");
 fprintf(plik,"<td>Diff\n");
 fprintf(plik,"<td>Num.Categ\n");
 fprintf(plik,"<td>Category\n");
 fprintf(plik,"<td>Magic\n");
 for (int i=0;i<db->ilosc[3];i++)
        {
         fprintf(plik,"<tr>\n");
         fprintf(plik,"<td>%d\n",i+1);
         fprintf(plik,"<td>%s\n",db->s_name[i]);
         fprintf(plik,"<td>%d\n",db->s_mod[i][MULTIPLE]);
         fprintf(plik,"<td>%d\n",db->s_mod[i][ATTRIBUTE]);
         fprintf(plik,"<td>%s\n",db->name_att[db->s_mod[i][ATTRIBUTE]]);
         fprintf(plik,"<td>%d\n",db->s_mod[i][MOD]);
         fprintf(plik,"<td>%d\n",db->s_mod[i][DIFF]);
         fprintf(plik,"<td>%d\n",db->s_mod[i][CATEG]);
         fprintf(plik,"<td>%s\n",db->c_name[db->s_mod[i][CATEG]]);
         fprintf(plik,"<td>%d\n",db->s_mod[i][MAGIC]);
        }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Number of categories %d</center>\n",db->ilosc[4]);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Number\n");
 fprintf(plik,"<td>category\n");
 for (int i=0;i<db->ilosc[4];i++)
        {
         fprintf(plik,"<tr>\n");
         fprintf(plik,"<td>%d\n",i+1);
         fprintf(plik,"<td>%s\n",db->c_name[i]);
        }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Number of skill levels %d</center>\n",db->ilosc[5]);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Level\n");
 fprintf(plik,"<td>Cost in SP\n");
 for (int i=0;i<db->ilosc[5];i++)
        {
         fprintf(plik,"<tr>\n");
         fprintf(plik,"<td>%d\n",i);
         fprintf(plik,"<td>%d\n",db->cost[i]);
        }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Number of Experience levels %d</center>\n",db->ilosc[6]);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Level\n");
 fprintf(plik,"<td>Cost in XP\n");
 for (int i=0;i<db->ilosc[6];i++)
        {
         fprintf(plik,"<tr>\n");
         fprintf(plik,"<td>%d\n",i);
         fprintf(plik,"<td>%d\n",db->pd_cost[i]);
        }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Gender modifier(woman)</center>\n");
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Attrib\n");
 fprintf(plik,"<td>Modifier\n");
 for (int i=0;i<db->ilosc[0];i++)
        {
         fprintf(plik,"<tr>\n");
         fprintf(plik,"<td>%s\n",db->name_att[i]);
         fprintf(plik,"<td>%d\n",db->p_mod[i]);
        }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Items in DataBase</center>\n");
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Number of equipment %d. Default cash for character: %d</center>\n",db->ilosc_ekw,db->def_cash);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Cost\n");
 fprintf(plik,"<td>Item\n");
 for (int i=0;i<db->ilosc_ekw;i++)
 	{
    fprintf(plik,"<tr><td>%d Cu\n<td>%s",db->c_ekw[i],db->ekw[i]);
   }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 fprintf(plik,"<center>Spells in DataBase</center>\n");
 fprintf(plik,"<center>Maximum levels is %d\n",db->ilosc_poziomow);
 for (int i=0;i<db->ilosc_poziomow;i++)
 {
 fprintf(plik,"<center> Spells Level %d\n",i);
 fprintf(plik,"<center><table border =\"3\">\n");
 fprintf(plik,"<tr>\n");
 fprintf(plik,"<td>Lp.\n");
 fprintf(plik,"<td>Spell name\n");
 fprintf(plik,"<td>Required magic\n");
 fprintf(plik,"<td>Spell Points Cost\n");
 for (int j=0;j<db->ilosc_czarow[i];j++)
 	{
               fprintf(plik,"<tr>\n");
               fprintf(plik,"<td>%d\n",j);
               fprintf(plik,"<td>%s\n",db->zaklecie[i][j]);
               fprintf(plik,"<td>%s\n",db->req_magic[i][j]);
               fprintf(plik,"<td>%d SP\n",db->spell_cost[i][j]);
              }
 fprintf(plik,"</table></center>\n");
 fprintf(plik,"<br>\n");
 }
 fprintf(plik,"<center>Data Base generated by rpg by morgothdbma copyleft@</center>\n",name);
 fprintf(plik,"</body>\n");
 fprintf(plik,"</html>\n");
}

void BaseHTML(DATA* db, char* file_name)
{
 fprintf(lg,"zapis w HTML:\n");
 char name[100];
 strcpy(name,file_name);
 strcat(name,".html");
 cout<<"stworzyc "<<name<<"? ";
 char zn;
 takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 FILE* html;
 if ((html = fopen(name,"r+")) != NULL)
        {
         cout<<"nadpisac "<<name<<"? ";
         takechar(&zn, 2, 't', 'n');
         if (zn != 't') {fclose(html);return;}
         fprintf(lg,"nadpisuje istniejaca\n");
        }
 if ((html = fopen(name,"w+")) == NULL)
        {
         cout<<"nie moge zapisac "<<name<<endl;
         fprintf(lg,"-- error -- nie da sie zapisac w pliku %s\n", name);
         int nc = ncont(); if (nc) return;
        }
 cout<<"zapisuje w formacie html...\n";
 BaseToHTML(db,html,name);
 fclose(html);
}
void CheckPassword(DATA* db)
{
 int a;
 fprintf(lg,"sprawdzam kod\n");
 if (db->sv == -1) {cout<<"odmowa dostepu\n";getch(1,'k');exit( -1 );}
 if (db->sv != 0)
        {
         cout<<"podaj kod: ";
         secretake(a);
         if (a != db->sv)
                {
                 cout<<"odmowa dostepu!\n";
                 getch(1,'k');
                 exit( -1 );
                }
        }
 cout<<"zmienic kod? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn == 't') {cout<<"nowy kod lub -1 znaczy odmowa dostepu: ";secretake(db->sv);}
}
void clear(int iter, int val, int& skip)
{
 fprintf(lg,".");
 int zn;
 if ((!((iter+1)%val)) && (!skip)) zn = getch(1,'s');
 if (zn == 's') skip = 1;
}
void ShowAttrib(DATA* db)
{
 fprintf(lg,"cechy\n");
 lns(2);
 int skip = 0;
 cout<<"wartosc plusa: "<<db->st<<endl;
 cout<<"za moc odpowiada "<<db->name_att[db->mm]<<".\n";
 cout<<"\t\tilosc cech: "<<db->ilosc[0]<<endl;
 for (int i=0;i<db->ilosc[0];i++)
        {
         clear(i, CLEAR, skip);
         printf("cecha: (%-2d)%-25s wartosc startowa: %-3d\n",i,db->name_att[i],db->value_att[i]);
        }
 lns(2);
}
void SetAttrib(DATA* db)
{
 fprintf(lg,"ustalam cechy\n");
 cout<<"maksymalna ilosc cech: "<<MAX_ATT<<endl;
 cout<<"zdefiniowane "<<db->ilosc[0]<<" cech.\n";
 ShowAttrib(db);
 cout<<"zmienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 ShowAttrib(db);
 cout<<"numer cechy ktora jest moca magiczna: ";
 int mmm;
 takeint(&mmm, 16);
 if (mmm<0) mmm = 0;
 if (mmm>db->ilosc[0]-1) mmm = db->ilosc[0] -1;
 db->mm = mmm;
 cout<<"wartosc plusa: ";
 takeint(&db->st, 10);
 cout<<"zmieniac dalej? ";
 takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 cout<<"ilosc cech: ";
 int temp;takeint(&temp, 16);
 if (temp>MAX_ATT) temp = MAX_ATT;
 if (temp<1)       temp = 1;
 db->ilosc[0] = temp;
 for (int i=0;i<db->ilosc[0];i++)
        {
         cout<<"nazwa cechy nr. "<<i<<": ";
         takestring(db->name_att[i]);
         cout<<"wartosc startowa "<<db->name_att[i]<<": ";
         takeint(&temp, 20);
         if (temp < 0) temp *= -1;
         db->value_att[i] = temp;
        }
}
bool Check(DATA* db)
{
 fprintf(lg,"sprawdzam kod\n");
 int a;
 if (db->sv == -1) {cout<<"odmowa dostepu!\n"; return false;}
 if (db->sv != 0)
        {
         cout<<"podaj kod: ";
         secretake(a);
         if (a != db->sv)
                {
                 cout<<"odmowa dostepu!\n";
                 return false;
                }
        }
 return true;
}
bool StrMore(char* str1, char* str2)
{
 int len = (strlen(str1) < strlen(str2)) ? strlen(str1) : strlen(str2);
 int i = 0;
 while ((str1[i] == str2[i]) && (i <= len)) i++;
 if (str1[i] > str2[i]) return true;
 else return false;
}
void SwapTrial(DATA* db, int trial, int i, int j)
{
 char pom[STRING];
 int mod;

 strcpy(pom, db->name[trial][j]);
 strcpy(db->name[trial][j], db->name[trial][i]);
 strcpy(db->name[trial][i], pom);

 for (int x=0;x<db->ilosc[0];x++)
        {
         mod = db->mod[trial][j][x];
         db->mod[trial][j][x] = db->mod[trial][i][x];
         db->mod[trial][i][x] = mod;
        }
}

void ASortTrial(DATA* db, int trial)
{
 fprintf(lg,"sortuje\n");
 for (int i=0;i<db->ilosc[trial+1];i++)
    {
     for (int j=0;j<db->ilosc[trial+1]-(i+1);j++)
        {
	 if (StrMore(db->name[trial][j], db->name[trial][j+1]))
	  {
	   SwapTrial(db, trial, j+1, j);
	  }
	}
     }
}

void QuickSave(DATA* db)
{
 fprintf(lg,"szybki zapis.\n");
 FILE* plik;
 char fn[STRING+20];
 strcpy(fn,"DataBase/");
 char temp[STRING];
 cout<<"nazwa bazy: ";
 takestring(temp, SECRET);
 strcat(fn,temp);
 if ((plik = fopen(fn,"w+")) == NULL)
        {
         cout<<"nie moge zapisac w "<<fn<<endl;
         fprintf(lg,"-- error -- nie mozna zapisac w %s\n", fn);
         int nc = ncont(); if (nc)
         return;
        }
 Save(db,plik);
 fclose(plik);
}
void ShowTrial(DATA* db,short trial)
{
 fprintf(lg,"pokazuje rasy/profesje\n");
 lns(2);
 int skip = 0;
 cout<<"\t\tilosc: "<<db->ilosc[trial+1]<<endl;
 for (int i=0;i<db->ilosc[trial+1];i++)
        {
         printf("(%-2d)%-20s ",i,db->name[trial][i]);
         for (int j=0;j<db->ilosc[0];j++)
                {
                 printf("%-2d ",db->mod[trial][i][j]);
                }
         ln();
         clear(i,CLEAR,skip);
        }
 lns(2);
}
void AddTrial(DATA* db, short trial,short add = TRUE)
{
 fprintf(lg,"dodaje rase/profesje\n");
 if ((db->ilosc[trial+1] == MAX_TYPE) && (add != TRUE)) {cout<<"rekord pelny\n";return;}
 int temp = 0;
 if (add == TRUE) {temp = db->ilosc[trial+1];}
 else
        {
         ShowTrial(db,trial);
         printf("numer (0 - %d): ",db->ilosc[trial+1]-1);
         takeint(&temp, 30);
         if (temp<0) return;
         if (temp>db->ilosc[trial+1]-1) return;
        }
 cout<<"nazwa: ";
 takestring(db->name[trial][temp]);
 for (int i=0;i<db->ilosc[0];i++)
        {
         cout<<"jak "<<db->name[trial][temp]<<" modyfikuje "<<db->name_att[i]<<": ";
         takeint(&db->mod[trial][temp][i], 5, 1);
        }
 if (add == TRUE) db->ilosc[trial+1]++;
}
void DelTrial(DATA* db, short trial,short last = TRUE)
{
 fprintf(lg,"usuwa rase/profesje\n");
 if (db->ilosc[trial+1] == 0) {cout<<"nic do usuniecia\n";return;}
 if (last == TRUE) db->ilosc[trial+1]--;
 else
        {
         int temp;
         ln();
         ShowTrial(db,trial);
         printf("numer (0 - %d): ",db->ilosc[trial+1]-1);
         takeint(&temp, 30);
         if (temp<0) return;
         if (temp>db->ilosc[trial+1]-1) return;
         db->ilosc[trial+1]--;
         for (int i=temp;i<db->ilosc[trial+1];i++)
                {
                 strcpy(db->name[trial][i],db->name[trial][i+1]);
                 for (int j=0;j<db->ilosc[0];j++)
                        {
                         db->mod[trial][i][j] = db->mod[trial][i+1][j];
                        }
                }
         ln();
        }
}

void CopyTrial(DATA* to, DATA* from, short trial)
{
 fprintf(lg,"kopiuje rasy/profesje z innej bazy\n");
 int max = to->ilosc[trial+1] + from->ilosc[trial+1];
 if (max > MAX_TYPE)
        {
         cout<<"powyzej "<<MAX_TYPE<<" obetne.\n";
         max = MAX_TYPE;
        }
  int old = to->ilosc[trial+1];
  to->ilosc[trial+1] += from->ilosc[trial+1];
  if (to->ilosc[trial+1] > MAX_TYPE) to->ilosc[trial+1] = MAX_TYPE;
 for (int i = old;i<max;i++)
        {
         strcpy(to->name[trial][i],from->name[trial][i-old]);
         for (int j=0;j<to->ilosc[0];j++)
                {
                 to->mod[trial][i][j] = from->mod[trial][i-old][j];
                }
         cout<<"#";
        }

}

void Editor(DATA* db, short trial)
{
 int il = db->ilosc[trial+1];
 ShowTrial(db,trial);
 cout<<"od: ";
 int st; takeint(&st,5);
 cout<<"do: ";
 int en; takeint(&en, 50);
 if ((st < 0) || (en >il-1)) return;
 for (int i=st;i<en;i++)
        {
         cout<<"aktualnie:\t"<<db->name[trial][i]<<endl;
         cout<<"y-edytuj\nn-nastepna\nc-anuluj\nd-usun\nwybor? ";
         char zn;
         takechar(&zn, 4, 'y', 'n', 'c', 'd');
         if (zn == 'c') return;
         else if (zn == 'y')
                {
                 for (int j=0;j<db->ilosc[0];j++)
                        {
                         cout<<"jak "<<db->name[trial][i]<<" modyfikuje "<<db->name_att[j]<<" lub 999 nastepna cecha: ";
                         int lc;
                         takeint(&lc, 5, 1);
                         if (lc != 999) db->mod[trial][i][j] = lc;
                         else goto cont;
                        }
                  cont:;
                }
          else if (zn == 'd')
                {
                 en--;
                 db->ilosc[trial+1] --;
                 for (int x=i;x<db->ilosc[trial+1];x++)
                        {
                         strcpy(db->name[trial][x],db->name[trial][x+1]);
                         for (int j=0;j<db->ilosc[0];j++) db->mod[trial][x][j] = db->mod[trial][x+1][j];
                         cout<<"*";
                        }
                 i--;
                 ln();
                }
        }
}

void ImportTrial(DATA* db, short trial)
{
 fprintf(lg,"import\n");
 cout<<"zapisz aktualna baze baze w: ";
 char bname[STRING];
 char folder[STRING];
 strcpy(folder,"DataBase/");
 takestring(bname);
 strcat(folder,bname);
 FILE* plik;
 if ((plik = fopen(folder,"w+")) == NULL)
        {
         cout<<"nie moge zapisac w: "<<folder<<endl;
         int nc = ncont(); if (nc)
         return;
        }
 Save(db,plik);
 fclose(plik);
 cout<<"importowac z: ";
 takestring(bname, 3, "morgoth", "galthar", "saradok");
 strcpy(folder,"DataBase/");
 strcat(folder,bname);
 if ((plik = fopen(folder,"r+")) == NULL)
        {
         cout<<"brak pliku: "<<folder<<endl;
         int nc = ncont(); if (nc)
         return;
        }
  DATA* db2 = new DATA;
  Load(db2,plik);
  int access = Check(db2);
  if (!access){cout<<"odmowa dostepu!\n";return;}
  CopyTrial(db,db2,trial);
  fclose(plik);
  delete db2;
}
void OptTrial(DATA* db, int trial)
{
 fprintf(lg,"opcje\n");
 cout<<"\t\tilosc: "<<db->ilosc[trial+1]<<endl;
 cout<<"a\t-dodaj\n";
 cout<<"b\t-usun\n";
 cout<<"c\t-zobacz\n";
 cout<<"d\t-zmien\n";
 cout<<"e\t-usun ze srodka\n";
 cout<<"f\t-wyczysc rekord\n";
 cout<<"g\t-zapisz\n";
 cout<<"i\t-importuj\n";
 cout<<"u\t-edytuj\n";
 cout<<"s\t-sortuj\n";
 cout<<"k\t-zakoncz\n";
 cout<<"z\t-zakoncz program\n";
 ln();
}
void SetTrial(DATA* db, short trial)
{
 fprintf(lg,"ustala rasy/profesje\n");
 if (trial) {cout<<"max profesji: "<<MAX_TYPE<<endl;}
 else {cout<<"max ras "<<MAX_TYPE<<endl;}
 cout<<"znalazlem ";
 if (trial == 0) {cout<<db->ilosc[1]<<" ras\n";}
 if (trial == 1) {cout<<db->ilosc[2]<<" profesji\n";}
 ShowTrial(db,trial);
 cout<<"zmienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 char z = '%';
 while (z != 'k')
   {
 cout<<"Opcja (0-lista): ";
 takechar(&z);
 switch(z)
        {
         case '0': OptTrial(db, trial);break;
         case 'a': AddTrial(db,trial);break;
         case 'b': DelTrial(db,trial);break;
         case 'c': ShowTrial(db,trial);break;
         case 'd': AddTrial(db,trial,FALSE);break;
         case 'e': DelTrial(db,trial,FALSE);break;
         case 'f': db->ilosc[trial+1] = 0;break;
         case 'g': QuickSave(db);break;
         case 'i': ImportTrial(db,trial);break;
         case 'u': Editor(db,trial);break;
         case 's': ASortTrial(db, trial);break;
         case 'z': exit( -1 );break;
        }
   }
}
void ShowCateg(DATA* db)
{
 int skip = 0;
 lns(2);
 cout<<"\t\tilosc kategorii: "<<db->ilosc[4]<<endl;
 for (int i=0;i<db->ilosc[4];i++)
        {
         printf("kateg: (%-3d)%-30s\n",i,db->c_name[i]);
         clear(i, CLEAR, skip);
        }
 lns(2);
}
void SetCateg(DATA* db)
{
 fprintf(lg,"ustala kategorie\n");
 cout<<"max kateg "<<MAX_CATEG<<endl;
 cout<<"znalazlem "<<db->ilosc[4]<<" zdefiniowanych.\n";
 ShowCateg(db);
 cout<<"zmienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 cout<<"ilosc kategorii: ";
 int temp;takeint(&temp, 10);
 if (temp>MAX_CATEG) temp = MAX_CATEG;
 if (temp<1)       temp = 1;
 db->ilosc[4] = temp;
 for (int i=0;i<db->ilosc[4];i++)
        {
         cout<<"nazwa "<<i<<" kategorii: ";
         takestring(db->c_name[i]);
        }
}
void OptSkill(DATA* db)
{
 fprintf(lg,"opcje\n");
 ln();
 cout<<"\t\tilosc: "<<db->ilosc[3]<<endl;
 cout<<"a\t-dodaj\n";
 cout<<"b\t-usun\n";
 cout<<"c\t-zobacz dokladnie\n";
 cout<<"d\t-zobacz nazwy\n";
 cout<<"e\t-zobacz dokladnie kategorie\n";
 cout<<"f\t-zobacz nazwy w danej kategorii\n";
 cout<<"g\t-Edytuj\n";
 cout<<"h\t-usun ze srodka\n";
 cout<<"i\t-posortuj w/g kategorii\n";
 cout<<"j\t-oczysc rekord\n";
 cout<<"s\t-zapisz\n";
 cout<<"k\t-zakoncz\n";
 cout<<"l\t-edytuj magikowosc\n";
 cout<<"m\t-edytuj trudnosci\n";
 cout<<"v\t-znajdz i zmien\n";
 cout<<"t\t-posortuj\n";
 cout<<"p\t-importuj\n";
 cout<<"z\t-zakoncz program\n";
 cout<<"u\t-edytuj skopiowane\n";
}
void ShowSkillMagic(DATA* db, int  var = USER)
{
 fprintf(lg,"pokazuje magiczne umiejetnosci\n");
 int skip = 0;
 int core = 0;
 if (var == USER)
        {
         cout<<"od jakiego poziou magii wyswietlac: ";
         takeint(&var, 2);
        }
 for (int i=0;i<db->ilosc[3];i++)
        {
         if (db->s_mod[i][MAGIC] >= var) {printf("(%-3d)%-25s\n",i,db->s_name[i]);core++;
         clear(core, CLEAR, skip);}
        }
}
void ShowSkill(DATA* db,short details = 1)
{
 fprintf(lg,"pokazuje umiejetnosci\n");
 cout<<"\t\tilosc: "<<db->ilosc[3]<<endl;
 ln();
 int skip = 0;
 if (details == 1)
 {
 for (int i=0;i<db->ilosc[3];i++)
        {
         printf("%-3d.%-18s %-15s Md %-2d Ml %-2d D %-2d %-14s M %d\n",i,db->s_name[i],db->name_att[db->s_mod[i][ATTRIBUTE]],db->s_mod[i][MOD],db->s_mod[i][MULTIPLE],db->s_mod[i][DIFF],db->c_name[db->s_mod[i][CATEG]],db->s_mod[i][MAGIC]);
         clear(i, CLEAR, skip);
        }
 }
 if (details == 0)
        {
         for (int i=0;i<db->ilosc[3];i++)
                {
                 printf("(%-3d)%-18s ",i,db->s_name[i]);
                 if (!(i % 3)) ln();
                 clear(i, 3*CLEAR, skip);
                }
        }
 if (details == 2)
        {
         ShowCateg(db);
         ln();
         int temp;
         int cat = 0;
         cout<<"numer kateg: ";
         takeint(&temp, 10);
         if ((temp>db->ilosc[4]-1) || (temp<0)) return;
         cout<<"kateg:\t"<< db->c_name[temp]<<endl;
         for (int i=0;i<db->ilosc[3];i++)
             {
              if (db->s_mod[i][CATEG] == temp)
                 {
                  printf("%-3d.%-18s %-15s Md %-2d Ml %-2d D %-2d %-14s M %d\n",i,db->s_name[i],db->name_att[db->s_mod[i][ATTRIBUTE]],db->s_mod[i][MOD],db->s_mod[i][MULTIPLE],db->s_mod[i][DIFF],db->c_name[db->s_mod[i][CATEG]],db->s_mod[i][MAGIC]);
                  cat++;
                  clear(cat, CLEAR, skip);
                 }

             }
        }
 if (details == 3)
        {
         ShowCateg(db);
         ln();
         int temp;
         cout<<"numer kateg: ";
         takeint(&temp, 10);
         if ((temp>db->ilosc[4]-1) || (temp<0)) return;
         cout<<"kateg:\t"<< db->c_name[temp]<<endl;
         int j = 0;
         for (int i=0;i<db->ilosc[3];i++)
             {
              if (db->s_mod[i][CATEG] == temp)
                {
                 j++;
                 printf("(%-3d)%-18s ",i,db->s_name[i]);
                 if (!(j % 3)) ln();
                 clear(j, 3*CLEAR, skip);
                }
             }
        }
 lns(2);
}

void DelSkill(DATA* db, short middle = 0)
{
fprintf(lg,"usuwa umiejetnosci\n");
 if ((db->ilosc[3] == 0) && (middle != 0)){cout<<"pusty rekord!\n";return;}
 if ((middle == 0) && (db->ilosc[3] > 0)) {db->ilosc[3]--;return;}
 ShowSkill(db,0);
 lns(2);int temp;
 printf("podaj numer (0 - %d): ",db->ilosc[3]-1);
 takeint(&temp, 10);
 if ((temp<0) || (temp>db->ilosc[3]-1)) return;
 db->ilosc[3]--;
 for (int i=temp;i<db->ilosc[3];i++)
        {
         strcpy(db->s_name[i],db->s_name[i+1]);
         for (int j=0;j<5;j++) db->s_mod[i][j] = db->s_mod[i+1][j];
        }
}
void FindSkill(DATA* db, int diff = 0)
{
 fprintf(lg,"szuka umiejetnosci\n");
 cout<<"kilka pierwszych liter: ";
 char part[STRING];
 int correct;
 int find = 0;
 int skip = 0;
 takestring(part);
 DecompressData(part);
 downcase(part);
 cout<<"znalazlem:\n\n";
 for (int i=0;i<db->ilosc[3];i++)
        {
         correct = 0;
         for (int j=0;j<(int)strlen(part);j++)
                {
                 if ((part[j] == db->s_name[i][j]) || (part[j] == db->s_name[i][j] + 0x20)) correct++;
                }
         if (correct == (int)strlen(part))
         	{
             if (!diff) printf("(%-3d) %s\n",i,db->s_name[i]);
             else printf("(%-3d) %-30s\t trudnosc %d\n",i,db->s_name[i],db->s_mod[i][DIFF]);
             find++;
             clear(find, CLEAR, skip);
            }
        }
 if (find == 0) cout<<"nie moge skojarzyc umejetnosci z "<<part<<endl;
}
void SwapSkill(DATA* db, int i, int j)
{
 char pom[STRING];
 int mod[6];

 strcpy(pom,db->s_name[j]);
 mod[0] =   db->s_mod[j][MULTIPLE];
 mod[1] =   db->s_mod[j][ATTRIBUTE];
 mod[2] =   db->s_mod[j][MOD];
 mod[3] =   db->s_mod[j][DIFF];
 mod[4] =   db->s_mod[j][CATEG];
 mod[5] =   db->s_mod[j][MAGIC];

 strcpy(db->s_name[j],db->s_name[i]);
 db->s_mod[j][MULTIPLE]   =   db->s_mod[i][MULTIPLE];
 db->s_mod[j][ATTRIBUTE]  =   db->s_mod[i][ATTRIBUTE];
 db->s_mod[j][MOD]        =   db->s_mod[i][MOD];
 db->s_mod[j][DIFF]       =   db->s_mod[i][DIFF];
 db->s_mod[j][CATEG]      =   db->s_mod[i][CATEG];
 db->s_mod[j][MAGIC]      =   db->s_mod[i][MAGIC];

 strcpy(db->s_name[i], pom);
 db->s_mod[i][MULTIPLE]   =  mod[0];
 db->s_mod[i][ATTRIBUTE]  =  mod[1];
 db->s_mod[i][MOD]        =  mod[2];
 db->s_mod[i][DIFF]       =  mod[3];
 db->s_mod[i][CATEG]      =  mod[4];
 db->s_mod[i][MAGIC]      =  mod[5];
}

void ASortSkill(DATA* db)
{
 fprintf(lg,"sortuje\n");
 for (int i=0;i<db->ilosc[3];i++)
    {
     for (int j=0;j<db->ilosc[3]-(i+1);j++)
        {
	 if (StrMore(db->s_name[j], db->s_name[j+1]))
	  {
	   SwapSkill(db, j+1, j);
	  }
	}
     }
}
void FindSkillWord(DATA* db, char* str)
{
 char part[STRING];
 int correct;
 int find = 0;
 int skip = 0;
 strcpy(part,str);
 DecompressData(part);
 downcase(part);
 cout<<"znalazlem:\n\n";
 for (int i=0;i<db->ilosc[3];i++)
        {
         correct = 0;
         for (int j=0;j<(int)strlen(part);j++)
                {
                 if ((part[j] == db->s_name[i][j]) || (part[j] == db->s_name[i][j] + 0x20)) correct++;
                }
         if (correct == (int)strlen(part))
         	{
             printf("(%-3d) %s\n",i,db->s_name[i]);
             find++;
             clear(find, CLEAR, skip);
            }
        }
 if (find == 0) cout<<"nie ma podobnych do "<<part<<endl;
}

void AddSkill(DATA* db, short middle = 0,short search = 0)
{
 fprintf(lg,"dodaje umiejetnosc\n");
 if ((db->ilosc[3] == MAX_SKI) && (middle != 0)) {cout<<"rekord pelny!\n";return;}
 int temp;
 if (middle == 0)
        {
         temp = db->ilosc[3];
        }
 else
        {
         if (!search) ShowSkill(db,0);
         else FindSkill(db);
         printf("numer (0 - %d): ",db->ilosc[3]-1);
         takeint(&temp, 10);
         if ((temp<0) || (temp>db->ilosc[3]-1)) return;
        }
 cout<<"nazwa "<<temp<<" umiejetnosci: ";
 takestring(db->s_name[temp]);
 ShowAttrib(db);lns(2);
 cout<<"numer cechy testujacej: ";
 int data;takeint(&data, 16);
 if (data<0) data = 0;
 if (data>db->ilosc[0]-1) data = db->ilosc[0]-1;
 db->s_mod[temp][ATTRIBUTE] = data;
 cout<<"modyfikator testu: ";
 takeint(&db->s_mod[temp][MOD], 2 ,1);
 cout<<"mnoznik: ";
 takeint(&db->s_mod[temp][MULTIPLE], 6);
 cout<<"poziom magii: ";
 takeint(&db->s_mod[temp][MAGIC], 3);
 cout<<"trudnosc: ";
 takeint(&data, 3);
 if (data<1) data = 1;
 db->s_mod[temp][DIFF] = data;
 lns(2);
 ShowCateg(db);
 lns(2);
 cout<<"numer kategorii: ";
 takeint(&data, 6);
 if (data<0) data = 0;
 if (data>db->ilosc[4]-1) data = db->ilosc[4]-1;
 db->s_mod[temp][CATEG] = data;
 if (middle == 0) db->ilosc[3]++;
}
void SortSkill(DATA* db)
{
 ShowSkill(db,FALSE);
 cout<<"\n\nod: ";
 int z1,z2;takeint(&z1, 1);
 cout<<"do: ";
 takeint(&z2, 50);
 if ((z1<0) || (z2<0)) return;
 if ((z1>db->ilosc[3]-1) || (z2>db->ilosc[3]-1)) return;
 if (z2<z1) z2 = z1;
 cout<<"modyfikuje <"<<z1<<" - "<<z2<<">\n";
 if ((z2-z1)>30)
        {
         cout<<"wiecej niz 30 to duzo, modyfikowac? ";
         char zn;takechar(&zn, 2, 't', 'n');
         if (zn != 't') return;
        }
 cout<<"OK\n";
 for (int i=z1;i<=z2;i++)
        {
         lns(3);
         printf("%d z <%d - %d>\n",i,z1,z2);
         cout<<"umiejetnosc "<<db->s_name[i]<<endl;
         ShowCateg(db);
         cout<<"numer kategorii lub poza zakresem aby anulowac: ";
         int num;takeint(&num, 10);
         if ((num<0) || (num>db->ilosc[4]-1)) return;
         db->s_mod[i][CATEG] = num;
        }
}
void SortConv(DATA* db, int level)
{
 ShowSkill(db,FALSE);
 cout<<"\n\nod: ";
 int z1,z2;takeint(&z1, 5);
 cout<<"do: ";
 takeint(&z2, 50);
 if ((z1<0) || (z2<0)) return;
 if ((z1>db->ilosc[3]-1) || (z2>db->ilosc[3]-1)) return;
 if (z2<z1) z2 = z1;
 cout<<"Modyfikuje <"<<z1<<" - "<<z2<<">\n";
 if ((z2-z1)>30)
        {
         cout<<"wiecej niz 30, kontynuowac ";
         char zn;takechar(&zn, 2, 't', 'n');
         if (zn != 't') return;
        }
 if (level == 1)
        {
         for (int i=z1;i<=z2;i++)
                {
                 lns(3);
                 printf("%d z <%d - %d>\n",i,z1,z2);
                 cout<<"umiejetnosc "<<db->s_name[i]<<endl;
                 cout<<"poziom magii lub -1 anuluj: ";
                 int num;takeint(&num, 5);
                 if (num == -1) return;
                 db->s_mod[i][MAGIC] = num;
                }
        }
 if (level == 2)
        {
         for (int i=z1;i<=z2;i++)
                {
                 lns(3);
                 printf("%d z <%d - %d>\n",i,z1,z2);
                 cout<<"umiejetnosc "<<db->s_name[i]<<endl;
                 cout<<"trudnosc lub -1 anuluj: ";
                 int num;takeint(&num, 4);
                 if (num == -1) return;
                 db->s_mod[i][DIFF] = num;
                }
        }
 if (level == 3)
        {
         for (int i=z1;i<=z2;i++)
                {
                 lns(3);
                 printf("%d z <%d - %d>\n",i,z1,z2);
                 cout<<"umiejetnosc "<<db->s_name[i]<<endl;
                 cout<<"mnoznik lub -1 anuluj: ";
                 int num;takeint(&num, 6);
                 if (num == -1) return;
                 db->s_mod[i][MULTIPLE] = num;
                }
        }
 if (level == 4)
        {
         for (int i=z1;i<=z2;i++)
                {
                 lns(3);
                 printf("%d z <%d - %d>\n",i,z1,z2);
                 cout<<"umiejetnosc "<<db->s_name[i]<<endl;
                 ShowAttrib(db);
                 cout<<"numer cechy tetujacej lub -1 anuluj: ";
                 int num;takeint(&num, 16);
                 if (num == -1) return;
                 db->s_mod[i][ATTRIBUTE] = num;
                }
        }
 if (level == 5)
        {
         for (int i=z1;i<=z2;i++)
                {
                 lns(3);
                 printf("%d z <%d - %d>\n",i,z1,z2);
                 cout<<"umiejetnosc "<<db->s_name[i]<<endl;
                 cout<<"modyfikator lub -1 anuluj: ";
                 int num;takeint(&num, 2, 1);
                 if (num == -1) return;
                 db->s_mod[i][MOD] = num;
                }
        }
}

void CopySkills(DATA* to, DATA* from)
{
  fprintf(lg,"kopiuje umiejetnosci z innej bazy\n");
 int max = to->ilosc[3] + from->ilosc[3];
 if (max > MAX_SKI)
        {
         cout<<"powyzej "<<MAX_SKI<<" obetne.\n";
         max = MAX_SKI;
        }
  int old = to->ilosc[3];
  to->ilosc[3] += from->ilosc[3];
  if (to->ilosc[3] > MAX_SKI) to->ilosc[3] = MAX_SKI;
 for (int i = old;i<max;i++)
        {
         strcpy(to->s_name[i],from->s_name[i-old]);
         to->s_mod[i][MULTIPLE] =   from->s_mod[i-old][MULTIPLE];
         to->s_mod[i][ATTRIBUTE] = from->s_mod[i-old][ATTRIBUTE];
         to->s_mod[i][MOD] =            from->s_mod[i-old][MOD];
         to->s_mod[i][DIFF] =            from->s_mod[i-old][DIFF];
         to->s_mod[i][CATEG] =       from->s_mod[i-old][CATEG];
         to->s_mod[i][MAGIC] =       from->s_mod[i-old][MAGIC];
         cout<<"#";
        }

}

void Import(DATA* db)
{
  fprintf(lg,"importuje umiejetnosci\n");
 cout<<"zapisz aktualna baze w: ";
 char bname[STRING];
 char folder[STRING];
 strcpy(folder,"DataBase/");
 takestring(bname);
 strcat(folder,bname);
 FILE* plik;
 if ((plik = fopen(folder,"w+")) == NULL)
        {
         cout<<"nie moge zapisac w: "<<folder<<endl;
         int nc = ncont(); if (nc)
         return;
        }
 Save(db,plik);
 fclose(plik);
 cout<<"importuj z: ";
 takestring(bname,3,"galthar", "morgoth", "saradok");
 strcpy(folder,"DataBase/");
 strcat(folder,bname);
 if ((plik = fopen(folder,"r+")) == NULL)
        {
         cout<<"brak pliku: "<<folder<<endl;
         int nc = ncont(); if (nc)
         return;
        }
  DATA* db2 = new DATA;
  Load(db2,plik);
  int access = Check(db2);
  if (!access){cout<<"odmowa dostepu!\n";return;}
  CopySkills(db,db2);
  fclose(plik);
  delete db2;
}

void Propeties(DATA* db)
{
 cout<<"1\t-edytuj mnozniki\n";
 cout<<"2\t-edytuj cechy testujace\n";
 cout<<"3\t-edytuj modyfikatory testow\n";
 cout<<"wybor? ";
 int lb;
 takeint(&lb, 4);
 if ((lb <1) || (lb >3)) {cout<<"blad!\n";return;}
 lb += 2;
 SortConv(db,lb);
}

void SetSkill(DATA* db)
{
  fprintf(lg,"ustala umiejetnosci\n");
 ln();
 ShowSkill(db);
 cout<<"\nmax umiejetnosci to "<<MAX_SKI<<endl;
 ln();
 cout<<"zmienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 char z = '%';
 while (z != 'k')
   {
 cout<<"Opcja (0-lista): ";
 takechar(&z);
 switch(z)
        {
         case '0': OptSkill(db); break;
         case 'a': AddSkill(db);break;
         case 'b': DelSkill(db);break;
         case 'c': ShowSkill(db);break;
         case 'd': ShowSkill(db,0);break;
         case 'e': ShowSkill(db,2);break;
         case 'f': ShowSkill(db,3);break;
         case 'g': AddSkill(db,TRUE);break;
         case 'h': DelSkill(db,TRUE);break;
         case 'i': SortSkill(db);break;
         case 'j': db->ilosc[3] = 0;break;
         case 'l': SortConv(db,1);break;
         case 'm': SortConv(db,2);break;
         case 's': QuickSave(db);break;
         case 'v': AddSkill(db,2,1);break;
         case 'p': Import(db);break;
         case 'u': Propeties(db);break;
         case 't': ASortSkill(db); break;
         case 'z': exit( -1 );break;
        }
   }
}

void ShowLevels(DATA* db)
{
 fprintf(lg,"pokazuje poziomy\n");
 int skip = 0;
 cout<<"\nilosc poziomow umiejetnosci to "<<db->ilosc[5]<<endl;
 for (int i=0;i<db->ilosc[5];i++)
        {
         cout<<"poziom "<<i<<" koszt punktow umiejetnosci: "<<db->cost[i]<<endl;
         clear(i, CLEAR, skip);
        }
 ln();
}
void ShowLevels2(DATA* db)
{
  fprintf(lg,"pokazuje poziomy\n");
 int skip = 0;
 cout<<"\nilosc poziomow PD to "<<db->ilosc[6]<<endl;
 for (int i=0;i<db->ilosc[6];i++)
        {
         cout<<"poziom "<<i<<" koszt PD: "<<db->pd_cost[i]<<endl;
         clear(i, CLEAR, skip);
        }
 ln();
}
void SetLevels(DATA* db)
{
  fprintf(lg,"ustala poziomy\n");
 cout<<"max poziomow "<<MAX_LEVELS<<endl;
 ShowLevels(db);
 cout<<"zmienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 cout<<"ilosc poziomow: ";
 int temp;
 takeint(&temp, 18);
 if ((temp<0) || (temp>MAX_LEVELS)) return;
 db->ilosc[5] = temp;
 for (int i=0;i<temp;i++)
        {
         cout<<"koszt punkow umiejetnosci za poziom "<<i<<": ";
         takeint(&db->cost[i], i*i);
        }
}
void SetLevels2(DATA* db)
{
  fprintf(lg,"ustala poziomy\n");
 cout<<"max poziomow "<<MAX_LEVELS<<endl;
 ShowLevels2(db);
 cout<<"zmnienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 cout<<"ilosc poziomow: ";
 int temp;
 takeint(&temp, 16);
 if ((temp<0) || (temp>MAX_LEVELS)) return;
 db->ilosc[6] = temp;
 for (int i=0;i<temp;i++)
        {
         cout<<"ile PD potrzeba aby przejsc na poziom "<<i<<" umiejetnosci: ";
         takeint(&db->pd_cost[i], i*i);
        }
}
void ShowWoman(DATA* db)
{
  fprintf(lg,"modyfikatory plci\n");
 cout<<"modyfikatory plci dla kobiety.\n";
 int skip = 0;
 for (int i=0;i<db->ilosc[0];i++)
        {
         printf("(%-3d)%-25s\t%-3d\n",i,db->name_att[i],db->p_mod[i]);
         clear(i, CLEAR, skip);
        }
}
void SetGender(DATA* db)
{
 ShowWoman(db);
 ln();
 cout<<"zmienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
  fprintf(lg,"zmienia modyfikatory plci\n");
 for (int i=0;i<db->ilosc[0];i++)
        {
         printf("modyfikator %-25s dla kobiety, aktualny %-3d: ",db->name_att[i],db->p_mod[i]);
         takeint(&db->p_mod[i], 3, 1);
        }
}
void OptItem(DATA* db)
{
  fprintf(lg,"opcje\n");
 cout<<"\t\tilosc: "<<db->ilosc_ekw<<endl;
 cout<<"a\t-dodaj\n";
 cout<<"b\t-usun\n";
 cout<<"c\t-zobacz\n";
 cout<<"d\t-wyczysc rekord\n";
 cout<<"e\t-zapisz\n";
 cout<<"f\t-znajdz\n";
 cout<<"i\t-importuj\n";
 cout<<"u\t-zmien ceny\n";
 cout<<"r\t-startowa ilosc pieniedzy\n";
 cout<<"s\t-posortuj\n";
 cout<<"k\t-zakoncz\n";
 cout<<"z\t-zakoncz program\n";
}

void ShowItem(DATA* db)
{
 ln();
 fprintf(lg,"pokazuje przedmioty\n");
 int skip = 0;
 cout<<"proponowana startowa ilosc gotowki to: "<<db->def_cash<<endl;
 cout<<"\t\tilosc przedmiotow: "<<db->ilosc_ekw<<endl;
 for (int i=0;i<db->ilosc_ekw;i+=2)
        {
         printf("(%3d): %15s [%-4d$]\t",i,db->ekw[i],db->c_ekw[i]);
         if (i+1 < db->ilosc_ekw) printf("  (%3d): %15s [%-4d$]\n",i+1,db->ekw[i+1],db->c_ekw[i+1]);
         else ln();
         clear(i/2, CLEAR, skip);
        }
 ln();
}

void AddItem(DATA* db)
{
fprintf(lg,"dodaje przedmiot\n");
 if (db->ilosc_ekw == MAX_EKW){cout<<"rekord pelny!\n";return;}
 int temp = db->ilosc_ekw;
 cout<<"nazwa "<<temp<<" przedmiotu: ";
 takestring(db->ekw[temp]);
 cout<<"koszt "<<db->ekw[temp]<<": ";
 takeint(&db->c_ekw[temp], 500);
 db->ilosc_ekw++;
}

void DelItem(DATA* db)
{
 fprintf(lg,"usuwa przedmiot\n");
 if (db->ilosc_ekw == 0){cout<<"nie ma nic do usuniecia!\n";return;}
 ShowItem(db);
 lns(2);
 int temp;
 printf("numer (0 - %d): ",db->ilosc_ekw-1);
 takeint(&temp, 30);
 if ((temp<0) || (temp>db->ilosc_ekw-1)) return;
 db->ilosc_ekw--;
 for (int i=temp;i<db->ilosc_ekw;i++)
        {
         strcpy(db->ekw[i],db->ekw[i+1]);
         db->c_ekw[i] = db->c_ekw[i+1];
        }
}

void CashOnStart(DATA* db)
{
 cout<<"ilosc kasy na poczatek, aktualna $"<<db->def_cash<<": $";
 takeint(&db->def_cash, 1000);
}

void CopyItem(DATA* to, DATA* from)
{
  fprintf(lg,"kopiuje przedmioty\n");
 int max = to->ilosc_ekw + from->ilosc_ekw;
 if (max > MAX_EKW)
        {
         cout<<"powyzej "<<MAX_EKW<<" obetne.\n";
         max = MAX_EKW;
        }
  int old = to->ilosc_ekw;
  to->ilosc_ekw += from->ilosc_ekw;
  if (to->ilosc_ekw > MAX_EKW) to->ilosc_ekw = MAX_EKW;
  cout<<"skopiowac tez startowa ilosc kasy  $"<<from->def_cash<<"? ";
  char zn;
  takechar(&zn, 2, 't', 'n');
  if (zn == 't') to->def_cash = from->def_cash;
  for (int i = old;i<max;i++)
        {
         strcpy(to->ekw[i],from->ekw[i-old]);
         to->c_ekw[i] =   from->c_ekw[i-old];
         cout<<"#";
        }
}

void ImportItem(DATA* db)
{
 cout<<"zapisz aktualna baze w: ";
 char bname[STRING];
 char folder[STRING];
 strcpy(folder,"DataBase/");
 takestring(bname);
 strcat(folder,bname);
 FILE* plik;
 if ((plik = fopen(folder,"w+")) == NULL)
        {
         cout<<"nie da sie zapisac w: "<<folder<<endl;
         int nc = ncont(); if (nc)
         return;
        }
 Save(db,plik);
 fclose(plik);
 cout<<"importuj z: ";
 takestring(bname,3, "saradok", "morgoth", "galthar");
 strcpy(folder,"DataBase/");
 strcat(folder,bname);
 if ((plik = fopen(folder,"r+")) == NULL)
        {
         cout<<"nie ma takiego pliku: "<<folder<<endl;
         int nc = ncont(); if (nc)
         return;
        }
  DATA* db2 = new DATA;
  Load(db2,plik);
  int access = Check(db2);
  if (!access){cout<<"odmowa dostepu!\n";return;}
  CopyItem(db,db2);
  fclose(plik);
  delete db2;
}

void FindItem(DATA* db)
{
 fprintf(lg,"szuka przedmiotu\n");
 cout<<"podaj kilka pierwszych liter: ";
 char part[STRING];
 int correct;
 int skip = 0;
 int find = 0;
 takestring(part);
 downcase(part);
 cout<<"znalazlem:\n\n";
 for (int i=0;i<db->ilosc_ekw;i++)
        {
         correct = 0;
         for (int j=0;j<(int)strlen(part);j++)
                {
                 if ((part[j] == db->ekw[i][j]) || (part[j] == db->ekw[i][j] + 0x20)) correct++;
                }
         if (correct == (int)strlen(part))
         	{
             printf("(%-3d) %-30s koszt %-4d\n",i,db->ekw[i],db->c_ekw[i]);
             find++;
             clear(find, CLEAR, skip);
            }
        }
 if (find == 0) cout<<"nie ma rzeczy podobnych do "<<part<<endl;
}

void EditorOfItems(DATA* db)
{
 int il = db->ilosc_ekw;
 ShowItem(db);
 cout<<"od: ";
 int st; takeint(&st, 5);
 cout<<"do: ";
 int en; takeint(&en, 50);
 if ((st < 0) || (en >il-1)) return;
 for (int i=st;i<en;i++)
        {
         cout<<"aktualny "<<db->ekw[i]<<", koszt "<<db->c_ekw[i]<<" podaj nowy koszt lub -1 nastepny -2 anuluj: ";
         int lbb;
         takeint(&lbb, 10);
         if (lbb == -2) return;
         else if (lbb == -1) goto lab_end;
         else db->c_ekw[i] = lbb;
         lab_end: ;
        }
}
void SwapItem(DATA* db, int i, int j)
{
 char pom[STRING];
 int mod;

 strcpy(pom, db->ekw[j]);
 strcpy(db->ekw[j], db->ekw[i]);
 strcpy(db->ekw[i], pom);

 mod = db->c_ekw[j];
 db->c_ekw[j] = db->c_ekw[i];
 db->c_ekw[i] = mod;
}

void ASortItems(DATA* db)
{
 for (int i=0;i<db->ilosc_ekw;i++)
    {
     for (int j=0;j<db->ilosc_ekw-(i+1);j++)
        {
	 if (StrMore(db->ekw[j], db->ekw[j+1]))
	  {
	   SwapItem(db, j+1, j);
	  }
	}
     }
}
void SetItem(DATA* db)
{
  fprintf(lg,"ustala przedmioty\n");
 cout<<"max przedmiotow "<<MAX_EKW<<endl;
 ShowItem(db);
 cout<<"zmienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 char z = '%';
 while (z != 'k')
   {
 cout<<"Opcja (0-lista): ";
 takechar(&z);
 switch(z)
        {
         case '0': OptItem(db); break;
         case 'a': AddItem(db);break;
         case 'b': DelItem(db);break;
         case 'c': ShowItem(db);break;
         case 'd': db->ilosc_ekw = 0;break;
         case 'e': QuickSave(db);break;
         case 'i': ImportItem(db);break;
         case 'r': CashOnStart(db);break;
         case 'f': FindItem(db);break;
         case 'u': EditorOfItems(db);break;
         case 's': ASortItems(db); break;
         case 'z': exit( -1 );break;
        }
   }
}

void OptSpell(DATA* db, int cl)
{
 ln();
 fprintf(lg,"opcje zaklec\n");
 if (db->ilosc_poziomow < 1) db->ilosc_poziomow = 1;
 printf("\t\taktualny poziom: (%d/%d) , czarow %d/%d\n",cl,db->ilosc_poziomow-1,db->ilosc_czarow[cl],MAX_SPELL);
 cout<<"a\t-dodaj do aktualnego poziomu\n";
 cout<<"b\t-dodaj do wybranego poziomu\n";
 cout<<"c\t-usun z aktualnego poziomu\n";
 cout<<"d\t-usun z dowolnego poziomu\n";
 cout<<"e\t-zobacz wszystkie czary\n";
 cout<<"f\t-zobacz czary aktualnego poziomu\n";
 cout<<"g\t-zobacz czary wybranego poziomu\n";
 cout<<"h\t-wyczysc aktualny poziom\n";
 cout<<"i\t-wyczysc caly rekord\n";
 cout<<"j\t-zapisz\n";
 cout<<"l\t-znajdz\n";
 cout<<"m\t-importuj\n";
 cout<<"n\t-edytuj koszty zaklec\n";
 cout<<"o\t-edytuj wymagania zaklec\n";
 cout<<"p\t-zmien na inny poziom\n";
 cout<<"q\t-dodaj pziom\n";
 cout<<"r\t-usun poziom\n";
 cout<<"s\t-posortuj czary\n";
 cout<<"k\t-zakoncz\n";
 cout<<"z\t-zakoncz program\n";
}
void PreviewSpells(DATA* db, int lev = -1)
{
 ln();
 fprintf(lg,"pokazuje czary\n");
 int skip = 0;
 int counter = 0;
 cout<<"ilosc poziomow "<<db->ilosc_poziomow<<endl;
 if (lev == -1)
 {
 for (int i=0;i<db->ilosc_poziomow;i++)
        {
         cout<<"poziom "<<i<<", ilosc zaklec "<<db->ilosc_czarow[i]<<endl;
         counter++;
         for (int j=0;j<db->ilosc_czarow[i];j++)
                {
                 counter++;
                 printf("\t(%-2d)%-17s wymaga: %-17s PTS: %-3d\n",j,db->zaklecie[i][j],db->req_magic[i][j],db->spell_cost[i][j]);
                 clear(counter, CLEAR, skip);
                }
         ln();
        }
 }
 else if (lev == ASK)
        {
         printf("poziom (0-%d): ",db->ilosc_poziomow-1);
         int level;
         takeint(&level, 10);
         if ((level < 0) || (level >= db->ilosc_poziomow)) return;
         cout<<"poziom "<<level<<", ilosc zaklec "<<db->ilosc_czarow[level]<<endl;
         for (int j=0;j<db->ilosc_czarow[level];j++)
                {
                 counter++;
                 printf("\t(%-2d)%-17s wymaga: %-17s PTS: %-3d\n",j,db->zaklecie[level][j],db->req_magic[level][j],db->spell_cost[level][j]);
                 clear(counter, CLEAR, skip);
                }
         ln();
        }
 else
        {
         int level = lev;
         if ((level < 0) || (level >= db->ilosc_poziomow)) return;
         cout<<"poziom "<<level<<", ilosc zaklec "<<db->ilosc_czarow[level]<<endl;
         for (int j=0;j<db->ilosc_czarow[level];j++)
                {
                 counter++;
                 printf("\t(%-2d)%-17s wymaga: %-17s PTS: %-3d\n",j,db->zaklecie[level][j],db->req_magic[level][j],db->spell_cost[level][j]);
                 clear(counter, CLEAR, skip);
                }
         ln();
        }
 ln();
}

void DelSpell(DATA* db, int lev = -1)
{
 fprintf(lg,"usuwa zaklecie\n");
 if (lev == -1)
        {
         printf("poziom (0-%d): ",db->ilosc_poziomow-1);
         takeint(&lev, 20);
         if ((lev < 0) || (lev >= db->ilosc_poziomow)) return;
        }
 if (db->ilosc_czarow[lev] == 0){cout<<"na poziomie "<<lev<<" nie ma nic do usuniecia\n";return;}
 PreviewSpells(db,lev);
 cout<<"podaj numer zaklecia: ";
 int num;
 takeint(&num, 20);
 if ((num <0) || (num >= db->ilosc_czarow[lev])) return;
 db->ilosc_czarow[lev]--;
 for (int i=num;i<db->ilosc_czarow[lev];i++)
 	{
    db->spell_cost[lev][i] = db->spell_cost[lev][i+1];
    strcpy(db->zaklecie[lev][i],db->zaklecie[lev][i+1]);
    strcpy(db->req_magic[lev][i],db->req_magic[lev][i+1]);
   }
}

void AddSpell(DATA* db, int lev = -1)
{
 fprintf(lg,"dodaje zaklecie\n");
 if (lev == -1)
        {
         printf("poziom (0-%d): ",db->ilosc_poziomow-1);
         takeint(&lev, 10);
         if ((lev < 0) || (lev >= db->ilosc_poziomow)) return;
        }
 if (db->ilosc_czarow[lev] == MAX_SPELL){cout<<"rekord pelny!\n";return;}

 int temp = db->ilosc_czarow[lev];
 cout<<"nazwa "<<temp<<" zaklecia (poziom "<<lev<<"): ";
 takestring(db->zaklecie[lev][temp]);
 cout<<"koszt mocy "<<db->zaklecie[lev][temp]<<": ";
 takeint(&db->spell_cost[lev][temp], 10);
 once_again:
 cout<<"\nwybor wymaganej magii. . .\n";
 cout<<"-1\t-pokaz wszystkie umiejetnosci\n";
 cout<<"-2\t-pokaz kategorie umiejetnosci\n";
 cout<<"-3\t-pokaz um. o wlasnosci magia>0\n";
 cout<<"-4\t-pokaz um. o wlasnosci magia>n\n";
 cout<<"-5\t-pokaz umiejetnosci zaczynajace sie slowem \"magia\"\n";
 cout<<"-6\t-pokaz wszystkie umiejetnosci dokladnie\n";
 cout<<"-7\t-znajdz umiejetnosc\n";
 cout<<"mozesz tez podac znany ci juz numer umiejetnosci\n";
 cout<<"\nwybor: ";
 int search_type;
 takeint(&search_type, 8);
 switch(search_type)
        {
          case -1: ShowSkill(db,0);break;
          case -2: ShowSkill(db,3);break;
          case -3: ShowSkillMagic(db,1);break;
          case -4: ShowSkillMagic(db,USER);break;
          case -5: FindSkillWord(db,"magia");break;
          case -6: ShowSkill(db);break;
          case -7: FindSkill(db,0);break;
        }
  if (search_type < 0) cout<<"podaj numer umiejetnosci: ";
  int num;
  if (search_type < 0) takeint(&num, 200);
  else num = search_type;
  if ((num <0) || (num > db->ilosc[3]))
  	{
    cout<<"poza zakresem s(t)op lb (o)d nowa: ";
    char znk;
    takechar(&znk, 2, 't', 'o');
    if (znk == 'o') goto once_again;
    else return;
   }
  strcpy(db->req_magic[lev][temp],db->s_name[num]);
  cout<<"poziom ("<<lev<<") "<<db->zaklecie[lev][temp]<<" koszt "<<db->spell_cost[lev][temp]<<", wymaga magii: "<<db->req_magic[lev][temp]<<endl;
  cout<<"OK? ";
  char oki;
  takechar(&oki, 2, 't', 'n');
  if (oki == 'n') goto once_again;
  db->ilosc_czarow[lev]++;
}

void CombineLevels(DATA* db)
{
     if (db->ilosc_poziomow == 0){cout<<"nie ma nic do usuniecia.\n";return;}
     printf("poziom (0-%d) lub -1 usun wszystkie: ",db->ilosc_poziomow-1);
     int lb;
     takeint(&lb, 3);
     if (lb == -1)
     	{
       cout<<"zabijam...\n";
       for (int i=0;i<db->ilosc_poziomow;i++)
       	{
          for (int j=0;j<db->ilosc_czarow[i];j++)
          	{
             strcpy(db->zaklecie[i][j],"KILLED");
             strcpy(db->req_magic[i][j],"KILLED");
             db->spell_cost[i][j] = 0;
            }
          db->ilosc_czarow[i] = 0;
         }
       db->ilosc_poziomow = 1;
       return;
      }
    if ((lb < 0) || (lb >= db->ilosc_poziomow)) return;
    db->ilosc_poziomow--;
    cout<<"kopiuje...\n";
    for (int i=lb;i<db->ilosc_poziomow;i++)
    	{
       db->ilosc_czarow[i] = db->ilosc_czarow[i+1];
       for (int j=0;j<db->ilosc_czarow[i];j++)
       	{
         strcpy(db->zaklecie[i][j],db->zaklecie[i+1][j]);
         strcpy(db->req_magic[i][j],db->req_magic[i][j]);
         db->spell_cost[i][j] = db->spell_cost[i][j];
         }
      }
}

void EditSpell(DATA* db, int mode)
{
 fprintf(lg,"edytuje zaklecie\n");
 printf("poziom (0-%d): ",db->ilosc_poziomow-1);
 int lev;
 takeint(&lev,10);
 if ((lev < 0) || (lev >= db->ilosc_poziomow))return;
 int il = db->ilosc_czarow[lev];
 PreviewSpells(db,lev);
 cout<<"od: ";
 int st; takeint(&st, 5);
 cout<<"do: ";
 int en; takeint(&en, 50);
 if ((st < 0) || (en >il-1)) return;
 for (int i=st;i<en;i++)
        {
         cout<<"aktualnie "<<db->zaklecie[lev][i]<<", koszt "<<db->spell_cost[lev][i]<<endl;
         if (mode == COST)
         	{
             cout<<"nowy koszt lub -1 nastepne -2 anuluj: ";
             int lbb;
             takeint(&lbb, 25);
             if (lbb == -2) return;
             else if (lbb == -1) goto lab_end;
             else db->spell_cost[lev][i] = lbb;
         	}
         if (mode == REQ)
         	{
             once_again:
             cout<<"\nwybor wymaganej magii. . .\n";
        cout<<"-1\t-pokaz wszystkie umiejetnosci\n";
        cout<<"-2\t-pokaz kategorie umiejetnosci\n";
        cout<<"-3\t-pokaz um. o wlasnosci magia>0\n";
        cout<<"-4\t-pokaz um. o wlasnosci magia>n\n";
        cout<<"-5\t-pokaz umiejetnosci zaczynajace sie slowem \"magia\"\n";
        cout<<"-6\t-pokaz wszystkie umiejetnosci dokladnie\n";
        cout<<"-7\t-znajdz umiejetnosc\n";
        cout<<"mozesz tez podac znany ci juz numer umiejetnosci\n";
             cout<<"\nwybor: ";
             int search_type;
             takeint(&search_type, 8);
             switch(search_type)
              {
          case 1: ShowSkill(db,0);break;
          case 2: ShowSkill(db,3);break;
          case 3: ShowSkillMagic(db,1);break;
          case 4: ShowSkillMagic(db,USER);break;
          case 5: FindSkillWord(db,"magia");break;
          case 6: ShowSkill(db);break;
          case 7: FindSkill(db,0);break;
              }
            if (search_type < 0)cout<<"numer lub -1 nastepne lub -2 anuluj: ";
            int num;
            if (search_type < 0) takeint(&num, 10);
            else num = search_type;
            if (num == -2) return;
            else if (num == -1) goto lab_end;
            if ((num <0) || (num > db->ilosc[3]))
  					{
                cout<<"Poza zakresem s(t)op czy (o)d nowa? ";
                char znk;
                takechar(&znk, 2, 't', 'o');
                if (znk == 'o') goto once_again;
                else return;
               }
            strcpy(db->req_magic[lev][i],db->s_name[num]);
            cout<<"poziom ("<<lev<<") "<<db->zaklecie[lev][i]<<" koszt "<<db->spell_cost[lev][i]<<", wymagana magia: "<<db->req_magic[lev][i]<<endl;
            cout<<"OK? ";
            char oki;
            takechar(&oki, 2, 't', 'n');
            if (oki == 'n') goto once_again;
              }
              lab_end: ;
         }
}

void CopySpells(DATA* to, DATA* from)
{
 fprintf(lg,"kopiuje zaklecia z innej bazy\n");
 to->ilosc_poziomow = from->ilosc_poziomow;
 if ((to->ilosc_poziomow <0) || (to->ilosc_poziomow > MAX_LEV_S))
 	{
    cout<<"Nastapil blad, skontaktuj sie z administratorem.\n";
    int nc = ncont(); if (nc)
    return;
   }
for (int i=0;i<to->ilosc_poziomow;i++)
	{
    int max = to->ilosc_czarow[i] + from->ilosc_czarow[i];
    if (max > MAX_SPELL)
        {
         cout<<"na poziomie "<<i<<", za duzo zaklec powyzej "<<MAX_SPELL<<" obetne.\n";
         max = MAX_SPELL;
        }
    int old = to->ilosc_czarow[i];
    to->ilosc_czarow[i] += from->ilosc_czarow[i];
    if (to->ilosc_czarow[i] > MAX_SPELL) to->ilosc_czarow[i] = MAX_SPELL;
    for (int j=old;j<max;j++)
    	{
      strcpy(to->zaklecie[i][j],from->zaklecie[i][j-old]);
      strcpy(to->req_magic[i][j],from->req_magic[i][j-old]);
      to->spell_cost[i][j] =   from->spell_cost[i][j-old];
      cout<<"#";
      }
   }
}


void ImportSpell(DATA* db)
{
 cout<<"zapisz aktualna baze w: ";
 char bname[STRING];
 char folder[STRING];
 strcpy(folder,"DataBase/");
 takestring(bname);
 strcat(folder,bname);
 FILE* plik;
 if ((plik = fopen(folder,"w+")) == NULL)
        {
         cout<<"nie moge zapisac w: "<<folder<<", skontaktuj sie z administratorem"<<endl;
         int nc = ncont(); if (nc)
         return;
        }
 Save(db,plik);
 fclose(plik);
 cout<<"importuj z: ";
 takestring(bname,3,"morgoth", "galthar", "saradok");
 strcpy(folder,"DataBase/");
 strcat(folder,bname);
 if ((plik = fopen(folder,"r+")) == NULL)
        {
         cout<<"brak pliku: "<<folder<<", zglos ten blad do administratora"<<endl;
         int nc = ncont(); if (nc)
         return;
        }
  DATA* db2 = new DATA;
  Load(db2,plik);
  int access = Check(db2);
  if (!access){cout<<"brak dostepu, pogadaj z administratorem\n";return;}
  CopySpells(db,db2);
  fclose(plik);
  delete db2;
}
void FindSpell(DATA* db)
{
 fprintf(lg,"szuka zaklecia\n");
 printf("poziom do przeszukania (0-%d) lub -1 wszystkie: ",db->ilosc_poziomow-1);
 int lev;
 takeint(&lev, 10);
 cout<<"podaj kilka pierwszych liter: ";
 char part[STRING];
 int correct;
 int find = 0;
 int skip = 0;
 takestring(part);
 DecompressData(part);
 downcase(part);
 cout<<"znalazlem:\n\n";
 if (lev >= 0)
 {
 int max = db->ilosc_czarow[lev];
 for (int i=0;i<max;i++)
        {
         correct = 0;
         for (int j=0;j<(int)strlen(part);j++)
                {
                 if ((part[j] == db->zaklecie[lev][i][j]) || (part[j] == db->zaklecie[lev][i][j] + 0x20)) correct++;
                }
         if (correct == (int)strlen(part))
         	{
             printf("(%-3d)%s poziom %d\n",i,db->zaklecie[lev][i],lev);
             find++;
             clear(find, CLEAR, skip);
            }
        }
 if (find == 0) cout<<"brak zaklec podobnych do "<<part<<", zloz zazalenia do administratora"<<endl;
 }
 else
 	{
 	 for (int poz=0;poz<db->ilosc_poziomow;poz++)
 		{
 		 int max = db->ilosc_czarow[poz];
       for (int i=0;i<max;i++)
          {
           correct = 0;
           for (int j=0;j<(int)strlen(part);j++)
                {
                 if ((part[j] == db->zaklecie[poz][i][j]) || (part[j] == db->zaklecie[poz][i][j] + 0x20)) correct++;
                }
           if (correct == (int)strlen(part))
         	    {
                 printf("(%-3d)%s poziom %d\n",i,db->zaklecie[poz][i],poz);
                 find++;
                 clear(find, CLEAR, skip);
                }
          }
     }
   if (find == 0) cout<<"brak zaklec podobnych do "<<part<<", skontaktuj sie z administratorem"<<endl;
  }
}
void SwapSpell(DATA* db, int lev, int i, int j)
{
 char pom[STRING];
 int mod;

 strcpy(pom,db->zaklecie[lev][j]);
 strcpy(db->zaklecie[lev][j],db->zaklecie[lev][i]);
 strcpy(db->zaklecie[lev][i], pom);

 strcpy(pom,db->req_magic[lev][j]);
 strcpy(db->req_magic[lev][j],db->req_magic[lev][i]);
 strcpy(db->req_magic[lev][i], pom);

 mod = db->spell_cost[lev][j];
 db->spell_cost[lev][j] = db->spell_cost[lev][i];
 db->spell_cost[lev][i] = mod;
}

void ASortSpells(DATA* db)
{
 fprintf(lg,"sortuje zaklecia\n");
 for (int O=0;O<db->ilosc_poziomow;O++)
  {
  for (int i=0;i<db->ilosc_czarow[O];i++)
    {
     for (int j=0;j<db->ilosc_czarow[O]-(i+1);j++)
        {
	 if (StrMore(db->zaklecie[O][j], db->zaklecie[O][j+1]))
	  {
	   SwapSpell(db, O, j+1, j);
	  }
	}
     }
  }
}
void SetSpells(DATA* db)
{
 fprintf(lg,"ustala zaklecia\n");
 PreviewSpells(db);
 ln();
 cout<<"zmienic? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 char z = '%';
 int cl = 0;
 int x,ll;
 while (z != 'k')
   {
 cout<<"Opcja (0-lista): ";
 takechar(&z);
 switch(z)
        {
         case '0': OptSpell(db, cl); break;
         case 'a': AddSpell(db,cl);break;
         case 'b': AddSpell(db); break;
         case 'c': DelSpell(db,cl);break;
         case 'd': DelSpell(db); break;
         case 'e': PreviewSpells(db);break;
         case 'f': PreviewSpells(db,cl); break;
         case 'g': PreviewSpells(db,ASK); break;
         case 'h': db->ilosc_czarow[cl] = 0;break;
         case 'i': for (x=0;x<db->ilosc_poziomow;x++) db->ilosc_czarow[x] = 0;break;
         case 'j': QuickSave(db);break;
         case 'l': FindSpell(db);break;
         case 'm': ImportSpell(db);break;
         case 'n': EditSpell(db,COST);break;
         case 's': ASortSpells(db);break;
         case 'o': EditSpell(db,REQ);break;
         case 'p':
         printf("skocz do (0-%d): ",db->ilosc_poziomow-1);
         takeint(&ll, 10);
         if ((ll>=0) && (ll <db->ilosc_poziomow)) cl = ll;
         break;
         case 'q':
         if (db->ilosc_poziomow < MAX_LEV_S) db->ilosc_poziomow++;
         break;
         case 'r':CombineLevels(db);break;
         case 'z': exit( -1 );break;
        }
   }
}

void Operations(DATA* base)
{
 fprintf(lg,"operacje na bazie danych:\n");
 CheckPassword(base);
 SetAttrib(base);
 SetTrial(base,0);
 SetTrial(base,1);
 SetCateg(base);
 SetSkill(base);
 SetLevels(base);
 SetLevels2(base);
 SetGender(base);
 SetItem(base);
 SetSpells(base);
}
void DataBase(char* db, DATA* base)
{
 fprintf(lg,"sprobuje wczytac baze %s\n", db);
 FILE* plik;
 if ((plik = fopen(db,"r+")) == NULL)
        {
         cout<<"blad, skontaktuj sie z administratorem: \nbrak pliku: "<<db<<endl;
         fprintf(lg,"-- error -- nie ma takiej bazy!\n");
         if ((plik = fopen(db,"w+")) == NULL)
                {
                 cout<<"nie mozna zapisywac w "<<db<<", to na pewno wina administratora\n";
                 fprintf(lg,"-- error -- nie da sie jej utworzyc!\n");
                 int nc = ncont(); if (nc)
                 exit( -1 );
                }
         else fclose(plik);
         cout<<"stworzylem plik "<<db<<" poinformuj o tym administratora\n\n";
         fprintf(lg,"baza utworzona\n");
         int nc = ncont(); if (nc)
         exit( -1 );
        }
 Load(base,plik);
 base->lr++;
 fprintf(lg,"bazy uzywano %d razy\n", base->lr);
 fclose(plik);
 if ((plik = fopen(db,"w+"))==NULL)
        {
         cout<<"nie moge zapisac w "<<db<<", zadzwon do administratora"<<endl;
         fprintf(lg,"--error -- bazy nie da sie zapisac\n");
         int nc = ncont(); if (nc)
         exit( -1 );
        }
 Save(base,plik);
 fclose(plik);
 cout<<"zmienic "<<db<<"? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 Operations(base);
 if ((plik = fopen(db,"w+"))==NULL)
        {
         cout<<"Bardzo zly blad, nie moge zapisac do "<<db<<" masz prawo wnerwiac sie na administraora"<<endl;
         fprintf(lg,"-- fatal -- error -- nie da sie zapisac bazy (to dziwne, jeszcze niedawno sie dalo)\n");
         int nc = ncont(); if (nc)
         exit( -1 );
        }
 Save(base,plik);
 fclose(plik);
 BaseHTML(base,db);
 fprintf(lg,"to tyle na temat DB\n");
}
void AboutMM0(RPG* post, char* bname2)
{
 fprintf(lg,"sprawdzam wpis MM w postaci\n");
 char bname[STRING];
 strcpy(bname, bname2);
 downcase(bname);
 if (strcmp(bname,"database/saradok") == 0)
        {
         if (post->cecha[post->numMM] < 0)
                {
                 post->cecha[post->numMM] = 0;
                }
        }
 else return;
}
void Protection(int check, RPG* post,FILE* plik)
{
 fprintf(lg,"sprawdzam poprawnosc zapisu postaci\n");
 if (!check)
        {
         fprintf(lg,"-- program oops -- postac blednie zapisana\n");
         lns(6);
         cout<<"blad przy wczytywaniu postaci\n";
         cout<<"kontynuacja moze ja uszkodzic (i pewnie to zrobi)\n";
         cout<<"skargi nalezy skladac do administratora\n";
         lns(4);
         char zn;
         cout<<"kontynuowac mimo to? ";
         takechar(&zn, 1, 'n');
         if (zn != 't') {delete post; exit( -1 );}
         }
}

void LoadChar(RPG* post, FILE* plik)
{
 cout<<"wczytuje postac.\n";
 fprintf(lg,"wczytuje postac\n");
 int check;
 check = fscanf(plik,"%d\n",&post->ilosc_danych);
 Protection(check,post,plik);
 for (int i=0;i<post->ilosc_danych;i++)
        {
         fscanf(plik,"%s\n",post->dane[i]);
         DecompressData(post->dane[i]);
        }
 fscanf(plik,"%d\n",&post->ilosc_cech);
 for (int i=0;i<post->ilosc_cech;i++)
        {
         fscanf(plik,"%d\n",&post->cecha[i]);
         fscanf(plik,"%s\n",post->n_cechy[i]);
         DecompressData(post->n_cechy[i]);
        }
 fscanf(plik,"%x\n",&post->sv);
 fscanf(plik,"%d\n",&post->st);
 fscanf(plik,"%d\n",&post->il_um);
 for (int i=0;i<post->il_um;i++)
        {
         fscanf(plik,"%s %d %d %d %d %d\n",post->um[i],&post->tesum[i],&post->pozum[i],&post->trum[i],&post->ceum[i],&post->mulum[i]);
         DecompressData(post->um[i]);
        }
 fscanf(plik,"%d\n",&post->il_ekw);
 for (int i=0;i<post->il_ekw;i++)
        {
         fscanf(plik,"%s\n",post->ekw[i]);
         DecompressData(post->ekw[i]);
        }
 fscanf(plik,"%d\n",&post->PD);
 for (int i=0;i<post->ilosc_cech;i++)
        {
         fscanf(plik,"%d\n",&post->p_c[i]);
        }
 for (int i=0;i<post->il_um;i++)
        {
         fscanf(plik,"%d\n",&post->p_u[i]);
        }
 check = fscanf(plik,"%d\n",&post->rl);
 Protection(check,post,plik);
 for (int i=0;i<post->il_um;i++)
        {
         fscanf(plik,"%d\n",&post->magum[i]);
        }
 fscanf(plik,"%d\n",&post->stMM);
 fscanf(plik,"%d\n",&post->numMM);
 fscanf(plik,"%d\n",&post->IsDead);
 fscanf(plik,"%d\n",&post->lev);
 fscanf(plik,"%d\n",&post->lev_p);
 fscanf(plik,"%s\n",post->pname);
 DecompressData(post->pname);
 fscanf(plik,"%d\n",&post->cash);
 fscanf(plik,"%d\n",&post->sp);
 fscanf(plik,"%d\n",&post->spt);
 fscanf(plik,"%d\n",&post->max_lev);
 check = fscanf(plik,"%d\n",&post->il_spell);
 Protection(check,post,plik);
 for (int i=0;i<post->il_spell;i++)
 	{
    fscanf(plik,"%d\n",&post->spell_cost[i]);
    fscanf(plik,"%d\n",&post->spell_level[i]);
    fscanf(plik,"%s\n",post->spell[i]);
    DecompressData(post->spell[i]);
   }
 check = fscanf(plik,"%d\n",&post->mage);
 Protection(check,post,plik);
 if (post->IsDead)
 	{
    lns(5);
    fprintf(lg,"-- oops -- wczytales trupa.\n");
    cout<<"ta postac to TRUPOSZ.\n";
    cout<<"ten program nie zezwala na bezczeszczenie zwlok\n";
    cout<<"wyslij maila administratorowi lub zobacz postac poleceniem: vpost [name]\n";
    exit( -1 );
   }
}

void CheckPassChar(RPG* db)
{
 int a;
 fprintf(lg,"sprawdzam kod postaci\n");
 if (db->sv == -1) {cout<<"odmowa dostepu\n"; exit( -1 );}
 if (db->sv != 0)
        {
         cout<<"podaj sekretny kod: ";
         secretake(a);
         if (a != db->sv)
                {
                 cout<<"odmowa dostepu!\n";
                 cout<<"pogadaj z administratorem\n";
                 exit( -1 );
                }
        }
 cout<<"zmienic sekretny kod? ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn == 't') {cout<<"podaj nowy lub -1 odmowa dostepu: ";secretake(db->sv);}
}
void DefaultValues(RPG* post, DATA* db)
{
  fprintf(lg,"wczytuje wartosci domyslne z bazy danych\n");
 post->ilosc_cech = db->ilosc[0];
 post->st = db->st;
 for (int i=0;i<db->ilosc[0];i++)
        {
         post->cecha[i] = db->value_att[i];
         strcpy(post->n_cechy[i],db->name_att[i]);
        }
 post->il_um  = 0;
 post->il_ekw = 0;
 post->numMM = db->mm;
}
void Woman(RPG* post, DATA* db)
{
 fprintf(lg,"transformuje te postac w kobiete\n");
 strcpy(post->dane[PLEC],"kobieta");
 for (int i=0;i<post->ilosc_cech;i++) post->cecha[i] += db->p_mod[i];
}
void MainData(RPG* post, DATA* db)
{
 fprintf(lg,"pobieram glowne dane\n");
 post->PD = 0;
 cout<<"imie: ";          takestring(post->dane[IMIE]);
 cout<<"nazwisko: ";       takestring(post->dane[NAZWISKO]);
 cout<<"charakter: ";     takestring(post->dane[CHARAKTER]);
 cout<<"kolor oczu: ";    takestring(post->dane[OCZY]);
 cout<<"kolor wlosow: ";    takestring(post->dane[WLOSY]);
 cout<<"kolor skory: ";          takestring(post->dane[CERA]);
 cout<<"wzrost: ";        takestring(post->dane[WZROST]);
 cout<<"waga: ";        takestring(post->dane[WAGA]);
 cout<<"wiek: ";           takestring(post->dane[WIEK]);
 char zn = 'a';
 while ((zn != 'm') && (zn != 'k'))
 {
 cout<<"plec [m/k] ";
 takechar(&zn, 2, 'm', 'k');
 }
 if (zn == 'k') Woman(post,db);
 else strcpy(post->dane[PLEC],"mezczyzna");
}
void Uncompress(RPG* post)
{
 fprintf(lg,"dekompresuje postac\n");
 DecompressData(post->pname);
 for (int i=0;i<post->ilosc_danych;i++)
        {
         DecompressData(post->dane[i]);
        }
 for (int i=0;i<post->ilosc_cech;i++)
        {
         DecompressData(post->n_cechy[i]);
        }
 for (int i=0;i<post->il_um;i++)
        {
         DecompressData(post->um[i]);
        }
 for (int i=0;i<post->il_ekw;i++)
        {
         DecompressData(post->ekw[i]);
        }
 for (int i=0;i<post->il_spell;i++)
        {
         DecompressData(post->spell[i]);
        }
}
void SaradokMM(RPG* post, char* name2)
{
 fprintf(lg,"przeliczm MM\n");
 char name[STRING];
 strcpy(name, name2);
 downcase(name);
 if (strcmp(name,"database/saradok") == 0)
        {
         int ran = (random(6) + random(6) +2);
         cout<<"losuje moc: "<<ran<<endl;
         post->cecha[10] += ran;
         cout<<"masz "<<post->cecha[10]<<" punktow mocy.\n";
         if (post->cecha[10] >= 10) cout<<"mozesz byc czarodziejem\n";
         if (post->cecha[10] >= 8) cout<<"mozesz byc kaplanem.\n";
        }
 if (strcmp(name,"database/base1940") == 0)
        {
         int ran = ((random(10)+1) * (random(10)+1) * (random(10)+1)/100);
         cout<<"losuje psi: "<<ran<<endl;
         post->cecha[9] += ran;
         cout<<"masz "<<post->cecha[9]<<" PSI.\n";
        }
 else return;
}
void Call(RPG* post)
{
 fprintf(lg,"postac\n");
 printf("%s %s %s %s\n",post->dane[IMIE],post->dane[NAZWISKO],post->dane[RASA],post->dane[PROFESJA]);
}
void ShowAttChar(RPG* post)
{
 fprintf(lg,"pokazuje cechy postaci\n");
 Call(post);
 int skip = 0;
 cout<<"\n\nilosc cech: "<<post->ilosc_cech<<endl;
 for (int i=0;i<post->ilosc_cech;i++)
        {
         printf("(%-3d)%-25s\t%d\n",i,post->n_cechy[i],post->cecha[i]);
         clear(i, CLEAR, skip);
        }
}

void EditDBRace(DATA* db, short trial)
{
 fprintf(lg,"probuje bezposredniego dostepu do ras z bazy\n");
 bool access = Check(db);
 if (!access) return;
 SetTrial(db,trial);
 cout<<"zapisz w lub null: ";
 char dane[STRING];
 char name[STRING];
 strcpy(name,"DataBase/");
 takestring(dane);
 if (strcmp(dane,"null") == 0) return;
 strcat(name,dane);
 FILE* plik;
 if ((plik = fopen(name,"w+")) == NULL)
 	{
    cout<<"nie moge zapisac "<<name<<" to wina administratora"<<endl;
    int nc = ncont(); if (nc)
    return;
   }
 Save(db,plik);
 fclose(plik);
}
void ChooseRace(RPG* post,DATA* db)
{
 fprintf(lg,"wybieram rase\n");
 strcpy(post->dane[RASA],".");
 strcpy(post->dane[PROFESJA],".");
 lab:
 Call(post);
 ShowTrial(db,0);ln();
 cout<<"numer rasy lub -1 wlasna lub -2 edytuj baze: ";
 int num;takeint(&num, 10);
 if (num  == -2)
 	{
    EditDBRace(db,0);
    goto lab;
   }
 if (num == -1)
        {
         cout<<"nazwa: ";
         takestring(post->dane[RASA]);
         int mod;
         for (int i=0;i<post->ilosc_cech;i++)
              {
               cout<<"modyfikator "<<post->n_cechy[i]<<": ";
               takeint(&mod, 16);
               post->cecha[i] += mod;
              }
        }
 if ((num<0) || (num>db->ilosc[1])) return;
 strcpy(post->dane[RASA],db->name[0][num]);
 for (int i=0;i<post->ilosc_cech;i++)
        {
         post->cecha[i] += db->mod[0][num][i];
        }
 ShowAttChar(post);
}
void ChooseProf(RPG* post,DATA* db)
{
 fprintf(lg,"wybieram profesje\n");
 lab:
 Call(post);
 ShowTrial(db,1);ln();
 cout<<"numer profesji lub -1 wlasna lub -2 edytuj baze: ";
 int num;takeint(&num, 16);
 if (num  == -2)
 	{
    EditDBRace(db,1);
    goto lab;
   }
 if (num == -1)
        {
         cout<<"nazwa: ";
         takestring(post->dane[PROFESJA]);
         int mod;
         for (int i=0;i<post->ilosc_cech;i++)
              {
               cout<<"modyfikator "<<post->n_cechy[i]<<": ";
               takeint(&mod, 16);
               post->cecha[i] += mod;
              }
        }
 if ((num<0) || (num>db->ilosc[2])) return;
 strcpy(post->dane[PROFESJA],db->name[1][num]);
 for (int i=0;i<post->ilosc_cech;i++)
        {
         post->cecha[i] += db->mod[1][num][i];
        }
 ShowAttChar(post);
}

void ProtectorChar(RPG* post)
{
 fprintf(lg,"ochrona\n");
 cout<<"chronic postac sekretnym kodem? ";
 char zn;
 takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 cout<<"podaj kod: ";
 secretake(post->sv);
}
void SaveChar(RPG* post, FILE* plik,char* fn)
{
 fprintf(lg,"zapisuje postac\n");
 if ((plik = fopen(fn,"w+")) == NULL)
        {
         cout<<"nie moge zapisac w "<<fn<<endl;
         cout<<"wyslij FAX do administratora\n";
         fprintf(lg,"-- error -- nie mozna zapisac postaci w %s", fn);
         int nc = ncont(); if (nc)
         exit( -1 );
        }
 cout<<"zapisuje postac.\n";
 fprintf(plik,"%d\n",post->ilosc_danych);
 for (int i=0;i<post->ilosc_danych;i++)
        {
         CompressData(post->dane[i]);
         fprintf(plik,"%s\n",post->dane[i]);
        }
 fprintf(plik,"%d\n",post->ilosc_cech);
 for (int i=0;i<post->ilosc_cech;i++)
        {
         fprintf(plik,"%d\n",post->cecha[i]);
         CompressData(post->n_cechy[i]);
         fprintf(plik,"%s\n",post->n_cechy[i]);
        }
 fprintf(plik,"%x\n",post->sv);
 fprintf(plik,"%d\n",post->st);
 fprintf(plik,"%d\n",post->il_um);
 for (int i=0;i<post->il_um;i++)
        {
         CompressData(post->um[i]);
         fprintf(plik,"%s %d %d %d %d %d\n",post->um[i],post->tesum[i],post->pozum[i],post->trum[i],post->ceum[i],post->mulum[i]);
        }
 fprintf(plik,"%d\n",post->il_ekw);
 for (int i=0;i<post->il_ekw;i++)
        {
         CompressData(post->ekw[i]);
         fprintf(plik,"%s\n",post->ekw[i]);
        }
 fprintf(plik,"%d\n",post->PD);
 for (int i=0;i<post->ilosc_cech;i++)
        {
         fprintf(plik,"%d\n",post->p_c[i]);
        }
 for (int i=0;i<post->il_um;i++)
        {
         fprintf(plik,"%d\n",post->p_u[i]);
        }
 fprintf(plik,"%d\n",post->rl);
  for (int i=0;i<post->il_um;i++)
        {
         fprintf(plik,"%d\n",post->magum[i]);
        }
 fprintf(plik,"%d\n",post->stMM);
 fprintf(plik,"%d\n",post->numMM);
 fprintf(plik,"%d\n",post->IsDead);
 fprintf(plik,"%d\n",post->lev);
 fprintf(plik,"%d\n",post->lev_p);
 CompressData(post->pname);
 fprintf(plik,"%s\n",post->pname);
 fprintf(plik,"%d\n",post->cash);
 fprintf(plik,"%d\n",post->sp);
 fprintf(plik,"%d\n",post->spt);
 fprintf(plik,"%d\n",post->max_lev);
 fprintf(plik,"%d\n",post->il_spell);
 for (int i=0;i<post->il_spell;i++)
 	{
    fprintf(plik,"%d\n",post->spell_cost[i]);
    fprintf(plik,"%d\n",post->spell_level[i]);
    CompressData(post->spell[i]);
    fprintf(plik,"%s\n",post->spell[i]);
   }
 fprintf(plik,"%d\n",post->mage);
 fclose(plik);
 Uncompress(post);
}
void RandomAtt(RPG* post,DATA* db)
{
 fprintf(lg,"losuje cechy\n");
 int pl = 2*post->st;
 int temp[MAX_ATT][5];
 for (int i=0;i<post->ilosc_cech;i++)
        {
         for (int j=0;j<5;j++)
                {
                 temp[i][j] = post->cecha[i] + random(2*pl+1)-pl;
                }
        }
 printf("%-20s ","ce, numer do wyboru>");
 for (int j=0;j<5;j++)
        {
         printf("%-3d ",j);
        }
 lns(2);
 for (int i=0;i<post->ilosc_cech;i++)
        {
          printf("%-20s ",post->n_cechy[i]);
         for (int j=0;j<5;j++)
                {
                 printf("%-3d ",temp[i][j]);
                }
         ln();
        }
 int suma;
 cout<<"\nnumer: ";
 takeint(&suma, 5);
 if ((suma<0) || (suma>5)) return;
 for (int i=0;i<post->ilosc_cech;i++)
        {
         post->cecha[i] = temp[i][suma];
        }
 ln();
 ShowAttChar(post);
}
void OwnAtt(RPG* post)
{
 fprintf(lg,"wlasne plusy\n");
 cout<<"ile plusow [0 - 3]: ";
 int pl;takeint(&pl, 4);
 if ((pl<1) || (pl>3)) return;
 int temp = pl;
 int num;
 int ile;
 int prev = -1;
 while (pl>0)
        {
         ShowAttChar(post);
         back:
         cout<<"numer cechy do zwiekszenia: ";
         takeint(&num, 16);
         if (num == prev) {cout<<"juz ja zmieniales\n";goto back;}
         if (num<0) num = 0;
         if (num>post->ilosc_cech-1) num = post->ilosc_cech-1;
         prev = num;
         cout<<"ile plusow do "<<post->n_cechy[num]<<": ";
         takeint(&ile, 3);
         if (ile<0) ile = 0;
         if (ile>2) ile = 2;
         if (ile>pl) ile = pl;
         post->cecha[num] += ile*post->st;
         pl -= ile;
        }
 pl = temp;
 prev = -1;
 while (pl>0)
        {
         ShowAttChar(post);
         back2:
         cout<<"cecha do zmniejszenia: ";
         takeint(&num, 16);
         if (num == prev) {cout<<"nie da rady\nskontaktuj sie z administratorem\n";goto back2;}
         if (num<0) num = 0;
         if (num>post->ilosc_cech-1) num = post->ilosc_cech-1;
         prev = num;
         cout<<"ile minusow do "<<post->n_cechy[num]<<": ";
         takeint(&ile, 3);
         if (ile<0) ile = 0;
         if (ile>2) ile = 2;
         if (ile>pl) ile = pl;
         post->cecha[num] -= ile*post->st;
         pl -= ile;
        }
}

void OptSkill(RPG* post, int lum)
{
 fprintf(lg,"opcje, lum=%d", lum);
 cout<<"\n\npunktow: "<<lum<<"\numiejetnosci: "<<post->il_um<<"\n\n";
 cout<<"a\t-dodaj\n";
 cout<<"v\t-znajdz i dodaj\n";
 cout<<"i\t-wyszukiwarka czasu rzeczywistego\n";
 cout<<"b\t-zmodyfikuj\n";
 cout<<"c\t-zobacz wszystkie z bazy\n";
 cout<<"t\t-zobacz wszystkie z bazy dokladnie\n";
 cout<<"d\t-zobacz jakie juz masz\n";
 cout<<"e\t-usun ostatnio wzieta\n";
 cout<<"f\t-usun wybrana\n";
 cout<<"g\t-usun wszystkie\n";
 cout<<"h\t-zobacz koszty poziomow\n";
 cout<<"s\t-zapisz postac\n";
 cout<<"k\t-koniec\n";
 cout<<"q\t-edytuj baze\n";
 cout<<"z\t-koniec programu\n";
}
void MiniShow(RPG* p, int i)
{
 fprintf(lg,".");
  printf("%-3d.(%-2d)%-18s %-2d %-3d\n",i,p->trum[i],p->um[i],p->pozum[i],p->tesum[i]);
}
void ShowSkill(RPG* post)
{
 fprintf(lg,"pokazuje umiejetnosci\n");
 int skip = 0;
 ln();
 for (int i=0;i<post->il_um;i++)
        {
         MiniShow(post,i);
         clear(i, CLEAR, skip);
        }
 ln();
}
void AddSkill(RPG* post, DATA* db, int* lum, short modify = 0)
{
 fprintf(lg,"dodaje umiejetnosc\n");
 if ((post->il_um == MAX_UM) && (modify == 0)) {cout<<"masz juz maxa ("<<MAX_UM<<")\n";return;}
 int current;
 int ret = 0;
 if (modify == TRUE)
        {
         ShowSkill(post);
         ln();
         cout<<"numer: ";
         int num;
         takeint(&num, 15);
         if (num<0) return;
         if (num>post->il_um-1) return;
         current = num;
         int l = post->pozum[num];
         int t = post->trum[num];
         ret = db->cost[l+t-1];
        }
 else current = post->il_um;
 if (modify != 10) ShowSkill(db,0);
 else FindSkill(db);
 ln();
 cout<<"numer umiejetnosci lub -1 wlasna: ";
 int num;takeint(&num, 250);
 if (num<-1) num = 0;
 if (num>db->ilosc[3]-1) num = db->ilosc[3]-1;
 if (num != -1)strcpy(post->um[current],db->s_name[num]);
 else
        {
         cout<<"nazwa: ";
         takestring(post->um[current]);
        }
 cout<<"poziom "<<post->um[current]<<" trudnosc "<<db->s_mod[num][DIFF]<<": ";
 int poz;takeint(&poz, 5);
 int ipoz;
 int tr = 0;
 int ce = 0;
 int mod = 0;
 if (num != -1) ipoz = poz + db->s_mod[num][DIFF] -1;
 else
        {
         cout<<"trudnosc: ";
         takeint(&tr, 3);
         ipoz = poz + tr -1;
        }
 if (num == -1)
        {
         ShowAttChar(post);
         ln();
         cout<<"numer cechy testujacej: ";
         takeint(&ce, 16);
         if (ce<0) ce = 0;
         if (ce>post->ilosc_cech-1) ce = post->ilosc_cech-1;
        }
 int cost = db->cost[ipoz] - ret;
 if (*lum<cost) {cout<<"za malo punktow, masz tylko "<<*lum<<" skontaktuj sie z administratorem"<<endl;return;}
 *lum -= cost;
 post->pozum[current] = poz;
 if (num != -1) post->trum[current]  = db->s_mod[num][DIFF];else post->trum[current] = tr;
 if (num != -1) post->ceum[current]  = db->s_mod[num][ATTRIBUTE];else post->ceum[current] = ce;
 if (num != -1) post->mulum[current] = db->s_mod[num][MULTIPLE];
 else
        {
         cout<<"mnoznik: ";
         takeint(&post->mulum[current], 6);
         cout<<"modyfikator testu: ";
         takeint(&mod, 3, 1);
        }
 if (num != -1) post->magum[current] = db->s_mod[num][MAGIC];
 else
        {
         cout<<"poziom magii: ";
         takeint(&post->magum[current], 3);
        }
 if (num != -1) mod = db->s_mod[num][MOD];
 post->tesum[current] = (post->cecha[post->ceum[current]]/2) + (post->mulum[current]*post->pozum[current])+ mod;
 MiniShow(post,current);
 if (modify != TRUE) post->il_um++;
}


void DelSkill(RPG* post,DATA* db,int* lum, short inside = FALSE)
{
 fprintf(lg,"usuwa umiejetnosc\n");
 if (post->il_um == 0) {cout<<"nie ma nic do usuniecia\n";return;}
 int num,ret;
 if (inside == TRUE)
        {
         ShowSkill(post);
         ln();
         cout<<"numer: ";
         takeint(&num, 10);
         if (num<0) return;
         if (num>post->il_um-1) return;
         int l = post->pozum[num];
         int t = post->trum[num];
         ret = db->cost[l+t-1];
         post->il_um --;
         for (int i=num;i<post->il_um;i++)
                {
                 strcpy(post->um[i],post->um[i+1]);
                 post->tesum[i] = post->tesum[i+1];
                 post->pozum[i] = post->pozum[i+1];
                 post->trum[i]  =  post->trum[i+1];
                 post->ceum[i]  =  post->ceum[i+1];
                 post->mulum[i] = post->mulum[i+1];
                }
         *lum += ret;
        }
 if (inside == FALSE)
        {
         post->il_um --;
         int l = post->pozum[post->il_um];
         int t = post->trum[post->il_um];
         ret = db->cost[l+t-1];
         *lum += ret;
        }
 if  (inside == ALL)
        {
         int l;
         int t;
         ret   = 0;
         for (int i=0;i<post->il_um;i++)
                {
                 l = post->pozum[i];
                 t = post->trum[i];
                 ret += db->cost[l+t-1];
                }
         *lum += ret;
         post->il_um = 0;
        }
}
void EditDBSkill(DATA* db)
{
 fprintf(lg,"probuje edytowac umiejetnosci w bazie\n");
 bool access = Check(db);
 if (!access) return;
 SetSkill(db);
 cout<<"zapisz w lub null: ";
 char dane[STRING];
 char name[STRING];
 strcpy(name,"DataBase/");
 takestring(dane);
 if (strcmp(dane,"null") == 0) return;
 strcat(name,dane);
 FILE* plik;
 if ((plik = fopen(name,"w+")) == NULL)
 	{
    cout<<"nie moge pisac do "<<name<<" pretensje nalezy kierowac do administratora"<<endl;
    fprintf(lg,"-- error -- nie da sie zapisac w %s\n", name);
    int nc = ncont(); if (nc)
    return;
   }
 Save(db,plik);
 fclose(plik);
}

void SeeLevCost(DATA* db)
{
 fprintf(lg,"koszty poziomow\n");
 cout<<"ilosc poziomow "<<db->ilosc[5]<<endl;
 int skip = 0;
 for (int i=0;i<db->ilosc[5];i++)
        {
         printf("poziom: %-3d\t\tkoszt: %-4d\n",i,db->cost[i]);
         clear(i, CLEAR, skip);
        }
}
void SPEdit(int* lum)
{
 fprintf(lg,"\n\n\n   -- CHEATER -- edytuje SP!!!  \t !!! \t !!!\n\n\n\n");
 cout<<"CHEATER\n";
 cout<<"podaj supertajny kod: ";
 int sccode = 0xdead;
 int taken;
 secretake(taken);
 if (taken != sccode) return;
 cout<<"dodaj puntow: ";
 int lb;
 takeint(&lb, 5);
 *lum = *lum + lb;
}

bool contain(char* strU, char* strinU)
{
 char str[STRING];
 char strin[STRING];
 strcpy(str, strU);
 strcpy(strin, strinU);
 downcase(str);
 downcase(strin);
 if (strstr(str, strin)) return true;
 else return false;
}

void SkillOperator(RPG* post, DATA* db, int* lum, FILE* plik, char* fn)
{
 fprintf(lg,"opcje wyboru umiejetnosci\n");
 char zn;
 int v=  0;
 cout<<"\nOpcja (0-lista): ";
 takechar(&zn);
 switch(zn)
        {
         case '0': OptSkill(post,*lum); break;
         case 'a': AddSkill(post,db,lum);break;
         case 'v': AddSkill(post,db,lum,10);break;
         case 'b': AddSkill(post,db,lum,TRUE);break;
         case 'c': ShowSkill(db,3);break;
         case 't': ShowSkill(db,2);break;
         case 'd': ShowSkill(post);break;
         case 'e': DelSkill(post,db,lum);break;
         case 'f': DelSkill(post,db,lum,TRUE);break;
         case 'g': DelSkill(post,db,lum,ALL);break;
         case 'x': SPEdit(lum);break;
         case 's': SaveChar(post,plik,fn);break;
         case 'h': SeeLevCost(db);break;
         case 'k': *lum = 0;break;
         case 'q': EditDBSkill(db);break;
         case 'z': exit( -1 );break;
        }
}

void SetSkill(RPG* post,DATA* db, FILE* plik, char* fn)
{
 fprintf(lg,"ustala umiejetnosci\n");
 int lum = 40;
 while (lum>0)
        {
         SkillOperator(post,db,&lum,plik,fn);
        }
}
void RealMM(RPG* post)
{
 fprintf(lg,"wylicza MM\n");
 int MM = post->numMM;
 post->cecha[MM] = post->stMM;
 for (int i=0;i<post->il_um;i++)
        {
         post->cecha[MM]+= post->magum[i]*(post->pozum[i]-1);
        }
  if (post->cecha[MM] < 0) post->cecha[MM] = 0;
 if (post->cecha[MM] < 0) post->cecha[MM] = 0;
}
void OptEkw(RPG* post, int cash)
{
 fprintf(lg,"opcje ekwipunku\n");
 cout<<"masz "<<post->il_ekw<<" przedmiotow, max to "<<MAX_EKW_P<<" twoja kasa: $"<<cash<<endl;
 cout<<"a\t-wez\n";
 cout<<"f\t-znajdz i wez\n";
 cout<<"c\t-zobacz\n";
 cout<<"e\t-edytuj baze\n";
 cout<<"g\t-edytuj swoj ekwipunek\n";
 cout<<"r\t-reczne branie ekwipunku\n";
 cout<<"k\t-to by bylo na tyle.\n";
}
void OptEdit()
{
 fprintf(lg,"edycja ekwipunku\n");
 cout<<"a\t-dodaj ekwipunek recznie\n";
 cout<<"s\t-dodaj ekwipunek z bazy danych\n";
 cout<<"b\t-usun\n";
 cout<<"c\t-wyczysc rekord\n";
 cout<<"d\t-zobacz\n";
 cout<<"k\t-koniec\n";
 cout<<"z\t-koniec programu\n";
 cout<<"e\t-edytuj ilosc kasy na starcie\n";
}
void EditEkw(RPG*, DATA*);

void ShowEkw(RPG* post)
{
 fprintf(lg,"pokazuje ekwipunek\n");
 cout<<"\nilosc "<<post->il_ekw<<endl;
 int skip = 0;
 for (int i=0;i<post->il_ekw;i++)
        {
         printf("(%-2d)%-60s\n",i,post->ekw[i]);
         clear(i, CLEAR, skip);
        }
 ln();
}
void TakeItem(RPG* post, DATA* db, int& cash, bool find = FALSE)
{
 fprintf(lg,"bierze przedmiot\n");
 if (post->il_ekw >= MAX_EKW_P)
 	{
    cout<<"masz juz maxa ("<<MAX_EKW_P<<") \n";
    return;
   }
 if (find) FindItem(db);
 else ShowItem(db);
 cout<<"podaj numer: ";
 int num;
 takeint(&num, 25);
 if ((num < 0) || (num > db->ilosc_ekw-1)) return;
 if (db->c_ekw[num] > cash) {cout<<"nie stac cie\npozycz od administratora";return;}
 strcpy(post->ekw[post->il_ekw],db->ekw[num]);
 cash -= db->c_ekw[num];
 post->il_ekw++;
 cout<<post->il_ekw<<endl;
}
void EditDBItems(DATA* db)
{
 fprintf(lg,"ma zamiar edytowac przedmioty w bazie\n");
 bool access = Check(db);
 if (!access) return;
 SetItem(db);
 cout<<"zapisz w lub null: ";
 char dane[STRING];
 char name[STRING];
 strcpy(name,"DataBase/");
 takestring(dane);
 if (strcmp(dane,"null") == 0) return;
 strcat(name,dane);
 FILE* plik;
 if ((plik = fopen(name,"w+")) == NULL)
 	{
    cout<<"nie da sie zapisac w "<<name<<" trzeba posepic u administratora"<<endl;
    int nc = ncont(); if (nc)
    return;
   }
 Save(db,plik);
 fclose(plik);
}
void DelEkw(RPG* post)
{
 fprintf(lg,"usuwa ekwipunek\n");
 if (post->il_ekw == 0) {cout<<"nie ma nic do usuniecia\n";return;}
 ShowEkw(post);
 ln();
 cout<<"podaj numer: ";
 int num;takeint(&num, 25);
 if ((num<0) || (num>post->il_ekw-1)) return;
 post->il_ekw--;
 for (int i=num;i<post->il_ekw;i++)
        {
         strcpy(post->ekw[i],post->ekw[i+1]);
        }
}
void AddEkw(RPG* post)
{
 fprintf(lg,"dodaje ekwipunek\n");
 if (post->il_ekw == MAX_EKW_P) {cout<<"bedzie ci za ciezko\n";return;}
 cout<<"nazwa "<<post->il_ekw<<" przedmiotu: ";
 takestring(post->ekw[post->il_ekw]);
 post->il_ekw ++;
}

void EditCash(RPG* post)
{
 cout<<"masz $"<<post->cash<<" nowa ilosc to: $";
 takeint(&post->cash, 1000);
}

void TakeFromDB(RPG* post, DATA* db, int& ret_mode, int& cash)
{
 ln();
 fprintf(lg,"bierze z bazy\n");
 char zn = '%';
 while (true)
 	{
 cout<<"\nOptcja (0-lista): ";
 ret_mode = post->il_ekw;
 takechar(&zn);
 switch(zn)
 	{
     case '0' : OptEkw(post, cash); break;
    case 'a': TakeItem(post,db,cash);break;
    case 'f': TakeItem(post,db,cash,TRUE);break;
    case 'c': ShowEkw(post); break;
    case 'e': EditDBItems(db);break;
    case 'g': EditEkw(post, db);
    case 'r': return;
    case 'k': ret_mode = MAX_EKW_P; return;
   }
  }
}
void EditEkw(RPG* post, DATA* db)
{
 fprintf(lg,"edytuje ekwipunek\n");
 ShowEkw(post);
 char zn ='l';
 int  tmp = 0;
 while (zn != 'k')
 {
 cout<<"Opcja (0-lista): ";
 takechar(&zn);
 switch(zn)
        {
         case '0':OptEdit(); break;
         case 'b': DelEkw(post);break;
         case 'a': AddEkw(post);break;
         case 's': TakeFromDB(post,db,tmp,post->cash);break;
         case 'c': post->il_ekw = 0;break;
         case 'd': ShowEkw(post);break;
         case 'e': EditCash(post);break;
         case 'z': exit( -1 );break;
        }
 }
}
void SetEkw(RPG* post, DATA* db)
{
 fprintf(lg,"ustala ekwipunek\n");
 cout<<"max ekwipunku to: "<<MAX_EKW_P<<endl;
 cout<<"Misiu podaj ilosc kasy (domyslnie "<<db->def_cash<<"): $";
 int cash;
 takeint(&cash, 1000);
 cout<<"b\t-wez z bazy danych\n";
 cout<<"k\t-zakoncz\n";
 char data[STRING];
 strcpy(data,"x");
 int i = 0;
 while ((strcmp(data,"k") != 0) && (post->il_ekw < MAX_EKW_P) && (i < MAX_EKW_P))
        {
         printf("(%-3d) ",i);
         takestring(data, 6, "k");
         if (strcmp(data,"b") == 0)
                {
                 TakeFromDB(post,db,i,cash);
                }
         else if (strcmp(data,"k") != 0)
                {
                 strcpy(post->ekw[i],data);
                 i++;
                 post->il_ekw++;
                }
        }
 post->cash = cash;
 cout<<"Zostalo:\t$"<<post->cash<<endl;
}
int Compute(RPG* post, int& max_spell)
{
 fprintf(lg,"oblicza PM\n");
 char tmp[STRING];
 int lev_rc = 0;
 int lev_me = 0;
 int lev_cer= 0;
 int op_mag = 0;
 for (int i=0;i<post->il_um;i++)
 	{
    strcpy(tmp, post->um[i]);
    downcase(tmp);
    if (!strcmp(tmp,"rzucanie czarow"))    lev_rc = post->pozum[i];
    if (!strcmp(tmp,"magia elementarna"))  lev_me = post->pozum[i];
    if (!strcmp(tmp,"ceremonie magiczne")) lev_cer = post->pozum[i];
    if (!strcmp(tmp,"operacje magiczne"))  op_mag = post->pozum[i];
   }
 ln();
 post->sp = post->cecha[post->numMM];
 if (op_mag == 0) post->spt = post->sp / 4;
 if (op_mag == 1) post->spt = post->sp / 3;
 if (op_mag == 2) post->spt = post->sp / 2;
 if (op_mag == 3) post->spt = (post->sp*2) / 3;
 if (op_mag == 4) post->spt = (post->sp*3) / 4;
 if (op_mag == 5) post->spt = post->sp;
 if (op_mag == 6) post->spt = (post->sp*5) / 4;
 if (op_mag == 7) post->spt = (post->sp*4) / 3;
 if (op_mag == 8) post->spt = (post->sp*3) / 2;
 if (op_mag >= 9) post->spt = post->sp* 2;
 int max = (lev_rc > lev_me) ? lev_me : lev_rc;
 int t_max = max;
 if (lev_rc)
 {
 if (lev_cer == max) max += 2;
 else if (lev_cer == max-1) max++;
 }
 post->max_lev = max;
 max_spell = max*3;
 cout<<"policzylem:\n";
 cout<<"\tmoc: "<<post->sp<<endl;
 cout<<"\tmoc / ture: "<<post->spt<<endl;
 cout<<"\tmax poziom zaklecia: "<<post->max_lev<<endl;
 max_spell *= 2;
 cout<<"\tmax ilosc zaklec: "<<max_spell<<endl;
 return t_max;
}

void AddSpell(DATA* db, RPG* post,int& max_spell, int rc, int see_denied = 0)
{
 fprintf(lg,"dodaje czar\n");
 if (post->il_spell == MAX_SP)
 	{
    cout<<"wiecej nie mozesz pojac\n";
    return;
   }
 int mlev = (db->ilosc_poziomow > post->max_lev) ? post->max_lev : db->ilosc_poziomow-1;
 for (int i=0;i<=mlev;i++)
 	{
    cout<<"czary poziomu "<<i<<endl;
   }
 cout<<"podaj poziom (-1 wlasne zaklecie): ";
 int slev;
 bool v_table[MAX_SPELL];
 for (int i=0;i<MAX_SPELL;i++) v_table[i] = 0;
 takeint(&slev, 4);
 if (slev == -1)
        {
         cout<<"poziom: ";
         int tmp;
         takeint(&tmp, 10);
         if ((tmp <0) || (tmp > mlev)) return;
         if (slev > max_spell)
 	{
              cout<<"musisz sie jeszcze wiele nauczyc\nnp. od administratora";
              return;
              }
           post->spell_level[post->il_spell] = tmp;
          cout<<"nazwal: ";
         takestring(post->spell[post->il_spell]);
         cout<<"koszt MM: ";
         takeint(&tmp, 20);
         if (tmp > post->spt)
                {
                 cout<<"bys sie spalil probujac rzucic to zaklecie\n";
                 return;
                }
         post->spell_cost[post->il_spell] = tmp;
         if (post->spell_level[post->il_spell]) max_spell -= post->spell_level[post->il_spell];
         else max_spell--;
         post->il_spell++;
         return;
        }
 if ((slev < 0) || (slev > mlev)) return;
 if (slev > max_spell)
 	{
    cout<<"poucz sie troche i sprobuj jeszcze raz.\n";
    return;
   }
 int plus = post->max_lev - rc;
 int disp = 0;
 int skip = 0;
 for (int i=0;i<db->ilosc_czarow[slev];i++)
 	{
    int have_mag = 0;
    for (int j=0;j<post->il_um;j++)
    	{
       if ((!strcmp(post->um[j],db->req_magic[slev][i])) && ((post->pozum[j] + plus) >= slev)) have_mag = 1;
      }
    if (db->spell_cost[slev][i] > post->spt) have_mag = 0;
    if (see_denied)
    	{
       printf("(%-3d)%-25s SP:%-3d  ",i,db->zaklecie[slev][i],db->spell_cost[slev][i]);
       if (!have_mag) cout<<"  [niedostepne]\n";
       else cout<<endl;
       disp++;
      }
    else if (have_mag) {printf("(%-3d)%-25s SP:%-3d\n",i,db->zaklecie[slev][i],db->spell_cost[slev][i]);disp++;}
    v_table[i] = have_mag;
    clear(disp, CLEAR, skip);
   }
 cout<<"numer zaklecia lub -1 anuluj: ";
 int num;
 takeint(&num, 25);
 if ((num < 0) || (num > db->ilosc_czarow[slev]-1)) return;
 if (!v_table[num]) {cout<<"nie da rady\n";return;}
 strcpy(post->spell[post->il_spell],db->zaklecie[slev][num]);
 post->spell_cost[post->il_spell] = db->spell_cost[slev][num];
 post->spell_level[post->il_spell]= slev;
 post->il_spell++;
 if (slev) max_spell -= slev;
 else max_spell--;
}

void ShowSpell(RPG* post)
{
 fprintf(lg,"pokazuje zaklecia\n");
 lns(2);
 int skip = 0;
 cout<<"ilosc zaklec "<<post->il_spell<<endl<<endl;
 for (int i=0;i<post->il_spell;i++)
 	{
    printf("(%-3d)%-25s MM:%3d poziom %d\n",i,post->spell[i], post->spell_cost[i], post->spell_level[i]);
    clear(i, CLEAR, skip);
   }
 lns(2);
}

void RemoveSpell(RPG* post, int& max_spell)
{
 fprintf(lg,"usuwa zaklecie\n");
 if (post->il_spell == 0) {cout<<"nie ma nic do usuniecia\n";return;}
 ShowSpell(post);
 ln();
 cout<<"numer: ";
 int num;takeint(&num, 25);
 if ((num<0) || (num>post->il_spell-1)) return;
 if (post->spell_level[num]) max_spell += post->spell_level[num];
 else max_spell++;
 post->il_spell--;
 for (int i=num;i<post->il_spell;i++)
        {
         strcpy(post->spell[i],post->spell[i+1]);
         post->spell_cost[i] = post->spell_cost[i+1];
         post->spell_level[i] = post->spell_level[i+1];
        }
}

void EditDBSpell(DATA* db)
{
 fprintf(lg,"probuje edytowac czary w DB\n");
 bool access = Check(db);
 if (!access) return;
 SetSpells(db);
 cout<<"zapisz w lub null: ";
 char dane[STRING];
 char name[STRING];
 strcpy(name,"DataBase/");
 takestring(dane);
 if (strcmp(dane,"null") == 0) return;
 strcat(name,dane);
 FILE* plik;
 if ((plik = fopen(name,"w+")) == NULL)
 	{
    cout<<"nie moge zapisac w "<<name<<" obudz administratora"<<endl;
    return;
   }
 Save(db,plik);
 fclose(plik);
}

void OptPSpell(int max_spell)
{
 fprintf(lg,"opcje czarow\n");
    cout<<"pozostalo czarow "<<max_spell<<endl<<endl;
    cout<<"a\t-dodaj zaklecie, zobacz wszystkie\n";
    cout<<"b\t-dodaj zklecie, zobacz dostepne\n";
    cout<<"c\t-usun zaklecie\n";
    cout<<"d\t-edytuj baze\n";
    cout<<"e\t-zobacz znane ci juz zaklecia\n";
    cout<<"k\t-zakoncz to menu\n";
}
void HandleSpells(DATA* db, RPG* post, int& pd)
{
 fprintf(lg,"uchwyt czarow\n");
 int max_spell;
 int rc = Compute(post, max_spell);
 if (post->max_lev == 0)
 	{
    cout<<"nie jestes czarodziejem.\nnie bedziesz wybieral czarow\nzglos to do administratora\n\n\n";
    return;
   }
 if (pd) max_spell = pd;
 cout<<"max czarow: "<<MAX_SP<<" mozesz sie nauczyc "<<max_spell<<endl;
 char zn = '%';
 while ((zn != 'k') && (max_spell > 0))
 	{
    pd = max_spell;
    cout<<"Opcja (0-lista): ";
    takechar(&zn);
    switch(zn)
    	{
       case '0': OptPSpell(max_spell); break;
       case 'a': AddSpell(db, post, max_spell, rc, 1);break;
       case 'b': AddSpell(db, post, max_spell, rc);break;
       case 'c': RemoveSpell(post, max_spell);break;
       case 'd': EditDBSpell(db);break;
       case 'e': ShowSpell(post);break;
       case 'k': break;
      }
   }
  if (post->il_spell) post->mage = 1;
}

void SetSpell(RPG* post, DATA* db)
{
 fprintf(lg,"ustala czary\n");
 int v_ref = 0;
 downcase(db->dbname);
 HandleSpells(db, post, v_ref);
}
void Alternative(RPG* p, FILE* f)
{
 fprintf(lg,"zapisuje jadro html\n");
 fprintf(f,"<center>\n");
 fprintf(f,"Character level: %d EXP points: %d</center><br>\n",p->lev,p->lev_p);
 fprintf(f,"<center>\n");
 fprintf(f,"<table border = \"3\">\n");
 fprintf(f,"<tr>\n");
 for (int i=0;i<10;i++) DecompressData(p->dane[i]);
 fprintf(f,"<td>Name: %s\n",p->dane[IMIE]);
 fprintf(f,"<td>Surname: %s\n",p->dane[NAZWISKO]);
 fprintf(f,"<td>Race: %s\n",p->dane[RASA]);
 fprintf(f,"<td>Profession: %s\n",p->dane[PROFESJA]);
 fprintf(f,"<tr>\n");
 fprintf(f,"<td>Height: %s\n",p->dane[WZROST]);
 fprintf(f,"<td>Weight: %s\n",p->dane[WAGA]);
 fprintf(f,"<td>Hair: %s\n",p->dane[WLOSY]);
 fprintf(f,"<td>Eyes: %s\n",p->dane[OCZY]);
 fprintf(f,"</table>\n");
 fprintf(f,"<table border = \"3\">\n");
 fprintf(f,"<tr>\n");
 fprintf(f,"<td>Character: %s\n",p->dane[CHARAKTER]);
 fprintf(f,"<td>Skin: %s\n",p->dane[CERA]);
 fprintf(f,"</table>\n");
 int max = (p->ilosc_cech > p->il_um) ? p->ilosc_cech : p->il_um;
 fprintf(f,"<table border =\"3\">\n");
 fprintf(f,"<tr>\n");
 fprintf(f,"<td>Pl\n");
 fprintf(f,"<td>Attrib\n");
 fprintf(f,"<td>value\n");
 fprintf(f,"<td>Pl\n");
 fprintf(f,"<td>Diff\n");
 fprintf(f,"<td>Skill\n");
 fprintf(f,"<td>Level\n");
 fprintf(f,"<td>Test\n");
 for (int i=0;i<max;i++)
        {
         fprintf(f,"<tr>\n");
         if (i<p->ilosc_cech)
                {
                 DecompressData(p->n_cechy[i]);
                 fprintf(f,"<td>[%d]\n",p->p_c[i]);
                 fprintf(f,"<td>%s\n",p->n_cechy[i]);
                 fprintf(f,"<td>%d\n",p->cecha[i]);
                }
         else
                {
                 fprintf(f,"<td>-\n");
                 fprintf(f,"<td>-\n");
                 fprintf(f,"<td>-\n");
                }
         if (i<p->il_um)
                {
                 DecompressData(p->um[i]);
                 fprintf(f,"<td>[%d]\n",p->p_u[i]);
                 fprintf(f,"<td>(%d)\n",p->trum[i]);
                 fprintf(f,"<td>%s\n",p->um[i]);
                 fprintf(f,"<td>%d\n",p->pozum[i]);
                 fprintf(f,"<td>%d\n",p->tesum[i]);

                }
         else
                {
                 fprintf(f,"<td>-\n");
                 fprintf(f,"<td>-\n");
                 fprintf(f,"<td>-\n");
                 fprintf(f,"<td>-\n");
                 fprintf(f,"<td>-\n");
                }
        }
 fprintf(f,"</table>\n");
 fprintf(f,"</center>\n");

 if (p->mage)
   {
    fprintf(f,"<center>Known Spells SpellPts / turn %d</center>\n",p->spt);
    fprintf(f,"<center><br>\n<table border =\"3\">\n");
    fprintf(f,"<tr>\n");
    fprintf(f,"<td>Spell");
    fprintf(f,"<td>Level");
    fprintf(f,"<td>Spell Cost");
    for (int i=0;i<p->il_spell;i++)
    	{
       fprintf(f,"<tr>\n");
       fprintf(f,"<td>%s", p->spell[i]);
       fprintf(f,"<td>%d", p->spell_level[i]);
       fprintf(f,"<td>%d", p->spell_cost[i]);
      }
    fprintf(f,"</table>\n");
    fprintf(f,"</center>\n");
   }
 fprintf(f,"<center><big><u>Equip:</u></big></big></center>\n");
 fprintf(f,"<pre>\n");
 if (p->il_ekw == 0) fprintf(f,"No equip\n");
 for (int i=0;i<p->il_ekw;i++)
        {
         DecompressData(p->ekw[i]);
         fprintf(f,"%s\n",p->ekw[i]);
        }
 fprintf(f,"\n From v4.1 Cash = %d\n",p->cash);
 fprintf(f,"</pre>\n");
}
void WriteHTML(RPG* post, FILE* plik,char* fn)
{
 fprintf(lg,"zapisuje postac w formacie HTML\n");
 fprintf(plik,"<html>\n");
 fprintf(plik,"<title>\n");
 fprintf(plik,"Charater Card %s %s  XP %d Gender: %s Age: %s\n",post->dane[IMIE],post->dane[NAZWISKO],post->PD, post->dane[PLEC],post->dane[WIEK]);
 fprintf(plik,"</title>\n");
 fprintf(plik,"<head>\n");
 fprintf(plik,"</head>\n");
 fprintf(plik,"<body>\n");
 fprintf(plik,"<center><big>Character %s %s  XP %d Gender: %s Age: %s</big></center>\n",post->dane[IMIE],post->dane[NAZWISKO],post->PD, post->dane[PLEC],post->dane[WIEK]);
 fprintf(plik,"</body>\n");
 Alternative(post,plik);
 fprintf(plik,"<pre>\n\nUsing Data Base %s</pre>\n",fn);
 fprintf(plik,"</html>\n");
}
void AboutHTML(RPG* post, FILE* plik, char* fn,char* db)
{
 char fn2[80];
 strcpy(fn2,fn);
 strcat(fn2,".html");
 cout<<"zapisze html w "<<fn2<<" OK? TAK.\n";
 cout<<"wiec zapisuje.\n";
 if ((plik = fopen(fn2,"w+"))==NULL)
        {
         cout<<"nastapil wysoce nieoczekiwany blad, nie da sie zapisac w "<<fn2<<endl;
         cout<<"szybko wyslij maila do administratora!\n";
         fprintf(lg,"--error -- nie da sie zapisac pliku HTML\n");
        }
 else
        {
         WriteHTML(post,plik,db);
         fclose(plik);
        }
}
void Statistics(RPG* post,char* db2)
{
 fprintf(lg,"oblicza statystyki postaci\n");
 char db[STRING];
 strcpy(db, db2);
 downcase(db);
 int for_skill = 10;
 int for_exp_skill = 4;
 int for_attribute = 10;
 int attribute_minus = 10;
 int for_exp_attribute = 4;
 int next_level = 1000;
 if (strcmp(db,"database/galthar") == 0)
        {
         for_skill = 9;
         for_exp_skill = 4;
         for_attribute = 2;
         attribute_minus = 70;
         for_exp_attribute = 5;
         next_level = 1200;
        }
 if (strcmp(db,"database/saradok") == 0)
        {
         for_skill = 10;
         for_exp_skill = 4;
         for_attribute = 10;
         attribute_minus = 11;
         for_exp_attribute = 4;
         next_level = 1000;
        }
 if (strcmp(db,"database/base1940") == 0)
        {
         for_skill = 10;
         for_exp_skill = 4;
         for_attribute = 15;
         attribute_minus = 5;
         for_exp_attribute = 5;
         next_level = 1000;
        }
 post->lev = 0;
 post->lev_p = 0;
 for (int i=0;i<post->il_um;i++)
 	{
    post->lev_p += for_skill*post->trum[i]*post->pozum[i]*post->pozum[i];
    post->lev_p += for_exp_skill*post->p_u[i];
   }
 for (int i=0;i<post->ilosc_cech;i++)
 	{
    post->lev_p += for_attribute*(post->cecha[i] - attribute_minus);
    post->lev_p += for_exp_attribute*post->p_c[i];
   }
 post->lev = (post->lev_p / next_level)+1;
 cout<<"poziom postaci "<<post->lev<<" punkty: "<<post->lev_p<<endl;
}
void Draw()
{
 fprintf(lg,"rysuje ramki\n");
 cout<<" *";
 for (int i=0;i<72;i++) cout<<"-";
 cout<<"*\n";
}


void ShowAll(RPG* post, char* db_n)
{
 fprintf(lg,"pokazuje cala karte postaci...\n");
 RealMM(post);
 int skip = 0;
 Statistics(post,db_n);
 lns(2);
 Draw();
 printf("\n\tPostac        %s %s  PD %d PLEC %s lat %s\n",post->dane[IMIE],post->dane[NAZWISKO],post->PD,post->dane[PLEC],post->dane[WIEK]);
 Draw();
 printf(" |Imie:     \t%-18s |\tNazwisko:\t%-18s|\n",post->dane[IMIE],post->dane[NAZWISKO]);
 printf(" |Rasa:     \t%-18s |\tProf    :\t%-18s|\n",post->dane[RASA],post->dane[PROFESJA]);
 printf(" |Oczy:     \t%-18s |\tWlosy:   \t%-18s|\n",post->dane[OCZY],post->dane[WLOSY]);
 printf(" |Wzrost:   \t%-18s |\tWaga:    \t%-18s|\n",post->dane[WZROST],post->dane[WAGA]);
 printf(" |Charakter:\t%-25s  \t         \t%-5s     |\n",post->dane[CHARAKTER],"");
 Draw();
 int max = (post->ilosc_cech > post->il_um) ? post->ilosc_cech : post->il_um;
 for (int i=0;i<max;i++)
        {
         if (i<post->ilosc_cech)
         printf(" | [%-2d]%-18s  %-3d",post->p_c[i],post->n_cechy[i],post->cecha[i]);
         else
         printf(" |%-2s %-18s  %-3s  ","","","");
         if (i<post->il_um)
         printf("|  [%-2d](%-2d)%-18s %-2d %-2d\t  |",post->p_u[i],post->trum[i],post->um[i],post->pozum[i],post->tesum[i]);
         else
         printf("|    %-2s %-2s %-18s %-2s %-2s\t  |","","","","","");
         ln();
	 clear(i, CLEAR, skip);
        }
 Draw();
 skip = 0;
 if (post->mage)
 	{
    printf(" |                           Znane Zaklecia                               |\n");
    printf(" |        Czarow: %-3d     Max poz.  %-2d      MM / ture   %-3d               |\n", post->il_spell, post->max_lev, post->spt);
    for (int i=0;i<post->il_spell;i++)
    	{
       printf(" |\t%-30s     Poz.  %-2d     MM koszt   %-4d   |\n", post->spell[i], post->spell_level[i], post->spell_cost[i]);
       clear(i, CLEAR, skip);
      }
   }
 Draw();
 skip = 0;
 for (int i=0;i<post->il_ekw;i++)
        {
         printf(" | %-70s |\n",post->ekw[i]);
	 clear(i, CLEAR, skip);
        }
 Draw();
 lns(2);
}

void CreateChar(DATA* db, RPG* post, FILE* plik, char* fn,char* db_n)
{
 fprintf(lg,"Proces Tworzenia Postaci Rozpoczety:\n");
 DefaultValues(post,db);
 fclose(plik);
 MainData(post,db);
 post->rl = 1;
 SaveChar(post,plik,fn);
 ChooseRace(post,db);
 post->rl = 2;
 SaveChar(post,plik,fn);
 SaradokMM(post,db_n);
 ChooseProf(post,db);
 post->rl = 3;
 SaveChar(post,plik,fn);
 RandomAtt(post,db);
 post->rl = 4;
 SaveChar(post,plik,fn);
 OwnAtt(post);
 post->rl = 5;
 SaveChar(post,plik,fn);
 SetSkill(post,db,plik,fn);
 post->rl = 6;
 post->stMM = post->cecha[post->numMM];
 SaveChar(post,plik,fn);
 RealMM(post);
 SetEkw(post,db);
 AboutMM0(post,db_n);
 post->rl = 7;
 SaveChar(post,plik,fn);
 SetSpell(post,db);
 SaveChar(post,plik,fn);
 AboutHTML(post,plik,fn,db_n);
 ProtectorChar(post);
 post->rl = 0;
 SaveChar(post,plik,fn);
 ShowAll(post,db_n);
}
void AboutSave(RPG* post, FILE* plik, char* fn,char* db)
{
 cout<<"zapisac "<<fn<<": ";
 char zn;takechar(&zn, 2, 't', 'n');
 if (zn != 't') return;
 post->rl = 0;
 if ((plik=fopen(fn,"w+"))==NULL) cout<<"nie moge zapisac "<<fn<<" musisz pogadac z administratorem"<<endl;
 else SaveChar(post,plik,fn);
 AboutHTML(post,plik,fn,db);
}
void OptPD(int pd)
{
 fprintf(lg,"opcje PD-sow ilosc: %d\n", pd);
 cout<<"PD\t"<<pd<<endl<<endl;
 cout<<"a\t-dodaj do cechy\n";
 cout<<"b\t-dodaj do umiejetnosci\n";
 cout<<"c\t-zobacz karte postaci\n";
 cout<<"d\t-nowa umiejetnosc\n";
 cout<<"t\t-nowa umiejetnosc (zobacz kategorie)\n";
 cout<<"e\t-edytuj ekwipunek\n";
 cout<<"v\t-znajdz i wez nowa umiejetnosc\n";
 cout<<"0\t-znajdz i dodaj plusy do um. poziomu 0\n";
 cout<<"i\t-wyszukiwarka czasu rzeczywistego\n";
 cout<<"w\t-usun umiejetnosc\n";
 cout<<"g\t-zmien wiek postaci\n";
 cout<<"q\t-edytuj umiejetnosci w bazie danych\n";
 cout<<"s\t-edytuj zaklecia postaci\n";
 cout<<"u\t-zapisz postac\n";
 cout<<"k\t-zakoncz\n";
 cout<<"z\t-zakoncz program\n";
 cout<<"!\t-zabij postac\n";
}
void AddAtt(RPG* post, int* pd)
{
 fprintf(lg,"dodaje do cechy\n");
 ShowAttChar(post);
 ln();
 cout<<"numer : ";
 int num;takeint(&num, 16);
 if ((num<0) || (num>post->ilosc_cech-1)) return;
 cout<<"cecha "<<post->n_cechy[num]<<" ma aktualnie "<<post->p_c[num]<<" plusow\n";
 cout<<"aby zwiekszyc potrzeba 13\n";
 int left = 13 - post->p_c[num];
 int plusy;
 cout<<"ile plusow? ";
 takeint(&plusy, 6);
 if (plusy<1) return;
 if (plusy>*pd) return;
 if (plusy>left){cout<<"za duzo\n";return;}
 post->PD +=plusy;
 *pd = *pd - plusy;
 post->p_c[num] += plusy;
 fprintf(lg,"cecha numer %d dodano plusow %d\n", num, plusy);
 if (post->p_c[num] >= 13)
        {
         cout<<"cecha "<<post->n_cechy[num]<<" zwiekszona\n";
         post->p_c[num] = 0;
         post->cecha[num]++;
        }
}
void AddToSkill(RPG* post, DATA* db, int* pd)
{
 fprintf(lg,"dodaje do umiejetnosci\n");
 ShowSkill(post);
 ln();
 cout<<"numer: ";
 int num;takeint(&num, 25);
 if ((num<0) || (num>post->il_um-1)) return;
 int dat = post->trum[num] + post->pozum[num];
 int req = db->pd_cost[dat];
 int left = req - post->p_u[num];
 cout<<"potrzeba "<<req<<" plusow aby zwiekszyc poziom\naktualnie ma "<<post->p_u[num]<<" plusow\n";
 int plusy;
 cout<<"ile plusow? ";
 takeint(&plusy, 6);
 if (plusy<1) return;
 if (plusy>*pd) return;
 if (plusy>left) {cout<<"za duzo\n";return;}
 post->PD += plusy;
 *pd = *pd - plusy;
 post->p_u[num] += plusy;
 fprintf(lg,"um. numer %d dodano plusow %d\n", num, plusy);
 if (post->p_u[num] >= req)
        {
         cout<<"umiejetnosc "<<post->um[num]<<" zwieksza poziom\n";
         post->p_u[num] = 0;
         post->pozum[num]++;
         post->tesum[num] += post->mulum[num];
        }
}
void NewSkill(RPG* post, DATA* db, int* pd, int find = 0)
{
 fprintf(lg,"nowa umiejetnosc.\n");
 if (post->il_um == MAX_UM) {cout<<"masz juz maxa umiejetnosci  "<<MAX_UM<<endl;return;}
 if (find == 1) FindSkill(db,TRUE);
 else if (find == 3) FindSkill(db, TRUE);
 else if (find == 2) ShowSkill(db,2);
 else ShowSkill(db);
 ln();
 cout<<"podaj jakiej trudnosci ma byc nowa umiejetnosc: ";
 int trd;takeint(&trd, 3);
 int cost = db->pd_cost[trd];
 if (find == 3) cost = 0;
 if (*pd<cost) {cout<<"nie masz tyle plusow, potrzeba "<<cost<<" pogadaj z administratorem.\n";return;}
 cout<<"numer lub -1 wlasna: ";
 int num;takeint(&num, 250);
 if ((num<-1) || (num>db->ilosc[3]-1)) return;
 if (num != -1) if (db->s_mod[num][DIFF] != trd) {cout<<"ta umiejetnosc ma inna trudnosc niz "<<trd<<endl;return;}
 post->PD += cost;
 * pd = *pd - cost;
 int current = post->il_um;
 if (num != -1) strcpy(post->um[current],db->s_name[num]);
 else
        {
         cout<<"nazwa: ";
         takestring(post->um[current]);
        }
 post->pozum[current] = 1;
 if (find == 3) post->pozum[current] = 0;
 post->trum[current] = trd;
 if (num != -1)post->ceum[current] = db->s_mod[num][ATTRIBUTE];
 else
        {
         ShowAttChar(post);
         ln();
         int n;
         cout<<"cecha testujaca: ";
         takeint(&n, 16);
         if (n<0) n = 0;
         if (n>post->ilosc_cech-1) n = post->ilosc_cech-1;
         post->ceum[current] = n;
        }
 if (num != -1) post->mulum[current] = db->s_mod[num][MULTIPLE];
 else
        {
         cout<<"mnoznik: ";
         takeint(&post->mulum[current], 6);
        }
 if (num != -1) post->magum[current] = db->s_mod[num][MAGIC];
  else
        {
         cout<<"poziom magii: ";
         takeint(&post->magum[current], 4);
        }
 int mod = 0;
 if (num != -1)mod = db->s_mod[num][MOD];
 else
        {
         cout<<"modyfikator testu: ";
         takeint(&mod, 4, 1);
        }
 post->tesum[current] = post->cecha[post->ceum[current]]/2 + post->mulum[current]+ mod;
 post->il_um++;
}


void Jump(int* pd)
{
 fprintf(lg,"\n\n\n    -- CHEATER -- dodaje sobie PD-sow !!!\t!!!\t!!!\n\n\n\n");
 cout<<"[CHEATER!] nie tak latwo\npodaj supertajny kod: ";
 int scod = 0xdead;
 int user;
 secretake(user);
 if (user != scod) return;
 cout<<"Dodaj PD: ";
 int il;takeint(&il, 25);
 *pd = *pd + il;
}
void DelSkill(RPG* post)
{
 fprintf(lg,"usuwa posiadana juz umiejetnosc\n");
 if (post->il_um == 0) {cout<<"nie ma nic do usuniecia\n";return;}
 int num;
 ShowSkill(post);
 ln();
 cout<<"numer: ";
 takeint(&num, 25);
 if (num<0) return;
 if (num>post->il_um-1) return;
 post->il_um --;
 	for (int i=num;i<post->il_um;i++)
   	{
       strcpy(post->um[i],post->um[i+1]);
       post->tesum[i] = post->tesum[i+1];
       post->pozum[i] = post->pozum[i+1];
       post->trum[i]  =  post->trum[i+1];
       post->ceum[i]  =  post->ceum[i+1];
       post->mulum[i] = post->mulum[i+1];
      }
}

void AgeOf(RPG* post)
{
 fprintf(lg,"zmienia wiek\n");
 post->ilosc_danych = DANE_POSTACI;
 cout<<"aktualny wiek: "<<post->dane[WIEK]<<", podaj nowy: ";
 takestring(post->dane[WIEK]);
}
void CharSave(RPG* post)
{
 fprintf(lg,"zapis postaci\n");
 FILE* plik = NULL;
 char name[STRING];
 char dir[STRING];
 strcpy(dir,"Characters/");
 cout<<"zapisz w: ";
 takestring(name);
 strcat(dir, name);
 SaveChar(post, plik, dir);
}
void DealPD(RPG* post, DATA* db, int* pd)
{
 fprintf(lg,"rozdawanie PD-sow\n");
 cout<<"Opcja (o-lista): ";
 char zn;takechar(&zn);
 int v = 0;
 switch(zn)
        {
         case 'o': OptPD(*pd); break;
         case 'k': *pd = 0;break;
         case 'a': AddAtt(post,pd);break;
         case 'b': AddToSkill(post,db,pd);break;
         case 'c': ShowAll(post,db->dbname);break;
         case 'd': NewSkill(post,db,pd);break;
         case 't': NewSkill(post,db,pd,2);break;
         case 'e': EditEkw(post,db);break;
         case 'v': NewSkill(post,db,pd,1);break;
         case '0': NewSkill(post,db,pd,3);break;
         case 'x': Jump(pd);break;
         case 'w': DelSkill(post);break;
         case 'q': EditDBSkill(db);break;
         case 'g': AgeOf(post);break;
         case 'u': CharSave(post);break;
         case 's': HandleSpells(db, post, *pd);break;
         case '!': post->IsDead = 1;break;
         case 'z': exit( -1 );break;
        }
}
void AddPD(RPG* post, DATA* db)
{
 fprintf(lg,"rozdawanie doswiadczenia\n");
 cout<<"Misiu podaj ilosc PD dla "<<post->dane[IMIE]<<": ";
 int pd;
 takeint(&pd, 20);
 if (pd<0) pd *= -1;
 while (pd>0)
        {
         DealPD(post,db,&pd);
        }
 RealMM(post);
}
void Experience(DATA* db, RPG* post, FILE* plik, char* fn,char* db_n)
{
 fprintf(lg,"wczytywanie juz gotowej postaci do edycji\n");
 fclose(plik);
 RealMM(post);
 ShowAll(post,db_n);
 AddPD(post,db);
 AboutSave(post,plik,fn,db_n);
}

int AboutRestore(DATA* db, RPG* post, FILE* plik, char* fn, char* db_n)
{
 fprintf(lg,"sprawdzam czy dokonczono tworzenie postaci\n");
 if (post->rl == 0) return 0;
 cout<<"wykrylem ze niedokonczyles postaci poprzednio.\n";
 cout<<"zaczniesz od punktu odzyskiwania: "<<post->rl<<endl;
 fprintf(lg,"-- oops -- nie dokonczono skocze do kroku nr. %d\n", post->rl);
 switch(post->rl)
   {
    case 1: goto l1;
    case 2: goto l2;
    case 3: goto l3;
    case 4: goto l4;
    case 5: goto l5;
    case 6: goto l6;
    case 7: goto l7;
    default: cout<<"bledne dane w pliku postaci.\n";int nc = ncont(); if (nc)return 0;
   }
 DefaultValues(post,db);
 fclose(plik);
 MainData(post,db);
 post->rl = 1;
 SaveChar(post,plik,fn);
 l1:
 ChooseRace(post,db);
 post->rl = 2;
 SaveChar(post,plik,fn);
 l2:
 SaradokMM(post,db_n);
 ChooseProf(post,db);
 post->rl = 3;
 SaveChar(post,plik,fn);
 l3:
 RandomAtt(post,db);
 post->rl = 4;
 SaveChar(post,plik,fn);
 l4:
 OwnAtt(post);
 post->rl = 5;
 SaveChar(post,plik,fn);
 l5:
 SetSkill(post,db,plik,fn);
 post->rl = 6;
 post->stMM = post->cecha[post->numMM];
 SaveChar(post,plik,fn);
 l6:
 RealMM(post);
 SetEkw(post,db);
 AboutMM0(post,db_n);
 post->rl = 7;
 SaveChar(post,plik,fn);
 l7:
 SetSpell(post,db);
 SaveChar(post,plik,fn);
 AboutHTML(post,plik,fn,db_n);
 ProtectorChar(post);
 post->rl = 0;
 SaveChar(post,plik,fn);
 ShowAll(post,db_n);
 fprintf(lg,"skonczone odzyskiwanie.\n");
}

void RolePlayConstructor(DATA* db, RPG* post, char* fn,char* base_name)
{
 fprintf(lg,"Konstruowanie zasobow RPG\n");
 FILE* plik;
 int restor = 0;
 if ((plik = fopen(fn,"r+")) != NULL)
        {
         LoadChar(post,plik);
	 restor = AboutRestore(db, post, plik, fn, base_name);
	 if (restor) return;
         AboutMM0(post,base_name);
         CheckPassChar(post);
         Experience(db,post,plik,fn,base_name);
         return;
        }
 if ((plik = fopen(fn,"w+")) == NULL)
        {
         cout<<"blad: nie da sie zapisac w "<<fn<<" zadzwon do administratora lub serwisu komputerowego.\n";
         fprintf(lg,"-- error -- nie da sie zapisac w %s\n", fn);
         int nc = ncont(); if (nc)
         return;
        }
 CreateChar(db,post,plik,fn,base_name);
}
void Process(char* db, char* cn)
{
 fprintf(lg,"glowny proces programu:\n");
 char temp[STRING],temp2[STRING];
 strcpy(temp,"DataBase/");
 strcat(temp,db);
 strcpy(temp2,"Characters/");
 strcat(temp2,cn);
 DATA* base = new DATA;
 strcpy(base->dbname,temp);
 DataBase(temp,base);
 if (strcmp(cn,"null"))
 {
  char lanc[STRING];
  strcpy(lanc, temp);
  RPG* post = new RPG;
  strcpy(post->pname,temp2);
  RolePlayConstructor(base,post,temp2,temp);
  delete base;
  delete post;
  return;
 }
 else delete base;
}
void Help()
{
 lns(2);
 cout<<"Opcje:\n";
 cout<<"lin -h                         Wyswietla ta pomoc\n";
 cout<<"lin -s  20                    Ustawia przesuwanie ekranu na 20\n";
 cout<<"lin --source code      Wyswietla wlasny kod zrodlowy.\n";
 cout<<"lin [baza] [postac]       Tworzy sesje na podst. bazy i postaci\n";
 cout<<"lin --debug mode       Debuguje program.\n";
 lns(2);
}

void Src()
{
 FILE* in;
 if ((in = fopen("./lin.c","r+")) == NULL)
        {
         cout<<"Brak pliku kodu zrodlowego: lin.c\n";
         return;
        }
 int zn;
 ln();
 while ((zn = fgetc(in)) != EOF) cout<<(char)zn;
 fclose(in);
 ln();
}


void init_dbg()
{
 char* tmp = "/dev/null";
 if ((lg = fopen(tmp,"w+")) == NULL)
        {
         cout<<"Plik logow "<<tmp<<" nie moze byc otworzony do zapisu.\nProgram zatrzymany.\n";
         exit( -1 );
        }
}

void DebugMode()
{
 fclose(lg);
 char*tmp = "./debug.txt";
 if ((lg = fopen(tmp,"w+")) == NULL)
        {
         cout<<"Plik debug: "<<tmp<<" nie moze byc otworzony do zapisu.\n";
         init_dbg();
        }
 fprintf(lg,"   Program uruchomiony w trybie DEBUG\n");
 fprintf(lg,"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n\n");
}

void Special(char* opt, char* par)
{
 downcase(opt);
 cout<<opt<<", "<<par<<endl;
 if (!strcmp(opt,"--source") && (!strcmp(par,"code"))) {Src(); return;}
 if (!strcmp(opt,"--debug") && (!strcmp(par,"mode"))) DebugMode();
 if ((!strcmp(opt,"-s")) || (!strcmp(opt,"--scroll")))
    {
     int correct;
     correct = sscanf(par,"%d",&CLEAR);
     if (!correct) {cout<<"bledny format drugiego parametru.\nzadzwon do serwisu komputerowego\n"; int nc = ncont(); if (nc)exit( -1 );}
    }
 int given = 0;
 char based[STRING], plikp[STRING];
 if ((opt[0] != '-') && (par[0] != '-'))
        {
         strcpy(based, opt);
         strcpy(plikp, par);
         given = 1;
        }
 cout<<"Program RPG na Unix'a v5.0 OpenSource\n";
 if (!given)
        {
        DisplayBases();
        cout<<"baza: ";
        takestring(based, 3, "morgoth", "galthar", "saradok");
        DisplayCharacters();
        cout<<"postac: ";
        takestring(plikp, SECRET);
       }
 else cout<<"pobralem parametry z konsoli.\n";
 Process(based,plikp);
}

int main(int lb, char** par)
{
 start_t:
 cout<<"Program written by morgoth, open source on BSD license.\n";
 char based[STRING], plikp[STRING];
 init_random(C_TMP);
 init_dbg();
 if (lb ==1 )
 {
 cout<<"Baza danych: "<<(sizeof(DATA)/1024)<<" kb.\n";
 cout<<"Postac:      "<<(sizeof(RPG)/1024)<<" kb.\n";
 cout<<"Program RPG na Unix'a v5.0 OpenSource\n";
 cout<<"Wpisz z konsoli lin -h aby uzyskac pomoc\n";
 DisplayBases();
 cout<<"baza: ";
 takestring(based, 3, "morgoth", "galthar", "saradok");
 DisplayCharacters();
 cout<<"postac: ";
 takestring(plikp, SECRET);
 Process(based,plikp);
 }
 else if (lb == 2) Help();
 else Special(par[1], par[2]);
 kill_random();
 fprintf(lg,"random juz zamkniety, to powinien byc ostatni napis...");
 fclose(lg);
 cout<<"do zobaczenia\n";
 goto start_t;
 return 0;
}
