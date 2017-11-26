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
#ifndef __HAVE_CLASS_SERVERMAIN__
#define __HAVE_CLASS_SERVERMAIN__

#include "XSemaphore.hh"
#include "XShM.hh"
#include "XHttpParser.hh"
#include "Common.hh"

class XServerMain
{
 public:
 	XServerMain(const int, const int);
 	XServerMain(const int);
 	XServerMain();
 	~XServerMain();
	void Run();
	void SetMaxClients(const int);
	void SetClientPort(const int);
	void SetAdminPort(const int);
	void SetDaemon(const int);
	void SetLogFile(const char*);
	void SetLogFilePointer(const FILE*);
	XSemaphore* GetMutex() const;
	XShM* GetSHM() const;
	char* GetLogFileName() const;
	void SetChildState();
	int IsChild() const;
	void CloseLogFile();
	int GetSocketByPID(const pid_t) const;
 private:
	void CheckDaemon() const;
	void CreateMutex();
	void CreateSHM();
	void CreateSockets();
	void Listen();
	void SetDefaults();
	void CreateWriterThread();
	void AcceptClient();
	void AcceptAdmin();
        void ProcessClient(struct ClientInfo* client);
	void DestroyAllClients();
	void SuspendAll();
	void ResumeAll();
	void ResetServer();
	int AnswerRequest(const int, const char*, const int, struct ClientInfo*, int&);
 private:
	int client_portnum;
	int admin_portnum;
	int max_clients;
	int daemon;
	XSemaphore* mutex;
	XShM* shm;
	char* log_file;
	pid_t log_thread;
	FILE* log;
	int client_socket;
	int admin_socket;
	int parent;
};
#endif
