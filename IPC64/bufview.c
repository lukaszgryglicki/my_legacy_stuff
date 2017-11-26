#include "common.h"

void go_viewbuff()
{
 key_t shmk;
 int shmid,newm,i;
 unsigned char* buff;
 shmk=512;
 printf("starting up...\n");
 if ((shmid=mshmnew(shmk,BUFS,&newm))==-1) fatal("initializing shm");
 if (newm) fatal("initialized new shm, should read from other process");
 if (mshmconn(shmid,(void*)(&buff))==-1) fatal("mapping segment shm");
 while (1)
   {
    for (i=0;i<BUFS;i++) printf("%03d:", buff[i]);
    printf("\n");
    sleep(1);
   }
}

int main()
{
 go_viewbuff();
 return 0;
}
