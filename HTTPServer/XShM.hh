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
#ifndef __HAVE_CLASS_SHM__
#define __HAVE_CLASS_SHM__
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct ClientInfo
{
 sockaddr_in addr;
 pid_t pid;
 int64_t nbytes;
 int sock;
};

struct ControlBlock
{
 int64_t nbytes;
 int num_clients;
};

class XShM
{
 public:
	 XShM(const key_t, const int);
	 ~XShM();
	 void SetGlobalBytes(const int64_t);
	 int64_t GetGlobalBytes() const;
	 void AppendGlobalBytes(const int64_t);
	 struct ClientInfo* GetClient(const int) const;
	 void SetClientBytes(const int, const int64_t);
	 int64_t GetClientBytes(const int) const;
	 void AppendClientBytes(const int, const int64_t);
	 pid_t GetClientPID(const int) const;
	 struct sockaddr_in* GetClientAddr(const int) const;
	 int AddClient(const struct ClientInfo*);
	 int RemoveClient(const pid_t);
	 int GetMaxClients() const;
	 int GetClientsCount() const;
	 void SetClientsCount(const int);
	 int GetClientSocket(const int) const;
	 void SetClientSocket(const int, const int);
	 int GetSocketByPID(const pid_t) const;
         int GetClientIdxByPID(const pid_t pid) const;
 private:
	 int  ClientHasPID(const struct ClientInfo*, pid_t) const;
	 void SetClientAddr(const int, const struct sockaddr_in*);
	 void SetClientPID(const int, const pid_t);
	 void SetClient(const struct ClientInfo*, const int);
	 void ZeroClient(const int);
	 void MoveClient(const int, const int);
	 int SHMIDFromKey(const key_t) const;
	 void SetDefaults();
 private:
	 int shmid;
	 key_t shmkey;
	 int max_clients;
	 void* mem;
	 int size;
};
#endif

