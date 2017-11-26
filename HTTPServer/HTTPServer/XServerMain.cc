/////////////////////////////////////////////////
//////// ********************************////////
///// **************************************/////
//// ****  HTTP  ****************************////
/// ***********  Server  *********************///
// ***********************  in C++  ***********//
// ********************************************//
/// **### StudentSoft copyleft@ ###***********///
//// *************   BSD License   **********////
///// **************************************/////
//////// ********************************////////
/////////////////////////////////////////////////
#include "XServerMain.hh"
#include "Common.hh"
#include "Definitions.hh"
#include "Logger.hh"


////
//	set default values to class
//	specially NULLs to pointers
//	WE *DONT* like uninitialized pointers
////
void XServerMain :: SetDefaults()
{
 max_clients = DEFAULT_MAX_CLIENTS;
 mutex = NULL;
 shm = NULL;
 log_file = new char[MAX_PATH_LENGTH+1];
 assert(log_file);
 strcpy(log_file, DEFAULT_LOG_FILE);
 log = NULL;
 client_socket = admin_socket = -1;
 parent = 1;
 daemon = 0;
}

////
//	construct server with default values
////
XServerMain :: XServerMain()
{
 client_portnum = 80;
 admin_portnum  = 81;
 SetDefaults();
}

////
//	construct server with client port number given
////
XServerMain :: XServerMain(const int portnum)
{
 assert(portnum>0 && portnum<MAX_PORT);
 client_portnum = portnum;
 admin_portnum  = 81;
 SetDefaults();
}

////
//	set children state
//	used when signal catched to detgermine
//	if we are children or parent
////
void XServerMain :: SetChildState()
{
 parent = 0;
}

////
//	returns answer to question "Am I child?"
////
int XServerMain :: IsChild() const
{
 return !parent;
}

////
//	create server with both (admin and client)
//	port numbers given
////
XServerMain :: XServerMain(const int portnumC, const int portnumA)
{
 assert(portnumA>0 && portnumA<=MAX_PORT);
 assert(portnumC>0 && portnumC<=MAX_PORT);
 client_portnum = portnumC;
 admin_portnum  = portnumA;
 SetDefaults();
}

////
//	close log file if opened
////
void XServerMain :: CloseLogFile()
{
 if (log) fclose(log);
 log = NULL;
}

////
//	send all clients connected SIGUSR1
//	and SIGUSR2 to logger
//	this will give them message to clean up
//	and exit
////
void XServerMain :: DestroyAllClients()
{
 Debug("Destroy all clients.\n");
 int nclients,sock;
 if (!mutex) return;
 mutex->Wait(); 
 if (kill(log_thread, SIGUSR2)==-1) 
   Debug("Error closing LOGGER, PID: %d, exited before.\n", log_thread);
 nclients = shm->GetClientsCount(); 
 for (int i=0;i<nclients;i++) 
   {
    Debug("PID=%d\n", shm->GetClientPID(i));
    if (kill(shm->GetClientPID(i), SIGUSR1)==-1) 
      {
       Debug("Cannot send signal to %d child, already stopped, continuing...\n",i);
       sock = shm->GetClientSocket(i);
       if (sock>0) close(sock);
       if (shm->RemoveClient(shm->GetClientPID(i))==FAILED)
	    Debug("Failed to remove client with PID=%d\n", shm->GetClientPID(i));
      }
   }
 mutex->Signal();
}

////
//	destroy main server class
//	first stop all children
//	remove all objects used
//	and finally exit
////
XServerMain :: ~XServerMain()
{
 DestroyAllClients();
 if (mutex) delete mutex;
 if (shm)   delete shm;
 if (log_file) delete [] log_file;
 if (log) fclose(log);
 if (client_socket>=0) close(client_socket);
 if (admin_socket>=0) close(admin_socket);
 client_socket = admin_socket = -1;
 log = NULL;
 log_file = NULL;
 mutex = NULL;
 shm = NULL;
}

////
//	create two listening sockets: admin and client
//	set their options (KEEP ALIVE) and bind them
////
void XServerMain :: CreateSockets()
{
 static struct sockaddr_in server_addr;
 int err,alive;
 alive = 1;
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = (in_port_t)(htons(client_portnum));
 server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 assert((client_socket=socket(AF_INET, SOCK_STREAM, 0))!=-1);
 assert(setsockopt(client_socket,SOL_SOCKET,SO_KEEPALIVE,&alive,sizeof(void*))!=-1);
 err=bind(client_socket,(struct sockaddr*)&server_addr, 
		 sizeof(struct sockaddr_in));
 if (err==-1) { perror("client_socket: bind"); assert(err!=-1); }
 server_addr.sin_family = AF_INET;
 server_addr.sin_port = (in_port_t)(htons(admin_portnum));
 server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 assert((admin_socket=socket(AF_INET, SOCK_STREAM, 0))!=-1);
 assert(setsockopt(admin_socket,SOL_SOCKET,SO_KEEPALIVE,&alive,sizeof(void*))!=-1);
 err=bind(admin_socket,(struct sockaddr*)&server_addr, 
		 sizeof(struct sockaddr_in));
 if (err==-1) { perror("admin_socket: bind"); assert(err!=-1); }
}

////
//	enables daemon mode if needed
////
void XServerMain :: CheckDaemon() const
{
 if (!daemon) return;
 if (fork()) exit(0);
 close(0);
 close(1);
 close(2);
}

////
//	run server, assumes all initialization has
//	completed successfully
////
void XServerMain :: Run()
{
 assert(client_portnum != admin_portnum);
 CheckDaemon();
 printf("Starting up Server\n");
 printf("client port is %d, admin port is %d, max_clients: %d\n",client_portnum, admin_portnum, max_clients);
 CreateSockets();
 CreateMutex();
 CreateSHM();
 CreateWriterThread();
 Listen();
}

////
//	create logger process
//	with selected log file name
//	it will have direct pointer to class
//	although after fork class data will be 
//	different for logger and parent
//	but SHM pointer will not change 
//	and using this pointer logger will have
//	access to current structures of parent
//	NOTE: all important data is stored in SHM
////
void XServerMain :: CreateWriterThread()
{
 Debug("Server LOG file is: %s\n", log_file);
 if (!(log_thread=fork())) { Logger((void*)this); exit(0); }
 assert(log_thread != -1);
}

////
//	set max clients number, bu not above MAX_CLIENTS
//	form Definitions.hh
////
void XServerMain :: SetMaxClients(const int maxcli)
{
 assert(maxcli>0 && maxcli<MAX_CLIENTS);
 max_clients = maxcli;
}

////
//	set log file name
////
void XServerMain :: SetLogFile(const char* log)
{
 assert(log);
 strncpy(log_file, log, MAX_PATH_LENGTH);
}

////
//	set client port nummer, assertions are checked
////
void XServerMain :: SetClientPort(const int portnum)
{
 assert(portnum>0 && portnum<=MAX_PORT);
 client_portnum = portnum;
}

////
//	set admin port number, assertions are checked
////
void XServerMain :: SetAdminPort(const int portnum)
{
 assert(portnum>0 && portnum<=MAX_PORT);
 admin_portnum = portnum;
}

////
//	set daemon mode
////
void XServerMain :: SetDaemon(const int d)
{
 assert(d==0 || d==1);
 daemon = d;
}

////
//	create mutex with UNIQUE KEY for all server
//	processes running (read about UNIQUE in
//	XSemaphore.cc:: class constructor comment)
////
void XServerMain :: CreateMutex()
{
 mutex = new XSemaphore((client_portnum<<0x10)+admin_portnum, 1);
 assert(mutex);
}

////
//	create SHM with UNIQUE KEY, see comment above (CreateMutex)
////
void XServerMain :: CreateSHM()
{
 shm = new XShM((client_portnum<<0x10)+admin_portnum, max_clients);
 assert(shm);
}

////
//	Answer to client request, append 
//	bytes send for client and for global
//	append also answer length bytes 
//	if error will be set in this
//	function, connection will be closed
//	write answer in loop untill all data written
//	can be quite long for example BIG PNG file
//	if client close socket no fatal error will be 
//	generated, just error set and connection closed)
////
int XServerMain :: AnswerRequest(const int fd, const char* buff, const int n
		,struct ClientInfo* client, int& error)
{
 int idx,len,nwrt,total,large;
 char* response;
 FILE* handler;
 assert(fd>=0);
 assert(buff);
 assert(n>0);
 assert(client);
 client->nbytes+=n;
 mutex->Wait();
 shm->AppendGlobalBytes((int64_t)n);
 idx = shm->GetClientIdxByPID(client->pid);
 assert(idx>=0);
 shm->AppendClientBytes(idx, (int64_t)n);
 mutex->Signal();
 XHttpParser* http_parser = new XHttpParser();
 assert(http_parser);
 http_parser->Parse(buff, n);
 error = 0;
 response = NULL;
 large = 0;
 handler = NULL;
 if (http_parser->SendingLargeFile()) 
   {
    Debug("LARGE FILE REQUEST!\n");
    large = 1;
    response = http_parser->GetLargeFile(len, error, handler);
   }
 else response = http_parser->GetResponse(len, error);
 assert(len>0);
 assert(response);
 assert(strcmp(response,""));
 client->nbytes+=len;
 mutex->Wait();
 shm->AppendGlobalBytes((int64_t)len);
 idx = shm->GetClientIdxByPID(client->pid);
 assert(idx>=0);
 shm->AppendClientBytes(idx, (int64_t)len);
 mutex->Signal();
 total=0;
 if (!large)
 {
 while (total<len)
   {
    nwrt=write(fd, response+total, len-total);
    total += nwrt;
    Debug("SF: Written part: nwrt=%d len=%d total=%d\n",nwrt,len,total);
    if (nwrt<0) 
      { 
       perror("write");
       Debug("Client probably lost part of data, or exited without recieving data.\n");
       if (http_parser) delete http_parser;
       http_parser = NULL;
       error = 1;
       return 0;
      }
   }
 }
 else
 {
  int headersiz;
  headersiz = strlen(response);
  total = 0;
  while (total<headersiz)
   {
    nwrt=write(fd, response+total, headersiz-total);
    total += nwrt;
    Debug("LFHDR: Written hdr_part: nwrt=%d len=%d total=%d\n",nwrt,len,total);
    if (nwrt<0) 
      { 
       perror("write");
       Debug("LFHDR: Client probably lost part of data, or exited without recieving data.\n");
       if (http_parser) delete http_parser;
       http_parser = NULL;
       error = 1;
       return 0;
      }
   }
  char buffer[BUFFER_SIZE];
  int totalread, nread;
  int totalwrite, nwrite;
  int buflen;
  int rfd = fileno(handler);
  total = 0;
  while (total<len)
   {
    totalread = 0;
    while (totalread<BUFFER_SIZE)
      {
       nread = read(rfd, buffer+totalread, BUFFER_SIZE);
       totalread += nread;
       if (nread<0)
         {
          perror("read");
          Debug("LF: Client probably lost part of data, or exited without recieving data.\n");
          if (http_parser) delete http_parser;
          http_parser = NULL;
          error = 1;
          return 0;
         }
       if (nread==0) 
         { 
          buflen = totalread; 
	  totalread=BUFFER_SIZE; 
	 }
       else buflen = BUFFER_SIZE;
      }
    totalwrite = 0;
    while (totalwrite<buflen)
      {
       nwrite = write(fd, buffer+totalwrite, buflen);
       totalwrite += nwrite;
       if (nwrite<0)
         {
          perror("write");
          Debug("LF: Client probably lost part of data, or exited without recieving data.\n");
          if (http_parser) delete http_parser;
          http_parser = NULL;
          error = 1;
          return 0;
         }
      }
    total += totalwrite;
    Debug("LF: Written total=%d/%d\n",total, len);
   }
 }
 Debug("LF: Written All: total=%d/%d\n",total, len);
 assert(total==len);
 if (large) Debug("LF completed.\n");
 Debug("WRITTEN ALL.\n");
 if (http_parser) delete http_parser;
 http_parser = NULL;
 return (len>0);
}

////
//	process client logged in at client->sock
//	register it, read from data from it
//	connection is closed if read <=0 bytes
//	or answer to request generated HTTP error
//	fe: NOT_IMPLEMENTED or NOT_FOUND
//	child state is set, client registered in SHM
//	and deregistered after closed connection
//	this method is RUN only by children, parent
//	returns immadiately and waits for another connection
////
void XServerMain :: ProcessClient(struct ClientInfo* client)
{
 int fd,nread,error;
 char buff[MSGSIZ+1];
 assert(client);
 SetChildState();
 client->pid = getpid();
 mutex->Wait();
 if (shm->AddClient(client)==FAILED) 
   {
    mutex->Signal();
    Debug("FAILED to register client.\n");
    return;
   }
 printf("Client registered, now clients: %d.\n", shm->GetClientsCount());
 mutex->Signal();
 fd = client->sock;
 nread=1;
 while (nread>0)
   {
    nread = read(fd, buff, MSGSIZ);	//FIXME CAN READ NOT ENOUGH?
    buff[nread] = 0;
    error = 0;
    if (nread>0) nread=AnswerRequest(fd, buff, nread, client, error);
    strcpy(buff,"");
    if (error) break;
   }
 printf("Client exiting, PID=%d\n", client->pid);
 mutex->Wait();
 if (shm->RemoveClient(client->pid)==FAILED) 
	 printf("Failed to remove client with PID=%d\n", client->pid);
 close(client->sock);
 mutex->Signal();
 Debug("Exited.\n");
}

////
//	Accept connection, if there is less than max client
//	process it else covardly refuse to speak with client
//	and close socket, fork new process to
//	handle client and parent return immadiately
////
void XServerMain :: AcceptClient()
{
 int sock;
 static struct sockaddr_in addr;
 static socklen_t size;
 static struct ClientInfo client;
 mutex->Wait();
 if (shm->GetClientsCount()>=max_clients)
   {
    assert((sock=accept(client_socket,NULL,NULL))!=-1);
    printf("Too much clients.\n");
    close(sock);
    mutex->Signal();
    return ;
   }
 size = (socklen_t)(sizeof(struct sockaddr_in));
 assert((sock=accept(client_socket,
	(struct sockaddr*)(&addr),&size))!=-1);
 shm->SetClientSocket(shm->GetClientsCount(), sock);
 printf("Client connected from: %s\n", StringAddress(&addr));
 memcpy(&client.addr, &addr, sizeof(struct sockaddr_in));
 client.nbytes = 0;
 client.sock = sock;
 mutex->Signal();
 if (!fork()) { ProcessClient(&client); exit(0); }
 return;
}

////
//	suspend all clients by sending them SIGSTOP
//	pids are taken from SHM
//	NOTE if You manually kill client
//	with kill -9, it will not have time
//	to finish and server wont't notice
//	its death, then zombie data (not zombie process)
//	will remain in SHM, and server won't be able to 
//	send signal to this client, if server detects
//	this it will immadiately remove such child from SHM
//	detection is possible in SUSPEND, RESET, RESUME opts
////
void XServerMain :: SuspendAll()
{
 printf("Suspend all clients.\n");
 int nclients,sock;
 mutex->Wait(); 
 if (kill(log_thread, SIGSTOP)==-1) 
   Debug("Error suspending LOGGER, PID: %d, already stopped or exited\n", log_thread);
 nclients = shm->GetClientsCount(); 
 for (int i=0;i<nclients;i++) 
   {
    Debug("PID=%d\n", shm->GetClientPID(i));
    if (kill(shm->GetClientPID(i), SIGSTOP)==-1) 
      {
       Debug("Cannot suspend %d child, already stopped or exited, continuing...\n",i);
       sock = shm->GetClientSocket(i);
       if (sock>0) close(sock);
       if (shm->RemoveClient(shm->GetClientPID(i))==FAILED)
	    Debug("Failed to remove client with PID=%d\n", shm->GetClientPID(i));
      }
   }
 mutex->Signal();
}

////
//	RESUME clients by SIGCONT
//	read comment at SuspendAll()
////
void XServerMain :: ResumeAll()
{
 printf("Resume all clients.\n");
 int nclients,sock;
 mutex->Wait(); 
 if (kill(log_thread, SIGCONT)==-1) 
   Debug("Error resume LOGGER, PID: %d, exited before\n", log_thread);
 nclients = shm->GetClientsCount(); 
 for (int i=0;i<nclients;i++) 
   {
    Debug("PID=%d\n", shm->GetClientPID(i));
    if (kill(shm->GetClientPID(i), SIGCONT)==-1) 
      {
       Debug("Cannot resume %d child, exited before, continuing...\n",i);
       sock = shm->GetClientSocket(i);
       if (sock>0) close(sock);
       if (shm->RemoveClient(shm->GetClientPID(i))==FAILED)
	    Debug("Failed to remove client with PID=%d\n", shm->GetClientPID(i));
      }
   }
 mutex->Signal();
}

////
//	resets server, closes all connections
//	removes all clients, closes all client sockets
//	after that server is awaiting another connections
////
void XServerMain :: ResetServer()
{
 printf("Resetting server\n");
 int nclients,sock;
 mutex->Wait(); 
 nclients = shm->GetClientsCount(); 
 for (int i=nclients-1;i>=0;i--) 
   {
    if (kill(shm->GetClientPID(i), SIGUSR1)==-1) 
       Debug("Cannot send signal to %d child, exited before, continuing...\n",i);
    sock = shm->GetClientSocket(i);
    if (sock >= 0) close(sock);
    if (shm->RemoveClient(shm->GetClientPID(i))==FAILED)
	    Debug("Failed to remove client with PID=%d\n", shm->GetClientPID(i));
   }
 Debug("Waiting for clients to finish...\n");
 while (shm->GetClientsCount()>0) usleep(SHEDULER_INTERVAL);
 printf("Full reset complete.\n");
 mutex->Signal();
}

////
//	returns socket of client with given pid, or -1
////
int XServerMain :: GetSocketByPID(const pid_t pid) const
{
 assert(shm);
 return shm->GetSocketByPID(pid); 
}

////
//	Accepts connection of admin, admin can be only
//	one at the time, admin is processed using
//	iteration method, server reads one command
//	executes is understand and closes connection
//	commends are (downcased by server):
//	suspend, resume, reset, halt
////
void XServerMain :: AcceptAdmin()
{
 int sock,nread,got,len;
 char cmd[MSGSIZ+1];
 static struct sockaddr_in addr;
 static socklen_t size;
 size = (socklen_t)(sizeof(struct sockaddr_in));
 assert((sock=accept(admin_socket,
	(struct sockaddr*)(&addr),&size))!=-1);
 printf("Admin connected from: %s\n", StringAddress(&addr));
 nread=read(sock, cmd, 16);
 got=0;
 if (nread>0)
   {
    Debug("AdminCMD = '%s'\n", cmd);
    len = strlen(cmd);
    if (cmd[len-1]==' ' || cmd[len-1]=='\t' || cmd[len-1]==CR || cmd[len-1]==LF) cmd[len-1] = 0;
    if (cmd[len-2]==' ' || cmd[len-2]=='\t' || cmd[len-2]==CR || cmd[len-2]==LF) cmd[len-2] = 0;
    for (unsigned int i=0;i<strlen(cmd);i++) if (cmd[i]>='A' && cmd[i]<='Z') cmd[i] += 0x20;
    if      (!strcmp(cmd, "suspend")) { SuspendAll();  got=1; }
    else if (!strcmp(cmd, "resume"))  { ResumeAll();   got=1; }
    else if (!strcmp(cmd, "reset"))   { ResetServer(); got=1; }
    else if (!strcmp(cmd, "halt"))    
       { 
	Debug("HALT request from admin, from: %s\n",StringAddress(&addr));
	write(sock,"SHRED", 6);
	close(sock);
	kill(getpid(), SIGINT); 
	return ;
       }
    if (got) { write(sock,"OK", 3); Debug("Admin command was OK\n"); }
    else     { write(sock,"UNKNOWN", 8); Debug("Unknown Admin command.\n"); }
   }
 else Debug("Cannot read admin command.\n");
 close(sock);
 return;
}

////
//	listens or socket, uses select to determine which socket got
//	connection (admin or client) admin socket is accepted and
//	processed iterally, and client socket is accepted and processed
//	parallel, fd sets are recovered after select
////
void XServerMain :: Listen()
{
 fd_set set,oldset;
 assert(listen(admin_socket ,BACKLOG_LENGTH)!=-1);
 assert(listen(client_socket,BACKLOG_LENGTH)!=-1);
 FD_ZERO(&set);
 FD_SET(client_socket, &set);
 FD_SET(admin_socket, &set);
 oldset=set;
 int err;
 while (1)
 {
  if ((err=select(FD_SETSIZE, &set, NULL,NULL,NULL))==-1) perror("select");
  assert(err != -1);
  Debug("Activity on socket, %d\n",err);
  if (FD_ISSET(client_socket, &set))  { Debug("CLIENT\n"); AcceptClient(); }
  if (FD_ISSET(admin_socket, &set))   { Debug("ADMIN\n"); AcceptAdmin(); }
  Debug("PARENT ON NEXT SELECT!\n");
  set=oldset;
 }
}

////
//	set log file pointer, asserts not NULL
////
void XServerMain :: SetLogFilePointer(const FILE* fp)
{
 assert(fp);
 log = (FILE*)fp;
}

////
//	returns global MUTEX (semaphore)
////
XSemaphore* XServerMain ::GetMutex() const
{
 if (!mutex) printf("WARNING: mutex is NULL %s:%d\n", __FILE__,__LINE__);
 return mutex;
}

////
//	returns global Shared Memory Segment Pointer
////
XShM* XServerMain :: GetSHM() const
{
 if (!shm) printf("WARNING: shm is NULL %s:%d\n", __FILE__,__LINE__);
 return shm;
}

////
//	returns log filename
////
char* XServerMain :: GetLogFileName() const
{
 if (!log_file) printf("WARNING: log_file is NULL %s:%d\n", __FILE__,__LINE__);
 return log_file;
}
