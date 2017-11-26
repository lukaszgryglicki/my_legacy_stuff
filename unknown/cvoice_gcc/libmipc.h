#ifndef __LIBMIPC_H__
#define __LIBMIPC_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define MSGSIZ 2048
#define PRIOR  1024

struct mmsg
{
 int32_t mtype;
 char    mtext[MSGSIZ];
};

union msemun
{
 int    val;
 struct semid_ds *buf;
 unsigned short* array;
};

int fatal(const char* fmt, ...);
int error(const char* fmt, ...);
int warn(const char* fmt, ...);




int msem(key_t semk, int val);
int msemget(key_t semk);
int msemnew(key_t semk, int val, int* news);
int mwait(int semid);
int msignal(int semid);
int mdropsem(int semid);
int mdropsemkey(int semkey);
int msemidfromkey(key_t semkey);





int mque(key_t quek);
int mquenew(key_t quek, int* newq);
int mdropmsg(int msgid);
int mdropmsgkey(int msgkey);
int mmsgsendtxt(int msgid, char* txt, int priority);
int mmsgsendstr(int msgid, struct mmsg* msg);
int mmsgrecvtxt(int msgid, char* txt, int type, int wt);/* returns -1, 0 or 1, 1 when NO_WAIT set EAGAIN*/
							/* reads only msgs of type */ 
							/* wt means wait or not for such? */
int mmsgrecvstr(int msgid, struct mmsg* msg, int wt);	/* need init mem for struct and for text above */
							/* in struct initially mtype must be set */
							/* do not set 0 on end of string, can be binary */
int mmsgrecptxt(int msgid, char* txt, int wt);
int mmsgrecpstr(int msgid, struct mmsg* msg, int wt);
int mmsgidfromkey(key_t msgkey);




int mshm(key_t shmk, int size);
int mshmnew(key_t shmk, int size, int* newm);
int mdropshm(int shmid);
int mdropshmkey(int shmkey);
int mshmidfromkey(key_t shmkey);
int mshmconn(int shmid, void** ptr);
int mshmdisconn(void** ptr);

#endif

