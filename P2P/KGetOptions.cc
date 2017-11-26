#include "KGetOptions.hh"
#include "KException.hh"
#include "KMemoryException.hh"
#include "Common.hh"

//create getoptions class
//fills with default values first
KGetOptions :: KGetOptions(int n, char** arr)
{
 assert(n>=1 && arr);
 numopts  = n;
 optarray = arr;
 port = DEFAULT_PORT;
 console   = false;
 numactive = DEFAULT_N;
 daemon    = false;
 try
   {
    download = new char[PATH_MAX+1];
    if (!download) throw new KMemoryException("cannot allocate memory for download");
    upload = new char[PATH_MAX+1];
    if (!upload) throw new KMemoryException("cannot allocate memory for upload");
    hostsfile = new char[PATH_MAX+1];
    if (!hostsfile) throw new KMemoryException("cannot allocate memory for hostsfile");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    printf("Exceptions occured, cannot continue.\n");
    abort();
   }  
 strcpy(download, DEFAULT_DOWNLOAD_DIR);
 strcpy(upload, DEFAULT_UPLOAD_DIR);
 strcpy(hostsfile, DEFAULT_HOSTS_FILE);
 RunGetOpt();
 assert(port>0 && port<=MAX_PORT);
 assert(numactive>0);
 assert(strcmp(download, ""));
 assert(strcmp(upload, ""));
 assert(strcmp(hostsfile, ""));
}

//deletes get options class
KGetOptions :: ~KGetOptions()
{
 if (hostsfile) delete [] hostsfile;
 if (download) delete [] download;
 if (upload) delete [] upload;
 hostsfile = NULL;
 download = NULL;
 upload = NULL;
}

//overwrites default values set
//if such option get from command-line
//if options is not known, server aborts
void KGetOptions :: RunGetOpt()
{
 char u;
 assert(numopts && optarray);
 while ((u = getopt(numopts,optarray,"p:n:d:u:h:cm"))!=-1)
 {
  assert(u != -1);
  switch (u)
   {
    case 'p': if (strlen(optarg)<12) port =  atoi(optarg); break;
    case 'c': console = true; break;
    case 'n': if (strlen(optarg)<12) numactive =  atoi(optarg); break;
    case 'd': if (strlen(optarg)<PATH_MAX) strncpy(download, optarg, PATH_MAX); break;
    case 'u': if (strlen(optarg)<PATH_MAX) strncpy(upload, optarg, PATH_MAX); break;
    case 'm': daemon = true; break;
    case 'h': if (strlen(optarg)<PATH_MAX) strncpy(hostsfile, optarg, PATH_MAX); break;
    default: printf("Unrecognized option: '-%c'\n", u); abort();
   }
 }
}
//these methods below are standard get methods
//dome of them returns warning
//if they think it is neccessary

int KGetOptions :: GetPort() const
{
 return port;
}

int KGetOptions :: GetNumActive() const
{
 return numactive;
}

bool KGetOptions :: GetConsole() const
{
 return console;
}

bool KGetOptions :: GetDaemon() const
{
 return daemon;
}

char* KGetOptions :: GetDownloadDir() const
{
 if (!download) printf("WARNING: download is NULL: %s %d\n", HERE);
 return download;
}

char* KGetOptions :: GetUploadDir() const
{
 if (!download) printf("WARNING: upload is NULL: %s %d\n", HERE);
 return upload;
}

char* KGetOptions :: GetHostsFile() const
{
 if (!hostsfile) printf("WARNING: hostsfile is NULL: %s %d\n", HERE);
 return hostsfile;
}

