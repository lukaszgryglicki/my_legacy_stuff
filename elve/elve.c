#include <mh.h>

void SwitchUp(int& zn)
{
 switch(zn)
        {
         case 'a': zn = 'e';  break;
         case 'b': zn = 'c';  break;
         case 'c': zn = 'd';  break;
         case 'd': zn = 'f';  break;
         case 'e': zn = 'i';  break;
         case 'f': zn = 'g';  break;
         case 'g': zn = 'h';  break;
         case 'h': zn = 'j';  break;
         case 'i': zn = 'o';  break;
         case 'j': zn = 'k';  break;
         case 'k': zn = 'l';  break;
         case 'l': zn = 'm';  break;
         case 'm': zn = 'n';  break;
         case 'n': zn = 'p';  break;
         case 'o': zn = 'u';  break;
         case 'p': zn = 'q';  break;
         case 'q': zn = 'r';  break;
         case 'r': zn = 's';  break;
         case 's': zn = 't';  break;
         case 't': zn = 'v';  break;
         case 'u': zn = 'y';  break;
         case 'v': zn = 'w';  break;
         case 'w': zn = 'x';  break;
         case 'x': zn = 'z';  break;
         case 'y': zn = 'a';  break;
         case 'z': zn = 'b';  break;
        }
}

void SwitchDown(int& zn)
{
 switch(zn)
        {
         case 'a': zn = 'y';  break;
         case 'b': zn = 'z';  break;
         case 'c': zn = 'b';  break;
         case 'd': zn = 'c';  break;
         case 'e': zn = 'a';  break;
         case 'f': zn = 'd';  break;
         case 'g': zn = 'f';  break;
         case 'h': zn = 'g';  break;
         case 'i': zn = 'e';  break;
         case 'j': zn = 'h';  break;
         case 'k': zn = 'j';  break;
         case 'l': zn = 'k';  break;
         case 'm': zn = 'l';  break;
         case 'n': zn = 'm';  break;
         case 'o': zn = 'i';  break;
         case 'p': zn = 'n';  break;
         case 'q': zn = 'p';  break;
         case 'r': zn = 'q';  break;
         case 's': zn = 'r';  break;
         case 't': zn = 's';  break;
         case 'u': zn = 'o';  break;
         case 'v': zn = 't';  break;
         case 'w': zn = 'v';  break;
         case 'x': zn = 'w';  break;
         case 'y': zn = 'u';  break;
         case 'z': zn = 'x';  break;
        }
}


void Transform(int& zn, int kod)
{
 bool large = false;
 if ((zn >= 'A') && (zn <= 'Z'))
        {
         large = true;
         zn += 0x20;
        }
 if (kod > 0) for (int i=0;i<kod;i++) SwitchUp(zn);
 if (kod < 0) for (int i=0;i<-1*kod;i++) SwitchDown(zn);

 if (large) zn -= 0x20;
}

void CodingSequence(FILE* in, FILE* out)
{
 int zn;
 cout<<"Mode? ";
 int mod = getch();
 ln();
 if ((mod != 'c') && (mod != 'd'))
        {
         cout<<"Statement error\n";
         return;
        }
 cout<<"Code? ";
 char code[STRING];
 takestring(code);
 int* kod = new int[strlen(code)+1];
 for (int i=0;i<(int)strlen(code);i++)
        {
         if ((code[i] >= '0') && (code[i] <= '9')) kod[i] = code[i] - 0x30;
         else kod[i] = 0;
        }
 cout<<"Code is: ("<<strlen(code)<<")->";
 for (int i=0;i<(int)strlen(code);i++) cout<<kod[i];
 ln();
 int cur = 0;
 int len = (int)strlen(code);
 int cod = 0;
 cout<<"Started.\n";
 while ((zn = fgetc(in)) != EOF)
        {
        cur %= len;
        if (mod == 'c') cod = kod[cur];
        else cod = -kod[cur];
         if (((zn >= 'A') && (zn <= 'Z')) || ((zn >= 'a') && (zn <= 'z')))
                {
                 Transform(zn,cod);
                }
         fprintf(out,"%c",zn);
         cur++;
        }
 cout<<"Done.\n";
}

void Code(char* inn, char* outn)
{
 FILE* in;
 FILE* out;
 if ((in = fopen(inn,"r+")) == NULL)
        {
         cout<<"No such file: "<<inn<<endl;
         return;
        }
 if ((out = fopen(outn,"r+")) != NULL)
        {
         cout<<"Already exists: "<<outn<<endl<<"Overwrite? ";
         int ov = getch();
         ln();
         fclose(out);
         if (ov != 'y')
                {
                 fclose(in);
                 return;
                }
        }
 if ((out = fopen(outn,"w+")) == NULL)
        {
         cout<<"Unable to create file: "<<outn<<endl;
         fclose(in);
         return;
        }
 CodingSequence(in,out);
 fclose(in);
 fclose(out);
}

int main(int lb, char** par)
{
 if (lb != 3)
        {
         cout<<"Usage elve file1 file2\n";
         return 0;
        }
 Code(par[1],par[2]);
}
