#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <termios.h>
#include <math.h>
static int r_type = 0;
#include "Mrandom.h"


#define STRING          70
#define OK              0
#define FALSE           0
#define TRUE            1
#define OS              "Linux"
#define EXISTS          1
#define NO_EXISTS       -1
#define CHECKED         20
#define NORMAL          21



static int GET_T = CHECKED;





inline void ln();
void takechar(char*);
void takestring(char*,int);
void lns(int);
void takeint(int*);
int random(int);
void DecompressData(char*);
void CompressData(char*);
void FromBinary(FILE*, FILE*);
void ToBinary(FILE*, FILE*);
void Trap();
inline void wait();
inline void Second();
void downcase(char*);
void downcase(char&);
bool StrMore(char* , char*);
int getch();






inline void ln() {cout<<endl;}


void Trap()
{
 cout<<"STOP!\n";
 int a;
 takeint(&a);
}


bool StrMore(char* str1, char* str2)
{
 int len = (strlen(str1) < strlen(str2)) ? strlen(str1) : strlen(str2);
 int i = 0;
 while ((str1[i] == str2[i]) && (i <= len)) i++;
 if (str1[i] > str2[i]) return true;
 else return false;
}

int getch()
{
 struct termios st,end;
 tcgetattr(fileno(stdin),&st);
 end = st;
 end.c_lflag &= ~ICANON;
 end.c_lflag &= ~ECHO;
 end.c_cc[VMIN] = 1;
 end.c_cc[VTIME] = 0;
 if (tcsetattr(fileno(stdin),TCSAFLUSH,&end)) return 0;
 st:
 int ret = 0;
 ret = getchar();
 if (ret == 10) goto st;
 tcsetattr(fileno(stdin),TCSAFLUSH,&st);
 cout<<(char)ret;
 return ret;
}





void GetStrType(int par)
{
 GET_T = par;
}


void SwitchIn()
{
 if (GET_T == CHECKED) GET_T = NORMAL;
 else GET_T = CHECKED;
}


void DecompressData(char* data)
{
 for (int i=0;i<STRING;i++) if (data[i] == '_') data[i] = ' ';
}


void CompressData(char* data)
{
 for (int i=0;i<STRING;i++) if (data[i] == ' ') data[i] = '_';
}


void FromBinary(FILE* in, FILE* out)
{
 int zn;
 int sign;
 int array[8];
 while ((zn = fgetc(in)) != EOF)
 	{
    array[0] = zn-48;
    for (int i=1;i<8;i++) array[i] = fgetc(in)-48;
    sign = 0;
    for (int i=0;i<8;i++) sign += int(pow(2,7-i)*array[i]);
    fprintf(out,"%c",sign);
   }
}



void ToBinary(FILE* in, FILE* out)
{
 int zn;
 int array[8];
 while ((zn = fgetc(in)) != EOF)
 	{
    for (int i = 7;i>=0;i--)
    	{
       if (zn >= (int)pow(2,i)) {array[7-i] = 1; zn -= (int)pow(2,i);} else array[7-i] = 0;
      }
    for (int i=0;i<8;i++) fprintf(out,"%d",array[i]);
   }
}




void takechar(char* zn)
{
 char znak[STRING];
 scanf("%s",znak);
 sscanf(znak,"%c",zn);
 if (strlen(znak)>1) cout<<"Podales za dluga nazwe\n";
}

void takestring(char* par, int sec = 0)
{
 struct termios st,end;
 tcgetattr(fileno(stdin),&st);
 end = st;
 end.c_lflag &= ~ICANON;
 end.c_lflag &= ~ECHO;
 end.c_cc[VMIN] = 1;
 end.c_cc[VTIME] = 0;
 if (tcsetattr(fileno(stdin),TCSAFLUSH,&end) != 0) return;
 int i;
 st:
 i = 0;
 par[0] = 0;
 while (i<STRING-1)
        {
         par[i] = getchar();
         if ((i ==0) && (par[i] == 10)) goto cont;
         if (par[i] == ' ') par[i] = '_';
         if (par[i] == 10) {par[i] = '\0';goto lab;}
         if (par[i] == 127) {if (i>0)printf("\b \b");i--;if (i<0) {i=0;}goto cont;}
         if (par[i] == 27) {cout<<"Sorry, you have used illegal character UNICODE is 0x1b, do it again.\n";goto st;}
         if (sec != 0xfade) cout<<par[i];
         i++;
         cont:;
        }
 lab:
 if (i == 0) {strcpy(par,"-");cout<<"-";}
 ln();
 tcsetattr(fileno(stdin),TCSAFLUSH,&st);
}


void lns(int par)
{
 for (short i=0;i<par;i++) ln();
}


inline void wait()
{
 char zn;
 takechar(&zn);
}



void takeint(int* par)
{
 char string[STRING];
 scanf("%s",string);
 if (!sscanf(string,"%d",par)) {*par = 0;cout<<"Parametr literowy! Zmnieniam na 0\n";}
}

void takeint(int& par)
{
 char string[STRING];
 scanf("%s", string);
 if (!sscanf(string,"%d", &par)) {par = 0; cout<<"Parametr literowy! Zmieniam na 0\n";}
}

void takeint(unsigned int* par)
{
 char string[STRING];
 scanf("%s",string);
 if (!sscanf(string,"%d",par)) {*par = 0;cout<<"Parametr literowy! Zmnieniam na 0\n";}
}

void takeint(unsigned int& par)
{
 char string[STRING];
 scanf("%s", string);
 if (!sscanf(string,"%d", &par)) {par = 0; cout<<"Parametr literowy! Zmieniam na 0\n";}
}

inline void downcase(char& str)
{
  if ((str >= 'A') && (str <= 'Z')) str += (char)0x20;
}

void downcase(char* str)
{
 for (int i=0;i<(int)strlen(str);i++)
 	{
    if ((str[i] >= 'A') && (str[i] <= 'Z')) str[i] += (char)0x20;
   }
}








