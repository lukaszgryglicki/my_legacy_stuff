#include "common.h"

void go_consume()
{
 key_t qFk,qEk,semspack,semprodk,semconsk,semfullk,shmk;
 int sem_space, sem_prod, sem_cons, sem_full,shmid;
 char* buff;
 unsigned char consumed;
 int qF,qE,from;
 int newq;
 qEk=512;
 qFk=513;
 shmk=512;
 semspack=512;
 semprodk=513;
 semconsk=514;
 semfullk=515;
 printf("starting up...\n");
 if ((qF=mquenew(qFk, &newq))==-1) fatal("cannot create FIFO F: mmsg");
 if (newq) fatal("producent not running\n");
 if ((qE=mquenew(qEk,&newq))==-1) fatal("cannot create FIFO E: mmsg");
 if (newq) fatal("producent crashed\n");
 if ((shmid=mshm(shmk,BUFS))==-1) fatal("initializing shm");
 if (mshmconn(shmid,(void*)(&buff))==-1) fatal("mapping segment shm");
 if ((sem_space=msemget(semspack))==-1) fatal("getting sem_space semaphore");
 if ((sem_prod =msemget(semprodk))==-1) fatal("getting sem_prod semaphore");
 if ((sem_cons =msemget(semconsk))==-1) fatal("getting sem_cons semaphore");
 if ((sem_full =msemget(semfullk))==-1) fatal("getting sem_full semaphore");
 printf("init complete.\n");
 fflush(stdout);
 while (1)
   {
    printf("waiting for full sem...\n");
    mwait(sem_full);
    printf("waiting fro other clients...\n");
    mwait(sem_cons);
    from = pop(qF);
    printf("getting index...%d\nsignaling consuments...\n", from);
    msignal(sem_cons);
    consumed = buff[from];
    printf("consuming...%d\n", (int)consumed);
    printf("waiting for producent...\n");
    mwait(sem_prod);
    printf("adding to empty FIFO...\n");
    push(qE, from);
    printf("signalling producer(s)...\n");
    msignal(sem_prod);
    printf("signalling there is free space...\n");
    msignal(sem_space);
   }
}

int main()
{
 go_consume();
 return 0;
}
