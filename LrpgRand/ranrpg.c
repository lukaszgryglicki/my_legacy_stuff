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

char base[STRING];
char race[STRING];
char prof[STRING];
int sp;
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
}
DATA::~DATA(){}
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
}
RPG::~RPG(){}
void usage()
{
 cout<<"usage:\n\n";
 cout<<"ranrpg\nranrpg -b base\nranrpg -r race\nranrpg -p profession\nranrpg -sp 40\nranrpg [combinations] fe. ranrpg -b morgoth -r czlowiek -p wojownik -sp 60\n";
 cout<<endl;
}

inline void ln(){cout<<endl;}
void lns(int par){for (int i=0;i<par;i++) ln();}
void downcase(char* str)
{
 for (int i=0;i<(int)strlen(str);i++) if ((str[i] >= 'A') && (str[i] <= 'Z')) str[i] += (char)0x20;
}


void DecompressData(char* data)
{
 for (int i=0;i<STRING;i++) if (data[i] == '_') data[i] = ' ';
}
void CompressData(char* data)
{
 for (int i=0;i<STRING;i++) if (data[i] == ' ') data[i] = '_';
}

void Uncompress(RPG* post)
{
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


void Protection(int check, DATA* db, FILE* plik)
{
 if (!check)
        {
         lns(6);
         cout<<"baza zostala blednie wczytana.\n";
         cout<<"kontynuacja moze ja uszkosdzic\n";
         cout<<"administrator cos o tym wie...\n";
         lns(4);
         exit( -1 );
        }
}
void Load(DATA* db,FILE* plik)
{
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

void DataBase(char* db, DATA* base)
{
 FILE* plik;
 if ((plik = fopen(db,"r+")) == NULL)
        {
         cout<<"blad, skontaktuj sie z administratorem: \nbrak pliku: "<<db<<endl;
         return;
        }
 Load(base,plik);
 base->lr++;
}
void Woman(RPG* post, DATA* db)
{
 strcpy(post->dane[PLEC],"kobieta");
 for (int i=0;i<post->ilosc_cech;i++) post->cecha[i] += db->p_mod[i];
}


void MainData(RPG* post, DATA* db)
{
 post->PD = 0;
 cout<<"imie: ";          strcpy(post->dane[IMIE],"randomized");
 cout<<"nazwisko: ";       strcpy(post->dane[NAZWISKO],"randomized");
 cout<<"charakter: ";     strcpy(post->dane[CHARAKTER],"neutralny");
 cout<<"kolor oczu: ";    strcpy(post->dane[OCZY],"randomized");
 cout<<"kolor wlosow: ";    strcpy(post->dane[WLOSY],"randomized");
 cout<<"kolor skory: ";          strcpy(post->dane[CERA],"normalna");
 cout<<"wzrost: ";        strcpy(post->dane[WZROST],"randomized");
 cout<<"waga: ";        strcpy(post->dane[WAGA],"randomized");
 cout<<"wiek: ";           strcpy(post->dane[WIEK],"randomized");
 int zn = random(2);
 if (zn) Woman(post,db);
 else strcpy(post->dane[PLEC],"mezczyzna");
}


void DefaultValues(RPG* post, DATA* db)
{
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

void AboutMM0(RPG* post, char* bname2)
{
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

void RealMM(RPG* post)
{
 int MM = post->numMM;
 post->cecha[MM] = post->stMM;
 for (int i=0;i<post->il_um;i++)
        {
         post->cecha[MM]+= post->magum[i]*(post->pozum[i]-1);
        }
  if (post->cecha[MM] < 0) post->cecha[MM] = 0;
 if (post->cecha[MM] < 0) post->cecha[MM] = 0;
}

void SaveChar(RPG* post, FILE* plik,char* fn)
{
 if ((plik = fopen(fn,"w+")) == NULL)
        {
         cout<<"nie moge zapisac w "<<fn<<endl;
         cout<<"wyslij FAX do administratora\n";
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
void Alternative(RPG* p, FILE* f)
{
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
        }
 else
        {
         WriteHTML(post,plik,db);
         fclose(plik);
        }
}

void Statistics(RPG* post,char* db2)
{
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
 cout<<" *";
 for (int i=0;i<72;i++) cout<<"-";
 cout<<"*\n";
}

void ShowAll(RPG* post, char* db_n)
{
 RealMM(post);
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
        }
 Draw();
 if (post->mage)
 	{
    printf(" |                           Znane Zaklecia                               |\n");
    printf(" |        Czarow: %-3d     Max poz.  %-2d      MM / ture   %-3d               |\n", post->il_spell, post->max_lev, post->spt);
    for (int i=0;i<post->il_spell;i++)
    	{
       printf(" |\t%-30s     Poz.  %-2d     MM koszt   %-4d   |\n", post->spell[i], post->spell_level[i], post->spell_cost[i]);
      }
   }
 Draw();
 for (int i=0;i<post->il_ekw;i++)
        {
         printf(" | %-70s |\n",post->ekw[i]);
        }
 Draw();
 lns(2);
}

void ChooseRace(RPG* post,DATA* db)
{
 strcpy(post->dane[RASA],".");
 strcpy(post->dane[PROFESJA],".");
 int num;
 if (!strcmp(race,"null"))
        {
         int ran = db->ilosc[1];
         num = random(ran);
        }
 else
        {
         downcase(race);
         char tmp[STRING];
         for (int i=0;i<db->ilosc[1];i++)
                {
                 strcpy(tmp,db->name[0][i]);
                 downcase(tmp);
                 if (!strcmp(tmp, race)) {num = i; goto nxt;}
                }
         cout<<"panic:\tno such race: "<<race<<endl;
         exit( -1 );
        }
 nxt:
 strcpy(post->dane[RASA],db->name[0][num]);
 for (int i=0;i<post->ilosc_cech;i++)
        {
         post->cecha[i] += db->mod[0][num][i];
        }
}

void ChooseProf(RPG* post,DATA* db)
{
 int num;
 if (!strcmp(prof,"null"))
        {
         int ran = db->ilosc[2];
         num = random(ran);
        }
 else
        {
         downcase(prof);
         char tmp[STRING];
         for (int i=0;i<db->ilosc[2];i++)
                {
                 strcpy(tmp,db->name[1][i]);
                 downcase(tmp);
                 if (!strcmp(tmp, prof)) {num = i; goto nxt;}
                }
         cout<<"panic:\tno such prof: "<<prof<<endl;
         exit( -1 );
        }
 nxt:
 strcpy(post->dane[PROFESJA],db->name[1][num]);
 for (int i=0;i<post->ilosc_cech;i++)
        {
         post->cecha[i] += db->mod[1][num][i];
        }
}


void SaradokMM(RPG* post, char* name2)
{
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
void RandomAtt(RPG* post,DATA* db)
{
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
 int suma = random(5);
 if ((suma<0) || (suma>5)) return;
 for (int i=0;i<post->ilosc_cech;i++)
        {
         post->cecha[i] = temp[i][suma];
        }
 ln();
}

void OwnAtt(RPG* post)
{
 int pl = random(4);
 if ((pl<1) || (pl>3)) return;
 int temp = pl;
 int num;
 int ile;
 int prev = -1;
 while (pl>0)
        {
         back:
         cout<<"numer cechy do zwiekszenia: ";
         num = random(post->ilosc_cech);
         if (num == prev) {cout<<"juz ja zmieniales\n";goto back;}
         if (num<0) num = 0;
         if (num>post->ilosc_cech-1) num = post->ilosc_cech-1;
         prev = num;
         cout<<"ile plusow do "<<post->n_cechy[num]<<": ";
         ile = random(2)+1;
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
         back2:
         cout<<"cecha do zmniejszenia: ";
         num = post->ilosc_cech;
         if (num == prev) {cout<<"nie da rady\nskontaktuj sie z administratorem\n";goto back2;}
         if (num<0) num = 0;
         if (num>post->ilosc_cech-1) num = post->ilosc_cech-1;
         prev = num;
         cout<<"ile minusow do "<<post->n_cechy[num]<<": ";
         ile = random(2)+1;
         if (ile<0) ile = 0;
         if (ile>2) ile = 2;
         if (ile>pl) ile = pl;
         post->cecha[num] -= ile*post->st;
         pl -= ile;
        }
}

void CreateChar(DATA* db, RPG* post, FILE* plik, char* fn,char* db_n)
{
 DefaultValues(post,db);
 fclose(plik);
 MainData(post,db);
 ChooseRace(post,db);
 SaradokMM(post,db_n);
 ChooseProf(post,db);
 RandomAtt(post,db);
 OwnAtt(post);
 /*SetSkill(post,db,plik,fn);*/
 post->stMM = post->cecha[post->numMM];
 RealMM(post);
 AboutMM0(post,db_n);
 AboutHTML(post,plik,fn,db_n);
 SaveChar(post,plik,fn);
 ShowAll(post,db_n);
}


void RolePlayConstructor(DATA* db, RPG* post, char* fn,char* base_name)
{
 FILE* plik;
 if ((plik = fopen(fn,"w+")) == NULL)
        {
         cout<<"blad: nie da sie zapisac w "<<fn<<" zadzwon do administratora lub serwisu komputerowego.\n";
         return;
        }
 CreateChar(db,post,plik,fn,base_name);
}

void startup()
{
 char temp[STRING],temp2[STRING];
 strcpy(temp,"DataBase/");
 strcat(temp,base);
 strcpy(temp2,"random.out");
 DATA* bs = new DATA;
 strcpy(bs->dbname,temp);
 DataBase(temp,bs);
 RPG* post = new RPG;
 RolePlayConstructor(bs,post,"random.out",temp);
 delete bs;
 delete post;
 return;
}

int main(int lb, char** par)
{
 usage();
 init_random(C_TMP);
 char spc[STRING];
 strcpy(base,"morgoth");
 strcpy(race,"null");
 strcpy(prof,"null");
 strcpy(spc,"40");
 for (int i=1;i<lb;i+=2)
        {
         if (!strcmp(par[i],"-b")) if (i+1<lb)strcpy(base,par[i+1]);
         if (!strcmp(par[i],"-r")) if (i+1<lb)strcpy(race,par[i+1]);
         if (!strcmp(par[i],"-p")) if (i+1<lb)strcpy(prof,par[i+1]);
         if (!strcmp(par[i],"-sp")) if (i+1<lb)strcpy(spc,par[i+1]);
        }
 sscanf(spc,"%d",&sp);
 cout<<base<<endl;
 cout<<race<<endl;
 cout<<prof<<endl;
 cout<<sp<<endl;
 startup();
 kill_random();
}
