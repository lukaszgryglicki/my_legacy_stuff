#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define mplayer_options " -ao oss -vo xv -geometry 800x600 "

void make_wildcard(char* t, char* w)
{
 int i, n;
 char l[5];
 n = strlen( w );
 t[0] = 0;
 for (i=0;i<n;i++)
 {
     if (w[i] >= 'a' && w[i] <= 'z')
     {
	 l[0] = '[';
	 l[1] = w[i];
	 l[2] = w[i] - 0x20;
	 l[3] = ']';
	 l[4] = 0;
	 strcat(t, l);
     }
     else if (w[i] >= 'A' && w[i] <= 'Z')
     {
	 l[0] = '[';
	 l[1] = w[i] + 0x20;
	 l[2] = w[i];
	 l[3] = ']';
	 l[4] = 0;
	 strcat(t, l);
     }
     else
     {
	 l[0] = w[i];
	 l[1] = 0;
	 strcat(t, l);
     }

 }
}


int main(int lb, char** par)
{
 printf("usage: %s word [word2 ...]\n", par[0]);
 if (lb < 2) return;
 int i;
 char *temp, *syscmd, *cwd;
 system("rm -f /tmp/temp_cwd.pls");
 cwd = NULL;
 cwd = getwd( cwd );
 for (i=1;i<lb;i++)
 {
     temp = (char*)malloc((strlen(par[i]) + 1) * 4);
     syscmd = (char*)malloc(((strlen(par[i]) + 1) * 4) + 200);
     make_wildcard(temp, par[i]);
     /*sprintf(syscmd, "find %s -path \"*%s*\" -exec ls \"{}\" >> \"/tmp/temp_cwd.pls\" \\;", cwd, temp);*/
     sprintf(syscmd, "find %s -path \"*%s*\"  >> /tmp/temp_cwd.pls", cwd, temp);
     printf("%s\n", syscmd);
     system(syscmd);
     free((void*)temp);
     free((void*)syscmd);
 }
 syscmd = (char*)malloc(500);
 sprintf(syscmd, "mplayer -playlist \"/tmp/temp_cwd.pls\" -shuffle %s ", mplayer_options);
 printf("%s\n", syscmd);
 free((void*)syscmd);
 system( syscmd );
 return 0;
}
