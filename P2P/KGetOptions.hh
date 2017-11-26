#ifndef __HAVE_GETOPTIONS_H__
#define __HAVE_GETOPTIONS_H__
class KGetOptions	//reads command line arguments
{
 public:
 	KGetOptions(int,char**);
 	~KGetOptions();

	int GetPort() const;
	bool GetConsole() const;
	int GetNumActive() const;
	char* GetDownloadDir()  const;
	char* GetUploadDir()  const;
	bool GetDaemon() const;
	char* GetHostsFile()  const;
 private:
	void RunGetOpt();
 private:
	int numopts;		//num options from main
	char** optarray;	//array of options
	
 	int port;		//values to pass to KApp class
	bool console;
 	int numactive;
 	char* download;
 	char* upload;
	bool daemon;
	char* hostsfile;
};
#endif
