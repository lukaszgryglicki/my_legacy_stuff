
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

void error(char* fmt, ...)
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

void client(char* ipstr, int port, int num)
{
 int sock,n;
 struct sockaddr_in server;
 char buff[MAXSIZ+1];
 printf("Starting up client num=%d\n", num);
 if (port<1 || port>0xffff) error("Port: %d is invalid", port);
 if (num <=0 ) error("Bad num value: %d. Try -h\n", num);
 server.sin_family = AF_INET;
 server.sin_port = htons(port);
 server.sin_addr.s_addr = inet_addr(ipstr);
 if (server.sin_addr.s_addr==(in_addr_t)(-1)) error("IP %s is invalid.\n", ipstr);
 printf("Connecting to %s, at port %d...\n", ipstr, port);
 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock==-1)
      { perror("socket"); error("create socket failed."); }
 if (connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in))==-1)
      { perror("connect"); error("connect to %s:%d failed",ipstr,port); }
 printf("connected.\n");
 fflush(stdout);
 sprintf(buff, "%d", num);
 if (write(sock,buff,strlen(buff))==-1) error("write to socket");
 if ((n=read(sock,buff,MAXSIZ))==-1) error("read from socket");
 if (n==0) { printf("server didn't answer correctly!\n"); return; }
 buff[n] = 0;
 printf("Server evaluation for num=%d is %s\n", num, buff);
 close(sock);
}

void help()
{
 printf("options are: -p PORT, -i IPNUM, -s HOSTNAME -n NUM\n");
}


int main(int lb, char** par)
{
 char u;
 char ip[16];
 char port[12];
 char num[16];
 strcpy(ip,"127.0.0.1");
 strcpy(port,"6666");
 strcpy(num,"0");
 printf("Starting client.., use -h to see available options...\n");
 while ((u = getopt(lb,par,"i:p:s:n:h"))!=-1)
 {
  switch (u)
   {
    case 'i': if (strlen(optarg)<16) strcpy(ip, optarg);      break;
    case 'n': if (strlen(optarg)<16) strcpy(num, optarg);     break;
    case 'p': if (strlen(optarg)<12) strcpy(port, optarg);    break;
    case 's': get_host(ip, optarg);                           break;
    case 'h': help(); return 0;
    default: printf("Unrecognized option\n"); return 1;
   }
 }
 client(ip,atoi(port), atoi(num));
 return 0;
}
