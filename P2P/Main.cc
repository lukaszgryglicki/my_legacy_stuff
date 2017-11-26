#include "Common.hh"
#include "KApp.hh"
#include "KMemoryException.hh"
#include <signal.h>

KApp* instance=NULL;	//needed when signal processing
KMutex* mut=NULL;	//to exclude multiple threads processing one signal
int exited = 0;		//set to 1 if one of thread is exiting
			//and others shouldn't mess up more
//displays help
void ShortHelp()
{
 printf("P2P Client/Server\tOptions available:\n");
 printf("\t-p\tport number [DEFAULT 7782]i {1-65535}\n");
 printf("\t-c\tenable console [RUN client also, DEFAULT no]\n");
 printf("\t-n\tnumber of active servers [DEFAULT 10] {1-32}\n");
 printf("\t-d\tdownload dir [DEFAULT ./] { no \"\" no NULL}\n");
 printf("\t-u\tupload dir [DEFAULT ./] { no \"\" no NULL}\n");
 printf("\t-m\tenable daemon mode for server [DEFAULT no]\n");
 printf("\t-h\tfile with hosts list [DEFAULT known_hosts] { no \"\" no NULL}\n");
}

//handled signals are:
//SIGINT, SIGABRT, SIGPIPE, SIGALRM
//SIGINT,SIGABRT:
//if some thread exiting then exit immadiately
//thread exiting means all resouces already freed
//if not exiting, block mutex free all resources
//then exit cleanly
//SIGALRM, SIGPIPE:
//printf information about timeout if not in ConsoleMode
//SIGPIPE will terminate some sys_function in server
//it will detect and handle it
void AbortHandler(int signo)
{
 if (exited) 
   {
    printf("Already freed resources, doing fast exit.\n");
    exit(3);
   }
 if (signo==SIGABRT) 
   {
    printf("Assertions failed, ABORT caught - cleaning up...\n");
    mut->Wait();
    delete instance;
    instance = NULL;
    mut->Signal();
    printf("Exit.\n");
    exited = 1;
    exit(1);
   }
 if (signo==SIGINT)
   {
    printf("Got interrupt - going down...\n");
    mut->Wait();
    delete instance;
    instance = NULL;
    mut->Signal();
    printf("Exit.\n");
    exited = 1;
    exit(2);
   }
 if (signo==SIGPIPE) 
   {
    if (instance && !instance->InConsoleMode()) 
	    printf("Signal PIPE caught...\n");
   }
 if (signo==SIGALRM) 
   {
    if (instance && !instance->InConsoleMode()) 
	    printf("Operation timeout...\n");
   }
}

//setups signals handlers
//handled signals are:
//SIGINT, SIGABRT, SIGPIPE, SIGALRM
void SetupAbortHandler()
{
 static struct sigaction act;
 act.sa_handler = AbortHandler;
 sigfillset(&(act.sa_mask));
 sigaction(SIGABRT, &act, NULL);
 sigaction(SIGINT, &act, NULL);
 sigaction(SIGALRM, &act, NULL);
 sigaction(SIGPIPE, &act, NULL);
 exited = 0;
}

//no comment for this function ;-)
int main(int argc, char** argv)
{
 ShortHelp();
 KApp* app=NULL;
 instance = NULL;
 mut = NULL;
 try 
   {
    app = new KApp();
    if (!app) throw new KMemoryException("cannot allocate KApp class");
    mut = new KMutex();
    if (!mut) throw new KMemoryException("cannot allocate KMutex class");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }			//till now we've created desired objects
 instance = app;	//gloal instance backup for signal processing
 SetupAbortHandler();
 app->GetOptions(argc, argv);
 app->Run();
 mut->Wait();
 delete app;
 instance = NULL;
 app = NULL;
 mut->Signal();
 return 0;		//return 0 to UNIX OS
}

//and it is all

