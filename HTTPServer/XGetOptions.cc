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
#include "XGetOptions.hh"
#include "Common.hh"
#include "Definitions.hh"

////
//	construct class, assert all failures
//	set default values to be chooses
//	if no command line is given for them
////
XGetOptions :: XGetOptions(int n, char** arr)
{
 assert(n>=1 && arr);
 numopts  = n;
 optarray = arr;
 maxclients = DEFAULT_MAX_CLIENTS;
 clientport = 80;
 adminport  = 81;
 daemon = 0;
 log_file = new char[MAX_PATH_LENGTH+1];
 assert(log_file);
 strcpy(log_file, DEFAULT_LOG_FILE);
 RunGetOpt();
 assert(clientport>0 && clientport<=MAX_PORT);
 assert(adminport>0  && adminport<=MAX_PORT);
 assert(maxclients>0 && maxclients<=MAX_CLIENTS);
}

////
//	destroy class, free memory
////
XGetOptions :: ~XGetOptions()
{
 if (log_file) delete [] log_file;
 log_file = NULL;
}

////
//	parse command line options
//	unknown switches are ignored
////
void XGetOptions :: RunGetOpt()
{
 char u;
 while ((u = getopt(numopts,optarray,"c:a:n:l:d"))!=-1)
 {
  switch (u)
   {
    case 'c': if (strlen(optarg)<12) clientport =  atoi(optarg); break;
    case 'd': daemon = 1; break;
    case 'a': if (strlen(optarg)<12) adminport  =  atoi(optarg); break;
    case 'n': if (strlen(optarg)<12) maxclients =  atoi(optarg); break;
    case 'l': if (strlen(optarg)<MAX_PATH_LENGTH) strncpy(log_file, optarg, MAX_PATH_LENGTH); break;
    default: printf("Unrecognized option: '-%c'\n", u); return;
   }
 }
}

////
//	return selected max clients
//	default is 8
////
int XGetOptions :: GetMaxClients() const
{
 return maxclients;
}
////
//	return selected port for client applications
//	default is 80 - WWW port 
//	WARNING: note that you will
//	need root previledges to run server
//	on this port
////
int XGetOptions :: GetClientPort() const
{
 return clientport;
}
////
//	return selected port for admin
//	default 81, read WARNING above
////
int XGetOptions :: GetAdminPort() const
{
 return adminport;
}
////
//	return daemon mode
////
int XGetOptions :: GetDaemon() const
{
 return daemon;
}
////
//	return log file name
//	default Server.LOG
////
char* XGetOptions :: GetLogFile() const
{
 return log_file;
}
