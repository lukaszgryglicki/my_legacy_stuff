#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* define it for UNIX-like sytem, commentout for Cygwin */
#define UNIX

#ifdef UNIX
#include <sys/types.h>
#include <unistd.h>
#endif

void killer(int lb, char** par)
{
	FILE* fp;
	char **t;
	int i, pid, finished, ns, n, ownpid;


	ownpid = (int)getpid();
	ns = 16;
	t  = (char**)malloc(ns * sizeof(char*));

	for (i=0;i<ns;i++) t[i] = (char*)malloc(1024 * sizeof(char));

	while (1)
	{
		if (lb == 1)
		{
#ifdef UNIX
			system("ps -ax | grep -i \"nVision\" > plist.out");
			system("ps -ax | grep -i \"AnuTest\" >> plist.out");
#else
			system("ps -W | grep -i \"nVision\" > plist.out");
			system("ps -W | grep -i \"AnuTest\" >> plist.out");
#endif
		}
		else 
		{
			system("> plist.out");
			for (i=1;i<lb;i++) 
			{
#ifdef UNIX
				sprintf(t[0], "ps -ax | grep -i \"%s\" >> plist.out", par[i]);
#else
				sprintf(t[0], "ps -W | grep -i \"%s\" >> plist.out", par[i]);
#endif
				system( t[0] );
/*				printf("%s\n", t[0]);*/
			}
		}

		system("echo \"end\" >> plist.out");

		fp = fopen("plist.out", "r");

		if (fp)
		{
			finished = 0;
	
			while (!finished)
			{
#ifdef UNIX
			        n = 5;
				i = fscanf(fp, "%s %s %s %s %[^\n]\n", t[0], t[1], t[2], t[3], t[4]);
/*				printf("%d: %s\n", i, t[n-1]);*/
/*				if (strstr(t[n-1], par[0])) continue;*/
/*				if (strncmp(t[n-1], "grep", 4)) continue;*/
/*				if (strncmp(t[n-1], "[grep", 5)) continue;*/
#else
			        n = 8;
				i = fscanf(fp, "%s %s %s %s %s %s %s %[^\n]\n", t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7]);
#endif

				if (i == n)
				{

					pid = -1;
					sscanf(t[0], "%d", &pid);
	
					if (pid > 0 && pid != ownpid) 
					{
/*						printf("PID to kill: %d [%s]\n", pid, t[n-1]);*/
#ifdef UNIX
						sprintf(t[0], "kill -9 %d 1> err 2>&1", pid);
#else
						sprintf(t[0], "taskkill /F /PID %d > err", pid);
#endif
						system( t[0] );
						/*sleep(1);*/
					}
				}
				else finished = 1;
			}
	
			fclose(fp);
			unlink("plist.out");
			unlink("err");
		}
	}
}

int main(int lb, char** par)
{
	killer(lb, par);
	return 0;
}
