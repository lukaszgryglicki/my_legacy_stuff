#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#define mplayer_options " -geometry 800x600+1036+30 -ao oss -vo xv "
/*#define mplayer_options " -fs -ao oss -vo xv "*/
#define where_play "/data/xxx"

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
 char *temp, *syscmd;
 system("rm -f /tmp/temp.pls");
 for (i=1;i<lb;i++)
 {
     temp = (char*)malloc((strlen(par[i]) + 1) * 4);
     syscmd = (char*)malloc(((strlen(par[i]) + 1) * 4) + 200);
     make_wildcard(temp, par[i]);
     sprintf(syscmd, "find %s -name \"*%s*\" -exec ls \"{}\" >> \"/tmp/temp.pls\" \\;", where_play, temp);
     printf("%s\n", syscmd);
     system(syscmd);
     free((void*)temp);
     free((void*)syscmd);
 }
 syscmd = (char*)malloc(500);
/* sprintf(syscmd, "mplayer -playlist \"/tmp/temp.pls\" -fs -vf fspp=4:0:16 -vf pp=ha/va/dr/tn/al/lb/ci/l5 -ao oss -vo xv -fs -shuffle");*/
 sprintf(syscmd, "mplayer -playlist \"/tmp/temp.pls\" -shuffle %s ", mplayer_options);
 printf("%s\n", syscmd);
 free((void*)syscmd);
 system( syscmd );
 return 0;
}
