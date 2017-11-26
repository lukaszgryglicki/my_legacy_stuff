#include <mh.h>

#define EXEC_ATTR  S_IXUSR | S_IXGRP | S_IXOTH
#define SCROLL	   24
static int debug = 0;
static int all	 = 0;

void find(char* word, char* add)
{
 if (add)
	{
	 if (debug) cout<<"addictional path is set checking. . .\n";
	 if ((add[0] != '/') || (add[strlen(add)-1] == ':'))
		{
		 cout<<"Addictional PATH parse error.\n";
		 return;
		}
	}
 if (!strcmp(word,"ALLCMD")) all = 1;
 if (debug) cout<<"see_all="<<all<<endl;
 char* path = getenv("PATH");
 if (debug) cout<<"PATH="<<path<<endl;
 int num_dw=2;
 for (int i=0;i<strlen(path);i++) if (path[i] == ':') num_dw++;
 if (add)
	{
	 for (int i=0;i<strlen(add);i++)  if (add[i] == ':')  num_dw++;
	 num_dw++;
	}
 if (debug) cout<<"array[x]="<<num_dw<<endl;
 int length;
 if (!add) length = strlen(path);
 else length = (strlen(path) > strlen(add)) ? strlen(path) : strlen(add);
 if (debug) cout<<"array[y]="<<length<<endl;
 char** search = new (char*)[num_dw];
 for (int i=0;i<num_dw;i++) search[i] = new char[length];
 int j=0;
 int k=0;
 for (int i=0;i<strlen(path);i++)
   {
    if (debug) cout<<"i="<<i<<" j="<<j<<" k="<<k<<endl;
    if (path[i] != ':') {search[j][k] = path[i];k++;}
    else {search[j][k] = '\0'; j++; k=0;}
   }
 search[j][k] = '\0';
 j++;
 if (debug) cout<<"after path j="<<j<<endl;
 k=0;
 if (add)
 {
 if (debug) cout<<"reserving for addictional path. . .\n";
 for (int i=0;i<strlen(add);i++)
	{
	 if (debug) cout<<"i="<<i<<" j="<<j<<" k="<<k<<endl;
	 if (add[i] != ':') {search[j][k] = add[i]; k++;}
	 else
		{
		 search[j][k] = '\0'; j++; k=0;
		 if (add[i+1] != '/')
			{
			 cout<<"Missing / after : in addictional path set!\n";
			 delete [] search;
			 return;
			}
		}
	}
 search[j][k] = '\0';
 j++;
 if (debug) cout<<"afterr addictional path j="<<j<<endl;
 }
 if (debug) cout<<"setting search["<<j<<"] = NULL\n";
 search[j] = NULL;
 DIR* dp;
 char* mword = new char[strlen(word)+5];
 char* Stat = new char[0x200];
 strcpy(mword,word);
 downcase(mword);
 struct dirent *wpis;
 struct stat statbufor;
 int math = 0;
 int no_skip = 1;
 int zn;
 for (int i=0;i<j;i++)
   {
    if (!(dp = opendir(search[i]))) {if (debug) cout<<"Failed to open "<<search[i]<<endl;continue;}
    if (chdir(search[i])) {if (debug) cout<<"Faled to cd to "<<search[i]<<endl;continue;}
    while (wpis = readdir(dp))
     {
      stat(wpis->d_name,&statbufor);
      if (EXEC_ATTR & statbufor.st_mode)
	 {
	  if (S_IFDIR & statbufor.st_mode) continue;
	  strcpy(Stat, wpis->d_name);
	  downcase(Stat);
	  if ((strstr(Stat,mword)) || (all))
		{
		 cout<<search[i]<<" ==> "<<wpis->d_name<<endl;
		 math++;
		 if ((no_skip) && (math) && (!(math%SCROLL)))
			{
			 cout<<"Press SPACE to continue or s to skip or CTRL+C to abort. . .";
			 zn=getch();
			 ln();
			 if ((zn == 's') || (zn == 'S')) no_skip = 0;
			}
		}
	 }
     }
   }
 cout<<"Matches: "<<math<<endl;
 if (debug) cout<<"deleting dynamical array.\n";
 for (int i=0;i<num_dw;i++) delete [] search[i];
 delete [] search;
 delete [] mword;
 delete [] Stat;
}

int main(int lb, char** par)
{
 if ((lb-2) && (lb-3) && (lb-4))
	{
	 cout<<"Not a Windows system\nRequired parameter\nAt least --help\nStop.\n";
	 return -1;
	}
 if (!strcmp(par[1],"--help"))
	{
	 cout<<"This program searches PATH to find commands\n";
	 cout<<"cantaining string in their name\n";
	 cout<<"Usage:\nfcmd [string]\nfcmd [string] [addictional path]\n";
	 cout<<"Example: (to find all commands containing pw):\n";
	 cout<<par[0]<<" pw /usr/local/bin:/mybin:/inny/katalog/polecen\n";
	 cout<<"You can add olso last parameter -d (debug)\n";
	 cout<<"To see all cmds just type ALLCMD instead word.\n";
	 cout<<"Author: MorDeus\n";
	 return 0;
	}
 if (lb==2) find(par[1],NULL);
 if (lb==3)
   {
    if (!strcmp(par[2],"-d")) {debug=1; find(par[1],NULL);}
    else find(par[1], par[2]);
   }
 if (lb==4)
   {
    if (!strcmp(par[3],"-d")) debug=1;
    find(par[1],par[2]);
   }
 return 0;
}
