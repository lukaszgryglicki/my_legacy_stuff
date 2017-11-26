static FILE* _ran_device;
static char* _ran_dev_name = "/dev/urandom";

int init_random()
{
 if ((_ran_device=fopen(_ran_dev_name,"r"))==NULL) return 0;
 else return 1;
}

int mrandom(int lb)
{
 int ins[4];
 for (int i=0;i<4;i++) ins[i] = fgetc(_ran_device);
 unsigned long result = ins[0] + 0x100*ins[1] + 0x10000*ins[2]+0x1000000*ins[3];
 result /= (0xffffffff/lb);
 return (int)result;
}

void kill_random()
{
 if (_ran_device) fclose(_ran_device);
}
