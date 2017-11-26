#include "Common.hh"
#include "KList.hh"
#include "KMemoryException.hh"

//sets default values to class
KList :: KList()
{
 srv_port = 0;
 srv_addr = 0;
 srv_name = NULL;
 srv_ip   = NULL;
 srv_descr= NULL;
 file_name= NULL;
 srv_sock = 0;
 srv_thread = 0;
 file_length= (off_t)(0);
 file_trans = (off_t)0;
 next = NULL;
}

//sets default values to variables
//srv_port and srv_addr are copied from addr
//tries to get host name to srv_name
//using gethostbyaddr
KList :: KList(struct sockaddr_in* addr)
{
 assert(addr);
 srv_name = NULL;
 srv_ip   = NULL;
 srv_descr= NULL;
 file_name= NULL;
 srv_port = addr->sin_port;
 srv_addr = addr->sin_addr.s_addr;
 try
   {
    srv_ip = new char[16];
    if (!srv_ip) throw new KMemoryException("cannot allocate srv_ip");
    srv_name = new char[MAX_LINE_LENGTH+1];
    if (!srv_name) throw new KMemoryException("cannot allocate srv_name");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 struct hostent* host;
 host = gethostbyaddr((const char*)(&addr->sin_addr), sizeof(struct in_addr), AF_INET);
 strcpy(srv_name, host->h_name);
 strcpy(srv_ip, inet_ntoa(addr->sin_addr));
 srv_sock = -1;
 file_length = (off_t)(0);
 file_trans  = (off_t)(0);
 srv_thread = 0;
 next = NULL;
}

//sets default values
//and file_name copies from fn
KList :: KList(char* fn)
{
 assert(fn);
 srv_port = 0;
 srv_addr = 0;
 file_name= NULL;
 try
   {
    file_name = new char[strlen(fn)+1];
    if (!file_name) throw new KMemoryException("cannot allocate file_name");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 srv_name = NULL;
 srv_ip = NULL;
 srv_descr = NULL;
 strcpy(file_name, fn);
 srv_sock = -1;
 file_length = (off_t)(0);
 file_trans  = (off_t)(0);
 srv_thread = 0;
 next = NULL;
}

//copies all values from ptr to class
//these strings which are NULLs are not copied
KList :: KList(KList* ptr)
{
 assert(ptr);
 srv_name = NULL;
 srv_ip   = NULL;
 srv_descr= NULL;
 file_name= NULL;
 srv_port = ptr->srv_port;
 srv_addr = ptr->srv_addr;
 try
   {
    srv_name = new char[strlen(ptr->srv_name)+1];
    if (!srv_name) throw new KMemoryException("cannot allocate srv_name");
    srv_ip = new char[strlen(ptr->srv_ip)+1];
    if (!srv_ip) throw new KMemoryException("cannot allocate srv_ip");
    if (ptr->srv_descr)
      {
       srv_descr = new char[strlen(ptr->srv_descr)+1];
       if (!srv_descr) throw new KMemoryException("cannot allocate srv_descr");
      }
    else srv_descr = NULL;
    if (ptr->file_name)
      {
       file_name = new char[strlen(ptr->file_name)+1];
       if (!file_name) throw new KMemoryException("cannot allocate file_name");
      }
    else file_name = NULL;
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 strcpy(srv_name, ptr->srv_name);
 strcpy(srv_ip, ptr->srv_ip);
 if (ptr->srv_descr) strcpy(srv_descr, ptr->srv_descr);
 if (ptr->file_name) strcpy(file_name, ptr->file_name);
 srv_sock = ptr->srv_sock;
 file_length = ptr->file_length;
 file_trans  = ptr->file_trans;
 srv_thread = 0;
 next = NULL;
}

//destroys class
KList :: ~KList() 
{
 if (srv_name) delete srv_name;
 srv_name = NULL;
 if (srv_ip) delete srv_ip;
 srv_ip = NULL;
 if (srv_descr) delete srv_descr;
 srv_descr = NULL;
 if (file_name) delete file_name;
 file_name = NULL;
}

//standard get methods
//some prints warnings if they thinks it is neccessary

int KList :: GetPort() const
{
 if (srv_port<=0 || srv_port>=MAX_PORT) printf("WARNING: bad port vaule: %d, HERE %s %d\n", srv_port, HERE);
 return srv_port;
}

int KList :: GetSocket() const
{
 if (srv_sock<=0) printf("WARNING: bad socket vaule: %d, HERE %s %d\n", srv_sock, HERE);
 return srv_sock;
}

off_t KList :: GetFileLength() const
{
 if (file_length<=0) printf("WARNING: bad file_length vaule: %lld, HERE %s %d\n", file_length, HERE);
 return file_length;
}

off_t KList :: GetTransferred() const
{
 if (file_trans<0) printf("WARNING: bad file_trans vaule: %lld, HERE %s %d\n", file_trans, HERE);
 return file_trans;
}

in_addr_t KList :: GetAddr() const
{
 return srv_addr;
}

char* KList :: GetName() const
{
 if (srv_name==NULL) printf("WARNING: srv_name is NULL, %s %d\n", HERE);
 return srv_name;
}

char* KList :: GetDescr() const
{
 return srv_descr;
}

char* KList :: GetFileName() const
{
 return file_name;
}

char* KList :: GetStringIP() const
{
 if (srv_ip==NULL) printf("WARNING: srv_ip is NULL, %s %d\n", HERE);
 return srv_ip;
}
//end of standard get methods
	 
//returns pointer to next KList
KList* KList :: Next() const
{
 return next;
}

//standard set methods

void KList :: SetPort(int port)
{
 assert(port>0 && port<MAX_PORT);
 srv_port = port;
}

void KList :: SetSocket(int sock)
{
 assert(sock>0);
 srv_sock = sock;
}

void KList :: SetFileLength(off_t flen)
{
 assert(flen>=0);
 file_length = flen;
}

void KList :: SetTransferred(off_t trans)
{
 assert(trans>=0);
 file_trans = trans;
}

void KList :: SetAddr(in_addr_t in_addr)
{
 srv_addr = in_addr;
}

//set file name, copies from fnam
//KMemoryException can be thrown
void KList :: SetFileName(char* fnam)
{
 assert(fnam);
 assert(strlen(fnam)<MAX_SERVER_NAME);
 if (file_name) delete [] file_name;
 try
   {
    file_name = new char[strlen(fnam)+1];
    if (!file_name) throw new KMemoryException("cannot allocate file_name");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 strcpy(file_name, fnam);
}

//sets description
void KList :: SetDescr(char* desc)
{
 assert(desc);
 assert(strlen(desc)<MAX_SERVER_NAME);
 if (srv_descr) delete [] srv_descr;
 try
   {
    srv_descr = new char[strlen(desc)+1];
    if (!srv_descr) throw new KMemoryException("cannot allocate srv_descr");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 strcpy(srv_descr, desc);
}
	 

//sets name
void KList :: SetName(char* name)
{
 assert(name);
 assert(strlen(name)<MAX_SERVER_NAME);
 if (srv_name) delete [] srv_name;
 try
   {
    srv_name = new char[strlen(name)+1];
    if (!srv_name) throw new KMemoryException("cannot allocate srv_name");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 strcpy(srv_name, name);
}


//sets server's IP string
void KList :: SetStringIP(char* name)
{
 assert(name);
 assert(strlen(name)<MAX_SERVER_NAME);
 if (srv_ip) delete [] srv_ip;
 try
   {
    srv_ip = new char[strlen(name)+1];
    if (!srv_ip) throw new KMemoryException("cannot allocate srv_ip");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 strcpy(srv_ip, name);
}

//sets pointer to next KList
//used to construct KList
void KList :: SetNext(KList* ptr)
{
 next = ptr;
}

//returns number of elements on KList
//begining from '*this'
int KList :: GetCount()
{
 KList* ptr;
 int i = 0;
 ptr = this;
 while (ptr)
   {
    i++;
    ptr = ptr->Next();
   }
 return i;
}

//spawns new thread running given 'routine'
//with addictional pointer 'ptr'
//called by KApp::TryConnect if connection ok
//called function will run 'KeepAliveNotifier'
//to ping given server (stored in this class)
//every PING_INTERVAL seconds to check if it is alive
void KList :: Run(void* (*routine)(void*), void* ptr)
{
 assert(!pthread_create(&srv_thread, NULL, routine, ptr));
}


