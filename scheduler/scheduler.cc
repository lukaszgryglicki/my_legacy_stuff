#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

class Scheduler
{
	public:
	    	Scheduler();
		~Scheduler();

		bool OpenLog();
		int Log(const char*, ...);
		bool IsRunning();
		bool MarkRunning( bool );
		void Start();

	private:

		FILE* log;
		int pid, uid;
		char logname[128];
};

Scheduler :: Scheduler()
{
    	log = NULL;
	strcpy(logname, "");
}

Scheduler :: ~Scheduler()
{
	if (!MarkRunning( false ))
	{
		Log("Error during running markup");
	}

	if (log)
	{
		fclose( log );
		log = NULL;
	}
}

int Scheduler :: Log(const char* fmt, ...)
{
	va_list lst;
	static struct tm *tms;
	time_t t;
	char tstr[128];
	int r;

	time(&t);
	tms = localtime(&t);

	log = fopen(logname, "a+");

	if (!log) return -1;

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	fprintf(log, "%6d %s: ", pid, tstr);

	va_start(lst, fmt);
	r = vfprintf(log, fmt, lst);
	vprintf(fmt, lst);
	va_end(lst);

	fprintf(log, "\n");
	printf("\n");

/*	fflush(log);*/
	fclose(log);

	return r;
}

bool Scheduler :: OpenLog()
{
	pid = getpid();
	uid = getuid();

	sprintf(logname, "/tmp/scheduler_%d_%d.log", uid, pid);

	log = fopen(logname, "a+");
	if (!log)
	{
		return false;
	}

	fclose( log );

	this->Log("Scheduler started.");

	return true;
}

bool Scheduler :: IsRunning()
{
	char s[128];
	FILE* f;
	int rpid;

	strcpy(s, "/tmp/scheduler.pid");

	f = fopen(s, "r+");

	if (!f) 
	{
		perror( strerror( errno ) );
	    	Log("No PID file: %s, assuming no scheduler is running", s);
		return false;
	}

	if (fscanf(f, "%d\n", &rpid) != 1) rpid = -1;

	Log("Scheduler is running with PID: %d, kill it and start again, if you are sure it is not running, then remove PID lock file: %s", s);

	fclose( f );

	return true;
}

bool Scheduler :: MarkRunning( bool run )
{
	char s[128];

	Log("MARK RUNNING: %d", run);
	strcpy(s, "/tmp/scheduler.pid");

	if ( run )
	{
		FILE* f;
		f = fopen( s, "w+");
		if (!f) 
		{
			Log("Cannot create PID file: %s", s);
			return false;
		}

		Log("Created PID file: %s", s);
		fprintf(f, "%d\n", pid);

		fclose( f );
	}
	else
	{
		if (!unlink( s )) 
		{
			Log("Cannot remove PID file: %s", s);
			return false;
		}

		Log("Removed PID file: %s", s);
	}

	return true;
}


void Scheduler :: Start()
{
	Log("Beggining main loop");

	while (true)
	{
	    Log("Processing events...");

	    sleep( 1 );
	}
}

static Scheduler sched;

void daemonize()
{
 	if (fork()) exit(2);
/* 	close(0);*/
/* 	close(1);*/
/* 	close(2);*/
}

void open_log()
{
	if (!sched.OpenLog()) exit(3);

	if (sched.IsRunning()) 
	{
		sched.Log("Already running scheduler, exiting");
		exit(4);
	}
	else sched.Log("No schedulers are running, starting new one");

	if (!sched.MarkRunning( true ))
	{
		sched.Log("Error marking running PID, exiting");
		exit(5);
	}
	else sched.Log("Marked start of new scheduler");

	sched.Log("Scheduler started");
}

void read_config()
{

	sched.Log("Configuration loaded");
}

void main_loop()
{
	sched.Start();
}

int main()
{
	daemonize();
	open_log();
	read_config();
	main_loop();
	// not reached
	exit(1);
}

