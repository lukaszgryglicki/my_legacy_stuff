#include "libmipc.h"
#include <signal.h>
#include <fcntl.h>
#define BUFS     20
#define SHMKEY  512
#define SEMKEY1 512
#define SEMKEY2 513
#define SEMKEY3 514
#define MSGKEY  512
#define MSGDATA   1

void drop_ipc()
{
 if (mdropsemkey(512)==-1) warn("Cannot drop semaphore  by key: %d",512);
 if (mdropmsgkey(512)==-1) warn("Cannot drop msgquery   by key: %d",512);
 if (mdropshmkey(512)==-1) warn("Cannot drop shmsegment by key: %d",512);
 if (mdropsemkey(513)==-1) warn("Cannot drop semaphore  by key: %d",513);
 if (mdropsemkey(514)==-1) warn("Cannot drop semaphore  by key: %d",514);
}


void push(int q, int from, int to, int type)
{
 char str[100];
 printf("push\n");
 if ((to-from)>20) fatal("too many data in buffer!");
 sprintf(str,"%d %d", from, to);
 if (mmsgsendtxt(q,str,type)==-1) fatal("mmsgsend");
 printf("push-ed\n");
}


void pop(int q, int* from, int* to)
{
 char str[255];
 printf("pop\n");
 if (mmsgrecvtxt(q,str,MSGDATA,1)==-1) fatal("mmsgrecvtxt");
 if (sscanf(str, "%d %d", from, to)<0) fatal("scanf");
 printf("pop-ed\n");
 printf("server: pop: %d %d\n", *from, *to);
}


int write_to_buffer(int fd, int a, int n, char* buff, int que, int part)
{
 int from,to,offset;
 off_t size;
 size   = lseek(fd, 0, SEEK_END);
 from   = (size*a)/n;
 to     = (size*(a+1))/n;
 offset = from+BUFS*part;
 to     = ((offset+20)>to)?(to):(offset+20);
 printf("child#%d: offset %d\n", a,offset);
 if (offset>to)
  {
   printf("push -1\n");
   push(que,-1, -1, MSGDATA);
   return 1;
  }
 if (lseek(fd,offset,SEEK_SET)==(off_t)(-1)) fatal("lseek in write_to_buffer");
 if (read(fd,buff,((to-offset)>20)?20:(to-offset))==-1) fatal("read in write_to_buffer");
 push(que, offset, to, MSGDATA);
 return 0;
}


void read_from_buffer(int fd, char* buff, int que,int cnt, int* cl)
{
 int from,to,siz;
 if (cnt)
   {
    pop(que, &from, &to);
    printf("server: %d %d %s\n", from, to, buff);
    siz=to-from;
    if (from==-1 && to==-1)
      {
       printf("GOT -1\n");
       (*cl)--;
       return ;
      }
    printf("siz=%d\n", siz);
    if (lseek(fd, from, SEEK_SET)==(off_t)(-1)) fatal("lseek in read from buffer");
    if (write(fd,buff,siz)==-1) fatal("write in read_from_buffer");
   }
}


void server_run(int n, int semc, int sems, int semf, char* buff, int que, int fd)
{
 int i;
 int clients;
 printf("server...\n");
 i=0;
 clients=n;
 while (1)
   {
    printf("wait.semf\n");
    mwait(semf);
    printf("wait.semc\n");
    mwait(semc);
    read_from_buffer(fd,buff,que,i,&clients);
    printf("server: writing...clients: %d\n",clients);
    if (!clients) break;
    printf("sign.semc\n");
    msignal(semc);
    printf("sign.sems\n");
    msignal(sems);
    i++;
   }
 drop_ipc();
 sleep(1);
}


void client_run(int a, int n, int semc, int sems, int semf, char* buff, int que, int fd)
{
 int part,done;
 printf("client...\n");
 done=part=0;
 while (!done)
   {
    printf("wait.semf\n");
    mwait(sems);
    printf("wait.semc\n");
    mwait(semc);
    printf("client #%d: reading...\n", a);
    done = write_to_buffer(fd,a,n,buff,que,part);
    printf("sign.semc\n");
    msignal(semc);
    printf("sign.semf\n");
    msignal(semf);
    part++;
   }
 printf("\n\nCLIENT DONE!\n\n");
 sleep(1);
}


void signals()
{
 static struct sigaction chldact;
 chldact.sa_handler = SIG_IGN;
 chldact.sa_flags = SA_NOCLDWAIT;
 sigfillset(&(chldact.sa_mask));
 sigaction(SIGCHLD, &chldact, NULL);
}


void setup_ipc(int* shmid, void** buff, int* sems, int* semc, int* semf, int* quec)
{
  if ((*shmid=mshm(SHMKEY,BUFS))==-1) fatal("mshm");
  if (mshmconn(*shmid,buff)==-1) fatal("mshmconn");
  if ((*semc=msem(SEMKEY1,1))==-1) fatal("msem-c");
  if ((*sems=msem(SEMKEY2,0))==-1) fatal("msem-s");
  if ((*semf=msem(SEMKEY3,1))==-1) fatal("msem-f");
  if ((*quec=mque(MSGKEY))==-1) fatal("mque");
}


void file_c(int* fd, int* fdo, char* name, char* oname)
{
 int fsiz;
 *fd = open(name,O_RDONLY);
 if (*fd==-1) fatal("cannot open input file: %s", name);
 *fdo = open(oname,O_WRONLY|O_CREAT|O_TRUNC,0660);
 if (*fdo==-1) fatal("cannot create output file: %s", oname);
 fsiz=lseek(*fd,0,SEEK_END);
 lseek(*fd,0,SEEK_SET);
 lseek(*fdo,fsiz-1,SEEK_SET);
 write(*fdo,"\000",1);
}


void run_server(int n, char* fname, char* outn)
{
 int shmid,semc,sems,semf,que,fd,fdo,i;
 char* buff;
 printf("run_server!\n");
 if (!fname || !outn || n<=0) fatal("parameters mismatch");
 signals();
 file_c(&fd, &fdo, fname, outn);
 setup_ipc(&shmid, (void**)(&buff), &sems, &semc, &semf, &que);
 printf("got numproc=%d filename=%s\n", n,fname);
 for (i=0;i<n;i++)
   {
    if (!fork())
      {
       client_run(i, n, semc, sems, semf, buff, que, fd);
       exit(0);
      }
   }
 server_run(n, semc, sems, semf, buff, que, fdo);
 printf("server_run completed!\n");
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
 char* ofile;
 ofile = file = NULL;
 num=5;
 printf("In 64bit linux function 'error' has been added to libc\n");
 printf("this program uses different 'error' from libmipc.so\n");
 printf("Remember to link libmipc.so BEFORE libc.so!!\n");
 printf("%s: -f file -o out_file -n number_of_processes\n",par[0]);
 if (lb<3) error("bad arguments\n");
 while ((ch = getopt(lb,par,"f:n:o:")) != -1)
   {
    switch (ch)
      {
       case 'n': num   = atoi(optarg); break;
       case 'f': file  = malloc(strlen(optarg)+1); strcpy(file, optarg);  break;
       case 'o': ofile = malloc(strlen(optarg)+1); strcpy(ofile, optarg); break;
      }
   }
 drop_ipc();
 printf("IPCs dropped!\n");
 run_server(num, file, ofile);
 if (file) free(file);
 if (ofile) free(ofile);
 return 0;
}

