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
#include "XShM.hh"
#include "Common.hh"
#include "Definitions.hh"
////
//	set default values, zeroes entire SHM
////
void XShM :: SetDefaults()
{
 assert(mem);
 for (int i=0;i<size;i++) ((char*)mem)[i] = 0;
}

////
//	creates SHM pointer, uses UNIQUE key
//	read comment on XSemaphore.cc: class constructor
//	to get more informations about UNIQUE key
//	SHM contains struct ControlBlock
//	and N * (struct ClientInfo)
//	just lays byte after byte
//	is such key exists removes and recreates
//	will not confuse other servers, read:
//	comment on XSemaphore.cc: class constructor
//	all data stored in SHM are common for
//	all children and accessed synchronously
//	using MUTEX (semaphore)
//	all operations below: gets, appends, sets
//	just runs on RAW memory (almosta all of them)
//	memcpy, memmove, memset etc.
////
XShM :: XShM(const key_t shmk, const int numcli)
{
 int id;
 assert(numcli>0 && numcli<MAX_CLIENTS);
 assert(shmk>0 && shmk<=MAX_KEY_T);
 id = SHMIDFromKey(shmk);
 if (id>=0) shmctl(id,IPC_RMID,NULL);
 size = sizeof(struct ControlBlock) + numcli*sizeof(struct ClientInfo);
 max_clients = numcli;
 if ((id=shmget(shmk,size,IPC_CREAT|IPC_EXCL|0660))==-1)
   {
    printf("FATAL: cannot create IPC: Shared Memory.\n");
    exit(4);
   }
 assert((mem=shmat(id,0,0))!=(void*)(-1));
 shmid = id;
 shmkey = shmk;
 SetDefaults();
}

////
//	destroys IPC object, disconnects SHM segment
////
XShM :: ~XShM()
{
 assert(shmdt((char*)mem)!=-1);
 shmctl(shmid,IPC_RMID,NULL);
 mem = 0;
}

////
//	internal: returns SHM ID for given KEY
////
int XShM :: SHMIDFromKey(key_t shmkey) const
{
 return shmget(shmkey,1,0660);
}

////
//	Set Global byes send to INT64 value
////
void XShM :: SetGlobalBytes(const int64_t bytes)
{
 assert(bytes>=0);
 memcpy(mem, &bytes, sizeof(int64_t));
}

////
//	returns INT64:global bytes written
////
int64_t XShM :: GetGlobalBytes() const
{
 int64_t ret64;
 memcpy(&ret64, mem, sizeof(int64_t));
 return ret64;
}

////
//	appends to global bytes written
////
void XShM :: AppendGlobalBytes(int64_t bytes)
{
 int64_t ret64;
 assert(bytes>=0);
 memcpy(&ret64, mem, sizeof(int64_t));
 ret64 += bytes;
 memcpy(mem, &ret64, sizeof(int64_t));
}

////
//	get client index = num
////
struct ClientInfo* XShM :: GetClient(const int num) const
{
 assert(num>=0 && num<max_clients);
 return (struct ClientInfo*)
	 ((char*)mem+sizeof(struct ControlBlock)+(num*sizeof(struct ClientInfo)));
}

////
//	set client, just memcopy struct
////
void XShM :: SetClient(const struct ClientInfo* client , const int num)
{
 assert(num>=0 && num<max_clients);
 assert(client);
 memcpy(((char*)mem+sizeof(struct ControlBlock)+(num*sizeof(struct ClientInfo)))
		 ,client, sizeof(struct ClientInfo));
}

////
//	clear clients, zero its bytes
////
void XShM :: ZeroClient(const int num)
{
 assert(num>=0 && num<max_clients);
 memset(((char*)mem+sizeof(struct ControlBlock)+(num*sizeof(struct ClientInfo)))
		 ,0, sizeof(struct ClientInfo));

}

////
//	set client number of bytes written
////
void XShM :: SetClientBytes(const int num, const int64_t bytes)
{
 assert(num>=0 && num<max_clients);
 assert(bytes>=0);
 memcpy((char*)mem+sizeof(struct ControlBlock)+(num*sizeof(struct ClientInfo))
		  +sizeof(struct sockaddr_in)+sizeof(pid_t)
		 ,&bytes, sizeof(int64_t));
}

////
//	append client written bytes
////
void XShM :: AppendClientBytes(const int num, const int64_t bytes)
{
 int64_t res64;
 assert(num>=0 && num<max_clients);
 assert(bytes>=0);
 memcpy(&res64, (char*)mem+sizeof(struct ControlBlock)
		 +(num*sizeof(struct ClientInfo))
		  +sizeof(struct sockaddr_in)+sizeof(pid_t)
		 ,sizeof(int64_t));
 res64 += bytes;
 memcpy((char*)mem+sizeof(struct ControlBlock)+
		 (num*sizeof(struct ClientInfo))
		  +sizeof(struct sockaddr_in)+sizeof(pid_t)
		 ,&res64, sizeof(int64_t));
}

////
//	get client bytes written count
////
int64_t XShM :: GetClientBytes(const int num) const
{
 int64_t res64;
 assert(num>=0 && num<max_clients);
 memcpy(&res64, (char*)mem+sizeof(struct ControlBlock)+
		  (num*sizeof(struct ClientInfo))
		  +sizeof(struct sockaddr_in)+sizeof(pid_t)
		 ,sizeof(int64_t));
 return res64;
}

////
//	get client pid
////
pid_t XShM :: GetClientPID(const int num) const
{
 pid_t res;
 assert(num>=0 && num<max_clients);
 memcpy(&res, (char*)mem+sizeof(struct ControlBlock)
		 +(num*sizeof(struct ClientInfo))
		  +sizeof(struct sockaddr_in)
		 ,sizeof(pid_t));
 return res;
}

////
//	set client pid
////
void XShM :: SetClientPID(const int num, const pid_t pid)
{
 assert(num>=0 && num<max_clients);
 assert(pid>=0);
 memcpy((char*)mem+sizeof(struct ControlBlock)
		 +(num*sizeof(struct ClientInfo))
		  +sizeof(struct sockaddr_in)
		 ,&pid, sizeof(pid_t));
}

////
//	get client address
////
struct sockaddr_in* XShM :: GetClientAddr(const int num) const
{
 assert(num>=0 && num<max_clients);
 return (struct sockaddr_in*)((char*)mem
		 +sizeof(struct ControlBlock)
		 +(num*sizeof(struct ClientInfo)));
}

////
//	set client address
////
void XShM :: SetClientAddr(const int num, const struct sockaddr_in* addr)
{
 assert(num>=0 && num<max_clients);
 assert(addr);
 memcpy((char*)mem+sizeof(struct ControlBlock)
		 +(num*sizeof(struct ClientInfo)), (void*)addr
		 ,sizeof(struct sockaddr_in));
}

////
//	get clients socket
////
int XShM :: GetClientSocket(const int num) const
{
 int res;
 assert(num>=0 && num<max_clients);
 memcpy(&res, (char*)mem+sizeof(struct ControlBlock)+
		  (num*sizeof(struct ClientInfo))
		  +sizeof(struct sockaddr_in)+sizeof(pid_t)
		  +sizeof(int64_t)
		 ,sizeof(int));
 return res;

}

////
//	set clients socket
////
void XShM :: SetClientSocket(const int num, const int s)
{
 assert(num>=0 && num<max_clients);
 assert(s>=0);
 memcpy((char*)mem+sizeof(struct ControlBlock)
		 +(num*sizeof(struct ClientInfo))
		  +sizeof(struct sockaddr_in)+sizeof(pid_t)
		  +sizeof(int64_t)
		 ,&s, sizeof(int));

}

////
//	locate client by PID (if exists) then 
//	return its socket, or -1 if client don't exists
////
int XShM :: GetSocketByPID(const pid_t pid) const
{
 assert(pid>0);
 int num;
 num = GetClientsCount();
 for (int i=0;i<num;i++)
    if (GetClientPID(i)==pid) return GetClientSocket(i);
 return -1;
}

////
//	locate client by pid, and return its
//	index, or -1 if don't exists
////
int XShM :: GetClientIdxByPID(const pid_t pid) const
{
 assert(pid>0);
 int num;
 num = GetClientsCount();
 for (int i=0;i<num;i++)
    if (GetClientPID(i)==pid) return i;
 return -1;
}

////
//	add client to next free place in shm
//	if no place is free return FAILED
//	if OK return SUCCESS
////
int XShM :: AddClient(const struct ClientInfo* client)
{
 int num;
 assert(client);
 num = GetClientsCount();
 if (num>=max_clients) return FAILED;
 SetClient(client, num);
 num++;
 SetClientsCount(num);
 return SUCCESS;
}

////
//	does client pointed by *client has pid given?
//	used to locate clients
////
int XShM :: ClientHasPID(const struct ClientInfo* client, pid_t pid) const
{
 assert(client);
 return (client->pid == pid);
}

////
//	moves client data from 'from' to 'to'
////
void XShM :: MoveClient(const int from, const int to)
{
 assert(to>=0   && to<=max_clients);
 assert(from>=0 && from<=max_clients);
 memmove((char*)mem+sizeof(struct ControlBlock)
	 +(to*sizeof(struct ClientInfo)),
         (char*)mem+sizeof(struct ControlBlock)
	 +(from*sizeof(struct ClientInfo)),
	 sizeof(struct ClientInfo));
}

////
//	removes client, if client is last in list then
//	returns else locates last client and moves it to
//	'hole' left by removed client
//	last client after move is zeroed
//	if no such PID (removing by pid)
//	memory is not touched and FAILED is returned
//	if ok SUCCESS is returned
//	see Definitions.hh for SUCCESS & FAILED
////
int XShM :: RemoveClient(const pid_t pid)
{
 assert(pid >= 0);
 int num = GetClientsCount();
 if (num<=0) return FAILED;
 for (int i=0;i<num;i++) if (ClientHasPID(GetClient(i), pid))
  {
   ZeroClient(i);
   if (i!=(num-1)) 
     {
      MoveClient(num-1, i);
      ZeroClient(num-1);
     }
   SetClientsCount(num-1);
   return SUCCESS;
  }
 return FAILED;
}

////
//	return maximum allowed clients number
//	SHM is allocated for max_client
////
int XShM :: GetMaxClients() const
{
 return max_clients;
}

////
//	return number of currently registered clients
//	from ControlBlock of SHM
////
int XShM :: GetClientsCount() const
{
 int ret;
 memcpy(&ret, (char*)mem+sizeof(int64_t), sizeof(int));
 return ret;
}

////
//	set curent clients number to SHM ConstrolBlock
////
void XShM :: SetClientsCount(const int arg)
{
 assert(arg>=0 && arg<=max_clients);
 memcpy((char*)mem+sizeof(int64_t), &arg, sizeof(int));
}

