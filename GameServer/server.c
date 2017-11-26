/* Written by MorgothDBMA: morgothdbma@o2.pl, tel +48693582014 */
/* license: BSD */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include "gcomdefs.h"
#include "mrandom.h"
#define SHED 25000

struct ThreadData
{
 pthread_t *thr, *thw;
 int fdr, fdw;
 int id;
 struct ThreadData* next;
};
typedef unsigned char uchar;
unsigned char* matrix;
int nclients;
struct ThreadData* clients;
static int shutdwn;
static int sig;
static int timeout;
static int main_sock;
char* sock_local;
int loc;
pthread_mutex_t mutex;
#define FREE_MUTEX  { debug("*** MUTEX FREE %s:%d ***\n",__FILE__,__LINE__);   pthread_mutex_unlock(&mutex); }
#define WAIT_MUTEX  { alarm(timeout); debug("***  MUTEX WAIT %s:%d ***\n",__FILE__,__LINE__);  pthread_mutex_lock(&mutex); debug("**** IN CS %s:%d****\n",__FILE__,__LINE__); alarm(0); }
#define TRYL_MUTEX  { debug("*** TRY LOCK MUTEX %s:%d >> %d ***\n",__FILE__,__LINE__,pthread_mutex_trylock(&mutex)); }
#define TRY_LOCK    pthread_mutex_trylock(&mutex);
void broadcast_board(int);

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


void error(char* fmt, ...)	/* debug error msg and exit */
{
 va_list lst;
 va_start(lst,fmt);
 printf("CRITICAL ERROR: ");
 vprintf(fmt,lst);
 printf("\nSERVER HALTED.\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}


void warning(char* fmt, ...)	/* debug error msg and exit */
{
 va_list lst;
 va_start(lst,fmt);
 printf("WARNING: ");
 vprintf(fmt,lst);
 printf("\n\tWARNING WAS DECLARED AS ERROR FOR DEBUGING PURPOSES.\n");
 fflush(stdout);
 exit(1);
}


void add_client(pthread_t* tr, pthread_t* tw, int fdr, int fdw, int id)
{
 struct ThreadData* newh;
 if (!clients)
   {
    clients = (struct ThreadData*)malloc(sizeof(struct ThreadData));
    if (!clients) { error("malloc clients failed"); return; }
    clients->thr  = tr;
    clients->thw  = tw;
    clients->fdr  = fdr;
    clients->fdw  = fdw;
    clients->id   = id;
    clients->next = NULL;
    return;
   }
 newh = (struct ThreadData*)malloc(sizeof(struct ThreadData));
 if (!newh) { error("malloc clients failed"); return; }
 newh->thr  = tr;
 newh->thw  = tw;
 newh->fdr  = fdr;
 newh->fdw  = fdw;
 newh->id   = id;
 newh->next = clients;
 clients    = newh;
 debug("CLIENT ADDED.\n");
}


void help(char* name)		/* usage help */
{
 printf("options are:\n");
 printf("\t-p PORTNUM (which port use, default 10001)\n");
 printf("\t-m MAXCONN (how much maximum pending connections, default 5)\n");
 printf("\t-w WAIT (how much seconds wait for resolving DEADLOCK, default INF)\n");
 printf("\t-l LEVELNAME (default=game.dat)\n");
 printf("\t-c CLIENTS (maximum clients, default=6)\n");
 printf("\t-h (prints this help msg.)\n");
 printf("]t-s SOCKNAME (use local FS socket, nodefault)\n");
}


void alloc_mem()
{
 matrix = (unsigned char*)malloc(0x1001*sizeof(unsigned char));
 if (!matrix) error("malloc main ptr_table");
 debug("MEMORY ALLOCATED.\n");
}


void free_mem()			/* SHOULD BE SYNC TOO (BY MUTEX)*/
{
 if (!matrix) return ;
 if (matrix) { free(matrix); matrix=0; }
 matrix = 0;
 if (loc || sock_local) free(sock_local);
 sock_local = 0;
 debug("MEMORY FREED.\n");
}


int shutdown_clients(int locked)
{
 struct ThreadData *td,*prv;
 char buff[20];
 pthread_t self;
 if (shutdwn) return 0;
 if (loc) unlink(sock_local);
 shutdwn=1;
 self = pthread_self();
 td = prv = clients;
 printf("\n\n\nSERVER:\tUSER INTERRUPT: SHUTDOWN\n\n\n");
 if (locked) WAIT_MUTEX
 while (td)
   {
    prv = td;
    td = td->next;
    if (!pthread_equal(self, *(prv->thr))) if (pthread_cancel(*(prv->thr))) error("stopping reader thread");
    if (!pthread_equal(self, *(prv->thw))) if (pthread_cancel(*(prv->thw))) error("stopping writer thread");
    if (write(prv->fdw,"KILL",5)==-1)   { debug("exiting client write\n"); if (locked) FREE_MUTEX return 1; }
    if (read(prv->fdw,buff,16)==-1)     { debug("exiting client read\n"); if (locked) FREE_MUTEX return 1; }
    if (strcmp(buff,"EXIT"))            { debug("bad client answer\n"); if (locked) FREE_MUTEX return 1; }
    if (write(prv->fdw,"FINE",5)==-1)   { debug("exiting client write2\n"); if (locked) FREE_MUTEX return 1; }
    if (read(prv->fdw,buff,16)==-1)     { debug("exiting client read2\n"); if (locked) FREE_MUTEX return 1; }
    if (strcmp(buff,"FINE"))            { debug("bad client answer2\n"); if (locked) FREE_MUTEX return 1; }
    if (close(prv->fdw)==-1) error("closing Wsocket");
    if (close(prv->fdr)==-1) error("closing Rsocket");
    free(prv);
    prv = NULL;
    debug("CLIENT STOPPED.\n");
   }
 clients = NULL;
 if (locked) FREE_MUTEX
 debug("FULL SHUTDOWN COMPLETE.\n");
 return 1;
}


void create_socket_list(int** tab, int* siz, int locked)
{
 struct ThreadData* td;
 int j;
 debug("CREATE SOCKET_LIST.\n");
 if (locked) WAIT_MUTEX
 td = clients;
 *siz = 0;
 *tab = 0;
 if (!clients) return;
 while (td)
  {
   (*siz)+=2;
   td = td->next;
  }
 (*siz)++;
 debug("SOCKETS: %d\n", *siz);
 *tab = (int*)malloc(((*siz)+2)*sizeof(int));
 if (!(*tab)) error("malloc socket_list failed.\n");
 td = clients;
 j=0;
 while (td)
  {
   (*tab)[j]   = td->fdr;
   (*tab)[j+1] = td->fdw;
   j+=2;
   td = td->next;
  }
 (*tab)[j] = main_sock;
 if (locked) FREE_MUTEX
}


void close_socket_list(int* tab, int siz, int locked)
{
 int i;
 debug("CLOSE SOCKET LIST.\n");
 if (locked) WAIT_MUTEX
 for (i=0;i<siz;i++) close(tab[i]);
 if (tab) free(tab);
 sync();
 if (locked) FREE_MUTEX
}


void catch_int(int signo)	/* catch signals */
{
 int *sockl,nsock;
 if (sig==1) { debug("ALREADY PROCESSING SIGNAL!\n"); sig++; return; }
 if (sig==2) { debug("VERY HARD RESET.\n"); exit(2); }
 sockl=0;
 nsock=0;
 sig = 1;
 debug("CATCH SIGNAL.\n");
 WAIT_MUTEX
 if (signo==SIGINT)
   {
    debug("Shutting dow server, interrupt occured.\n");
    create_socket_list(&sockl, &nsock, 0);
    if (!shutdown_clients(0)) debug("SHUTDOWN IN PROGRESS!\nDOING HARD RESET!!!");
    debug("done.\n");
    debug("SIGHNDLR INT DONE.\n");
   }
 else if (signo==SIGPIPE)		/* FIXME, never tested */
   {
    debug("Shutting dow server, broken pipe on socket\n");
    create_socket_list(&sockl, &nsock, 0);
    if (!shutdown_clients(0)) debug("SHUTDOWN IN PROGRESS!\nDOING HARD RESET!!!");
    debug("done.\n");
    debug("SIGHNDLR PIPE DONE.\n");
   }
 else if (signo==SIGALRM)
   {
    printf("\n\niSERVER:\tRESOLVING DEADLOCK\n\n");
    return ;
   }
 free_mem();
 kill_random();
 if (close(main_sock)==-1) error("closing main server socket");
 else debug("MAIN SERVER SOCKET CLOSED.\n");
 debug("EXIT SIGHNDLR.\n");
 sig = 0;
 close_socket_list(sockl, nsock, 0);
 FREE_MUTEX
 exit(0);
}


void init_game(char* dat)
{
 FILE* df;
 int i,z;
 df = fopen(dat,"r");
 if (!df) error("cannot open datafile: %s", dat);
 alloc_mem();
 for (i=0;i<0x1000;i++)
   {
    z = fgetc(df);
    if (z==EOF) error("EOF while expecting data");
    matrix[i] = (unsigned char)z;
   }
 matrix[0x1000] = 0;
 fclose(df);
 z = 0;
 for (i=0;i<0x1000;i++)
   {
    if (matrix[i]<FIELD_NONE) error("board contains special RUNTIME ONLY units, <0x21, IDX=%04x",i);
    if (matrix[i]==FIELD_NONE) z++;
   }
 if (z<2) error("NON ENOUGH FREE ROOM ONBOARD, LESS THAN 2\n");
 debug("INIT GAME COMPLETE: %d\n", strlen((char*)matrix));
}


void* writer_thread(void* ptr)		/* FIXME, ADD PING/PONG HERE IF ALL SYNCS OK */
{
 int fd = (int)ptr;
 debug("S_WRITER: %d\n", fd);
 while (1)
   {
    sleep(1);
/*    debug("Alive connection to: %d\n", fd);*/
   }
 debug("WRITER THREAD DONE.\n");
 return NULL;
}


void finish_threads(struct ThreadData* td)	/* FIXME IS OK IGNORING ERRORS */
{
 pthread_t self;
 char buff[20];
 sig = 1;
 self = pthread_self();
 if (!pthread_equal(self, *(td->thr))) if (pthread_cancel(*(td->thr))) error("stopping reader when client exited");
 if (!pthread_equal(self, *(td->thw))) if (pthread_cancel(*(td->thw))) error("stopping writer when client exited");
 if (write(td->fdr,"DONE",5)==-1) { debug("response to client exit1\n");  goto skip_err; }
 if (read(td->fdr,buff,16)==-1)   { debug("response to client exit2\n"); goto skip_err; }
 if (strcmp(buff,"FINE"))         { debug("response to client exit3\n"); goto skip_err; }
 if (write(td->fdr,"FINE",5)==-1) { debug("response to client exit4\n"); goto skip_err; }
 skip_err:
 if (close(td->fdr)==-1)          debug("closing client's Rsocketi\n");
 else debug("CLIENT SOCKETR CLOSED.\n");
 if (close(td->fdw)==-1) debug("closing client's Wsocket\n");
 else debug("CLIENT SOCKETW CLOSED.\n");
 debug("CLIENT REMOVED. FINISH THREADS DONE.\n");
 nclients--;
 sig = 0;
}


void finish_threads_dead(struct ThreadData* td)
{
 pthread_t self;
 sig = 1;
 self = pthread_self();
 if (!pthread_equal(self, *(td->thr))) if (pthread_cancel(*(td->thr))) error("stopping reader when client exited");
 if (!pthread_equal(self, *(td->thw))) if (pthread_cancel(*(td->thw))) error("stopping writer when client exited");
 if (close(td->fdr)==-1) error("closing client's Rsocket");
 else debug("CLIENT SOCKETR CLOSED.\n");
 if (close(td->fdw)==-1) error("closing client's Wsocket");
 else debug("CLIENT SOCKETW CLOSED.\n");
 debug("CLIENT REMOVED. FINISH THREADS DONE.\n");
 nclients--;
 sig = 0;
}


void remove_id(int id)
{
 int i,j;
 debug("REMOVE ID: %d\n", id);
 j=0;
 for (i=0;i<0x1000;i++) if (matrix[i]==id) { matrix[i] = FIELD_NONE; j++; }
 if (j>1) error("THERE WERE MORE THAN ONE(%d) CLIENT WITH ID: %d", j,id);
}


void remove_from_boardR(int fdr, int locked)
{
 struct ThreadData* td;
 debug("REMOVE FROM BOARDR.\n");
 td = clients;
 if (locked) WAIT_MUTEX
 while (td)
   {
    if (td->fdr==fdr) remove_id(td->id);
    td = td->next;
   }
 if (locked) FREE_MUTEX
}


void exit_client_dead(int fdr)	/* FIXME IS THAT FUNCTION SAFE: LOOKS LIKE IT IS ;-) */
{
 struct ThreadData *tmp,*prev;
 tmp = prev = clients;
 sig = 1;
 debug("EXIT CLIENT DEAD.\n");
 WAIT_MUTEX
 while (tmp)
   {
    if (tmp->fdr==fdr)
      {
       if (tmp==clients)
         {
	  remove_from_boardR(fdr,0);
          clients = tmp->next;
	  finish_threads_dead(tmp);
	  free(tmp);
	  tmp = NULL;
	  debug("EXIT CLIENT ROOT DONE.\n");
	  FREE_MUTEX
	  sig = 0;
	  return ;
         }
       remove_from_boardR(fdr,0);
       prev->next = tmp->next;
       finish_threads_dead(tmp);
       free(tmp);
       tmp = NULL;
       debug("EXIT CLIENT NONROOT DONE.\n");
       FREE_MUTEX
       sig = 0;
       return ;
      }
    prev = tmp;
    tmp = tmp->next;
   }
 error("exit_client_dead: No such client");
}


void exit_client(int fdr)	/* FIXME IS THAT FUNCTION SAFE: LOOK LIKE IT IS ;-) */
{
 struct ThreadData *tmp,*prev;
 tmp = prev = clients;
 sig = 1;
 WAIT_MUTEX
 while (tmp)
   {
    if (tmp->fdr==fdr)
      {
       if (tmp==clients)
         {
	  remove_from_boardR(fdr,0);
          clients = tmp->next;
	  finish_threads(tmp);
	  free(tmp);
	  tmp = NULL;
	  debug("EXIT CLIENT ROOT DONE.\n");
	  FREE_MUTEX
	  sig = 0;
	  return ;
         }
       remove_from_boardR(fdr,0);
       prev->next = tmp->next;
       finish_threads(tmp);
       free(tmp);
       tmp = NULL;
       debug("EXIT CLIENT NONROOT DONE.\n");
       FREE_MUTEX
       sig = 0;
       return ;
      }
    prev = tmp;
    tmp = tmp->next;
   }
 error("exit_client: No such client");
}


void player_hit(int id)
{
 debug("PLAYER HIT.\n");
 struct ThreadData *td;
 char buff[20];
 int found;
 td = clients;
 found = 0;
 while (td)
   {
    if (td->id == id)
      {
	      /* FIXME, ASYNC PROCESSING MOVE OR FIRE ON CLINT'S SIDE */
	      /* APPEARS TO BE OK */
       debug("WRITE DEAD.1\n");
       if (write(td->fdw,"DEAD",5)==-1) error("writing DEAD message to client.");
       debug("WRITE DEAD.2\n");
       if (read(td->fdw,buff,16)==-1) error("writing DEAD message to client.2");
       debug("WRITE DEAD.3\n");
       if (strcmp(buff,"FINE")) error("writing DEAD message to client.3");
       debug("WRITE DEAD.4\n");
       if (write(td->fdw,"FINE",5)==-1) error("writing DEAD message to client.4");
       debug("WRITE DEAD.5\n");
       			/* INNSIDE PROCESSING MOVE_X || FIRE, ON CLIENTS SIDE */
       found = 1;
       break;
      }
    td = td->next;
   }
 if (!found) error("PLAYER HIT, BUT CAN BE FOUND, LEFT THE GAME?");
}


int fire_type(int fire)
{
 debug("FIRE TYPE: %x\n", fire);
 if (fire>=FIELD_FLEFT && fire<=FIELD_FUP) return FTYPE_NONE;
 else if (fire>=FIELD_WATER_FLEFT && fire<=FIELD_WATER_FUP) return FTYPE_WATER;
 else if (fire>=FIELD_GRASS_FLEFT && fire<=FIELD_GRASS_FUP) return FTYPE_GRASS;
 else return FTYPE_ERROR;
}


int direction(int fire)
{
 debug("direction fire: %x\n", fire);
 if (fire>=FIELD_FLEFT && fire<=FIELD_FUP) return fire-FIELD_FLEFT;
 else if (fire>=FIELD_WATER_FLEFT && fire<=FIELD_WATER_FUP) return fire-FIELD_WATER_FLEFT;
 else if (fire>=FIELD_GRASS_FLEFT && fire<=FIELD_GRASS_FUP) return fire-FIELD_GRASS_FLEFT;
 else return 0;		/* FIXME just left is returned */
}


int fire_root(int fire)
{
 debug("fire_root: %x\n", fire);
 if (fire>=FIELD_FLEFT && fire<=FIELD_FUP) return FIELD_NONE;
 else if (fire>=FIELD_WATER_FLEFT && fire<=FIELD_WATER_FUP) return FIELD_WATER;
 else if (fire>=FIELD_GRASS_FLEFT && fire<=FIELD_GRASS_FUP) return FIELD_GRASS;
 else return FIELD_NONE;
}


void field_hit(int idx, uchar* bmatrix, int what)
{
 debug("FIELD HIT: %x\n",idx);
 int id,field;
 id = matrix[idx];
 					/* BROADCAST TO ALL PLAYERS, BUT NOT THE DEAD ONE WHO WILL DISAPPER */
 					/*WILL BE SENT AFTER PROCESSING ENTIRE MOVE_X,FIRE EVENT */
 if (id<FIELD_NONE) player_hit(id);
 field = matrix[idx];
 debug("FIELD TYPE: 0x%x\n", field);
 switch (field)
   {
    case FIELD_WALL:    field = FIELD_WALL_D1; break;
    case FIELD_ROCK:    field = FIELD_NONE;    break;
    case FIELD_TELEPRT: field = FIELD_LAVA;    break;
    case FIELD_WALL_D1: field = FIELD_WALL_D2; break;
    case FIELD_WALL_D2: field = FIELD_WALL_D3; break;
    case FIELD_WALL_D3: field = FIELD_NONE;    break;
    case FIELD_MIRROR:
		switch (what)
		  {
                   case FIELD_FUP:          field = FIELD_FDOWN; break;
                   case FIELD_FDOWN:        field = FIELD_FUP; break;
                   case FIELD_FLEFT:        field = FIELD_FRIGHT; break;
                   case FIELD_FRIGHT:       field = FIELD_FLEFT; break;
                   case FIELD_WATER_FUP:    field = FIELD_WATER_FDOWN ; break;
                   case FIELD_WATER_FDOWN:  field = FIELD_WATER_FUP ; break;
                   case FIELD_WATER_FLEFT:  field = FIELD_WATER_FRIGHT ; break;
                   case FIELD_WATER_FRIGHT: field = FIELD_WATER_FLEFT ; break;
                   case FIELD_GRASS_FUP:    field = FIELD_GRASS_FDOWN ; break;
                   case FIELD_GRASS_FDOWN:  field = FIELD_GRASS_FUP ; break;
                   case FIELD_GRASS_FLEFT:  field = FIELD_GRASS_FRIGHT ; break;
                   case FIELD_GRASS_FRIGHT: field = FIELD_GRASS_FLEFT ; break;
		  }
		break;
    case FIELD_NONE:
		switch (fire_type(what))
		  {
		   case FTYPE_WATER: field = FIELD_FLEFT + direction(what); break;
		   case FTYPE_NONE:  field = FIELD_FLEFT + direction(what);  break;
		   case FTYPE_GRASS: field = FIELD_FLEFT + direction(what); break;
		   default: { debug("FIRE_TYPE_INVALID: %x\n", what); return; }
		  }
		break;
    case FIELD_GRASS:
		switch (fire_type(what))
		  {
		   case FTYPE_WATER: field = FIELD_WATER_FLEFT + direction(what); break;
		   case FTYPE_NONE:  field = FIELD_GRASS_FLEFT + direction(what);  break;
		   case FTYPE_GRASS: field = FIELD_GRASS_FLEFT + direction(what); break;
		   default: { debug("FIRE_TYPE_INVALID: %x\n", what); return; }
		  }
		break;
    case FIELD_WATER:
		switch (fire_type(what))
		  {
		   case FTYPE_WATER: field = FIELD_WATER_FLEFT + direction(what); break;
		   case FTYPE_NONE:  field = FIELD_WATER_FLEFT + direction(what);  break;
		   case FTYPE_GRASS: field = FIELD_WATER_FLEFT + direction(what); break;
		   default: { debug("FIRE_TYPE_INVALID: %x\n", what); return; }
		  }
		break;
   }
 bmatrix[idx] = field;
}


void progress_fire()
{
 int i,x,y,to;
 uchar* bmatrix;
 bmatrix = (uchar*)malloc(0x1001*sizeof(uchar));
 if (!bmatrix) error("malloc bmatrix, needed for FIRE_PROGRESS.");
 for (i=0;i<0x1000;i++) bmatrix[i] = matrix[i];
 debug("PROGRESS FIRE.\n");
 for (i=0;i<0x1000;i++)
   {
    x = i/0x40;
    y = i%0x40;
    switch (matrix[i])
      {
       case FIELD_FUP:
       case FIELD_WATER_FUP:
       case FIELD_GRASS_FUP:
	       bmatrix[i] = fire_root(matrix[i]);
	       y--;
	       if (y<0) y+=0x40;
	       to = x*0x40+y;
	       field_hit(to, bmatrix, matrix[i]);
	       break;
       case FIELD_FDOWN:
       case FIELD_WATER_FDOWN:
       case FIELD_GRASS_FDOWN:
	       bmatrix[i] = fire_root(matrix[i]);
	       y++;
	       if (y>=0x40) y-=0x40;
	       to = x*0x40+y;
	       field_hit(to, bmatrix, matrix[i]);
	       break;
       case FIELD_FLEFT:
       case FIELD_WATER_FLEFT:
       case FIELD_GRASS_FLEFT:
	       bmatrix[i] = fire_root(matrix[i]);
	       x--;
	       if (x<0) x+=0x40;
	       to = x*0x40+y;
	       field_hit(to, bmatrix, matrix[i]);
	       break;
       case FIELD_FRIGHT:
       case FIELD_WATER_FRIGHT:
       case FIELD_GRASS_FRIGHT:
	       bmatrix[i] = fire_root(matrix[i]);
	       x++;
	       if (x>=0x40) x-=0x40;
	       to = x*0x40+y;
	       field_hit(to, bmatrix, matrix[i]);
	       break;
       default:
	       break;
      }
   }
 for (i=0;i<0x1000;i++) matrix[i] = bmatrix[i];
 free(bmatrix);
}


int teleport()
{
 int* free_idx;
 int i,nfree,j;
 WAIT_MUTEX
 nfree=0;
 for (i=0;i<0x1000;i++) if (matrix[i] == FIELD_NONE) nfree++;
 if (nfree==0) { debug("NO ROOM FOR TELEPORTATION.\n"); FREE_MUTEX return -1; }
 free_idx = (int*)malloc(nfree*sizeof(int));
 if (!free_idx) error("malloc error, SERVER PANIC");
 j=0;
 for (i=0;i<0x1000;i++) if (matrix[i] == FIELD_NONE) { free_idx[j] = i; j++; }
 j = free_idx[mrandom(nfree)];
 debug("TELEPORTED TO: %04x\n", j);
 free(free_idx);
 FREE_MUTEX
 return j;
}


int move_id_up(int id)
{
 int i,where,x,y,to;
 where=-1;
 debug("MOVE_ID_UP");
 for (i=0;i<0x1000;i++) if (matrix[i]==id) where = i;
 if (where==-1) error("move_id_up: NO SUCH ID: %d", id);
 x = where/0x40;
 y = where%0x40;
 y--;
 if (y<0) y+=0x40;
 to = x*0x40+y;
 progress_fire();
 if (matrix[to] == FIELD_NONE)
   {
    matrix[to] = id;
    matrix[where] = FIELD_NONE;
    return 1;
   }
 else if (matrix[to] == FIELD_GRASS)
   {
    matrix[to] = id;
    matrix[where] = FIELD_GRASS;
    return 1;
   }
 else if (matrix[to]==FIELD_TELEPRT)
   {
    to = teleport();
    if (to<0) return 0;
    matrix[to] = id;
    matrix[where] = FIELD_FDOWN;
    return 1;
   }
 else return 0;
}


void move_up(int fd)
{
 struct ThreadData* td;
 int found,can;
 int locked;
 can = 0;
 debug("MOVE UP REQUEST FROM: %d\n", fd);
 locked = TRY_LOCK
 if (locked) { if (write(fd,"CANT",5)==-1) error("WHEN REFUSING TO LOCK MUTEX FOR MOVE UP"); return; }
 td = clients;
 found=0;
 while (td)
   {
    if (td->fdr == fd) { found=1; can=move_id_up(td->id); }
    td = td->next;
   }
 if (!found) error("move_up, such client not found: %d\n", fd);
 if (!can)
   {
    if (write(fd,"CANT",5)==-1) error("writing CANT answer to MOVE_UP REQUEST");
   }
 else
   {
    if (write(fd,"MOVE",5)==-1) error("writing MOVE answer to MOVE_UP REQUEST");
    broadcast_board(0);
   }
 FREE_MUTEX
}


int move_id_down(int id)
{
 int i,where,x,y,to;
 where=-1;
 debug("MOVE_ID_DOWN");
 for (i=0;i<0x1000;i++) if (matrix[i]==id) where = i;
 if (where==-1) error("move_id_down: NO SUCH ID: %d", id);
 x = where/0x40;
 y = where%0x40;
 y++;
 if (y>=0x40) y-=0x40;
 to = x*0x40+y;
 progress_fire();
 if (matrix[to] == FIELD_NONE)
   {
    matrix[to] = id;
    matrix[where] = FIELD_NONE;
    return 1;
   }
 else if (matrix[to] == FIELD_GRASS)
   {
    matrix[to] = id;
    matrix[where] = FIELD_GRASS;
    return 1;
   }
 else if (matrix[to]==FIELD_TELEPRT)
   {
    to = teleport();
    if (to<0) return 0;
    matrix[to] = id;
    matrix[where] = FIELD_FUP;
    return 1;
   }
 else return 0;
}


void move_down(int fd)
{
 struct ThreadData* td;
 int found,can;
 debug("MOVE DOWN REQUEST FROM: %d\n", fd);
 int locked;
 can = 0;
 debug("TRY LOCK\n");
 locked = TRY_LOCK
 debug("AFTER TRY LOCK\n");
 debug("FREED\n");
 if (locked) { debug("LOCKED\n"); if (write(fd,"CANT",5)==-1) error("WHEN REFUSING TO LOCK MUTEX FOR MOVE UP"); return; }
 debug("GRANTED\n");
 td = clients;
 found=0;
 while (td)
   {
    if (td->fdr == fd) { found=1; can=move_id_down(td->id); }
    td = td->next;
   }
 if (!found) error("move_down, such client not found: %d\n", fd);
 if (!can)
   {
    if (write(fd,"CANT",5)==-1) error("writing CANT answer to MOVE_DOWN REQUEST");
   }
 else
   {
    if (write(fd,"MOVE",5)==-1) error("writing MOVE answer to MOVE_DOWN REQUEST");
    broadcast_board(0);
   }
 debug("MOVE DOWN FINISHED\n");
 FREE_MUTEX
}


int move_id_left(int id)
{
 int i,where,x,y,to;
 where=-1;
 debug("MOVE_ID_LEFT");
 for (i=0;i<0x1000;i++) if (matrix[i]==id) where = i;
 if (where==-1) error("move_id_left: NO SUCH ID: %d", id);
 x = where/0x40;
 y = where%0x40;
 x--;
 if (x<0) x+=0x40;
 to = x*0x40+y;
 progress_fire();
 if (matrix[to] == FIELD_NONE)
   {
    matrix[to] = id;
    matrix[where] = FIELD_NONE;
    return 1;
   }
 else if (matrix[to] == FIELD_GRASS)
   {
    matrix[to] = id;
    matrix[where] = FIELD_GRASS;
    return 1;
   }
 else if (matrix[to]==FIELD_TELEPRT)
   {
    to = teleport();
    if (to<0) return 0;
    matrix[to] = id;
    matrix[where] = FIELD_FRIGHT;
    return 1;
   }
 else return 0;
}


void move_left(int fd)
{
 struct ThreadData* td;
 int found,can;
 debug("MOVE LEFT REQUEST FROM: %d\n", fd);
 int locked;
 can = 0;
 locked = TRY_LOCK
 if (locked) { if (write(fd,"CANT",5)==-1) error("WHEN REFUSING TO LOCK MUTEX FOR MOVE UP"); return; }
 td = clients;
 found=0;
 while (td)
   {
    if (td->fdr == fd) { found=1; can=move_id_left(td->id); }
    td = td->next;
   }
 if (!found) error("move_left, such client not found: %d\n", fd);
 if (!can)
   {
    if (write(fd,"CANT",5)==-1) error("writing CANT answer to MOVE_LEFT REQUEST");
   }
 else
   {
    if (write(fd,"MOVE",5)==-1) error("writing MOVE answer to MOVE_LEFT REQUEST");
    broadcast_board(0);
   }
 FREE_MUTEX
}


int move_id_right(int id)
{
 int i,where,x,y,to;
 where=-1;
 debug("MOVE_ID_RIGHT");
 for (i=0;i<0x1000;i++) if (matrix[i]==id) where = i;
 if (where==-1) error("move_id_right: NO SUCH ID: %d", id);
 x = where/0x40;
 y = where%0x40;
 x++;
 if (x>=0x40) x-=0x40;
 to = x*0x40+y;
 progress_fire();
 if (matrix[to] == FIELD_NONE)
   {
    matrix[to] = id;
    matrix[where] = FIELD_NONE;
    return 1;
   }
 else if (matrix[to] == FIELD_GRASS)
   {
    matrix[to] = id;
    matrix[where] = FIELD_GRASS;
    return 1;
   }
 else if (matrix[to]==FIELD_TELEPRT)
   {
    to = teleport();
    if (to<0) return 0;
    matrix[to] = id;
    matrix[where] = FIELD_FLEFT;
    return 1;
   }
 else return 0;
}


void move_right(int fd)
{
 struct ThreadData* td;
 int found,can;
 debug("MOVE RIGHT REQUEST FROM: %d\n", fd);
 int locked;
 can = 0;
 locked = TRY_LOCK
 if (locked) { if (write(fd,"CANT",5)==-1) error("WHEN REFUSING TO LOCK MUTEX FOR MOVE UP"); return; }
 td = clients;
 found=0;
 while (td)
   {
    if (td->fdr == fd) { found=1; can=move_id_right(td->id); }
    td = td->next;
   }
 if (!found) error("move_right, such client not found: %d\n", fd);
 if (!can)
   {
    if (write(fd,"CANT",5)==-1) error("writing CANT answer to MOVE_RIGHT REQUEST");
   }
 else
   {
    if (write(fd,"MOVE",5)==-1) error("writing MOVE answer to MOVE_RIGHT REQUEST");
    broadcast_board(0);
   }
 FREE_MUTEX
}


void fire_id(int id, int stype)
{
 int i,where,xu,yu,xd,yd,xl,yl,xr,yr,tou,tod,tol,tor;
 where=-1;
 debug("FIRE_ID");
 for (i=0;i<0x1000;i++) if (matrix[i]==id) where = i;
 if (where==-1) error("fire_id: NO SUCH ID: %d", id);
 xd=xl=xr=xu=where/0x40;
 yd=yl=yr=yu=where%0x40;
 yu--;
 yd++;
 xl--;
 xr++;
 if (yu<0)     yu+=0x40;
 if (xl<0)     xl+=0x40;
 if (yd>=0x40) yd-=0x40;
 if (xr>=0x40) xr-=0x40;
 tou = xu*0x40+yu;
 tod = xd*0x40+yd;
 tol = xl*0x40+yl;
 tor = xr*0x40+yr;
 if (stype==FIRE_ALL || stype==FIRE_LEFT)
   {
    if (matrix[tol] == FIELD_NONE) matrix[tol] = FIELD_FLEFT;
    else if (matrix[tol] == FIELD_WATER) matrix[tol] = FIELD_WATER_FLEFT;
    else if (matrix[tol] == FIELD_GRASS) matrix[tol] = FIELD_GRASS_FLEFT;
   }
 if (stype==FIRE_ALL || stype==FIRE_RIGHT)
   {
    if (matrix[tor] == FIELD_NONE) matrix[tor] = FIELD_FRIGHT;
    else if (matrix[tor] == FIELD_WATER) matrix[tor] = FIELD_WATER_FRIGHT;
    else if (matrix[tor] == FIELD_GRASS) matrix[tor] = FIELD_GRASS_FRIGHT;
   }
 if (stype==FIRE_ALL || stype==FIRE_UP)
   {
    if (matrix[tou] == FIELD_NONE) matrix[tou] = FIELD_FUP;
    else if (matrix[tou] == FIELD_WATER) matrix[tou] = FIELD_WATER_FUP;
    else if (matrix[tou] == FIELD_GRASS) matrix[tou] = FIELD_GRASS_FUP;
   }
 if (stype==FIRE_ALL || stype==FIRE_DOWN)
   {
    if (matrix[tod] == FIELD_NONE) matrix[tod] = FIELD_FDOWN;
    else if (matrix[tod] == FIELD_WATER) matrix[tod] = FIELD_WATER_FDOWN;
    else if (matrix[tod] == FIELD_GRASS) matrix[tod] = FIELD_GRASS_FDOWN;
   }
 progress_fire();
}


void fire(int fd, int where)
{
 struct ThreadData* td;
 int found;
 debug("FIRE(0x%x) REQUEST FROM: %d\n", where, fd);
 int locked;
 locked = TRY_LOCK
 if (locked) { if (write(fd,"CANT",5)==-1) error("WHEN REFUSING TO LOCK MUTEX FOR FIR(X)"); return; }
 td = clients;
 found=0;
 while (td)
   {
    if (td->fdr == fd) { found=1; fire_id(td->id, where); }
    td = td->next;
   }
 if (!found) error("fire, such client not found: %d\n", fd);
 if (write(fd,"FIRO",5)==-1) error("writing FIRO answer to FIR(X) REQUEST");
 broadcast_board(0);
 FREE_MUTEX
}


void* reader_thread(void* ptr)
{
 char buff[20];
 int nread;
 int fd = (int)ptr;
 debug("S_READER %d\n", fd);
 while (1)
   {
    strcpy(buff,"");
/*    while (sig) usleep(SHED);*/
    while (sig) pthread_yield();
    if ((nread=read(fd,buff,5))==-1) { debug("SERVER_READ_THREAD"); exit_client(fd); broadcast_board(1); break; }
    debug("SR: %s\n", buff);
    if (nread==0)             { exit_client_dead(fd); broadcast_board(1); break; }
    if (!strcmp(buff,"KILL")) { exit_client(fd); broadcast_board(1); break; }
    if (!strcmp(buff,"MVUP")) move_up(fd);
    if (!strcmp(buff,"MVDO")) move_down(fd);
    if (!strcmp(buff,"MVLE")) move_left(fd);
    if (!strcmp(buff,"MVRI")) move_right(fd);
    if (!strcmp(buff,"FIRE")) fire(fd, FIRE_ALL);
    if (!strcmp(buff,"FIRU")) fire(fd, FIRE_UP);
    if (!strcmp(buff,"FIRD")) fire(fd, FIRE_DOWN);
    if (!strcmp(buff,"FIRL")) fire(fd, FIRE_LEFT);
    if (!strcmp(buff,"FIRR")) fire(fd, FIRE_RIGHT);
   }
 debug("READER THREAD DONE.\n");
 printf("SERVER: Client exited, now players: %d\n", nclients);
 return NULL;
}


void inform_refuse(int fd)
{
 char buff[20];
 debug("REFUSED TO ACCEPT CLIENT, too much clients.\n");
 WAIT_MUTEX
 if (write(fd,"BANE",5)==-1) { warning("Client refuses to listen server"); return; }
 if (read(fd,buff,16)==-1) { warning("Cannot read bane confirm from client"); return; }
 if (strcmp(buff,"BANE")) { warning("Client do not want to disconnect, forcing."); return; }
 if (write(fd,"FINE",5)==-1) { warning("Client refuses to listen server2"); return; }
 if (read(fd,buff,16)==-1) { warning("Cannot read bane confirm from client2"); return; }
 if (strcmp(buff,"FINE")) { warning("Client do not want to disconnect, forcing2"); return; }
 FREE_MUTEX
}

int send_client_id(int fd, int id)	/* SHULD SYNCHRONIZE IT? */
{					/* I THINK YES, OTHER THREADS CAN TRY TO SEND A BOAR AT THIS TIME */
 char buff[20];
 debug("SENDING CLIENT ID\n");
 if (id<0) error("cannot get distinct ID, serious algorithm problem.");
 WAIT_MUTEX
 if (write(fd,"IDCL",5)==-1) { debug("Client refuses to get ID"); FREE_MUTEX return 0; }
 if (read(fd,buff,16)==-1)   { debug("Cannot read ID confirm from client"); FREE_MUTEX return 0; }
 if (strcmp(buff,"IDIS"))    { debug("Client do not intrested in own ID"); FREE_MUTEX return 0; }
 if (write(fd,(void*)(&id),sizeof(int))==-1) { debug("writing INT id to client"); FREE_MUTEX return 0; }
 if (read(fd,buff,16)==-1)   { debug("Cannot read ID sent response from client"); FREE_MUTEX return 0; }
 if (strcmp(buff,"IDOK"))    { debug("Client do not send correct ID response"); FREE_MUTEX return 0; }
 debug("Send ID to client: %d\n", id);
 return 1;
}


int send_board(int fd, int with_mut)	/* FIXME, THIS AS ALWAYS NEED A LOT OF CHECKING */
{
 char buff[20];
 int nwritten;
 debug("SENDING BOARD to %d, mutex=%d...\n",fd,with_mut);
 if (with_mut) WAIT_MUTEX
 debug("GRANTED1\n");
 if (write(fd,"BOAR",5)==-1) { debug("Client refuses to get BOARD"); if (with_mut) FREE_MUTEX return 0; }
 debug("GRANTED2\n");
 if (read(fd,buff,16)==-1)   { debug("Cannot read BOARD confirm from client");if (with_mut) FREE_MUTEX return 0; }
 debug("GRANTED3\n");
 if (strcmp(buff,"BOIS"))    { debug("Client do not intrested in own BOARD");if (with_mut) FREE_MUTEX return 0; }
 debug("GRANTED4\n");
 if ((nwritten=write(fd,matrix,4096))==-1) { debug("writing BOARD id to client");if (with_mut) FREE_MUTEX  return 0; }
 debug("SEND %d BYTES.\n", nwritten);
 if (nwritten!=4096) error("WRITTEN NOT ENOUGH: %d", nwritten);
 if (read(fd,buff,16)==-1)   { debug("Cannot read BOARD sent response from client");if (with_mut) FREE_MUTEX return 0; }
 if (strcmp(buff,"BOOK"))    { debug("Client do not send correct BOARD response");if (with_mut) FREE_MUTEX return 0; }
 debug("Send BOARD to client\n");
 if (with_mut) FREE_MUTEX
 return 1;
}


void broadcast_board(int locked)
{
 struct ThreadData* td;
 if (locked) WAIT_MUTEX
 debug("BROADCASTING BOARD TO PLAYERS.\n");
 td = clients;
 while (td)
   {
    send_board(td->fdw, 0);
    td = td->next;
   }
 if (locked) FREE_MUTEX
 debug("BROADCAST DONE.\n");
}


int get_first_free()
{
 int tab[0x20];
 int i;
 struct ThreadData* td;
 WAIT_MUTEX
 for (i=0;i<0x20;i++) tab[i] = 0;
 td = clients;
 while (td)
   {
    tab[td->id] = 1;
    td = td->next;
   }
 for (i=0;i<0x20;i++) if (!tab[i]) { FREE_MUTEX return i; }
 FREE_MUTEX
 return -1;
}


int place_client_onboard(int id)
{
 int* free_idx;
 int i,nfree,j;
 WAIT_MUTEX
 nfree=0;
 for (i=0;i<0x1000;i++) if (matrix[i] == FIELD_NONE) nfree++;
 if (nfree==0) { debug("NO ROOM FOR NEW CLIENT.\n"); FREE_MUTEX return 0; }
 free_idx = (int*)malloc(nfree*sizeof(int));
 if (!free_idx) error("malloc error, SERVER PANIC");
 j=0;
 for (i=0;i<0x1000;i++) if (matrix[i] == FIELD_NONE) { free_idx[j] = i; j++; }
 j = free_idx[mrandom(nfree)];
 debug("PLACED ON: %04x\n", j);
 matrix[j] = id;
 free(free_idx);
 FREE_MUTEX
 debug("CLIENT PLACED ON BOARD.\n");
 return 1;
}


void register_client(int fdw, int fdr, int maxc)
{
 pthread_t *tr,*tw;
 int id;
 tr = (pthread_t*)malloc(sizeof(pthread_t));
 tw = (pthread_t*)malloc(sizeof(pthread_t));
 if (!tr || !tw) error("malloc new thread");
 if (nclients>=maxc) { inform_refuse(fdw); return; }
 id = get_first_free();
 if (!place_client_onboard(id)) { inform_refuse(fdw); return; }
 if (!send_client_id(fdw,id)) { remove_id(id); return; }
 /*send_board(fdw, 1);*/
 if (!send_board(fdw, 1)) { remove_id(id); return; }		/* WHEN CLIENT KILLED WHILE SETUP ITS INSTANCE */
 if (pthread_create(tr, NULL, reader_thread, (void*)fdr)) error("pthread_createR");
 if (pthread_create(tw, NULL, writer_thread, (void*)fdw)) error("pthread_createW");
 WAIT_MUTEX
 add_client(tr,tw,fdr,fdw,id);
 FREE_MUTEX
 nclients++;
 broadcast_board(1);
 debug("AFTER_REGISTER_CLIENT, clients: %d\n",nclients);
 printf("SERVER: New client registered, now players: %d\n", nclients);
}


void init_mutex()
{
 pthread_mutexattr_t mutex_attr;
 if (pthread_mutexattr_init(&mutex_attr)) error("creating MUTEX attributes.\n");
 if (pthread_mutex_init(&mutex, (const pthread_mutexattr_t*)(&mutex_attr))) error("createing MUTEX");
}


void server(int port, int maxc, int maxw, int cls, char* gamedat, char* sockf)
{
 int sock,c1sock,c2sock,err,alive,local;
 static struct sockaddr_in server;
 static struct sockaddr_un serverl;
 static struct sigaction act;
 matrix=0;
 nclients=0;
 clients=0;
 shutdwn=0;
 sig=0;
 local=0;
 loc = 0;
 sock_local = 0;
 init_mutex();
 if (!init_random()) error("INIT RANDOM DEVICE FAILED.");
 else debug("INIT RANDOM DEVICE DONE.\n");
 if (cls<=0 || cls>=0x1F) error("bad clients number: %d\n", cls);
 if (!gamedat || !strcmp(gamedat,"")) error("bad game data file.");
 if (port<1 || port>0xffff) error("port: %d is invalid", port);
 if (maxc<1 || maxc>0xff) error("max connections: %d is bad", maxc);
 if (maxw<0 || maxw>0xffffff) error("max wait: %d is bad", maxw);
 if (port<1024) debug("warning: port: %d is less than 1024 and can be OS-protected.\n", port);
 if (strcmp(sockf,"")) local=1;
 if (!local) debug("Starting server on local machine, port %d, gamedata=%s\n", port,gamedat);
 else
   {
    unlink(sockf);
    debug("Starting server on local machine, sockfile=%s, gamedata=%s\n", sockf,gamedat);
   }
 timeout=maxw;
 init_game(gamedat);
/* print_matrix();*/
 act.sa_handler = catch_int;		/* handle other signals */
 sigfillset(&(act.sa_mask));
 sigaction(SIGPIPE, &act, NULL);
 sigaction(SIGINT, &act, NULL);
 sigaction(SIGALRM, &act, NULL);
 if (!local)
   {
    server.sin_family = AF_INET;		/* setup server socket */
    server.sin_port = (in_port_t)(port);
    server.sin_addr.s_addr = INADDR_ANY;
    sock = socket(AF_INET, SOCK_STREAM, 0); /* create it */
   }
 else
   {
    serverl.sun_family = AF_UNIX;
    strncpy(serverl.sun_path, sockf, MAX_SOCK_PATH);
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
   }
 if (sock==-1)
   {
    perror("socket");
    error("create socket failed.");
   }
 alive=1;				/* keep connection alive */
 err=setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,&alive,sizeof(void*));
 if (err==-1)
     {
      perror("setsockopt");
      error("set socket options failed");
     }
 if (!local) err=bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
 else        err=bind(sock, (struct sockaddr*)&serverl, sizeof(struct sockaddr_un));
 if (err==-1)
   {
    perror("bind");
    error("bind address failed.");
   }
 if (local)
   {
    sock_local = (char*)malloc(MAX_SOCK_PATH+1);
    if (!sock_local) error("malloc local socket failed.\n");
    strncpy(sock_local, sockf, MAX_SOCK_PATH);
    loc = 1;
   }
 if (listen(sock,maxc)==-1)		/* listen connections, maxc */
   {
    perror("listen");
    error("listen on socket failed");
   }
 debug("Server started\n");
 main_sock = sock;
 printf("GameServer for UNIX started.\n");
 while (1)				/* server loop, ^C ends cleanly. */
   {
    debug("waiting for client...");
    fflush(stdout);			/* if no client in maxw seconds */
    if (maxw) alarm(maxw);		/* timeout for connection */
    if ((c1sock=accept(sock,NULL,NULL))==-1)
      {					/* accept client */
       perror("accept");
       error("accept failed");
       continue;
      }
    if (maxw) alarm(0);
    if (maxw) alarm(maxw);		/* timeout for connection */
    if ((c2sock=accept(sock,NULL,NULL))==-1)
      {					/* accept client */
       perror("accept");
       error("accept failed");
      }
    if (maxw) alarm(0);
    register_client(c1sock, c2sock, cls);
    fflush(stdout);			/* client works with child */
   }
 debug("SERVER DONE.\n");
 if (close(main_sock)==-1) error("closing main server socket");
 else debug("MAIN SERVER SOCKET CLOSED.\n");
}


int main(int lb, char** par)		/* server starts */
{
 char u;
 char port[12];
 char maxc[12];
 char maxC[12];
 char maxw[12];
 char file[1024];
 char sockf[1024];
 strcpy(port,"10001");			/* defaults */
 strcpy(maxc,"5");
 strcpy(maxC,"6");
 strcpy(maxw,"0");
 strcpy(file,"game.dat");
 strcpy(sockf,"");
 while ((u = getopt(lb,par,"c:p:m:w:l:s:h"))!=-1)	/* parse options */
 {
  switch (u)
   {
    case 'p': if (strlen(optarg)<12)   strcpy(port, optarg); break;
    case 'm': if (strlen(optarg)<12)   strcpy(maxc, optarg); break;
    case 'w': if (strlen(optarg)<12)   strcpy(maxw, optarg); break;
    case 'l': if (strlen(optarg)<1024) strcpy(file, optarg); break;
    case 's': if (strlen(optarg)<1024) strcpy(sockf,optarg); break;
    case 'c': if (strlen(optarg)<12)   strcpy(maxC, optarg); break;
    case 'h': help(par[0]); return 0;
    default: debug("Unrecognized option\n"); return 1;
   }
 }
 debug("SERVER: %d,%d,%d,%d,%s\n", atoi(port),atoi(maxc),atoi(maxw),atoi(maxC),file);
 server(atoi(port), atoi(maxc), atoi(maxw), atoi(maxC), file, sockf);
 debug("MAIN DONE.\n");
 return 0;
}

