#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define SECTSIZ 0x200

int rd(char* where, int from, int n)
{
 int disk,i;
 char buff[SECTSIZ+1];
 if (!where) 
   { 
    printf("no disk to search on!\n"); 
    return 2; 
   }
 disk = open(where, O_RDONLY);
 if (disk==-1) 
   { 
    printf("cannot open disk given!\n"); 
    perror("open");
    return 4; 
   }
 if (lseek(disk, from*SECTSIZ,SEEK_SET)==-1)
   {
    perror("lseek");
    return 8;
   }
 for (i=0;i<n;i++)
   {
    if (read(disk, buff, SECTSIZ)!=SECTSIZ)
      {
        perror("read");
	return 0x10;
      }
    if (write(1, buff, SECTSIZ)!=SECTSIZ)
      {
        perror("write");
	return 0x20;
      }
   }
 close(disk);
 return 0;
}

int main(int lb, char** par)
{
 if (lb<4) 
   {
    printf("usage:\t%s file_disk from nsects\n", par[0]); 
    printf("writes to stdout.\n");
    return 1;
   }
 return rd(par[1], atoi(par[2]), atoi(par[3]));
}

