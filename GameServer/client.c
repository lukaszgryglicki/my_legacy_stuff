#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include "gcomdefs.h"
#define SHED 25000
#define MAX_WAIT 10
#define BACK_UPDATE 8
static int gfdr;
static int gfdw;
static pthread_t tr;
static pthread_t tw;
static int sig;
static int id;
static int game_progress;
static int timeout;		/* FIXME ALL MUTEX_WAIT ENPACK WITH timeout alarm, better than deadlock, isn't it? */
static int got_board;		/* BUT IN FINAL RELEASE ONLY, BECAUSE THIS SHOULDN'T HAPPEN */
static unsigned char* matrix;
static unsigned char* bmatrix;
static Display* dsp;
static Window win;
static GC gc;
static int Xup;
static int nmoves;
pthread_mutex_t mutex;
typedef unsigned long ulong;
typedef unsigned char uchar;
uchar pix[0x100][8][8][3];
ulong pixf[0x100][8][8];
#define FREE_MUTEX  { debug("*** MUTEX FREE %s:%d ***\n",__FILE__,__LINE__);   pthread_mutex_unlock(&mutex); }
#define WAIT_MUTEX  { alarm(timeout); debug("***  MUTEX WAIT %s:%d ***\n",__FILE__,__LINE__);  pthread_mutex_lock(&mutex); debug("**** IN CS %s:%d****\n",__FILE__,__LINE__); alarm(0); }
#define BITS16
//#define BITS24
#ifdef BITS24

unsigned long RGB(int r, int g, int b)  /*set color from r,g,b*/
{
 return ((r&0xFF)<<0X10)+((g&0XFF)<<0X8)+b;
}


int ReturnRed(ulong col)
{
 return (int)((0xff0000&col)>>0X10);
}


int ReturnGreen(ulong col)
{
 return (int)((0x00ff00&col)>>0X8);
}


int ReturnBlue(ulong col)
{
 return (int)(0x0000ff&col);
}

#endif
#ifdef BITS16

unsigned long RGB(int r, int g, int b)  /*set color from r,g,b*/
{
 return (((r>>0x03)&0x1F)<<0X0B)+(((g>>0x02)&0X3F)<<0X05)+((b>>0x03)&0x1F);
}


int ReturnRed(ulong col)
{
 return (int)(((0XF800&col)>>0XB)<<0X3);
}


int ReturnGreen(ulong col)
{
 return (int)(((0x7e0&col)>>0X5)<<0X2);
}


int ReturnBlue(ulong col)
{
 return (int)((0X1F&col)<<0X3);
}

#endif

int debug(char* fmt, ...)
{
#ifdef DEBUG
 va_list ap;
 int err;
 if (!fmt) return -1;
 va_start(ap,fmt);
 printf("debug: ");
 err = vprintf(fmt,ap);
 va_end(ap);
 fflush(stdout);
 return err;
#endif
#ifndef DEBUG
 return 0;
#endif
}


void error(char* fmt, ...)
{
 va_list lst;
 va_start(lst,fmt);
 printf("CRITICAL ERROR: \t");
 vprintf(fmt,lst);
 printf("\n\tCLIENT HALTED\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}


void warning(char* fmt, ...)
{
 va_list lst;
 va_start(lst,fmt);
 printf("***** WARNING: \n\t");
 vprintf(fmt,lst);
 printf("\n\tWARNING WAS DECLARED AS ERROR FOR DEBUGING PURPOSES.\n");
 printf("\n\n\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}


void help(char* name)
{
 printf("options are:\n");
 printf("\t-i IP_NUM (server's IP, default 127.0.0.1)\n");
 printf("\t-s SERVER_NAME (DNS needed, no default)\n");
 printf("\t-p PORT (server's port, default 10001)\n");
 printf("\t-w MAXWAIT (max wait for connection, in seconeds)\n");
 printf("\t-g GRAPHICS (default=graphics.dat)\n");
 printf("\t-h (displays this help)\n");
 printf("\t-f SOCKNAME (if local socket, tell the path)\n");
 printf("ENTER REFRESES WND, SPACE SHOTS, ARROWS MOVE\n");
 printf("W,S,A,D Shot specific direction\n");
}


void alloc_mem()
{
 matrix = (unsigned char*)malloc(0x1001*sizeof(unsigned char));
 if (!matrix) error("malloc main ptr_table");
 bmatrix = (unsigned char*)malloc(0x1001*sizeof(unsigned char));
 if (!bmatrix) error("malloc main ptr_table");
 debug("MEMORY ALLOCATED.\n");
}


void free_mem()
{
 if (!matrix) { bmatrix=0; return ; }
 if (matrix) { free(matrix); matrix=0; }
 matrix = 0;
 if (!bmatrix) return ;
 if (bmatrix) { free(bmatrix); bmatrix=0; }
 bmatrix = 0;
 debug("MEMORY FREED.\n");
}


void get_host(char* to, char* name)
{
 struct hostent* hose;
 struct in_addr addr;
 hose = gethostbyname(name);
 if (!hose)
   {
    perror("gethostbyname");
    error("Cannot get IP for hostname: %s\nis DNS running?, is Gateway IP set?\n", name);
   }
 memcpy((void*)&addr,(void*)(hose->h_addr_list[0]),sizeof(struct in_addr));
 strcpy(to, inet_ntoa(addr));
 debug("Got IP %s for hostname %s\n", to, name);
 fflush(stdout);
}


void catch_signal(int signo)		/* FIXME, CHECK IT INTENSIVELY */
{
 if (sig==1) { debug("ALREADY PROCESSING SIGNAL!\n"); sig++; FREE_MUTEX return; }
 if (sig==2) { debug("HARDCORE EXIT FORCED\n"); FREE_MUTEX exit(2); }
 sig = 1;
 WAIT_MUTEX
 if (signo==SIGALRM)
   {
    debug("alarm clock: connection timeout.\n");
    printf("UNIX client: connection timeout, can't connect to server.\n");
    if (gfdr==-1 && gfdw==-1) debug("sockets are already closed, bye!\n");
    close(gfdr);
    close(gfdw);
    gfdr=gfdw=-1;
    FREE_MUTEX
   }
 else if (signo==SIGPIPE)
   {
    debug("server has closed socket, exiting...\n");	/* not need inform_server(), server is probably dead */
    printf("UNIX client: server exited implicitlly.\n");
    if (gfdr==-1 && gfdw==-1) debug("sockets are already closed, bye!\n");
    close(gfdr);
    close(gfdw);
    gfdr=gfdw=-1;
    FREE_MUTEX
   }
 else if (signo==SIGINT)
   {
    debug("got interrupt, cleaning up...\n");
    printf("\n\nUNIX client: USER INTERRUPT, exiting\n");
    if (gfdr==-1 && gfdw==-1) debug("sockets are already closed, bye!\n");
    else
      {
       debug("informing server about interrupt...\n");
       debug("done.\n");			/* REALLY THIS IS NOT NEEDED, WHEN 4 clients INT, 3 exiting and */
       close(gfdr);				/* ALL GETS MASSIVE SIGNAL, THIS WILL PROBABLY DEADLOCK SERVER BY */
       close(gfdw);				/* READ IN CS FROM ONE CLIENT, AND WAITING ON MUTEX WITH ANOTHER CLIENT */
       gfdr=gfdw=-1;				/* AND READ WILL NOT RETURN BECAUSE CLIENT WAITS IN CS, TO GET BOARD AFTER */
      }						/* DEAD OF ANOTHER CLIENT WHO SENDS DEAD EVENT AND CAUSES SERVER TO WAIT IN MUTEX */
    debug("FINAL EXIT FROM SIGHNDLR.\n");	/* BUT IF SIGNAL EXITS CLIENT __WITHOUT__ CS, THEN SERVER CAN PASS ALL MSGS */
    free_mem();					/* WITHOUT DEADLOCK */
    exit(1);
   }
 debug("EXIT FROM SIGHNDLR.\n");
 FREE_MUTEX
 free_mem();
 exit(0);
}


void draw_field(int x, int y, int idx)
{
 register int i,j;		/* FIXME, REGISTER IS SAFE WHEN MULTITHREAD?? */
 for (i=0;i<8;i++)
 for (j=0;j<8;j++)
   {
    XSetForeground(dsp,gc,pixf[(uint)matrix[idx]][i][j]);
    XDrawPoint(dsp,win,gc, 8*x+i,8*y+j);
   }
}

void finalize();

void refresh_window()
{
 int i,j;
 debug("REFRESH WINDOW.\n");
 if (!matrix) return;
 j=0;
 while (!got_board)			/* SOMTIMES NEEDED, WHEN SERVER OVERLOADED */
   {					/* BUT THEN PROBABLY BOOM WILL APPEAR (DEADLOCK) */
    pthread_yield();
    debug("WAITING FOR BOARD DATA...\n");
    j++;		/*FIXME, DOESN'T IT MEAN THAT SERVER IS BROKEN, INFORMING IT MAY CAUSE DEADLOCK */
    			/* NOW SERVER WILL READ 0 OR -1 FROM SOCKET, AND FLUSH TRASHED CLIENTS */
    if (j==MAX_WAIT) { finalize(0); game_progress=0; exit(0); }
   }
 for (i=0;i<0x40;i++)
 for (j=0;j<0x40;j++) draw_field(i, j, i*0x40+j);
}


void refresh_fields()
{
 int i,j;
 debug("REFRESH FIELDS.\n");
 if (!matrix) return;
 j=0;
 for (i=0;i<0x40;i++)
 for (j=0;j<0x40;j++) if(matrix[i*0x40+j]==FIELD_NONE) draw_field(i, j, i*0x40+j);
}

void signal_from_thread()		/* ASYNC OF COURSE */
{					/* FIXME, EVERY ASYNC EVENT IS DANGEROUS FOR X, WHAT IS WITH THREADS AND XFREE86??*/
 int i,j,n;
 debug("PARTIAL REFRESH.\n");
 if (!matrix) return;
 n=0;
 for (i=0;i<0x40;i++)
 for (j=0;j<0x40;j++)
  {
   if (bmatrix[i*0x40+j]!=matrix[i*0x40+j]) { draw_field(i,j,i*0x40+j); n++; }
  }
 debug("REFRESHED FIELDS: %d\n", n);
 if (n==0) refresh_fields();
}


void finalize(int locked)
{
 debug("done.\n");
 if (gfdw!=-1 || gfdr!=-1)
   {
    if (locked) WAIT_MUTEX
    close(gfdr);
    close(gfdw);
    gfdw=gfdr=-1;
    if (locked) FREE_MUTEX
   }
 free_mem();
}


int error_handler(Display*d,  XErrorEvent* xer)	/*FIXME, ERRORS SHOULDN'T HAPPEN, IS X THREAD-SAFE OR NOT?? */
{
 warning("XServer - ASYNC - ERROR, ");
 warning("**** CHECK MUTEXES ****");
 warning("**** CHECK MUTEXES ****");
 warning("**** CHECK MUTEXES ****");
 return 0;
}


void set_error_handler()	/* FIXME NOT USED, BUT... */
{
 debug("SET ERROR HANDLER");
 XSetErrorHandler(error_handler);
}

void proc_dead()		/* APPEARS IN THE MIDDLE OF PROCESSING MOVE OR FIRE */
{				/* FIXME CORRECTLY ASYNC HANDLED?? */
 char buff[20];
 debug("PROC_DEAD");
 if (read(gfdw,buff,16)==-1) error("Cannot process own death.");
 if (strcmp(buff,"FINE")) error("Cannot process own death.2");
 if (write(gfdw,"FINE",5)==-1) error("Cannot process own death.3");
 if (read(gfdw,buff,16)==-1) error("Cannot process own death.4");	/* CANT, MOVE, FIRO Probably */
 debug("POSTMORTEM: %s\n", buff);
 finalize(0);
 game_progress=0;
}


void move_up()			/*FIXME, THIS AND 4 NEXT FUNCS ARE NOT GOOD TESTED */
{
 char buff[20];
 int dead;
 dead=0;
 debug("MOVE UP REQUEST.\n");
 if (write(gfdw,"MVUP",5)==-1) error("SENDING MOVEUP REQUEST TO SERVER");
 if (read(gfdw,buff,16)==-1) error("RECIEVING SERVER ANSWER TO MOVE_UP REQUEST");
 if (strcmp(buff,"MOVE") && strcmp(buff,"CANT") && strcmp(buff,"DEAD")) error("BAD SERVER ANSWER TO MOVE_UP REQUEST: %s", buff);
 if (!strcmp(buff,"CANT")) { debug("CAN'T MOVE THERE.\n"); return; }
 else if (!strcmp(buff,"DEAD")) { debug("CAN'T MOVE THERE.\n"); dead=1; }
 else debug("MOVE IS OK.\n");
 if (dead) proc_dead();
}


void move_down()
{
 char buff[20];
 int dead;
 dead=0;
 debug("MOVE DOWN REQUEST.\n");
 if (write(gfdw,"MVDO",5)==-1) error("SENDING MOVEDOWN REQUEST TO SERVER");
 debug("MV 1\n");
 if (read(gfdw,buff,16)==-1) error("RECIEVING SERVER ANSWER TO MOVE_DOWN REQUEST");
 debug("MV 2\n");
 if (strcmp(buff,"MOVE") && strcmp(buff,"CANT") && strcmp(buff,"DEAD")) error("BAD SERVER ANSWER TO MOVE_DOWN REQUEST: %s", buff);
 debug("MV 3\n");
 if (!strcmp(buff,"CANT")) { debug("CAN'T MOVE THERE.\n"); return; }
 else if (!strcmp(buff,"DEAD")) { debug("CAN'T MOVE THERE.\n"); dead=1; }
 else debug("MOVE IS OK.\n");
 debug("MV 4\n");
 if (dead) proc_dead();
 debug("MV 5\n");
}


void move_left()
{
 char buff[20];
 int dead;			/* FIXME NOT NEEDED REALLY */
 dead=0;
 debug("MOVE LEFT REQUEST.\n");
 if (write(gfdw,"MVLE",5)==-1) error("SENDING MOVELEFT REQUEST TO SERVER");
 if (read(gfdw,buff,16)==-1) error("RECIEVING SERVER ANSWER TO MOVE_LEFT REQUEST");
 if (strcmp(buff,"MOVE") && strcmp(buff,"CANT") && strcmp(buff,"DEAD")) error("BAD SERVER ANSWER TO MOVE_LEFT REQUEST: %s", buff);
 if (!strcmp(buff,"CANT")) { debug("CAN'T MOVE THERE.\n"); return; }
 else if (!strcmp(buff,"DEAD")) { debug("CAN'T MOVE THERE.\n"); dead=1; }
 else debug("MOVE IS OK.\n");
 if (dead) proc_dead();
}


void move_right()
{
 char buff[20];
 int dead;
 dead=0;
 debug("MOVE RIGHT REQUEST.\n");
 if (write(gfdw,"MVRI",5)==-1) error("SENDING MOVERIGHT REQUEST TO SERVER");
 if (read(gfdw,buff,16)==-1) error("RECIEVING SERVER ANSWER TO MOVE_RIGHT REQUEST");
 if (strcmp(buff,"MOVE") && strcmp(buff,"CANT") && strcmp(buff,"DEAD")) error("BAD SERVER ANSWER TO MOVE_RIGHT REQUEST: %s", buff);
 if (!strcmp(buff,"CANT")) { debug("CAN'T MOVE THERE.\n"); return; }
 else if (!strcmp(buff,"DEAD")) { debug("CAN'T MOVE THERE.\n"); dead=1; }
 else debug("MOVE IS OK.\n");
 if (dead) proc_dead();
}


void fire_gun(int where)
{
 char buff[20];
 int dead;
 dead=0;
 debug("FIRE REQUEST.\n");
 if (where==FIRE_ALL)        { if (write(gfdw,"FIRE",5)==-1) error("SENDING FIRE REQUEST TO SERVER"); }
 else if (where==FIRE_UP)    { if (write(gfdw,"FIRU",5)==-1) error("SENDING FIRU REQUEST TO SERVER"); }
 else if (where==FIRE_DOWN)  { if (write(gfdw,"FIRD",5)==-1) error("SENDING FIRD REQUEST TO SERVER"); }
 else if (where==FIRE_LEFT)  { if (write(gfdw,"FIRL",5)==-1) error("SENDING FIRL REQUEST TO SERVER"); }
 else if (where==FIRE_RIGHT) { if (write(gfdw,"FIRR",5)==-1) error("SENDING FIRD REQUEST TO SERVER"); }
 else { debug("Unknown direction.\n"); return; }
 if (read(gfdw,buff,16)==-1) error("RECIEVING SERVER ANSWER TO FIRE REQUEST");
 if (strcmp(buff,"FIRO") && strcmp(buff,"DEAD") && strcmp(buff,"CANT")) error("BAD SERVER ANSWER TO FIRE: %s", buff);
 if (!strcmp(buff,"DEAD")) { debug("CAN'T FIRE THERE.\n"); dead=1; }
 debug("FIRED.\n");
 if (dead) proc_dead();
}


void X()		/* WRITER THREAD PROCESSES X EVENTS */
{
 int s_num;
 int dx,dy;
 int nkeyb;
 int nasync;
 XEvent an_event;
 if ((dsp=XOpenDisplay(NULL))==NULL) error("connect to X");
 s_num = DefaultScreen(dsp);
 dx = DisplayWidth(dsp, s_num);
 dy = DisplayHeight(dsp, s_num);
 win = XCreateSimpleWindow(dsp, RootWindow(dsp, s_num),0, 0, 0x200, 0x200, 1,WhitePixel(dsp, s_num),BlackPixel(dsp, s_num));
 XMapWindow(dsp, win);
 XFlush(dsp);
 gc = XCreateGC(dsp, win, 0, NULL);
 if ((int)gc<0) error("create GC");
 XSetForeground(dsp, gc, WhitePixel(dsp, s_num));
 XSetBackground(dsp, gc, BlackPixel(dsp, s_num));
 XSelectInput(dsp, win, ExposureMask | KeyPressMask | StructureNotifyMask);
 nkeyb=0;
 nasync=0;
 set_error_handler();
 refresh_window();
 printf("UNIX client: game started.\n");
/* XAutoRepeatOff(dsp);*/
 Xup=1;
 while (game_progress)
    {
     XNextEvent(dsp, &an_event);
     WAIT_MUTEX				/* FIXME THIS MUTEX IS NEEDED TO AVOID X ERRORS, HMM... ? */
     switch (an_event.type)
        {
         case Expose:
	     signal_from_thread();
	     if (an_event.xexpose.send_event)
	       {
		debug("SEND BY SIGNAL!!!\n");
		nasync++;
		signal_from_thread();
		//if (nasync>=BACK_UPDATE) { nasync=0; refresh_fields(); }
	       }
	     else refresh_window();
             break;
        case ConfigureNotify:
             XResizeWindow(dsp,win,0x200,0x200);
	     refresh_window();
             break;
        case KeyPress:
	     if (sig) break;
             debug("keyboard!\n");
	     if (an_event.xkey.keycode == KEY_Q)     { finalize(0); game_progress=0; }
	     else if (an_event.xkey.keycode == KEY_UP)    { move_up();    nkeyb++; }
	     else if (an_event.xkey.keycode == KEY_DOWN)  { move_down();  nkeyb++; }
	     else if (an_event.xkey.keycode == KEY_LEFT)  { move_left();  nkeyb++; }
	     else if (an_event.xkey.keycode == KEY_RIGHT) { move_right(); nkeyb++; }
	     else if (an_event.xkey.keycode == KEY_SPC)   fire_gun(FIRE_ALL);
	     else if (an_event.xkey.keycode == KEY_W)     fire_gun(FIRE_UP);
	     else if (an_event.xkey.keycode == KEY_S)     fire_gun(FIRE_DOWN);
	     else if (an_event.xkey.keycode == KEY_A)     fire_gun(FIRE_LEFT);
	     else if (an_event.xkey.keycode == KEY_D)     fire_gun(FIRE_RIGHT);
	     else if (an_event.xkey.keycode == KEY_CR)    { XClearWindow(dsp,win); refresh_window(); }
	     else debug("UNKNOWN KEYBOARD ENTRY.\n");
	     nmoves++;
	     //if (nkeyb>=BACK_UPDATE) { nkeyb=0; refresh_fields(); }
/*	     if (nmoves<5) refresh_fields();*/
	     debug("NMOVES=%d\n", nmoves);
             break;
        default:
             break;
        }
     FREE_MUTEX
     pthread_yield();
   }
}


void* writer_thread(void* ptr)
{
 int fd = (int)ptr;
 debug("C_WRITER %d\n", fd);
 while (!got_board) pthread_yield();
 X();					/* HEH, WRITER IS GUI ITSELF */
 debug("EXIT WRITER THREAD.\n");
 return NULL;
}


void exit_game(int fd, int locked)	/* FIXME write WAS ENPACKED BY MUTEX, DON'T NEED IT (PROBABLY)*/
{
 char buff[20];
 sig=1;
 debug("EXIT GAME.1\n");
 if (locked) WAIT_MUTEX
 if (write(fd,"EXIT",5)==-1) error("exiting1");
 if (read(fd,buff,16)==-1) error("exiting2");
 if (write(fd,"FINE",5)==-1) error("exiting3");
 if (locked) FREE_MUTEX
 debug("EXIT GAME.2\n");
 if (pthread_cancel(tw)) error("stopping writer thread");
 if (pthread_cancel(tr)) error("stopping reader thread");
 debug("EXIT GAME, ON SERVER REQUEST\n");
 sig=0;
}


void no_room(int fd)	/* FIXME THERE WAS MUTEX, DONT NEED IT? */
{
 char buff[20];
 sig=1;
 debug("SERVER HAS NO ROOM, WE ARE BANNED!\n");
 WAIT_MUTEX
 if (write(fd,"BANE",5)==-1) error("exiting bane");
 if (read(fd,buff,16)==-1) error("exiting bane2");
 if (write(fd,"FINE",5)==-1) error("exiting bane3");
 FREE_MUTEX
 debug("EXIT GAME (BANNED).\n");
 if (pthread_cancel(tw)) error("stopping writer thread, bane");
 if (pthread_cancel(tr)) error("stopping reader thread, bane");
 debug("EXIT - NO ROOM\n");
}


void get_id(int fd)	/* FIXME MUTEX NEEDED, WHEN ASYNC EXIT OR SIGNAL?? */
{
 sig=1;
 debug("GETTING ID\n");
 WAIT_MUTEX		/* FIXME PROBABLY NOT NEEDED */
 if (write(fd,"IDIS",5)==-1) error("sending id query");
 if (read(fd,(void*)(&id),sizeof(int))==-1) error("recieving id from server");
 if (write(fd,"IDOK",5)==-1) error("sending id confirm");
 FREE_MUTEX
 debug("GOT ID FROM SERVER: %d\n", id);
 sig = 0;
}


void send_signal()
{
 debug("X already UP & RUNNING, sending signal...\n");
 XEvent an_event;
 int err;
 an_event.xclient.type = Expose;	/* FIXME FIXME, AM I OK, SENDING SIGNAL JUST THAT?????? */
 debug("TEST SIGNAL\n");
 if (!Xup) { warning("cannot send event, X Window not created yet.\n"); return ; }
 err=XSendEvent(dsp,win,False,0,&an_event);
 XFlush(dsp);				/* FIXME WHY THIS IS NEEDED, HEH??? */
}

void get_board(int fd)		/* VERY FIXME, MUTEX WHEN GETTING TO AVOID SIGNAL THERE AND INTERRUPT GETTING */
{				/* WHICH COULD CONFUSE SERVER, NOW IN CTRL+C HNDL AND QUIT 'Q' */
 int nread,i,total_read;	/* WHICH WAITS ON MUTEX */
 sig=1;				/* GENERAL, WHEN ERROR WHILE CONVERSATION WITH SERVER */
 debug("GETTING BOARD\n");	/* FINISH CONVERSATION AND __THEN__ EXIT */
 for (i=0;i<0x1000;i++) bmatrix[i] = matrix[i];
/* WAIT_MUTEX*/			/* CAUSED MUTEX/READ_SOCKET DEADLOCK */
 if (write(fd,"BOIS",5)==-1) error("sending board query");	/* FIXME LIKE THERE */
 nread=0;
 total_read=0;
 while (total_read<4096)
 {
  if ((nread=read(fd,(matrix+total_read),4096))==-1) error("recieving board from server");
  total_read += nread;						/* FIXME AND THERE */
 }
 nread = total_read;
 matrix[0x1000] = 0;
 debug("RECIEVED %d BOARD BYTES.\n", nread);
 if (nread<0x1000) error("RECIEVED NOT ENOUGH BYTES, CONNECTION IS BAD OR SERVER OVERLOADED:\n%s",matrix);
 if (write(fd,"BOOK",5)==-1) error("sending id confirm");	/* FIXME BUT MAYBE ANSWER OK TO SERVER NOT TO TEASE IT  (ABOVE LINE) */
 WAIT_MUTEX
 debug("GOT BOARD FROM SERVER\n");
 if (Xup) send_signal();	/*FIXM NEEDE MUTEX THERE ? */
 else debug("X-Window not created yet.\n");
 FREE_MUTEX
 sig = 0;
 got_board=1;
}


void exit_game_server_down()
{
 sig=1;
 if (pthread_cancel(tw)) error("stopping writer thread, when server dead");
 if (pthread_cancel(tr)) error("stopping reader thread, when server dead");
 debug("EXIT GAME, SERVER SHUTDOWN\n");
}


void proc_dead_rd(int fd)
{
 char buff[20];
 debug("PROC_DEAD_RD\n");
 if (write(gfdr,"FINE",5)==-1) error("Cannot process own death.3");
 debug("PROC_DEAD_RD.2: %s\n");
 if (read(gfdr,buff,16)==-1) error("Cannot process own death.4");	/* CANT, MOVE, FIRO Probably */
 debug("POSTMORTEM: %s\n", buff);
 if (pthread_cancel(tw)) error("stopping writer thread");
 if (pthread_cancel(tr)) error("stopping reader thread");
 debug("EXIT GAME, ON SERVER REQUEST\n");
 debug("WE ARE KILLED!\nFINAL EXIT!\n\n\n\n");
 sig=0;
}


void* reader_thread(void* ptr)
{
 char buff[20];
 int fd = (int)ptr;
 int nread;
 debug("C_READER %d\n", fd);
 while (1)
   {
    strcpy(buff,"");
    while (sig) pthread_yield();
    if ((nread=read(fd,buff,5))==-1) if (errno!=EINTR) { debug("CLIENT_READ_THREAD FINISHED\n"); break; }
    debug("CR: %s\n", buff);
    if (nread<=0) { exit_game_server_down(); break; }
    if (!strcmp(buff,"KILL")) { exit_game(fd, 0); break; }
    if (!strcmp(buff,"DEAD")) { printf("UNIX client: WE're DEAD.\n"); proc_dead_rd(fd); break; }
    if (!strcmp(buff,"BANE")) { no_room(fd); break; }
    if (!strcmp(buff,"IDCL")) get_id(fd);
    if (!strcmp(buff,"BOAR")) get_board(fd);	/* FIXME ASYNC SIGNAL TO SERVER, CHECK CAREFULLY, MUTEXES ETC */
   }
 debug("EXIT READER THREAD.\n");
 game_progress=0;
 finalize(0);			/* FIXME OK THERE? */
 return NULL;
}

void init_matrixes();

void play_game(int fdr, int fdw)
{
 tr = tw = (pthread_t)(0);
 alloc_mem();
 init_matrixes();
 game_progress=1;
 if (pthread_create(&tr, NULL, reader_thread, (void*)fdr)) error("pthread_createR");
 if (pthread_create(&tw, NULL, writer_thread, (void*)fdw)) error("pthread_createW");
 debug("WAIT FOR FINISH R/W THREADS.\n");	/* WHEN ALL OK 2 NEXT JOIN PASSES AND CLIENT EXITS */
 if (pthread_join(tw,NULL)) error("waiting for writer thread");
 if (pthread_join(tr,NULL)) error("waiting for reader thread");
 game_progress = 0;
 FREE_MUTEX
 printf("UNIX client: GAME FINISHED.\n");
}


void init_mutex()
{
 pthread_mutexattr_t mutex_attr;
 if (pthread_mutexattr_init(&mutex_attr)) error("creating MUTEX attributes.\n");
 if (pthread_mutex_init(&mutex, (const pthread_mutexattr_t*)(&mutex_attr))) error("createing MUTEX");
}


void transform_pixels()
{
 int i,j,k;
 debug("TRANSFORMING PIXEL DATA...\n");
 for (i=0;i<0x100;i++)
 for (j=0;j<0x8;j++)
 for (k=0;k<0x8;k++) pixf[i][j][k] = RGB(pix[i][j][k][0],pix[i][j][k][1],pix[i][j][k][2]);
}


void load_pixels(char* fn)
{
 FILE* dat;
 int i,j,k;
 int tmp,tmp2;
 char str[0x100];
 dat = fopen(fn,"r");
 if (!dat) error("cannot read DATAFILE: %s", fn);
 for (i=0;i<0x100;i++)
  {
   tmp = fscanf(dat,"%s %02x\n", str,(uint*)(&tmp2));
   if (tmp!=2) error("bad DATAFILE format: %s", fn);
   if (strcmp(str,"FIELD")) error("bad DATAFILE format: %s", fn);
   for (j=0;j<8;j++)
     {
      for (k=0;k<8;k++) if (fscanf(dat,"(%02x,%02x,%02x) ", (uint*)(&pix[i][j][k][0]), (uint*)(&pix[i][j][k][1]), (uint*)(&pix[i][j][k][2]))!=3)
         error("bad DATAFILE format: %s", fn);
      fscanf(dat,"\n");
     }
  }
 fclose(dat);
}


void init_matrixes()
{
 int i;
 for (i=0;i<0x1000;i++) matrix[i] = bmatrix[i] = FIELD_NONE;
}


void client(char* ipstr, int port, int secs, char* graphics, char* sockf)
{
 int sock,i,err,local;
 static struct sockaddr_in server;
 static struct sockaddr_un serverl;
 static struct sigaction act;
 printf("UNIX clinet: initializing game...\n");
 if (port<1 || port>0xffff) error("port: %d is invalid", port);
 if (secs<0 || secs>0xffff) error("seconds: %d is bad value", secs);
 if (port<1024) debug("warning: port: %d is less than 1024 and can be OS-protected.\n", port);
 local = 0;
 if (strcmp(sockf,"")) local = 1;
 load_pixels(graphics);
 transform_pixels();
 if (!local)
   {
    server.sin_family = AF_INET;
    server.sin_port = (in_port_t)(port);
    server.sin_addr.s_addr = inet_addr(ipstr);
   }
 else
   {
    serverl.sun_family = AF_UNIX;
    strncpy(serverl.sun_path, sockf, MAX_SOCK_PATH);
   }
 act.sa_handler = catch_signal;
 sigfillset(&(act.sa_mask));
 sigaction(SIGALRM, &act, NULL);
 sigaction(SIGPIPE, &act, NULL);
 sigaction(SIGINT, &act, NULL);
 if (server.sin_addr.s_addr==(in_addr_t)(-1)) error("IP %s is invalid.\n", ipstr);
 debug("Connecting to %s, at port %d\n", ipstr, port);
 init_mutex();
 nmoves = 0;
 gfdr = -1;
 gfdw = -1;
 id   = -1;
 sig  =  0;
 got_board = 0;
 Xup=0;
 timeout=secs;
 if (!local) sock = socket(AF_INET, SOCK_STREAM, 0);
 else        sock = socket(AF_UNIX, SOCK_STREAM, 0);
 if (sock==-1) { perror("socket"); error("create socket failed."); }
 err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &i, sizeof(void*));
 if (err==-1)  { perror("setsockopt"); error("set socket options"); }
 if (secs) alarm(secs);
 if (!local) err=connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in));
 else        err=connect(sock,(struct sockaddr*)&serverl, sizeof(struct sockaddr_un));
 if (err==-1)
   { perror("connect"); error("connect to %s:%d failed",ipstr,port); }
 gfdr=sock;
 if (secs) alarm(0);
 debug("connect ok...\n");
 if (!local) sock = socket(AF_INET, SOCK_STREAM, 0);
 else        sock = socket(AF_UNIX, SOCK_STREAM, 0);
 if (sock==-1) { perror("socket"); error("create socket failed."); }
 err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &i, sizeof(void*));
 if (err==-1)  { perror("setsockopt"); error("set socket options"); }
 if (secs) alarm(secs);
 if (!local) err=connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in));
 else        err=connect(sock,(struct sockaddr*)&serverl, sizeof(struct sockaddr_un));
 if (err==-1)
   { perror("connect"); error("connect to %s:%d failed",ipstr,port); }
 gfdw=sock;
 if (secs) alarm(0);
 debug("connect ok...\n");
 printf("UNIX clinet: connected.\n");
 play_game(gfdr, gfdw);
 debug("CLOSE SOCKETS.\n");
 close(gfdr);
 close(gfdw);
 debug("CLOSE SOCKETS DONE.\n");
 debug("FREE MEM\n");
 free_mem();
 debug("CLIENT EXITED.\n");
}


int main(int lb, char** par)
{
 char u;
 char ip[16];
 char port[12];
 char secs[12];
 char graphics[1024];
 char sockf[1024];
 strcpy(ip,"127.0.0.1");
 strcpy(port,"10001");
 strcpy(secs,"10");
 strcpy(graphics,"graphics.dat");
 strcpy(sockf,"");
 while ((u = getopt(lb,par,"i:s:w:p:g:f:h"))!=-1)
 {
  switch (u)
   {
    case 'i': if (strlen(optarg)<16)   strcpy(ip, optarg);       break;
    case 's': get_host(ip, optarg);                              break;
    case 'p': if (strlen(optarg)<12)   strcpy(port, optarg);     break;
    case 'g': if (strlen(optarg)<1024) strcpy(graphics, optarg); break;
    case 'f': if (strlen(optarg)<1024) strcpy(sockf, optarg);    break;
    case 'w': if (strlen(optarg)<12)   strcpy(secs, optarg);     break;
    case 'h': help(par[0]); return 0;
    default: debug("Unrecognized option\n"); return 1;
   }
 }
 client(ip,atoi(port), atoi(secs), graphics, sockf);
 debug("MAIN EXITED.\n");
 return 0;
}

