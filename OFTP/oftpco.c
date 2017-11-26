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
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#define BUFS 65536		/* buffer for data */
static int gfd=-1;		/* global server socket */
				/* global because when SIGINT, we need */

void error(char* fmt, ...)	/* to inform server, and the close socket */
{
 va_list lst;			/* printf error and exit nicely */
 va_start(lst,fmt);
 printf("CRITICAL ERROR: \t");
 vprintf(fmt,lst);
 printf("\n\tCLIENT HALTED\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}


void help(char* name)		/* print usage */
{
 printf("usage:\n%s [-i IP |-s SERVER] [-p PORT] [-a ALARM] [-h] [-n] [-e PASS] file1 [file2 [..]]\n", name);
 printf("defaults: IP:127.0.0.1, SERVER: localhost, PORT: 8087, ALARM: 10\n");
 printf("alarm: how many seeconds wait on connetc to terminate. [0-infinite]\n");
 printf("\t-h gives this help msg.\n");
 printf("\t-n do not encrypt.\n");
 fflush(stdout);
}


void get_host(char* to, char* name)	/* get IP of host from its NAME */
{
 struct hostent* hose;
 struct in_addr addr;
 hose = gethostbyname(name);
 if (!hose)
   {
    perror("gethostbyname");
    error("Cannot get IP for hostname: %s\nis DNS running?, is Gateway IP set?\n", name);
   }
 memcpy((void*)&addr,(void*)(hose->h_addr_list[0]),sizeof(struct in_addr));
 strcpy(to, inet_ntoa(addr));
 printf("Got IP %s for hostname %s\n", to, name);
 fflush(stdout);
}

int is_regfile(char* file)	/* is file regular file or something else */
{				/* like directory, fifo, blkdev etc. */
 int err;
 struct stat st;
 err=stat(file, &st);
 if (err==-1)
   {
    perror("stat");
    error("stat failed");
    return 0;
   }
 if (st.st_mode & S_IFREG) return 1;	/* is regular? */
 else return 0;
}

void get_fmodes(char* to, char* file)	/* get modes of `file' */
{					/* and write to `to' */
 int mode,err;				/* example to="0755" */
 struct stat st;
 mode=0;
 err=stat(file, &st);
 if (err==-1)
   {
    perror("stat");
    error("stat failed");
   }
 if (st.st_mode & S_IRUSR) mode += 0400;	/* test modes */
 if (st.st_mode & S_IWUSR) mode += 0200;
 if (st.st_mode & S_IXUSR) mode += 0100;
 if (st.st_mode & S_IRGRP) mode += 0040;
 if (st.st_mode & S_IWGRP) mode += 0020;
 if (st.st_mode & S_IXGRP) mode += 0010;
 if (st.st_mode & S_IROTH) mode += 0004;
 if (st.st_mode & S_IWOTH) mode += 0002;
 if (st.st_mode & S_IXOTH) mode += 0001;
 sprintf(to,"0%o", mode);			/* write to... */
/* printf("file:  %-50s\t mode: %s\n", file, to);*/
}


void encode(char* buff, char* pass, int blen, int len, int bytes)/* encode buffer */
{
 register int i;
 for (i=0;i<blen;i++) buff[i] += pass[(i+bytes)%len];
}

void send_file(int fd, char* fname, int enc, char* pass)
					/* send `fname' to server */
{					/* socket fd */
 int rd,cnt,bytes,iter,len;
 char buff[BUFS+1];
 if (!fname)				/* filename is NULL */
 {
    if (write(fd,"!ERR!",16)==-1) 	/* when this written to server */
	    				/* it will stop processing this client */
	    				/* and clear all errors on server side */
	    error("cannot inform server about NULL file, SERVER CAN BE REALLY CRASHED NOW.");
    error("no filename given");		/* abort with error msg */
   }
 rd=open(fname,O_RDONLY);		/* open file to send */
 if (rd==-1)
   {
    if (write(fd,"!ERR!",16)==-1) 	/* cannot do it, inform server */
	    error("cannot inform server about errors, SERVER CAN BE REALLY CRASHED NOW.");
    error("cannot open file");
   }
 bytes=0;
 iter=0;
 strcpy(buff,fname);		/* write filaname to server */
 if (write(fd,buff,256)==-1) error("cannot write filename to server.");
 get_fmodes(buff, fname);	/* get modes of file, example: 0555 */
 				/* write filemodes to server */
 if (write(fd,buff,16)==-1) error("cannot write filemode to server.");
 				/* read server confirmation */
 if (read(fd,buff,16)==-1) error("cannot read confirmation from server.");
 				/* if server said OK then proceed */
 if (strcmp(buff,"OK"))
 {
  printf("server said: %s\n", buff);	/* short answer from server */
  error("server refuses to create outfile: %s", fname);
 }
 printf("%s ", fname);			/* filename */
 fflush(stdout);
 if (enc) len=strlen(pass);
 else len=0;
 while ((cnt=read(rd,buff,BUFS))>0)	/*while read data from file */
   {
/*    printf("cnt: %d\n", cnt);*/
    iter++;
    if (enc) encode(buff,pass,cnt,len,bytes); /* encode stream */
    if (write(fd,buff,cnt)==-1) error("cannot write data packet to server, bytes send: %d", bytes);
    bytes+=cnt;
    if (!(iter%0x10)) { printf("."); fflush(stdout); }
    	/* every 64k*0x10=1M byte write `.' and flush stdout */
    	/* so 60M file will produce 60 dots, and about 100 dots on server sidee */
    	/* cause server writes dots too, but at server side */
        /* read() returns less than 64k */
   }
 if (cnt==-1) error("read data from input file failed, read: %d", bytes);
 printf("%d bytes send.\n", bytes);	/* stats */
 fflush(stdout);
 close(rd);				/* close reead file */
}

void catch_signal(int signo)		/* catch signals */
{					/* and switch it */
 if (signo==SIGALRM) printf("alarm clock: connection timeout.\n");
 				/* ALRM was set to timeout connection */
 				/* default 10 seconds */
 				/* deliveered only when IDLE */
 				/* so needn't close anything */
 else if (signo==SIGPIPE)printf("server has closed socket, exiting...\n");
 else if (signo==SIGINT)	/* INTERRUPTED, inform server and close socket*/
   {				/* gfd is -1 when socket is not opened */
    printf("got interrupt, cleaning up...\n");
    				/* at the moment, this is set in client() func */
    if (gfd==-1) printf("socket is already closed, bye!\n");
    else			/* socket is active */
      {
       printf("informing server about interrupt...\n");
       				/* write to server that error occured */
       if (write(gfd,"!ERR!", 16)==-1) error("cannot inform server about interrupt");
       				/* if write fail, then is not good */
       printf("done.\n");
       close(gfd);		/* close socket */
      }
    exit(1);			/* emergency, SIGNNALLED exit */
   }
 exit(0);			/* alarm clock exit */
}

	/* client: connect to ipstr on port, wait max secs to connect */
	/* if ok send files from filename list */
	/* if enc=1 and pass!=NULL, encryption by pass is made */

void client(char* ipstr, int port, int secs, char**filename, int from, int all, int enc, char* pass)
{
 int sock,i,err,x;
 struct sockaddr_in server;
 static struct sigaction act;
 if (from==all) error("no filename(s) given.");
 if (enc && !pass) error("encryption is set but  no password is given.\n");
 if (port<1 || port>0xffff) error("port: %d is invalid", port);
 				/* there are 1<-->65535 ports */
 if (secs<0 || secs>0xffff) error("seconds: %d is bad value", secs);
 if (port<1024) printf("warning: port: %d is less than 1024 and can be OS-protected.\n", port);
 server.sin_family = AF_INET;	/* setup socket */
 server.sin_port = (in_port_t)(port);
 server.sin_addr.s_addr = inet_addr(ipstr);
 act.sa_handler = catch_signal;
 sigfillset(&(act.sa_mask));
 sigaction(SIGALRM, &act, NULL);	/* setup signals */
 sigaction(SIGPIPE, &act, NULL);	/* when server killed while client processig */
 sigaction(SIGINT, &act, NULL);		/* ^C handler */
 if (server.sin_addr.s_addr==(in_addr_t)(-1))
	 error("IP %s is invalid.\n", ipstr);
 printf("Connecting to %s, at port %d\n", ipstr, port);
 for (x=from;x<all;x++)
   {
    gfd=-1;			/* no connection yet */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock==-1)
      { perror("socket"); error("create socket failed."); }
    i=1;			/* got socket */
    err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &i, sizeof(void*));
    if (err==-1)		/* keep connection alive */
      { perror("setsockopt"); error("set socket options"); }
    if (secs) alarm(secs);	/* timeout connection to seerver */
    if (connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in))==-1)
      { perror("connect"); error("connect to %s:%d failed",ipstr,port); }
    gfd=sock;			/* set global socket, needed by signal handlers */
    				/* to inform server about INTR and close socket before exit */
    if (secs) alarm(0);		/* got connection, cancel alarm clock */
    printf("connect ok... ");
    fflush(stdout);
    if (!is_regfile(filename[x]))	/* file is not normal, don't send */
      {
       if (write(sock, "!ERR!", 16)==-1) /* tell server about error */
	      error("cannot send error msg to server, this means SERVER CAN BE REALLY CRASHED NOW!");
       error("%s is not a normal file.", filename[x]);
      }
    send_file(sock, filename[x], enc, pass);
    					/* send file, see above */
    close(sock);			/* no more needed socket */
    					/* immadiately next while loop */
    					/* will set gfd to -1 */
   }
}


int main(int lb, char** par)		/* client starts */
{
 char u;
 int enc;
 char ip[16];
 char port[12];
 char secs[12];
 char pass[129];
 int scanned;
 scanned=0;
 enc=1;				/* no encode */
 strcpy(ip,"127.0.0.1");	/* defaults */
 strcpy(port,"8087");
 strcpy(secs,"10");
 strcpy(pass,
  "kjlsh#%$%$#DFFBCVBKJH&(*__#@!#!  SDFDF#@$#$@%^fdsfsd$#@$#@DSF"
  "DFFASSDFSDXCVXCVCFDSRWEREW$#$$#$@A_+?><<::dfkjhoiwe5$#@#QSDSD");
 while ((u = getopt(lb,par,"i:s:p:a:e:hn"))!=-1)	/* parse command-line options */
 {
  scanned++;
  switch (u)
   {
    case 'i': if (strlen(optarg)<16) strcpy(ip, optarg);      break;
    case 's': get_host(ip, optarg);                           break;
    case 'p': if (strlen(optarg)<12) strcpy(port, optarg);    break;
    case 'a': if (strlen(optarg)<12) strcpy(secs, optarg);    break;
    case 'e': if (strlen(optarg)<128)strcpy(pass, optarg);    break;
    case 'h': help(par[0]); return 0;
    case 'n': enc=0; break;				/* disable encryption */
    default: printf("Unrecognized option\n"); return 1;
   }
 }
 client(ip,atoi(port), atoi(secs), par, optind, lb, enc, pass);	/* run client */
 return 0;
}

/* end, written by MorgothDBMA: morgothdbma@o2.pl, tel +48693582014 */
/* license: BSD */
