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
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#define BUFS 65536		/* BUFFER for data */

void error(char* fmt, ...)	/* printf error msg and exit */
{
 va_list lst;
 va_start(lst,fmt);
 printf("CRITICAL ERROR: ");
 vprintf(fmt,lst);
 printf("\nSERVER HALTED.\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}


void help(char* name)		/* usage help */
{
 printf("usage:\n\t%s  [-p PORT] [-m MAXCONN] [-a MAXWAIT] [-e PASS] [-h] [-o] [-n] &\n", name);
 printf("defaults: PORT: 8087, MAXCONN: 5, MAXWAIT: 0 (infinite)\n");
 printf("\t-o overwrite flag.\n");
 printf("\t-n do not encrypt.\n");
 printf("\t-h gives this help msg.\n");
 fflush(stdout);
}


void catch_int(int signo)	/* catch signals */
{
 if (signo==SIGINT) printf("Shutting dow server, interrupt occured.\n");
 else if (signo==SIGPIPE) printf("Shutting dow server, broken pipe on socket\n");
 else if (signo==SIGALRM) printf("Shutting dow server, no clients connected within timer clock\n");
 exit(0);
}


void decode(char* buff, char* pass, int blen, int len, int bytes)/* decode buffer */
{
 register int i;
 for (i=0;i<blen;i++) buff[i] -= pass[(i+bytes)%len];
}

void handle_client(int fd, int overwr, int enc, char* pass)
				       /* client, connected to server by fd */
{				       /* if overwr is set, server will */
 int cnt,bytes,fdw,i,iter,len;	       /* overwrite files it gets */
 unsigned int mode;
 char buff[BUFS+1];
 char fname[BUFS+1];
 bytes=0;
 iter=0;
 printf("\n");			/* read fillename */
 if (read(fd,buff,256)==-1) error("cannot read filename from client.");
 strcpy(fname, buff);		/* store to buff */
 if (!strcmp(fname,"!ERR!"))
   {
    printf("cancelling errors on client's file.\n");
    return;			/* client said that there were error on */
   }				/* it's side, so abort processing */
 printf("%s... ", buff); 	/* filename got */
 fflush(stdout);		/* get file creation modes */
 if (read(fd,buff,16)==-1) error("cannot read filemodes from client");
 if (!strcmp(fname,"!ERR!"))	/* same as above */
   {
    printf("cancelling errors on client's file.\n");
    return;
   }
 sscanf(buff,"0%o", &mode);	/* for example 0755 */
 for (i=0;i<strlen(fname);i++) if (fname[i]=='/')
   {				/* this program doesn't transport dirs */
    printf("directories separator '/' in names is not supported.\n");
    if (write(fd,"NO_DIR_SPRT",16)==1) error("cannot write error msg to client");
    return;			/* wee don't like absolute names too */
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
/*    printf("cnt: %d\n", cnt);*/	/* debug */
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

void server(int port, int maxc, int maxw, int overwr, int enc, char* pass)
{
 int sock,csock,err,alive;
 struct sockaddr_in server;
 static struct sigaction act;
 static struct sigaction chldact;	/* handle SIGCHLD */
 if (enc && !pass) error("encryption is set but no password is given.");
 if (port<1 || port>0xffff) error("port: %d is invalid", port);
 if (maxc<1 || maxc>0xff) error("max connections: %d is bad", maxc);
 if (maxw<0 || maxc>0xffffff) error("max wait: %d is bad", maxw);
 if (port<1024) printf("warning: port: %d is less than 1024 and can be OS-protected.\n", port);
 printf("Starting server on local machine, port %d\n", port);
 act.sa_handler = catch_int;		/* handle other signals */
 chldact.sa_handler = SIG_IGN;		/* ignore child finished */
 chldact.sa_flags = SA_NOCLDWAIT;	/* DO NOT CREATE ZOMBIE  */
 sigfillset(&(act.sa_mask));		/* WHEN PARENT DOESN'T WAIT FOR CHILDREN */
 sigfillset(&(chldact.sa_mask));
 sigaction(SIGPIPE, &act, NULL);
 sigaction(SIGINT, &act, NULL);
 sigaction(SIGALRM, &act, NULL);
 sigaction(SIGCHLD, &chldact, NULL);	/* CHILDREN ARE SIMPLY IGNORED */
 server.sin_family = AF_INET;		/* setup server socket */
 server.sin_port = (in_port_t)(port);
 server.sin_addr.s_addr = INADDR_ANY;
 sock = socket(AF_INET, SOCK_STREAM, 0); /* create it */
 if (sock==-1)
   {
    perror("socket");
    error("create socket failed.");
   }
 alive=1;				/* keep connection alive */
 err=setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,&alive,sizeof(void*));
 if (err==-1)
   {
    perror("setsockopt");
    error("set socket options failed");
   }					/* bind address,port,socket */
 if (bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in))==-1)
   {
    perror("bind");
    error("bind address failed.");
   }
 if (listen(sock,maxc)==-1)		/* listen connections, maxc */
   {
    perror("listen");
    error("listen on socket failed");
   }
 printf("Server started.\n");
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
 int enc;
 char port[12];
 char maxc[12];
 char maxw[12];
 char pass[129];
 int over;
 int scanned;
 scanned=0;
 enc=1;
 strcpy(port,"8087");			/* defaults */
 strcpy(maxc,"5");
 strcpy(maxw,"0");
 strcpy(pass,
  "kjlsh#%$%$#DFFBCVBKJH&(*__#@!#!  SDFDF#@$#$@%^fdsfsd$#@$#@DSF"
  "DFFASSDFSDXCVXCVCFDSRWEREW$#$$#$@A_+?><<::dfkjhoiwe5$#@#QSDSD");
 over=0;
 while ((u = getopt(lb,par,"p:m:a:e:hon"))!=-1)	/* parse options */
 {
  scanned++;
  switch (u)
   {
    case 'p': if (strlen(optarg)<12) strcpy(port, optarg); break;
    case 'm': if (strlen(optarg)<12) strcpy(maxc, optarg); break;
    case 'a': if (strlen(optarg)<12) strcpy(maxw, optarg); break;
    case 'e': if (strlen(optarg)<128)strcpy(pass, optarg); break;
    case 'h': help(par[0]); return 0;
    case 'o': over=1; break;
    case 'n': enc=0;  break;
    default: printf("Unrecognized option\n"); return 1;
   }
 }
 server(atoi(port), atoi(maxc), atoi(maxw), over, enc, pass);
 return 0;
}

/* end, written by MorgothDBMA: morgothdbma@o2.pl, tel +48693582014 */
/* license: BSD */
