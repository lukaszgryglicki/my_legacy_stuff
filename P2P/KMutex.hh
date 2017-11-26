#ifndef __HAVE_KMUTEX_H__
#define __HAVE_KMUTEX_H__
#include "Common.hh"
//used to mutual exclude threads from data
//used for synchronization
class KMutex
{
 public:
	 KMutex();
	 ~KMutex();
	 void Wait();
	 void Signal();
 private:
	 pthread_mutex_t mutex;	//system mutex
};
#endif
