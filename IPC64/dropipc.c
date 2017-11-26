
#include "libmipc.h"


int main(int lb, char** par)
{
 if (mdropsemkey(512)==-1) warn("Cannot drop semaphore  by key: %d",512);
 if (mdropmsgkey(512)==-1) warn("Cannot drop msgquery   by key: %d",512);
 if (mdropshmkey(512)==-1) warn("Cannot drop shmsegment by key: %d",512);
 if (mdropsemkey(513)==-1) warn("Cannot drop semaphore  by key: %d",513);
 if (mdropmsgkey(513)==-1) warn("Cannot drop msgquery   by key: %d",513);
 if (mdropshmkey(513)==-1) warn("Cannot drop shmsegment by key: %d",513);
 if (mdropsemkey(514)==-1) warn("Cannot drop semaphore  by key: %d",514);
 if (mdropmsgkey(514)==-1) warn("Cannot drop msgquery   by key: %d",514);
 if (mdropshmkey(514)==-1) warn("Cannot drop shmsegment by key: %d",514);
 if (mdropsemkey(515)==-1) warn("Cannot drop semaphore  by key: %d",515);
 if (mdropmsgkey(515)==-1) warn("Cannot drop msgquery   by key: %d",515);
 if (mdropshmkey(515)==-1) warn("Cannot drop shmsegment by key: %d",515);
 return 0;
}
