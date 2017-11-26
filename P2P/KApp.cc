#include "Common.hh"
#include "KApp.hh"
#include "KGetOptions.hh"
#include "KMemoryException.hh"
#include "KIOException.hh"
#include "KList.hh"
#include "KFile.hh"

//Constructor, sets default values and rememberes WorkingDirectory
KApp :: KApp()
{
 port      = DEFAULT_PORT;
 console   = false;
 numactive = DEFAULT_N;
 download  = NULL;
 upload    = NULL;
 daemon    = false;
 hostsfile = NULL;
 log       = NULL;
 known_servers  = NULL;
 active_servers = NULL;
 files_found    = NULL;
 downloads	= NULL;
 uploads	= NULL;
 mutex          = NULL;
 num_conns      = 0;
 preserver       = 0;
 udp_listener    = 0;
 tcp_listener    = 0;
 udp_socket      = -1;
 tcp_socket	 = -1;
 tcp_port	 = DEFAULT_PORT;
 try
   {
    start_dir = new char[PATH_MAX+1];
    if (!start_dir) throw new KMemoryException("cannot allocate start_dir");
    getcwd(start_dir, PATH_MAX);
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    exit(1);
   }
}

//checks if download/upload directories exists
void KApp :: CheckDirectories()
{
 if (chdir(download)==-1)
   {
    printf("Bad download dir: %s\n", download);
    exit(1);
   }
 if (chdir(upload)==-1)
   {
    printf("Bad download dir: %s\n", upload);
    exit(1);
   }
 if (chdir(start_dir)==-1)
   {
    printf("Cannot return to start_dir: %s\n", start_dir);
    exit(1);
   }

}

//coundowns about 2 seconds and prints .
void* Countdown(void* dummy)
{
 for (int i=0;i<6;i++)
   {
    printf(".");
    fflush(stdout);
    usleep(300000);
   }
 return NULL;
}

//deletes class, unlockes mutex first
//because it can be called from anywhere
KApp :: ~KApp()
{
 if (log) log->Debug("Freeing resources...");
 if (mutex) mutex->Signal();
 if (download)  delete [] download;
 if (upload)    delete [] upload;
 if (hostsfile) delete [] hostsfile;
 if (start_dir) delete [] start_dir;
 hostsfile = download = upload = start_dir = NULL;
 alarm(DEFAULT_TIMEOUT);	//alarm before try to lock mutex
 pthread_t pth;
 pthread_create(&pth, NULL, Countdown, NULL);
 if (mutex) mutex->Wait();	//lock mutex, othrer threads cannot free mem asynchronuously
 alarm(0);
 printf("\n");
 if (known_servers) FreeList(known_servers);	//frees entire list
 known_servers = NULL;
 if (active_servers) FreeList(active_servers);
 active_servers = NULL;
 if (files_found) FreeList(files_found);
 files_found = NULL;
 if (downloads) FreeList(downloads);
 downloads = NULL;
 if (uploads) FreeList(uploads);
 uploads = NULL;
 mutex = NULL;
 if (log) delete log;
 log = NULL;
 if (udp_socket > 0) close(udp_socket);
 udp_socket = -1;
 if (tcp_socket > 0) close(tcp_socket);
 tcp_socket = -1;
 preserver = udp_listener = tcp_listener = 0;
 if (mutex) mutex->Signal();			//frees mutex
 if (mutex) delete mutex;			//deltes mutex, from now synchronization impossible
 printf("Memory freed.\n");			//but end of destructor reached
}

//frees list, should be called synchronized
void KApp :: FreeList(KList* list)
{
 assert(list);
 KList *h, *p;
 h = list;
 while (h)
   {
    p = h;
    h = h->Next();
    delete p;
    p = NULL;
   }
}
//returns char* addres from in_addr struct
char* StringAddress(struct sockaddr_in* addr)
{
 if (!addr) return "(null)";
 return inet_ntoa(addr->sin_addr);
}

//Get options from KGetOptions class
//Run getopt internally
void KApp :: GetOptions(int argc, char** argv)
{
 KGetOptions* kgo=NULL;
 try
   {
    kgo = new KGetOptions(argc, argv);
    if (!kgo) throw new KMemoryException("cannot allocate KGetOption class");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 port      = kgo->GetPort();
 console   = kgo->GetConsole();
 numactive = kgo->GetNumActive();
 daemon    = kgo->GetDaemon();
 try
   {
    download = new char[strlen(kgo->GetDownloadDir())+1];
    if (!download) throw new KMemoryException("cannot allocate download");
    upload = new char[strlen(kgo->GetUploadDir())+1];
    if (!upload) throw new KMemoryException("cannot allocate upload");
    hostsfile = new char[strlen(kgo->GetHostsFile())+1];
    if (!hostsfile) throw new KMemoryException("cannot allocate hostsfile");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 strcpy(download, kgo->GetDownloadDir());
 strcpy(upload, kgo->GetUploadDir());
 strcpy(hostsfile, kgo->GetHostsFile());
 if (kgo) delete kgo;
 printf("If daemon mode enabled, then console unavailable.\n");
 assert(!daemon || (daemon && !console));
 assert (download && upload && hostsfile);
}

//sets udp_sopket to sock
void KApp :: SetUDPSocket(int sock)
{
 assert(sock>=0);
 udp_socket = sock;
}

//similar to above
void KApp :: SetTCPSocket(int sock)
{
 assert(sock>=0);
 tcp_socket = sock;
}

//creates KSysLog class
//sets it to appropriate state
//after this methot logging is enabled
void KApp :: StartLogger()
{
 log = NULL;
 try
   {
    log = new KSysLog();
    if (!log) throw new KMemoryException("cannot create class KSysLog");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 log->SetDaemonMode(daemon);	//daemon writes only to log
 log->SetConsoleMode(console);	//console writes to log, and to stdout only Printfs and Errors
 				//if no console mode and no daemon mode
				//then all logs printed to stdout to
				//Debug used only if -DDEBUG set from compiler
 log->AddMessage("Logging started, PID is %d", getpid());
}

//go to daemon mode
//after fork leave daemon child with all descriptors closed
//parent exits
void KApp :: Daemonize()
{
 if (fork()) exit(0);
 close(0);
 close(1);
 close(2);
 log->AddMessage("Daemon mode activated: my pid %d", getpid());
}

//returns char* ip of char* host if it is possible
//it is possible when host in /etc/hosts
//or DNS server set and network is up
//failure will not confuse the server
//it will continue without this host
char* KApp :: GetHostIP(char* name) const
{
 assert(name);
 struct hostent* hose;
 struct in_addr addr;
 char* to;
 try
   {
    to = new char[20];
    if (!to) throw new KMemoryException("cannot alocate memory for IPstring");
    hose = gethostbyname(name);
    if (!hose) throw new KException("host is unknown");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
    return NULL;
   }
 catch (KException* kex)
   {
    kex->PrintType();
    log->Error("Cannot get IP for hostname: %s IS DNS/Gateway set?", name);
    delete kex;
    perror("gethostbyname");
    return NULL;
   }
 memcpy((void*)&addr,(void*)(hose->h_addr_list[0]),sizeof(struct in_addr));
 strcpy(to, inet_ntoa(addr));
 return to;
}

//adds host to known_hosts list
//host name and port in 'line' arg
//it will be processed to get IP
//and if successfull added to known_hosts list
void KApp :: AddServer(char* line)
{
 assert(line);
 int s_port = DEFAULT_PORT;
 char s_name[MAX_LINE_LENGTH+1];
 char* s_ip;
 in_addr_t s_addr;
 for (unsigned int i=0;i<strlen(line);i++) if (line[i]==':') line[i] = ' ';
 sscanf(line, "%s %d", s_name, &s_port);
 assert(strlen(s_name)<MAX_SERVER_NAME);
 alarm(DEFAULT_TIMEOUT);
 s_ip=GetHostIP(s_name);
 alarm(0);
 if (!s_ip)
   {
    log->Printf("Cannot get IP for host: %s skipping...", s_name);
    return;
   }
 s_addr = inet_addr(s_ip);
 if (s_addr == INADDR_NONE)
   {
    log->Printf("Cannot get IP for host: %s, %s is bad.", s_name, s_ip);
    delete s_ip;
    return;
   }
 if (!known_servers)
   {
    try
      {
       known_servers = new KList();
       if (!known_servers) throw new KMemoryException("Cannot allocate KList class");
       known_servers->SetPort(s_port);
       known_servers->SetAddr(s_addr);
       known_servers->SetName(s_name);
       known_servers->SetStringIP(s_ip);
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       abort();
      }
   }
 else
   {
    try
      {
       KList* head = known_servers;
       known_servers = new KList();
       if (!known_servers) throw new KMemoryException("Cannot allocate KList class");
       known_servers->SetPort(s_port);
       known_servers->SetAddr(s_addr);
       known_servers->SetName(s_name);
       known_servers->SetStringIP(s_ip);
       known_servers->SetNext(head);
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       abort();
      }
   }
 delete s_ip;
}

//returns list's head
KList* KApp :: GetKnownServers() const
{
 if (!log) printf("WARNING: known_servers is NULL %s %d\n", HERE);
 return known_servers;
}

//similar to above
KList* KApp :: GetActiveServers() const
{
 return active_servers;
}

//returns system log object
KSysLog* KApp :: GetLog() const
{
 if (!log) printf("WARNING: log is NULL %s %d\n", HERE);
 return log;
}

//returns mutex object
KMutex* KApp :: GetMutex() const
{
 if (!mutex) printf("WARNING: mutex is NULL %s %d\n", HERE);
 return mutex;
}

//returns number of currently active connections to known servers
//syncer will every 30 second actualize it
//if we have not enough servers
//desired number of server is in numactive variable
int KApp :: GetNumConnections() const
{
 return num_conns;
}

//returns socket
int KApp :: GetUDPSocket() const
{
 if (udp_socket<=0) printf("WARNING: udp_socket is: %d %s %d\n", udp_socket, HERE);
 return udp_socket;
}

//same as above
int KApp :: GetTCPSocket() const
{
 if (tcp_socket<=0) printf("WARNING: tcp_socket is: %d %s %d\n", tcp_socket, HERE);
 return tcp_socket;
}

//returns number of connections
//that server wants to reach
//currently reached is num_conns
int KApp :: GetDesiredConnections() const
{
 return numactive;
}

//returns port number
int KApp :: GetPort() const
{
 return port;
}

//this is thread function
//it will keep connection with server alive
//and every time interval
//will ping it to check if it is ok
//if ok server will answer
//data is passed by ThreadDataPasser struct
//if connection is lost DecCons will be called to remove
//server from active list
void* KeepAliveNotifier(void* pointer)
{
 struct ThreadDataPasser* tdp;
 int sock;
 KList* head;
 KList* self;
 KApp*  klass;
 KSysLog* log;
 KMutex* mut;
 tdp = (struct ThreadDataPasser*)(pointer);
 head  = tdp->head;
 self  = tdp->elem;
 klass = tdp->klass;
 assert(klass && self && head);
 delete tdp;
 sock = self->GetSocket();
 assert(sock>=0);
 tdp = NULL;
 log = klass->GetLog();
 assert(log);
 mut = klass->GetMutex();
 assert(mut);
 log->Debug("KeepAliveNotifier response, sending package to server %s:%d", self->GetStringIP(), self->GetPort());
 int32_t mtype;
 int nwrite;
 int nread;
 char buff[BUF_SIZE+1];
 mtype = 1;
 while (true)
   {
    //printf("KAN Write: %s:%d\n", self->GetStringIP(), self->GetPort());
    mtype = 1;
    mut->Wait();
    nwrite = write(sock, (void*)(&mtype), 4);
    //printf("KAN_NWRITE = %d\n", nwrite);
    if (nwrite == -1 || nwrite == 0)
      {
       log->Warning("Write failed %s %d, tried to: %s:%d", HERE,self->GetStringIP(), self->GetPort());
       //mut->Wait();
       //remove from active_servers
       //printf("deccons - from read\n");
       if (!klass->DecCons(self)) log->Debug("Cannot remove server from list.");
       //mut->Signal();
       close(sock);
       mut->Signal();
       goto label_lost;
      }
    else
     {
      //printf("KAN Read: %s:%d\n", self->GetStringIP(), self->GetPort());
      nread = read(sock, (void*)(buff), BUF_SIZE);
      //printf("KAN_NREAD = %d\n", nread);
      if (nread == -1 || nread == 0)
        {
         log->Warning("Read failed %s %d, tried from: %s:%d", HERE,self->GetStringIP(), self->GetPort());
         //mut->Wait();
         //remove from active_servers
	 //printf("deccons - from read\n");
         if (!klass->DecCons(self)) log->Debug("Cannot remove server from list.");
         //mut->Signal();
	 close(sock);
	 mut->Signal();
         goto label_lost;
      }
      else  //add description to active servers list
        {
         buff[nread] = 0;	//for safe
         //mut->Wait();
	 KList* l = klass->GetActiveServers();
	 while (l)
	   {
            if (l->GetPort() == self->GetPort() && l->GetAddr() == self->GetAddr())
	      {
               l->SetDescr(buff+8);	//description from server
	       break;
	      }
	    l = l->Next();
	   }
	 //mut->Signal();
        }
     }
    mut->Signal();
    sleep(PING_INTERVAL);		//check every PING_INTERVAL seconds
   }
 label_lost:
 log->Warning("Lost connection to: %s %d", self->GetStringIP(), self->GetPort());
 return NULL;
}

//adds found file stored in srv->file_name
//to files_found list
//NOTE: this will not start downloading
//'files_found' is the list of files that CAN be downloaded
//and 'downloads' are list of files downloading NOW
//when file is downloaded (which is done by other function)
//entry will be removed from 'files_found' and 'downloads' list
//this list 'files_found' is used by 'lfind' command
//and 'downloads' list is used by 'listd' command
bool KApp :: AddDownloadEntry(KList* srv)
{
 assert(srv);
 if (!srv->GetFileName()) 
   {
    log->Debug("Will not add NULL file to download list");
    return false;
   }
 KList* list = files_found;
 if (files_found)
   {
    while (list)
      {
       if (list->GetFileName() && !strcmp(list->GetFileName(), srv->GetFileName()))
        {
         log->Debug("Such file already on list.");
         return false;
        }
       list = list->Next();
      }
   }
  if (!files_found)
   {
    try
      {
       files_found = new KList(srv);
       if (!files_found) throw new KMemoryException("Cannot allocate KList class");
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       abort();
      }
   }
 else
   {
    try
      {
       KList* head = files_found;
       files_found = new KList(srv);
       if (!files_found) throw new KMemoryException("Cannot allocate KList class");
       files_found->SetNext(head);
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       abort();
      }
   }
 log->Debug("Download entry added.");
 return true;
}

//adds 'fn' to 'uploads' list
//after upload it will be removed (this is done by other function)
bool KApp :: InsertUEntry(char* fn)
{
 assert(fn);
 mutex->Wait();
 if (!uploads)
   {
    try
      {
       uploads = new KList(fn);
       if (!uploads) throw new KMemoryException("Cannot allocate KList class");
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       mutex->Signal();
       abort();
      }
   }
 else
   {
    try
      {
       KList* head = uploads;
       uploads = new KList(fn);
       if (!uploads) throw new KMemoryException("Cannot allocate KList class");
       uploads->SetNext(head);
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       mutex->Signal();
       abort();
      }
   }
 log->Debug("Added download entry");
 mutex->Signal();
 return true;
}

//removes 'fn' entry from 'uploads' list
//this is typically called after upload has finished
//or there were upload error (when upload ends from any reason)
bool KApp :: RemoveUEntry(char* fn)
{
 assert(fn);
 mutex->Wait();
 KList* list = uploads;
 KList* prev = list;
 while (list)
   {
    if (!strcmp(fn, list->GetFileName()))
      {
       if (list == uploads)
         {
          uploads = uploads->Next();
	  list->SetNext(NULL);
	  delete list;
	  list = NULL;
	  goto deleted;
         }
       else
         {
          prev->SetNext(list->Next());
	  list->SetNext(NULL);
	  delete list;
	  list = NULL;
	  goto deleted;
         }
       break;
      }
    prev = list;
    list = list->Next();
   }
 log->Debug("No such element: filename=%s in DecConns", fn);
 mutex->Signal();
 return false;
 deleted:
 log->Debug("Upload entry removed");
 mutex->Signal();
 return true;
}

//removes entry from 'downloads'
//similar to RemoveUEntry
//any function working on KApp's KLists SHOULD BE CALLED synchronized
bool KApp :: RemoveDEntry(KList* srv)
{
 assert(srv);
 mutex->Wait();
 KList* list = downloads;
 KList* prev = list;
 while (list)
   {
    if (!strcmp(srv->GetFileName(), list->GetFileName()))
      {
       if (list == downloads)
         {
          downloads = downloads->Next();
	  list->SetNext(NULL);
	  delete list;
	  list = NULL;
	  goto deleted;
         }
       else
         {
          prev->SetNext(list->Next());
	  list->SetNext(NULL);
	  delete list;
	  list = NULL;
	  goto deleted;
         }
       break;
      }
    prev = list;
    list = list->Next();
   }
 log->Debug("No such element: filename=%s in DecConns", srv->GetFileName());
 mutex->Signal();
 return false;
 deleted:
 log->Debug("Download entry removed");
 mutex->Signal();
 return true;
}

//insert download entry to 'downloads' list
//typically called when download ends (from any reason: error or completed)
//after download finished entry will be removed from 'files_found' list too
//(but this is done but other function)
bool KApp :: InsertDEntry(KList* srv)
{
 assert(srv);
 mutex->Wait();
 if (!downloads)
   {
    try
      {
       downloads = new KList(srv);
       if (!downloads) throw new KMemoryException("Cannot allocate KList class");
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       mutex->Signal();
       abort();
      }
   }
 else
   {
    try
      {
       KList* head = downloads;
       downloads = new KList(srv);
       if (!downloads) throw new KMemoryException("Cannot allocate KList class");
       downloads->SetNext(head);
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       mutex->Signal();
       abort();
      }
   }
 log->Debug("Added download entry");
 mutex->Signal();
 return true;
}

//adds entry to active_servers list
//if it doesn't exists on list
//should be called synchronously
bool KApp :: IncCons(KList* srv)
{
 assert(srv);
 in_addr_t addr = srv->GetAddr();
 int prt        = srv->GetPort();
/* printf("ADDING active server: %d %d...\n", addr, prt);*/
 KList* list = active_servers;
 if (active_servers)
   {
    while (list)
      {
       if (list->GetAddr() == addr && list->GetPort() == prt)
        {
         log->Debug("Such server already on list.");
         return false;
        }
       list = list->Next();
      }
   }
  if (!active_servers)
   {
    try
      {
       active_servers = new KList(srv);
       if (!active_servers) throw new KMemoryException("Cannot allocate KList class");
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       abort();
      }
   }
 else
   {
    try
      {
       KList* head = active_servers;
       active_servers = new KList(srv);
       if (!active_servers) throw new KMemoryException("Cannot allocate KList class");
       active_servers->SetNext(head);
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       abort();
      }
   }
 num_conns++;
 log->Debug("Active server added.");
 return true;
}

//removes server from active_servers list
//called when connection to that server lost
//should be called synchronously
bool KApp :: DecCons(KList* srv)
{
 assert(srv);
 assert(active_servers>0);
 in_addr_t addr = srv->GetAddr();
 int prt        = srv->GetPort();
/* printf("REMOVING inactive server: %d %d...\n", addr, prt);*/
 KList* list = active_servers;
 KList* prev = list;
 while (list)
   {
    if (list->GetAddr() == addr && list->GetPort() == prt)
      {
       if (list == active_servers)
         {
          active_servers = active_servers->Next();
	  list->SetNext(NULL);
	  delete list;
	  list = NULL;
	  goto deleted;
         }
       else
         {
          prev->SetNext(list->Next());
	  list->SetNext(NULL);
	  delete list;
	  list = NULL;
	  goto deleted;
         }
       break;
      }
    prev = list;
    list = list->Next();
   }
 log->Debug("No such element: port=%d in DecConns");
 return false;
 deleted:
 num_conns--;
 log->Debug("InActive server removed.");
 return true;
}

//creates UDP package of given type
//and returns it in buff and length in len
void KApp :: CreateUDPPackage(int32_t mtype, char*& buff, int& len)
{
 assert(mtype>=1);
 buff = NULL;
 len = 0;
 char* msg;
 if (mtype == 2)
   {
    try
      {
       msg = new char[128+strlen(download)+strlen(upload)+strlen(hostsfile)];
       if (!msg) throw new KMemoryException("cannot allocate server_description");
       sprintf(msg, "port=%d, console=%c, daemon=%c, upload_dir=%s, download_dir=%s, active_srvs=%d, hostsfile=%s"
		  , port, (console?('+'):('-')), (daemon?('+'):('-'))
		  , upload, download, num_conns, hostsfile);
       int32_t mlen = strlen(msg);
       buff = new char[mlen + 10];
       if (!buff) throw new KMemoryException("cannot allocate buff for UDP_PKG_2");
       memcpy((void*)buff, (void*)&mtype, 4);
       memcpy((void*)(buff+4), (void*)&mlen, 4);
       memcpy((void*)(buff+8), (void*)msg, mlen);
       len = mlen+8;
       delete msg;	//this is safe because we used memcpy 
       			//and this is other memory location
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       abort();
      }
   }
}

//try connect to srv and if succeded
//add it to active_server list
//and spawn a thread for it to check if
//connection is alive
//pass data in ThreadDataPasser
//connects only to servers that aren't on
//active servers list
void KApp :: TryConnect(KList* srv)
{
 struct ThreadDataPasser* tdp;
 try
   {
    tdp = new ThreadDataPasser();
    if (!tdp) throw new KMemoryException("cannot allocate ThreadDataPasser struct");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 tdp->head = known_servers;
 tdp->elem = srv;
 tdp->klass = this;
 int sock;
 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock == -1) { log->Debug("socket call failed %s %d", HERE); return; }
 struct sockaddr_in saddr;
 saddr.sin_family = AF_INET;
 saddr.sin_port  = htons(srv->GetPort());
 saddr.sin_addr.s_addr = inet_addr(srv->GetStringIP());
 alarm(3);	//give connection a timeout
 if (connect(sock, (struct sockaddr*)(&saddr), sizeof(struct sockaddr_in)) == -1)
   {
    log->Debug("connect failed to %s %d", HERE);
    alarm(0);
    return;
   }
 alarm(0);
 srv->SetSocket(sock);
 mutex->Wait();
 //add to active_servers
 //if cannot add means that exists
 //so then don't use this server
 if (!IncCons(srv)) 
   {
    log->Debug("Cannot add server to list.");
    mutex->Signal();
    close(sock);
    return;
   }
 mutex->Signal();
 srv->Run(KeepAliveNotifier, tdp);
}


//printf server's description
void KApp :: DescribeServer(KList* srv) const
{
 assert(srv);
 char* descr = srv->GetDescr();
 printf("Entry: %s(%s:%d), socket=%d\ndescription ## %s ##\n", srv->GetName(), srv->GetStringIP(), srv->GetPort(), srv->GetSocket(), descr?descr:" (no description) ");
}


//printf active servers with description
//by default it uses silent mode
//when silent mode set and running console it returns immadiately
//because server uses it internal and we won't mess up console
//if console wants data for user request
//sets silent to 0
void KApp :: SyncerInfo(int silent) const
{
 if (silent && console) return ;
 if (!silent) printf("Listing active servers..\n");
 printf("==> Syncer: ***** (%d/%d/%d) *****\n", GetNumConnections(), GetDesiredConnections(), known_servers->GetCount());
 KList* lp;
 mutex->Wait();
 lp = active_servers;
 if (!lp) { printf("No active servers.\n"); return; }
 while (lp)
   {
    DescribeServer(lp);
    lp = lp->Next();
   }
 mutex->Signal();
 printf("===========================\n");
}

//this thread wakes up every 30 seconds
//checks if server got desired number of connection
//if so sleeps again
//else tries to connect to known servers
//connection function used here 'TryConnect'
//skips servers already on list
void* PreserverThread(void* pointer)
{
 KApp* klass = (KApp*)(pointer);
 KSysLog* log;
 assert(klass);
 KList* listp = klass->GetKnownServers();
 KList* ptr;
 assert(listp);
 log = klass->GetLog();
 assert(log);
 log->Debug("PreserverThread started.");
 int nhosts = listp->GetCount();
 log->Debug("Checking if we can connect to desired number of hosts...");
 log->Debug("We have %d entiures in host_list", nhosts);
 log->Debug("And we want to keep %d connections alive...", klass->GetDesiredConnections());
 sleep(1);	//wait until TCP and UDP listener starts asynchronously
 		//after that start connections
 assert( nhosts >= klass->GetDesiredConnections());
 while (true)	// works all the time
   {
    klass->SyncerInfo();
    if (klass->GetNumConnections() < klass->GetDesiredConnections())
      {
       log->Debug("RECONNECT needed.\n");
       ptr = listp;
       while (klass->GetNumConnections() < klass->GetDesiredConnections())
         {
          klass->TryConnect(ptr);
	  ptr = ptr->Next();
	  if (ptr==NULL)
	     {
              if (klass->GetNumConnections() < klass->GetDesiredConnections())
              log->Warning("Cannot reach desired num of servers, reached %d/%d/%d", klass->GetNumConnections(), klass->GetDesiredConnections(), listp->GetCount());
	      log->Debug("Retrying in %d seconds...", PRESERVE_INTERVAL);
	      break;
	     }
         }
      }
    sleep(PRESERVE_INTERVAL);		//usually 30 seconds
   }
 return NULL;
}

//creates new thread running function PreserverThread
//keeping desired number of connection alive
void KApp :: CreateConnectionSyncer()
{
 num_conns = 0;
 assert(!pthread_create(&preserver, NULL, PreserverThread, this));
}

//creates a mutex object
void KApp :: CreateMutex()
{
 try
   {
    mutex = new KMutex();
    if (!mutex) throw new KMemoryException("cannot allocate KMutex class");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 log->Debug("Mutex created.");
}

//creates list of known servers named 'known_servers'
void KApp :: CreateLists()
{
 log->Debug("Creating lists");
 KFile* file = NULL;
 try
   {
    file = new KFile(hostsfile);
    if (!file) throw new KMemoryException("cannot allocate KFile class");
    if (file->OpenR() == -1) throw new KIOException("cannot open file");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 catch (KIOException* kioe)
   {
    kioe->PrintType();
    delete kioe;
    abort();
   }
 char* line;
 line = NULL;
 while ((line=file->GetLine()))
   {
    AddServer(line);
    delete line;
    line = NULL;
   }
 delete file;
 file = NULL;
}

//returns upload directory
char* KApp :: GetUploadDir() const
{
 if (upload == NULL) printf("WARNING: upload is NULL %s %d\n", HERE);
 return upload;
}

//returns download directory
char* KApp :: GetDownloadDir() const
{
 if (download == NULL) printf("WARNING: download is NULL %s %d\n", HERE);
 return download;
}

//searches for pattern in dir
//when find returns dir/found_file
//means that recursive usage of this function
//will bhuild up pathname to file
//if no file matching pattern found returns NULL
//if directory call itself recursive
//skips links
char* MatchInDir(char* dir, char* pattern)
{
 //printf("SEARCHING IN DIRECTORY %s for %s\n", dir, pattern);
 DIR* dp;
 struct dirent* entry;
 struct stat statbufor;
 char wd[PATH_MAX+1];
 if (!dir) return NULL;
 getcwd(wd, PATH_MAX);
 if (chdir(dir)==-1) return NULL;
 dp = opendir(".");
 if (!dp) return NULL;
 char* dname = NULL;
 char* tmp;
 while ((entry = readdir(dp)) != NULL)
   {
      if (stat(entry->d_name,&statbufor)==-1) continue;
      if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
      if (S_ISLNK(statbufor.st_mode)) continue;
      if (S_ISDIR(statbufor.st_mode))
      {
       tmp = MatchInDir(entry->d_name, pattern);
       if (tmp)
         {
          dname = new char[PATH_MAX+1];
	  strcpy(dname, dir);
	  strcat(dname,"/");
	  strcat(dname, tmp);
	  delete tmp;
	  return dname;
         }
       continue;
      }
    if (!fnmatch(pattern, entry->d_name, FNM_PATHNAME))
        {
          dname = new char[PATH_MAX+1];
	  strcpy(dname, dir);
	  strcat(dname,"/");
	  strcat(dname, entry->d_name);
	  return dname;
	}
   }
 chdir(wd);
 return NULL;
}

//will try to find file matching pattern
//starting from upload directory
//and using MatchInDir fuction for directories
//will also generate answer for client
//that file was found or not (with error description)
int FindPattern(KApp* klass, char* pattern, int sock)
{
 KSysLog* log = klass->GetLog();
 assert(log);
 log->Debug("SERVER will search for: %s", pattern);
 char wd[PATH_MAX+1];
 char fn[PATH_MAX+1];
 char buff[PATH_MAX+24];
 strcpy(fn, "");
 getcwd(wd, PATH_MAX);
 KMutex* mut = klass->GetMutex();
 assert(mut);
 if (chdir(klass->GetUploadDir())==-1)
   {
    log->Error("Cannot cd to upload dir: %s", klass->GetUploadDir());
    return -1;
   }
 DIR* dp;
 struct dirent* entry;
 struct stat statbufor;
 int32_t mtype;
 int nwrite;
 dp = opendir(".");
 if (!dp)
   {
    log->Error("Cannot open upload dir: %s", klass->GetUploadDir());
    chdir(wd);
    return -1;
   }
 char* dname = NULL; 
 while ((entry = readdir(dp)) != NULL)
   {
      if (stat(entry->d_name,&statbufor)==-1) continue;
      if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
      if (S_ISLNK(statbufor.st_mode)) continue;
      if (S_ISDIR(statbufor.st_mode))
      {
       dname = MatchInDir(entry->d_name, pattern);
       if (dname)
         {
          //printf("MATCH %s\n", dname);
          strcpy(fn, dname);
	  delete dname;
	  goto fn_find;
         }
       continue;
      }
    if (!fnmatch(pattern, entry->d_name, FNM_PATHNAME))
        {
         strcpy(fn, entry->d_name);
	 goto fn_find;
	}
   }
 chdir(wd);
 closedir(dp);
 dp = NULL;
 no_file:
 mtype = 5;	//NO SUCH FILE MSGTYPE
 memcpy((void*)buff, (void*)(&mtype) , 4);
 nwrite = write(sock, buff, 4);
 log->Debug("NONE found\n");
 return nwrite;
 fn_find:
 closedir(dp);
 dp = NULL;
 mtype = 4;	//FILE FOUND MSGTYPE
 int32_t slen;
 int32_t lpdk = 1;
 off_t flen;
 KFile* file;
 chdir(klass->GetUploadDir());
 /*char bla[PATH_MAX+1];
 getcwd(bla, PATH_MAX);
 printf("INDIR: %s\n", bla);*/
 try
   {
    file = new KFile(fn);
    if (!file) throw new KMemoryException("cannot allocate KFile class");
    if (file->OpenR() == -1) throw new KIOException("cannot open file");
    flen = file->GetLength();
    delete file;
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    chdir(wd);
    mut->Signal();	/* because of ghost deadlock */
    			/* client probably waits on read */
    			/* with mutex loacked, and we want */
    			/* only to free alocated resources */
    abort();		/* will can AbortHandler(), with freed mutex */
   }
 catch (KIOException* kiox)
   {
    kiox->PrintType();
    delete kiox;
    chdir(wd);
    goto no_file;
   }
 slen = strlen(fn);
 memcpy((void*)buff, (void*)(&mtype) , 4);
 memcpy((void*)(buff+4), (void*)(&slen) , 4);
 memcpy((void*)(buff+8), (void*)(fn), slen);
 memcpy((void*)(buff+slen+8), (void*)(&flen), 8);
 memcpy((void*)(buff+slen+16), (void*)(&lpdk), 4);
 nwrite = write(sock, buff, 20+slen);
 log->Debug("server find: %s\n", fn);
 chdir(wd);
 return nwrite;
}

//handles messages from client
//and answert to them
//msg type 1 (ping) answer (2 with description)
//msg type 2 ignored
//msg type 3 will start searching for a given file
//through FindPattern, will return 4 (found) 
//or 5(not found with description)
//4 will contain file_length also and then
//file transfer asynchronously will be started
void* UDPhandler(void* pointer)
{
 assert(pointer);
 ThreadDataPasser* tdp = (ThreadDataPasser*)(pointer);
 assert(tdp);
 KApp* klass = (KApp*)(tdp->klass);
 assert(klass);
 KSysLog* log;
 log = klass->GetLog();
 assert(log);
 int sock;
 sock = tdp->sock;
 KMutex* mut = klass->GetMutex();
 assert(mut);
 delete tdp;
 tdp = NULL;
 int nread;
 int32_t mtype;
 char buff[BUF_SIZE+1];
 while (true)
   {
    //printf("UH Read.\n");
    nread=read(sock, &buff, BUF_SIZE);
    if (nread==-1 || nread==0)
      {
       log->Warning("Read failed %s %d", HERE);
       close(sock);
       return NULL;
      }
    memcpy((void*)&mtype, (void*)buff, 4);
    //log->Warning("Recieved package type %d, from %s:%d", mtype, StringAddress(&caddr), ntohs(caddr.sin_port));
    if (mtype<=0)
      {
       log->Warning("Bad Message Type: %d, there %s %d", mtype, HERE);
       continue;
      }
    int status=0;
    char* cbuff;
    int blen;
    char* pattern = NULL;
    //SHOULDN'T be THERE mutexes??
    switch (mtype)
      {
       case 1:
	       klass->CreateUDPPackage(2, cbuff, blen);
	       if (!cbuff)
	         {
                  log->Warning("CreateUDPPackage returned NULL");
		  continue;
	         }
	       //printf("UH write.\n");
               status = write(sock, cbuff, blen);
	       delete cbuff;
	       cbuff = 0;
	       break;
       case 2:
	       log->Debug("Got msgtype 2.");
	       break;
       case 3:
	       int32_t lpdk;
	       int32_t slen;
	       if (nread<=12) { log->Warning("Message too short."); break; }
	       //buff[nread] = 0;		//for safe
	       memcpy((void*)(&lpdk), (void*)(buff+4), 4);
	       memcpy((void*)(&slen), (void*)(buff+8), 4);
	       try
	         {
                  pattern = new char[slen+20];
		  if (!pattern) throw new KMemoryException("cannot allocate pattern");
                  strncpy(pattern, (buff+12), slen);
		  pattern[slen] = 0;
	         }
	       catch (KMemoryException* kmex)
	         {
                  kmex->PrintType();
		  delete kmex;
		  abort();
	         }
	       status = FindPattern(klass, pattern, sock);
	       delete pattern;
	       pattern = NULL;
	       break;
       default:
       	       log->Warning("Unknown Message Type: %d, there %s %d", mtype, HERE);
	       break;
      }
    if (status == -1)
      {
       log->Warning("Write failed. %s %d", HERE);
       close(sock);
       return NULL;
      }
    else if (status == 0)
      {
       log->Warning("Write 0 bytes. %s %d", HERE);
       close(sock);
       return NULL;
      }
    else 
      {
       //printf("Write was OK\n");
      }
    //If caddr not on known_servers add it to known_servers list*/
    //after use of any pthread operating system
    //always fill structure of client address with zeros
    //so often I cannot get clients address
    //functions accepting client checks if cli_length is 0
    //then they don't try to record new server on known_servers list
   }
 return NULL;
}

//writes known_hosts to file line by line
//in name:port form, if name cannot be found then IP is written
//name is extracted using gethostbyyaddr syscall
//but in another function (Klist(sockaddr_in* addr)
//coinstructor tries to gen name through gethostbyaddr
//addr is taken from accept() call this is done in CheckIfKnown method
void KApp :: WriteKnownHostsFile() const
{
 KFile* file=NULL;
 try
   {
    file = new KFile(hostsfile);
    if (!hostsfile) throw new KMemoryException("cannot allocate KFile struct");
    if (file->OpenW()==-1) throw new KIOException("Cannot write to hostsfile");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    mutex->Signal();
    abort();
   }
 catch (KIOException* kiox)
   {
    kiox->PrintType();
    delete kiox;
    delete file;
    printf("Write new hosts_file failed.\n");
    return;
   }
 KList* lp = known_servers;
 while (lp)
  {
   file->PutHostLine(lp);
   lp = lp->Next();
  }
 delete file;
}

//checks host ip and port
//if it is on known_server list
//if not adds and then writes hosts_file 
//using WriteKnownHostsFile()
//host name is get by gethostbyaddr 
//(read WriteKnownHostsFile description)
void KApp :: CheckIfKnown(struct sockaddr_in* addr, int len)
{
 if (!len) return;
 int prt = addr->sin_port;
 in_addr_t iadr = addr->sin_addr.s_addr;
 if (prt==0 || iadr==0) return;
 mutex->Wait();
 KList* list = known_servers;
 if (known_servers)
   {
    while (list)
      {
	      //we've already got this entry
       if (list->GetAddr() == iadr && list->GetPort() == prt) 
         {
	  mutex->Signal();
	  return;
	 }
       list = list->Next();
      }
   }
  if (!known_servers)
   {
    try
      {
       known_servers = new KList(addr);
       if (!known_servers) throw new KMemoryException("Cannot allocate KList class");
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       mutex->Signal();
       abort();
      }
   }
 else
   {
    try
      {
       KList* head = known_servers;
       known_servers = new KList(addr);
       if (!known_servers) throw new KMemoryException("Cannot allocate KList class");
       known_servers->SetNext(head);
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       mutex->Signal();
       abort();
      }
   }
 log->Debug("New server added to list");
 WriteKnownHostsFile();
 mutex->Signal();
 return;
}

//will listen for connections
//and correctly handle them
void* UDPListener(void* pointer)
{
 assert(pointer);
 KApp* klass = (KApp*)(pointer);
 assert(klass);
 KSysLog* log;
 log = klass->GetLog();
 assert(log);
 int sock;
 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock == -1)
   {
    log->Error("socket call failed in UDPlistener %s %d", HERE);
    abort();
    return NULL;
   }
 struct sockaddr_in saddr;
 struct sockaddr_in caddr;
 socklen_t clilen;
 saddr.sin_family = AF_INET;
 saddr.sin_port = htons(klass->GetPort());
 saddr.sin_addr.s_addr = INADDR_ANY;
 if ((bind(sock, (struct sockaddr*)(&saddr), sizeof(struct sockaddr_in))) == -1)
     {
      log->Error("bind failed in UDPlistener %s %d, addr %s; port %d", HERE, "INADDR_ANY", klass->GetPort());
      klass->GetMutex()->Signal();
      close(sock);
      abort();
      return NULL;
     }
 assert(listen(sock, 5)!=-1);
 log->Debug("UDP listening started at port: %d", klass->GetPort());
 klass->SetUDPSocket(sock);
 int csock;
 while (true)
   {
    if ((csock=accept(sock, (struct sockaddr*)&caddr, &clilen))==-1)
      {
       log->Warning("Accept failed %s %d", HERE);
       continue;
      }
    else
      {
       klass->CheckIfKnown(&caddr, clilen);
       try
         {
          pthread_t thr;
          ThreadDataPasser* tdp = new ThreadDataPasser();
	  if (!tdp) throw new KMemoryException("cannot allocate ThreadDataPasser struct");
	  tdp->klass = klass;
	  tdp->sock  = csock;
	  assert(!pthread_create(&thr, NULL, UDPhandler, (void*)tdp));
         }
       catch(KMemoryException* kmex)
         {
          kmex->PrintType();
	  delete kmex;
	  abort();
         }
      }
   }
 return NULL;
}

//creates listening thread
void KApp :: CreateUDPListener()
{
 assert(!pthread_create(&udp_listener, NULL, UDPListener, this));
}


//generates error to transfer requests
//as said in errstr, mtype = 5
//informations are send send OOB (Out of Bound)
int KApp :: GenerateError(int sock, char* errstr) const
{
 assert(errstr && (strlen(errstr)<MAX_LINE_LENGTH));
 int nwrite, nread;
 char buff[MAX_LINE_LENGTH+10];
 int32_t mtype;
 int32_t len;
 mtype = 5;
 len = strlen(errstr);
 memcpy((void*)buff, (void*)(&mtype), 4);
 memcpy((void*)(buff+4), (void*)(&len), 4);
 memcpy((void*)(buff+8), (void*)(errstr), len);
 nwrite = send(sock, buff, len+9, MSG_OOB);
 if (nwrite <= 0)
   {
    log->Debug("Transfer failed: %s", errstr);
    return nwrite;
   }
 nread = read(sock, (void*)(&mtype), 4);
 if (nread <= 0)
   {
    log->Debug("Error confirmation failed: %s");
    return nread;
   }
 if (mtype != 6) log->Debug("Bad client answer to error: %d\n", mtype);
 //else printf("ERROR HANDLE COMPLETE!\n");
 return nread;
}


//asynchronously uploads file
//it is called in TCPhandler and every TCPhandler
//is a isolated thread
//upload is logged file can be > 4GB
//file is get relative to upload directory
//NOTE: on FreeBSD before starting async I/O's
//do as root: kldload aio
int KApp :: AIORead(int sock, char* fname)
{
 assert(fname && sock>0);
 char wd[PATH_MAX+1];
 chdir(start_dir);
 getcwd(wd, PATH_MAX);
 chdir(upload);
 int fd;
 off_t f_length;
 KFile* file=NULL;
 try
   {
    file = new KFile(fname);
    if (!file) throw new KMemoryException("cannot aloocate class KFile");
    if ((fd=file->OpenR())==-1) throw new KIOException("cannot read file");
    f_length = file->GetLength();
    if (f_length <= 0) throw new KIOException("file length is less or equal 0");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 catch (KIOException* kiox)
   {
    kiox->PrintType();
    chdir(wd);
    delete kiox;
    delete file;
    return -1;
   }
 int32_t mtype;
 char hdr[16];
 int nwrite;
 mtype = 4;	//sending file
 memcpy((void*)hdr, (void*)(&mtype), 4);
 memcpy((void*)(hdr+4), (void*)(&f_length), 8);
 nwrite = write(sock, (void*)hdr, 12);
 if (nwrite != 12)
   {
    log->Error("WriteHdr: Written not enough: %d/12", nwrite);
    chdir(wd);
    delete file;
    return -1;
   }
 log->Printf("Uploading of %s started.", fname);
 if (!InsertUEntry(fname)) printf("Cannot insert UEntry\n");
 //add to ulist
 static struct aiocb* cbr;
 off_t from,to;
 char* aiobuff;
 try
   {
    aiobuff = new char[AIO_BUFSIZ+1];
    if (!aiobuff) throw new KMemoryException("cannot allocate aiobuff");
    cbr = (struct aiocb*)malloc(sizeof(struct aiocb));
    if (!cbr) throw new KMemoryException("cannot malloc aiocbR");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    delete file;
    abort();
   }
 int status = 1;	//status OK
 for (off_t i=0;i<f_length;i+=AIO_BUFSIZ)
  {
    // BEGINS ASYNC VERSION
    cbr->aio_fildes  = fd;
    from             = i;
    to               = i+AIO_BUFSIZ;
    if (to>f_length) to = f_length;
    cbr->aio_offset = from;
    cbr->aio_nbytes = to-from;
    cbr->aio_buf    = aiobuff;
    cbr->aio_sigevent.sigev_notify = SIGEV_NONE;
    //printf("AIOR STARTED\n");
    if (aio_read(cbr)==-1)
      {
       //perror("aio_read");
       log->Printf("aio_read failed");
       chdir(wd);
       delete file;
       free(cbr);
       return -1;
      }
    status = aio_error(cbr);
    if (status == EINPROGRESS)
      {
       //printf("SINPROGRESSR\n");
       if (aio_waitcomplete((struct aiocb**)(&cbr), NULL)==-1)
         {
          perror("aio_suspend");
          log->Printf("upload: aio_suspend failed");
          chdir(wd);
          delete file;
          free(cbr);
          return -1;
         }
       //printf("SAFTER_AIOSUSPR\n");
      }
    else if (status!=0)
      {
       //perror("aio_error");
       log->Printf("aio_error failed");
       chdir(wd);
       free(cbr);
       delete file;
       return -1;
      }
    aio_return(cbr);
    /*if (cbr && aio_return(cbr)==-1)
      {
       perror("aio_return");
       log->Printf("upload: aio_return failed");
       chdir(wd);
       free(cbr);
       delete file;
       return -1;
      }*/
    //printf("SAIO ok i = %lld\n", i);
    // ENDS ASYNC VERSION
    
    
    /* BEGINS SYNC VERSION, without AIO
    to = f_length-i;
    if (to>AIO_BUFSIZ) to = AIO_BUFSIZ;
    off_t total;
    int nread;
    total=0;
    while (total<to)
      {
       nread = read(fd, aiobuff+total, to-total);
       total += nread;
       if (nread <= 0) 
         {  
          log->Debug("Error while executing read\n");
          chdir(wd);
          free(cbr);
          delete file;
          if (!RemoveUEntry(fname)) printf("Cannot remove UEntry\n");
          return -1;
	 }
      }
    assert(total==to);
    //ENDS sync version */
    
    to = f_length-i;
    if (to>AIO_BUFSIZ) to = AIO_BUFSIZ;
    off_t total;
    int nread;
    total=0;
    while (total<to)
      {
       //printf("w_part; %lld\n", total);
       nwrite = write(sock, aiobuff+total, to-total);
       total += nwrite;
       if (nwrite <= 0) 
         {  
          log->Debug("Error while executing write\n");
          chdir(wd);
          free(cbr);
          delete file;
          if (!RemoveUEntry(fname)) printf("Cannot remove UEntry\n");
          return -1;
	 }
      }
    //printf("total = %lld, to = %lld\n", total, to);
    assert(total==to);
    //printf("SWRITE\n");
    int32_t ok;
    nread=read(sock, (void*)(&ok), 4);		//wait for download_part confirmation
    if (nread != 4) 
      { 
       log->Debug("Server: Error occured!"); 
       chdir(wd);
       free(cbr);
       delete file;
       if (!RemoveUEntry(fname)) printf("Cannot remove UEntry\n");
       return -1;
      }
    if (ok != 1)    
      { 
       log->Debug("Bad client ansver to aio_bloc_transferred"); 
       chdir(wd);
       free(cbr);
       delete file;
       if (!RemoveUEntry(fname)) printf("Cannot remove UEntry\n");
      }
    //printf("SCONFIRM ok\n");
    //printf("SLOOP\n");
  }
 log->Printf("Uploading of %s finished.", fname);
 if (!RemoveUEntry(fname)) printf("Cannot remove UEntry\n");
 //remove from ulist
 chdir(wd);
 delete file;
 free(cbr);
 return status;
}

//thread hadles each TCP connection
//every connection is a separate thread
//it answers to msgtype 3
//with 4 (if file found) and start AIO transfer
//with 5 (on error) sends OOB ansver to the client
void* TCPhandler(void* pointer)
{
 assert(pointer);
 ThreadDataPasser* tdp = (ThreadDataPasser*)(pointer);
 assert(tdp);
 KApp* klass = (KApp*)(tdp->klass);
 assert(klass);
 KSysLog* log;
 log = klass->GetLog();
 assert(log);
 int sock;
 sock = tdp->sock;
 delete tdp;
 tdp = NULL;
 int nread;
 int32_t mtype;
 char buff[BUF_SIZE+1];
 //printf("GOT CONNECTION\n");
    //printf("TCPh Read.\n");
    nread=read(sock, &buff, 4);
    if (nread==-1 || nread==0)
      {
       log->Warning("TCPhandler Read failed %s %d", HERE);
       close(sock);
       return NULL;
      }
    memcpy((void*)&mtype, (void*)buff, 4);
    //printf("GOT MSGTYPE=%d\n", mtype);
    //log->Warning("Recieved package type %d, from %s:%d", mtype, StringAddress(&caddr), ntohs(caddr.sin_port));
    if (mtype<=0)
      {
       log->Warning("TCPhandler Bad Message Type: %d, there %s %d", mtype, HERE);
       close(sock);
       return NULL;
      }
    int status=0;
    switch (mtype)
      {
       case 3:
	       char    fnam[PATH_MAX+1];
	       int32_t flen;
	       status = read(sock, (void*)(&flen), 4);
	       if (status != 4) { log->Debug("read failed at %s %d", HERE); break; }
	       if (flen < 0 || flen >= PATH_MAX) { log->Debug("flen: %d: bad value", flen); break; }
	       status = read(sock, (void*)(fnam), flen);
	       if (status != flen) { log->Debug("read failed at %s %d", HERE); break; }
	       fnam[flen] = 0;
	       status = klass->AIORead(sock, fnam); 
	       if (status == -1)
	          status = klass->GenerateError(sock, "Cannot open/find file"); 
	       break;
       default:
       	       log->Warning("TCPhandler Unknown Message Type: %d, there %s %d", mtype, HERE);
	       status = klass->GenerateError(sock, "Unknown mtype"); 
	       break;
      }
    if (status == -1)
      {
       log->Warning("TCPhandler Write failed. %s %d", HERE);
       close(sock);
       return NULL;
      }
    else if (status == 0)
      {
       log->Warning("TCPhandler Write 0 bytes. %s %d", HERE);
       close(sock);
       return NULL;
      }
    //write successful
 close(sock);
 return NULL;
}


//main server method, listens on TCP socket
//if new client connected checks if it is known server
//if not adds to known_servers
//spawns new pthread to handle connections
//wait for another connection
void* TCPListener(void* pointer)
{
 assert(pointer);
 KApp* klass = (KApp*)(pointer);
 assert(klass);
 KSysLog* log = klass->GetLog();
 assert(log);
 int tcp_socket;
 int tcp_port;
 tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
 tcp_port = klass->GetPort()+0x8000;
 if (tcp_socket == -1)
   {
    log->Error("socket call failed in TCPlistener %s %d", HERE);
    abort();
   }
 struct sockaddr_in saddr;
 struct sockaddr_in caddr;
 socklen_t clilen;
 saddr.sin_family = AF_INET;
 saddr.sin_port = htons(tcp_port);
 saddr.sin_addr.s_addr = INADDR_ANY;
 if ((bind(tcp_socket, (struct sockaddr*)(&saddr), sizeof(struct sockaddr_in))) == -1)
     {
      log->Error("bind failed in TCPlistener %s %d, addr %s; port %d", HERE, "INADDR_ANY", tcp_port);
      close(tcp_socket);
      abort();
      return NULL;
     }
 assert(listen(tcp_socket, 5)!=-1);
 klass->SetTCPSocket(tcp_socket);
 log->Debug("TCP listening started at port: %d", tcp_port);
 //printf("TCP listening started at port: %d\n", tcp_port);
 int csock;
 while (true)
   {
    if ((csock=accept(tcp_socket, (struct sockaddr*)&caddr, &clilen))==-1)
      {
       log->Warning("Accept failed %s %d", HERE);
       continue;
      }
    else
      {
       try
         {
          pthread_t thr;
          ThreadDataPasser* tdp = new ThreadDataPasser();
	  if (!tdp) throw new KMemoryException("cannot allocate ThreadDataPasser struct");
	  tdp->klass = klass;
	  tdp->sock  = csock;
	  assert(!pthread_create(&thr, NULL, TCPhandler, (void*)tdp));
         }
       catch(KMemoryException* kmex)
         {
          kmex->PrintType();
	  delete kmex;
	  abort();
	  return NULL;
         }
      }
   }
 return NULL;
}

//creates separate MAIN server thread
//which will implement handling connections
//on own TCP socket
void KApp :: CreateTCPListener()
{
 assert(!pthread_create(&tcp_listener, NULL, TCPListener, this));
}

//No console was used or console exited
//Main thread runs IDLE
//because if it exits all its threads do the same
void KApp :: ServerIdle()
{
 printf("Going IDLE...zzz\n");
 while (true) sleep(PRESERVE_INTERVAL);
 //no return
}

//displays console help
//commands marked with ok works fine
//get SHOULD work fine on FreeBSD
//on other systems I have no idea
//if we even go so far to see this help
void Help()
{
 printf("Console ver 0.1\nAvailable commands are:\n");
 printf("find wildcard             find a wildcard on servers.\n");	//ok
 printf("lfind                     list files found\n");		//ok
 printf("lclear                    clear list of files found\n");	//ok
 printf("get [number]              get n-th file from list\n");		//ok (probaly...)
 printf("lhosts                    list known hosts\n");		//ok
 printf("lactive                   list currently active hosts\n");	//ok
 printf("listd                     list downloads\n");			//ok
 printf("listu                     list uploads\n");			//ok
 printf("info			   informations about current instance\n"); //ok 
 printf("help                      display this help\n");		//ok
 printf("shred                     kill client & server\n");		//ok
 printf("quit                      exit console, continue server\n");	//ok
}

//kills server & client and all subthreads
//just commites suicide on demand
void Shred()
{
 kill(getpid(), SIGINT);
}

//lists known servers
void KApp :: ListKnown() const
{
 printf("Listing known servers..\n");
 KList* lp;
 lp = known_servers;
 if (!lp) { printf("ERROR: I don't know any servers\n"); return; }
 while (lp)
   {
    printf("Server: %s(%s:%d)\n", lp->GetName(), lp->GetStringIP(), lp->GetPort());
    lp = lp->Next();
   }
 printf("===========================\n");
}

//thread to ask server for search for a wildcard
//console caller returns immadiately
//thread data passed in ThreadDataPasser
void* FindWildcard(void* pointer)
{
 assert(pointer);
 struct ThreadDataPasser* tdp;
 tdp = (ThreadDataPasser*)(pointer);
 assert(tdp);
 KApp* klass = (KApp*)(tdp->klass);
 assert(klass);
 KSysLog* log = klass->GetLog();
 assert(log);
 char* pattern = tdp->pattern;
 assert(pattern);
 KList* srv = tdp->elem;
 assert(srv);
 KMutex* mut = klass->GetMutex();
 assert(mut);
 int sock;
 sock = srv->GetSocket();
 int32_t lpdk;
 lpdk = tdp->lpdk;
 delete tdp;
 mut->Wait();
 log->Debug("ASYNC: search for %s, LPDK=%d", pattern, lpdk);
 char* buff;
 int32_t mtype;
 int blen=0;
 int nwrite;
 mtype = 3;
 try
      {
       buff = new char[strlen(pattern)+20];
       if (!buff) throw new KMemoryException("cannot allocate msg");
       memcpy((void*)buff, (void*)&mtype, 4);
       memcpy((void*)(buff+4), (void*)(&lpdk), 4);
       int32_t mlen = strlen(pattern);
       memcpy((void*)(buff+8), (void*)(&mlen), 4);
       memcpy((void*)(buff+12), (void*)(pattern), mlen);
       blen = mlen+12;
      }
    catch (KMemoryException* kmex)
      {
       kmex->PrintType();
       delete kmex;
       mut->Signal();
       abort();
      }
 if (blen<=0 || !buff)
   {
    log->Warning("CreateUDPPackage failed: %s %d", HERE);
    mut->Signal();
    return NULL;
   }
 nwrite = write(sock, (void*)buff, blen);
 if (nwrite <= 0)
   {
    log->Warning("Write failed: %s %d", HERE);
    mut->Signal();
    return NULL;
   }
 else
   {
    int nread;
    int reslt;
    char fbuff[PATH_MAX+24];
    nread = read(sock, fbuff, PATH_MAX+24);
    if (nread <= 0)
      {
       log->Warning("Read failed: %s %d", HERE);
       mut->Signal();
       return NULL;
      }
    reslt = (int)(*fbuff);
    if (reslt == 5)
       printf("File matching \"%s\" was not found.\n",  pattern);
    else if (reslt == 4)
      {
       int32_t slen;
       char fnam[PATH_MAX+1];
       off_t flen;
       memcpy((void*)(&slen), (void*)(fbuff+4), 4);
       memcpy((void*)fnam, (void*)(fbuff+8), slen);
       fnam[slen] = 0;
       memcpy((void*)(&flen), (void*)(fbuff+slen+8), 8);
       KList* ent;
       try
         {
          ent = new KList(srv);
	  if (!ent) throw new KMemoryException("cannot allocate KList class");
	  ent->SetFileLength(flen);
	  ent->SetFileName(fnam);
	  if (klass->AddDownloadEntry(ent))
	    printf("Matching file was found (%s,%lldb) and added to download list\n", fnam, flen); 
	  else printf("Matching file (%s,%lldb) was found but another file\nNamed %s exists on list, so skipping it\n", fnam,flen,fnam);
	  fflush(stdout);
         }
       catch (KMemoryException* kmex)
         {
          kmex->PrintType();
          delete kmex;
          mut->Signal();
          abort();
         }
      }
    else log->Error("Bad server answer type: %d\n", reslt);
   }
 if (buff) delete buff;
 buff = NULL;
 mut->Signal();
 return NULL;
}


//will spawn a new thread for searching a wildcard
//data will be passed by a struct ThreadDataPasser
//exceptions will be handled
void KApp :: AsyncSearch(KList* list, char* pattern)
{
 try
   {
    struct ThreadDataPasser* tdp = new ThreadDataPasser();
    if (!tdp) throw new KMemoryException("cannot allocate ThreadDataStruct");
    tdp->klass = this;
    tdp->pattern = pattern;
    tdp->elem = list;
    tdp->lpdk = 1;		//ONLY to caller
    pthread_t thr;
    assert(!pthread_create(&thr, NULL, FindWildcard, tdp));
   }
 catch (KMemoryException* kmex)
  { 
   kmex->PrintType();
   delete kmex;
   abort();
  }
}

//parses 'cmd' for wildcard
//invokes AsyncSearch for wildcard
//or error if given name is "" or *
//* will return any file (first found in upload what is sensless)
void KApp :: FindFile(char* cmd)
{
 char pattern[MAX_LINE_LENGTH+1];
 strcpy(pattern, "");
 assert(cmd);
 sscanf(cmd, "find %s", pattern);
 if (!strcmp(pattern, "")) 
   {
    printf("You must give an argument for find\nFor example find song.mp3\n");
    return;
   }
 if (!strcmp(pattern, "*")) 
   {
    printf("Will not search for '*' - any file match!\n");
    return;
   }
 log->Printf("Searching for %s", pattern); 
 mutex->Wait();
 KList* lp = active_servers;
 if (!lp) 
   {
    printf("There are no active servers.\n");
    mutex->Signal();
    return;
   }		    
 while (lp)
   {
    AsyncSearch(lp, pattern);
    lp = lp->Next();
   }
 mutex->Signal();
}

//lists active uploads (filenames only)
//because rest in not set in KList entries
void KApp :: ListUploads() const
{
 printf("Listing uploads..\n");
 KList* lp;
 lp = uploads;
 if (!lp) { printf("No uploads\n"); return; }
 int i=0;
 while (lp)
   {
    printf("%d) %s\n", i, lp->GetFileName());
    lp = lp->Next();
    i++;
   }
 printf("===========================\n");
}

//lists downloads with all possible info:
//from (name,ip,port), file_name, file_length, bytes_transferred
//all is stored in KList entries
void KApp :: ListDownloads() const
{
 printf("Listing downloads..\n");
 KList* lp;
 lp = downloads;
 if (!lp) { printf("No downloads\n"); return; }
 int i=0;
 while (lp)
   {
    printf("%d) %s, %lld/%lld bytes from %s(%s:%d)\n", i, lp->GetFileName(), lp->GetTransferred(), lp->GetFileLength(), lp->GetName(), lp->GetStringIP(), lp->GetPort());
    lp = lp->Next();
    i++;
   }
 printf("===========================\n");
}


//list files found on server
//there are files found - not downloaded yet
//so these not downloaded at the moment
//and these downloaded at the moment
//after download of file succeded , this file is
//removed from file_found list
void KApp :: FilesFoundList() const
{
 printf("Listing files found..\n");
 KList* lp;
 lp = files_found;
 if (!lp) { printf("No such files\n"); return; }
 int i=0;
 while (lp)
   {
    printf("%d) File: %s, %lld bytes from %s(%s:%d)\n", i, lp->GetFileName(), lp->GetFileLength(), lp->GetName(), lp->GetStringIP(), lp->GetPort());
    lp = lp->Next();
    i++;
   }
 printf("===========================\n");
}

//constructs path where to store file from server
//for example we've been searching for dir1/dir2/file.3
//and our download dir is ./ddir so it will do
//dir1/dir2/file.3 --> file.3 --> ./ddir/file.3
void KApp :: ConstructDownloadFileName(char* file, char* fnam) const
{
 assert(file);
 int i;
 strcpy(fnam, download);
 if (fnam[strlen(fnam)-1] != '/') strcat(fnam, "/");
 i = strlen(file)-1;
 while (i && file[i] != '/') i--;
 if (!i) strcat(fnam, file);
 else strcat(fnam, (file+i+1));
}

//returns server's working directory
//filled in on server startup
char* KApp :: GetWD() const
{
 if (!start_dir) printf("WARNING: wd is NULL: %s %d\n", HERE);
 return start_dir;
}

//set number of bytes transferred of 'fnam' to 'trans' bytes
//uses mutex for accessing list
void KApp :: SetTransferred(char*fnam, off_t trans) const
{
 mutex->Wait();
 KList* lp = downloads;
 while (lp)
   {
    if (!strcmp(lp->GetFileName(), fnam))
      {
       lp->SetTransferred(trans);
       mutex->Signal();
       return;
      }
    lp = lp->Next();
   }
 mutex->Signal();
}

//separate thread to do an async download
//updates downoaded bytes every AIO_BUFSIZ bytes
//can store files longer than 4GB
//records download in syslog
//saves to download dir only file name
//do not saves server's path to file
//only final file is written just to download directory
void* TCPTransfer(void* pointer)
{
 assert(pointer);
 struct ThreadDataPasser* tdp;
 tdp = (ThreadDataPasser*)(pointer);
 assert(tdp);
 KApp* klass = (KApp*)(tdp->klass);
 assert(klass);
 KSysLog* log = klass->GetLog();
 assert(log);
 KList* file = tdp->elem;
 assert(file);
 assert(file->GetFileName());
 off_t fsiz;
 fsiz = file->GetFileLength();
 char fnam[PATH_MAX+1];
 strcpy(fnam, file->GetFileName());
 delete tdp;
 KList* srv = file;
 KList* srvc = NULL;
 int sock;
 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock == -1) { log->Debug("socket call failed %s %d", HERE); return NULL; }
 struct sockaddr_in saddr;
 saddr.sin_family = AF_INET;
 saddr.sin_port  = htons(file->GetPort()+0x8000);
 saddr.sin_addr.s_addr = inet_addr(file->GetStringIP());
 alarm(3);
 if (connect(sock, (struct sockaddr*)(&saddr), sizeof(struct sockaddr_in)) == -1)
   {
    log->Debug("connect failed to %s %d", HERE);
    return NULL;
   }
 alarm(0);
 log->Debug("Request for transfer: %s(%lld)", fnam, fsiz);
 char buff[PATH_MAX+10];
 int32_t mtype;
 int32_t slen;
 int nwrite;
 int nread;
 int32_t sansver;
 mtype = 3;
 slen = strlen(fnam);
 memcpy((void*)(buff), (void*)(&mtype), 4);
 memcpy((void*)(buff+4), (void*)(&slen), 4);
 memcpy((void*)(buff+8), (void*)(fnam), slen);
 nwrite = write(sock, buff, slen+8);
 if (nwrite <= 0)
   {
    log->Debug("Transfer failed: write failed");
    close(sock);
    return NULL;
   }
 nread = read(sock, (void*)(&sansver), 4);
 if (nread <= 0)
   {
    log->Debug("Transfer failed: read failed");
    close(sock);
    return NULL;
   }
 if (sansver == 5)	//ERROR occured on server's side
   {
    char emsg[MAX_LINE_LENGTH+1];
    char ebuff[MAX_LINE_LENGTH+1];
    log->Debug("Transfer failed: server returned error state");
    sansver = 6;	//ERROR confiramtion
    nread = read(sock, (void*)(ebuff), MAX_LINE_LENGTH+5);
    if (nread <= 0)
      {
       log->Debug("Transfer failed: read failed");
       close(sock);
       return NULL;
      }
    int32_t len;
    memcpy((void*)(&len), (void*)ebuff, 4);
    memcpy((void*)(emsg), (void*)(ebuff+4), len);
    emsg[len] = 0;
    log->Printf("Transfer failed: Error was: %s\n", emsg);
    if (!klass->RemoveDownloadEntry(fnam))
    nwrite = write(sock, (void*)(&sansver), 4);
    if (nwrite <= 0)
      {
       log->Debug("Error while confirming error ;-)");
       close(sock);
       return NULL;
      }
    close(sock);
    return NULL;
   }
 else if (sansver == 4) //OK TRANSFER can be started
   {
    log->Printf("Transfer of %s:%d:(upload_dir)%s started.", srv->GetStringIP(), srv->GetPort(), fnam);
    if (!klass->InsertDEntry(srv)) printf("Cannot insert DEntry\n");
    off_t f_length;
    nread = read(sock, (void*)(&f_length), 8);
    if (nread != 8)
      {
       log->Error("Error while getting file size, not enough bytes: %d/8", nread);
       if (!klass->RemoveDEntry(srv)) printf("Cannot remove DEntry\n");
       close(sock);
       return NULL;
      }
    log->Debug("File size is: %lld\n", f_length); 
    if (f_length <=0)
      {
       log->Error("Bad file size: %lld", f_length);
       if (!klass->RemoveDEntry(srv)) printf("Cannot remove DEntry\n");
       close(sock);
       return NULL;
      }
    char wd[PATH_MAX+1];
    int fd;
    static struct aiocb* cbw;
    getcwd(wd, PATH_MAX);
    //chdir(klass->GetDownloadDir());
    chdir(klass->GetWD());
    KFile* file=NULL;
    off_t from,to;
    char* aiobuff;
    char dfn[PATH_MAX+1];
    try
      {
       aiobuff = new char[AIO_BUFSIZ+1];
       if (!aiobuff) throw new KMemoryException("cannot allocate aiobuff");
       cbw = (struct aiocb*)malloc(sizeof(struct aiocb));
       if (!cbw) throw new KMemoryException("cannot malloc aiocbW");
       klass->ConstructDownloadFileName(fnam, dfn);
       log->Printf("Downloading %s to %s", fnam, dfn);
       file = new KFile(dfn);
       if (!file) throw new KMemoryException("cannot allocate KFile struct");
       if ((fd=file->OpenW(1))==-1) throw new KIOException("cannot write to file");
       srvc = new KList(srv);	//we need a copy because if uses asynchronously use lclear
       				//then he/she will destroy list of files found and also srv which is its member
				//and we are using it to display informations
				//for example when transfer completed we should
				//display what we just had downlkoaded
				//so that whats this copy is for
       if (!srvc) throw new KMemoryException("cannot make copy of srv --> srvc");
     }
 catch (KMemoryException* kmex)
    {
    kmex->PrintType();
    delete kmex;
    abort();
    }
 catch (KIOException* kiox)
   {
    kiox->PrintType();
    delete kiox;
    log->Debug("Transfer failed: I/O exception on client side");
    close(sock);	//on server's side socket will be closed
    free(cbw);		//but server will handle it correctly
    delete file;	//even while upload transfer
    if (!klass->RemoveDEntry(srvc)) printf("Cannot remove DEntry\n");
    if (!klass->RemoveDownloadEntry(fnam)) printf("Cannot remove download entry!\n");
    return NULL;
   }
 int status = 1;	//status OK initially
 for (off_t i=0;i<f_length;i+=AIO_BUFSIZ)
    {
     //printf("CLOOP - started i = %lld\n", i);
     to               = f_length-i;
     if (to>AIO_BUFSIZ) to = AIO_BUFSIZ;
     off_t total;
     total=0;
     while (total<to)
       {
       nread = read(sock, aiobuff+total, to-total);
       total += nread;
       //printf("r_part; %lld\n", total);
       if (nread <= 0) 
         {  
          log->Debug("Error while executing read\n");
          chdir(wd);
          delete file;
	  delete srvc;
          free(cbw);
          close(sock);
          if (!klass->RemoveDEntry(srvc)) printf("Cannot remove DEntry\n");
          return NULL;
	 }
       }
      //printf("total = %lld, to=%lld\n", total,to);
      assert(total==to);
      
      /*
      //SYNC VERSION to use whe AIO fails...
      total=0;
      while (total<to)
       {
       nwrite = write(fd, aiobuff+total, to-total);
       total += nwrite;
       if (nwrite <= 0) 
         {  
          log->Debug("Error while executing read\n");
          chdir(wd);
          delete file;
          free(cbw);
          close(sock);
          if (!klass->RemoveDEntry(srv)) printf("Cannot remove DEntry\n");
          return NULL;
	 }
       }
      //printf("total = %lld, to = %lld\n", total, to);
      assert(total==to);
      //SYNC VERSION ENDS
      */
      
      //ASYNC VERSION
      cbw->aio_fildes = fd;
      from             = i;
      to               = i+AIO_BUFSIZ;
      if (to>f_length) to = f_length;
      cbw->aio_offset = from;
      cbw->aio_nbytes = to-from;
      cbw->aio_buf    = aiobuff;
      cbw->aio_sigevent.sigev_notify = SIGEV_NONE;
      if (aio_write(cbw)==-1)
        {
         log->Printf("aio_write failed");
         chdir(wd);
         delete file;
	 delete srvc;
         free(cbw);
         close(sock);
         return NULL;
        }
      status = aio_error(cbw);
      if (status == EINPROGRESS)
       {
        if (aio_waitcomplete((struct aiocb**)(&cbw),NULL)==-1)
         {
	  perror("aio_suspend");
          log->Printf("download: aio_suspend failed");
          chdir(wd);
	  delete srvc;
          delete file;
          free(cbw);
          close(sock);
          return NULL;
         }
       }
      else if (status!=0)
       {
        log->Printf("aio_error failed");
        chdir(wd);
        free(cbw);
        close(sock);
        delete file;
	delete srvc;
        return NULL;
       }
      aio_return(cbw);
      /*if (cbw && aio_return(cbw)==-1)
       {
	perror("aio_return");
        log->Printf("download: aio_return failed");
        chdir(wd);
        free(cbw);
        close(sock);
        delete file;
        return NULL;
       }*/
      //ASYNC VERSION ENDS
      //printf("CAIO ok i = %lld\n", i);
      
      //printf("CLOOP\n");
      int32_t ok=1;
      nwrite=write(sock, (void*)(&ok), 4);	//write a download_part confirmation
      if (nwrite != 4) 
        { 
         log->Debug("Client: Error occured!"); 
          chdir(wd);
          delete file;
	  delete srvc;
          free(cbw);
          close(sock);
          if (!klass->RemoveDEntry(srvc)) printf("Cannot remove DEntry\n");
	  return NULL;
	 }
      klass->SetTransferred(fnam, i);
      //printf("CCONFIRM OK\n");
    }
    log->Printf("Transfer of %s:%d:(upload_dir)%s finished.", srvc->GetStringIP(), srvc->GetPort(), fnam);
    if (!klass->RemoveDEntry(srvc)) printf("Cannot remove DEntry\n");
    if (!klass->RemoveDownloadEntry(fnam))
       log->Debug("Error removing %s from download list.", fnam);
    delete file;
    delete srvc;
    chdir(wd);
    free(cbw);
    close(sock);
    //END
   }
 else 
   {
    log->Debug("Transfer failed: Unknown server ansver.");
    close(sock);
    return NULL;
   }
 return NULL;
}

//removes download entry from a 'downloads' list
//first occurence of name is removed
//so if there are more than one download with the same name
//then first will be removed (not correctly maybe)
bool KApp :: RemoveDownloadEntry(char* srv)
{
 assert(srv);
 KList* list = files_found;
 KList* prev = list;
 while (list)
   {
    if (!strcmp(list->GetFileName(), srv))
      {
       if (list == files_found)
         {
          files_found = files_found->Next();
	  list->SetNext(NULL);
	  delete list;
	  list = NULL;
	  goto deleted;
         }
       else
         {
          prev->SetNext(list->Next());
	  list->SetNext(NULL);
	  delete list;
	  list = NULL;
	  goto deleted;
         }
       break;
      }
    prev = list;
    list = list->Next();
   }
 log->Debug("No such element: file_name=%s", srv);
 return false;
 deleted:
 log->Debug("File removed.");
 return true;
}


//will spawn a new thread to download file
//caller will return immadiately and transfer will
//be started asynchronously; data for thread will be passed
//using THreadDataPasser struct
//read TCPTransfer description
void KApp :: AsyncGetFile(KList* file)
{
 printf("Request for: %s\n", file->GetFileName());
 try
   {
    struct ThreadDataPasser* tdp = new ThreadDataPasser();
    if (!tdp) throw new KMemoryException("cannot allocate ThreadDataStruct");
    tdp->klass = this;
    tdp->elem = file;
    pthread_t thr;
    assert(!pthread_create(&thr, NULL, TCPTransfer, tdp));
   }
 catch (KMemoryException* kmex)
  { 
   kmex->PrintType();
   delete kmex;
   abort();
  }
}

//will parse cmd for a filename
//then if number of file is ok AsyncGetFile will be called
void KApp :: GetFile(char* cmd)
{
 assert(cmd);
 int num = -1;
 if (!files_found)
   {
    printf("There is no found files.\n");
    return;
   }
 sscanf(cmd, "get %d", &num);
 mutex->Wait();
 int numl = files_found->GetCount();
 mutex->Signal();
 if (num<0 || num>=numl)
   {
    printf("Bad number, select something from [%d-%d]\n", 0, numl-1);
    return;
   }
 log->Debug("Getting file #%d", num); 
 mutex->Wait();
 KList* lp = files_found;
 int i=0;
 while (lp)
   {
    if (i==num) AsyncGetFile(lp);
    lp = lp->Next();
    i++;
   }
 mutex->Signal();
}

//parses user input for a known command
//if known executes it
void KApp :: DoACommand(char* cmd, char* dcmd)
{
 char word[MAX_LINE_LENGTH+1];
 assert(cmd && dcmd);
 if (strlen(cmd)<3) return;
 if (!strcmp(dcmd, "help"))    Help();
 if (!strcmp(dcmd, "shred"))   Shred();
 if (!strcmp(dcmd, "lactive")) SyncerInfo(0);
 if (!strcmp(dcmd, "lhosts"))  ListKnown();
 if (!strcmp(dcmd, "lfind"))   FilesFoundList();
 if (!strcmp(dcmd, "listd"))   ListDownloads();
 if (!strcmp(dcmd, "listu"))   ListUploads();
 if (!strcmp(dcmd, "info"))    Informations();
 if (!strcmp(dcmd, "lclear")) 
   {
    if (files_found) FreeList(files_found);
    files_found = NULL;
   }
 sscanf(cmd, "%s", word);
 if (!strcmp(word, "find")) FindFile(cmd);
 if (!strcmp(word, "get"))  GetFile(cmd);
}

//creates console for user
//waits 0,05 seconds to servers thread to establish
//then display Console> prompt
//if user quits console using ^D or 'quit'
//it will disable console mode, inform logger
//and run into ServerIdle()
void KApp :: CreateConsole()
{
 if (!console) ServerIdle();
 char command[MAX_LINE_LENGTH+1];
 char dcommand[MAX_LINE_LENGTH+1];
 int nread;
 files_found = NULL;
 usleep(50000);
 printf("Console, type quit  or ^D to stop\n");
 printf("NOTICE: quit will terminate client only\n");
 printf("If You want to stop server, try ^C\n");
 printf("If You don't know whats happening try 'help'\n");
 while (true)
   {
    strcpy(command, "");
    strcpy(dcommand, "");
    printf("Console> ");
    fflush(stdout);
    nread=read(0, (void*)command, MAX_LINE_LENGTH);
    if (nread<0)
      {
       log->Warning("Console read failed.\n");
       break;
      }
    if (nread==0) 
      {
       printf("CTRL+D pressed, exiting Console.\n");
       break;
      }
    if (!strcmp(command, "")) continue;
    command[nread-1] = 0;
    strcpy(dcommand, command);
    for (unsigned int i=0;i<strlen(dcommand);i++) 
	    if (dcommand[i]>='A' && dcommand[i]<='Z') dcommand[i]+=0x20;
    if (!strcmp(dcommand, "quit")) 
      {
       printf("quit command exiting Console.\n");
       break;
      }
    DoACommand(command, dcommand);
   }
 console = false;
 log->SetConsoleMode(console);
 printf("Console mode exited.\n");
 ServerIdle();
}

//is console mode active
bool KApp :: InConsoleMode() const
{
 return console;
}

//Main routine called by user using object->Run
//after object->GetOptions()
//implements entire program logic
void KApp :: Run()
{
 CheckDirectories();		//are download and upload directories ok?
 Informations();		//display informations gathered until now
 StartLogger();			//start logging procedure, enable SysLog
 if (daemon) Daemonize();	//start daemon mode if asked for 
 				//if daemon mode then from there server will be
				//silent and all info will be written to SysLog
 CreateLists();			//create list of known hosts, read hosts from file
 CreateMutex();			//create mutual exclusion object
 CreateConnectionSyncer();	//create syncer thread, will keep connections alive
 CreateUDPListener();		//create listener for search requests in new thread
 CreateTCPListener();		//create TCP listener in new thread (handles uploads)
 CreateConsole();		//create console if asked or go server IDLE
}

//display server's informations
void KApp :: Informations() const
{
 printf("Running server:                   YES\n");
 if (daemon)
 printf("Running server in daemon mode:    YES\n");
 else
 printf("Running server in daemon mode:    NO\n");
 if (console)
 printf("Running client console:           YES\n");
 else
 printf("Running client console:           NO\n");
 printf("Port Number:                      %d\n", port);
 printf("Desired Number of active servers: %d\n", numactive);
 printf("Download directory:               %s\n", download);
 printf("Upload directory:                 %s\n", upload);
 printf("List of known hosts in file:      %s\n", hostsfile);
}

//Thats all
