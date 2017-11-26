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
#include <dirent.h>
#include <errno.h>
#include <sys/un.h>
#define BUFS 65536		/* buffer for data */
#define PATH_MAX 1024
#define HERE __FILE__,__LINE__
#define s6_addr8  __u6_addr.__u6_addr8
#define s6_addr16 __u6_addr.__u6_addr16
#define s6_addr32 __u6_addr.__u6_addr32
static int gfd=-1;		/* global server socket */
				/* global because when SIGINT, we need */

void error(char* fmt, ...)	/* to inform server, and the close socket */
{
 va_list lst;			/* printf error and exit nicely */
 if (errno) perror("error");
 va_start(lst,fmt);
 printf("CRITICAL ERROR: \t");
 vprintf(fmt,lst);
 printf("\n\tCLIENT HALTED\n");
 fflush(stdout);
 va_end(lst);
 exit(1);
}

/* FIXME USE setitimer instead of alarm EVERYWHERE */

void help(char* name)		/* print usage */
{
 printf("usage:\n%s [-i IP |-s SERVER] [-p PORT] [-a ALARM] [-h] [-n] [-e PASS] file1 [file2 [..]]\n", name);
 printf("\t-l [LOCAL_SOCKET] -P [PATH_TO_LOCAL_SOCK]\n");
 printf("\tWHEN -l used, then all INET options are ignored and when and normally\n\t option -P is ignored (unless -l is givn)\n");
 printf("]t-6 use IPv6 -4 use IPv4 (default), when IPv6 used to resolve\n\thostname, place switch -6 before -s\n");
 printf("\tdefaults: IP:127.0.0.1, SERVER: localhost, PORT: 8087, ALARM: 10\n");
 printf("\tPATH_TO_SOCKET: /tmp/.oftps\n");
 printf("\talarm: how many seeconds wait on connetc to terminate. [0-infinite]\n");
 printf("\t-h gives this help msg.\n");
 printf("\t-n do not encrypt.\n");
 printf("\tuse option -6 BEFORE -s serv, if You want to\n\tprocess servername as IPv6\n");
 printf("\tto send directory just give it as a parameter\n");
 fflush(stdout);
}


void get_host(char* to, char* name, int ipv6)	/* get IP of host from its NAME */
{
 struct hostent* hose;
 struct in_addr addr;
 struct in6_addr addr6;
 if (ipv6)  printf("Config type is: IPv6\n");
 else       printf("Config type is: IPv4\n");
 if (!ipv6) hose = gethostbyname2(name, AF_INET);
 else hose = gethostbyname2(name, AF_INET6);
 if (!hose)
   {
    perror("gethostbyname");
    error("Cannot get IP for hostname: %s\nIs DNS running?\nIs Gateway IP set?\n", name);
   }
 if (!ipv6) memcpy((void*)&addr,(void*)(hose->h_addr_list[0]),sizeof(struct in_addr));
 else  memcpy((void*)&addr6,(void*)(hose->h_addr_list[0]),sizeof(struct in6_addr));
 if (!ipv6)
   {
    strcpy(to, inet_ntoa(addr));
    printf("Got IP %s for hostname %s\n", to, name);
   }
 else
   {
    if (inet_ntop(AF_INET6, (void*)&addr6, (void*)to, 59)==NULL)
     {
      perror("inet_ntop");
      error("cannot convert addres INET6");
     }
    printf("Got IP %s for hostname %s\n", to, name);
   }
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
 else if (st.st_mode & S_IFDIR) return EISDIR;
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

void send_file(int fd, char* fname, int enc, char* pass);

int reconnect(int loc, int ipv6, int secs, struct sockaddr_in* server, struct sockaddr_in6* server6, struct sockaddr_un* serverl)
{
 int sock;
 int i,err;
 gfd=-1;
 if (!loc)
   {
    if (!ipv6) sock = socket(AF_INET,  SOCK_STREAM, 0);
    else       sock = socket(AF_INET6, SOCK_STREAM, 0);
   }
 else sock = socket(AF_UNIX, SOCK_STREAM, 0);
 if (sock==-1)
   { perror("socket"); error("create socket failed."); }
 i=1;
 if (!loc)
   {
    err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &i, sizeof(void*));
    if (err==-1)
         { perror("setsockopt"); error("set socket options"); }
   }
 if (secs) alarm(secs);
 if (!loc)
   {
    if (!ipv6) err=connect(sock,(struct sockaddr*)server, sizeof(struct sockaddr_in));
    else       err=connect(sock,(struct sockaddr*)server6, sizeof(struct sockaddr_in6));
   }
 else      err=connect(sock,(struct sockaddr*)serverl, sizeof(struct sockaddr_un));
 if (err==-1)
      {
       perror("connect");
       error("reconnect failed");
      }
 gfd=sock;
 if (secs) alarm(0);
 printf("connect ok... ");
 fflush(stdout);
 return sock;
}


void send_dir(int fd, char* fname, int enc, char* pass, char* postfix, int loc, int ipv6, int secs, struct sockaddr_in* server, struct sockaddr_in6* server6, struct sockaddr_un* serverl)
{
 char buff[PATH_MAX+32];
 DIR* dp;
 int type;
 struct dirent *wpis;
 struct stat statbufor;
 char start_dir[PATH_MAX+32];
 char pw[PATH_MAX+32];
 char tmp[PATH_MAX+32];
 char fmods[0x10];
 if (!fname || !postfix)
 {
   if (write(fd,"!ERR!",16)==-1)
      error("cannot inform server about NULL dir, SERVER CAN BE REALLY CRASHED NOW.");
    error("no dirname given or postfix");
   }
 strcpy(buff, fname);
 strcat(buff, postfix);
 if (buff[0]=='/') error("Directories starting with '/' aren't supported");
 if (strlen(buff)>=PATH_MAX)
   {
   if (write(fd,"!ERR!",16)==-1)
      error("cannot inform server about 2LONG dirname, SERVER CAN BE REALLY CRASHED NOW.");
    error("name too long: %s", buff);
   }
 dp = opendir(buff);
 if (dp==NULL)
   {
    if (write(fd,"!ERR!",16)==-1) 	/* cannot do it, inform server */
	    error("cannot inform server about errors, SERVER CAN BE REALLY CRASHED NOW.");
    error("cannot open dir: %s", buff);
   }
 printf("DIR: %s\n", buff);
 strcpy(tmp,"*");
 strcat(tmp, buff);
 if (gfd<0) fd = reconnect(loc, ipv6, secs, server, server6, serverl);
 if (write(fd,tmp,PATH_MAX)==-1)
   {
    perror("write");
    error("cannot write filename to server.");
   }
 get_fmodes(fmods, buff);
 if (write(fd,fmods,16)==-1) error("cannot write filemode to server.");
 				/* read server confirmation */
 if (read(fd,fmods,16)==-1) error("cannot read confirmation from server.");
 				/* if server said OK then proceed */
 if (strcmp(fmods,"OK"))
 {
  printf("server said: %s\n", fmods);	/* short answer from server */
  error("server refuses to create outfile: %s", buff);
 }
 close(fd);
 fd = -1;
 gfd = -1;
 fd = reconnect(loc, ipv6, secs, server, server6, serverl);
 if (!getcwd(start_dir, PATH_MAX+31))
   {
    if (write(fd,"!ERR!",16)==-1) 	/* cannot do it, inform server */
	    error("cannot inform server about errors, SERVER CAN BE REALLY CRASHED NOW.");
    error("getcwd1");
   }
 if (!getcwd(pw, PATH_MAX+31))
   {
    if (write(fd,"!ERR!",16)==-1) 	/* cannot do it, inform server */
	    error("cannot inform server about errors, SERVER CAN BE REALLY CRASHED NOW.");
    error("getcwd2");
    chdir(start_dir);
    return;
   }
/* printf("pw = %s\n", pw);*/
/* printf("buff = %s\n", buff);*/
 while ((wpis=readdir(dp))!=NULL)
     {
      strcpy(tmp, buff);
      if (tmp[strlen(tmp)-1]!='/') strcat(tmp, "/");
      strcat(tmp, wpis->d_name);
/*      printf("tmp = %s\n", tmp);*/
      if (stat(tmp,&statbufor)==-1)
        {
         if (write(fd,"!ERR!",16)==-1) 	/* cannot do it, inform server */
	    error("cannot inform server about errors, SERVER CAN BE REALLY CRASHED NOW.");
         error("stat for dirent");
         return;
	}
      if (!strcmp(wpis->d_name,".") || !strcmp(wpis->d_name, "..")) continue;
      if (S_ISLNK(statbufor.st_mode)) continue;
      if (S_ISDIR(statbufor.st_mode))
	  {
	   strcpy(tmp, buff);
	   if (tmp[strlen(tmp)-1]!='/')
		   strcat(tmp, "/");
	   send_dir(fd, tmp, enc, pass, wpis->d_name, loc, ipv6, secs, server, server6, serverl);
	  }
      else
          {
	   strcpy(tmp, buff);
	   if (tmp[strlen(tmp)-1]!='/')
		   strcat(tmp, "/");
	   strcat(tmp, wpis->d_name);
/*	   printf("FILE: %s\n", tmp);*/
           if ((type=is_regfile(tmp))!=1)
             {
              if (write(fd, "!ERR!", 16)==-1)
	      error("cannot send error msg to server, this means SERVER CAN BE REALLY CRASHED NOW!");
              error("%s is not a normal file.", tmp);
             }
           else
	     {
	      if (gfd<0) fd = reconnect(loc, ipv6, secs, server, server6, serverl);
	      send_file(fd, tmp, enc, pass);
	      close(fd);
	      fd  = -1;
	      gfd = -1;
	     }
          }
     }
 chdir(start_dir);
/* printf("done.\n");*/
 closedir(dp);
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
 if (fname[0]=='/')
   {
    if (write(fd,"!ERR!",16)==-1) 	/* when this written to server */
	    				/* it will stop processing this client */
	    				/* and clear all errors on server side */
	    error("cannot inform server about ERROR, SERVER CAN BE REALLY CRASHED NOW.");
    error("absolute pathnames not allowed");		/* abort with error msg */
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
 if (write(fd,buff,PATH_MAX)==-1)
   {
    perror("write");
    error("cannot write filename to server.");
   }
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
 close(rd);				/* close read file */
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


int has_double(char* str)
{
 int i,l,dd;
 l = strlen(str);
 dd=0;
 for (i=0;i<l;i++)
   {
    if (str[i]==':' && !dd) dd=1;
    else if (str[i]==':' &&  dd) return 1;
    else if (str[i]!=':') dd=0;
   }
 return 0;
}


void create_ipv6addr(char* there, char* from)
{
 int i,current,j,ddot,breakat;
 unsigned int ip16[8],fill;
 char nums[8][6];
 fill=0;
 if (from[0]=='-')
  {
   i=1;
   while (from[i]!='-')
     {
      nums[0][i-1]=from[i];
      i++;
      if (i>5) error("too long fill-prefix %s:%d",HERE);
     }
   nums[0][i-1] = 0;
   from += i+1;
   if (sscanf(nums[0], "%x", &fill)!=1) error("cannot scanf prefix value %s:%d", HERE);
  }
 nums[0][0] = 0;
 for (i=0;i<8;i++) { ip16[i] = fill; strcpy(nums[i],""); }
 current = j = ddot = breakat = 0;
 for (i=0;i<strlen(from);i++)
   {
    if (from[i]!=':')
      {
       nums[current][j] = from[i];
       j++;
       if (j>4) error("too long number %s:%d", HERE);
       ddot=0;
      }
    else if (from[i]==':' && !ddot)
      {
       nums[current][j] = 0;
       current++;
       if (j>4) error("too long number %s:%d", HERE);
       j=0;
       ddot=1;
      }
    else if (from[i]==':' && ddot==1) { breakat=current; ddot=2; }
    else if (from[i]==':' && ddot>1)  { error("to much `:' symbols %s:%d", HERE); }
    if (j>6) error("too long number %s:%d", HERE);
    if (current>7) error("to high current %s:%d", HERE);
   }
 nums[current][j] = 0;
 printf("j=%d, current=%d, breakat=%d\n",j,current,breakat);
 for (i=0;i<=current;i++)
 {
  if (i<breakat) j=sscanf(nums[i],"%x",&ip16[i]);
  else           j=sscanf(nums[i],"%x",&ip16[7+i-current]);
  if (j!=1 && i && i!=current) error("scanning hex value i=%d %s:%d",i,HERE);
 }
 sprintf(there, "%x:%x:%x:%x:%x:%x:%x:%x", ip16[0],ip16[1],ip16[2],ip16[3],ip16[4],ip16[5],ip16[6],ip16[7]);
}


void client(int ipv6, int loc, char* sockpth, char* ipstra, int port, int secs, char**filename, int from, int all, int enc, char* pass)
{
 int sock,i,err,x,type;
 struct sockaddr_in server;
 struct sockaddr_un serverl;
 struct sockaddr_in6 server6;
 static struct sigaction act;
 char ipstr[60];
 strcpy(ipstr, ipstra);
 if (from==all) error("no filename(s) given.");
 if (ipv6 && has_double(ipstra)) create_ipv6addr(ipstr, ipstra);
 if (enc && !pass) error("encryption is set but  no password is given.\n");
 if (!loc && (port<1 || port>0xffff)) error("port: %d is invalid", port);
 				/* there are 1<-->65535 ports */
 if (secs<0 || secs>0xffff) error("seconds: %d is bad value", secs);
 if (!loc && port<1024) printf("warning: port: %d is less than 1024 and can be OS-protected.\n", port);
 if (!loc)
 {
  if (!ipv6)
  {
   server.sin_family = AF_INET;	/* setup socket */
   server.sin_port = (in_port_t)(htons(port));
   server.sin_addr.s_addr = inet_addr(ipstr);
  }
  else
  {
   server6.sin6_family = AF_INET6;
   server6.sin6_port = (in_port_t)(htons(port));
   if (inet_pton(AF_INET6, (void*)ipstr, (void*)&(server6.sin6_addr))!=1)
     {
      perror("inet_pton");
      error("cannot understand addres (IP6): %s\n", ipstr);
     }
  }
 }
 else
 {
  serverl.sun_family = AF_UNIX;
  if (!strcmp(sockpth,"")) error("socket path cannot be: \"\"");
  strncpy(serverl.sun_path,sockpth,100);
 }
 act.sa_handler = catch_signal;
 sigfillset(&(act.sa_mask));
 sigaction(SIGALRM, &act, NULL);	/* setup signals */
 sigaction(SIGPIPE, &act, NULL);	/* when server killed while client processig */
 sigaction(SIGINT, &act, NULL);		/* ^C handler */
 if (!loc && !ipv6 && server.sin_addr.s_addr==(in_addr_t)(-1))
	 error("IP %s is invalid.\n", ipstr);
 if (!loc) printf("Connecting to %s, at port %d\n", ipstr, port);
 else      printf("Connecting to local machine on FS socket: %s\n", sockpth);
 printf("Client type: ");
 if (!loc && ipv6)  printf("IPv6\n");
 if (!loc && !ipv6) printf("IPv4\n");
 if (loc)           printf("LocalFS\n");
 for (x=from;x<all;x++)
   {
    gfd=-1;			/* no connection yet */
    if (!loc)
      {
       if (!ipv6) sock = socket(AF_INET,  SOCK_STREAM, 0);
       else       sock = socket(AF_INET6, SOCK_STREAM, 0);
      }
    else sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock==-1)
      { perror("socket"); error("create socket failed."); }
    i=1;			/* got socket */
    if (!loc)
      {
       err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &i, sizeof(void*));
       if (err==-1)		/* keep connection alive */
         { perror("setsockopt"); error("set socket options"); }
      }
    if (secs) alarm(secs);	/* timeout connection to server */
    				/* TODO use setitimer here */
    if (!loc)
      {
       if (!ipv6) err=connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in));
       else       err=connect(sock,(struct sockaddr*)&server6, sizeof(struct sockaddr_in6));
      }
    else      err=connect(sock,(struct sockaddr*)&serverl, sizeof(struct sockaddr_un));
    if (err==-1)
      {
       perror("connect");
       if (!loc && !ipv6) error("connect to %s:%d failed",ipstr,port);
       if (!loc && ipv6)  error("connect to %s-->%d failed",ipstr,port);
       else      error("connect to: FS:%s failed", sockpth);
      }
    gfd=sock;			/* set global socket, needed by signal handlers */
    				/* to inform server about INTR and close socket before exit */
    if (secs) alarm(0);		/* got connection, cancel alarm clock */
    printf("connect ok... ");
    fflush(stdout);
    if ((type=is_regfile(filename[x]))!=1)/* file is not normal, don't send */
      {
       if (type==EISDIR) send_dir(sock, filename[x], enc, pass, "", loc, ipv6, secs, &server, &server6, &serverl);
       else
         {
          if (write(sock, "!ERR!", 16)==-1) /* tell server about error */
	      error("cannot send error msg to server, this means SERVER CAN BE REALLY CRASHED NOW!");
          error("%s is not a normal file.", filename[x]);
         }
      }
    else send_file(sock, filename[x], enc, pass);
    					/* send file, see above */
    close(sock);			/* no more needed socket */
    					/* immadiately next while loop */
    					/* will set gfd to -1 */
   }
}


void ver()
{
 printf("OFTP CLIENT; version 0.99, %s:%d %s %s\n",__FILE__,__LINE__,__DATE__,__TIME__);
 printf("%dbit CPU\n", (int)(8*sizeof(void*)));
}


int main(int lb, char** par)		/* client starts */
{
 char u;
 int local;
 int enc;
 char ip[60];
 char port[12];
 char secs[12];
 char pass[129];
 char  sock[101];
 int scanned;
 int ipv6;
 ipv6=0;
 scanned=0;
 enc=1;				/* no encode */
 local=0;
 ver();
 strcpy(ip,"127.0.0.1");	/* defaults */
 strcpy(port,"8087");
 strcpy(secs,"10");
 strcpy(sock,"/tmp/.oftps");
 strcpy(pass,
  "kjlsh#%$%$#DFFBCVBKJH&(*__#@!#!  SDFDF#@$#$@%^fdsfsd$#@$#@DSF"
  "DFFASSDFSDXCVXCVCFDSRWEREW$#$$#$@A_+?><<::dfkjhoiwe5$#@#QSDSD");
 while ((u = getopt(lb,par,"i:s:p:P:a:e:hnl64"))!=-1)	/* parse command-line options */
 {
  scanned++;
  switch (u)
   {
    case 'i': if (strlen(optarg)<59) strcpy(ip, optarg);      break;
    case 's': get_host(ip, optarg, ipv6);                     break;
    case 'p': if (strlen(optarg)<12) strcpy(port, optarg);    break;
    case 'a': if (strlen(optarg)<12) strcpy(secs, optarg);    break;
    case 'e': if (strlen(optarg)<128)strcpy(pass, optarg);    break;
    case 'P': if (strlen(optarg)<100)strcpy(sock, optarg);    break;
    case 'h': help(par[0]); return 0;
    case 'n': enc=0; break;				/* disable encryption */
    case 'l': local=1; break;
    case '6': ipv6=1; break;
    case '4': ipv6=0; break;
    default: printf("Unrecognized option\n"); return 1;
   }
 }
 client(ipv6,local,sock,ip,atoi(port), atoi(secs), par, optind, lb, enc, pass);	/* run client */
 return 0;
}

/* end, written by MorgothDBMA: morgothdbma@o2.pl, tel +48693582014 */
/* license: BSD */
