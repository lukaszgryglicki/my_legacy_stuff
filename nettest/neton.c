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

#define ALRM_SECS 30

void catch_signal(int signo)
{
    return;
}

int get_host(char* to, char* name)	
{
    struct hostent* hose;
    struct in_addr addr;

    alarm(ALRM_SECS);
    hose = gethostbyname(name);
    alarm(0);

    if (!hose)
    {
	return 1;
    }

    memcpy((void*)&addr,(void*)(hose->h_addr_list[0]),sizeof(struct in_addr));
    strcpy(to, inet_ntoa(addr));

    return 0;
}

char neton()
{
    struct sockaddr_in server;
    int port, i, sock;
    char ip[16], buff[16];

    if (get_host(ip, "www.google.pl") != 0)
    {
	return '-';
    }

    port = 80;
    server.sin_family = AF_INET;	
    server.sin_port = (in_port_t)(htons(port));
    server.sin_addr.s_addr = inet_addr(ip);

    if (server.sin_addr.s_addr==(in_addr_t)(-1))
    {
	return '?';
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
	return 'S';
    }

    i = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &i, sizeof(void*)) < 0)
    {
	close(sock);
	return 'O';
    }

    alarm(ALRM_SECS);
    if (connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) < 0)
    {
	close(sock);
	alarm(0);
	return '!';
    }
    alarm(0);

    strcpy(buff, "GET HTTP/1.1\n");
    i = strlen(buff) + 1;

    alarm(ALRM_SECS);
    if (write(sock, (void*)buff, i) < i)
    {
	close(sock);
	alarm(0);
	return '>';
    }
    alarm(0);

    alarm(ALRM_SECS);
    if (read(sock, (void*)buff, 15) < 15)
    {
	close(sock);
	alarm(0);
	return '<';
    }
    alarm(0);
    buff[15] = 0;

    close(sock);
    return '+';
}

int main(int lb, char** par)
{
	char c;
        static struct sigaction act;

        act.sa_handler = catch_signal;

        sigfillset(&(act.sa_mask));

        sigaction(SIGALRM, &act, NULL);
        sigaction(SIGINT, &act, NULL);

        siginterrupt(SIGALRM, 1);

	c = neton();
	printf("%c\n", c);
	return (c != '+');
}
