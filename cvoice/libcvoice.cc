#include <libcvoice.h>

cv_cfg :: cv_cfg()
{
    clear();
}

void cv_cfg :: clear()
{
	inum = new char[1024];
	onum = new char[1024];
	ip = new char[32];
    	myid = targetid = 0;
	cache_size = 50;	// wide: in 320b packets, 8bit: in 160b packets
	debug = 0;
	quality = 5;
	complexity = 10;
	vbr = 1;
	vad = 1;
	dtx = 1;
	timeout = -20;
	strcpy(inum, "/dev/dsp");
	strcpy(onum, "/dev/dsp");
	exmode = 0;
	srv_sock = cli_sock = 0;
	wide = 1;
	enh_r = 1.0;
	enh_w = 1.0;
	save = 0;
	n_sync = 20;
	hz = 8000;
	strcpy(ip, "127.0.0.1");
	port = 9000;
	pidlog = 0;
}

cv_cfg :: ~cv_cfg()
{
	delete [] inum;
	delete [] onum;
	delete [] ip;
}

#ifdef MSPX

speex_cfg :: speex_cfg()
{
	enc_state = dec_state = NULL;
	enc_fsize = dec_fsize = 0;
	enc_osize = dec_osize = 0;
	input = output = NULL;
}

speex_cfg spx;
#endif

#ifdef MMT

pthread_mutex_t mutex;
pthread_t tr_send;
pthread_t tr_play;
std::set<pthread_t> set_tr_send;
std::set<pthread_t> set_tr_play;
bool have_tr_send;
bool have_tr_play;

#ifdef MNDPLX
pthread_mutex_t mutex_audio;
#endif

#endif

#ifdef MIPC

int mshmid;
key_t mshmkey;
void*  mshm_ptr;

#endif

cv_cfg cfg;
char log_pfx[32];
time_t time_start;
int n_read, n_write, n_sent, n_recieved, n_steps;

void my_exit(int ecode)
{
    	cvoice_log("Exiting with code: %d", ecode);
#ifndef MNOEXIT
	exit( ecode );
#endif
}

void cvoice_log(const char* fmt, ...)
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

	if (cfg.pidlog)
	{
		sprintf(tstr, "/tmp/cvoice_%d_%d.log", uid, pid);
		f = fopen(tstr, "a+");
	}
	else
	{
		sprintf(tstr, "/tmp/cvoice_%d.log", uid);
		f = fopen(tstr, "a+");
	}
	if (!f) return;

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	fprintf(f, "%6d %s: %s: ", pid, tstr, log_pfx);

	va_start(lst,fmt);
	vfprintf(f, fmt,lst);
	va_end(lst);

	fprintf(f, "\n");

	fflush(f);
	fclose(f);

#ifdef MDBG
	fprintf(stdout, "%6d %s: %s: ", pid, tstr, log_pfx);

	va_start(lst,fmt);
	vfprintf(stdout, fmt,lst);
	va_end(lst);

	fprintf(stdout, "\n");
#endif
}

void cvoice_logd(const char* fmt, ...)
{
	FILE* f;
	va_list lst;
	static struct tm *tms;
	time_t t;
	char tstr[128];
	int pid;
	int uid;

	if (!cfg.debug) return;

	time(&t);
	tms = localtime(&t);

	pid = getpid();
	uid = getuid();

	if (cfg.pidlog)
	{
		sprintf(tstr, "/tmp/cvoice_%d_%d.log", uid, pid);
		f = fopen(tstr, "a+");
	}
	else
	{
		sprintf(tstr, "/tmp/cvoice_%d.log", uid);
		f = fopen(tstr, "a+");
	}
	if (!f) return;

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	fprintf(f, "%6d %s: %s: ", pid, tstr, log_pfx);

	va_start(lst,fmt);
	vfprintf(f, fmt,lst);
	va_end(lst);

	fprintf(f, "\n");

	fflush(f);
	fclose(f);
}

void cvoice_error(const char* fmt, ...)
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

	if (cfg.pidlog)
	{
		sprintf(tstr, "/tmp/cvoice_%d_%d.log", uid, pid);
		f = fopen(tstr, "a+");
	}
	else
	{
		sprintf(tstr, "/tmp/cvoice_%d.log", uid);
		f = fopen(tstr, "a+");
	}
	if (!f) return;

	sprintf(tstr, "%04d-%02d-%02d %02d:%02d:%02d", 
		1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	fprintf(f, "%6d %s: %s: ", pid, tstr, log_pfx);

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

#ifdef MDBG
	fprintf(stdout, "%6d %s: %s: ", pid, tstr, log_pfx);

	va_start(lst,fmt);
	vfprintf(stdout, fmt,lst);
	va_end(lst);

	fprintf(stdout, "\n");
#endif

	cvoice_log("Exiting due to error");

	cvoice_clear_mshm();
	my_exit(1);
}

inline void mwaitaudio()
{
#ifdef MMT
#ifdef MNDPLX
    	//printf("waita\n");
    	cvoice_logd("wait mutex_audio");
 	pthread_mutex_lock(&mutex_audio);
	cvoice_logd("mutex_audio locked");
#endif
#endif
}

inline void msignalaudio()
{
#ifdef MMT
#ifdef MNDPLX
    	//printf("signala\n");
    	cvoice_logd("unlocking mutex_audio");
 	pthread_mutex_unlock(&mutex_audio);
	cvoice_logd("mutex_audio unlocked");
#endif
#endif
}

inline void mwait()
{
#ifdef MMT
    	cvoice_logd("wait mutex");
 	pthread_mutex_lock(&mutex);
	cvoice_logd("mutex locked");
#endif
}

inline void msignal()
{
#ifdef MMT
    	cvoice_logd("unlocking mutex");
 	pthread_mutex_unlock(&mutex);
	cvoice_logd("mutex unlocked");
#endif
}

void cvoice_init_mipc(int clear)
{
#ifdef MIPC

	mshm_ptr = NULL;
 	mshmkey = 9000;

 	if ((mshmid = mshm(mshmkey,1024)) == -1) 
	{
	    cvoice_error("Creating 1Kb SHM key=%d", mshmkey);
	}

 	cvoice_log("SHM key: %d %d", mshmid, mshmidfromkey(mshmkey));

 	if (mshmconn(mshmid, &mshm_ptr) == -1) 
	{
	    cvoice_error("Connect shm to local address");
	}

    	if (clear) 
    	{
	    cvoice_clear_mshm();
    	}

	cvoice_log("SHM initialized");

	//mdropshm(mshmid);
#endif
}

void init_mutex()
{
#ifdef MMT
 	pthread_mutexattr_t mutex_attr;

 	if (pthread_mutexattr_init(&mutex_attr)) cvoice_error("creating MUTEX attributes.\n");
 	if (pthread_mutex_init(&mutex, (const pthread_mutexattr_t*)(&mutex_attr))) cvoice_error("createing MUTEX");

#ifdef MNDPLX
 	pthread_mutexattr_t mutex_audio_attr;

 	if (pthread_mutexattr_init(&mutex_audio_attr)) cvoice_error("creating MUTEX_AUDIO attributes.\n");
 	if (pthread_mutex_init(&mutex_audio, (const pthread_mutexattr_t*)(&mutex_audio_attr))) cvoice_error("createing MUTEX_AUDIO");
#endif

	have_tr_send = have_tr_play = false;

	cvoice_logd("mutex created");
#endif
}

void join_threads(std::set<pthread_t>& thrs)
{
#ifdef MMT
    if (thrs.size() < NJOIN) 
    {
	cvoice_logd("Not joining, size is: %d", thrs.size());
	return;
    }

    cvoice_logd("Joining %d threads", thrs.size());
    for (std::set<pthread_t>::iterator i=thrs.begin();i!=thrs.end();++i)
    {
	pthread_join(*i, NULL);
	cvoice_logd("joined");
    }

    thrs.clear();
    cvoice_logd("Joined all");

#endif
}

void mipc_stats()
{
#ifdef MIPC
    	if (!mshm_ptr) return;
	time_t time_end;

	time(&time_end);

	double tdiff = (double)(time_end - time_start);

	if (tdiff <= 0.0) return;

	if (n_sent > 0 && n_recieved > 0)
	{
	    	double bps[10];

		bps[0] = (double)(n_sent + n_recieved) / tdiff;
		bps[1] = (((double)n_recieved + (double)n_sent) * 100.0) / ((double)n_write + (double)n_read);
		bps[2] = tdiff;
		bps[3] = (double)n_sent;
		bps[4] = (double)n_recieved;
		bps[5] = (double)n_read;
		bps[6] = (double)n_write;

		memcpy(mshm_ptr, (void*)bps, 10 * sizeof(double));
	}
#endif
}

void traffic_stats(int out)
{
#ifdef MDBG
	cvoice_logd("Time start: %d", time_start);
#endif

	if (time_start <= 0) return;
	
	time_t time_end;

	time(&time_end);

	double tdiff = (double)(time_end - time_start);

	if (tdiff <= 0.0) return;

	cvoice_log("Elapsed time %.0f", tdiff);

	if (n_read > 0 && n_sent > 0)
	{
		cvoice_log("Bytes read %d", n_read);
		double bps = (double)n_read / tdiff;
		cvoice_log("BPS uncompressed %f", bps);

		cvoice_log("Bytes sent %d", n_sent);
		bps = (double)n_sent / tdiff;
		cvoice_log("BPS send %f", bps);

		bps = ((double)n_sent * 100.0) / (double)n_read;
		cvoice_log("Compression sent %f%%", bps);
	}

	if (n_write > 0 && n_recieved > 0)
	{
		cvoice_log("Bytes recieved %d", n_recieved);
		double bps = (double)n_recieved / tdiff;
		cvoice_log("BPS recieve %f", bps);

		cvoice_log("Bytes written %d", n_write);
		bps = (double)n_write / tdiff;
		cvoice_log("BPS uncompressed %f", bps);

		bps = ((double)n_recieved * 100.0) / (double)n_write;
		cvoice_log("Compression recieved %f%%", bps);
	}

	if (n_sent > 0 && n_recieved > 0)
	{
		double bps = (double)(n_sent + n_recieved) / tdiff;
		cvoice_log("Traffic BPS: %f", bps);

		double bps2 = (((double)n_recieved + (double)n_sent) * 100.0) / ((double)n_write + (double)n_read);
		cvoice_log("Traffic compression all %f%%", bps2);

		if (out) printf("Elapsed: %5.0f BPS: %8.3f Compression: %5.2f%%\n", tdiff, bps, bps2);
	}
}

void catch_int(int signo)
{
	cvoice_log("Interrupt nr. %d", signo);

	if (signo == SIGHUP)
	{
	    	cvoice_log("Statistics demand");
		traffic_stats(0);
		return;
	}
	if (signo == SIGCONT)
	{
	    	cvoice_log("Statistics stdout demand");
		traffic_stats(1);
		return;
	}
	else if (signo == SIGALRM)
	{
	    	cvoice_log("Warning: Network timeout");
	    	cvoice_log("Current operation timeout");
	}
	else if (signo == SIGUSR1)	// was also SIGINFO
	{
	    	cvoice_log("Manual user stop request");
	}

	traffic_stats(0);
	cvoice_log("Exiting...");

	if (cfg.srv_sock) 
	{
	    close(cfg.srv_sock);
	    cfg.srv_sock = 0;
	}

	if (cfg.cli_sock) 
	{
	    close(cfg.cli_sock);
	    cfg.cli_sock = 0;
	}

	cvoice_clear_mshm();
	my_exit(2);
}

void setup_signals()
{
	static struct sigaction chldact;
	static struct sigaction intact;

	cvoice_log("Setting up signals handlers...");

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
	sigaction(SIGUSR1, &intact, NULL);
	sigaction(SIGCONT, &intact, NULL);
	//sigaction(SIGINFO, &intact, NULL);
	sigaction(SIGHUP, &intact, NULL);

	siginterrupt(SIGHUP, 0);
	siginterrupt(SIGCONT, 0);

	cvoice_log("Handlers installed.");
}

void cvoice_get_host(char* to, char* name)
{
	struct hostent* hose;
	struct in_addr addr;

	cvoice_log("DNS resolving IP for: %s", name);
	hose = gethostbyname(name);
	if (!hose)
	{
		//pcvoice_error("gethostbyname");
		cvoice_log(strerror( errno ));
		cvoice_error("Cannot get IP for hostname: %s\n", name);
	}
	memcpy((void*)&addr,(void*)(hose->h_addr_list[0]),sizeof(struct in_addr));
	strcpy(to, inet_ntoa(addr));
	cvoice_log("Got IP %s for hostname %s", to, name);
}

void cvoice_help(char* name)
{
	printf("%s usage:"
		"\n\t-a\tEnable server mode (off)"
		"\n\t-s\tServer name to connect to (localhost)"
		"\n\t-i\tIP number to connect to (127.0.0.1)"
		"\n\t-p\tPort to connect to or start server at (9000)"
		"\n\t-f\tOwn ID in ext mode (enables ext mode, default 0 and off)"
		"\n\t-t\tTarget ID in ext mode (enables ext mode, default 0 and off)"
		"\n\t-h\tThis help"
		"\n\t-c\tNetwork cache in 160b (0.02s) packets (50: 1s)"
		"\n\t-d\tEnables debug"
		"\n\t-q\tSpeex encoder quality [0-10] (5)"
		"\n\t-T\tNet timeout (-20) 0=off negative enables timeout after connect"
		"\n\t-I\tInput device (microphone) (/dev/dsp)"
		"\n\t-O\tOutput device (speakers) (/dev/dsp)"
		"\n\t-P\tUse PID in log name (off)"
		"\n\t-8\tUse 8bit audio instead of 16bit (off)"
		"\n\t-R\tEnchance input float (1.0)"
		"\n\t-W\tEnchance output float (1.0)"
		"\n\t-S\tSave VoIP data to files"
		"\n\t-C\tCompexity 0-10 (10)"
		"\n\t-y\tSync communication after N cycles (20 * cache = 20s)"
		"\n\t-H\tSet sample rate in Hz (8000)"
		"\n\t-L\tLoad configuration from file name given"
		"\n\t-Z\tSave configuration to file name given"
		"\n", name);
	my_exit(0);
}


void cvoice_check_config()
{
	int ipi[4];

	cvoice_log("Checking configuration...");
	cvoice_log("Requested cache size is: %d packets", cfg.cache_size);

	if (sscanf(cfg.ip, "%d.%d.%d.%d", &ipi[0], &ipi[1], &ipi[2], &ipi[3]) != 4)
	{
	    cvoice_error("Bad IP format: %s", cfg.ip);
	}

	for (int i=0;i<4;i++)
	{
		if (ipi[i] < 0 || ipi[i] > 255) cvoice_error("Bad IP format: %s", cfg.ip);
	}

	if (cfg.port < 1 || cfg.port > 0xffff) cvoice_error("Bad port number: %d", cfg.port);
	
	if (cfg.myid > 0 && cfg.targetid > 0) cfg.exmode = 1;
	else cfg.exmode = 0;
	
	if (cfg.exmode && (cfg.myid < 1 || cfg.targetid < 1 || cfg.myid == cfg.targetid))
	    cvoice_error("Bad extened mode IDs: %d %d", cfg.myid, cfg.targetid);

	if (!cfg.exmode && (cfg.myid > 0 || cfg.targetid > 0))
	    cvoice_error("If you want to use ext_mode, you have to select both myid and targetid");

	if (cfg.cache_size < 1 || cfg.cache_size > 500)
	    cvoice_error("Network cache too low or too high");

	if (cfg.quality < 0 || cfg.quality > 10)
	    cvoice_error("Speex quality must be in range of 0-10");

	if (abs(cfg.timeout) > 0x8000)
	    cvoice_error("Network timeout to high (absolute value)");

	if (cfg.enh_r < 0.01 || cfg.enh_r > 10.) cvoice_error("Enchance input not in range 0.01-10");
	if (cfg.enh_w < 0.01 || cfg.enh_w > 10.) cvoice_error("Enchance output not in range 0.01-10");

	if (cfg.complexity < 0 || cfg.complexity > 10) cvoice_error("Complexity must be in range 0-10");

	if (cfg.n_sync < 0 || cfg.n_sync > 1000) cvoice_error("Sync communication must be in range 0-1000");

	if (cfg.hz < 2000 || cfg.hz > 24000) cvoice_error("Bad samplerate value, allowed 2kHz - 24 kHz");

	cvoice_log("Enchance factors: %f %f", cfg.enh_r, cfg.enh_w);

	cvoice_log("Configuration test passed.");
}

void my_save(char* buff, unsigned short l, int dir, int pid)
{
	FILE* fp;
	char fn[64];

	cvoice_logd("my_save: %d", l+2);

	if (!dir) sprintf(fn, "input_%d.mspx", pid);
	else sprintf(fn, "output_%d.mspx", pid);

	fp = fopen(fn, "a+");
	if (!fp) return;

	fwrite((void*)&l, sizeof(unsigned short), 1, fp);
	fwrite((void*)buff, l, 1, fp);

	fclose(fp);
}

void my_waitpid(int pid, int opt)
{
	int stat;

	cvoice_logd("Wait for pid %d, option %d", pid, opt);

	if (opt == WNOHANG)
	{
		cvoice_logd("Fast wait");
		waitpid( pid, &stat, opt );
		cvoice_logd("Status returned: %d", stat);
		return;
	}

	if (pid <= 0)
	{
		cvoice_logd("No waiting for 0 PID");
		return;
	}

	if (cfg.timeout> 0) alarm(cfg.timeout);

	waitpid( pid, &stat, opt );

	if (cfg.timeout > 0) alarm(0);

	cvoice_logd("Wait finished");
}

void* thread_sender(void* ptr)
{
    	thread_data_send* td;
    	int sock, n, ns;
    	char* bstr;

	cvoice_logd("thread: sender start");

    	td = (thread_data_send*)ptr;

    	n = td->l;
    	sock = td->s;
    	bstr = (char*)td->ptr;

	delete td;

	//cvoice_logd("thread sender started: L=%d S=%d PTR=%p", n, sock, bstr);

	int nsend = 0;

	mwait();
	if (cfg.timeout > 0) alarm(cfg.timeout);

	while (nsend < n)
	{
#ifdef MWIN
		ns = send(sock, (const char*)(&bstr[nsend]), n - nsend, 0);
#else
		ns = send(sock, (void*)(&bstr[nsend]), n - nsend, 0);
#endif
#ifdef MSLOW
		usleep( SLOWDOWN );
		//printf("%d\n", SLOWDOWN);
#endif
		if (ns <= 0)
		{
		    msignal();
		    if (cfg.timeout > 0) alarm(0);
		    cvoice_error("send data error: %d", ns);
		}
		nsend += ns;
		cvoice_logd("nsent: %d, ns=%d", nsend, ns);
		//printf("nsent: %d, ns=%d\n", nsend, ns);
	}
	
	msignal();
	if (cfg.timeout > 0) alarm(0);

	delete [] bstr;
	cvoice_logd("thread sender finished");

	//cvoice_log(">> thread: sender end");

#ifdef MMT
	pthread_exit((void*)nsend);
#endif
	return (void*)nsend;
}

int my_send_mt(int sock, char* bstr, int n)
{
    	thread_data_send* td;
	cvoice_logd("my_send_mt: %d", n);

	td = new thread_data_send;

	td->l = n;
	td->s = sock;
	td->ptr = (void*)(new char[n]);
	memcpy(td->ptr, (void*)bstr, n);

#ifdef MMT
 	if (pthread_create(&tr_send, NULL, thread_sender, (void*)td)) 
	//if (0)
	{
	    cvoice_error("creating pthread send");
	    return 0;
	}

	set_tr_send.insert( tr_send );

	//thread_sender( td );

	have_tr_send = true;
	//pthread_yield();
#else	
	n = (int)thread_sender((void*)td);
#endif
	cvoice_logd("thread sender spawned");
	return n;
}

inline int my_send_mt(int sock, unsigned char* bstr, int n)
{
    	return my_send_mt(sock, (char*)bstr, n);
}

int my_write(int fd, unsigned char* bstr, int n)
{
	int nw, nwritten = 0;
	int nerr = 0;
	cvoice_logd("my_write: %d", n);

	while (nwritten < n)
	{
	    	mwaitaudio();
		nw = write(fd, (void*)(&bstr[nwritten]), n - nwritten);
		msignalaudio();
		//printf("%d/%d/%d\n", nw, nwritten, n);
		if (nw <= 0)
		{
		    cvoice_logd("Partial written: %d,%d/%d (%5.2f%%)", 
		    	    nw, nwritten, n, ((float)nwritten * 100.) / (float)n);
		    //printf("Partial written: %d/%d (%5.2f%%)\n", 
		    //	    nwritten, n, ((float)nwritten * 100.) / (float)n);
		    usleep( 1000 );
		    nerr ++;
		    if (nerr > 100) 
		    {
		    	cvoice_log("Error writing: %d,%d/%d (%5.2f%%) - buffer played truncated", 
		    	    nw, nwritten, n, ((float)nwritten * 100.) / (float)n);
		    	//printf("Error writing: %d/%d (%5.2f%%) - buffer played truncated\n", 
		    	//    nwritten, n, ((float)nwritten * 100.) / (float)n);
			return nwritten;
		    }
		    continue;
		}
		nwritten += nw;
	}
	
	//printf("Success write: %d, retries: %d\n", n, nerr);
	return nwritten;
}

int my_send(int sock, char* bstr, int n)
{
	int nsend = 0;
	int ns;
	cvoice_logd("my_send: %d", n);

	if (cfg.timeout > 0) alarm(cfg.timeout);

	while (nsend < n)
	{
#ifdef MWIN
		ns = send(sock, (const char*)(&bstr[nsend]), n - nsend, 0);
#else
		ns = send(sock, (void*)(&bstr[nsend]), n - nsend, 0);
#endif
#ifdef MSLOW
		usleep( SLOWDOWN );
#endif
		if (ns <= 0)
		{
		    cvoice_log("Error sending sock=%d, ns=%d, nsend=%d, n=%d", sock, ns, nsend, n);
		    return nsend;
		}

		nsend += ns;
	}
	
	if (cfg.timeout > 0) alarm(0);

	return nsend;
}

inline int my_send(int sock, unsigned char* bstr, int n)
{
    return my_send(sock, (char*)bstr, n);
}

int my_read(int fd, unsigned char* bstr, int n)
{
	int nread = 0;
	int nr;
	cvoice_logd("my_read: %d", n);

	if (cfg.timeout > 0) alarm(cfg.timeout);

	while (nread < n)
	{
	    	mwaitaudio();
	    	nr = read(fd, (void*)(&bstr[nread]), n - nread);
		msignalaudio();

		if (nr <= 0)
		{
		    cvoice_log("Error reading data: fd=%d, nr=%d, nread=%d, n=%d", fd, nr, nread, n);
		    return nread;
		}

		nread += nr;
	}

	if (cfg.timeout > 0) alarm(0);

	return nread;
}

int my_recv(int sock, char* bstr, int n)
{
	int nrecv = 0;
	int nr;
	cvoice_logd("my_recv: %d", n);

	if (cfg.timeout > 0) alarm(cfg.timeout);

	while (nrecv < n)
	{
#ifdef MWIN
	    	nr = recv(sock, (char*)(&bstr[nrecv]), n - nrecv, 0);
#else
	    	nr = recv(sock, (void*)(&bstr[nrecv]), n - nrecv, 0);
#endif
#ifdef MSLOW
		usleep( SLOWDOWN );
#endif
		if (nr <= 0)
		{
		    cvoice_log("Error recieving data: sock=%d, nr=%d nrecv=%d, n=%d", sock, nr, nrecv, n);
		    return nrecv;
		}

		nrecv += nr;

	}

	if (cfg.timeout > 0) alarm(0);

	return nrecv;
}

inline int my_recv(int sock, unsigned char* bstr, int n)
{
    return my_recv(sock, (char*)bstr, n);
}

int cvoice_mspeex_encode_init()
{
#ifdef MSPX
	int tmp;
	float tmpf;
	spx.enc_state = speex_encoder_init(&speex_nb_mode);
	if (!spx.enc_state)
	{
	    	cvoice_error("Error creating speex encoder");
	}

	if (speex_encoder_ctl(spx.enc_state, SPEEX_GET_FRAME_SIZE, &tmp) != 0)
	{
	    	cvoice_error("Error getting speex encoder frame size");
	}
	spx.enc_fsize = tmp;

	if (spx.enc_fsize <= 0)
	{
	    	cvoice_error("Bad speex frame encoder size: %d", spx.enc_fsize);
	}

	spx.enc_osize = spx.enc_fsize + 40;

    	tmp = cfg.quality;
   	if (speex_encoder_ctl(spx.enc_state, SPEEX_SET_QUALITY, &tmp) != 0)
	{
	    	cvoice_error("Error seting speex encoder parameters: cbr quality");
	}

    	tmpf = (float)cfg.quality;
   	if (speex_encoder_ctl(spx.enc_state, SPEEX_SET_VBR_QUALITY, &tmpf) != 0)
	{
	    	cvoice_error("Error seting speex encoder parameters: vbr quality");
	}

    	tmp = cfg.complexity;
   	if (speex_encoder_ctl(spx.enc_state, SPEEX_SET_COMPLEXITY, &tmp) != 0)
	{
	    	cvoice_error("Error seting speex encoder parameters: complexity");
	}

    	tmp = cfg.vbr;
   	if (speex_encoder_ctl(spx.enc_state, SPEEX_SET_VBR, &tmp) != 0)
	{
	    	cvoice_error("Error seting speex encoder parameters: vbr");
	}

    	tmp = cfg.vad;
   	if (speex_encoder_ctl(spx.enc_state, SPEEX_SET_VAD, &tmp) != 0)
	{
	    	cvoice_error("Error seting speex encoder parameters: vad");
	}

    	tmp = cfg.dtx;
   	if (speex_encoder_ctl(spx.enc_state, SPEEX_SET_DTX, &tmp) != 0)
	{
	    	cvoice_error("Error seting speex encoder parameters: dtx");
	}

    	speex_bits_init(&spx.enc_bits);

	spx.input = new short[spx.enc_osize];
	if (!spx.input)
	{
	    cvoice_error("Error allocating speex input buffer");
	}

	cvoice_log("Speex encoder initialized, buffers: %d %d", spx.enc_fsize, spx.enc_osize);
#endif
	return 0;
}

int cvoice_mspeex_decode_init()
{
#ifdef MSPX
	int tmp;
	spx.dec_state = speex_decoder_init(&speex_nb_mode);
	if (!spx.dec_state)
	{
	    	cvoice_error("Error creating speex decoder");
	}

	if (speex_decoder_ctl(spx.dec_state, SPEEX_GET_FRAME_SIZE, &tmp) != 0)
	{
	    	cvoice_error("Error getting speex decoder frame size");
	}
	spx.dec_fsize = tmp;

	if (spx.dec_fsize <= 0)
	{
	    	cvoice_error("Bad speex decoder frame size: %d", spx.dec_fsize);
	}

	spx.dec_osize = spx.dec_fsize + 40;

    	tmp = 1;
   	if (speex_decoder_ctl(spx.dec_state, SPEEX_SET_ENH, &tmp) != 0)
	{
	    	cvoice_error("Error seting speex decoder parameters: enhancer");
	}

    	speex_bits_init(&spx.dec_bits);

	spx.output = new short[spx.dec_osize];
	if (!spx.output)
	{
	    cvoice_error("Error allocating speex output buffer");
	}

	cvoice_log("Speex decoder initialized, buffers: %d %d", spx.dec_fsize, spx.dec_osize);
#endif
	return 0;
}

int cvoice_mspeex_encode_destroy()
{
#ifdef MSPX
	cvoice_log("Destroying speex encoder");

	speex_encoder_destroy(spx.enc_state);
	speex_bits_destroy(&spx.enc_bits);
	delete [] spx.input;
#endif
	return 0;
}

int cvoice_mspeex_decode_destroy()
{
#ifdef MSPX
	cvoice_log("Destroying speex decoder");

	speex_decoder_destroy(spx.dec_state);
	speex_bits_destroy(&spx.dec_bits);
	delete [] spx.output;
#endif
	return 0;
}

int cvoice_mspeex_encode( unsigned char* in, int n, char* out, int& m)    
{
#ifdef MSPX
	register short sample;
	register int ne, nt, x;
	int enc;
	unsigned char fs;
	register float enh, s;

	cvoice_logd("Speex encoding %d bytes", n);

#ifdef MDBG
	char str[800], tmp[3];
	strcpy(str, "");
	tmp[2] = 0;
	for (int i=0;i<16;i++)
	{
	    sprintf(tmp, "%02x", in[i]);
	    strcat(str, tmp);
	}
	strcat(str, "...");
	cvoice_logd("ToEncode: %s...", str); 
#endif

	m = 0;
	if (n <= 0) return 1;

	ne = 0;
	enh = cfg.enh_r;

	while (ne < n)
	{
		nt = ne + spx.enc_fsize;
		if (cfg.wide) nt += spx.enc_fsize;
		if (nt > n) 
		{
			cvoice_log("Skipping buffer <%d, %d>, n=%d", ne, nt, n);
			break;
			ne = n;
		}

		if (cfg.wide)
		{
			x = 0;
			for (register int i=ne;i<nt;i+=2)
			{
				memcpy((void*)&spx.input[x], (void*)&in[i], 2);
				s = float(spx.input[x]) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.input[x] = short(s);
#ifdef MDBG
				cvoice_logd("SampleEnc: %d,%d --> %d", in[i], in[i+1], spx.input[x]);
#endif
				x ++;
			}
		}
		else
		{
			for (register int i=ne;i<nt;i++)
			{
				//input[i-ne] = ((short)in[i] << 8) ^ 0x8000;
				sample = (short)in[i];
				sample = (sample - 128) << 8;
				s = float(sample) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.input[i-ne] = short(s);
#ifdef MDBG
				cvoice_logd("SampleEnc: %d --> %d", in[i], spx.input[i-ne]);
#endif
			}
		}

		speex_bits_reset(&spx.enc_bits);
		enc = speex_encode_int(spx.enc_state, spx.input, &spx.enc_bits);
		if (enc < 0)
		{
			cvoice_log("Speex encoder error, enc: %d, n: %d", enc, n);
			return 1;
		}

		enc = speex_bits_nbytes(&spx.enc_bits);
		if (enc <= 0)
		{
			cvoice_log("Error getting frame size: <%d, %d> enc=%d", ne, nt, enc);
			return 1;
		}
		fs = (unsigned char)enc;
#ifdef MDBG
		cvoice_logd("U8 size is: %d", fs);
#endif

		memcpy((void*)(&out[m]), (void*)&fs, 1);
		m ++;

		enc = speex_bits_write(&spx.enc_bits, &out[m], spx.enc_osize);

		if (enc <= 0)
		{
			cvoice_log("Error encoding frame: <%d, %d> enc=%d", ne, nt, enc);
			return 1;
		}

#ifdef MDBG
		cvoice_logd("Encoded: frame <%d, %d>: %d --> %d", ne, nt, spx.enc_fsize, enc);
#endif
		ne += spx.enc_fsize;
		if (cfg.wide) ne += spx.enc_fsize;
		m += enc;
#ifdef MDBG
		cvoice_logd("Encoded %d input, output is %d", ne, m);
#endif
	}

	cvoice_logd("Finally encoded %d --> %d", n, m);

#ifdef MDBG
	strcpy(str, "");
	tmp[2] = 0;
	for (int i=0;i<m;i++)
	{
	    sprintf(tmp, "%02x", (unsigned char)out[i]);
	    strcat(str, tmp);
	}
	cvoice_logd("Frame encoded(%d): %s", m, str); 
#endif

#else
	memcpy((void*)out, (void*)in, n);
	m = n;
#endif
	return 0;
}

int cvoice_mspeex_decode( char* in, int n, unsigned char* out, int& m)    
{
#ifdef MSPX
    	register int dec, x;
	int nd, nt, ds;
	unsigned char fs;
	register float enh, s;

	cvoice_logd("Speex decoding %d bytes", n);

#ifdef MDBG
	char str[800], tmp[3];
	strcpy(str, "");
	tmp[2] = 0;
	for (int i=0;i<n;i++)
	{
	    sprintf(tmp, "%02x", (unsigned char)in[i]);
	    strcat(str, tmp);
	}
	cvoice_logd("ToDecode(%d): %s", n, str);
#endif

	m = 0;
	if (n <= 0) return 1;

	nd = 0;
	enh = cfg.enh_w;

	while (nd < n)
	{
		memcpy((void*)(&fs), (void*)&in[nd], 1);

		ds = (int)fs;
#ifdef MDBG
		cvoice_logd("Frame to decode size: %d", ds);
#endif

		if (ds < 1 || ds > n)
		{
			cvoice_log("Error geting speex decoded frame size, size is: %d, n=%d", ds, n);
			return 1;
		}

		nd ++;
		nt = nd + ds;

		speex_bits_read_from(&spx.dec_bits, &in[nd], ds);
		dec = speex_decode_int(spx.dec_state, &spx.dec_bits, spx.output);
		if (dec < 0)
		{
			cvoice_log("Error decoding frame: <%d, %d>, dec=%d", nd, nt, dec);
			return 1;
		}

		dec = spx.dec_fsize;

		if (cfg.wide)
		{
			dec *= 2;
			x = 0;
			for (int i=0;i<dec;i+=2)
			{
				s = float(spx.output[x]) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.output[x] = short(s);
				memcpy((void*)&out[m+i], (void*)&spx.output[x], 2);
#ifdef MDBG
				cvoice_logd("SampleDec: %d --> %d,%d", spx.output[x], out[i+m], out[i+m+1]);
#endif
				x++;
			}
		}
		else
		{
			for (int i=0;i<dec;i++)
			{
				s = float(spx.output[i]) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.output[i] = short(s);
				out[i+m] = (spx.output[i] >> 8) + 0x80;
#ifdef MDBG
				cvoice_logd("SampleDec: %d --> %d", spx.output[i], out[i+m]);
#endif
			}
		}

#ifdef MDBG
		cvoice_logd("Decoded frame: <%d, %d> --> %d --> %d", nd, nt, ds, dec);
#endif
		nd += ds;
		m += dec;
#ifdef MDBG
		cvoice_logd("Decoded %d input, output is %d", nd, m);
#endif
		
	}

	cvoice_logd("Finally decoded %d --> %d", n, m);

#ifdef MDBG
	strcpy(str, "");
	tmp[2] = 0;
	for (int i=0;i<16;i++)
	{
	    sprintf(tmp, "%02x", out[i]);
	    strcat(str, tmp);
	}
	strcat(str, "...");
	cvoice_logd("Frame decoded: %s", str); 
#endif

#else
	memcpy((void*)out, (void*)in, n);
	m = n;
#endif
	return 0;
}

void transfer_finalize(int n, int sock, int audio, char* buff, unsigned char* cdata, int dir)
{
	if (n < 0) cvoice_log(strerror( errno ));

	if (sock > 0) close(sock);
	if (audio > 0) close(audio);

	if (!dir) cvoice_mspeex_encode_destroy();
	else cvoice_mspeex_decode_destroy();

	delete [] buff;
	delete [] cdata;
#ifdef MWIN

	if (dir) windsp_close(0);
	else windsp_close(1);
#endif
}

void transfer_finalize(int n, int sock, int audio, unsigned char* buff, char* cdata, int dir)
{
	//int tmp;

	if (n < 0) cvoice_log(strerror( errno ));

	//tmp = 0;
	//memcpy((void*)buff, (void*)(&tmp), 4);
	//my_send(sock, buff, 4);
	//my_recv(sock, buff, 1);
	//
	if (sock > 0) close(sock);
	if (audio > 0) close(audio);

	if (!dir) cvoice_mspeex_encode_destroy();
	else cvoice_mspeex_decode_destroy();

	delete [] buff;
	delete [] cdata;
#ifdef MWIN

	if (dir) windsp_close(0);
	else windsp_close(1);
#endif
}

void* thread_play(void* ptr)
{
    	int n, bn, m, err, sock, audio, l;
    	char* buff;
    	unsigned char* udata;
    	thread_data_play* td;

	cvoice_logd("thread: play start");

    	td = (thread_data_play*)ptr;

    	n    = td->n;
	bn   = td->bn;
    	buff = td->buff;
	sock = td->sock;
	audio = td->audio;
	l = n;

    	delete td;

	//printf("N=%d UDATA=%p SOCK=%d AUDIO=%d BUFF=%p\n", 
	//	n, udata, sock, audio, buff);
	
	udata = new unsigned char[bn];

	mwait();
	err = cvoice_mspeex_decode( buff, n, udata, m );
	//printf("err=%d, m=%d\n", err, m);

	if (m <= 0 || err !=0)
	{
		cvoice_log("Speex decoder error: err=%d, %d -> %d, skipping frame", err, n, m);
		m = 1;
		udata[0] = 0;
		//printf("error.1!\n");
	}

	if (cfg.save) my_save( buff, n, 1, getpid());

	if (cfg.timeout > 0) alarm(cfg.timeout);
#ifdef MWIN
	mwaitaudio();
	err = windsp_write((char*)udata, m);
	msignalaudio();
	if (err != 0)
	{
		transfer_finalize(n, sock, audio, buff, udata, 1);
		msignal();
		if (cfg.timeout > 0) alarm(0);
		cvoice_error("Win play buffer failed: %d", err);
	}
	n = m;
#else
	n = my_write(audio, udata, m);
#endif
	if (cfg.timeout > 0) alarm(0);
	msignal();

	// __mt__
	if (n <= 0)
	//if (n < m)
	{
		cvoice_log(strerror( errno ));
	    	//printf("error.2\n");
		transfer_finalize(n, sock, audio, buff, udata, 1);
		cvoice_error("Write audio data w failed: n=%d, m=%d", n, m);
	}

	delete [] buff;
	delete [] udata;

	n_recieved += l + 2;
	////n_sent  += 2;
	n_write += m;
	n_sent ++;

	//cvoice_log(">> thread: reciever: n_steps = %d", n_steps);
	if (!(n_steps % 600)) traffic_stats(cfg.debug);
#ifdef MIPC
	mipc_stats();
#endif

	cvoice_logd("Reciever thread: net recieved %d, net sent %d, audio writted %d", n_recieved, n_sent, n_write);
	//printf("Reciever: net recieved %d, net sent %d, audio writted %d\n", n_recieved, n_sent, n_write);

	cvoice_logd("thread: play end");
#ifdef MMT
	pthread_exit(NULL);
#endif
	return NULL;
}

int my_play_mt(int sock, int audio, char* buff, int n, int bn)
{
#ifdef MMT
    	thread_data_play* td;
	cvoice_logd("my_play_mt: %d", n);

	td = new thread_data_play;

	td->n     = n;
	td->bn    = bn;
	td->sock  = sock;
	td->audio = audio;
	td->buff  = new char[n];
	memcpy(td->buff, (void*)buff, n);

	//printf("N=%d UDATA=%p SOCK=%d AUDIO=%d BUFF=%p\n", 
	//	td->n, td->udata, td->sock, td->audio, td->buff);

 	if (pthread_create(&tr_play, NULL, thread_play, (void*)td)) 
	//if (0)
	{
	    cvoice_error("creating pthread play");
	    return 0;
	}

	set_tr_play.insert( tr_play );
	
	//thread_play( td );

	have_tr_play = true;
	//pthread_yield();
	cvoice_logd("thread play spawned");
#endif	
	return 1;
}

void input_reciever(int sock)
{
	// Speaker reciever
	int audio;

	init_mutex();

#ifdef MWIN
	int e;
	audio = -1;

	cvoice_log("Voice target: win sound");

	windsp_init();
	if (cfg.wide) e = windsp_open(0, 1, 16, cfg.hz, cfg.cache_size);
	else e = windsp_open(0, 1, 8, cfg.hz, cfg.cache_size);
	
	if (e != 0)
	{
		close(sock);
		cvoice_error("Open win audio w failed: %d", e);
	}

	cvoice_log("Win audio player opened and configured");

#else
	int afmt, tmp;

	cvoice_log("Voice target: %s", cfg.onum);

	audio = open(cfg.onum, O_WRONLY, 0);
	if (audio < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Open audio w failed");
	}

	if (cfg.wide) afmt = AFMT_S16_LE;
	else afmt = AFMT_U8;

	tmp = afmt;
	if (ioctl(audio, SNDCTL_DSP_SETFMT, &tmp) < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Set afmt audio w failed");
	}

	if (tmp != afmt)
	{
	    	close(sock);
	    	cvoice_error("Failed to set requested format w: %d<>%d", tmp, afmt);
	}

	tmp = 1;
	if (ioctl(audio, SNDCTL_DSP_CHANNELS, &tmp) < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Set achn audio w failed");
	}

	if (tmp != 1)
	{
	    	close(sock);
	    	cvoice_error("Failed to set requested channels w: %d<>%d", tmp, 1);
	}

	tmp = cfg.hz;
	if (ioctl(audio, SNDCTL_DSP_SPEED, &tmp) < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Set aspd audio w failed");
	}

	if (tmp != cfg.hz)
	{
	    	close(sock);
	    	cvoice_error("Failed to set requested sampling rate w: %d<>%d", tmp, cfg.hz);
	}

#ifndef MMT
	tmp = 0;
	if (ioctl(audio, SNDCTL_DSP_NONBLOCK, &tmp) < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Set block mode w failed");
	}

	if (tmp != 0)
	{
	    	close(sock);
	    	cvoice_error("Failed to set requested block mode: %d<>0", tmp);
	}
#endif

	cvoice_log("Speaker device %s opened and configured", cfg.onum);
#endif

	if (cvoice_mspeex_decode_init() != 0)
	{
		close(sock);
		cvoice_error("Error initializing speex decoder");
	}

	cvoice_log("Begining audio recieving...");

#ifndef MMT
	int err, m;
	int mpid = getpid();
#endif

	int n;
	int bn = cfg.cache_size * APCK;
	unsigned char *udata;
	char* buff;
	unsigned short l;

	if (cfg.wide) bn *= 2;
	cvoice_log("Final write cache: %d bytes", bn);

	n_steps = 0;

	n_read = 0;
	n_write = 0;
	n_sent = 0;
	n_recieved = 0;

	buff = new char[bn];
	udata = new unsigned char[bn];

	if (cfg.timeout < 0) cfg.timeout *= -1;

	time(&time_start);
	cvoice_log("Start time is: %d, timeout: %d", time_start, cfg.timeout);

	while (1)
	{
		////udata[0] = '>';
		////my_send(sock, udata, 1);
		// __mt__?
		if (n_steps && cfg.n_sync && !(n_steps % cfg.n_sync)) 
		{
#ifdef MMT
		    if (have_tr_send) join_threads( set_tr_send );
#endif
		    cvoice_logd("sending sync");
		    //cvoice_log(">> sending sync");
		    my_send_mt(sock, udata, 1);
		    cvoice_logd("synced");
		    //my_send(sock, udata, 1);
		    //cvoice_log(">> sent sync");
		}

	    	//printf("recieving...\n");
		//cvoice_log(">> recieving size");
		n = my_recv(sock, buff, 2);
		//cvoice_log(">> recieved size");
	    	//printf("recieved: %d\n", n);
		if (n <= 0)
		{
			transfer_finalize(n, sock, audio, buff, udata, 1);
			cvoice_error("Read audio data r failed: n=%d", n);
		}

		memcpy((void*)&l, (void*)buff, sizeof(unsigned short));
		cvoice_logd("Buffer to recieve: %d bytes", l);
		if (l <= 0 || l > bn*2)
		{
			transfer_finalize(l, sock, audio, buff, udata, 1);
			cvoice_error("Recieve buffer too big: %d, cache: %d", l, bn);
		}

		////buff[0] = '>';
		////my_send(sock, buff, 1);

		//cvoice_log(">> recieving buff");
		n = my_recv(sock, buff, l);
		//cvoice_log(">> recieved buff");
		if (n < l)
		{
			transfer_finalize(n, sock, audio, buff, udata, 1);
			cvoice_error("Read audio buffer: %d < %d", n, l);
		}
#ifndef MMT
		if (cfg.save) my_save(buff, l, 1, mpid);

		err = cvoice_mspeex_decode( buff, n, udata, m );
		if (m <= 0 || err !=0)
		{
			cvoice_log("Speex decoder error: err=%d, %d -> %d, skipping frame", err, n, m);
			m = 1;
			udata[0] = 0;
		}

		if (cfg.timeout > 0) alarm(cfg.timeout);
#ifdef MWIN
		mwaitaudio();
		e = windsp_write((char*)udata, m);
		msignalaudio();
		if (e != 0)
		{
			transfer_finalize(n, sock, audio, buff, udata, 1);
			cvoice_error("Win play buffer failed: %d", e);
		}
		n = m;
#else
		n = my_write(audio, udata, m);
#endif
		if (cfg.timeout > 0) alarm(0);
		if (n <= 0)
		{
			transfer_finalize(n, sock, audio, buff, udata, 1);
			cvoice_error("Write audio data w failed: n=%d", n);
		}

		n_recieved += l + 2;
		////n_sent  += 2;
		n_write += m;
		n_sent ++;

		//cvoice_log(">> reciever: n_steps = %d", n_steps);
		n_steps ++;
		if (!(n_steps % 600)) traffic_stats(cfg.debug);
#ifdef MIPC
		mipc_stats();
#endif

		cvoice_logd("Reciever: net recieved %d, net sent %d, audio writted %d", n_recieved, n_sent, n_write);
#else
		if (have_tr_play) join_threads( set_tr_play );
		//printf("sock=%d, audio = %d\n", sock, audio);
		//cvoice_log(">> playing_mt");
		my_play_mt(sock, audio, buff, n, bn);
		n_steps ++;
		//cvoice_log(">> played_mt");
#endif
	}

	// not reached
	if (cvoice_mspeex_decode_destroy() != 0)
	{
		transfer_finalize(-1, sock, audio, buff, udata, 1);
		cvoice_error("Error destroying speex decoder");
	}

	transfer_finalize(-1, sock, audio, buff, udata, 1);
	
	cvoice_clear_mshm();
	my_exit(0);
}

void input_sender(int sock)
{
	// Microphone sender
	int audio;

	init_mutex();

#ifdef MWIN
	int e;
	audio = -1;

	cvoice_log("Recording source: win sound");

	windsp_init();
	if (cfg.wide) e = windsp_open(1, 1, 16, cfg.hz, cfg.cache_size);
	else e = windsp_open(1, 1, 8, cfg.hz, cfg.cache_size);
	
	if (e != 0)
	{
		close(sock);
		cvoice_error("Open win audio r failed: %d", e);
	}

	cvoice_log("Win audio recorder opened and configured");
#else
	int afmt, tmp;

	cvoice_log("Recording source: %s", cfg.inum);

	audio = open(cfg.inum, O_RDONLY, 0);
	if (audio < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Open audio r failed");
	}

	if (cfg.wide) afmt = AFMT_S16_LE;
	else afmt = AFMT_U8;

	tmp = afmt;
	if (ioctl(audio, SNDCTL_DSP_SETFMT, &tmp) < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Set afmt audio r failed");
	}

	if (tmp != afmt)
	{
	    	close(sock);
	    	cvoice_error("Failed to set requested format r: %d<>%d", tmp, afmt);
	}

	tmp = 1;
	if (ioctl(audio, SNDCTL_DSP_CHANNELS, &tmp) < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Set achn audio r failed");
	}

	if (tmp != 1)
	{
	    	close(sock);
	    	cvoice_error("Failed to set requested channels r: %d<>%d", tmp, 1);
	}

	tmp = cfg.hz;
	if (ioctl(audio, SNDCTL_DSP_SPEED, &tmp) < 0)
	{
		cvoice_log(strerror( errno ));
		close(sock);
		cvoice_error("Set aspd audio r failed");
	}

	if (tmp != cfg.hz)
	{
	    	close(sock);
	    	cvoice_error("Failed to set requested sampling rate r: %d<>%d", tmp, cfg.hz);
	}

	cvoice_log("Microphone device %s opened and configured", cfg.inum);
#endif

	if (cvoice_mspeex_encode_init() != 0)
	{
		close(sock);
		cvoice_error("Error initializing speex encoder");
	}

	cvoice_log("Begining audio recording...");

	int err, n, m;
	int bn = cfg.cache_size * APCK;
	unsigned char *buff;
	char *cdata;
	unsigned short sm;
#ifndef MMT
	int mpid = getpid();
#endif

	if (cfg.wide) bn *= 2;

	cvoice_log("Final read cache: %d bytes", bn);

	n_steps = 0;

	n_read = 0;
	n_write = 0;
	n_sent = 0;
	n_recieved = 0;

	buff = new unsigned char[bn];
	cdata = new char[bn+2];

	if (cfg.timeout < 0) cfg.timeout *= -1;

	time(&time_start);
	cvoice_log("Start time is: %d, timeout: %d", time_start, cfg.timeout);

	while (1)
	{
		if (cfg.timeout > 0) alarm(cfg.timeout);
#ifdef MWIN
		mwaitaudio();
		e = windsp_read((char*)buff, n);
		msignalaudio();
		if (e != 0)
		{
			transfer_finalize(n, sock, audio, buff, cdata, 0);
			cvoice_error("Win record buffer failed: %d", e);
		}
#else
		//cvoice_log(">> reading audio");
		//n = read(audio, buff, bn);
		n = my_read(audio, buff, bn);
		//cvoice_log(">> read audio");
#endif
		if (cfg.timeout > 0) alarm(0);
		if(n <= 0)
		//if(n < bn)
		{
			transfer_finalize(n, sock, audio, buff, cdata, 0);
			cvoice_error("Read audio data r failed: n=%d", n);
		}

		cvoice_logd("Read %d audio bytes", n);

		err = cvoice_mspeex_encode( buff, n, &cdata[2], m);
		if (m <= 0 || err != 0)
		{
			cvoice_log("Speex encoder error: err=%d, %d -> %d, skipping frame", err, n, m);
			m = 1;
			cdata[0] = 0;
		}

		if (m > 0x7000) cvoice_log("Warning: near to error, encoded sample size is quite close to max_short 64K");

		sm = (unsigned short)m;
		m += 2;
		memcpy((void*)cdata, (void*)(&sm), 2);
		//my_send(sock, buff, 2);
		////my_recv(sock, buff, 1);
		//
#ifdef MMT
		if (have_tr_send) join_threads( set_tr_send );
#endif
		if (n_steps && cfg.n_sync && !(n_steps % cfg.n_sync)) 
		{
		    //cvoice_log(">> waiting sync");
		    cvoice_logd("waiting for sync");
		    my_recv(sock, buff, 1);
		    cvoice_logd("synced");
		    //cvoice_log(">> synced");
		}

		//cvoice_log(">> sending audio mt");
		n = my_send_mt(sock, cdata, m);
		//cvoice_log(">> sent audio mt");

		if (n < m)
		{
			transfer_finalize(n, sock, audio, buff, cdata, 0);
			cvoice_error("Read audio data r failed: n=%d, m=%d", n, m);
		}

		//__mt__: consider new thread for write
		// in MT mode we dont have cdata computed at this moment
#ifndef MMT
		if (cfg.save) my_save(cdata, sm, 0, mpid);
#endif

		n_read += bn;
		////n_sent += m + 2;
		////n_recieved += 2;
		n_sent += m;
		n_recieved ++;

		//cvoice_log(">> sender: n_steps = %d", n_steps);
		n_steps ++;
		if (!(n_steps % 600)) traffic_stats(cfg.debug);
#ifdef MIPC
		mipc_stats();
#endif

		cvoice_logd("Sender: net recieved %d, net sent %d, audio read %d bytes", n_recieved, n_sent, n_read);
	}


	// not reached
	if (cvoice_mspeex_encode_destroy() != 0)
	{
		transfer_finalize(-1, sock, audio, buff, cdata, 0);
		cvoice_error("Error destroying speex encoder");
	}

	cvoice_clear_mshm();
	my_exit(0);
}


int cvoice_client(int inp)
{
	int pid;
	int sock,n;
	struct sockaddr_in server;

	sprintf(log_pfx, "<client>");

	pid = fork();

	if (pid > 0) return pid;
	else if (pid < 0)
	{
		cvoice_log(strerror( errno ));
		cvoice_error("fork failed");
	}

	sprintf(log_pfx, "<clifork>");

	setup_signals();

	server.sin_family = AF_INET;
	server.sin_port = htons(cfg.port);
	server.sin_addr.s_addr = inet_addr(cfg.ip);

	cvoice_log("Connecting to %s, at port %d", cfg.ip, cfg.port);

	if (server.sin_addr.s_addr == (in_addr_t)(-1)) cvoice_error("IP %s is invalid.\n", cfg.ip);


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)		
	{ 
		cvoice_log(strerror( errno ));
		cvoice_error("create socket failed."); 
	}

	if (connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1)
	{ 
		cvoice_log(strerror( errno ));
		cvoice_error("connect to %s:%d failed", cfg.ip, cfg.port); 
	}

	cfg.cli_sock = sock;
	cvoice_log("Connected, socket: %d", sock);

	sprintf(log_pfx, "<cli%d>", sock);

	char buff[16];

	if (!cfg.exmode)
	{
		cvoice_log("Using internal server mode");

		if (inp)
		{
			cvoice_log("Input mode client");
			strcpy(buff, "I");
		}
		else
		{
			cvoice_log("Output mode client");
			strcpy(buff, "O");
		}

		n = my_send(sock, buff, 1);
		if (n < 1)
		{
			cvoice_log(strerror( errno ));
			close(sock);
			cvoice_error("my_send failed: %d/1", n);
			return 0;
		}

		n = my_recv(sock, buff, 1);
		if (n < 1)
		{
			cvoice_log(strerror( errno ));
			close(sock);
			cvoice_error("my_recv failed: %d/1", n);
			return 0;
		}

	}
	else
	{
		cvoice_log("Using external server mode");

		memcpy((void*)(&buff[0]), (void*)(&cfg.myid), 4);
		memcpy((void*)(&buff[4]), (void*)(&cfg.targetid), 4);

		if (inp)
		{
			cvoice_log("Input mode client");
		    	n = 0;
		    	memcpy((void*)(&buff[8]), (void*)(&n), 4);

		}
		else
		{
			cvoice_log("Output mode client");
		    	n = 1;
		    	memcpy((void*)(&buff[8]), (void*)(&n), 4);
		}

		n = my_send(sock, buff, 12);
		if (n < 12)
		{
			cvoice_log(strerror( errno ));
			close(sock);
			cvoice_error("my_send failed: %d/12", n);
			return 0;
		}
	}

	// data flow starts
	if (inp)
	{
		cvoice_log("Starting client sender");
		input_sender( sock );
	}
	else
	{
		if (!cfg.exmode)
		{
			strcpy(buff, "K");
			n = my_send(sock, buff, 1);
			if (n < 1)
			{
				cvoice_log(strerror( errno ));
				close(sock);
				cvoice_error("my_send failed: %d/1", n);
				return 0;
			}
		}

		cvoice_log("Starting client reciever");
		input_reciever( sock );
	}

	cvoice_log("Client exiting");

	cvoice_clear_mshm();
	my_exit(0);
	// not reached
	return 0;
}

int handle_client(int s)
{
	int pid, n, inp;
	char buff[8];

	pid = fork();

	if (pid > 0) return pid;
	else if (pid < 0)
	{
		cvoice_log(strerror( errno ));
		cvoice_error("fork failed");
	}

	sprintf(log_pfx, "<srv_cli%d>", s);

	setup_signals();

	n = my_recv(s, buff, 1);
	if (n < 1)
	{
		cvoice_log(strerror( errno ));
		close(s);
		cvoice_error("my_recv failed: %d/1", n);
		return 0;
	}

	char cli_type = buff[0];
	cvoice_log("Client type is: '%c'", cli_type);

	if (cli_type == 'i' || cli_type == 'I')
	{
	    cvoice_log("Input client mode");
	    inp = 1;
	}
	else if (cli_type == 'o' || cli_type == 'O')
	{
	    cvoice_log("Output client mode");
	    inp = 0;
	}
	else
	{
	    cvoice_log("Unknown client type");
	    close(s);
	    return 0;
	}

	buff[0] = 'K';
	n = my_send(s, buff, 1);
	if (n < 1)
	{
		cvoice_log(strerror( errno ));
		close(s);
		cvoice_error("my_send failed: %d/1", n);
		return 0;
	}

	if (inp)
	{
		cvoice_log("Starting reciever server");
		input_reciever(s);
	}
	else
	{
		cvoice_log("Starting sender server");
		n = my_recv(s, buff, 1);
		if (n < 1)
		{
			cvoice_log(strerror( errno ));
			close(s);
			cvoice_error("my_recv failedi.2: %d/1", n);
			return 0;
		}

		input_sender(s);
	}

	cvoice_log("Client finished");

	cvoice_clear_mshm();
	my_exit(0);
	// not reached
	return 0;
}

int cvoice_server()
{
	int pid;
	int csock, sock, alive, reuse, err;
	struct sockaddr_in server;

	strcpy(log_pfx, "<server>");

	pid = fork();

	if (pid > 0) return pid;
	else if (pid < 0)
	{
		cvoice_log(strerror( errno ));
		cvoice_error("fork failed");
	}

	strcpy(log_pfx, "<srvfork>");

	setup_signals();

	server.sin_family = AF_INET;
	server.sin_port = htons(cfg.port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == -1)
	{
		cvoice_log(strerror( errno ));
		cvoice_error("Create socket failed.");
	}

	alive=1;

#ifdef MWIN
	err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&alive, sizeof(void*));
#else
	err=setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&alive, sizeof(void*));
#endif

	if (err == -1)
        {
		cvoice_log(strerror( errno ));
		cvoice_error("Set socket options failed: keepalive");
        }

	reuse=1;

#ifdef MWIN
	err=setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(void*));
#else
	err=setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&reuse, sizeof(void*));
#endif

	if (err == -1)
        {
		cvoice_log(strerror( errno ));
		cvoice_error("Set socket options failed: reuseaddr");
        }

	if (bind(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1)
	{
		cvoice_log(strerror( errno ));
		cvoice_error("Bind address failed.");
	}

	cvoice_log("Beginning listen on server socket: %d", sock);

	if (listen(sock,5) == -1)
	{
		cvoice_log(strerror( errno ));
		cvoice_error("Listen on socket failed");
	}

	int running = 1;
	cfg.srv_sock = sock;

	while (running)
	{
		cvoice_log("Wait for client...");

		if ((csock = accept(sock,NULL,NULL)) == -1)
		{
			cvoice_log(strerror( errno ));
			cvoice_log("Accept on socket failed");
		}

		cvoice_log("New client on socket: %d", csock);
		pid = handle_client( csock );
		cvoice_log("New client PID: %d", pid);

		my_waitpid(0, WNOHANG);
	}

	
	cvoice_clear_mshm();
	my_exit(0);
	// no ret
	return 0;
}

void cvoice_forked(int listener, int& pid1, int& pid2)
{
	pid1 = pid2 = 0;

	cvoice_init_mipc(1);

	if (listener)
	{
	    	pid1 = cvoice_server();
	    	cvoice_log("Server PID: %d", pid1);
	    	printf("Server PID: %d\n", pid1);
	}
	else
	{
		pid1 = cvoice_client(1);
		pid2 = cvoice_client(0);
	    	cvoice_log("Client PIDs: %d %d", pid1, pid2);
	    	printf("Client PIDs: %d %d\n", pid1, pid2);
	}

	cvoice_log("Spawned");
}


void cvoice(int listener)
{
	int pid1, pid2;

	cvoice_init_mipc(1);

	if (listener)
	{
	    	pid1 = cvoice_server();
	    	cvoice_log("Server PID: %d", pid1);
	    	printf("Server PID: %d\n", pid1);
	}
	else
	{
		pid1 = cvoice_client(1);
		pid2 = cvoice_client(0);
	    	cvoice_log("Client PIDs: %d %d", pid1, pid2);
	    	printf("Client PIDs: %d %d\n", pid1, pid2);
	}

	cvoice_log("Exiting");
	my_exit(0);
}

void cvoice_clear_mshm()
{
#ifdef MIPC

    double dummy[10];

    if (mshm_ptr) 
    {
    	    for (int i=0;i<10;i++) dummy[i] = 0.0;

	    memcpy(mshm_ptr, (void*)dummy, 10 * sizeof(double));
    }

#endif
}

void cvoice_save_config(FILE* fp, cv_cfg& c)
{
	fprintf(fp, "Input=%s\n", c.inum);
	fprintf(fp, "Output=%s\n", c.onum);
	fprintf(fp, "CacheSize=%d\n", c.cache_size);
	fprintf(fp, "Debug=%d\n", c.debug);
	fprintf(fp, "Quality=%d\n", c.quality);
	fprintf(fp, "Complexity=%d\n", c.complexity);
	fprintf(fp, "Timeout=%d\n", c.timeout);
	fprintf(fp, "Wide=%d\n", c.wide);
	fprintf(fp, "EnchanceR=%.3f\n", c.enh_r);
	fprintf(fp, "EnchanceW=%.3f\n", c.enh_w);
	fprintf(fp, "Save=%d\n", c.save);
	fprintf(fp, "NSync=%d\n", c.n_sync);
	fprintf(fp, "HZ=%d\n", c.hz);
	fprintf(fp, "PIDlogs=%d\n", c.pidlog);
	fprintf(fp, "Port=%d\n", c.port);
	fprintf(fp, "IP=%s\n", c.ip);

	cvoice_log("Configuration saved");
}

void cvoice_load_config(FILE* fp, cv_cfg& c)
{
	fscanf(fp, "Input=%s\n", c.inum);
	fscanf(fp, "Output=%s\n", c.onum);
	fscanf(fp, "CacheSize=%d\n", &c.cache_size);
	fscanf(fp, "Debug=%d\n", &c.debug);
	fscanf(fp, "Quality=%d\n", &c.quality);
	fscanf(fp, "Complexity=%d\n", &c.complexity);
	fscanf(fp, "Timeout=%d\n", &c.timeout);
	fscanf(fp, "Wide=%d\n", &c.wide);
	fscanf(fp, "EnchanceR=%f\n", &c.enh_r);
	fscanf(fp, "EnchanceW=%f\n", &c.enh_w);
	fscanf(fp, "Save=%d\n", &c.save);
	fscanf(fp, "NSync=%d\n", &c.n_sync);
	fscanf(fp, "HZ=%d\n", &c.hz);
	fscanf(fp, "PIDlogs=%d\n", &c.pidlog);
	fscanf(fp, "Port=%d\n", &c.port);
	fscanf(fp, "IP=%s\n", c.ip);

    	cvoice_log("Configuration loaded");
}

int cvoice_save_config(const char* fn, cv_cfg& c)
{
    FILE* fp;

    cvoice_log("Saving config to: %s", fn);

    fp = fopen(fn, "w");
    if (!fp)
    {
	cvoice_log("Cannot write to file: %s", fn);
	return 0;
    }

    cvoice_save_config( fp, c );
    fclose( fp );

    
    cvoice_log("Saved to: %s", fn);
    return 1;
}

int cvoice_load_config(const char* fn, cv_cfg& c)
{
    FILE* fp;

    cvoice_log("Loading config from: %s", fn);

    fp = fopen(fn, "r");
    if (!fp)
    {
    	cvoice_log("Error: cannot open file: %s", fn);
	return 0;
    }

    cvoice_load_config( fp, c );
    fclose( fp );

    cvoice_log("Loaded from: %s", fn);

    return 1;
}

int cvoice_getopt(int lb, char** par)
{
	int is_server;
	char u;
	// a: server mode
	// s: server
	// i: ip
	// p: port 
	// f: myid activates ex mode
	// t: targetid activates ex mode
	// h: help
	// c: cache size
	// d: debug on
	// q: quality
	// T: timeout
	// I: input(microphone) device number
	// O: output(speakers) device number
	// P: use PID in log file name
	// W: use 16bit audio instead of 8bit
	// S: save input/output to mspx files
	// C: set encoder complexity 0-10
	// y: set sync after n cycles
	// H: set sample rate (if other than 8000 Hz)
	// L: load config from file
	// Z: store config to file
	
	time_start = 0;
	is_server = 0;
	strcpy(log_pfx, "<init>");

#ifdef MDBG
	cfg.debug = 1;
#endif

	while ((u = getopt(lb,par,"S8as:i:p:f:t:hc:dq:T:I:O:PR:W:C:y:H:L:Z:"))!=-1)
	{
		switch (u)
		{
			case 'a': is_server = 1; 		break;
			case 's': cvoice_get_host(cfg.ip, optarg);	break;
			case 'i': if (strlen(optarg)<16) strcpy(cfg.ip, optarg); break;
			case 'p': cfg.port = atoi(optarg);	break;
			case 'f': cfg.myid = atoi(optarg);	break;
			case 't': cfg.targetid = atoi(optarg);	break;
			case 'h': cvoice_help(par[0]);		break;
			case 'c': cfg.cache_size = atoi(optarg);break;
			case 'd': cfg.debug = 1;		break;
			case 'q': cfg.quality = atoi(optarg);	break;
			case 'T': cfg.timeout = atoi(optarg);	break;
			case 'I': strcpy(cfg.inum, optarg);	break;
			case 'O': strcpy(cfg.onum, optarg);	break;
			case 'P': cfg.pidlog = 1;		break;
			case '8': cfg.wide = 0;			break;
			case 'R': cfg.enh_r = atof(optarg);	break;
			case 'W': cfg.enh_w = atof(optarg);	break;
			case 'S': cfg.save = 1;			break;
			case 'C': cfg.complexity = atoi(optarg);break;
			case 'y': cfg.n_sync = atoi(optarg);	break;
			case 'H': cfg.hz = atoi(optarg);	break;
			case 'L': 
				if (!cvoice_load_config(optarg, cfg)) 
				{
				    printf("Failed to load configuration from %s\n", optarg);
				    return -1;
				}
				else printf("Configuration loaded from %s\n", optarg);
				break;
			case 'Z': 
				if (!cvoice_save_config(optarg, cfg)) 
				{
				    printf("Failed to save configuration to %s\n", optarg);
				    return -2;
				}
				else printf("Configuration saved to %s\n", optarg);
				break;
			default: printf("Unrecognized option\n"); return -3;
		}
	}

	cvoice_log("Connect is %s:%d", cfg.ip, cfg.port);

	return is_server;
}


