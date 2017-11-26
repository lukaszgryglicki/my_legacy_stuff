#include "Common.hh"
#include "KMutex.hh"

//creates and initializes mutex routine
KMutex :: KMutex()
{
 pthread_mutexattr_t mutex_attr;
 assert(!pthread_mutexattr_init(&mutex_attr));
 assert(!pthread_mutex_init(&mutex, (const pthread_mutexattr_t*)(&mutex_attr)));
}

//destroys mutex
KMutex :: ~KMutex() 
{
 pthread_mutex_destroy(&mutex);
}

//waits for mutex
//SEMAPHORE operation P()
void KMutex :: Wait()
{
#ifdef DEBUG
 printf("locking mutex...\n");
#endif
 pthread_mutex_lock(&mutex);
#ifdef DEBUG
 printf("mutex locked...\n");
#endif
}

//signals mutex
//SEMAPHORE operation V()
void KMutex :: Signal()
{
#ifdef DEBUG
 printf("unlocking mutex...\n");
#endif
 pthread_mutex_unlock(&mutex);
#ifdef DEBUG
 printf("mutex unlocked...\n");
#endif
}

