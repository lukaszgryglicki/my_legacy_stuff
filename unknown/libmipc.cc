#include "libmipc.h"

int fatal(const char* fmt, ...)
{
 va_list ap;
 va_start(ap,fmt);
 printf("\n\npanic\n\t%c%cfatal: ",7,7);
 vprintf(fmt,ap);
 printf("\napplication terminated.%c%c\n\n", 7,7);
 va_end(ap);
 exit(1);
}

int warn(const char* fmt, ...)
{
 va_list ap;
 int rt;
 va_start(ap,fmt);
 printf("warning: ");
 rt=vprintf(fmt,ap);
 printf("\n");
 va_end(ap);
 return rt;
}

int error(const char* fmt, ...)
{
 va_list ap;
 int rt;
 va_start(ap,fmt);
 printf("\n\nerror:\n\t ");
 rt=vprintf(fmt,ap);
 printf("\n\n");
 va_end(ap);
 return rt;
}

int msemidfromkey(key_t semkey)
{
 int ret;
 ret=semget(semkey,1,0660);
 if (ret==-1) warn("semget in msemidfromkey, key=%d", semkey);
 return ret;
}

int msemget(key_t semk)
{
 int semid;
 semid = semget(semk,0,0660);
 if (semid==-1) warn("semaphore don't exists, msemget:key=%d", semk);
 return semid;
}


int msemnew(key_t semk, int val, int* news)
{
 int status,semid;
 status = 0;
 *news = 1;
 if (val<0) { warn("in msem, val below zero: %d", val); return -1; }
 if ((semid=semget(semk,1,0660|IPC_CREAT|IPC_EXCL))==-1)
   {
    if (errno==EEXIST) { semid = semget(semk,0,0660); *news=0; }
    else perror("semget in msem");
   }
 else
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
 return semid;
}

int msem(key_t semk, int val)
{
 int status,semid;
 status = 0;
 if (val<0) { warn("in msem, val below zero: %d", val); return -1; }
 if ((semid=semget(semk,1,0660|IPC_CREAT|IPC_EXCL))==-1)
   {
    if (errno==EEXIST) semid = semget(semk,0,0660);
    else perror("semget in msem");
   }
 else
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
 return semid;
}

int mwait(int semid)
{
 struct sembuf buf;
 buf.sem_num = 0;
 buf.sem_op = -1;
 buf.sem_flg = SEM_UNDO;
 if (semop(semid,&buf,1)==-1)
   {
    perror("semop in mwait");
    return -1;
   }
 return 0;
}

int msignal(int semid)
{
 struct sembuf buf;
 buf.sem_num = 0;
 buf.sem_op =  1;
 buf.sem_flg = SEM_UNDO;
 if (semop(semid,&buf,1)==-1)
   {
    perror("semop in msignal");
    return -1;
   }
 return 0;
}

int mdropsem(int semid)
{
 if (semctl(semid,IPC_RMID,(int)NULL)==-1) { warn("mdropsem failed for semid %d", semid); return -1; }
 else return 0;
}

int mdropsemkey(int semkey)
{
 int semid;
 if ((semid=semget(semkey,1,0660))==-1) { warn("semget in mdropsemkey"); return -1; }
 if (semctl(semid,IPC_RMID,(int)NULL)==-1) { warn("semctl in mdropsemkey"); return -1; }
 else return 0;
}

int mque(key_t quek)
{
 int queid;
 if ((queid=msgget(quek,IPC_CREAT|IPC_EXCL|0660))==-1)
   {
    if (errno==EEXIST) queid=msgget(quek, 0660);
    else { perror("msgget in mque"); return -1; }
   }
 return queid;
}

int mquenew(key_t quek, int* newq)
{
 int queid;
 *newq = 1;
 if ((queid=msgget(quek,IPC_CREAT|IPC_EXCL|0660))==-1)
   {
    if (errno==EEXIST) { queid=msgget(quek, 0660); *newq = 0; }
    else { perror("msgget in mque"); return -1; }
   }
 return queid;
}

int mdropmsg(int msgid)
{
 if (msgctl(msgid,IPC_RMID,NULL)==-1) { warn("msgctl in mdropmsg"); return -1; }
 else return 0;
}

int mdropmsgkey(int msgkey)
{
 int msgid;
 if ((msgid=msgget(msgkey,0660))==-1) { warn("msgget in mdropmsgkey"); return -1; }
 if (msgctl(msgid,IPC_RMID,NULL)==-1) { warn("msgctl in mdropmsgkey"); return -1; }
 else return 0;
}

int mmsgsendtxt(int msgid, char* text, int pri)
{
 int len;
 struct mmsg msg;
 len = strlen(text);
 if (len>=MSGSIZ) { error("text: %s too long"); return -1; }
 if (pri<0)       { error("priority below 0: %d", pri); return -1; }
 if (pri>=PRIOR)  { error("priority too high: %d", pri); return -1; }
 msg.mtype = pri;
 strncpy(msg.mtext, text, MSGSIZ);
 if (msgsnd(msgid,&msg,len,0)==-1) { perror("msgsnd in mmsgsendtxt"); return -1; }
 return 0;
}

int mmsgsendstr(int msgid, struct mmsg* msg)
{
 int len,type;
 len = strlen(msg->mtext);
 type = msg->mtype;
 if (len>=MSGSIZ) { error("text: %s too long"); return -1; }
 if (type<0)      { error("priority below 0: %d", type); return -1; }
 if (type>=PRIOR) { error("priority too high: %d", type); return -1; }
 if (msgsnd(msgid,msg,len,0)==-1) { perror("msgsnd in mmsgsendstr"); return -1; }
 return 0;
}

int mmsgrecvtxt(int msgid, char* txt, int type, int wt)	/* GET OF GIVEN TYPE, wt:wait for such? */
{							/* MEM FOR TEXT MUST BE ALLOCATED, MSGSIZ+1 bytes */
 int len;
 struct mmsg msg;
 if (wt)		/* IF WAIT SET */
   {
    if ((len=msgrcv(msgid,&msg,MSGSIZ,type,MSG_NOERROR))==-1) { perror("msgrcv in mmsgrecvtxt"); return -1; }
    if (len>=MSGSIZ) { perror("message too long, mmsgrecvtxt"); return -1; }
    msg.mtext[len] = 0;
    strcpy(txt, msg.mtext);
    return 0;
   }
 else			/* RETURN IMMADIATELY */
  {
    if ((len=msgrcv(msgid,&msg,MSGSIZ,type,MSG_NOERROR|IPC_NOWAIT))==-1)
    {
     if (errno==EAGAIN) return 1;	/* NEED ONCE AGAIN */
     else { perror("msgrcv in mmsgrecvtxt"); return -1; }
		     			/* ERROR */
    }
    if (len>=MSGSIZ) { perror("message too long, mmsgrecvtxt"); return -1; }
    msg.mtext[len] = 0;
    strcpy(txt, msg.mtext);
    return 0;				/* GOT IT */
  }
}

int mmsgrecvstr(int msgid, struct mmsg* msg, int wt)	/* MEM FOR STRUCT MUST BE ALLOCATED */
{
 int len;
 int type;
 type = msg->mtype;	/* initially set in struct */
 if (wt)		/* IF WAIT SET */
   {
    if ((len=msgrcv(msgid,&msg,MSGSIZ,type,MSG_NOERROR))==-1) { perror("msgrcv in mmsgrecvstr"); return -1; }
    /* needn't set 0 here, because struct can hold binary data */
    return 0;
   }
 else			/* RETURN IMMADIATELY */
  {
    if ((len=msgrcv(msgid,&msg,MSGSIZ,type,MSG_NOERROR|IPC_NOWAIT))==-1)
    {
     if (errno==EAGAIN) return 1;	/* NEED ONCE AGAIN */
     else { perror("msgrcv in mmsgrecvstr"); return -1;	}		
     					/* ERROR */
    }
    return 0;				/* GOT IT */
  }
}

int mmsgrecptxt(int msgid, char* txt, int wt)	/* 'recp' uses priority and fetch with minimal */
{
 int len;
 struct mmsg msg;
 printf("wt=%d\n", wt);
 fflush(stdout);
 if (wt)		/* IF WAIT SET */
   {
    if ((len=msgrcv(msgid,&msg,MSGSIZ,(-PRIOR),MSG_NOERROR))==-1) { perror("msgrcv in mmsgrecptxt"); return -1; }
    if (len>=MSGSIZ) { perror("message too long, mmsgrecptxt"); return -1; }
    msg.mtext[len] = 0;
    strcpy(txt, msg.mtext);
    return 0;
   }
 else			/* RETURN IMMADIATELY */
  {
    if ((len=msgrcv(msgid,&msg,MSGSIZ,(-PRIOR),MSG_NOERROR|IPC_NOWAIT))==-1)
    {
     if (errno==EAGAIN) return 1;	/* NEED ONCE AGAIN */
     else { perror("msgrcv in mmsgrecptxt"); return -1; }
     					/* ERROR */
    }
    if (len>=MSGSIZ) { perror("message too long, mmsgrecptxt"); return -1; }
    msg.mtext[len] = 0;
    strcpy(txt, msg.mtext);
    return 0;				/* GOT IT */
  }
}

int mmsgrecpstr(int msgid, struct mmsg* msg, int wt)	/* MEM FOR STRUCT MUST BE ALLOCATED */
{
 int len;
 if (wt)		/* IF WAIT SET */
   {
    if ((len=msgrcv(msgid,&msg,MSGSIZ,(-PRIOR),MSG_NOERROR))==-1) { perror("msgrcv in mmsgrecpstr"); return -1; }
    /* needn't set 0 here, because struct can hold binary data */
    return 0;
   }
 else			/* RETURN IMMADIATELY */
  {
    if ((len=msgrcv(msgid,&msg,MSGSIZ,(-PRIOR),MSG_NOERROR|IPC_NOWAIT))==-1)
    {
     if (errno==EAGAIN) return 1;	/* NEED ONCE AGAIN */
     else { perror("msgrcv in mmsgrecpstr"); return -1;	}
     					/* ERROR */
    }
    return 0;				/* GOT IT */
  }
}

int mmsgidfromkey(key_t msgkey)
{
 int ret;
 ret = msgget(msgkey,0660);
 if (ret==-1) warn("msgget in mmsgidfromkey, key=%d", msgkey);
 return ret;
}

int mshm(key_t shmk, int size)
{
 int shmid;
 if ((shmid=shmget(shmk,size,IPC_CREAT|IPC_EXCL|0660))==-1)
   {
    if (errno==EEXIST) shmid=shmget(shmk, size,0660);
    else { perror("shmget in mshm"); return -1; }
   }
 return shmid;
}

int mshmnew(key_t shmk, int size, int* newm)
{
 int shmid;
 *newm = 1;
 if ((shmid=shmget(shmk,size,IPC_CREAT|IPC_EXCL|0660))==-1)
   {
    if (errno==EEXIST) { shmid=shmget(shmk, size,0660); *newm = 0; }
    else { perror("shmget in mshm"); return -1; }
   }
 return shmid;
}

int mdropshm(int shmid)
{
 if (shmctl(shmid,IPC_RMID,NULL)==-1) { warn("shmctl in mdropshm"); return -1; }
 else return 0;
}

int mdropshmkey(int shmkey)
{
 int shmid;
 if ((shmid=shmget(shmkey,1,0660))==-1) { warn("shmget in mdropshmkey"); return -1; }
 if (shmctl(shmid,IPC_RMID,NULL)==-1)   { warn("shmctl in mdropshmkey"); return -1; }
 else return 0;
}

int mshmidfromkey(key_t shmkey)		/* SIZE ISN'T KNOW !! */
{
 int ret;
 ret = shmget(shmkey,1,0660);
 if (ret==-1) warn("shmget in mshmidfromkey, key=%d", shmkey);
 return ret;
}

int mshmconn(int shmid, void** ptr)
{
 if ((*ptr=shmat(shmid,0,0))==(void*)(-1)) { warn("shmat in mshmconn"); return -1; }
 else return 0;
}

int mshmdisconn(void** ptr)
{
 if (shmdt(*ptr)==-1) { warn("shmdt in mshmdisconn"); *ptr = 0; return -1; }
 else { *ptr=0; return 0; }
}

