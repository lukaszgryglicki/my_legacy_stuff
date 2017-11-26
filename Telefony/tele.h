#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>

//#include "oswindows"
#include "oslinux"

#if OSTYPE == WINDOWS
#include <conio.h>
#endif

#include <mh.h>
//#include "mh.h" //for remote systems.

#if !defined TELE_H
#define TELE_H

#define MAX_LIT  1200

class TELE
{
 public:

 TELE();
 ~TELE();

 char* PokazNazwe();
 void DodajTelefon();
 void NadajNazwe();
 void ZobaczTelefony();
 void Zobacz(char);
 void ZapiszTelefony();
 void WczytajTelefony();
 void UsunTelefon();
 void ModyfikujTelefon();
 void Modyfikuj(int);
 void ZobaczWszystko();
 void Szukaj();
 void GenerujTXT();
 void CheckOut();
 void Sortuj();
 void Zamien(int, int);

 private:

 int ilosc;
 char naz[STRING];

 char nazwa[MAX_LIT][STRING];
 char numer[MAX_LIT][STRING];
 char email[MAX_LIT][STRING];

};
TELE :: TELE()
{
 ilosc = 0;
 strcpy(naz,"Brak_nazwy");
 for (int i=0;i<MAX_LIT;i++)
 	{
    strcpy(nazwa[i],"(null)");
    strcpy(numer[i],"000-000");
    strcpy(email[i],"no-mail");
   }
}
TELE :: ~TELE()
{
 printf("Uwolnienie %d KB pamieci.\n",sizeof(TELE)/1024);
 cout<<"Nacisnij cokolwiek aby zakonczyc dzialanie programu. . .\n";
}

char* TELE :: PokazNazwe()
{
 return naz;
}

void TELE :: NadajNazwe()
{
 cout<<"Podaj nazwe dla spisu telefonow: ";
 takestring(naz,20);
}

void TELE :: DodajTelefon()
{
 char zz = 't';
 while (zz == 't')
 {
 if (ilosc == MAX_LIT)
 	{
    cout<<"To juz maksymalna ilosc telefonow: "<<MAX_LIT<<endl;
    return;
   }
 if (strcmp(naz,"Brak_nazwy") == 0)
 	{
    NadajNazwe();
   }
 cout<<"Podaj nazwe telefonu: ";
 takestring(nazwa[ilosc]);
 cout<<"Podaj numer do "<<nazwa[ilosc]<<": ";
 takestring(numer[ilosc]);
 cout<<"Podaj e-maila do "<<nazwa[ilosc]<<": ";
 takestring(email[ilosc]);
 cout<<"Dodano nastepujocy telefon "<<nazwa[ilosc]<<" "<<numer[ilosc]<<" mail: "<<email[ilosc]<<endl;
 ilosc++;
 cout<<"Dodac nastepny? ";
 takechar(&zz);
 }
}

void TELE :: ZobaczTelefony()
{
 char zn;
 cout<<"Podaj jaka litere chcesz zobaczyc: ";
 takechar(&zn);
 downcase(zn);
 cout<<"Wszystkie telefony na litere "<<zn<<endl;
 Zobacz(zn);
}

void TELE :: Zobacz(char lit)
{
 int num = 0;
 for (int i=0;i<ilosc;i++)
   {
    if ((nazwa[i][0] == lit) || (nazwa[i][0] == lit - 0x20)) num ++;
   }
 if (num) cout<<"LITERA:\t"<<lit<<", telefonow "<<num<<endl;
 num = 0;
 for (int i=0;i<ilosc;i++)
   {
    if ((nazwa[i][0] == lit) || (nazwa[i][0] == lit - 0x20))
      {
       num++;
       printf("%-2d) %-30s NUMER: %-20s mail: %-20s\n",num,nazwa[i],numer[i], email[i]);
      }
   }
 if (num) lns(2);
}
void TELE :: ZapiszTelefony()
{
 if (strcmp(naz,"Brak_nazwy") == 0)
 	{
    cout<<"Spis nie ma nadanej nazwy, nie moge go zapisac.\n";
    cout<<"Nadaj nazwe uzywajac polecenia d.\n";
    return;
   }
 char name[STRING];
 strcpy(name,"Data/");
 strcat(name,naz);
 strcat(name,".dat");
 FILE* plik;
 if ((plik = fopen(name,"r+")) != NULL)
 	{
    cout<<"Plik "<<name<<" istnieje, nadpisac go? ";
    char zn;
    takechar(&zn);
    fclose(plik);
    if (zn != 't') return;
   }
 if ((plik = fopen(name,"w+")) == NULL)
 	{
    cout<<"Nie moge otworzyc pliku "<<name<<" do zapisu.\n";
    return;
   }
 CheckOut();
 fprintf(plik,"%s\n",naz);
 fprintf(plik,"%d\n",ilosc);
 for (int i=0;i<ilosc;i++)
 	{
    fprintf(plik,"%s\n",nazwa[i]);
    fprintf(plik,"%s\n",numer[i]);
    fprintf(plik,"%s\n",email[i]);
   }
 fprintf(plik,"Made by morgothdbma\n");
 fclose(plik);
}

void TELE :: WczytajTelefony()
{
 char name[STRING];
 strcpy(name,"Data/");
 char str2[STRING];
 cout<<"Podaj nazwe pliku do wczytania: ";
 takestring(str2,25);
 strcat(name,str2);
 strcat(name,".dat");
 FILE* plik;
 if ((plik = fopen(name,"r+")) == NULL)
 	{
    cout<<"Plik "<<name<<" nie istnieje.\n";
    return;
   }
 fscanf(plik,"%s\n",naz);
 fscanf(plik,"%d\n",&ilosc);
 for (int i=0;i<ilosc;i++)
 	{
    fscanf(plik,"%s\n",nazwa[i]);
    fscanf(plik,"%s\n",numer[i]);
    fscanf(plik,"%s\n",email[i]);
   }
 fclose(plik);
 CheckOut();
}

void TELE :: UsunTelefon()
{
 char zz = 't';
 while (zz == 't')
 {
 cout<<"Na jaka litere zaczyna sie telefon: ";
 char zn;
 takechar(&zn);
 downcase(zn);
 for (int i=0;i<ilosc;i++)
 	{
    if ((nazwa[i][0] == zn) || (nazwa[i][0] == zn - 0x20))
    	{
       printf("%4d) %s\n",i,nazwa[i]);
      }
   }
    cout<<"Podaj numer pod ktorym znajduje sie telefon do usuniecia: ";
    int num;
    takeint(&num);
    if ((num < 0) || (num > ilosc -1 ))
    	{
       cout<<"Poza zakresem tabeli. . .\n";
       return;
      }
    ilosc--;
    for (int i=num;i<ilosc;i++)
    	{
       strcpy(nazwa[i],nazwa[i+1]);
       strcpy(numer[i],numer[i+1]);
       strcpy(email[i],email[i+1]);
      }
 cout<<"Usuwac dalej? ";
 takechar(&zz);
 }
}

void TELE :: Zamien(int i, int j)
{
 char tmp[STRING];
 strcpy(tmp, nazwa[i]);
 strcpy(nazwa[i], nazwa[j]);
 strcpy(nazwa[j], tmp);
 strcpy(tmp, numer[i]);
 strcpy(numer[i], numer[j]);
 strcpy(numer[j], tmp);
 strcpy(tmp, email[i]);
 strcpy(email[i], email[j]);
 strcpy(email[j], tmp);
}

void TELE :: Sortuj()
{
 for (int i=0;i<ilosc;i++)
    {
     for (int j=0;j<ilosc-(i+1);j++)
        {
	 if (StrMore(nazwa[j], nazwa[j+1])) Zamien(j+1, j);
	}
     }
}



void TELE :: ModyfikujTelefon()
{
 char zz = 't';
 while (zz == 't')
 {
    Szukaj();
    cout<<"Podaj numer pod ktorym znajduje sie telefon do modyfikacji: ";
    int num;
    takeint(&num);
    if ((num < 0) || (num > ilosc -1 ))
    	{
       cout<<"Poza zakresem tabeli. . .\n";
       return;
      }
 Modyfikuj(num);
 cout<<"Modyfikowac dalej? ";
 takechar(&zz);
 }
}
void TELE :: Modyfikuj(int num)
{
 cout<<"Aktualnie "<<nazwa[num]<<" NUMER "<<numer[num]<<" mail: "<<email[num]<<endl;
 cout<<"Modyfikowac nazwe? ";
 char zn;
 takechar(&zn);
 if (zn == 't')
 	{
    cout<<"Podaj nowa nazwe: ";
    takestring(nazwa[num]);
   }
 cout<<"Modyfikowac numer? ";
 takechar(&zn);
 if (zn == 't')
 	{
    cout<<"Podaj nowy numer: ";
    takestring(numer[num]);
   }
 cout<<"Modyfikowac e-mail? ";
 takechar(&zn);
 if (zn == 't')
 	{
    cout<<"Podaj nowy e-mail: ";
    takestring(email[num]);
   }
}

void TELE :: ZobaczWszystko()
{
 int all = 0;
 for (char lit = 'a'; lit<= 'z';lit++)
 	{
    for (int i=0;i<ilosc;i++)
    	{
       if ((nazwa[i][0] == lit) || (nazwa[i][0] == lit -0x20))
       	{
          all ++;
          printf("%4d) %-30s %-20s mail: %-20s\n",all,nazwa[i],numer[i], email[i]);
         }
      }
   }
}

void TELE :: Szukaj()
{
 cout<<"Podaj wzorzec: ";
 char rdz[STRING];
 takestring(rdz,15);
 for (int i=0;i<(int)strlen(rdz);i++) downcase(rdz[i]);
 cout<<"Wzorzec: "<<rdz<<endl;
 char ok[STRING];
 for (int i=0;i<ilosc;i++)
 	{
       strcpy(ok, nazwa[i]);
       for (int j=0;j<(int)strlen(ok);j++) downcase(ok[j]);
       if (strstr(ok, rdz))
       printf("%4d) %-30s %-20s mail: %-20s\n",i,nazwa[i],numer[i], email[i]);
   }
}

void TELE :: GenerujTXT()
{
 if (strcmp(naz,"Brak_nazwy") == 0)
 	{
    cout<<"Spis nie ma nadanej nazwy, nie moge wygenerowac pliku TXT.\n";
    cout<<"Nadaj nazwe uzywajac polecenia d.\n";
    return;
   }
 char name[STRING];
 strcpy(name,"Data/");
 strcat(name,naz);
 strcat(name,".txt");
 FILE* plik;
 if ((plik = fopen(name,"r+")) != NULL)
 	{
    cout<<"Plik "<<name<<" istnieje, nadpisac go? ";
    char zn;
    takechar(&zn);
    fclose(plik);
    if (zn != 't') return;
   }
 if ((plik = fopen(name,"w+")) == NULL)
 	{
    cout<<"Nie moge otworzyc pliku "<<name<<" do zapisu.\n";
    return;
   }
 cout<<"Generuje plik "<<name<<endl;
 int all = 0;
 int num;
 for (char lit = 'a'; lit<= 'z';lit++)
 	{
    num = 0;
    for (int i=0;i<ilosc;i++)
    	{
       if ((nazwa[i][0] == lit) || (nazwa[i][0] == lit -0x20)) num ++;
      }
    if (num)
    	{
       fprintf(plik,"LITERA\t%c\n",lit);
    	 for (int i=0;i<ilosc;i++)
    		{
       	 if ((nazwa[i][0] == lit) || (nazwa[i][0] == lit -0x20))
       	 	{
             all ++;
             fprintf(plik,"%4d) %-30s %-20s mail: %-20s\n",all,nazwa[i],numer[i], email[i]);
         	}
      	}
       fprintf(plik,"\n");
      }
   }
 fclose(plik);
}

void TELE :: CheckOut()
{
 for (int i=0;i<ilosc;i++)
 	{
    if ((nazwa[i][0] > 'z') || ((nazwa[i][0] > 'Z') && (nazwa[i][0] < 'a')) || (nazwa[i][0] < 'A'))
       {
        cout<<"("<<i<<")Wpis "<<nazwa[i]<<" jest  niepoprawny\nNiepoprawny jest pierwszy znak "<<nazwa[i][0]<<" to nie jest litera.\n";
        cout<<"Zmienic ten wpis? ";
        char zn;
        takechar(&zn);
        if (zn == 't')
        	{
          cout<<"Zamien "<<nazwa[i][0]<<" na ";
          takechar(&nazwa[i][0]);
         }
       }
   }
}

#endif

