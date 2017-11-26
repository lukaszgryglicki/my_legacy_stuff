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
#include "Definitions.hh"
#include <time.h>
////
//	Converts sockaddr_in to Human-readable string	
////
char* StringAddress(struct sockaddr_in* addr)
{
 if (!addr) return "(null)";
 return inet_ntoa(addr->sin_addr);
}

////
//	if -DDEBUG compilation flag is set, works like a printf
//	else do nothing
////
int Debug(const char* fmt, ...)
{
 va_list lst;
 va_start(lst,fmt);
#ifdef DEBUG
 printf("Debug: \t");
 vprintf(fmt,lst);
 fflush(stdout);
#endif
 va_end(lst);
 return 0;
}

////
//	save Client info 'clinet' in file, 
//	these are statistics, from, how much bytes etc
//	extracted from SHM
////
void WriteClientInfo(int num, FILE* file, struct ClientInfo* client)
{
 assert(file);
 assert(client);
 fprintf(file, "Client #%d, bytes transferred: %lld, pid: %d, "
	       "address: %s, socket: %d\n",
		 num+1, client->nbytes, (int)client->pid, 
		 StringAddress((&client->addr)), client->sock);
}

////
//	write all statistics, total and about each client
//	all information is get from SHM
//	also current time is written
////
void WriteStats(XServerMain* server, FILE* file)
{
 int num_clients;
 time_t tim;
 assert(server);
 assert(file);
 num_clients = (server->GetSHM())->GetClientsCount();
 fprintf(file,"===================================\n");
 tim = time(NULL);
 fprintf(file, "%s", ctime(&tim));
 fprintf(file,"Total bytes transferred: %lld\n", 
		 (server->GetSHM())->GetGlobalBytes());
 fprintf(file,"Currently clients: %d\n", num_clients);
 for (int i=0;i<num_clients;i++)
    WriteClientInfo(i, file, (server->GetSHM())->GetClient(i));
 fprintf(file,"===================================\n");
}

////
//	write info to file every LOGGER_INTERVAL seconds
//	block SHM by MUTEX before write and SIGNAL after
////
void Logger(void* klass)
{
 FILE* file;
 assert(klass);
 XServerMain* server = (XServerMain*)klass;
 server->SetChildState();
 file = fopen(server->GetLogFileName(), "w");
 if (!file) 
   { 
    printf("Cannot write to LOG file: %s, but continuing without LOG...\n"
		    ,server->GetLogFileName());
    return;
   }
 server->SetLogFilePointer(file);
 if (server->GetMutex()==NULL) 
   { 
    printf("Server MUTEX is USELESS!\n"); 
    return; 
   }
 if (server->GetSHM()==NULL)   
   { 
    printf("Server SHM is USELESS!\n"); 
    return; 
   }
 while (1)
   {
    sleep(LOGGER_INTERVAL);
    (server->GetMutex())->Wait();
    WriteStats(server, file);
    (server->GetMutex())->Signal();
    sync();
   }
 return;
}

