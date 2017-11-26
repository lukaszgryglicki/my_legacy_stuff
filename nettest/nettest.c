/* Written by MorgothDBMA: morgothdbma@o2.pl, tel +48693582014 */
/* license: BSD */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#define ALRM_SECS 8
#define SLEEP_PERIOD 30

static struct timeval t_gh, t_c, t_w, t_r;
static char* hdir;
static int usehome;

void oflush(char* str)
{
    time_t tm;
    FILE* fp;
    unsigned long lgh, lc, lr, lw;
    char fname[1024];

    time(&tm);
    fprintf(stdout, "%s", str);
    fflush(stdout);

    if (usehome)
    {
        sprintf(fname, "%s/.nettest/network.db", hdir);
    }
    else
    {
	strcpy(fname, hdir);
    }

    fp = fopen(fname, "a");

    if (!fp) 
    { 
	perror("fopen");
	printf("Error creating/opening network.db\n"); 
	exit(2);
    }

    lgh = t_gh.tv_sec * 1000000 + t_gh.tv_usec;
    lc  = t_c.tv_sec  * 1000000 + t_c.tv_usec;
    lw  = t_w.tv_sec  * 1000000 + t_w.tv_usec;
    lr  = t_r.tv_sec  * 1000000 + t_r.tv_usec;

/*    fprintf(fp, "%08lx %s %9ld %9ld %9ld %9ld\n", (unsigned long)tm, str, lgh, lc, lw, lr);*/

    fwrite((void*)&tm, sizeof(time_t), 1, fp);
    fwrite((void*)str, 1, 1, fp);
    fwrite((void*)&lgh, sizeof(unsigned long), 1, fp);
    fwrite((void*)&lc, sizeof(unsigned long), 1, fp);
    fwrite((void*)&lw, sizeof(unsigned long), 1, fp);
    fwrite((void*)&lr, sizeof(unsigned long), 1, fp);

    fclose(fp);
}

void catch_signal(int signo)
{
    if (signo == SIGINT)
    {
	oflush("C");
	exit(1);
    }
    if (signo == SIGALRM)
    {
/*	oflush(".");*/
/*	alarm(ALRM_SECS);*/
    }
}

int get_host(char* to, char* name)	
{
    struct hostent* hose;
    struct in_addr addr;
    struct timeval t1, t2;

    alarm(ALRM_SECS);
    gettimeofday(&t1, NULL);
    hose = gethostbyname(name);
    gettimeofday(&t2, NULL);
    alarm(0);
    timersub(&t2, &t1, &t_gh);

    if (!hose)
    {
	return 1;
    }

    memcpy((void*)&addr,(void*)(hose->h_addr_list[0]),sizeof(struct in_addr));
    strcpy(to, inet_ntoa(addr));

    return 0;
}

void nettest()
{
    struct sockaddr_in server;
    struct timeval t1, t2;
    int port, i, sock;
    char ip[16], buff[16];

    timerclear(&t_gh);
    timerclear(&t_c);
    timerclear(&t_w);
    timerclear(&t_r);

    if (get_host(ip, "www.google.pl") != 0)
    {
	oflush("-");
	return;
    }

    port = 80;
    server.sin_family = AF_INET;	
    server.sin_port = (in_port_t)(htons(port));
    server.sin_addr.s_addr = inet_addr(ip);

/*    printf("%d %d (%s)\n", server.sin_port, server.sin_addr.s_addr, ip);*/

    if (server.sin_addr.s_addr==(in_addr_t)(-1))
    {
	oflush("!1");
	return;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
	oflush("!2");
	return;
    }

    i = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &i, sizeof(void*)) < 0)
    {
	close(sock);
	oflush("!3");
	return;
    }

    alarm(ALRM_SECS);
    gettimeofday(&t1, NULL);
    if (connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) < 0)
    {
/*	perror("connect");*/
	close(sock);
	oflush("!");
	alarm(0);
	return;
    }
    gettimeofday(&t2, NULL);
    timersub(&t2, &t1, &t_c);
    alarm(0);

    strcpy(buff, "GET HTTP/1.1\n");
    i = strlen(buff) + 1;

    alarm(ALRM_SECS);
    gettimeofday(&t1, NULL);
    if (write(sock, (void*)buff, i) < i)
    {
/*	perror("write");*/
	close(sock);
	oflush(">");
	alarm(0);
	return;
    }
    gettimeofday(&t2, NULL);
    timersub(&t2, &t1, &t_w);
    alarm(0);

    alarm(ALRM_SECS);
    gettimeofday(&t1, NULL);
    if (read(sock, (void*)buff, 15) < 15)
    {
/*	perror("read");*/
	close(sock);
	oflush("<");
	alarm(0);
	return;
    }
    gettimeofday(&t2, NULL);
    timersub(&t2, &t1, &t_r);
    alarm(0);
    buff[15] = 0;

/*    printf("%s\n", buff);*/

    close(sock);
    oflush("+");
    return;
}

int main(int lb, char** par)
{
    static struct sigaction act;

    act.sa_handler = catch_signal;

    sigfillset(&(act.sa_mask));

    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGINT, &act, NULL);

    siginterrupt(SIGALRM, 1);

    if (lb == 1)
    {
        hdir = getenv("HOME");
        if (!hdir)
        {
	    printf("Current user is homeless, cannot fetch config\n");
	    exit(3);
        }
	usehome = 1;
    }
    else
    {
	hdir = (char*)malloc((strlen(par[1])+1)*sizeof(char));
	strcpy(hdir, par[1]);
	usehome = 0;
    }

    while (1)
    {
	nettest();
	sleep(SLEEP_PERIOD);
    }
    return 0;
}
