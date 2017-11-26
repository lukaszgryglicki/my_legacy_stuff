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
#ifndef __HAVE_CLASS_SEMAPHORE__
#define __HAVE_CLASS_SEMAPHORE__
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
union msemun
{
 int    val;
 struct semid_ds *buf;
 unsigned short* array;
};

class XSemaphore
{
 public:
	 XSemaphore(const key_t, const int);
	 ~XSemaphore();
	 void Wait();
	 void Signal();
 private:
	 int SemIDFromKey(const key_t) const;
 private:
	 int semid;
	 key_t semkey;
};
#endif

