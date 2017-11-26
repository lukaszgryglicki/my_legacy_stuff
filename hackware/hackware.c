#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
    
void hackware_forever()
{
    struct timeval tv1, tv2, tvd;
    int ticks;
    unsigned short r;

    gettimeofday(&tv1, NULL);
    ticks = 0;

    while (0xffff)
    {
	 ticks ++;
/*	 sched_yield();*/

	 gettimeofday(&tv2, NULL);
    	 timersub(&tv2, &tv1, &tvd);
	 tvd.tv_usec /= 1000;

         if (tvd.tv_usec > 200)
         {
	     memcpy((void*)&tv1, (void*)&tv2, sizeof(struct timeval));
	     r = (unsigned short)(ticks / tvd.tv_usec);
             printf("\b\b\b\b\b\b%6d", r);
/*	     fwrite((void*)&r, sizeof(unsigned short), 1, stdout); */
	     fflush( stdout );
             ticks = 0;
	 }
	 
	 
    }
}

int main(int lb, char** par)
{
    hackware_forever();
    return 0;
}

