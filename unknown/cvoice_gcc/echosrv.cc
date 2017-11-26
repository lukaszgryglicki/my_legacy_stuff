#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <set>

struct ClientData
{
	ClientData()
	{
		id_from = id_to = id_type = sock = sock_to = 0;
	}
	int id_from, id_to, id_type, sock, sock_to;
};

struct ClientUni
{
	ClientUni()
	{
		id_from = id_type = 0;
	}
	long id_from, id_type;
};

std::set<ClientData> set_clients;
std::set<ClientUni> set_ids;

static long srv_sock = 0;
pthread_mutex_t mutex;
static int timeout = 2;
static int sig = 0;
static int srv_bidir = 0;
static int srv_dbg = 0;
static int srv_cache = 2048;
static int PyVoiceDedicated = 0;

bool operator<(const ClientUni& _left, const ClientUni& _right)
{
	if (_left.id_from == _right.id_from)
	{
		return (bool)(_left.id_type  < _right.id_type );
	}
	else return (bool)(_left.id_from < _right.id_from);
}

bool operator<(const ClientData& _left, const ClientData& _right)
{
	if (_left.id_from == _right.id_from)
	{
		if (_left.id_to == _right.id_to)
		{
			if (_left.id_type == _right.id_type)
			{
				return (bool)(_left.sock  < _right.sock );
			}
			else return (bool)(_left.id_type < _right.id_type);
		}
		else return (bool)(_left.id_to < _right.id_to);
	}
	else return (bool)(_left.id_from < _right.id_from);
}

void signal_mutex()
{
	pthread_mutex_unlock(&mutex); 
}

void wait_mutex()
{
	alarm(timeout); 
	pthread_mutex_lock(&mutex); 
	alarm(0); 
}

void log(const char* fmt, ...)
{
	FILE* f;
	va_list lst;
	static struct tm *tms;
	time_t t;
	char tstr[128];

	time(&t);
	tms = localtime(&t);

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	f = fopen("/tmp/echosrv.log", "a+");
	if (!f) return;

	fprintf(f, "%s: ", tstr);

	va_start(lst,fmt);
	vfprintf(f, fmt,lst);
	va_end(lst);

	fprintf(f, "\n");

	fflush(f);
	fclose(f);
}

void logd(const char* fmt, ...)
{
	FILE* f;
	va_list lst;
	static struct tm *tms;
	time_t t;
	char tstr[128];

	if (!srv_dbg) return;
	time(&t);
	tms = localtime(&t);

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	f = fopen("/tmp/echosrv.log", "a+");
	if (!f) return;

	fprintf(f, "%s: ", tstr);

	va_start(lst,fmt);
	vfprintf(f, fmt,lst);
	va_end(lst);

	fprintf(f, "\n");

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

	time(&t);
	tms = localtime(&t);

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	f = fopen("/tmp/echosrv.log", "a+");
	if (!f) return;

	fprintf(f, "%s: ", tstr);

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

	log("Exiting due to error");

	exit(1);
}


void help()
{
 	printf("Options are:"
	       "\n\t-h this help"
	       "\n\t-p port (default 9000)"
	       "\n\t-b (enables bidirecional on each channel RWRW...)"
	       "\n\t-d (enables debug mode in logs)"
	       "\n\t-c size (cache size of network packet, default 8192)"
	       "\n\t-P PyVoice dedicated mode\n");
}

void remove_clients()
{
    std::set<ClientData>::iterator iter;

    wait_mutex();
    for (iter = set_clients.begin(); iter != set_clients.end(); ++iter)
    {
	if (iter->sock > 1) close(iter->sock);
    }

    set_clients.clear();
    set_ids.clear();
    signal_mutex();
}

void catch_int(int signo)
{
	log("Interrupt nr. %d", signo);

	if (sig==1) 
	{ 
		log("Already processing signal!\n"); 
		sig++; 
		return; 
	}

	if (sig==2) 
	{ 
		error("Dirty shutdown, many signals recieved\n"); 
		exit(2); 
	}

	sig = 1;

	if (signo==SIGINT)
	{
	    remove_clients();
	}
	else if (signo==SIGPIPE)
	{
	    remove_clients();
	}
	else if (signo==SIGALRM)
	{
		log("Deadlock detected");
		return;
	}

	if (close(srv_sock) == -1) 
	{
		log(strerror( errno ));
		error("Closing main server socket");
	}
	else 
	{
		log("Server main socket closed");
	}

	sig = 0;

	exit(0);
}


void setup_signals()
{
	static struct sigaction chldact;
	static struct sigaction intact;

	log("Setting up signals handlers...");

	chldact.sa_handler = SIG_IGN;
	/*chldact.sa_flags = SA_NOCLDWAIT;*/
	sigfillset(&(chldact.sa_mask));
	sigaction(SIGCHLD, &chldact, NULL);

	intact.sa_handler = catch_int;
	sigfillset(&(intact.sa_mask));
	sigaction(SIGINT, &intact, NULL);
	sigaction(SIGPIPE, &intact, NULL);
	sigaction(SIGALRM, &intact, NULL);
}

void init_mutex()
{
	pthread_mutexattr_t mutex_attr;

	log("Setting up mutex...");
	if (pthread_mutexattr_init(&mutex_attr)) error("Creating MUTEX attributes.\n");
	if (pthread_mutex_init(&mutex, (const pthread_mutexattr_t*)(&mutex_attr))) error("Createing MUTEX");
}

void remove_client(long id_from, long id_to, long id_type)
{
	std::set<ClientData>::iterator itercd;
	ClientUni cu;

	cu.id_from = id_from;
	cu.id_type = id_type;

	wait_mutex();
	set_ids.erase( cu );
	for (itercd = set_clients.begin(); itercd != set_clients.end(); ++itercd)
	{
		if (itercd->id_from == id_from && itercd->id_to == id_to && itercd->id_type == id_type)
		{
			log("Removed client: id_from=%d, id_to=%d, id_type=%d", id_from, id_to, id_type);
			set_clients.erase( itercd );
			break;
		}
	}
	signal_mutex();
}

int my_read(int fd, char* buff, int n)
{
	long nrd, nn;

	nrd = 0;
	while (nrd < n)
	{
		nn = read(fd, (void*)(&buff[nrd]), n-nrd);
		if (nn <= 0) return nn;
		nrd += nn;
	}

	return nrd;
}

int my_write(int fd, char* buff, int n)
{
	long nwrt, nn;

	nwrt = 0;
	while (nwrt < n)
	{
		nn = write(fd, (void*)(&buff[nwrt]), n-nwrt);
		if (nn <= 0) return nn;
		nwrt += nn;
	}

	return nwrt;
}

void adjust_cache(int bs)
{
    int tmp;

    logd("Adjust cache: %d", bs);
    tmp = 1;
    while (tmp < bs) 
    {
	tmp <<= 1;
    }
    tmp <<= 1;

    if (tmp < 0x0200) tmp = 0x0200;
    if (tmp > 0x8000) tmp = 0x8000;

    if (srv_cache != tmp)
    {
    	srv_cache = tmp;
    	logd("Adjusted read cache: %d", srv_cache);
    }
    else 
    {
	logd("Cache remained at %d bytes", srv_cache);
    }
}

void* client_thread(void* ptr)
{
	char buff[20];
	int nread, fd;
	int id_from, id_to, id_type, found, ms, all_ms;

	memcpy((void*)&fd, (void*)&ptr, sizeof(int));
	all_ms = 0;
	ms = 100000;

	if ((nread = read(fd, buff, 12)) < 12) 
	{ 
		log(strerror( errno ));
		log("Error reading client data, read %d bytes", nread);
		close(fd);
		return NULL;
	}

	memcpy((void*)&id_from, (const void*)(&buff[0]), 4);
	memcpy((void*)&id_to, (const void*)(&buff[4]), 4);
	memcpy((void*)&id_type, (const void*)(&buff[8]), 4);

	if (id_from <= 0 || id_to <= 0 || id_from == id_to || (id_type != 0 && id_type != 1))
	{
		log("Bad params from client: request ID: %d, connect to ID: %d, type: %d", id_from, id_to, id_type);
		close(fd);
		return NULL;
	}

	log("Client: request ID: %d, connect to ID: %d, type: %d", id_from, id_to, id_type);

	std::set<ClientUni>::iterator iter;
	std::set<ClientData>::iterator itercd;
	ClientUni cu, cu_to;
	ClientData cd;

	cu.id_from = id_from;
	cu.id_type = id_type;

	cd.id_from = id_from;
	cd.id_type = id_type;
	cd.id_to   = id_to;
	cd.sock    = fd;
	cd.sock_to = 0;

	wait_mutex();
	iter = set_ids.find( cu );
	if (iter != set_ids.end())
	{
		signal_mutex();
		log("Such client is already online: id_from: %d, id_type: %d", iter->id_from, iter->id_type);
		close(fd);
		return NULL;
	}

	set_ids.insert( cu );
	set_clients.insert( cd );
	signal_mutex();

	cu_to.id_from = id_to;
	if (id_type) cu_to.id_type = 0;
	else cu_to.id_type = 1;

	found = 0;
	while (!found)
	{
		wait_mutex();
		iter = set_ids.find( cu_to );
		if (iter != set_ids.end()) found = 1;
		else found = 0;
		signal_mutex();

		if (!found) 
		{
			pthread_yield();
			usleep(ms);		
			all_ms += ms;
		}
	}

	log("Waited %d ms.", all_ms / 1000);
	log("Found target: id_from=%d id_type=%d", cu_to.id_from, cu_to.id_type);

	found = 0;
	wait_mutex();
	for (itercd = set_clients.begin(); itercd != set_clients.end(); ++itercd)
	{
		if (itercd->id_from == id_to && itercd->id_type == cu_to.id_type)
		{
			cd.sock_to = itercd->sock;
			found = 1;
			break;
		}
	}
	signal_mutex();

	if (!found)
	{
		log("Error finding target: id_to: %d, type: %d", id_to, cu_to.id_type);
		remove_client(id_from, id_to, id_type);
		close(fd);
		return NULL;
	}

	log("Type: %d, communication IDs: %d <-> %d, sockets: %d <-> %d", id_type, id_from, id_to, cd.sock, cd.sock_to);

	char bf[0x8000];
	int done;
	int bn;
	int n, m, nc;
	long s1, s2;
	char io;
	short sm;

	if (srv_cache > 0x8000) srv_cache = 0x8000;

	nc = done = 0;
	log("Network buffer size is: %d", srv_cache);
	if (PyVoiceDedicated) log("PyVoice dedicated mode on");

	if (id_type == 0)
	{
	    s1 = cd.sock;
	    s2 = cd.sock_to;
	    io = 'I';
	}
	else
	{
	    s1 = cd.sock_to;
	    s2 = cd.sock;
	    io = 'O';
	}

	bn = srv_cache;

	while(!done)
	{
		n = 0;
		logd("%cRwait", io);
		n = read(s1, bf, bn);
		logd("%cR: %d", io, n);
		if (n > 0)
		{
			if (PyVoiceDedicated && n == 4)
			{
			    memcpy((void*)(&m), (void*)bf, 4);
			    adjust_cache(m);
			    bn = srv_cache;
			}
			if (PyVoiceDedicated && n == 2)
			{
			    memcpy((void*)(&sm), (void*)bf, 2);
			    m = (int)sm;
			    adjust_cache(m);
			    bn = srv_cache;
			}
			bf[n] = 0;
			logd("<%s>", bf);
		       	logd("%cWwait: %p", io, bf);
			n = write(s2, bf, n);
			logd("%cW: %d", io, n);
		}
		if (srv_bidir || PyVoiceDedicated)		
		{
		        logd("%cRwait B", io);
			n = read(s2, bf, bn);
			logd("%cRb: %d", io, n);
			if (n > 0)
			{
				bf[n] = 0;
				logd("<%s>", bf);
		       		logd("%cWwait B", io);
				n = write(s1, bf, n);
				logd("%cWb: %d", io, n);
			}
		}

		if (n < 0)
		{
			log(strerror( errno ));
			log("%c failed: id_type: %d, n: %d", io, id_type, n);
			remove_client(id_from, id_to, id_type);
			close(fd);
			return NULL;
		}
		else if (n == 0)
		{
			log("Communication finished after %d cycles", nc);
			done = 1;
		}
		else 
		{
			nc ++;
			if (nc > 0 && !(nc % 100))
			{
				log("Processed %d buffers: %d Kb of data", nc, (nc * srv_cache)/1024);
			}
		}
	}

	remove_client(id_from, id_to, id_type);
	close(fd);
	return NULL;
}

void register_client(int fd)
{
	pthread_t *t;

	log("New client on socket: %d", fd);

	t = (pthread_t*)malloc(sizeof(pthread_t));
	if (!t) 
	{
		error("Malloc new thread");
	}

	if (pthread_create(t, NULL, client_thread, (void*)fd)) 
	{
		log("Create client thread");
		close(fd);
	}

	log("Client thread started");
}

void server(int port, int bidir)
{
	int csock, sock, alive, reuse, err;
	struct sockaddr_in server;

	log("PID: %d", getpid());
	log("Starting server at port %d, bidirecional mode: %d", port, bidir);

	if (port < 1 || port > 0xffff) error("Port: %d is invalid", port);
	if (port < 1024) log("Warning: port: %d is less than 1024 and can be OS-protected.", port);

	setup_signals();
	init_mutex();

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == -1)
	{
		log(strerror( errno ));
		error("Create socket failed.");
	}

	alive=1;
	err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &alive, sizeof(void*));

	if (err == -1)
        {
		log(strerror( errno ));
		error("Set socket options failed");
        }

	reuse=1;
	err=setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(void*));

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

	if (listen(sock,5) == -1)
	{
		log(strerror( errno ));
		error("Listen on socket failed");
	}

	srv_sock = sock;
	srv_bidir = bidir;

	log("Main server socket: %d", srv_sock);

	while (1)
	{
		log("Wait for client...");

		if ((csock = accept(sock,NULL,NULL)) == -1)
		{
			log(strerror( errno ));
			error("Accept on socket failed");
		}

		register_client(csock);
	}

}

int main(int lb, char** par)
{
	char u;
	char port[12];
	int bidir;

	bidir = 0;
	strcpy(port,"9000");

	while ((u = getopt(lb, par, "p:c:hbdP"))!=-1)
	{
		switch (u)
		{
			case 'p': if (strlen(optarg)<12) strcpy(port, optarg); break;
			case 'c': srv_cache = atoi(optarg);
			case 'b': bidir = 1; break;
			case 'P': PyVoiceDedicated = 1; break;
			case 'd': srv_dbg = 1; break;
			case 'h': help(); return 0;
			default: printf("Unrecognized option\n"); return 1;
		}
	}

	if (!fork()) 
	{
		server(atoi(port), bidir);
	}

	return 0;
}

