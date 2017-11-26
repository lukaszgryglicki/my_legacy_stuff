#include "common.h"

void go_produce()
{
 key_t qFk,qEk,semspack,semprodk,semconsk,semfullk, shmk;
 int sem_space, sem_prod, sem_cons, sem_full, i, shmid;
 char* buff;
 unsigned char product;
 int qF,qE,to;
 int newq;
 qEk=512;
 qFk=513;
 shmk=512;
 semspack=512;
 semprodk=513;
 semconsk=514;
 semfullk=515;
 /*sranddev();*/
 printf("starting up...\n");
 if ((shmid=mshm(shmk,BUFS))==-1) fatal("initializing shm");
 if (mshmconn(shmid,(void*)(&buff))==-1) fatal("mapping segment shm");
 if ((qF=mque(qFk))==-1) fatal("cannot create FIFO F: mmsg");
 if ((qE=mquenew(qEk,&newq))==-1) fatal("cannot create FIFO E: mmsg");
 if ((sem_space=msem(semspack,BUFS-1))==-1) fatal("creating sem_space semaphore");	/* IS IT OK */
 if ((sem_prod =msem(semprodk,1))==-1) fatal("creating sem_prod semaphore");
 if ((sem_cons =msem(semconsk,1))==-1) fatal("creating sem_cons semaphore");
 if ((sem_full =msem(semfullk,0))==-1) fatal("creating sem_full semaphore");
 printf("filling FIFO...\n");
 if (newq) for (i=0;i<BUFS;i++) push(qE, i);
 printf("init complete.\n");
 fflush(stdout);
 while (1)
   {
    product = (rand()%0xFF)+1;
    printf("producting...(%d)\n", product);
    printf("wait for space/empty...\n");
    mwait(sem_space);
    printf("wait for other producents...\n");
    mwait(sem_prod);
    printf("getting index...\n");
    to = pop(qE);
    msignal(sem_prod);
    printf("index got %d, filling buffer...\n",to);
    buff[to] = product;
    printf("waiting for client...\n");
    mwait(sem_cons);
    printf("storing index in FIFO...\n");
    push(qF, to);
    printf("index stored in FIFO...\n");
    msignal(sem_cons);
    printf("signalling full...\n");
    msignal(sem_full);    
    printf("relooping...\n");
   }
}

int main()
{
 go_produce();
 return 0;
}
