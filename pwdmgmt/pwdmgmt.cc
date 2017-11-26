#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

static int pidlog = 1;

void log(const char* fmt, ...)
{
	FILE* f;
	va_list lst;
	static struct tm *tms;
	time_t t;
	char tstr[128];
	int pid;
	int uid;

	time(&t);
	tms = localtime(&t);

	pid = getpid();
	uid = getuid();

	if (pidlog)
	{
		sprintf(tstr, "/tmp/pwdmgmt_%d_%d.log", uid, pid);
		f = fopen(tstr, "a+");
	}
	else
	{
		sprintf(tstr, "/tmp/pwdmgmt_%d.log", uid);
		f = fopen(tstr, "a+");
	}
	if (!f) return;

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	fprintf(f, "%6d %s: ", pid, tstr);

	va_start(lst,fmt);
	vfprintf(f, fmt,lst);
	va_end(lst);

	fprintf(f, "\n");

#ifdef MDBG
	fprintf(stdout, "%6d %s: ", pid, tstr);

	va_start(lst,fmt);
	vfprintf(stdout, fmt,lst);
	va_end(lst);

	fprintf(stdout, "\n");
#endif

	fflush(f);
	fclose(f);
}

void error(const char* fmt, ...)
{
	FILE* f;
	va_list lst;
	static struct tm *tms;
	time_t t;
	char tstr[128];
	int pid;
	int uid;

	time(&t);
	tms = localtime(&t);

	pid = getpid();
	uid = getuid();

	if (pidlog)
	{
		sprintf(tstr, "/tmp/pwdmgmt_%d_%d.log", uid, pid);
		f = fopen(tstr, "a+");
	}
	else
	{
		sprintf(tstr, "/tmp/pwdmgmt_%d.log", uid);
		f = fopen(tstr, "a+");
	}
	if (!f) return;

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	fprintf(f, "%6d %s: ", pid, tstr);

	va_start(lst,fmt);
	vfprintf(f, fmt,lst);
	va_end(lst);

	fprintf(f, "\n");

	fflush(f);
	fclose(f);

	va_start(lst,fmt);
	vfprintf(stdout, fmt,lst);
	va_end(lst);

	fprintf(stdout, "\n");
	fflush(stdout);

	fprintf(stdout, "%6d %s: ", pid, tstr);

	va_start(lst,fmt);
	vfprintf(stdout, fmt,lst);
	va_end(lst);

	fprintf(stdout, "\n");

	log("Exiting due to error");

	exit(1);
}

void catch_int(int signo)
{
	log("Interrupt nr. %d", signo);

	if (signo == SIGALRM)
	{
	    	log("Warning: Network timeout");
	    	log("Current operation timeout");
	}

	log("Exiting...");
	exit(2);
}

void setup_signals()
{
	static struct sigaction chldact;
	static struct sigaction intact;

	log("Setting up signals handlers...");

	chldact.sa_handler = SIG_IGN;
	chldact.sa_flags = SA_NOCLDWAIT;
	sigfillset(&(chldact.sa_mask));
	sigaction(SIGCHLD, &chldact, NULL);

	intact.sa_handler = catch_int;
	sigfillset(&(intact.sa_mask));
	sigaction(SIGINT, &intact, NULL);
	sigaction(SIGTERM, &intact, NULL);
	sigaction(SIGPIPE, &intact, NULL);
	sigaction(SIGALRM, &intact, NULL);

	log("Handlers installed.");
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
	
void get_password(int pass_num, int& pass_len, char*& pass_val)
{
    int idx, offset1, offset2;
    pass_len = 0;
    pass_val = NULL;

    if (pass_num == 1)	// GGssh service login password
    {
	pass_val = (char*)malloc( 11 + (rand() % 10));
	pass_len = 10;
	idx = 0;
	offset1 = '6';
	offset2 = -6;

	for (int i=0;i<10;i++)
	{
		pass_val[idx] = (char)offset1 + (char)offset2;

		switch (i)
		{
		    case 0:
			offset1 ++;
			offset2 --;
			break;
		    case 1:
			offset1 += 3;
			offset2 -= 3;
			break;
		    case 2:
			offset1 += 2;
			offset2 -= 2;
			break;
		    case 3:
			offset2 += 4;
			break;
		    case 4:
			offset1 += 2;
			offset2 -= 3;
			break;
		    case 5:
			offset1 += 2;
			offset2 -= 3;
			break;
		    case 6:
			offset1 ++;
			offset2 += 2;
			break;
		    case 7:
			offset1 += 2;
			offset2 -= 6;
			break;
		    case 8:
			offset1 ++;
			offset2 ++;
			break;

		}
		idx ++;
	}
	pass_val[pass_len] = 0;
    }
    else if (pass_num == 2)	// GGssh service UIN 36226227 password
    {
	pass_val = (char*)malloc( 11 + (rand() % 10));
	pass_len = 10;
	idx = 0;
	offset1 = 'h';
	offset2 = -3;

	for (int i=0;i<10;i++)
	{
		pass_val[idx] = (char)offset1 + (char)offset2;

		switch (i)
		{
		    case 0:
			offset1 += 9;
			offset2 --;
			break;
		    case 1:
			offset1 = '9';
			break;
		    case 2:
			offset1 --;
			offset2 ++;
			break;
		    case 3:
			offset1 -= 3;
			offset2 += 2;
			break;
		    case 4:
			offset1 = 't';
			offset2 = -3;
			break;
		    case 6:
			offset1 += 5;
			break;
		    case 7:
			offset1 = '9';
			offset2 += 2;
			break;
		    case 8:
			offset1 ++;
			offset2 -= 7;
			break;

		}
		idx ++;
	}
	pass_val[pass_len] = 0;
    }
    else
    {
	log("Unknown password request: %d", pass_num);
	return;
    }

    for (int i=0;i<pass_len;i++) pass_val[i] = ~pass_val[i];

    //printf("pass: '%s'\n", pass_val);

}

int handle_client(int s)
{
	int pid, n, pass_num, pass_len;
	char buff[5];
	char* pass_val;

	pid = fork();

	if (pid > 0) return pid;
	else if (pid < 0)
	{
		log(strerror( errno ));
		error("fork failed");
	}

	log("Client handler PID: %d", getpid());

	bool done = false;
	while (!done)
	{
		n = my_recv(s, buff, 4);
		if (n < 4)
		{
			log(strerror( errno ));
			close(s);
			error("my_recv failed: %d/4", n);
			return 0;
		}

		memcpy((void*)&pass_num, (void*)buff, 4);

		log("Pass num is: %d", pass_num);
		if (pass_num == 0)
		{
		    log("Client request finished");
		    close(s);
		    done = true;
		    break;
		}

		get_password(pass_num, pass_len, pass_val);

		memcpy((void*)buff, (void*)&pass_len, 4);
		n = my_send(s, buff, 4);
		if (n < 4)
		{
			log(strerror( errno ));
			close(s);
			error("my_send failed: %d/3", n);
			return 0;
		}

		if (pass_len > 0)
		{
			n = my_send(s, pass_val, pass_len);
			free( pass_val );
			if (n < pass_len)
			{
				log(strerror( errno ));
				close(s);
				error("my_send failed: %d/%d", n, pass_len);
				return 0;
			}
		}
		else
		{
		    log("No password number %d", pass_num);
		}
	}

	log("Client finished");

	exit(0);
	// not reached
	return 0;
}


int server()
{
   	int pid;
	int csock, sock, alive, reuse, err;
	struct sockaddr_in server;

#ifndef MDBG
	pid = fork();

	if (pid > 0) return 0;
	else if (pid < 0) error("error fork failed.\n");
#else
	pid = 0;
#endif

	log("Password server PID: %d", getpid());

	setup_signals();

	server.sin_family = AF_INET;
	server.sin_port = htons(9002);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == -1)
	{
		log(strerror( errno ));
		error("Create socket failed.");
	}

	alive=1;

	err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&alive, sizeof(void*));

	if (err == -1)
        {
		log(strerror( errno ));
		error("Set socket options failed: keepalive");
        }

	reuse=1;

	err=setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&reuse, sizeof(void*));

	if (err == -1)
        {
		log(strerror( errno ));
		error("Set socket options failed: reuseaddr");
        }

	if (bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1)
	{
		log(strerror( errno ));
		error("Bind address failed.");
	}

	log("Beginning listen on server socket: %d", sock);

	if (listen(sock,5) == -1)
	{
		log(strerror( errno ));
		error("Listen on socket failed");
	}

	int running = 1;

	while (running)
	{
		log("Wait for client...");

		if ((csock = accept(sock,NULL,NULL)) == -1)
		{
			log(strerror( errno ));
			log("Accept on socket failed");
		}

		log("New client on socket: %d", csock);
		pid = handle_client( csock );
		log("New client PID: %d", pid);
	}

	exit(0);

	return 0;
}

int main(int lb, char** par)
{
    /*
    int l;
    char* s;
    get_password(0, l, s);
    get_password(1, l, s);
    exit(0);
    */

    return server();
}

