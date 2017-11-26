/////////////////////////////////////////////////
//////// ********************************////////
///// **************************************/////
//// ****  HTTP  ****************************////
/// ***********  Server  *********************///
// ***********************  in C++  ***********//
// ********************************************//
/// **### StudentSoft copyleft@ ###***********///
//// *************   BSD License   **********////
///// **************************************/////
//////// ********************************////////
/////////////////////////////////////////////////
#include "XSemaphore.hh"
#include "Common.hh"
#include "Definitions.hh"

///
//	initializes semaphore, if such exists destroy
//	it WLL NOT destroy semaphores of  working server
//	because it is called after creation of sockets
//	and sockets will not be created because they are USED
//	by server instantion
//	semaphore key is (and SHM) is choosen as
//	(client_portnum<<0x10)+admin_portnum
//	so it is UNIQUE for all instantions of server
//	running on different ports because maximum port is 0XFFFF
//	this comment is also on XShM :: XShM()
////
XSemaphore :: XSemaphore(const key_t semk, const int val)
{
 int status,id;
 status = 0;
 assert(val==0 || val==1);
 assert(semk>0 && semk<=MAX_KEY_T);
 id = SemIDFromKey(semk);
 if (id>=0) semctl(id,IPC_RMID,0);
 if ((id=semget(semk,1,0660|IPC_CREAT|IPC_EXCL))==-1)
   {
    printf("FATAL error, cannot create IPC: Semaphore\n");
    exit(4);
   }
 union msemun arg;
 arg.val = val;
 status = semctl(id,0,SETVAL,arg);
 assert(id!=-1 && status!=-1);
 semid = id;
 semkey = semk;
}

////
//	destroy class and IPC object: semaphore
////
XSemaphore :: ~XSemaphore()
{
 semctl(semid,IPC_RMID,0);
}

////
//	internal: get semaphore ID using its KEY
////
int XSemaphore :: SemIDFromKey(const key_t semkey) const
{
 return semget(semkey,1,0660);
}

////
//	wait on semaphore, this guaranties
//	MUTUAL EXCLUSION
//	after this we're is CRITICAL SECTION
//	and NO other client can access this area
//	simultaneusly
////
void XSemaphore :: Wait()
{
 struct sembuf buf;
 buf.sem_num = 0;
 buf.sem_op = -1;
 buf.sem_flg = SEM_UNDO;
/* Debug("WAIT FOR MUTEX\n");*/
 assert(semop(semid,&buf,1)!=-1);
/* Debug("CS GRANTED.\n");*/
}

////
//	signal the semaphore, this will wake up
//	one of awaiting clients, or if none such
//	just set the semaphore free
////
void XSemaphore :: Signal()
{
 struct sembuf buf;
 buf.sem_num = 0;
 buf.sem_op =  1;
 buf.sem_flg = SEM_UNDO;
/* Debug("SIGNALLING MUTEX\n");*/
 assert(semop(semid,&buf,1)!=-1);
/* Debug("MUTEX SIGNALLED.\n");*/
}

