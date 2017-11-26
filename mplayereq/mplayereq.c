#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/rtprio.h>

#define PDIR "/data/dev/mplayereq/"

int load_preset(char* pn, int** v, float* spd, int* vol, int* prio)
{
    char* fn;
    FILE* f;
    int n;

    *v = NULL;
    *spd = 1.;
    *vol = 100;
    fn = (char*)malloc((strlen(pn) + strlen(PDIR) + 1)*sizeof(char));

    sprintf(fn, "%s%s", PDIR, pn);
    f = fopen(fn, "r+");

    if (!f)
    {
	printf("Cannot open file: %s\n", fn);
	return 0;
    }

    *v = (int*)malloc(10*sizeof(int));
    n = fscanf(f, "eq=(%d %d %d %d %d %d %d %d %d %d) speed=%f volume=%d prio=(%d,%d)\n", 
	    &((*v)[0]), &((*v)[1]), &((*v)[2]), &((*v)[3]), &((*v)[4]), 
	    &((*v)[5]), &((*v)[6]), &((*v)[7]), &((*v)[8]), &((*v)[9]),
	    &(*spd), &(*vol), &prio[0], &prio[1]);
    fclose(f);

    if (n < 14)
    {
	printf("Cannot scan 14 values from file: %s, scanned: %d\n", fn, n);
	free((void*)(*v));
	*v = NULL;
	return 0;
    }

    *vol /= 5.;

    return 1;
}

int mplayereq(int lb, char** par)
{
    int *v, i, l, vol, prio[2];
    char* cmd;
    float s;
    uid_t uid;
    struct rtprio rts;

    if (!load_preset(par[1], &v, &s, &vol, prio))
    {
	printf("Cannot load preset: %s, preset dir: %s\n", par[1], PDIR);
	return 1;
    }

    uid = geteuid();

    /*
    if (setpriority(PRIO_PROCESS, 0,-20) != 0)
    {
	printf("Failed to set high priority for user %d, you may need to set SUID bit\n", uid);
	perror("setpriority");
    }
    */


    if (prio[0] == -1) rts.type = RTP_PRIO_IDLE;
    else if (prio[0] == 0) rts.type = RTP_PRIO_NORMAL;
    else if (prio[0] == 1) rts.type = RTP_PRIO_REALTIME;
    else
    {
	printf("Unknown 1st prio value: %d, allowed: -1=idle, 0=normal, 1=realtime\n", prio[0]);
	return 1;
    }

    if (prio[1] < 0 || prio[1] > RTP_PRIO_MAX)
    {
	printf("Unknown 2nd prio value: %d, allowed are 0-%d\n", prio[1], RTP_PRIO_MAX);
	return 1;
    }

    rts.prio = prio[1];
    if (rtprio(RTP_SET, 0, &rts) != 0)
    {
	printf("Failed to set realtime priority for user %d, you may need to set SUID bit\n", uid);
	perror("setpriority");
    }

    l = 1024;
    for (i=2;i<lb;i++) l += strlen(par[i]) + 3;

    cmd = (char*)malloc(l*sizeof(char));

    sprintf(cmd, "mplayer -cache 8192 -speed %f -af equalizer=%d:%d:%d:%d:%d:%d:%d:%d:%d:%d,volnorm,extrastereo -softvol -softvol-max 500 -volstep 1 -volume %d ", 
	    s, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], vol);

    for (i=2;i<lb;i++)
    {
	if (strstr(par[i], " ")) strcat(cmd, "\""); 
	strcat(cmd, par[i]);
	if (strstr(par[i], " ")) strcat(cmd, "\""); 
	strcat(cmd, " ");
    }

    printf("%s\n", cmd);
    system(cmd);

    free((void*)cmd);
    cmd = NULL;
    return 0;
}

int main(int lb, char** par)
{
    if (lb <= 2)
    {
	printf("Usage: %s preset_name other_options\n", par[0]);
	printf("Presets in: %s\n", PDIR);
    }
    else return mplayereq(lb, par);
}

