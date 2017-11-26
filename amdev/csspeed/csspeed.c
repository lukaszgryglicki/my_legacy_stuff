#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int COUNTER;
pthread_mutex_t LOCK;
pthread_mutex_t START;
pthread_cond_t CONDITION;

void* threads (void* unused) 
{
        pthread_mutex_lock(&START);
        pthread_mutex_unlock(&START);
        pthread_mutex_lock(&LOCK);
        if (COUNTER > 0) {
                pthread_cond_signal(&CONDITION);
        }
        for (;;) {
                COUNTER++;
                pthread_cond_wait(&CONDITION, &LOCK);
                pthread_cond_signal(&CONDITION);
        }
        pthread_mutex_unlock(&LOCK);
}

void cs_speed()
{
    	pthread_t t1, t2;
	struct timeval ti1, ti2, tsub;
	unsigned int t;

        pthread_mutex_lock(&START);
        COUNTER = 0;

        pthread_create(&t1, NULL, threads, NULL);
        pthread_create(&t2, NULL, threads, NULL);
        pthread_detach(t1);
        pthread_detach(t2);

        gettimeofday(&ti1, NULL);
        pthread_mutex_unlock(&START);
        sleep(10);
        // Lock both simulaneous threads
        pthread_mutex_lock(&LOCK);
        //Normalize the result in second precision
        gettimeofday(&ti2, NULL);
	timersub(&ti2, &ti1, &tsub);
	t = tsub.tv_sec * 1000000 + tsub.tv_usec;

	printf("COUNTER: %d, time: %d us, CS per us: %lf\n", COUNTER, t, (double)COUNTER / (double)t);
}

int main(int lb, char** par)
{
    cs_speed();
    return 0;
}

