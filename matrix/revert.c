#include "mh.h"
#define MAX 100
static double A[MAX][MAX];

void SeeTmp(double tmp[MAX][2*MAX], int siz)
{
 cout<<"\n\n";				//wyswietla macierz
 for (int i=0;i<siz;i++){
  for (int j=0;j<2*siz;j++){ 
    if (j == siz) cout<<"  |  ";
    printf("%7.3f ", tmp[i][j]);
   }
  cout<<endl;
 }
 cout<<endl;
}

int Tryswitch(double tmp[MAX][MAX*2], int i, int siz)
{
 int x; 				//wyszukuje wiersz do zamiany
 for (x=i;x<siz;x++) if (tmp[x][i]) return x;
 return -1;
}

void RevertMatrix(int siz)
{					//odwraca macierz
 double tmp[MAX][2*MAX];
 for (int i=0;i<siz;i++) for (int j=0;j<siz;j++) tmp[i][j] = A[i][j];
 for (int i=0;i<siz;i++) for (int j=siz;j<2*siz;j++) if (i == j-siz) tmp[i][j] = 1.0; else tmp[i][j] = 0.0; 
 SeeTmp(tmp, siz);
 double div;
 double pom;
 int swit;
 double p[2*MAX];
 for (int i=0;i<siz;i++)
  {
   div = tmp[i][i];
   if (!div) 
      {
      swit = Tryswitch(tmp, i, siz);
      if (swit == -1) {cout<<"Macierz Osobliwa!\n"; exit( -1 );}
      for (int x=0;x<2*siz;x++) p[x] = tmp[i][x];
      for (int x=0;x<2*siz;x++) tmp[i][x] = tmp[swit][x];
      for (int x=0;x<2*siz;x++) tmp[swit][x] = p[x];
      div = tmp[i][i];
     }
   for (int j=0;j<2*siz;j++) tmp[i][j] /= div; 
   for (int k=0;k<siz;k++)
	{
	 pom = tmp[k][i];
	 if (k-i) for (int j=0;j<2*siz;j++) tmp[k][j] -= pom*tmp[i][j]; 
	}     
  }
 SeeTmp(tmp, siz);
}

int ReadMatrix(char* fmt)
{
 FILE* in;
 char name[STRING];
 int size = 0;
 sscanf(fmt,"%s", name);
 if ((in = fopen(name,"r")) == NULL) {cout<<"Panic!\n"<<name<<" no such file.\n"; exit(-1);}
 cout<<"File "<<name<<" opened.\n";
 fscanf(in,"%d\n",&size);
 if (!size) {cout<<"Panic!\nError while reading "<<name<<endl; exit(-1);}
 for (int i=0;i<size;i++) 
   {
    for (int j=0;j<size;j++) fscanf(in,"%lf ", &A[i][j]);
    fscanf(in,"\n");
   }
 fclose(in);
 return size;
}

void CreateMatrix(int siz)
{
 init_random(DEV_NULL);
 for (int i=0;i<siz;i++) for (int j=0;j<siz;j++) A[i][j] = double(random(0x2000)-0x1000) / (double)0x100;
}
int main(int lb, char** par)
{
 if (lb-2) {cout<<"Stop\nRequired parameter.\n"; exit(-1);}
 int siz = -1;
 int ok = sscanf(par[1],"%d",&siz);
 if (!ok) siz = ReadMatrix(par[1]);
 else CreateMatrix(siz);
 if ((siz < 0) || (siz > MAX)) {cout<<"Panic!\nSize mismatch.\nMAX_SIZE is "<<MAX<<endl; exit(-1);}
 RevertMatrix(siz);
 cout<<"OK.\n";
 kill_random();
 return 0;
}
