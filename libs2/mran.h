/*
 default type for random is DEV_NULL wchich uses /dev/urandom this is initialized by randomize()
 or init_random(); possible is: randomize(); randomize(C_TMP); init_random(); init_random(C_TMP);
 to close random seed use kill_random();
 
 Type: DEV_NULL: starts with: randomize(); or randomize(DEV_NULL); or analogic init_random();
                 closes with: kill_random();
 by default uses /dev/urandom but function flag_uran(); changes it to /dev/random
 note that /dev/random is about 10-100x slower than /dev/urandom: if use flag_uran();
 again it will change /dev/random to /dev/urandom (it is just a switch) This function uses reopen();
 to reopen stream but if actually stream closes function does nothing.\n";
 Function type_ran(); tells you about type of random you are using see #defines


 Type: C_TMP starts with: randomize(C_TMP); or init_random(C_TMP); closes with kill_random();
 by default it is the fastest option but using flag_ctmp(); can change to slower but safer
 random seed (it is just switch as forward flag_uran()) works that: always when random(int) called
 writes data to /ctmp/__random gives you security even when program causes 'segmentation fault'
 seed was written and numbers are different than its were (in FAST option data is saved only with)
 kill_random(); so when program does not kill_random(); you will see the same numbers... :(
 to see current flag use type_ran();

 Speed:							 |  per milion:  |
 C_TMP    + FAST             10.000.000 ***  3,680       |  0,37 sec     |
 C_TMP    + SLOW             100.000    ***  16,96       |  2 min 49 sec |
 DEV_NULL + URANDOM          1.000.000  ***  6,225       |  6,225 sec    |
 DEV_NULL + RANDOM           100        ***  20,25 (*)   |  #! arg       | 


*/

static FILE* _ran_device;
static char _ran_dev_name[30];
static int _rval = 1;
static int r_type = 0;
static int __SLOW_RND = 0;
static int __URAN = 1;  


#define DEV_NULL 0
#define  C_TMP   1

#define CTMP_SLOW 0x111
#define CTMP_FAST 0x112
#define URANDOM   0x113
#define SRANDOM   0x114

int random(int);


void reopen()
{
 if (!_ran_device) return;
 fclose(_ran_device);
 if ((_ran_device = fopen(_ran_dev_name, "r")) == NULL) printf("RANLIB Ooops:\nCannot open device: %s\n", _ran_dev_name);
}

void flag_ctmp()
{
 if (r_type == C_TMP) {
 if (__SLOW_RND) __SLOW_RND = 0; else __SLOW_RND = 1; return;}
 printf("RANLIB Ooops:\nInvalid flag operation for /dev/urandom.\nTried to use option for C_TMP with /dev/random\n");
}

void flag_uran()
{
 if (r_type == DEV_NULL)
 {
  if (__URAN) {__URAN = 0; strcpy(_ran_dev_name, "/dev/random"); reopen(); return;}
  if (!__URAN) {__URAN = 1; strcpy(_ran_dev_name, "/dev/urandom"); reopen(); return;}
 }
 printf("RANLIB Ooops:\nInvalid flag operation for C_TMP.\nTried to use option for /dev/random with C_TMP\n");
}

int type_ran()
{
 if      ((r_type == C_TMP) && (__SLOW_RND))  return CTMP_SLOW;
 else if ((r_type == C_TMP) && (!__SLOW_RND)) return CTMP_FAST;
 else if ((r_type == DEV_NULL) && (__URAN))   return URANDOM;
 else if ((r_type == DEV_NULL) && (!__URAN))  return SRANDOM;
 else { printf("RANLIB Ooops:\nUsing unknown random type.\n"); return 0;}
}

bool init_random(int type = DEV_NULL)
{
 r_type = type;
 if (r_type == DEV_NULL)
 {
 strcpy(_ran_dev_name, "/dev/urandom");
 if ((_ran_device=fopen(_ran_dev_name,"r"))==NULL) {printf("RANLIB Ooops:\nCannot open device: %s", _ran_dev_name);return false;}
 return true;
 }
 else if (r_type == C_TMP)
 {
 FILE* ran;
 char* name = "/ctmp/__random";
 if ((ran = fopen(name,"r+")) == NULL)
        {
         if ((ran = fopen(name,"w")) == NULL) printf("RANLIB Ooops:\nFatal Error\nCannot create %s\n",name);
         else
                {
                 fprintf(ran,"%d",1);
                 fclose(ran);
                }
        }
  else
        {
         fscanf(ran,"%d",&_rval);
         fclose(ran);
        }
 short x = _rval;
 for (int i=0;i<x;i++) random(5);
 }
}

void randomize(int type = DEV_NULL)
{
 init_random(type);
}

void do_mess()
{
 _rval += 2;
 if (_rval>=0x1000) _rval = 0;
 FILE* ran;
 char* name = "/ctmp/__random";
 if ((ran = fopen(name,"w")) == NULL) printf("RANLIB Ooops:\nFatal Error\nCannot create %s\n",name);
 else
        {
         fprintf(ran,"%d",_rval);
         fclose(ran);
        }
}
int random(int lb)
 {
 if (r_type == DEV_NULL)
 {
 int ins[4];
 for (int i=0;i<4;i++) ins[i] = fgetc(_ran_device);
 unsigned long result = ins[0] + 0x100*ins[1] + 0x10000*ins[2]+0x1000000*ins[3];
 result /= (0xffffffff/lb);
 return (int)result;
 }
 else if (r_type == C_TMP)
 {
 if (__SLOW_RND) do_mess();
 if (lb<=0) return -1;
 else return (random()) / (0x88888888 / lb);
 }
 }

void kill_random()
 {
  if (r_type == DEV_NULL)
  {
  if (_ran_device) fclose(_ran_device);
  }
  else if (r_type == C_TMP)
  {
 _rval += random(20)+10;
 if (_rval>=0x1000) _rval = 0;
 FILE* ran;
 char* name = "/ctmp/__random";
 if ((ran = fopen(name,"w")) == NULL) printf("RANLIB Ooops:\nFatal Error\nCannot create %s\n",name);
 else
        {
         fprintf(ran,"%d",_rval);
         fclose(ran);
        }
  }
 }



