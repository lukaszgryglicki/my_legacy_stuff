

#include <stdio.h>    /*  standardowy naglowek C    */
#include <dos.h>      /*  atrybuty plikow           */
#include <dir.h>      /*  rekursywne dzialanie :)   */

#define VATTR FA_RDONLY | FA_SYSTEM | FA_HIDDEN | FA_DIREC | FA_ARCH
#define LENGTH 255
#define LINE    40

static bool  rec_rd   = false;
static bool  rec_sys  = false;
static bool  rec_hid  = false;
static bool  urec_rd  = false;
static bool  urec_sys = false;
static bool  urec_hid = false;

inline bool __fastcall isDir(ffblk& ff)
{if (ff.ff_attrib & FA_DIREC) return true; else return false;}

void __fastcall rdStart(char*);

void __fastcall RecDir(char* st1, char* st2)
{
 if (!strcmp(st2, ".") || !strcmp(st2, "..")) return;
 char path[LENGTH];
 strcpy(path, st1);
 strcat(path, st2);
 strcat(path, "\\");
 rdStart(path);
}

void __fastcall sattr(ffblk& ff)
{
 //printf("%s\n",ff.ff_name);
 if  (rec_rd)  ff.ff_attrib &= ~FA_RDONLY;
 if  (rec_hid) ff.ff_attrib &= ~FA_HIDDEN;
 if  (rec_sys) ff.ff_attrib &= ~FA_SYSTEM;

 if  (urec_rd)  ff.ff_attrib |= FA_RDONLY;
 if  (urec_hid) ff.ff_attrib |= FA_HIDDEN;
 if  (urec_sys) ff.ff_attrib |= FA_SYSTEM;
 _dos_setfileattr(ff.ff_name,ff.ff_attrib);
}


void __fastcall rdStart(char* path)    /*niszczy rekursywnie katalog podany jako disk */
{

   struct ffblk ff;
   int done;
   int notOK;
   char par[LENGTH];
   strcpy(par,path);
   if (par[strlen(par)-1] != '\\') strcat(par,"\\");
   notOK = chdir(par);
   if (notOK) return;
   done = findfirst("*.*",&ff,VATTR);
   sattr(ff);
   if (isDir(ff)) RecDir(par, ff.ff_name);
   while (!done)
        {
         done = findnext(&ff);
         sattr(ff);
         if (isDir(ff)) RecDir(par, ff.ff_name);
        }
}

void execute_flag(char* f)
{
 if (strlen(f) < 2) return;
 if (f[0] == '-')
  {
   if (f[1] == 'r') rec_rd = true;
   if (f[1] == 's') rec_sys = true;
   if (f[1] == 'h') rec_hid = true;
  }
 if (f[0] == '+')
  {
   if (f[1] == 'r') urec_rd = true;
   if (f[1] == 's') urec_sys = true;
   if (f[1] == 'h') urec_hid = true;
  }
}


int main(int lb, char** par)
{
 if (lb < 3) {printf("Too Small parameters:\nurdonly [flags] disk:\\directory\\\n");return -1;}
 execute_flag(par[1]);
 printf("flag is: rsh-rsh:%d%d%d%d%d%d\n",rec_rd,rec_sys,rec_hid,urec_rd,urec_sys,urec_hid);
 for (int i=2;i<lb;i++) rdStart(par[i]);
 return 0;
}
