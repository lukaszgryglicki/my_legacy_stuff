/* Written by MorgothDBMA: morgothdbma@o2.pl, tel +48693582014 */
/* license: BSD */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/un.h>
#define PATH_MAX 1024
#define BUFS 65536		/* BUFFER for data */
int loc = 0;
char globsock[101];

void error(char* fmt, ...)	/* printf error msg and exit */
{
 va_list lst;
 if (errno) perror("error");
 va_start(lst,fmt);
 printf("CRITICAL ERROR: ");
 vprintf(fmt,lst);
 printf("\nSERVER HALTED.\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}

void ver()
{
 printf("OFTP SERVER; version 0.99, %s:%d %s %s\n",__FILE__,__LINE__,__DATE__,__TIME__);
}


void help(char* name)		/* usage help */
{
 printf("usage:\n\t%s  [-p PORT] [-m MAXCONN] [-a MAXWAIT] [-e PASS] [-h] [-o] [-n] &\n", name);
 printf("\t-l [USE LOCAL SOCKET], -i [USE INTERNET SOCKET, DEFAULT] -s [SOCKNAME]\n");
 printf("\t-6 USE IPv6, -4 use IPv4 (default)\n");
 printf("\tdefaults: PORT: 8087, MAXCONN: 5, MAXWAIT: 0 (infinite),\n\tSOCKNAME /tmp/.oftps\n");
 printf("\t-o overwrite flag.\n");
 printf("\t-n do not encrypt.\n");
 printf("\t-h gives this help msg.\n");
 printf("\tLOCAL options:  -l -s, INET: the rest\n");
 printf("\toptions for INET are ignored when -l set AND VICE VERSA for -i\n");
 printf("\tserver is able to create directories.\n");
 fflush(stdout);
}

/* FIXME USE setitimer everywhere, instead of alarm */

void catch_int(int signo)	/* catch signals */
{
 if (signo==SIGINT)
   {
    printf("Shutting dow server, interrupt occured.\n");
    if (loc && strcmp(globsock,"")) { printf("unlinking server socket..\n"); unlink(globsock); }
   }
 else if (signo==SIGPIPE) printf("Shutting dow server, broken pipe on socket\n");
 else if (signo==SIGALRM) printf("Shutting dow server, no clients connected within timer clock\n");
 exit(0);
}


void decode(char* buff, char* pass, int blen, int len, int bytes)/* decode buffer */
{
 register int i;
 if (!buff) error("buff is null");
 for (i=0;i<blen;i++) buff[i] -= pass[(i+bytes)%len];
}

void handle_client(int fd, int overwr, int enc, char* pass)
				       /* client, connected to server by fd */
{				       /* if overwr is set, server will */
 int cnt,bytes,fdw,i,iter,len;	       /* overwrite files it gets */
 unsigned int mode;
 int pdir;
 char buff[BUFS+1];
 char fname[BUFS+1];
 char dirname[BUFS+1];
 bytes=0;
 iter=0;
 pdir=0;
 printf("\n");			/* read fillename */
 if (read(fd,buff,PATH_MAX)==-1) error("cannot read filename from client.");
 if (strlen(buff)>=PATH_MAX) 
   {
    if (write(fd,"NAME_2LONG",16)==1) error("cannot write error msg to client");
    return;
   }
 strcpy(fname, buff);		/* store to buff */
 if (!strcmp(fname,"!ERR!"))
   {
    printf("cancelling errors on client's file.\n");
    return;			/* client said that there were error on */
   }				/* it's side, so abort processing */
 strcpy(dirname, "");
 if (buff[0]=='*') pdir=1;
 if (!pdir) printf("%s... ", buff); 	/* filename got */
 else       
   { 
    printf("directory: %s*\n", buff);
    strcpy(dirname, (buff+1));
   }
 fflush(stdout);		/* get file creation modes */
 if (read(fd,buff,16)==-1) error("cannot read filemodes from client");
 if (!strcmp(fname,"!ERR!"))	/* same as above */
   {
    printf("cancelling errors on client's file.\n");
    return;
   }
 sscanf(buff,"0%o", &mode);	/* for example 0755 */
 if (pdir)
   {
    i=mkdir(dirname, mode);
    if (i==-1 && errno!=EEXIST)
      {
       perror("mkdir");
       if (write(fd,"ERROR",16)==-1) error("cannot write FAIL msg to client");
       return;
      }
    if (write(fd,"OK",16)==-1) error("cannot write OK msg to client");
    return;
   }
 if ((fdw=open(fname,O_RDONLY))>=0)
   {				/* target exists? */
    printf("file exists.\n");
    if (!overwr)		/* we cannot overwrite it */
      {
       close(fdw);		/* inform client about it */
       if (write(fd,"EEXISTS",16)==-1) error("cannot write FAIL msg to client");
       return;
      }				/* overwrite, as flag overwr is set */
    else printf("but I will overwrite it\n");
   }
 if ((fdw=open(fname, O_WRONLY|O_CREAT|O_TRUNC,mode))==-1)
   {				/* create, if exists trunc, open to WRITE */
    printf("cannot create output file.\n");
    perror("open");
    if (write(fd,"CANNT_WRT_OUT",16)==-1) error("cannot write FATAL msg to client");
    return;
   }				/* write OK to client, only the it proceeds */
 if (write(fd,"OK",16)==-1) error("cannot write METADATA confirm to client.");
 if (enc) len=strlen(pass);
 else len=0;
 while ((cnt=read(fd,buff,BUFS))>0)	/* while client writes */
   {
/*    printf("cnt: %d\n", cnt);	  */
    if (enc) decode(buff,pass,cnt,len,bytes); /* decode stream */
    if (write(fdw,buff,cnt)==-1) error("cannot write data to output file: %s", fname);
    bytes+=cnt;
    iter++;
    if (!(iter%0x100)) { printf("."); fflush(stdout); } /* each buffer is less then 64k */
   }					/* although we want read() to read 64k */
 					/* average buffer is 4-8k, so * 100 means */
 					/* every 400-800k a `.' is put */
 if (cnt==-1) error("cannot read data from client socket, read: %d\n");
 close(fdw);				/* close outfile */
 printf("...%d bytes\n", bytes);	/* stats */
 fflush(stdout);
}

		/* server at port, maxconnections, maxwait, overwrite flag */

void server(int ipv6, int local, char* sockpth, int port, int maxc, int maxw, int overwr, int enc, char* pass)
{
 int sock,csock,err,alive;
 struct sockaddr_in server;
 struct sockaddr_un serverl;
 struct sockaddr_in6 server6;
 static struct sigaction act;
 static struct sigaction chldact;	/* handle SIGCHLD */
 if (local) loc=1; else loc=0;
 if (enc && !pass) error("encryption is set but no password is given.");
 if (!loc && (port<1 || port>0xffff)) error("port: %d is invalid", port);
 if (maxc<1 || maxc>0xff) error("max connections: %d is bad", maxc);
 if (maxw<0 || maxw>0xffffff) error("max wait: %d is bad", maxw);
 if (!loc && port<1024) printf("warning: port: %d is less than 1024 and can be OS-protected.\n", port);
 if (!loc) printf("Starting server on local machine, port %d\n", port);
 else printf("Starting server on FS socket: %s\n",sockpth);
 strncpy(globsock,"",100);
 act.sa_handler = catch_int;		/* handle other signals */
 chldact.sa_handler = SIG_IGN;		/* ignore child finished */
 chldact.sa_flags = SA_NOCLDWAIT;	/* DO NOT CREATE ZOMBIE  */
 sigfillset(&(act.sa_mask));		/* WHEN PARENT DOESN'T WAIT FOR CHILDREN */
 sigfillset(&(chldact.sa_mask));
 sigaction(SIGPIPE, &act, NULL);
 sigaction(SIGINT, &act, NULL);
 sigaction(SIGALRM, &act, NULL);
 sigaction(SIGCHLD, &chldact, NULL);	/* CHILDREN ARE SIMPLY IGNORED */
 if (!loc)
 {
  if (!ipv6)
   {
    server.sin_family = AF_INET;		/* setup server socket */
    server.sin_port = (in_port_t)(htons(port));
    server.sin_addr.s_addr = INADDR_ANY;
    sock = socket(AF_INET, SOCK_STREAM, 0); /* create it */
   }
  else
   {
    server6.sin6_family = AF_INET6;
    server6.sin6_port = (in_port_t)(htons(port));
   if (inet_pton(AF_INET6, (void*)("0:0:0:0:0:0:0:0"),
			   (void*)&(server6.sin6_addr))!=1)
     {
      perror("inet_pton, create INET6_ANY_ADDR");
      error("cannot understand addres (IP6): 0:0:0:0:0:0:0:0 (0::0)\n");
     }
    sock = socket(AF_INET6, SOCK_STREAM, 0);
   }
 }
 else
 {
  if (!strcmp(sockpth,"")) error("socket path cannot be: \"\"");
  serverl.sun_family = AF_UNIX;
  strncpy(serverl.sun_path, sockpth, 100);
  sock = socket(AF_UNIX, SOCK_STREAM, 0);
 }
 if (sock==-1)
   {
    perror("socket");
    error("create socket failed.");
   }
 alive=1;				/* keep connection alive */
 if (!loc)
  {
   err=setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,&alive,sizeof(void*));
   if (err==-1)
     {
      perror("setsockopt");
      error("set socket options failed");
     }					/* bind address,port,socket */
  }
 if (!loc)
   {
    if (!ipv6) err=bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
    else err=bind(sock, (struct sockaddr*)&server6, sizeof(struct sockaddr_in6));
   }
 else err=bind(sock,(struct sockaddr*)(&serverl), sizeof(struct sockaddr_un));
 if (err==-1)
   {
    if (errno==EADDRINUSE) printf("Address is already in use, kill all server processes or remove FS socket\n");
    perror("bind");
    error("bind address failed.");
   }
 if (listen(sock,maxc)==-1)		/* listen connections, maxc */
   {
    perror("listen");
    error("listen on socket failed");
   }
 if (loc) strncpy(globsock, sockpth,100);
 else strncpy(globsock,"",100);
 printf("Server started: type: ");
 if (!loc && ipv6)  printf("IPv6\n");
 if (!loc && !ipv6) printf("IPv4\n");
 if (loc)           printf("LocalFS\n");
 while (1)				/* server loop, ^C ends cleanly. */
   {
    printf("waiting for client...");
    fflush(stdout);			/* if no client in maxw seconds */
    if (maxw) alarm(maxw);		/* timeout for connection */
    if ((csock=accept(sock,NULL,NULL))==-1)
      {					/* accept client */
       perror("accept");
       error("accept failed");
      }
    if (maxw) alarm(0);			/* switch off alarm */
    fflush(stdout);			/* child to handle client, parent wait for another */
    if (!fork())
       {
	handle_client(csock,overwr,enc,pass);
	close(csock);
	exit(0);
       }
    close(csock);			/* no more needed client socket */
    fflush(stdout);			/* client works with child */
   }
}


int main(int lb, char** par)		/* server starts */
{
 char u;
 int local;
 int enc;
 char port[12];
 char maxc[12];
 char maxw[12];
 char pass[129];
 char sock[100];
 int over;
 int scanned;
 int ipv6;
 scanned=0;
 enc=1;
 local=0;
 ipv6=0;
 ver();
 strcpy(port,"8087");			/* defaults */
 strcpy(maxc,"5");
 strcpy(maxw,"0");
 strcpy(sock,"/tmp/.oftps");
 strcpy(pass,
  "kjlsh#%$%$#DFFBCVBKJH&(*__#@!#!  SDFDF#@$#$@%^fdsfsd$#@$#@DSF"
  "DFFASSDFSDXCVXCVCFDSRWEREW$#$$#$@A_+?><<::dfkjhoiwe5$#@#QSDSD");
 over=0;
 while ((u = getopt(lb,par,"p:m:a:e:s:honli64"))!=-1)	/* parse options */
 {
  scanned++;
  switch (u)
   {
    case 'p': if (strlen(optarg)<12) strcpy(port, optarg); break;
    case 'm': if (strlen(optarg)<12) strcpy(maxc, optarg); break;
    case 'a': if (strlen(optarg)<12) strcpy(maxw, optarg); break;
    case 'e': if (strlen(optarg)<128)strcpy(pass, optarg); break;
    case 's': if (strlen(optarg)<100)strcpy(sock, optarg); break;
    case 'h': help(par[0]); return 0;
    case 'o': over=1; break;
    case '4': ipv6=0; break;
    case '6': ipv6=1; break;
    case 'l': local=1; break;
    case 'i': local=0; break;
    case 'n': enc=0;  break;
    default: printf("Unrecognized option\n"); return 1;
   }
 }
 server(ipv6, local, sock, atoi(port), atoi(maxc), atoi(maxw), over, enc, pass);
 return 0;
}

/* end, written by MorgothDBMA: morgothdbma@o2.pl, tel +48693582014 */
/* license: BSD */
