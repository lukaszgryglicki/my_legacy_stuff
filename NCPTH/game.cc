#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <curses.h>
#include <iostream>
#include "CursesWins.h"

using namespace std;
int fd;
int fd2;
int pid;
char fifo[] = "./fifo1";
char fifo2[]= "./fifo2";
void remove()
{
 system("rm -f ./fifo?");
}

void listen_UI_events()
{
 int zn;
 while (1)
 {
  zn = getch();
  move(14,15);
  printw("%c%c%c", (char)zn,(char)zn,char(zn));
  move(15,15);
  printw("%c%c%c", (char)zn,(char)zn,char(zn));
  move(16,15);
  printw("%c%c%c", (char)zn,(char)zn,char(zn));
  refresh();
  if (zn=='q') break;
 }
 endwin();
}

void init()
{
 initscr();
 init_main_window("Gra w kosci!");
 msgbox("jakis","bardzo bardzo durny i do tego dlugi tekst o programie");
}

void* game_proc_1(void* arg)
{
 char zn;
 while (1) 
 {
  read(fd,&zn,1);
  if (zn=='Q') break;
  move(10,10);
  printw("P1: Read from P2: %c",zn);
  refresh();
  sleep(1);
  move(20,20);
  printw("P1: Write to P2: %c",char(zn+0x20));
  refresh();
  zn+=0x20;
  write(fd2,&zn,1);
 }
 endwin();
 cout<<"Read EOF"<<endl;
 close(fd);
 close(fd2);
 cout<<"done.\n";
 return NULL;
}

void* game_proc_2(void* arg)
{
 char ans;
 for (char i='A';i<'F';i++) 
  {
   move(10,10);
   printw("P2: Write to P1: %c",i);	
   refresh();
   sleep(1);
   write(fd,&i,1); 
   read(fd2,&ans,1);
   move(20,10);
   printw("P2: Read from P1: %c",ans);	
   refresh();
  }
 ans='Q';
 write(fd,&ans,1);
 endwin();
 close(fd);
 close(fd2);
 cout<<"done.\n";
 return NULL;
}

void first_player()
{
 fd=open(fifo,O_WRONLY);
 cout<<"first player!\n";
 if (mkfifo(fifo,0666) == -1)
  {
   cout<<"Cannot create a FIFO!\n";
   exit(1);
  }
 fd = open(fifo,O_RDONLY);
 if (fd<0) 
  {
   cout<<"Cannot open FIFO for reading!\n";
   exit(2);
  }
 cout<<"opened.\n";
 if (mkfifo(fifo2,0666) == -1)
  {
   cout<<"Cannot create FIFO2!\n";
   exit(3);
  }
 fd2 = open(fifo2,O_WRONLY);
 if (fd2<0) 
  {
   cout<<"Cannot open FIFO2 for writing!\n";
   exit(4);
  }
 cout<<"opened2.\n";
 init();
 pthread_t thread;
 int error;
 error = pthread_create(&thread,NULL,game_proc_1,NULL);
 if (error) { cout<<"bledny blad, brak obslugi bledow!!\n"; exit(666); }
 listen_UI_events();
 error = pthread_join(thread,NULL);
 if (error) 
  { 
   cout<<"strasznie zly blad, komputer nadaje sie do reperacji!\n";
   exit(777);
  }
}

void second_player()
{
 cout<<"second player!\n";
 cout<<"opened!\n";
 fd2 = open(fifo2,O_RDONLY);
 if (fd<0) 
  {
   cout<<"Cannot open FIFO2 for reading!\n";
   exit(2);
  }
 cout<<"opened2.\n";
 init();
 pthread_t thread;
 int error;
 error = pthread_create(&thread,NULL,game_proc_2,NULL);
 if (error) { cout<<"bledny blad, brak obslugi bledow!!\n"; exit(666); }
 listen_UI_events();
 error = pthread_join(thread,NULL);
 if (error) 
  { 
   cout<<"strasznie zly blad, komputer nadaje sie do reperacji!\n";
   exit(777);
  }
 listen_UI_events();
}


int main(int lb, char** par)
{
 if ((fd=open(fifo,O_WRONLY))<0) first_player();
 else second_player();
 remove();
}
