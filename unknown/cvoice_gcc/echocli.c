#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAXSIZ 1024

void error(const char* fmt, ...)
{
	va_list lst;
	va_start(lst,fmt);
	printf("Error: \t");
	vprintf(fmt,lst);
	printf("\n\tClient halted\n");
	fflush(stdout);
	va_end(lst);
	exit(1);
}
void get_host(char* to, char* name)
{
	struct hostent* hose;
	struct in_addr addr;
	printf("DNS resolving IP for: %s\n", name);
	hose = gethostbyname(name);
	if (!hose)
	{
		perror("gethostbyname");
		error("Cannot get IP for hostname: %s\n", name);
	}
	memcpy((void*)&addr,(void*)(hose->h_addr_list[0]),sizeof(struct in_addr));
	strcpy(to, inet_ntoa(addr));
	printf("Got IP %s for hostname %s\n", to, name);
	fflush(stdout);
}

void client(char* ipstr, int port, int from, int to, int mode)
{
	int sock, n, done;
	struct sockaddr_in server;
	char buff[12], strline[1025];

	printf("Starting up client mode=%d\n", mode);

	if (port<1 || port>0xffff) error("Port: %d is invalid", port);
	if (mode != 0 && mode != 1 ) error("Bad mode value: %d. Try -h\n", mode);

	if (mode == 1)
	{
		printf("Going to background\n");
		if (fork()) return;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ipstr);

	if (server.sin_addr.s_addr == (in_addr_t)(-1)) error("IP %s is invalid.\n", ipstr);

	printf("Connecting to %s, at port %d, mode=%d from=%d, to=%d...\n", ipstr, port, mode, from, to);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)		
	{ 
		perror("socket"); 
		error("create socket failed."); 
	}

	if (connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1)
	{ 
		perror("connect"); 
		error("connect to %s:%d failed",ipstr,port); 
	}
	printf("connected.\n");
	fflush(stdout);

	memcpy((void*)&buff[0], (void*)(&from), sizeof(int));
	memcpy((void*)&buff[4], (void*)(&to), sizeof(int));
	memcpy((void*)&buff[8], (void*)(&mode), sizeof(int));

	if (write(sock,buff,12) == -1) error("write to socket");

	done = 0;
	if (mode == 0)		
	{
		while (!done)
		{ 
			n = fscanf(stdin, "%[^\n]\n", strline);
			if (n <= 0 || !strcmp(strline, "")) done = 1;
			else
			{
				if (n > 0) strcat(strline, "\n");
				if ((n = write(sock,strline,strlen(strline))) == -1) error("write");
			}
		}
	}
	else
	{
		while (!done)
		{
			n = read(sock, strline, 1024);
			if (n <= 0) 
			{
				if (n < 0) perror("read"); 
				done = 1;
			}
			else
			{
				strline[n] = 0;
				fprintf(stdout, "%s", strline);
			}
		}		
	}

	printf("Client done.\n");
	close(sock);
}

void help()
{
	printf("options are: -p port, -i ip, -s hostname -I -O (input/output) -f from_num -t to_num\n");
}


int main(int lb, char** par)
{
	char u;
	char ip[16];
	char port[12];
	int from, to, tp;

	strcpy(ip,"127.0.0.1");
	strcpy(port,"9000");
	tp = 0;
	from = 1001;
	to = 1002;

	printf("Starting client.., use -h to see available options...\n");

	while ((u = getopt(lb,par,"f:t:i:p:s:hIO"))!=-1)
	{
		switch (u)
		{
			case 'i': if (strlen(optarg)<16) strcpy(ip, optarg);      break;
			case 'I': tp = 0;										  break;
			case 'O': tp = 1;										  break;
			case 'f': from = atoi(optarg);							  break;
			case 't': to = atoi(optarg);							  break;
			case 'p': if (strlen(optarg)<12) strcpy(port, optarg);    break;
			case 's': get_host(ip, optarg);                           break;
			case 'h': help();										  return 0;
			default: printf("Unrecognized option\n");                 return 1;
		}
	}

	client(ip, atoi(port), from, to, tp);

	return 0;
}
