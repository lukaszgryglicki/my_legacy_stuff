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
#ifndef __HAVE_CLASS_GETOPTIONS__
#define __HAVE_CLASS_GETOPTIONS__
class XGetOptions
{
 public:
 	XGetOptions(int,char**);
 	~XGetOptions();
	int GetMaxClients() const;
	int GetClientPort() const;
	int GetAdminPort()  const;
	int GetDaemon()  const;
	char* GetLogFile()  const;
 private:
	void RunGetOpt();
 private:
	int numopts;
	char** optarray;
 	int maxclients;
 	int clientport;
 	int adminport;
	int daemon;
	char* log_file;
};
#endif
