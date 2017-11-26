#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define MAXSIZ 1024
int sock;
int semaphore;
key_t semkey;
int file;

union msemun
{
 int    val;
 struct semid_ds *buf;
 unsigned short* array;
};

void error(char* fmt, ...)
{
 va_list lst;
 va_start(lst,fmt);
 printf("Error: ");
 vprintf(fmt,lst);
 printf("\nServer halted.\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}


int msem(key_t semk, int val)
{
 int status,semid;
 printf("Creating semaphore with key: %d and start value: %d\n", (int)semk,val);
 status = 0;
 if (val<0) return -1;
 if ((semid=semget(semk,1,0660|IPC_CREAT|IPC_EXCL))==-1)
   {
    if (errno==EEXIST) semid = semget(semk,0,0660);
    else perror("semget in msem");
   }
 if (semid != -1)
   {
    union msemun arg;
    arg.val = val;
    status = semctl(semid,0,SETVAL,arg);
   }
 if (semid==-1 || status==-1)
   {
    perror("semctl in msem");
    return -1;
   }
 printf("New semaphore key is: %d\n", semid);
 return semid;
}


int mwait(int semid)
{
 struct sembuf buf;
 printf("Waiting for semaphore...\n");
 buf.sem_num = 0;
 buf.sem_op = -1;
 buf.sem_flg = SEM_UNDO;
 if (semop(semid,&buf,1)==-1)
   {
    perror("semop in mwait");
    return -1;
   }
 printf("Semaphore free, entering CS\n");
 return 0;
}


int msignal(int semid)
{
 struct sembuf buf;
 buf.sem_num = 0;
 buf.sem_op =  1;
 buf.sem_flg = SEM_UNDO;
 printf("Signalling semaphore...\n");
 if (semop(semid,&buf,1)==-1)
   {
    perror("semop in msignal");
    return -1;
   }
 printf("Semaphore freed.\n");
 return 0;
}


int mdropsem(int semid)
{
 printf("Destroying semaphore: server is exiting...\n");
 if (semctl(semid,IPC_RMID,0)==-1) return -1;
 else return 0;
}


void help()
{
 printf("Options are: -p PORT\n");
}

void get_from_edb(double* reslt, int* nearest, int n)
{
 int i;
 double value;
 mwait(semaphore);
 *reslt = 0.0;
 *nearest = 0;
 printf("Searching for E-value for n=%d in EDB\n",n);
 for (i=n;i>=0;i--)
   {
    lseek(file, i*(sizeof(int)+sizeof(double))+sizeof(int), SEEK_SET);
    read(file, &value, sizeof(double));
    if (value != 0.0)
      {
       *nearest = i;
       *reslt   = value;
       if (i!=n) printf("Found for i=%d (%f), will compute from %d-->%d\n", i,value,i,n);
       else printf("Exactly matching value exists in EDB, i=%d (%f)\n", i, value);
       break;
      }
   }
 msignal(semaphore);
}

void set_edb(int n, double reslt)
{
 printf("Setting new E-value in EDB for n=%d (%f)\n", n,reslt);
 mwait(semaphore);
 lseek(file, n*(sizeof(int)+sizeof(double))+sizeof(int), SEEK_SET);
 write(file, &reslt, sizeof(double));
 msignal(semaphore);
}

double silnia(int arg)
{
 if (arg>100) return 0.0;
 if (arg<=1.0) return 1.0;
 return arg*silnia(arg-1);
}

double evaluate_rest(int from, int to)
{
 double reslt;
 double sil;
 int i;
 printf("Rest evaluate from %d to %d\n", from, to);
 reslt = 0.0;
 printf("Must evaluate from %d to %d wait...\n", from, to);
 sil = silnia(from);
 printf("Root computed, continuing in loop...\n");
 for (i=from;i<to;i++)
   {
    reslt += 1.0/sil;
    sil *= i+1;
   }
 printf("Evaluated from %d to %d rest is: %f\n", from ,to, reslt);
 return reslt;
}


void handle_client(int sock)
{
 char buff[MAXSIZ+1];
 int n,nearest;
 double reslt;
 strcpy(buff,"");
 if ((n=read(sock,buff,MAXSIZ))==-1) error("reading socket");
 if (n==0) return;
 buff[n] = 0;
 sscanf(buff,"%d", &n);
 printf("Will compute E-value for client, n=%d\n", n);
 if (n<0) { if (write(sock,"ERR", 4)==-1) error("write"); }
 else if (n>100) { if (write(sock,"N2BIG", 6)==-1) error("write"); }
 else
   {
    get_from_edb(&reslt, &nearest, n);
    if (nearest == 0) printf("No usable values in EDB, must compute from 0\n");
    if (nearest != n) 
      {
       printf("Don't find matching value in EDB, computing...\n");
       reslt += evaluate_rest(nearest, n);
       set_edb(n, reslt);
      }
    if (nearest == n) printf("Found matching entry in EDB, using it\n");
    sprintf(buff,"%f", reslt);
    printf("Writing final result (%s)\n", buff);
    if (write(sock,buff, strlen(buff))==-1) error("write");
   }
}


void catch_int(int signo)
{
 printf("Caught deadly signal %d\n", signo);
 printf("Closing server socket...\n");
 if (sock) close(sock);
 if (mdropsem(semaphore)) error("cannot drop semaphore");
 printf("Closing EDB file...\n");
 close(file);
 printf("Exiting...\n");
 exit(0);
}


void setup_signals()
{
 static struct sigaction chldact;
 static struct sigaction intact;
 printf("Setting up signals handlers...\n");
 chldact.sa_handler = SIG_IGN;
 chldact.sa_flags = SA_NOCLDWAIT;
 sigfillset(&(chldact.sa_mask));
 sigaction(SIGCHLD, &chldact, NULL);
 intact.sa_handler = catch_int;
 sigfillset(&(intact.sa_mask));
 sigaction(SIGINT, &intact, NULL);
}


void setup_semaphore()
{
 printf("Setting up EDB semaphore\n");
 semkey = 6666;
 semaphore = msem(semkey, 1);
 if (!semaphore) error("cannot create semaphore");
}

void setup_file()
{
 int i;
 double zero;
 printf("Setting up EDB database file...\n");
 file = open("EDB", O_CREAT|O_TRUNC|O_RDWR, 0660);
 if (file<0) error("cannot create file: EDB");
 zero = 0.0;
 for (i=0;i<=100;i++)
   {
    write(file, &i, sizeof(int));
    write(file, &zero, sizeof(double));
   }
}


void server(int port)
{
 int csock;
 struct sockaddr_in server;
 if (port<1 || port>0xffff) error("port: %d is invalid", port);
 if (port<1024) printf("warning: port: %d is less than 1024 and can be OS-protected.\n", port);
 printf("Starting server: port %d\n", port);
 server.sin_family = AF_INET;
 server.sin_port = htons(port);
 server.sin_addr.s_addr = htonl(INADDR_ANY);
 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock==-1)
   {
    perror("socket");
    error("create socket failed.");
   }
 if (bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in))==-1)
   {
    perror("bind");
    error("bind address failed.");
   }
 if (listen(sock,5)==-1)
   {
    perror("listen");
    error("listen on socket failed");
   }
 setup_signals();
 setup_file();
 setup_semaphore();
 printf("Server started.\n");
 while (1)
   {
    printf("waiting...");
    fflush(stdout);
    if ((csock=accept(sock,NULL,NULL))==-1)
      {
       perror("accept");
       error("accept failed");
      }
    if (!fork())
       {
	handle_client(csock);
	close(csock);
	exit(0);
       }
    close(csock);
    printf("\n");
    fflush(stdout);
   }
}


int main(int lb, char** par)
{
 char u;
 char port[12];
 strcpy(port,"6666");
 printf("Starting server.., use -h to see available options...\n");
 while ((u = getopt(lb,par,"p:h"))!=-1)
 {
  switch (u)
   {
    case 'p': if (strlen(optarg)<12) strcpy(port, optarg); break;
    case 'h': help(); return 0;
    default: printf("Unrecognized option\n"); return 1;
   }
 }
 server(atoi(port));
 return 0;
}

