#define _WITH_GETLINE
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <libgadu.h>
#include <time.h>
#include <termios.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <set>

int  glob_service_uin = 0;
char* glob_service_pass;
int glob_warning_uin = 0;
char* gpass = NULL;
gg_session* gggs = NULL;
int gid = 0;
std::set<long> authuids;
char gcmd[2000];
char mhost[256];
char gstat[512];

void log(int lev, const char* fmt, ...)
{
	FILE* f;
	va_list lst;
	static struct tm *tms;
	time_t t;
	char tstr[128];
	int pid;
	int uid;

#ifndef MDBG
	if (lev > 0) return;
#endif

	time(&t);
	tms = localtime(&t);

	pid = getpid();
	uid = getuid();

	sprintf(tstr, "/tmp/ggssh_%d.log", uid);
	f = fopen(tstr, "a+");
	if (!f) return;

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	fprintf(f, "%6d %s: ", pid, tstr);

	va_start(lst,fmt);
	vfprintf(f, fmt,lst);
	va_end(lst);

	fprintf(f, "\n");

#ifdef MDBG
	if (lev <= 1)
#else
	if (lev < 0)
#endif
	{
		fprintf(stdout, "%6d %s: ", pid, tstr);

		va_start(lst,fmt);
		vfprintf(stdout, fmt,lst);
		va_end(lst);

		fprintf(stdout, "\n");
	}

	fflush(f);
	fclose(f);
}

int my_recv(int sock, char* bstr, int n)
{
	int nrecv = 0;

	alarm(30);

	while (nrecv < n)
	{
	    	nrecv += recv(sock, (void*)(&bstr[nrecv]), n - nrecv, 0);
	}

	alarm(0);

	return nrecv;
}

int my_send(int sock, char* bstr, int n)
{
	int nsend = 0;

	alarm(30);

	while (nsend < n)
	{
		nsend += send(sock, (void*)(&bstr[nsend]), n - nsend, 0);
	}
	
	alarm(0);

	return nsend;
}

void make_ping_process(gg_session* ggs)
{
    	log(1, "Pinger forking out");
	int pid = fork();

	if (pid > 0) return;
	else if (pid < 0)
	{
	    log(-1, "Error forking");
	    return;
	}

	// pid = 0: child
	log(1, "Pinger forked out");

	log(0, "GGping PID: %d", getpid());
	int err = 0;

	while(1)
	{
	    if (!ggs)
	    {
		log(-1, "Abort ping due to NULL pointer");
		exit(3);
	    }

	    log(1, "ping");
	    if (gg_ping(ggs) < 0) 
	    {
		err ++;
	    	log(0, "GG ping error: %d", err);
	    }

	    if (err > 10) break;
	    sleep(40);
	}

	log(-1, "GGping exiting: too many errors");
	exit(2);
}

int ggmsg(gg_session* ggs, int uin, char* m, int l)
{
	unsigned char* msg;
	int t, t2;
	int r = 0;

	if (uin <= 0)
	{
	    log(0, "Attempt to send to 0 UIN,  skipped");
	    return 1;
	}

	if (!m || !ggs || l < 0)
	{
	    log(-1, "Abort ggmsg s=%p, ggs=%p, l=%d", m, ggs, l);
	}

	if (l <= 0) l = strlen( m );
	log(0, "Send to UIN %d, message length: %d", uin, l);

	if (l < 1900)
	{
		msg = (unsigned char*)malloc(l+1);
		strcpy((char*)msg, m);
		msg[l] = 0;
		for (int x=0;x<l;x++) if (!msg[x]) msg[x] = ' ';

		log(1, (char*)msg);
		r = gg_send_message(ggs, GG_CLASS_CHAT, uin, msg);
		if (uin != glob_warning_uin) gg_send_message(ggs, GG_CLASS_CHAT, glob_warning_uin, msg);

		free( msg );
	}
	else
	{
		msg = (unsigned char*)malloc(1901);

		for (int x=0;x<l;x+=1900)
		{
		    t = x + 1900;
		    if (t > l) t = l;
		    t2 = t - x;
		    log(1, "<%d - %d>: %d", x, t, t2);

		    strncpy((char*)msg, &m[x], t2);
		    msg[t2] = 0;
		    for (int x=0;x<t2;x++) if (!msg[x]) msg[x] = ' ';

		    log(1, (char*)msg);
		    r = gg_send_message(ggs, GG_CLASS_CHAT, uin, msg);
		    if (uin != glob_warning_uin) gg_send_message(ggs, GG_CLASS_CHAT, glob_warning_uin, msg);
		}

		free( msg );
	}

	return r;
}


char* readfile(FILE* fp, int& l)
{
    	char* s;

	if (!fp)
	{
	    log(-1, "Error readfile abort fp is null");
	    return NULL;
	}

    	fseek(fp, 0, SEEK_END);
    	l = ftell(fp);
    	fseek(fp, 0, SEEK_SET);

    	if (!l) 
	{
	    log(1, "Empty result");
	    return NULL;
	}

    	s = (char*)malloc(l+1);
    	if (!s) 
	{
	    log(-1, "Readfile malloc error");
	    return NULL;
	}

    	fread((void*)s, l, 1, fp);
    	s[l] = 0;

    	return s;
}

int ggremote(gg_session* ggs, int id, char* scmd)
{
    	FILE* fp;
    	char* cmd;
	char* s;
	int l;

	if (id <= 0)
	{
	    log(0, "Attempt to send to 0 UIN, skipped");
	    return 1;
	}

	if (!ggs || !scmd)
	{
	    log(-1, "ggremote abort ggs=%p scmd=%p", ggs, scmd);
	    return -5;
	}

	//printf("Before alarm\n");
	log(0, "UIN %d execute now \"%s\"", id, scmd);
	gggs = ggs;
	gid = id;
	int uid = getuid();
	char fname[64];

    	cmd = (char*)malloc(strlen(scmd) + 128);
	if (!cmd)
	{
	    log(-1, "Abort due to malloc error");
	    return -6;
	}

	if (id != glob_warning_uin)
	{
	    sprintf(cmd, "UIN %d: %s", id, scmd);
	    if (ggmsg(ggs, glob_warning_uin, cmd, 0) < 0) log(-1, "Error echoying %s to supervisor", cmd);
	}

	strcpy(gcmd, scmd);

	sprintf(cmd, "rm -f /tmp/ggssh_%d.dat", uid);
	system( cmd );

    	sprintf(cmd, "%s 1>/tmp/ggssh_%d.dat 2>/tmp/ggssh_%d.dat", scmd, uid, uid);

	alarm(30);
    	system(cmd);
	alarm(0);

	log(0, "Success executing");
	//printf("After alarm\n");
    
	sprintf(fname, "/tmp/ggssh_%d.dat", uid);
	fp = fopen(fname, "r");

	if (!fp) 
	{
		log(0, "\"%s\" - command not found", scmd);
		sprintf(cmd, "%s\nNot found\n", scmd);
		if (ggmsg(ggs, id, cmd, 0) < 0) 
		{
		    	log(-1, "Error sending \"%s\" to UIN %d", cmd, id);
		    	free( cmd );
		    	return -2;
		}

		free( cmd );
		return 0;
	}

	s = readfile(fp, l);
	fclose( fp );

	if (!s) 
	{
	    	log(0, "\"%s\" - produced no terminal output", scmd);
		sprintf(cmd, "%s\nNo terminal output\n", scmd);
		if (ggmsg(ggs, id, cmd, 0) < 0) 
		{
		    	log(-1, "Error sending \"%s\" to UIN %d", cmd, id);
		    	free( cmd );
		    	return -3;
		}

		free( cmd );
		return 0;
	}

	free( cmd );

	log(0, "\"%s\" - produced %d bytes of output", scmd, l);
	if (ggmsg(ggs, id, s, l) < 0) 
	{
		log(-1, "Error sending results of \"%s\" to UIN %d", scmd, id);
	    	free( s );
	    	return -4;
	}

	log(1, "Results sucessfully sent");
	free( s );

	return 1;
}


void parse_msg(gg_session* s, gg_event* e)
{
    gg_event_msg* msg;
    bool auth;

    if (!s || !e)
    {
	log(-1, "Abort parse_msg s=%p e=%p", s, e);
	return;
    }

    msg = (gg_event_msg*)(&e->event);

    if (!msg)
    {
	log(-1, "Abort parse_msg msg=%p", msg);
	return;
    }

    int id = msg->sender;
    char pass[400];
    char cmd[400];
    char m[2000];

    if (authuids.find(id) != authuids.end()) auth = true;
    else auth = false;

    log(0, "Checking authority for UIN: %d --> %d", id, auth);

    strcpy(m, (char*)msg->message);

    if (!strcmp(m, "gghelp")) 
    {
	sprintf(cmd, "echo gghelp ggwho ggquit ggkill ggkick ggpass ggstat");
	log(0, cmd );

	if (ggremote(s, id, cmd) < 0) log(-1, "Error sending logout info message to UIN %d", id);
	return;
    }
    else if (!strcmp(m, "ggwho")) 
    {
	int l = authuids.size();
	char* tmp = (char*)malloc( 100 + 10 * l);

	sprintf(tmp, "echo Currently %d authorized ", l);
	if (auth)
	{
		for (std::set<long>::iterator i=authuids.begin();i!=authuids.end();++i)
		{
	    		sprintf(cmd, "%ld ", *i);
	    		strcat(tmp, cmd); 
		}
	}

	log(0, tmp );

	if (ggremote(s, id, tmp) < 0) log(-1, "Error sending list info message to UIN %d", id);
	free( tmp );

	return;
    }
    else if (!strcmp(m, "ggkill")) 
    {
	if (auth && id == glob_warning_uin)
	{
	    	log(-1, "COMMITING SUICIDE");
		sprintf(cmd, "killall ggssh ggsshd");
	}
	else
	{
	    	sprintf(cmd, "echo Must be authorized as supervisor to do this");
	}
	log(0, cmd );

	if (ggremote(s, id, cmd) < 0) log(-1, "Error sending KILL info message to UIN %d", id);
	return;
    }
    else if (!strcmp(m, "ggkick")) 
    {
	if (auth && id == glob_warning_uin)
	{
	    	log(0, "Kick out all");
		authuids.clear();
		authuids.insert( glob_warning_uin );
		sprintf(cmd, "echo Done");
	}
	else
	{
	    	sprintf(cmd, "echo Must be authorized as supervisor to do this");
	}
	log(0, cmd );

	if (ggremote(s, id, cmd) < 0) log(-1, "Error sending kick out info message to UIN %d", id);
	return;
    }
    else if (!strcmp(m, "ggquit")) 
    {
	if (auth)
	{
		auth = false;
		authuids.erase( id );

		sprintf(cmd, "echo UIN %d logout, currently %d allowed", id, authuids.size());
	}
	else
	{
	    	sprintf(cmd, "echo Must be authorized to do this");
	}
	log(0, cmd );

	if (ggremote(s, id, cmd) < 0) log(-1, "Error sending logout info message to UIN %d", id);
	return;
    }
    else if (!strcmp(m, "ggstat")) 
    {
	if (auth && id == glob_warning_uin)
	{
	    	log(0, "Recover status");
		gg_change_status_descr(s, GG_STATUS_AVAIL_DESCR, gstat);
		sprintf(cmd, "echo Done");
	}
	else
	{
	    	sprintf(cmd, "echo Must be authorized as supervisor to do this");
	}
	log(0, cmd );

	if (ggremote(s, id, cmd) < 0) log(-1, "Error sending status change info message to UIN %d", id);
	return;
    }
    else if (sscanf(m, "ggpass %s", cmd) == 1)
    {
	if (auth && id == glob_warning_uin && strlen(cmd) > 5)
	{
	    	log(0, "Changing password");

		authuids.clear();
		authuids.insert( glob_warning_uin );
		strcpy(gpass, cmd);

		sprintf(cmd, "echo Done");
	}
	else
	{
	    	sprintf(cmd, "echo Must be authorized as supervisor to do this and password must be strong enough");
	}
	log(0, cmd );

	if (ggremote(s, id, cmd) < 0) log(-1, "Error sending password change info message to UIN %d", id);
	return;
    }


    if (!auth)
    {
	log(0, "Not autorized, checking password, UIN %d", id);
	strcpy(pass, gpass);

	//log(1, "Check: '%s' == '%s'\n", m, pass);

    	if (strcmp(m, pass)) 
	{ 
		sprintf(cmd, "echo Password required for UIN %d, just enter password as message or try gghelp, now allowed %d UINs", id, authuids.size());
		log(0, "UIN %d bad password, access denied", id);
		if (ggremote(s, id, cmd) < 0) log(-1, "Error sending access denied message for UIN %d", id);
		return;
	}

	log(0, "UIN %d successfully authorised", id);
	auth = true;
	authuids.insert( id );
	sprintf(cmd, "echo Access granted for UIN %d, currently %d allowed", id, authuids.size());

	if (ggremote(s, id, cmd) < 0) log(0, "Error sending access granted info to UIN %d", id);

	sprintf(cmd, "echo Authorised new user UIN: %d", id);
	if (ggremote(s, glob_warning_uin, cmd) < 0) log(-1, "Error sending new access info to supervisor");

	return;
    }

//#ifndef MDBG
    log(1, "Forking before executing command: \"%s\"", m);
    //printf("Before fork\n");
    int pid = fork();

    if (pid > 0) 
    {
	log(1, "Parent returns\n");
	return;
    }
    else if (pid < 0)
    {
	log(-1, "Error forking\n");
	return;
    }

    // pid == 0: child
    log(1, "Forked, starting gg execute in new process for UIN %d", id);
//#endif
    //printf("After fork\n");
    if (ggremote( s, id, m ) < 0) 
    {
	log(-1, "Error executing '%s' from UIN: %d\n", m, id);
    }

    log(1, "Executor process ends");
    exit(0);
}

void catch_int(int signo)
{
    	char cmd[2500];

	sprintf(cmd, "echo PID: %d: UIN: %d, Signal %d, current cmd ''%s''", getpid(), gid, signo, gcmd);
	log(0, "Got signal %d, UIN %d, last cmd \"%s\"", signo, gid, gcmd);

	if (signo < 1 || signo > 32)
	{
	    log(-1, "Broken signal no. %d", signo);
	    exit(3);
	}

	if (ggremote(gggs, gid, cmd) < 0) log(-1, "Error sending signal %d info to UIN %d", signo, gid);

	if (signo == SIGHUP || signo == SIGCHLD)
	{
	    log(0, "Not deadly signal, trying to continue");
	}
	else
	{
	    log(0, "Exit due to deadly signal");

	    /*
	    sleep(5);
	    gg_logoff( gggs );
	    gg_free_session( gggs );
	    sleep(3);
	    */

	    exit(1);
	}
}

void setup_signals()
{
	static struct sigaction chldact;
	static struct sigaction intact;

	log(0, "Setting signal handlers");

	chldact.sa_handler = SIG_IGN;
#ifndef MWIN
	chldact.sa_flags = SA_NOCLDWAIT;
#endif
	sigfillset(&(chldact.sa_mask));
	sigaction(SIGCHLD, &chldact, NULL);

	intact.sa_handler = catch_int;
	sigfillset(&(intact.sa_mask));
	sigaction(SIGINT, &intact, NULL);
	sigaction(SIGTERM, &intact, NULL);
	sigaction(SIGPIPE, &intact, NULL);
	sigaction(SIGALRM, &intact, NULL);

	sigaction(SIGHUP, &intact, NULL);
	siginterrupt(SIGHUP, 0);

	log(0, "Signal handlers installed");
}

void my_getline(char* str, size_t l)
{
 	struct termios st, end;
	int fno;

#ifndef MWIN
	fno = fileno(stdin);
#else
	fno = 0;
#endif
 	tcgetattr(fno,&st);

 	end = st;
 	//end.c_lflag &= ~ICANON;
 	end.c_lflag &= ~ECHO;

	if (tcsetattr(fno,TCSAFLUSH,&end)) 
	{
	    log(-1, "Error seting terminal");
	    exit(1);
	}

	getline(&str, &l, stdin);

 	tcsetattr(fno,TCSAFLUSH,&st);

	l = strlen(str);
	l --;

	if (l <= 0) 
	{
	    strcpy(str, "");
	    return;
	}

	str[l] = 0;
	printf("\n");
}

int load_from_file()
{
    FILE* fp;
    char* hdir;
    char fn[1024];

    hdir = getenv("HOME");
    if (!hdir)
    {
	log(0, "Current user is homeless, cannot fetch config, falling back to stdin");
	return 0;
    }

    sprintf(fn, "%s/.ggssh/keys", hdir);
    log(0, "Config file: %s", fn);

    fp = fopen(fn, "r+");

    if (!fp)
    {
	log(0, "No %s file, fetching keys from stdin", fn);
	return 0;
    }

    if (fscanf(fp, "%d %d\n", &glob_service_uin, &glob_warning_uin) != 2)
    {
	log(0, "Cannot log keys from ~/.ggssh/keys format is: service_uin warning_uin");
	fclose(fp);
	return 0;
    }

    log(0, "~/.ggssh/keys loaded");

    fclose(fp);
    return 1;
}

void help()
{
    	log(1, "This GGssh server needs keys (service_uin, supervisor_uin)");
	log(1, "They can be loaded from configuration file ~/.ggssh/keys or");
	log(1, "Recieved from user's terminal input");
	log(1, "Format of key file is: service_uin supervisor_uin");
	log(1, "Password are checked always on terminal");
	log(1, "Server logs are in /tmp/ggssh*.log, debug version is ggsshd");

	printf("If you want help examine logs: /tmp/ggssh*.log, keys file: %s/.ggssh/keys or try debug version: ggsshd\n", getenv("HOME"));
}

bool get_from_pwdmgmt()
{
	int port;
	int sock,n;
	struct sockaddr_in server;
	char pass_ip[16];

	port = 9002;
	strcpy(pass_ip, "127.0.0.1");

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(pass_ip);

	log(0, "Connecting to password server '%s', at port %d", pass_ip, port);

	if (server.sin_addr.s_addr == (in_addr_t)(-1)) 
	{
	    log(0, "Bad IP address '%s'", pass_ip);
	    return false;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)		
	{ 
		log(0, strerror( errno ));
		log(0, "Create socket failed");
		return false;
	}

	if (connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1)
	{ 
		log(0, strerror( errno ));
		log(0, "connect to %s:%d failed",pass_ip, port); 
		return false;
	}

	log(1, "Connected, socket: %d", sock);

	char buff[5];
	int l;

	l = 1;	// GGssh server password
	memcpy((void*)buff, (void*)&l, 4);
	n = my_send(sock, buff, 4);
	if (n < 4)
	{
		log(0, strerror( errno ));
		close(sock);
		log(0, "my_send failed: %d/4", n);
		return false;
	}

	// Get password length
	n = my_recv(sock, buff, 4);
	if (n < 4)
	{
		log(0, strerror( errno ));
		close(sock);
		log(0, "my_recv failed: %d/1", n);
		return false;
	}

	memcpy((void*)&l, (void*)buff, 4);
	//printf("Password length: %d\n", l);

	if (l < 6 || l > 2048)
	{
		close(sock);
		log(0, "Error: bad password length: %d", l);
		return false;
	}

	gpass = (char*)malloc( l + 1 );

	// Get password
	n = my_recv(sock, gpass, l);
	if (n < l)
	{
		log(0, strerror( errno ));
		close(sock);
		log(0, "my_recv failed: %d/%d", n, l);
		return false;
	}

	gpass[l] = 0;
	for (int i=0;i<l;i++) gpass[i] = ~gpass[i];

	//printf("Password: '%s'\n", gpass);

	l = 2;	// Service UIN password
	memcpy((void*)buff, (void*)&l, 4);
	n = my_send(sock, buff, 4);
	if (n < 4)
	{
		log(0, strerror( errno ));
		close(sock);
		log(0, "my_send failed: %d/4", n);
		return false;
	}

	// Get password length
	n = my_recv(sock, buff, 4);
	if (n < 4)
	{
		log(0, strerror( errno ));
		close(sock);
		log(0, "my_recv failed: %d/1", n);
		return false;
	}

	memcpy((void*)&l, (void*)buff, 4);
	//printf("Password length: %d\n", l);

	if (l < 1 || l > 2048)
	{
		close(sock);
		log(0, "Error: bad password length: %d", l);
		return false;
	}

	glob_service_pass = (char*)malloc( l + 1 );

	// Get password
	n = my_recv(sock, glob_service_pass, l);
	if (n < l)
	{
		log(0, strerror( errno ));
		close(sock);
		log(0, "my_recv failed: %d/%d", n, l);
		return false;
	}

	glob_service_pass[l] = 0;
	for (int i=0;i<l;i++) glob_service_pass[i] = ~glob_service_pass[i];

	//printf("Password: '%s'\n", glob_service_pass);

	l = 0;	// Quit request
	memcpy((void*)buff, (void*)&l, 4);
	n = my_send(sock, buff, 4);
	if (n < 4)
	{
		log(0, strerror( errno ));
		close(sock);
		log(0, "my_send failed: %d/4", n);
		return false;
	}

	close(sock);
	return true;
}

void getpwd()
{
	char temp[256];

	help();

	if (load_from_file()) 
	{
	    log(0, "UINs loaded from configuration file");
	}
	else
	{
	    log(0, "UINs not loaded, must be set manually");
	}

	bool pwd_got = false;

	if (pwd_got = get_from_pwdmgmt()) 
	{
	    log(0, "Passwords successfully recieved from PwdMgmt server");
	}

	if (!pwd_got)
	{
		gpass = (char*)malloc(513);
		printf("GG ssh server password: ");
		my_getline(gpass, (size_t)512);
		if (strlen(gpass) < 5)
		{
		    log(-1, "Password must be at least 5 letters/digits");
		    exit(1);
		}
	}

	if (glob_service_uin <= 0)
	{
		printf("GG ssh service uin: ");
		my_getline(temp, (size_t)255);
		if (sscanf(temp, "%d", &glob_service_uin) != 1)
		{
		    log(-1, "Bad service UIN '%s'", temp);
		}
	}

	if (glob_warning_uin <= 0)
	{
		printf("GG ssh warning/supervisor uin: ");
		my_getline(temp, (size_t)255);
		if (sscanf(temp, "%d", &glob_warning_uin) != 1)
		{
		    log(-1, "Bad warning UIN '%s'", temp);
		}
	}

	if (!pwd_got)
	{
		glob_service_pass = (char*)malloc(513);
		printf("GG service UIN password: ");
		my_getline(glob_service_pass, (size_t)512);
	}
}

void ggssh()
{
	struct gg_session *ggs;
	struct gg_event *gge;

	getpwd();
#ifndef MDBG
	log(1, "Forking out");
	int pid = fork();

	if (pid > 0) return;
	else if (pid < 0)
	{
	    log(-1, "Error main forking\n");
	    return;
	}
	log(1, "Forked out");
	gg_debug_level = 0;
#else
	gg_debug_level = 255;
#endif

	//log(1, "Password: '%s'", gpass);

	setup_signals();
	strcpy(gcmd, "");

	//close(0);
	//close(1);

	gg_login_params gglp;

	memset((void*)(&gglp), 0, sizeof(gglp));
	gethostname(mhost, 250);

	//From justa account
	gglp.uin = glob_service_uin;
	gglp.password = (char*)malloc(512);
	gglp.status_descr = (char*)malloc(512);
	gglp.async = 0;
	//gglp.status = GG_STATUS_INVISIBLE;
	gglp.status = GG_STATUS_AVAIL_DESCR;
	strcpy(gglp.password, glob_service_pass);
	sprintf(gglp.status_descr, "GGssh@%s version %s %s UID %d(%s) by MorgothV8: lukaszgryglicki@o2.pl", mhost, __DATE__, __TIME__, (int)getuid(), getlogin());
	log(-1, gglp.status_descr);

	strcpy(gstat, gglp.status_descr);

	while (1)
	{
	    	log(0, "Attempt GGlogin to UIN %d", gglp.uin);

		if (!(ggs = gg_login(&gglp)))
		{
	   		log(-1, "GGlogin error\n");
			sleep(60);
			log(-1, "Slept one minute, now next login attempt");
			continue;
		}

		log(1, "GG notify list");
		gg_notify(ggs, NULL, 0);

		log(0, "Start GG pinger");
		make_ping_process( ggs );

		//printf("GGssh process PID: %d\n", getpid());

		log(1, "Start GG watching");
		while ((gge = gg_watch_fd( ggs )))
		{
		    	log(1, "Gg event type: %d", gge->type);
		    	if (gge->type == GG_EVENT_MSG) 
		    	{
			    	log(0, "Processing GG message");
				parse_msg( ggs, gge );
				//printf("Parsed\n");
		    		gg_event_free( gge );
	    	    	}
			else if (gge->type == GG_EVENT_ACK) 
		    	{
			    	log(1, "GG server commits message delivery");
		    		gg_event_free( gge );
	    	    	}
			else if (gge->type == GG_EVENT_NONE) 
		    	{
			    	log(1, "...");
		    		gg_event_free( gge );
	    	    	}
	    		else if (gge->type == GG_EVENT_DISCONNECT)
	    		{
				//FIXME: test it!
				log(-1, "DISCONNECT detected: reconnecting...");

	    			gg_event_free( gge );
				gg_logoff(ggs);
				gg_free_session( ggs );
				break;
	    		}
	    		else
	    		{
			    	log(1, "Other event skipped");
	    			gg_event_free( gge );
	    		}
		}

		log(-1, "Reconnect attempt\n");
	}

	log(-1, "Error: this should not be reached");
}

int main(int lb, char** par)
{
    	ggssh();
    	return 0;
}

