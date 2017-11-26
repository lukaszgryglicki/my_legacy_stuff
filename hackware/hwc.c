#include <stdio.h>
#include <signal.h>

#define REFRESH 	5000000
#define DELTA 		200000
#define NUMD  (REFRESH / DELTA)

register volatile unsigned long ticks asm("edi");
/*unsigned long ticks;*/
float gops;
int ft;
int av;

void catch_int(int signo)
{
     if (ft)
     {
	 fflush(stdout);
	 ft = 0;
	 ticks = 0;
	 av = 0;
	 return;
     }

     av ++;
     gops += (float)(ticks * (1000000 / DELTA)) / 1000000000.;
/*     printf("%d %d %f\n", ticks, av, gops);*/
     ticks = 0;

     if (av == NUMD)
     {
	 gops /= (float)NUMD;
	 printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%12.6f GOPS", gops);
	 fflush(stdout);
	 av = 0;
	 gops = 0.;
     }
}

void signal_setup()
{
    static struct sigaction act;
    act.sa_handler = catch_int;
    sigfillset(&(act.sa_mask));
    sigaction(SIGALRM, &act, NULL);
}
    
void hwc()
{
    signal_setup();

    ft = 1;
    ticks = 0;
    av = 0;

    ualarm(DELTA, DELTA);

    while (1)
    {
	ticks ++;
    }
}

int main(int lb, char** par)
{
    hwc();
    return 0;
}

