#include "libmipc.h"
#include <signal.h>

#define BUFS     20
#define SHMKEY  512
#define SEMKEY1 512
#define SEMKEY2 513
#define MSGKEY  512

void server_run(int n, char* fname, int semc, int sems, char* buff, int que)
{
 printf("server...\n");
 while (1)
   {
    mwait(semc);
    printf("server: writing...\n");
/*    sleep(rand()%3);*/
    msignal(semc);
    msignal(sems);		/* SERVER SKONCZYL */
   }
}

void client_run(int a, int n, char* fname, int semc, int sems, char* buff, int que)
{
 printf("client...\n"); 	
 while (1)
   {
    mwait(sems);		/* CZEKAJ AZ SERVER PRZETWORZY, ABY BYLO NA ZMIANE */
    mwait(semc);
    printf("client #%d: reading...\n", a);
    sleep(rand()%3);
    msignal(semc);
   }
}

void signals()
{
 static struct sigaction chldact;
 chldact.sa_handler = SIG_IGN;	
 chldact.sa_flags = SA_NOCLDWAIT;
 sigfillset(&(chldact.sa_mask));
 sigaction(SIGCHLD, &chldact, NULL);	
}

void setup_ipc(int* shmid, void** buff, int* sems, int* semc, int* quec)
{
  if ((*shmid=mshm(SHMKEY,BUFS))==-1) fatal("mshm");
  if (mshmconn(*shmid,buff)==-1) fatal("mshmconn");
  if ((*semc=msem(SEMKEY1,1))==-1) fatal("msem-c");
  if ((*sems=msem(SEMKEY2,0))==-1) fatal("msem-s");
  if ((*quec=mque(MSGKEY))==-1) fatal("mque");
}

void run_server(int n, char* fname)
{
 int i;
 int shmid,semc,sems,que;
 char* buff;
 if (!fname) return ;
 signals();
 setup_ipc(&shmid, (void**)(&buff), &sems, &semc, &que);
 printf("got numproc=%d filename=%s\n", n,fname);
 for (i=0;i<n;i++)
   {
    if (!fork()) 
      { 
       client_run(i, n, fname, semc, sems, buff, que); 
       exit(0); 
      }
   }
 server_run(n, fname, semc, sems, buff, que);
 mshmdisconn((void*)(&buff));
 mdropshm(shmid);
 mdropsem(semc);
 mdropsem(sems);
 mdropmsg(que);
}

int main(int lb, char** par)
{
 int ch,num;
 char* file;
 file = NULL;
 num=5;
 if (lb<3) error("%s: -f file -n number_of_processes\n",par[0]); 
 while ((ch = getopt(lb,par,"f:n:")) != -1)
   {
    switch (ch)
      {
       case 'n': num   = atoi(optarg); break;
       case 'f': file  = malloc(strlen(optarg)+1); strcpy(file, optarg); break;
      }
   }
 run_server(num, file);
 if (file) free(file);
 return 0;
}
