#include "libmipc.h"
#include <sys/wait.h>

void testmsem()
{
 int semid;
 int child;
 key_t semkey;
 semkey = 0x200;
 child = 0;
 printf("id=%d\n", msemidfromkey(semkey));
 if ((semid=msem(semkey,1))==-1) fatal("msem failed");
 printf("id=%d\n", semid);
 printf("id=%d\n", msemidfromkey(semkey));
 printf("Before CS...\n");
 if (!fork()) child=1;
 mwait(semid);
 printf("In CS...\n");
 sleep(1);
 printf("After CS..\n");
 msignal(semid);
 if (child) exit(0);
 wait(NULL);
 mdropsem(semid);
}

void testmmsg()
{
 int queid;
 char buff[MSGSIZ];
 key_t quekey;
 quekey=0x200;
 printf("id=%d\n", mmsgidfromkey(quekey));
 if ((queid=mque(quekey))==-1) fatal("mque failed");
 printf("id=%d\n", queid);
 printf("id=%d\n", mmsgidfromkey(quekey));
 if (mmsgsendtxt(queid,"Hello world #1", 6)==-1) error("mmsgsendtxt");
 if (mmsgsendtxt(queid,"Hello world #2", 3)==-1) error("mmsgsendtxt");
 if (mmsgsendtxt(queid,"Hello world #3", 1)==-1) error("mmsgsendtxt");
 if (mmsgsendtxt(queid,"Hello world #4", 2)==-1) error("mmsgsendtxt");
 if (mmsgsendtxt(queid,"Hello world #5", 4)==-1) error("mmsgsendtxt");
 if (mmsgsendtxt(queid,"Hello world #6", 5)==-1) error("mmsgsendtxt");
 if (mmsgrecptxt(queid,buff,1)==-1) error("mmsgrecptxt"); else printf("%s\n", buff);
 if (mmsgrecptxt(queid,buff,1)==-1) error("mmsgrecptxt"); else printf("%s\n", buff);
 if (mmsgrecptxt(queid,buff,1)==-1) error("mmsgrecptxt"); else printf("%s\n", buff);
 if (mmsgrecptxt(queid,buff,1)==-1) error("mmsgrecptxt"); else printf("%s\n", buff);
 if (mmsgrecptxt(queid,buff,1)==-1) error("mmsgrecptxt"); else printf("%s\n", buff);
 if (mmsgrecptxt(queid,buff,1)==-1) error("mmsgrecptxt"); else printf("%s\n", buff);
 /*if (mmsgrecptxt(queid,buff,0)!=0)  error("mmsgrecptxt"); else printf("%s\n", buff);*/
 /* will cause error because IPC_NOWAIT is set, will return 1 means EAGAIN */
 mdropmsg(queid);
}

void child_proc(int shm, int sem, void* ptr)
{
 printf("child...\n");
 mwait(sem);
 printf("%s\n", (char*)ptr);
 if (mshmdisconn(&ptr)==-1) error("mshmdisconn");
}


void parent_proc(int shm, int sem, void* ptr)
{
 printf("parent...\n");
 sprintf((char*)ptr,"HELLO_SHM");
 msignal(sem);
}

void testmshm()
{
 int shmid,semid;
 key_t shmkey,semkey;
 void* ptr;
 semkey = shmkey = 0x200;
 printf("id=%d\n", mshmidfromkey(shmkey));
 if ((shmid=mshm(shmkey,1024))==-1) fatal("mshm failed");
 printf("id=%d\n", shmid);
 printf("id=%d\n", mshmidfromkey(shmkey));
 if ((semid=msem(semkey,0))==-1) fatal("msem (read) failed");
 if (mshmconn(shmid, &ptr)==-1) fatal("connect shm");
 sprintf((char*)ptr,"BAD_VAL");
 switch (fork())
 {
  case -1: fatal("fork failed");
  case 0:
	   /*child*/
	   child_proc(shmid, semid, ptr);
	   exit(0);
  default:
           parent_proc(shmid, semid, ptr);
	   wait(NULL);
	   if (mshmdisconn(&ptr)==-1) error("mshmdisconn");
	   mdropshm(shmid);
	   mdropsem(semid);
 }
}

int main(int lb, char** par)
{
 testmsem();
 testmmsg();
 testmshm();
 return 0;
}
