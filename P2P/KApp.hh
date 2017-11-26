#ifndef __HAVE_KAPP_H__
#define __HAVE_KAPP_H__
#include "KSysLog.hh"
#include "KList.hh"
#include "KMutex.hh"
//MAIN Server/Client class
class KApp
{
 public:
	 KApp();
	 ~KApp();
	 void GetOptions(int, char**);
	 void Run();
 	 void Informations() const;
	 KList* GetKnownServers() const;
	 KList* GetActiveServers() const;
	 KSysLog* GetLog() const;
	 int GetNumConnections() const;
	 int GetDesiredConnections() const;
	 int GetPort() const;
	 void TryConnect(KList*);
	 void SetUDPSocket(int);
	 int GetUDPSocket() const;
	 void SetTCPSocket(int);
	 int GetTCPSocket() const;
	 void CheckIfKnown(struct sockaddr_in*, int);
	 KMutex* GetMutex() const;
	 bool IncCons(KList*);
	 bool DecCons(KList*);
	 void CreateUDPPackage(int32_t, char*&, int&);
	 //void CreateTCPPackage(int32_t, char*&, int&);
	 void SyncerInfo(int = 1) const;
	 bool InConsoleMode() const;
	 char* GetUploadDir() const;
	 char* GetDownloadDir() const;
	 bool AddDownloadEntry(KList*);
	 int GenerateError(int, char*) const;
	 int AIORead(int, char*);
	 void ConstructDownloadFileName(char*, char*) const;
	 char* GetWD() const;
	 bool RemoveDownloadEntry(char*);
	 bool InsertDEntry(KList*);
	 bool InsertUEntry(char*);
	 bool RemoveDEntry(KList*);
	 bool RemoveUEntry(char*);
	 void SetTransferred(char*, off_t) const;
 private:
	 void CheckDirectories();
	 void Daemonize();
	 void StartLogger();
	 void CreateLists();
	 void AddServer(char*);
	 char* GetHostIP(char*) const;
	 void FreeList(KList*);
	 void CreateConnectionSyncer();
	 void CreateConsole();
	 void CreateMutex();
	 void CreateUDPListener();
	 void CreateTCPListener();
	 void ServerIdle();
	 void DescribeServer(KList*) const;
	 void DoACommand(char*, char*);
	 void ListKnown() const;
	 void FilesFoundList() const;
	 void FindFile(char*);
	 void AsyncSearch(KList*, char*);
	 void AsyncGetFile(KList*);
	 void GetFile(char*);
	 void ListDownloads() const;
	 void ListUploads() const;
	 void WriteKnownHostsFile() const;
 private:
 	int      port;		//port number
	bool     console;	//consloe active?
 	int      numactive;	//how many active servers should be
 	char*    download;	//download directory
 	char*    upload;	//upload directory
	bool     daemon;	//daemon mode active?
	char*    hostsfile;	//name of file with hosts list
	KSysLog* log;		//system log (/var/log/messages probably)
	KList*   known_servers;	//list of known servers
	KList*   active_servers;//list of currently active servers
	KList*   files_found;	//list of files to download
	KList*   downloads;	//list of current downloads
	KList*   uploads;	//list of current uploads
	KMutex*  mutex;		//mutual exclusion object
	pthread_t preserver;	//thread of connection syncer (keeps desired number of connections alive, retrys connections)
	pthread_t udp_listener;	//listens for search requests
	pthread_t tcp_listener;	//listens for download requests
	int      num_conns;	//number of active servers at the moment
	int      udp_socket;	//socket of udp_listener
	int	 tcp_socket;	//socket of tcp_listener
	int 	 tcp_port;	//port number
	char*    start_dir;	//server's starting WD
};
#endif
