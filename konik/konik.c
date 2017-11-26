#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#define KEYU 65
#define KEYD 66
#define KEYL 68
#define KEYR 67
#define ENTR 10

int mgetch()
{
 struct termios st,end;
 int ret;
 tcgetattr(fileno(stdin),&st);
 ret = 0;
 end = st;
 end.c_lflag &= ~ICANON;
 end.c_lflag &= ~ECHO;
 end.c_cc[VMIN] = 1;
 end.c_cc[VTIME] = 0;
 if (tcsetattr(fileno(stdin),TCSAFLUSH,&end)) return 0;
 ret = getchar();
 tcsetattr(fileno(stdin),TCSAFLUSH,&st);
 return ret;
}


void draw_board(int** tab, int dim, int x, int y, int px, int py)
{
 int i,j;
 printf("/");
 for (i=0;i<dim;i++) printf("----");
 printf("\\\n");
 for (i=0;i<dim;i++)
 {
  printf("|");
  for (j=0;j<dim;j++)
    {
     if (i==(dim-y-1) && j==x) printf("X");
     else if (i==(dim-py-1) && j==px) printf("P");
     else printf("|");
     if (tab[i][j] == 0) printf("  ");
     else printf("%02x", tab[i][j]);
     if (i==(dim-y-1) && j==x) printf("X");
     else if (i==(dim-py-1) && j==px) printf("P");
     else printf("|");
    }
  printf("|\n");
 }
 printf("\\");
 for (i=0;i<dim;i++) printf("----");
 printf("/\n");
}

int get_move()
{
 int zn;
 zn = mgetch();
 if (zn == 27) zn = mgetch();
 if (zn == 91) zn = mgetch();
 return zn;
}

void save_state(int** tab, int dim)
{
 int i,j;
 char str[256]; 
 FILE* ptr;
 getfn:
 printf("Enter filename: ");
 fgets(str, 255, stdin);
 str[strlen(str)-1] = 0;
 if (!strcmp(str,"")) exit(0);
 ptr = fopen(str,"r");
 if (ptr) 
   {
    printf("Exists %s, type ENTER to skip saving...\n", str);
    fclose(ptr);
    goto getfn;
   }
 ptr = fopen(str,"w");
 if (!ptr) { printf("Cannot write to: %s, type ENTER to skip saving...\n", str); goto getfn; }
 for (i=0;i<dim;i++)
   {
    for (j=0;j<dim;j++) if (tab[i][j]) fprintf(ptr, "%-4d ", tab[i][j]);
    			else fprintf(ptr, "#### ");
    fprintf(ptr, "\n");
   }
 fclose(ptr);
}


void victory(int** tab, int dim)
{
 int zn;
 int i,j;
 char str[256]; 
 FILE* ptr;
 printf("You have win! Congratulation\n");
 printf("Want to save result? (Y/N) ");
 zn = mgetch();
 printf("\n");
 if (zn != 'Y' && zn != 'y') exit(0);
 getfn:
 printf("Enter filename: ");
 fgets(str, 255, stdin);
 str[strlen(str)-1] = 0;
 if (!strcmp(str,"")) exit(0);
 ptr = fopen(str,"r");
 if (ptr) 
   {
    printf("Exists %s, type ENTER to skip saving...\n", str);
    fclose(ptr);
    goto getfn;
   }
 ptr = fopen(str,"w");
 if (!ptr) { printf("Cannot write to: %s, type ENTER to skip saving...\n", str); goto getfn; }
 for (i=0;i<dim;i++)
   {
    for (j=0;j<dim;j++) fprintf(ptr, "%-4d ", tab[i][j]);
    fprintf(ptr, "\n");
   }
 fclose(ptr);
 exit(0);
}

void try_move(int** tab, int dim, int x, int y, int* px, int* py, int* cnt, int emirr)
{
 int dx,dy;
 if (*px<0 && *py<0 && *cnt==0)
   {
    (*cnt)++;
    tab[dim-y-1][x] = *cnt;
    *px = x;
    *py = y;
    return;
   } 
 dx = abs(*px-x);
 dy = abs(*py-y);
 if (emirr)
  {
   if (dx == dim-2) dx = 2;
   if (dy == dim-2) dy = 2;
   if (dx == dim-1) dx = 1;
   if (dy == dim-1) dy = 1;
  }
 printf("(%d,%d)\n", dx,dy);
 if ((!tab[dim-y-1][x]) && ((dx==1 && dy==2) || (dx==2 && dy==1)))
   {
    (*cnt)++;
    tab[dim-y-1][x] = *cnt;
    *px = x;
    *py = y;
    if ((*cnt) == (dim*dim)) victory(tab, dim);
    return;
   }
 else printf("illegal move.\n");
}

void konik(int dim, int emirr)
{
 int zn,i,j;
 int pos_X;
 int pos_Y;
 int prev_X;
 int prev_Y;
 int cnt;
 int** table;
 if (dim<=4 || dim>16) { printf("bad dim value: %d\n", dim); exit(1); }
 zn = 'x';
 table = (int**)malloc(dim*sizeof(int*));
 pos_X = pos_Y = 0;
 prev_X = prev_Y = -1;
 cnt = 0;
 for (i=0;i<dim;i++) table[i] = (int*)malloc(dim*sizeof(int));
 for (int i=0;i<dim;i++) for (j=0;j<dim;j++) table[i][j] = 0;
 while (zn != 'q')
  {
   draw_board(table, dim , pos_X, pos_Y, prev_X, prev_Y);
   zn = get_move();
   if (zn == KEYU) pos_Y++;
   else if (zn == KEYD) pos_Y--;
   else if (zn == KEYR) pos_X++;
   else if (zn == KEYL) pos_X--;
   else if (zn == ENTR) try_move(table, dim, pos_X, pos_Y, &prev_X, &prev_Y, &cnt, emirr);
   else if (zn == 's' || zn == 'S') save_state(table, dim);
   else continue;
   if (pos_X<0) pos_X += dim;
   if (pos_Y<0) pos_Y += dim;
   if (pos_X>=dim) pos_X -= dim;
   if (pos_Y>=dim) pos_Y -= dim;
  }
 for (i=0;i<dim;i++) free(table[i]);
 free(table);
}

int main(int lb, char **par)
{
 printf("%s board_dimnesion [5-16] enable_mirrors [0|1]\n", par[0]);
 printf("Keys are: Up/Down/left/Right/Q/S/ENTER\n");
 konik((lb>=2)?(atoi(par[1])):10, (lb>=3)?(atoi(par[2])):1);
 return 0;
}

