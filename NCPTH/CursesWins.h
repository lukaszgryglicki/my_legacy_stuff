#include <curses.h>
#define K_SPA 32
#define K_END 10070
#define K_DEL 0x7f
#define K_INS 10076
#define K_HOM 10072
#define K_ESC 27
#define K_PGU 10073
#define K_PGD 10071
#define K_BAC 8
#define K_ENT 10
#define K_UP  10065
#define K_DOW 10066
#define K_LEF 10068
#define K_RIG 10067

typedef WINDOW*&    pwnd;

int special_getch();


int msgbox(char* header, char* text)
{
  int c = strlen(header) + 30;
  int r = strlen(text)/strlen(header)+6;
  refresh();
  WINDOW* new_w = newwin(r+7, c+4, ((LINES-r)/2)-5, ((COLS-c)/2)-3);
  WINDOW* inter = newwin(r+3, c+2, ((LINES-r)/2)-2, ((COLS-c)/2)-2);
  WINDOW* inter2 = newwin(r, c, (LINES-r)/2, (COLS-c)/2-1);
  mvwprintw(new_w,1,11,"%s",header);
  box(new_w,ACS_VLINE, ACS_HLINE);
  box(inter,ACS_VLINE, ACS_HLINE);
  wprintw(inter2,"%s",text);
  WINDOW* oki = newwin(3, 6, (LINES/2)+r/6, (COLS)/2-3);
  box(oki,ACS_VLINE, ACS_HLINE);
  mvwprintw(oki,1,1,"OK");
  touchwin(new_w);
  wrefresh(new_w);
  touchwin(inter);
  wrefresh(inter);
  wrefresh(inter2);
  wrefresh(oki);
  delwin(new_w);
  delwin(inter);
  delwin(inter2);
  int ret = 0;
  noecho();
  while ((ret != K_ENT) && (ret != K_BAC))  ret = special_getch();
  echo();
  touchwin(stdscr);
  refresh();
  if (ret == K_ENT) return 1;
  else return 0;
}

int special_getch()
{
 int t[3];
 t[0] = getch();
 if (t[0] != 27) return t[0];
 t[1] = getch();
 if (t[1] != 91) return K_ESC;
 t[2] = getch();
 return 10000+t[2]; 
}

void draw(pwnd wnd)
{
 touchwin(wnd);
 wrefresh(wnd);
}


void init_main_window(char* title)
{
 int lens = strlen(title);
 int lenx = COLS;
 int offset = (((lenx-lens)/2)>=0) ? ((lenx-lens)/2) : 0;
 move(1, offset);
 attron(A_BOLD);
 printw(title);
 attroff(A_BOLD);
 box(stdscr, ACS_VLINE, ACS_HLINE);
 refresh(); 
}

WINDOW* create_window(int wrow, int wcol, int rows, int cols, char* title, bool show=true)
{
 WINDOW* ret = newwin(rows, cols, wrow, wcol);
 if (!ret) { printw("Cannot create window"); refresh(); return (WINDOW*)(0); }
 box(ret, ACS_VLINE, ACS_HLINE);
 if (show) wrefresh(ret);
 return ret;
}
