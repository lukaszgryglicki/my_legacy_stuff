#include <libcvoice.h>

void cv_cfg_clear(cv_cfg* c)
{
    	if (!c) return;

	c->inum = (char*)malloc(1024*sizeof(char));
	c->onum = (char*)malloc(1024*sizeof(char));
	c->ip = (char*)malloc(32*sizeof(char));

	strcpy(c->inum, "/dev/dsp");
	strcpy(c->onum, "/dev/dsp");
	strcpy(c->ip, "127.0.0.1");

    	c->myid = c->targetid = 0;

	c->cache_size = 50;	/* wide: in 320b packets, 8bit: in 160b packets	*/
	c->n_sync = 0;
	c->quality = 8;
	c->hz = 8000;
	c->max_oos = 6;	/* out of sync is after max_oos s, 0 to disable */

	c->debug = 0;
	c->complexity = 10;
	c->vbr = 1;
	c->vad = 1;
	c->dtx = 1;
	c->timeout = -60;
	c->exmode = 0;
	c->srv_sock = c->cli_sock = c->sync_sock = 0;
	c->wide = 1;
	c->enh_r = 1.0;
	c->enh_w = 1.0;
	c->save = 0;
	c->port = 9000;
	c->pidlog = 0;

	time(&c->init_tm);

	cvoice_log("Configuration cleared");
}

void cv_cfg_free(cv_cfg* c)
{
    	if (!c) return;

	free(c->inum);
	free(c->onum);
	free(c->ip);

	c->inum = c->onum = c->ip = NULL;
	cvoice_log("Configuration deinitialized");
}

#ifdef MSPX

void speex_cfg_clear(speex_cfg* c)
{
    	if (!c) return;

	c->enc_state = c->dec_state = NULL;
	c->enc_fsize = c->dec_fsize = 0;
	c->enc_osize = c->dec_osize = 0;
	c->input = c->output = NULL;

	cvoice_log("Speex deinitialized");
}

speex_cfg spx;
#endif

#ifdef MMT

pthread_mutex_t mutex;
pthread_t tr_send;
pthread_t tr_play;
pthread_t tr_sync;
thread_struct set_tr_send;
thread_struct set_tr_play;
short have_tr_send;
short have_tr_play;
/*short have_tr_sync;*/

#ifdef MNDPLX
pthread_mutex_t mutex_audio;
#endif

#endif

#ifdef MIPC

int mshmid, msemid;
key_t mshmkey, msemkey;
void*  mshm_ptr;

#endif

cv_cfg cfg;
char log_pfx[32];
time_t time_start;
int n_read, n_write, n_sent, n_recieved, n_steps, n_steps2;

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

void cvoice_logd(int lev, const char* fmt, ...)
{
	FILE* f;
	va_list lst;
	static struct tm *tms;
	time_t t;
	char tstr[128];
	int pid;
	int uid;

	if (cfg.debug < lev) return;

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

void cvoice_mwaitaudio()
{
#ifdef MMT
#ifdef MNDPLX
    	/*printf("waita\n");*/
    	cvoice_logd(2, "wait mutex_audio");
 	pthread_mutex_lock(&mutex_audio);
	cvoice_logd(2, "mutex_audio locked");
#endif
#endif
}

void cvoice_msignalaudio()
{
#ifdef MMT
#ifdef MNDPLX
    	/*printf("signala\n");*/
    	cvoice_logd(2, "unlocking mutex_audio");
 	pthread_mutex_unlock(&mutex_audio);
	cvoice_logd(2, "mutex_audio unlocked");
#endif
#endif
}

void cvoice_mwait()
{
#ifdef MMT
    	cvoice_logd(2, "wait mutex");
 	pthread_mutex_lock(&mutex);
	cvoice_logd(2, "mutex locked");
#endif
}

void cvoice_msignal()
{
#ifdef MMT
    	cvoice_logd(2, "unlocking mutex");
 	pthread_mutex_unlock(&mutex);
	cvoice_logd(2, "mutex unlocked");
#endif
}

void cvoice_init_mipc(int clear)
{
#ifdef MIPC
    	int n, n2;

	mshm_ptr = NULL;
 	mshmkey = 9000;
	msemkey = 9000;

 	if ((mshmid = mshmnew(mshmkey, 1024, &n)) == -1) 
	{
	    cvoice_log("Error creating 1Kb SHM key=%d", mshmkey);
	    return;
	}

 	cvoice_log("SHM key: %d %d", mshmid, mshmidfromkey(mshmkey));

 	if (mshmconn(mshmid, &mshm_ptr) == -1) 
	{
	    cvoice_log("Error connecting shm to local address");
	    mshm_ptr = NULL;
	    return;
	}

    	if (clear) 
    	{
	    cvoice_clear_mshm();
    	}

 	if ((msemid = msemnew(msemkey, 1, &n2)) == -1) 
	{
	    cvoice_log("Error creating process semaphore", msemkey);
	    return;
	}

	cvoice_log("SHM initialized, SHM key: %d, new: %d, SEM key: %d, new: %d", mshmid, n, msemid, n2);

	/*mdropshm(mshmid);*/
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

	have_tr_send = have_tr_play = 0;

	cvoice_logd(1, "mutex created");
#endif
}

#ifdef MMT
void join_threads(thread_struct* thrs)
{
    int i, n;

    if (!thrs) return;

    n = thrs->n;

    if (n < NJOIN) 
    {
	cvoice_logd(2, "Not joining, size is: %d", n);
	return;
    }

    cvoice_logd(2, "Joining %d threads", n);
    for (i=0;i<n;i++)
    {
	pthread_join(thrs->t[i], NULL);
	cvoice_logd(2, "joined %d", i);
    }

    thrs->n = 0;

    cvoice_logd(2, "Joined all");
}
#endif

unsigned short cvoice_timestamp()
{
    time_t tm;

    time(&tm);

    return (unsigned short)(tm - time_start);
}

void mipc_incsteps()
{
#ifdef MIPC
    	double dtmp, bps[10];

	mwait( msemid );
	cvoice_mwait();
#endif
    	n_steps ++;
#ifdef MIPC

	memcpy((void*)bps, mshm_ptr, 10 * sizeof(double));

	dtmp = (double)n_steps;

/*	if (dtmp > bps[7]) bps[7] = dtmp;*/
	if (dtmp > 0) bps[7] = dtmp;

	memcpy(mshm_ptr, (void*)bps, 10 * sizeof(double));

	cvoice_msignal();
	msignal( msemid );
#endif
}

int mipc_getsteps()
{
#ifdef MIPC
	double bps[10];

	mwait( msemid );
	cvoice_mwait();
	memcpy((void*)bps, mshm_ptr, 10 * sizeof(double));
	cvoice_msignal();
	msignal( msemid );

	return (int)(bps[7] + 0.000001);
#else
	return n_steps;
#endif
}

int mipc_getsteps2()
{
#ifdef MIPC
	double bps[10];

	mwait( msemid );
	cvoice_mwait();
	memcpy((void*)bps, mshm_ptr, 10 * sizeof(double));
	cvoice_msignal();
	msignal( msemid );

	return (int)(bps[8] + 0.000001);
#else
	return n_steps2;
#endif
}

void mipc_putsteps(int st)
{
#ifdef MIPC
    	double bps[10];

	mwait( msemid );
	cvoice_mwait();
	memcpy((void*)bps, mshm_ptr, 10 * sizeof(double));
	bps[7] = (double)st;
	memcpy(mshm_ptr, (void*)bps, 10 * sizeof(double));
	cvoice_msignal();
	msignal( msemid );
#endif
}

void mipc_putsteps2(int st2)
{
#ifdef MIPC
    	double bps[10];

	mwait( msemid );
	cvoice_mwait();
	memcpy((void*)bps, mshm_ptr, 10 * sizeof(double));
	bps[8] = (double)st2;
	memcpy(mshm_ptr, (void*)bps, 10 * sizeof(double));
	cvoice_msignal();
	msignal( msemid );
#endif
}

void mipc_stats()
{
#ifdef MIPC
	time_t time_end;
	double tdiff;
	double bps[10];
	double dtmp;

    	if (!mshm_ptr) return;

	time(&time_end);

	tdiff = (double)(time_end - time_start);

	if (tdiff <= 0.0) return;

	if (n_sent > 0 && n_recieved > 0)
	{
	    	/* Current values */
		memcpy((void*)bps, mshm_ptr, 10 * sizeof(double));

	    	/* BPS */
		dtmp   = (double)(n_sent + n_recieved) / tdiff;
		if (dtmp > 0.0) bps[0] = dtmp;

		/* Compression */
		dtmp = (((double)n_recieved + (double)n_sent) * 100.0) / ((double)n_write + (double)n_read);
		if (dtmp > 0.0) bps[1] = dtmp;

		/* Time running */
		dtmp = tdiff;
		if (dtmp > 0.0) bps[2] = dtmp;

		/* Bytes sent */
		dtmp = (double)n_sent;
		if (dtmp > bps[3]) bps[3] = dtmp;

		/* Bytes recieved */
		dtmp = (double)n_recieved;
		if (dtmp > bps[4]) bps[4] = dtmp;

		/* Bytes read from device */
		dtmp = (double)n_read;
		if (dtmp > bps[5]) bps[5] = dtmp;

		/* Bytes written to device */
		dtmp = (double)n_write;
		if (dtmp > bps[6]) bps[6] = dtmp;

		/* Packets processed */
		dtmp = (double)n_steps;
		if (dtmp > bps[7]) bps[7] = dtmp;

		memcpy(mshm_ptr, (void*)bps, 10 * sizeof(double));
	}
#endif
}

void traffic_stats(int out)
{
	time_t time_end;
	double tdiff, bps, bps2;

	cvoice_logd(1, "Time start: %d", time_start);

	if (time_start <= 0) return;

	time(&time_end);

	tdiff = (double)(time_end - time_start);

	if (tdiff <= 0.0) return;

	cvoice_log("Elapsed time %.0f", tdiff);

	if (n_read > 0 && n_sent > 0)
	{
		cvoice_log("Bytes read %d", n_read);
		bps = (double)n_read / tdiff;
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
		bps = (double)n_recieved / tdiff;
		cvoice_log("BPS recieve %f", bps);

		cvoice_log("Bytes written %d", n_write);
		bps = (double)n_write / tdiff;
		cvoice_log("BPS uncompressed %f", bps);

		bps = ((double)n_recieved * 100.0) / (double)n_write;
		cvoice_log("Compression recieved %f%%", bps);
	}

	if (n_sent > 0 && n_recieved > 0)
	{
		bps = (double)(n_sent + n_recieved) / tdiff;
		cvoice_log("Traffic BPS: %f", bps);

		bps2 = (((double)n_recieved + (double)n_sent) * 100.0) / ((double)n_write + (double)n_read);
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
	    	cvoice_log("Error: Network timeout");
	    	cvoice_log("Current operation timeout");
	}
	else if (signo == SIGUSR1)	/* was also SIGINFO */
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
	/*sigaction(SIGINFO, &intact, NULL);*/
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
		"\n\t-T\tNet timeout (-20) 0=off negative enables timeout after connect", name);
	printf(
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
		"\n\t-o\tOut of sync after seconds, 0 to disable"
		"\n");

	my_exit(0);
}


void cvoice_check_config()
{
	int i, ipi[4];

	cvoice_log("Checking configuration...");
	cvoice_log("Requested cache size is: %d packets", cfg.cache_size);

	if (sscanf(cfg.ip, "%d.%d.%d.%d", &ipi[0], &ipi[1], &ipi[2], &ipi[3]) != 4)
	{
	    cvoice_error("Bad IP format: %s", cfg.ip);
	}

	for (i=0;i<4;i++)
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

	cvoice_logd(2, "my_save: %d", l+2);

	if (!dir) sprintf(fn, "input_%d_%d.mspx", pid, (int)cfg.init_tm);
	else sprintf(fn, "output_%d_%d.mspx", pid, (int)cfg.init_tm);

	fp = fopen(fn, "a+");
	if (!fp) return;

	fwrite((void*)&l, sizeof(unsigned short), 1, fp);
	fwrite((void*)buff, l, 1, fp);

	fclose(fp);
}

void my_waitpid(int pid, int opt)
{
	int stat;

	cvoice_logd(2, "Wait for pid %d, option %d", pid, opt);

	if (opt == WNOHANG)
	{
		cvoice_logd(2, "Fast wait");
		waitpid( pid, &stat, opt );
		cvoice_logd(2, "Status returned: %d", stat);
		return;
	}

	if (pid <= 0)
	{
		cvoice_logd(2, "No waiting for 0 PID");
		return;
	}

	if (cfg.timeout> 0) alarm(cfg.timeout);

	waitpid( pid, &stat, opt );

	if (cfg.timeout > 0) alarm(0);

	cvoice_logd(2, "Wait finished");
}

void* thread_sender(void* ptr)
{
    	thread_data_send* td;
    	int sock, n, ns, nsend;
    	char* bstr;

	cvoice_logd(2, "thread: sender start");

    	td = (thread_data_send*)ptr;

    	n = td->l;
    	sock = td->s;
    	bstr = (char*)td->ptr;

	free( (void*)td );

	cvoice_logd(3, "thread sender started: L=%d S=%d PTR=%p", n, sock, bstr);

	nsend = 0;

	cvoice_mwait();
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
		/*printf("%d\n", SLOWDOWN);*/
#endif
		if (ns <= 0)
		{
		    cvoice_msignal();
		    if (cfg.timeout > 0) alarm(0);
		    cvoice_error("send data error: %d", ns);
		}
		nsend += ns;
		cvoice_logd(2, "nsent: %d, ns=%d", nsend, ns);
		/*printf("nsent: %d, ns=%d\n", nsend, ns);*/
	}
	
	cvoice_msignal();
	if (cfg.timeout > 0) alarm(0);

	free( (void*)bstr );
	cvoice_logd(2, "thread sender finished");

	/*cvoice_log(">> thread: sender end");*/

#ifdef MMT
	pthread_exit((void*)nsend);
#endif
	return (void*)nsend;
}

int my_send_mt(int sock, char* bstr, int n)
{
    	thread_data_send* td;
	cvoice_logd(2, "my_send_mt: %d", n);

	td = (thread_data_send*)malloc(sizeof(thread_data_send));

	td->l = n;
	td->s = sock;
	td->ptr = malloc(n * sizeof(char));
	memcpy(td->ptr, (void*)bstr, n);

#ifdef MMT
 	if (pthread_create(&tr_send, NULL, thread_sender, (void*)td)) 
	/*if (0)*/
	{
	    cvoice_error("creating pthread send");
	    return 0;
	}

	set_tr_send.t[set_tr_send.n] = tr_send;
	set_tr_send.n ++;

	/*thread_sender( td );*/

	have_tr_send = 1;
	/*pthread_yield();*/
#else	
	n = (int)thread_sender((void*)td);
#endif
	cvoice_logd(2, "thread sender spawned");
	return n;
}

int my_send_mt_uc(int sock, unsigned char* bstr, int n)
{
    	return my_send_mt(sock, (char*)bstr, n);
}

int my_write(int fd, unsigned char* bstr, int n)
{
	int nw, nwritten = 0;
	int nerr = 0;
	cvoice_logd(2, "my_write: %d", n);

	while (nwritten < n)
	{
	    	cvoice_mwaitaudio();
		nw = write(fd, (void*)(&bstr[nwritten]), n - nwritten);
		cvoice_msignalaudio();
		/*printf("%d/%d/%d\n", nw, nwritten, n);*/
		if (nw <= 0)
		{
		    cvoice_logd(1, "Partial written: %d,%d/%d (%5.2f%%)", 
		    	    nw, nwritten, n, ((float)nwritten * 100.) / (float)n);
		    /*printf("Partial written: %d/%d (%5.2f%%)\n", */
		    /*	    nwritten, n, ((float)nwritten * 100.) / (float)n);*/
		    usleep( 1000 );
		    nerr ++;
		    if (nerr > 100) 
		    {
		    	cvoice_log("Error writing: %d,%d/%d (%5.2f%%) - buffer played truncated", 
		    	    nw, nwritten, n, ((float)nwritten * 100.) / (float)n);
		    	/*printf("Error writing: %d/%d (%5.2f%%) - buffer played truncated\n", */
		    	/*    nwritten, n, ((float)nwritten * 100.) / (float)n);*/
			return nwritten;
		    }
		    continue;
		}
		nwritten += nw;
	}
	
	/*printf("Success write: %d, retries: %d\n", n, nerr);*/
	return nwritten;
}

int my_send(int sock, char* bstr, int n)
{
	int nsend = 0;
	int ns;
	cvoice_logd(2, "my_send: %d", n);

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

int my_send_uc(int sock, unsigned char* bstr, int n)
{
    return my_send(sock, (char*)bstr, n);
}

int my_read(int fd, unsigned char* bstr, int n)
{
	int nread = 0;
	int nr;
	cvoice_logd(2, "my_read: %d", n);

	if (cfg.timeout > 0) alarm(cfg.timeout);

	while (nread < n)
	{
	    	cvoice_mwaitaudio();
	    	nr = read(fd, (void*)(&bstr[nread]), n - nread);
		cvoice_msignalaudio();

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
	cvoice_logd(2, "my_recv: %d", n);

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

int my_recv_uc(int sock, unsigned char* bstr, int n)
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

	spx.input = (short*)malloc(spx.enc_osize*sizeof(short));
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

	spx.output = (short*)malloc(spx.dec_osize*sizeof(short));
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
	free( (void*)spx.input );
#endif
	return 0;
}

int cvoice_mspeex_decode_destroy()
{
#ifdef MSPX
	cvoice_log("Destroying speex decoder");

	speex_decoder_destroy(spx.dec_state);
	speex_bits_destroy(&spx.dec_bits);
	free( (void*)spx.output );
#endif
	return 0;
}

int cvoice_mspeex_encode( unsigned char* in, int n, char* out, int* m)    
{
#ifdef MSPX
	register short sample;
	register int ne, nt, x, i;
	int enc;
	unsigned char fs;
	register float enh, s;

#ifdef MDBG
	char *str, tmp[3];

	str = (char*)malloc((2 * n) + 16);

	strcpy(str, "");
	tmp[2] = 0;
	for (i=0;i<16;i++)
	{
	    sprintf(tmp, "%02x", in[i]);
	    strcat(str, tmp);
	}
	strcat(str, "...");
	cvoice_logd(2, "ToEncode: %s...", str); 
#endif

	cvoice_logd(2, "Speex encoding %d bytes", n);
	*m = 0;
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
			for (i=ne;i<nt;i+=2)
			{
				memcpy((void*)&spx.input[x], (void*)&in[i], 2);
				s = (float)(spx.input[x]) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.input[x] = (short)(s);
				cvoice_logd(3, "SampleEnc: %d,%d --> %d", in[i], in[i+1], spx.input[x]);
				x ++;
			}
		}
		else
		{
			for (i=ne;i<nt;i++)
			{
				/*input[i-ne] = ((short)in[i] << 8) ^ 0x8000;*/
				sample = (short)in[i];
				sample = (sample - 128) << 8;
				s = (float)(sample) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.input[i-ne] = (short)(s);
				cvoice_logd(3, "SampleEnc: %d --> %d", in[i], spx.input[i-ne]);
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
		cvoice_logd(2, "U8 size is: %d", fs);

		memcpy((void*)(&out[*m]), (void*)&fs, 1);
		(*m) ++;

		enc = speex_bits_write(&spx.enc_bits, &out[*m], spx.enc_osize);

		if (enc <= 0)
		{
			cvoice_log("Error encoding frame: <%d, %d> enc=%d", ne, nt, enc);
			return 1;
		}

		cvoice_logd(2, "Encoded: frame <%d, %d>: %d --> %d", ne, nt, spx.enc_fsize, enc);
		ne += spx.enc_fsize;
		if (cfg.wide) ne += spx.enc_fsize;
		*m += enc;
		cvoice_logd(2, "Encoded %d input, output is %d", ne, *m);
	}

	cvoice_logd(1, "Finally encoded %d --> %d", n, *m);

#ifdef MDBG
	strcpy(str, "");
	tmp[2] = 0;
	for (i=0;i<*m;i++)
	{
	    sprintf(tmp, "%02x", (unsigned char)out[i]);
	    strcat(str, tmp);
	}
	cvoice_logd(2, "Frame encoded(%d): %s", *m, str); 

	free((void*)str);
#endif

#else
	memcpy((void*)out, (void*)in, n);
	*m = n;
#endif
	return 0;
}

int cvoice_mspeex_decode( char* in, int n, unsigned char* out, int* m)    
{
#ifdef MSPX
    	register int dec, x, i;
	int nd, nt, ds;
	unsigned char fs;
	register float enh, s;

#ifdef MDBG
	char *str, tmp[3];

	str = (char*)malloc((2 * cfg.cache_size * APCK) + 16);

	strcpy(str, "");
	tmp[2] = 0;
	for (i=0;i<n;i++)
	{
	    sprintf(tmp, "%02x", (unsigned char)in[i]);
	    strcat(str, tmp);
	}
	cvoice_logd(2, "ToDecode(%d): %s", n, str);
#endif

	cvoice_logd(2, "Speex decoding %d bytes", n);
	*m = 0;
	if (n <= 0) return 1;

	nd = 0;
	enh = cfg.enh_w;

	while (nd < n)
	{
		memcpy((void*)(&fs), (void*)&in[nd], 1);

		ds = (int)fs;
		cvoice_logd(2, "Frame to decode size: %d", ds);

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
			for (i=0;i<dec;i+=2)
			{
				s = (float)(spx.output[x]) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.output[x] = (short)(s);
				memcpy((void*)&out[(*m)+i], (void*)&spx.output[x], 2);
				cvoice_logd(3, "SampleDec: %d --> %d,%d", spx.output[x], out[i+(*m)], out[i+(*m)+1]);
				x++;
			}
		}
		else
		{
			for (i=0;i<dec;i++)
			{
				s = (float)(spx.output[i]) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.output[i] = (short)(s);
				out[i+(*m)] = (spx.output[i] >> 8) + 0x80;
				cvoice_logd(3, "SampleDec: %d --> %d", spx.output[i], out[i+(*m)]);
			}
		}

		cvoice_logd(2, "Decoded frame: <%d, %d> --> %d --> %d", nd, nt, ds, dec);
		nd += ds;
		*m += dec;
		cvoice_logd(2, "Decoded %d input, output is %d", nd, *m);
		
	}

	cvoice_logd(1, "Finally decoded %d --> %d", n, *m);

#ifdef MDBG
	strcpy(str, "");
	tmp[2] = 0;
	for (i=0;i<16;i++)
	{
	    sprintf(tmp, "%02x", out[i]);
	    strcat(str, tmp);
	}
	strcat(str, "...");
	cvoice_logd(2, "Frame decoded: %s", str); 

	free((void*)str);
#endif

#else
	memcpy((void*)out, (void*)in, n);
	*m = n;
#endif
	return 0;
}

void transfer_finalize(int n, int sock, int audio, char* buff, unsigned char* cdata, int dir)
{
    	cvoice_log("Finalizing transfer on fd %d", sock);
	if (n < 0) cvoice_log(strerror( errno ));

	if (sock > 0) close(sock);
	if (audio > 0) close(audio);

	if (!dir) cvoice_mspeex_encode_destroy();
	else cvoice_mspeex_decode_destroy();

	free( (void*)buff );
	free( (void*)cdata );
#ifdef MWIN

	if (dir) windsp_close(0);
	else windsp_close(1);
#endif
}

void transfer_finalize_sw(int n, int sock, int audio, unsigned char* buff, char* cdata, int dir)
{
	/*int tmp;*/
    	cvoice_log("Finalizing transfer on fd %d", sock);

	if (n < 0) cvoice_log(strerror( errno ));

	/*
	tmp = 0;
	memcpy((void*)buff, (void*)(&tmp), 4);
	my_send(sock, buff, 4);
	my_recv(sock, buff, 1);
	*/
	if (sock > 0) close(sock);
	if (audio > 0) close(audio);

	if (!dir) cvoice_mspeex_encode_destroy();
	else cvoice_mspeex_decode_destroy();

	free( (void*)buff );
	free( (void*)cdata );
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

	cvoice_logd(2, "thread: play start");

    	td = (thread_data_play*)ptr;

    	n    = td->n;
	bn   = td->bn;
    	buff = td->buff;
	sock = td->sock;
	audio = td->audio;
	l = n;

    	free( (void*)td );

	/*printf("N=%d UDATA=%p SOCK=%d AUDIO=%d BUFF=%p\n", */
	/*	n, udata, sock, audio, buff);*/
	
	udata = (unsigned char*)malloc(bn * sizeof(unsigned char));

	cvoice_mwait();
	err = cvoice_mspeex_decode( buff, n, udata, &m );
	/*printf("err=%d, m=%d\n", err, m);*/

	if (m <= 0 || err !=0)
	{
		cvoice_log("Speex decoder error: err=%d, %d -> %d, skipping frame", err, n, m);
		m = 1;
		udata[0] = 0;
		/*printf("error.1!\n");*/
	}

	if (cfg.save) my_save( buff, n, 1, getpid());

	if (cfg.timeout > 0) alarm(cfg.timeout);
#ifdef MWIN
	cvoice_mwaitaudio();
	err = windsp_write((char*)udata, m);
	cvoice_msignalaudio();
	if (err != 0)
	{
		transfer_finalize(n, sock, audio, buff, udata, 1);
		cvoice_msignal();
		if (cfg.timeout > 0) alarm(0);
		cvoice_error("Win play buffer failed: %d", err);
	}
	n = m;
#else
	n = my_write(audio, udata, m);
#endif
	if (cfg.timeout > 0) alarm(0);
	cvoice_msignal();

	/* __mt__ */
	if (n <= 0)
	/*if (n < m)*/
	{
		cvoice_log(strerror( errno ));
	    	/*printf("error.2\n");*/
		transfer_finalize(n, sock, audio, buff, udata, 1);
		cvoice_error("Write audio data w failed: n=%d, m=%d", n, m);
	}

	free( (void*)buff );
	free ( (void*)udata );

	n_recieved += l + 4;
	/*n_sent  += 2;*/
	n_write += m;
	n_sent ++;

	/*cvoice_log(">> thread: reciever: n_steps = %d", n_steps);*/
	if (!(n_steps % NSTEPSSTATS)) 
	{
#ifdef MDBG
	    traffic_stats(1);
#else
	    traffic_stats(0);
#endif
	}

#ifdef MIPC
	mipc_stats();
#endif

	cvoice_logd(2, "Reciever thread: net recieved %d, net sent %d, audio writted %d", n_recieved, n_sent, n_write);
	/*printf("Reciever: net recieved %d, net sent %d, audio writted %d\n", n_recieved, n_sent, n_write);*/

	cvoice_logd(2, "thread: play end");
#ifdef MMT
	pthread_exit(NULL);
#endif
	return NULL;
}

int my_play_mt(int sock, int audio, char* buff, int n, int bn)
{
#ifdef MMT
    	thread_data_play* td;
	cvoice_logd(2, "my_play_mt: %d", n);

	td = (thread_data_play*)malloc(sizeof(thread_data_play));

	td->n     = n;
	td->bn    = bn;
	td->sock  = sock;
	td->audio = audio;
	td->buff  = (char*)malloc(n*sizeof(char));
	memcpy(td->buff, (void*)buff, n);

	/*printf("N=%d UDATA=%p SOCK=%d AUDIO=%d BUFF=%p\n", */
	/*	td->n, td->udata, td->sock, td->audio, td->buff);*/

 	if (pthread_create(&tr_play, NULL, thread_play, (void*)td)) 
	/*if (0)*/
	{
	    cvoice_error("creating pthread play");
	    return 0;
	}

	set_tr_play.t[ set_tr_play.n ] = tr_play;
	set_tr_play.n ++;
	
	/*thread_play( td );*/

	have_tr_play = 1;
	/*pthread_yield();*/
	cvoice_logd(2, "thread play spawned");
#endif	
	return 1;
}

void sync_sender(int s)
{
    char buff[5];
    int n, d, t, bs, i;
    unsigned short sd;
    time_t ts, te;
    double bps;

    cvoice_log("Sync sender started");

    t = cfg.cache_size * 6000;
    if (t < 100000) t = 100000;
    if (t > 2000000) t = 2000000;

    buff[4] = 0;
    bs = 0;
    i = 0;
    usleep( SYNCSETTLE );

    cvoice_log("Syncer sender sleep period is: %d ms", t / 1000);
    time( &ts );

    while(1)
    {
	usleep( t );

	d = mipc_getsteps();
	if (d <= 0) 
	{
	    cvoice_logd(1, "No steps made so far");
	    continue;
	}

	sd = (unsigned short)d;
	
	memcpy((void*)buff, (void*)&d, 2);

	n = my_send(s, buff, 2);
	cvoice_logd(2, "sync sent: %d", n);

	if (n < 2)
	{
		cvoice_log(strerror( errno ));
		close(s);
		cvoice_log("my_send failed in sync sender: %d/1", n);
		return;
	}
	else bs += 2;

	i ++;
	time( &te );
	if (te > ts)
	{
		bps = (double)bs / ((double)te - (double)ts);
		cvoice_logd(1, "Syncer sender SBPS: %6.3f", bps);
		if (i && !(i % 100)) cvoice_log("Syncer sender SBPS: %6.3f", bps);
	}

    }
}

void sync_reciever(int s)
{
    char buff[5];
    int n, d, t, bs, i;
    unsigned short sd;
    time_t ts, te;
    double bps;

    cvoice_log("Sync reciever started");

    t = cfg.cache_size * 6000;
    if (t < 100000) t = 100000;
    if (t > 2000000) t = 2000000;

    buff[4] = 0;
    bs = 0;
    i = 0;
    usleep( SYNCSETTLE );

    /* only one wait */
    usleep( t * 2 );

    cvoice_log("Syncer reciever settle sleep is: %d ms", t / 1000);
    time( &ts );

    while(1)
    {
	/* Reader doesnt need to wait, socket will buffer it */
/*	usleep( t );*/
	
	n = my_recv(s, buff, 2);
	cvoice_logd(2, "sync recv: %d", n);

	if (n < 2)
	{
		cvoice_log(strerror( errno ));
		close(s);
		cvoice_log("my_recv failed: %d/1", n);
		return;
	}
	else bs += 2;

	memcpy((void*)&sd, (void*)buff, 2);
	n_steps2 = (int)sd;
	mipc_putsteps2( n_steps2 );
	d = mipc_getsteps();

	i ++;
	time( &te );
	if (te > ts)
	{
		bps = (double)bs / ((double)te - (double)ts);
		cvoice_logd(1, "Steps sync: (%d, %d), Syncer reciever SBPS: %6.3f", d, n_steps2, bps);
		if (i && !(i % 100)) cvoice_log("Steps sync: (%d, %d), Syncer reciever SBPS: %6.3f", d, n_steps2, bps);
	}
    }
}

void* thread_sync(void* ptr)
{
    	int inp, s, tmp, n;
	char buff[2];

	buff[1] = 0;
/*	sprintf(log_pfx, "<syncer>");*/

	cvoice_log("Thread: sync start");

    	tmp = (int)ptr;

	inp = tmp % 2;
	s = tmp >> 1;

	cvoice_log("syncer: input=%d, sock=%d", inp, s);
/*	sprintf(log_pfx, "<syncer%c%d>", inp?'I':'O', s);*/

	if (inp)
	{
		cvoice_log("Input sync client");
		strcpy(buff, "J");
	}
	else
	{
		cvoice_log("Output sync client");
		strcpy(buff, "P");
	}

	n = my_send(s, buff, 1);
	if (n < 1)
	{
		cvoice_log(strerror( errno ));
		close(s);
		cvoice_log("Error: my_send failed in syncer: %d/1 - non fatal", n);
		return NULL;
	}

	n = my_recv(s, buff, 1);
	if (n < 1)
	{
		cvoice_log(strerror( errno ));
		close(s);
		cvoice_log("Error: my_recv failed in syncer: %d/1 - non fatal", n);
		return NULL;
	}

	/* syncer starts */
	if (!inp)
	{
		cvoice_log("Starting syncer sender");
		sync_sender( s );
	}
	else
	{
		cvoice_log("Starting syncer reciever.1");
		strcpy(buff, "K");
		n = my_send(s, buff, 1);
		if (n < 1)
		{
			cvoice_log(strerror( errno ));
			close(s);
			cvoice_log("Error: my_send failed in syncer: %d/1 - non fatal", n);
			return NULL;
		}

		cvoice_log("Starting syncer reciever.2");
		sync_reciever( s );
	}

	return NULL;
}


int start_thread_syncer(int inp, int s)
{
#ifdef MMT
/*    	int pid;*/
    	cvoice_log("Spawning to syncer: input=%d, socket=%d", inp, s);

	s = s << 1;
	if (inp) s ++;

	/*
	pid = fork();

	if (pid > 0) return pid;
	else if (pid < 0)
	{
		cvoice_log(strerror( errno ));
		cvoice_error("fork failed");
	}

	return (int)thread_sync((void*)s);
	*/

 	if (pthread_create(&tr_sync, NULL, thread_sync, (void*)s)) 
	{
	    cvoice_log("Error creating pthread syncer - non fatal");
	}
	
	return (int)tr_sync;
#else
	return 0;
#endif
}


void input_reciever(int sock, int sync)
{
	/* Speaker reciever */
	int audio;
	int afmt, tmp;
	int n;
	int bn;
	unsigned char *udata;
	char* buff;
	unsigned short l, tstamp1, tstamp2;
	int oos, oos2, nmax;
	int spid;
#ifndef MMT
	int err, m;
	int mpid;
#endif

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
	mpid = getpid();
#endif

	bn = cfg.cache_size * APCK;

	if (cfg.wide) bn *= 2;

	cvoice_log("Final write cache: %d bytes", bn);

	n_steps = 0;
	n_steps2 = 0;

	n_read = 0;
	n_write = 0;
	n_sent = 0;
	n_recieved = 0;

	buff = (char*)malloc(bn*sizeof(char));
	udata = (unsigned char*)malloc(bn*sizeof(unsigned char));

	if (cfg.timeout < 0) cfg.timeout *= -1;

	if (sync)
	{
		cvoice_log("Starting output syncer thread");
		spid = start_thread_syncer( 0, sync );
		cvoice_log("Syncer thread is %d", spid);
	}

	nmax = (cfg.cache_size / 15) + 1;
	if (nmax < 2) nmax = 2;

	time(&time_start);
	cvoice_log("Start time is: %d, timeout: %d, max player desync: %d", time_start, cfg.timeout, nmax);

	while (1)
	{
		n_steps2 = mipc_getsteps2();
		if (!cfg.n_sync && n_steps > 0 && n_steps2 > 0)
		{
			oos2 = (n_steps % 0x8000) - (n_steps2 % 0x8000);
			cvoice_logd(1, "Syncer player state: %d (%d, %d)", oos2, n_steps, n_steps2);
			if (oos2 > 0x4000) oos2 = 0;

			/* Odtworzylismy duzo mniej niz wyslano */
			/*
			if (oos2 < -5) 			
			{
			    	cvoice_log("Syncer: player going to skip buffer: I'm out of %d packets", oos2);
			    	mipc_incsteps();
				if (!(n_steps % NSTEPSSTATS)) 
				{
#ifdef MDBG
				    	traffic_stats(1);
#else
				    	traffic_stats(0);
#endif
				}
			    	continue;
			}
			*/
			/* Dziwne, wyglada jakbysmy sie zapedzili... */
			/*
			if (oos2 > 2) 
			{
			    oos2 = 5000 * cfg.cache_size * (oos2 - 2);
			    cvoice_log("Syncer: player going to sleep for %d ms", oos2 / 1000);
			    usleep( oos2 );
			}
			*/
		}

	        oos = 0;
		/*udata[0] = '>';*/
		/*my_send(sock, udata, 1);*/
		/* __mt__? */
	        tstamp1 = cvoice_timestamp();

		if (n_steps && cfg.n_sync && !(n_steps % cfg.n_sync)) 
		{
#ifdef MMT
		    if (have_tr_send) join_threads( &set_tr_send );
#endif
		    cvoice_logd(1, "sending sync: steps made: %d", n_steps);
		    memcpy((void*)udata, (void*)(&tstamp1), sizeof(unsigned short));
		    /*cvoice_log(">> sending sync");*/
		    my_send_mt_uc(sock, udata, 2);
		    cvoice_logd(1, "synced");
		    n_sent += 2;

		    /*my_send(sock, udata, 1);*/
		    /*cvoice_log(">> sent sync");*/
		}

	    	/*printf("recieving...\n");*/
		/*cvoice_log(">> recieving size");*/
		n = my_recv(sock, buff, 4);
		/*cvoice_log(">> recieved size");*/
	    	/*printf("recieved: %d\n", n);*/
		if (n <= 0)
		{
			transfer_finalize(n, sock, audio, buff, udata, 1);
			cvoice_error("Read audio data r failed: n=%d", n);
		}

		memcpy((void*)&l, (void*)buff, sizeof(unsigned short));
		memcpy((void*)(&tstamp2), (void*)(&buff[2]), sizeof(unsigned short));

		oos = abs(tstamp1 - tstamp2);
/*		cvoice_log("Reciever sync ratio %d: %d -- %d", oos, tstamp1, tstamp2);*/

		cvoice_logd(1, "Buffer to recieve: %d bytes", l);
		if (l > bn*2)
		{
			transfer_finalize(l, sock, audio, buff, udata, 1);
			cvoice_error("Recieve buffer too big: %d, cache: %d", l, bn);
		}

		if (l == 0)
		{
			cvoice_log("Reciever, zero byte buffer, attempt to sync");
			mipc_incsteps();
			n_recieved +=  4;
			n_sent ++;	/* to force correct statistics */
			if (!(n_steps % NSTEPSSTATS)) 
			{
#ifdef MDBG
			    traffic_stats(1);
#else
			    traffic_stats(0);
#endif
			}
			continue;
		}

		/*buff[0] = '>';*/
		/*my_send(sock, buff, 1);*/

		/*cvoice_log(">> recieving buff");*/
		n = my_recv(sock, buff, l);
		/*cvoice_log(">> recieved buff");*/
		if (n < l)
		{
			transfer_finalize(n, sock, audio, buff, udata, 1);
			cvoice_error("Read audio buffer: %d < %d", n, l);
		}

		if (cfg.max_oos && cfg.n_sync && oos > cfg.max_oos)
		{
		        cvoice_log("OOS> Reciever out of sync is %d, limit is %d", oos, cfg.max_oos);
/*			mipc_incsteps() ++;*/
/*			n_recieved += l + 4;*/
/*			n_sent ++;	*/
/*			if (!(n_steps % NSTEPSSTATS)) traffic_stats(cfg.debug);*/
/*			continue;*/
		}
#ifndef MMT
		if (cfg.save) my_save(buff, l, 1, mpid);

		err = cvoice_mspeex_decode( buff, n, udata, &m );
		if (m <= 0 || err !=0)
		{
			cvoice_log("Speex decoder error: err=%d, %d -> %d, skipping frame", err, n, m);
			m = 1;
			udata[0] = 0;
		}

		if (cfg.timeout > 0) alarm(cfg.timeout);
#ifdef MWIN
		cvoice_mwaitaudio();
		e = windsp_write((char*)udata, m);
		cvoice_msignalaudio();
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

		n_recieved += l + 4;
		/*n_sent  += 2;*/
		n_write += m;
		n_sent ++;	/* to force correct statistics */

		/*cvoice_log(">> reciever: n_steps = %d", n_steps);*/
		mipc_incsteps();
		if (!(n_steps % NSTEPSSTATS)) 
		{
#ifdef MDBG
		    traffic_stats(1);
#else
		    traffic_stats(0);
#endif
		}
#ifdef MIPC
		mipc_stats();
#endif

		cvoice_logd(1, "Reciever: net recieved %d, net sent %d, audio written %d", n_recieved, n_sent, n_write);
#else
		if (have_tr_play) join_threads( &set_tr_play );
		/*printf("sock=%d, audio = %d\n", sock, audio);*/
		/*cvoice_log(">> playing_mt");*/
		my_play_mt(sock, audio, buff, n, bn);
		mipc_incsteps();
		/*cvoice_log(">> played_mt");*/
#endif
	}

	/* not reached */
	if (cvoice_mspeex_decode_destroy() != 0)
	{
		transfer_finalize(-1, sock, audio, buff, udata, 1);
		cvoice_error("Error destroying speex decoder");
	}

	transfer_finalize(-1, sock, audio, buff, udata, 1);
	
	cvoice_clear_mshm();
	my_exit(0);
}

void input_sender(int sock, int sync)
{
	/* Microphone sender */
	int audio;
	int afmt, tmp;
	int err, n, m, nn;
	int bn, oos, oos2, nmax;
	int spid;
	unsigned char *buff;
	char *cdata;
	unsigned short sm, tstamp1, tstamp2;
#ifndef MMT
	int mpid;
#endif

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

	bn = cfg.cache_size * APCK;
#ifndef MMT
	mpid = getpid();
#endif

	if (cfg.wide) bn *= 2;

	cvoice_log("Final read cache: %d bytes", bn);

	n_steps = 0;
	n_steps2 = 0;

	n_read = 0;
	n_write = 0;
	n_sent = 0;
	n_recieved = 0;

	buff = (unsigned char*)malloc(bn * sizeof(unsigned char));
	cdata = (char*)malloc((bn + 4) * sizeof(char));

	if (cfg.timeout < 0) cfg.timeout *= -1;

	if (sync)
	{
		cvoice_log("Starting input syncer thread");
		spid = start_thread_syncer( 1, sync );
		cvoice_log("Syncer thread is %d", spid);
	}

	oos = 0;
	nmax = (cfg.cache_size / 15) + 1;
	if (nmax < 2) nmax = 2;

	time(&time_start);
	cvoice_log("Start time is: %d, timeout: %d, max recorder desync: %d", time_start, cfg.timeout, nmax);

	while (1)
	{
		n_steps2 = mipc_getsteps2();
		if (!cfg.n_sync && n_steps > 0 && n_steps2 > 0)
		{
		    	/* This is when more recorded when played */
			oos2 = (n_steps % 0x8000) - (n_steps2 % 0x8000);

			/* This were tests only, oos2 escaped to -inf */
/*			oos2 = (n_steps2 % 0x8000) - (n_steps % 0x8000);*/
			cvoice_logd(1, "Syncer recorder state: %d (%d, %d)", oos2, n_steps, n_steps2);
			if (oos2 > 0x4000) oos2 = 0;

			if (oos2 > nmax) /* Wyslalismy wiecej niz odtworzono, nalezy poczekac */
			{
			    cvoice_log("Desync is %d which is more than allowed %d", oos2, nmax);
			    oos2 = 2000 * cfg.cache_size * (oos2 - nmax);
			    if (oos2 > 1000000) oos2 = 1000000;
			    cvoice_log("Syncer: recorder going to sleep for %d ms", oos2 / 1000);
			    usleep( oos2 );
			}
		}

		if (cfg.timeout > 0) alarm(cfg.timeout);

		if (cfg.max_oos && cfg.n_sync && oos > cfg.max_oos)
		{
		    	cvoice_log("OOS> Shrinking input buffer to sync with reciever - data loss occured");
		    	nn = bn;
			oos --;
		}
		else 
		{
		    	nn = bn;
		}

/*		else*/
/*		if (cfg.max_oos && cfg.n_sync && oos > cfg.max_oos)*/
/*		{*/
/*		        cvoice_log("Sender out of sync, skipping audio recording, network not fast enough ", oos, cfg.max_oos, m);*/
/*			oos --;*/
/*			cvoice_log("New oos=%d", oos);*/
/*			if (oos < 0) oos = 0;*/
/*			n = bn;*/
/*			for (x=0;x<bn;x++) buff[x] = rand() % 0xff;*/
/*			n = SSYNC;*/
/*		}*/
/*		else*/
		{
#ifdef MWIN
			cvoice_mwaitaudio();
			e = windsp_read((char*)buff, nn);
			cvoice_msignalaudio();
			if (e != 0)
			{
				transfer_finalize(nn, sock, audio, buff, cdata, 0);
				cvoice_error("Win record buffer failed: %d", e);
			}
#else
			/*cvoice_log(">> reading audio");*/
			/*n = read(audio, buff, bn);*/
			nn = my_read(audio, buff, nn);

		/*cvoice_log(">> read audio");*/
#endif
		}

		if (cfg.timeout > 0) alarm(0);
		if (nn <= 0)
		/*if(n < bn)*/
		{
			transfer_finalize_sw(nn, sock, audio, buff, cdata, 0);
			cvoice_error("Read audio data r failed: n=%d", nn);
		}

		if (nn != SSYNC)
		{
			cvoice_logd(2, "Read %d audio bytes", nn);

			err = cvoice_mspeex_encode( buff, nn, &cdata[4], &m);
			if (m <= 0 || err != 0)
			{
				cvoice_log("Speex encoder error: err=%d, %d -> %d, skipping frame", err, nn, m);
				m = 1;
				cdata[0] = 0;
			}

			if (m > 0x7000) cvoice_log("Warning: near to error, encoded sample size is quite close to max_short 64K");
		}
		else
		{
		    	cvoice_log("Attempting to sync by sending shrinked data");
		    	err = 0;
			m = 0;
		}

		sm = (unsigned short)m;
		m += 4;
		tstamp1 = cvoice_timestamp();
		memcpy((void*)cdata, (void*)(&sm), sizeof(unsigned short));
		memcpy((void*)(&cdata[2]), (void*)(&tstamp1), sizeof(unsigned short));
		/*my_send(sock, buff, 2);*/
		/*my_recv(sock, buff, 1);*/
#ifdef MMT
		if (have_tr_send) join_threads( &set_tr_send );
#endif
		if (n_steps && cfg.n_sync && !(n_steps % cfg.n_sync)) 
		{
		    /*cvoice_log(">> waiting sync");*/
/*		    cvoice_logd(1, "waiting for sync");*/
		    cvoice_logd(1, "waiting for sync, steps made: %d", n_steps);
		    my_recv_uc(sock, buff, 2);
		    cvoice_logd(1, "synced");
		    memcpy((void*)(&tstamp2), (void*)buff, sizeof(unsigned short));
		    oos = abs(tstamp1 - tstamp2);
/*		    cvoice_log("Sender sync ratio %d: %d -- %d", oos, tstamp1, tstamp2);*/
		    n_sent += 2;
		    /*cvoice_log(">> synced");*/
		}

		if (cfg.max_oos && cfg.n_sync && oos > cfg.max_oos)
		{
		        cvoice_log("OOS> Sender out of sync is %d, limit is %d", oos, cfg.max_oos);
/*			mipc_incsteps();*/
/*			n_read += bn;*/
/*			n_recieved ++;*/
/*			if (!(n_steps % NSTEPSSTATS)) traffic_stats(cfg.debug);*/
/*			continue;*/
		}

		/*cvoice_log(">> sending audio mt");*/
		n = my_send_mt(sock, cdata, m);
		/*cvoice_log(">> sent audio mt");*/

		if (n < m)
		{
			transfer_finalize_sw(n, sock, audio, buff, cdata, 0);
			cvoice_error("Read audio data r failed: n=%d, m=%d", n, m);
		}

		/*__mt__: consider new thread for write */
		/* in MT mode we dont have cdata computed at this moment */
#ifndef MMT
		if (cfg.save && m > 4) 
		{
/*		    my_save(cdata, sm, 0, mpid);*/
		    my_save(cdata, 2, 0, mpid);			/* not saving timestamp */
		    my_save(&cdata[4], sm - 2, 0, mpid);
		}
#endif

		n_read += nn;
		/*n_sent += m + 2; */
		/*n_recieved += 2; */
		n_sent += m;
		n_recieved ++;

		/*cvoice_log(">> sender: n_steps = %d", n_steps);*/
		mipc_incsteps();
		if (!(n_steps % NSTEPSSTATS)) 
		{
#ifdef MDBG
		    traffic_stats(1);
#else
		    traffic_stats(0);
#endif
		}
#ifdef MIPC
		mipc_stats();
#endif

		cvoice_logd(1, "Sender: net recieved %d, net sent %d, audio read %d bytes", n_recieved, n_sent, n_read);
	}


	/* not reached */
	if (cvoice_mspeex_encode_destroy() != 0)
	{
		transfer_finalize_sw(-1, sock, audio, buff, cdata, 0);
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
	char buff[16];
	int ssock;	/* syncer socket */

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

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	if (ssock == -1)		
	{ 
		cvoice_log(strerror( errno ));
		cvoice_error("create syncer socket failed."); 
	}

	if (connect(sock,(struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1)
	{ 
		cvoice_log(strerror( errno ));
		cvoice_error("connect to %s:%d failed", cfg.ip, cfg.port); 
	}

	if (connect(ssock,(struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1)
	{ 
		cvoice_log(strerror( errno ));
		cvoice_error("syncer connect to %s:%d failed", cfg.ip, cfg.port); 
	}

	cfg.cli_sock = sock;
	cfg.sync_sock = ssock;
	cvoice_log("Connected, socket: %d", sock);

	sprintf(log_pfx, "<cli%d>", sock);

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

	/*
	cvoice_log("starting syncer thread");
	start_thread_syncer( inp, ssock );
	*/

	/* data flow starts */
	if (inp)
	{
		cvoice_log("Starting client sender");
		input_sender( sock, ssock );
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
		input_reciever( sock, ssock );
	}

	cvoice_log("Client exiting");

	cvoice_clear_mshm();
	my_exit(0);
	/* not reached */
	return 0;
}

int handle_client(int s)
{
	int pid, n, inp, sync;
	char buff[8];
	char cli_type;

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

	cli_type = buff[0];
	cvoice_log("Client type is: '%c'", cli_type);

	if (cli_type == 'i' || cli_type == 'I')
	{
	    cvoice_log("Input client mode");
	    inp = 1;
	    sync = 0;
	}
	else if (cli_type == 'j' || cli_type == 'J')
	{
	    cvoice_log("Input syncer client mode");
	    inp = 1;
	    sync = 1;
	}
	else if (cli_type == 'o' || cli_type == 'O')
	{
	    cvoice_log("Output client mode");
	    inp = 0;
	    sync = 0;
	}
	else if (cli_type == 'p' || cli_type == 'P')
	{
	    cvoice_log("Output syncer client mode");
	    inp = 0;
	    sync = 1;
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

	if (sync)
	{
	    	cvoice_log("Handling syncer client");
		if (!inp)
		{
			cvoice_log("Starting sync reciever server");
			sync_reciever(s);
		}
		else
		{
			cvoice_log("Starting sync sender server.1");
			n = my_recv(s, buff, 1);
			if (n < 1)
			{
				cvoice_log(strerror( errno ));
				close(s);
				cvoice_error("my_recv failed.2 in syncer: %d/1", n);
				return 0;
			}

			cvoice_log("Starting sync sender server.2");
			sync_sender(s);
		}
	}
	else
	{
		cvoice_log("Handling normal client");
		if (inp)
		{
			cvoice_log("Starting reciever server");
			input_reciever(s, 0);
		}
		else
		{
			cvoice_log("Starting sender server.1");
			n = my_recv(s, buff, 1);
			if (n < 1)
			{
				cvoice_log(strerror( errno ));
				close(s);
				cvoice_error("my_recv failedi.2: %d/1", n);
				return 0;
			}
			cvoice_log("Starting sender server.2");
			input_sender(s, 0);
		}
	}

	cvoice_log("Client finished");

	cvoice_clear_mshm();
	my_exit(0);
	/* not reached */
	return 0;
}

int cvoice_server()
{
	int pid;
	int csock, sock, alive, reuse, err;
	struct sockaddr_in server;
	int running;

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

	running = 1;
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
	/* no ret */
	return 0;
}

void cvoice_init()
{
	cv_cfg_clear( &cfg );
#ifdef MSPX
	speex_cfg_clear( &spx );
#endif
	cvoice_init_mipc(1);
	cvoice_log("cvoice written by Lucas Gryglicki ver 0.1: %s %s", __DATE__, __TIME__);
	printf("cvoice written by Lucas Gryglicki ver 0.1: %s %s\n", __DATE__, __TIME__);
}

void cvoice_free()
{
	cv_cfg_free( &cfg );
}

void cvoice_forked(int listener, int* pid1, int* pid2)
{
	*pid1 = *pid2 = 0;

	if (listener)
	{
	    	*pid1 = cvoice_server();
	    	cvoice_log("Server PID: %d", *pid1);
	    	printf("Server PID: %d\n", *pid1);
	}
	else
	{
		*pid1 = cvoice_client(1);
		*pid2 = cvoice_client(0);
	    	cvoice_log("Client PIDs: %d %d", *pid1, *pid2);
	    	printf("Client PIDs: %d %d\n", *pid1, *pid2);
	}

	cvoice_log("Spawned");
}


void cvoice(int listener)
{
	int pid1, pid2;

	pid1 = pid2 = 0;

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
    int i;

    if (mshm_ptr) 
    {
    	    for (i=0;i<10;i++) dummy[i] = 0.0;

	    memcpy(mshm_ptr, (void*)dummy, 10 * sizeof(double));
    }

#endif
}

void cvoice_save_config_fp(FILE* fp, cv_cfg* c)
{
    	if (!fp) return;

	fprintf(fp, "Input=%s\n", c->inum);
	fprintf(fp, "Output=%s\n", c->onum);
	fprintf(fp, "CacheSize=%d\n", c->cache_size);
	fprintf(fp, "Debug=%d\n", c->debug);
	fprintf(fp, "Quality=%d\n", c->quality);
	fprintf(fp, "Complexity=%d\n", c->complexity);
	fprintf(fp, "Timeout=%d\n", c->timeout);
	fprintf(fp, "Wide=%d\n", c->wide);
	fprintf(fp, "EnchanceR=%.3f\n", c->enh_r);
	fprintf(fp, "EnchanceW=%.3f\n", c->enh_w);
	fprintf(fp, "Save=%d\n", c->save);
	fprintf(fp, "NSync=%d\n", c->n_sync);
	fprintf(fp, "HZ=%d\n", c->hz);
	fprintf(fp, "PIDlogs=%d\n", c->pidlog);
	fprintf(fp, "Port=%d\n", c->port);
	fprintf(fp, "IP=%s\n", c->ip);

	cvoice_log("Configuration saved");
}

void cvoice_load_config_fp(FILE* fp, cv_cfg* c)
{
    	if (!fp) return;
	fscanf(fp, "Input=%s\n", c->inum);
	fscanf(fp, "Output=%s\n", c->onum);
	fscanf(fp, "CacheSize=%d\n", &c->cache_size);
	fscanf(fp, "Debug=%d\n", &c->debug);
	fscanf(fp, "Quality=%d\n", &c->quality);
	fscanf(fp, "Complexity=%d\n", &c->complexity);
	fscanf(fp, "Timeout=%d\n", &c->timeout);
	fscanf(fp, "Wide=%d\n", &c->wide);
	fscanf(fp, "EnchanceR=%f\n", &c->enh_r);
	fscanf(fp, "EnchanceW=%f\n", &c->enh_w);
	fscanf(fp, "Save=%d\n", &c->save);
	fscanf(fp, "NSync=%d\n", &c->n_sync);
	fscanf(fp, "HZ=%d\n", &c->hz);
	fscanf(fp, "PIDlogs=%d\n", &c->pidlog);
	fscanf(fp, "Port=%d\n", &c->port);
	fscanf(fp, "IP=%s\n", c->ip);

    	cvoice_log("Configuration loaded");
}

int cvoice_save_config(const char* fn, cv_cfg* c)
{
    FILE* fp;

    cvoice_log("Saving config to: %s", fn);

    fp = fopen(fn, "w");
    if (!fp)
    {
	cvoice_log("Cannot write to file: %s", fn);
	return 0;
    }

    cvoice_save_config_fp( fp, c );
    fclose( fp );

    
    cvoice_log("Saved to: %s", fn);
    return 1;
}

int cvoice_load_config(const char* fn, cv_cfg* c)
{
    FILE* fp;

    cvoice_log("Loading config from: %s", fn);

    fp = fopen(fn, "r");
    if (!fp)
    {
    	cvoice_log("Error: cannot open file: %s", fn);
	return 0;
    }

    cvoice_load_config_fp( fp, c );
    fclose( fp );

    cvoice_log("Loaded from: %s", fn);

    return 1;
}

int cvoice_getopt(int lb, char** par)
{
	int is_server;
	char u;
	/*
	a: server mode
	s: server
	i: ip
	p: port 
	f: myid activates ex mode
	t: targetid activates ex mode
	h: help
	c: cache size
	d: debug on
	q: quality
	T: timeout
	I: input(microphone) device number
	O: output(speakers) device number
	P: use PID in log file name
	W: use 16bit audio instead of 8bit
	S: save input/output to mspx files
	C: set encoder complexity 0-10
	y: set sync after n cycles
	H: set sample rate (if other than 8000 Hz)
	L: load config from file
	Z: store config to file
	o: Turn of out of sync checking
	*/
	
	time_start = 0;
	is_server = 0;
	strcpy(log_pfx, "<init>");

#ifdef MDBG
	cfg.debug = 1;
#endif

	while ((u = getopt(lb,par,"S8as:i:p:f:t:hc:dq:T:I:O:PR:W:C:y:H:L:Z:o:"))!=-1)
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
			case 'd': cfg.debug ++;			break;
			case 'q': cfg.quality = atoi(optarg);	break;
			case 'T': cfg.timeout = atoi(optarg);	break;
			case 'I': strcpy(cfg.inum, optarg);	break;
			case 'O': strcpy(cfg.onum, optarg);	break;
			case 'P': cfg.pidlog = 1;		break;
			case '8': cfg.wide = 0;			break;
			case 'o': cfg.max_oos = atoi(optarg);	break;
			case 'R': cfg.enh_r = atof(optarg);	break;
			case 'W': cfg.enh_w = atof(optarg);	break;
			case 'S': cfg.save = 1;			break;
			case 'C': cfg.complexity = atoi(optarg);break;
			case 'y': cfg.n_sync = atoi(optarg);	break;
			case 'H': cfg.hz = atoi(optarg);	break;
			case 'L': 
				if (!cvoice_load_config(optarg, &cfg)) 
				{
				    printf("Failed to load configuration from %s\n", optarg);
				    return -1;
				}
				else printf("Configuration loaded from %s\n", optarg);
				break;
			case 'Z': 
				if (!cvoice_save_config(optarg, &cfg)) 
				{
				    printf("Failed to save configuration to %s\n", optarg);
				    return -2;
				}
				else printf("Configuration saved to %s\n", optarg);
				break;
			default: printf("Unrecognized option\n"); return -3;
		}
	}

	cvoice_log("Connect is %s:%d, debug:%d", cfg.ip, cfg.port, cfg.debug);

	return is_server;
}


