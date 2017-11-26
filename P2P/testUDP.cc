#include "Common.hh"
//testing UDP protocol program
//this will show up that server gets bat client
//address from recvfrom() syscall
//it makes UDP implementation really hard

//returns with error
void error(char* err)
{
 printf("error: %s\n", err);
 exit(1);
}

//runs server on UDP port
void server()
{
 printf("server\n");
 int sock;
 char buff[16];
 strcpy(buff, "***************");
 sock = socket(AF_INET, SOCK_DGRAM, 0);
 if (sock == -1) error("server socket");
 struct sockaddr_in saddr;
 struct sockaddr_in caddr;
 socklen_t clilen;
 saddr.sin_family      = AF_INET;
 saddr.sin_port        = htons(7000);
 saddr.sin_addr.s_addr = htonl(INADDR_ANY);
 if ((bind(sock, (struct sockaddr*)(&saddr), sizeof(struct sockaddr_in))) == -1)
     {
      error("server bind");
      close(sock); 
     }
 printf("server ok\n");
 for (int i=0;i<15;i++)
   {
    if (recvfrom(sock, buff, 10, 0, (struct sockaddr*)&caddr, &clilen)==-1)
      {
       error("server recvfrom");
      }
    printf("server: %s\n", buff);
    printf("server recvfrom %d %d\n", caddr.sin_port, htonl(caddr.sin_addr.s_addr));
    if (sendto(sock, buff, 10, 0, (struct sockaddr*)&caddr, sizeof(struct sockaddr_in))==-1)
      {
       error("server sendto");
      }
   }
 printf("server done\n");
 exit(0);
}

//runs client on UDP port
void client()
{
 printf("client\n");
 char buff[16];
 usleep(50000);		//wait until server is ready...
 strcpy(buff, "***************");
 int sock;
 sock = socket(AF_INET, SOCK_DGRAM, 0);
 if (sock == -1) error("client socket");
 struct sockaddr_in saddr, raddr;
 socklen_t srvlen;
 struct  sockaddr_in caddr = { AF_INET, htonl(INADDR_ANY), htonl(INADDR_ANY) };
 saddr.sin_family      = AF_INET;
 saddr.sin_port        = htons(7000); 
 saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
 if ((bind(sock, (struct sockaddr*)(&caddr), sizeof(struct sockaddr_in))) == -1)
     {
      error("client bind");
      close(sock); 
     }
 printf("client ok\n");
 for (int i=0;i<15;i++)
   {
    buff[i] = 'X';
    if (sendto(sock, buff, 10, 0, (struct sockaddr*)&saddr, sizeof(struct sockaddr_in))==-1)
      {
       error("client sendto");
      }
    printf("client: %s\n", buff);
    if (recvfrom(sock, buff, 10, 0, (struct sockaddr*)&raddr, &srvlen)==-1)
      {
       error("client recvfrom");
      }
    printf("client recvfrom %d %d\n", htons(raddr.sin_port), htonl(raddr.sin_addr.s_addr));
   }
 printf("client done\n");
 exit(0);
}

//no comment for this function
int main(int lb, char** par)
{
 if (lb>1 && par[1][0]=='s' || lb==1) if (fork()) server();
 if (lb>1 && par[1][0]=='c' || lb==1) client();
 return 0;
}

//thats all


