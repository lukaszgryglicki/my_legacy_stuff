#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>

#include "libmipc.h"

#define MOFFSET "/tmp/mat_offset"
#define MSPAWN  "/tmp/mat_spawn"
#define MAXSHMN 1023

void spawn_process(time_t t, char* cmd, int toff);
void remove_from_shm(time_t t, int toff, char* cmd);
void clear_shm();
void print_shm();

typedef struct _spawn
{
    time_t t;
    int len, toff;
    char* syscmd;
} spawn;

typedef struct _shmcmd
{
    int t, toff, l;
    char cmd[0xF4];		/* maxlen 0xF3 */
} shmcmd;

time_t g_t, g_ts, g_tt, g_toff;
char* cmd_ptr;
int semid, shmid, nspawn, rshm_flag,pshm_flag;
spawn* spawns;
void* shm;

void logfmt(const char* fmt, ...)
{
	va_list lst;
	FILE* log;
	time_t t;
	static struct tm *tms;
	char tstr[128];

	log = fopen("/tmp/mat.log", "a");
	if (!log) return;

	time(&t);
	tms = localtime(&t);
	sprintf(tstr, "%05d> %04d-%02d-%02d %02d:%02d:%02d", 
		getpid(), 1900 + tms->tm_year, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);

	va_start(lst,fmt);

	fprintf(log, "%s: ", tstr);
	vfprintf(log, fmt ,lst);

	fflush(log);
	fclose(log);

	va_end(lst);
}

void write_spawn_file()
{
    FILE* f;
    int i;

/*    logfmt("Writing %d spawns to \"%s\"\n", nspawn, MSPAWN);*/

    f = fopen(MSPAWN, "w");
    if (!f)
    {
	fprintf(stderr, "Cannot write to spawn file: %s\n", MSPAWN);
	return;
    }

    fprintf(f, "n: %d\n", nspawn);
    for (i=0;i<nspawn;i++)
    {
	fprintf(f, "%d,%d,%d,%s\n", (int)spawns[i].t, (int)strlen(spawns[i].syscmd), (int)spawns[i].toff, spawns[i].syscmd);
    }

    fclose(f);
}

void make_empty_spawn()
{
    nspawn = 0;
    spawns = (spawn*)malloc(sizeof(spawn));
}

void read_spawn_file()
{
    FILE* f;
    int i, ti, nscan;

/*    logfmt("Reading from spawn file \"%s\"\n", MSPAWN);*/

    f = fopen(MSPAWN, "r");
    if (!f)
    {
	logfmt("No spawn file: %s\n", MSPAWN);
	make_empty_spawn();
	return;
    }

    nspawn = -1;
    i = fscanf(f, "n: %d\n", &nspawn);
    if(i < 1 || nspawn < 0)
    {
	logfmt("Bad spawns count or scan value: %d,%d\n", nspawn,i);
	fclose(f);
	make_empty_spawn();
	return;
    }

    spawns = (spawn*)malloc((nspawn+1)*sizeof(spawn));
    if (!spawns)
    {
	fprintf(stderr, "Cannot allocate memory for spawns, count: %d\n", nspawn);
	fclose(f);
	make_empty_spawn();
	return;
    }

    for (i=0;i<nspawn;i++)
    {
	if ((nscan=fscanf(f, "%d,%d,%d,", &ti, &spawns[i].len, &spawns[i].toff)) != 3)
	{
	    fprintf(stderr, "Cannot scan %d/%d spawn: nscan: %d\n", i, nspawn, nscan);
	    free((void*)spawns);
	    fclose(f);
	    make_empty_spawn();
	    return;
	}
	spawns[i].t = (time_t)ti;

	spawns[i].syscmd = (char*)malloc(spawns[i].len * sizeof(char));

	if ((nscan=fscanf(f, "%[^\n]\n", spawns[i].syscmd)) != 1)
	{
	    fprintf(stderr, "Cannot scan %d/%d cmd: nscan: %d\n", i, nspawn, nscan);
	    free((void*)spawns);
	    fclose(f);
	    make_empty_spawn();
	    return;
	}

/*	printf("Scanned: \"%s\" at (%d,%d)\n", spawns[i].syscmd, spawns[i].t, spawns[i].toff);*/
    }

/*    printf("Allocated %d spawn table, now elements %d\n", nspawn+1, nspawn);*/
    fclose(f);
}
    
void add_to_spawn_file(time_t t, char* cmd, int toff)
{
/*    printf("Adding record: \"%s\" time (%d, %d) at index: %d\n", cmd, t, toff, nspawn);*/
    spawns[nspawn].t    = t;
    spawns[nspawn].toff = toff;
    spawns[nspawn].len  = strlen(cmd) + 1;
    spawns[nspawn].syscmd = (char*)malloc(spawns[nspawn].len*sizeof(char));
    strcpy(spawns[nspawn].syscmd, cmd);
/*    printf("Added record: \"%s\" time (%d, %d)\n", spawns[nspawn].syscmd, spawns[nspawn].t, spawns[nspawn].toff);*/
    nspawn ++;
}

void validate_spawn_file()
{
    time_t tnow;
    int i, j, nn;
    spawn* ns;

/*    printf("Validating %d element spawn file\n", nspawn);*/

    time(&tnow);

    nn = 0;
    for (i=0;i<nspawn;i++)
    {
/*	printf("validate %d || %d >= %d\n", spawns[i].toff, spawns[i].t, tnow);*/
	if (spawns[i].toff || spawns[i].t > tnow) nn ++;
    }

    logfmt("New number of spawns: %d\n", nn);

    ns = (spawn*)malloc((nn+1)*sizeof(spawn));
    if (!ns)
    {
	fprintf(stderr, "Cannot allocate new spawn table for %d elements\n", nn);
	return;
    }

    j = 0;
    for (i=0;i<nspawn;i++)
    {
	if (spawns[i].toff || spawns[i].t > tnow)
	{
	    ns[j].t      = spawns[i].t;
	    ns[j].len    = spawns[i].len;
	    ns[j].toff   = spawns[i].toff;
	    ns[j].syscmd = (char*)malloc(ns[j].len*sizeof(char));
	    strcpy(ns[j].syscmd, spawns[i].syscmd);

	    j ++;
	}
	free((void*)spawns[i].syscmd);
    }

    free((void*)spawns);

    spawns = ns;
    nspawn = j;

/*    printf("Spawn file reallocated: %d\n", nspawn);*/
}
	    

void spawn_delayed(char* cmdline, char* cmd, int tdiff, int toff)
{
    int p;
    if ((p = fork()) != 0)
    {
	printf("Spawned delayed loop clock PID: %d, will wait %ds and loop each %ds\n", p, tdiff, toff);
	return ;
    }

    sprintf(cmdline, "/usr/local/bin/mat =%ds %s", toff, cmd);
    logfmt("SYSTEM after %ds: \"%s\"\n", tdiff, cmdline);

    if (tdiff > 1) 
    {
	sleep(tdiff-1);

        sprintf(cmdline, "/usr/local/bin/mat +1s %s", cmd);
        logfmt("SYSTEM: \"%s\"\n", cmdline);
        system(cmdline);
    }

    if (tdiff > 0) sleep(1);

    sprintf(cmdline, "/usr/local/bin/mat -R =%ds %s", toff, cmd);
    logfmt("SYSTEM: \"%s\"\n", cmdline);
    system(cmdline);

    logfmt("Delayed invocation done (+%ds, =%ds)\n", tdiff, toff);
    exit(0);
}

void spawn_all()
{
    int i, l;
    char* cmdline;
    int tdiff;
    time_t t;

    printf("About to respawn %d clocks\n", nspawn);

    l = 0;
    for (i=0;i<nspawn;i++)
    {
	logfmt("Clock %d, command \"%s\" time (%d,%d)\n", i, spawns[i].syscmd, spawns[i].t, spawns[i].toff);
	if (spawns[i].len > l) l = spawns[i].len;
    }

    cmdline = (char*)malloc((l+32) * sizeof(char));

    for (i=0;i<nspawn;i++)
    {
	time(&t);
	tdiff = spawns[i].t - t;
	if (spawns[i].toff)
	{
	    if (tdiff< 0) logfmt("Adjusting tloop: %d\n", spawns[i].t);
	    while (tdiff < 0) 
	    {
		spawns[i].t += spawns[i].toff;
	        tdiff = spawns[i].t - t;
	    }
	    logfmt("Adjusted t_start: %d, t_off: %d\n", spawns[i].t, spawns[i].toff);
	}
	else if (!spawns[i].toff && tdiff < 0)
	{
	    logfmt("Event in the past, skipping\n");
	    continue;
	}

	if (spawns[i].toff)
	{
	    spawn_delayed(cmdline, spawns[i].syscmd, tdiff, spawns[i].toff);
	}
	else
	{
            sprintf(cmdline, "/usr/local/bin/mat %c%ds %s", spawns[i].toff ? '=' : '+', tdiff, spawns[i].syscmd);
	    logfmt("SYSTEM: \"%s\"\n", cmdline);
	    system(cmdline);
	}
    }
}

void desetup_mipc()
{
    key_t semk, shmk;
    semk = shmk = 20120630;

    if (mdropsemkey(semk) != 0) warn("Warning, cannot drop semaphore with key: %d\n", (int)semk);
    if (mdropshmkey(shmk) != 0) warn("Warning, cannot drop shared memory with key: %d\n", (int)shmk);
}


void setup_mipc(int respawn)
{
    key_t semk, shmk;
    int is_newM, is_newS;

/*    printf("SetUp LibMIPC\n");*/

    semid = is_newS = is_newM = 0;
    semk = shmk = 20120630;

    semid = msemnew(semk, 1, &is_newS);
    if (semid < 0) fatal("create semaphore with kay: %x\n", semk);

    shmid = mshmnew(shmk, (1 << 18), &is_newM);
    if (shmid < 0) fatal("create shared memory with key: %x\n", shmk);

    if (mshmconn(shmid, &shm) < 0) fatal("connecting shared memory id=%d\n", shmid);

    if (is_newM || is_newS) 
    {
	logfmt("First run, created semaphore: (%d, %d)i, memory: (5d, %d)\n", semid, (int)semk, shmid, (int)shmk);
    }
    else
    {
	logfmt("Using existing semaphore: (%d, %d), memory (%d, %d)\n", semid, (int)semk, shmid, (int)shmk);
    }

    if (respawn < 0) return;

    mwait(semid);
    read_spawn_file();
    validate_spawn_file();
    clear_shm();
    msignal(semid);

    if (is_newS || is_newM || respawn) 
    {
	spawn_all();
	mwait(semid);
	write_spawn_file();
	msignal(semid);
    }
}

void catch_signal(int signo)
{
    char cts[32], cts0[32], ctss[32];
    char wd[0x200];
    time_t t0;
    int dt;

    if (signo == SIGINT || signo == SIGTERM)
    {
	logfmt("Handle interrupt: %d\n", signo);
        mwait(semid);
	remove_from_shm(g_tt, g_toff, cmd_ptr);
	msignal(semid);
	exit(0);
    }

    time(&t0);

    ctime_r(&t0, cts0);
    ctime_r(&g_t, cts);
    ctime_r(&g_ts, ctss);

    dt   = g_t - t0;
    if (dt < 0) return;
    getcwd(wd, 0x1ff);

    logfmt("MAT compiled: %s %s\nCought signal number: %d\n", __DATE__, __TIME__, signo);

    printf("Start date: %sCurrent date: %sAlarm date: %sTime left: %ds (%d hours, %d days)\nCommand: %s: '%s'\n", 
            ctss, cts0, cts, dt, dt/3600, dt/(3600*24), wd, cmd_ptr);
    logfmt("Start date: %sCurrent date: %sAlarm date: %sTime left: %ds (%d hours, %d days)\nCommand: %s: '%s'\n", 
	    ctss, cts0, cts, dt, dt/3600, dt/(3600*24), wd, cmd_ptr);

}

void setup_signals()
{
    static struct sigaction act;

    act.sa_handler = catch_signal;
    sigfillset(&(act.sa_mask));

    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGINT,  &act, NULL);
    sigaction(SIGTERM,  &act, NULL);
}

int compare_with_shm(time_t tm, int toff, char* cmd, int idx)
{
    /*
    t, toff, l
    cmd[0xf4] maxlen F3
    */
    shmcmd s;
    int t;
    unsigned char* mem;

    t = (int)tm;
    mem = (unsigned char*)shm;

    memcpy((void*)&s, (void*)(&mem[4+idx*sizeof(shmcmd)]), sizeof(shmcmd));

/*    printf("--------->\ntime: %d -- %d\ntoff: %d -- %d\ncmd:  \"%s\" -- \"%s\"\n", t, s.t, toff, s.toff, cmd, s.cmd);*/

    if (t == s.t && toff == s.toff && !strcmp(cmd, s.cmd))
    {
	return 1;
    }
    else
    {
	if (toff && s.toff == toff)
	{
/*	    printf("The same toff: %d\n", toff);*/
/*	    printf("ABS: %d, REST: %d\n", abs(t - s.t), abs(t - s.t) % toff);*/
	    /* sytuacja gdy maj± te same offsety czasowe, jesli dodatkowo czasy startowe roznia sie o wielokrotnosc toff */
	    /* to s± tozsamo rowne */
	    if (!(abs(t - s.t) % toff))
	    {
		logfmt("The same effective time: start: %d -- %d, offset: %d\n", t, s.t, toff);
		return 1;
	    }
	}
	return 0;
    }
}

void add_to_shm(time_t tm, int toff, char* cmd)
{
    int nm;
    shmcmd s;
    unsigned char* mem;

    nm = -1;
    memcpy((void*)&nm, shm, sizeof(int));
/*    printf("Adding to SHM, number of SHM entires: %d\n", nm);*/

    if (nm > MAXSHMN)
    {
	fprintf(stderr, "Too many SHM entires: %d, skipping\n", nm);
	return;
    }

    mem = (unsigned char*)shm;

    s.t = (int)tm;
    s.toff = toff;
    s.l = strlen(cmd);

    if (s.l >= 0xf3)
    {
	strncpy(s.cmd, cmd, 0x73);
	s.l = 0xf3;
	s.cmd[0x73] = 0;
    }
    else
    {
	strcpy(s.cmd, cmd);
    }

    memcpy((void*)(&mem[4+nm*sizeof(shmcmd)]), (void*)&s, sizeof(shmcmd));
    nm ++;
    memcpy(shm, (void*)&nm, sizeof(int));

    logfmt("Added to SHM, now entires: %d\n", nm);
}

int check_shm(time_t t, int toff, char* cmd)
{
    int i, nm;

    nm = -1;
    memcpy((void*)&nm, shm, sizeof(int));
/*    printf("Number of SHM entires: %d\n", nm);*/

    if (nm > 0)
    {
	for (i=0;i<nm;i++)
	{
	    if (compare_with_shm(t, toff, cmd, i))
	    {
		logfmt("Command \"%s\" time (%d,%d) identical to SHM at idx: %d\n", cmd, t, toff, i);
		return 1;
	    }
	}
    }

    add_to_shm(t, toff, cmd);

    return 0;
}

void print_shm()
{
    int i, nm;
    shmcmd s;
    unsigned char* mem;

    nm = -1;
    memcpy((void*)&nm, shm, sizeof(int));
    mem = (unsigned char*)shm;

    printf("Num entires in SHM: %d\n", nm);

    for (i=0;i<nm;i++)
    {
        memcpy((void*)&s, (void*)(&mem[4+i*sizeof(shmcmd)]), sizeof(shmcmd));
	printf("%d) %d %d %d \"%s\"\n", i, s.t, s.toff, s.l, s.cmd);
    }
}

void remove_from_shm(time_t t, int toff, char* cmd)
{
    int i, del, nm, i1, i2, ncp;
    unsigned char* mem;

    nm = -1;
    del = 0;

    memcpy((void*)&nm, shm, sizeof(int));
    mem = (unsigned char*)shm;

    logfmt("Deleting \"%s\" (%d,%d), number of SHM entires: %d\n", cmd, t, toff, nm);
/*    print_shm();*/

    if (nm > 0)
    {
	for (i=0;i<nm;i++)
	{
	    if (compare_with_shm(t, toff, cmd, i))
	    {
		logfmt("Found \"%s\" in SHM at idx: %d\n", cmd, i);

		i1 = i;
		i2 = i + 1;
		ncp = nm - i2;

		if (i2 < nm)
		{
/*		    printf("Move: offsets: %d,%d,%d\n", i1, i2, ncp);*/
		    memmove((void*)(&mem[4+i1*sizeof(shmcmd)]), (void*)(&mem[4+i2*sizeof(shmcmd)]), ncp*sizeof(shmcmd));
		}
		del = 1;

		break;
	    }
	}
    }

    if (del)
    {
        nm --;
        memcpy(shm, (void*)&nm, sizeof(int));

        logfmt("Deleted from SHM, now entires: %d\n", nm);
    }
    else
    {
	logfmt("Error: Not found\n");
    }

/*    print_shm();*/
/*    printf("delete complete\n");*/
}

void clear_shm()
{
    int i, t, nm;
    shmcmd s;
    unsigned char* mem;
    time_t tm;

    nm = -1;
    memcpy((void*)&nm, shm, sizeof(int));
    mem = (unsigned char*)shm;
    time(&tm);
    t = (int)tm;

/*    printf("Clearing SHM, now entires: %d\n", nm);*/
/*    print_shm();*/

    for (i=0;i<nm;i++)
    {
        memcpy((void*)&s, (void*)(&mem[4+i*sizeof(shmcmd)]), sizeof(shmcmd));

	if (!s.toff && s.t <= t)
	{
/*	    printf("Entry: %d, time is %d -- now is %d, toff is %d\n", i, s.t, t, s.toff);*/
	    remove_from_shm(s.t, s.toff, s.cmd);
	}
    }

/*    printf("SHM cleared\n");*/
/*    print_shm();*/
/*    printf("clear done\n");*/
}

void test_mipc()
{
    char cmd[16];
    strcpy(cmd, "test");

    setup_mipc(0);
    mwait(semid);

    add_to_shm(1001, 10, cmd); 
    add_to_shm(1002, 10, cmd); 
    add_to_shm(1003, 10, cmd); 
    add_to_shm(1004, 10, cmd); 
    add_to_shm(1005, 10, cmd); 
    add_to_shm(1006, 10, cmd); 
    add_to_shm(1007, 10, cmd); 
    add_to_shm(1008, 10, cmd); 
    add_to_shm(1009, 10, cmd); 


    remove_from_shm(1003, 10, cmd);
    remove_from_shm(1007, 10, cmd);
    remove_from_shm(1002, 10, cmd);
    remove_from_shm(1005, 10, cmd);

    print_shm();
    msignal(semid);

    desetup_mipc();

    exit(1);
}

void spawn_process(time_t t, char* cmd, int toff)
{
    int p, done, finished;
    time_t tn;
    char ct[32];

    mwait(semid);
    if (check_shm(t, toff, cmd) != 0)
    {
	msignal(semid);

	if (rshm_flag)
	{
	    logfmt("Drop duplicate keys from SHM\n");
	    
	    mwait(semid);
	    remove_from_shm(t, toff, cmd);
	    msignal(semid);
	}
	else
	{
	    logfmt("Duplicate process, skipped\n");
	    printf("Duplicate process, skipped\n");
            mwait(semid);

            validate_spawn_file();
            write_spawn_file();

            msignal(semid);
	    return;
	}
    }
    else 
    {
	msignal(semid);
    }

    if ((p = fork()) != 0)
    {
	printf("Spawned clock PID: %d\n", p);
	return ;
    }

    logfmt("Command: '%s'\n", cmd);
    setup_signals();

    cmd_ptr = (char*)malloc((strlen(cmd)+1)*sizeof(char));
    g_tt    = t;
    g_toff  = toff;
    strcpy(cmd_ptr, cmd);


    mwait(semid);

    validate_spawn_file();
    add_to_spawn_file(t, cmd, toff);
    write_spawn_file();

    msignal(semid);

    finished = 0;
    while (!finished)
    {
        g_t = t;
        done = 0;

        while (!done)
        {
	    time(&tn);

	    if (tn >= t)
	    {
                ctime_r(&tn, ct);
	        done = 1;
	    }

	    usleep(500000);
        }

        t += toff;

        logfmt("Time is: %sExecute: '%s'\n", ct, cmd);
        if (1)
/*      if (fork())*/
        {
            system(cmd);

    	    mwait(semid);
	    validate_spawn_file();
            write_spawn_file();
            msignal(semid);
        }

	if (!toff) finished = 1;
    }

    mwait(semid);
    remove_from_shm(t, toff, cmd);
    clear_shm();
    msignal(semid);

    return;
}

int moffset()
{
    FILE* f;
    int loff;

    f = fopen(MOFFSET, "r");
    if (!f)
    {
	logfmt("No time offset file: %s\n", MOFFSET);
	return 0;
    }

    if (fscanf(f, "%d", &loff) != 1)
    {
	logfmt("Error scanning integer value from time offset file: %s\n", MOFFSET);
	return 0;
    }

    logfmt("New time offset from %s is %d\n", MOFFSET, loff);

    return loff;
}

int mat(char* ts, int lb, char** par)
{
    struct tm tms;
    char Ys[5], Ms[3], Ds[3], hs[3], ms[3], ss[3];
    int Yi, Mi, Di, hi, mi, si;
    time_t t, t0;
    char ctss[32], cts[32], *cmd, toff;
    int td, l, i, noff, loop;

/*    printf("mat: %s, %d, %s\n", ts, lb, par[0]);*/

    noff = 0;
    logfmt("MAT compiled: %s %s\n", __DATE__, __TIME__);

    loop = 0;
    i = sscanf(ts, "+%d%c", &noff, &toff);

    if (i != 2)
    {
	i = sscanf(ts, "=%d%c", &noff, &toff);
	if (i == 2) loop = 1;
    }

    if (i == 2)
    {
	if (toff == 's' || toff == 'S') ;
	else if (toff == 'm' || toff == 'M') noff *= 60;
	else if (toff == 'h' || toff == 'H') noff *= 3600;
	else if (toff == 'd' || toff == 'D') noff *= 86400;
	else if (toff == 'w' || toff == 'W') noff *= 604800;
	else
	{
	    printf("Unknown time offset: '%d%c'\n", noff, toff);
	    return 18;
	}

	if (noff <= 0)
	{
	    printf("Time offset is <= 0: %d\n", noff);
	    return 19;
	}
    }
    else
    {
        if (strlen(ts) < 14)
        {
	    printf("'%s' is not in format: YYYYMMDDhh24mmss\n", ts);
	    return 2;
        }

        strncpy(Ys, ts, 4);
        Ys[4] = 0;

        strncpy(Ms, ts+4, 2);
        Ms[2] = 0;

        strncpy(Ds, ts+6, 2);
        Ds[2] = 0;

        strncpy(hs, ts+8, 2);
        hs[2] = 0;

        strncpy(ms, ts+10, 2);
        ms[2] = 0;

        strncpy(ss, ts+12, 2);
        ss[2] = 0;

        logfmt("Preparsed date is: %s-%s-%s %s:%s:%s\n", Ys, Ms, Ds, hs, ms, ss);

        if (sscanf(Ys, "%d", &Yi) != 1) 
        {
	    printf("Cannot scan year from '%s'\n", Ys);
	    return 3;
        }

        if (sscanf(Ms, "%d", &Mi) != 1) 
        {
	    printf("Cannot scan month from '%s'\n", Ms);
	    return 4;
        }

        if (sscanf(Ds, "%d", &Di) != 1) 
        {
	    printf("Cannot scan day from '%s'\n", Ds);
	    return 5;
        }

        if (sscanf(hs, "%d", &hi) != 1) 
        {
	    printf("Cannot scan hour from '%s'\n", hs);
	    return 6;
        }

        if (sscanf(ms, "%d", &mi) != 1) 
        {
	    printf("Cannot scan minute from '%s'\n", ms);
	    return 7;
        }

        if (sscanf(ss, "%d", &si) != 1) 
        {
	    printf("Cannot scan second from '%s'\n", ss);
	    return 8;
        }

        if (Yi < 1900 || Yi > 2100)
        {
	    printf("Year %d out of range 1900-2100\n", Yi);
	    return 9;
        }

        if (Mi < 1 || Mi > 12)
        {
	    printf("Month %d out of range 1-12\n", Mi);
	    return 10;
        }

        if (Di < 1 || Di > 31)
        {
	    printf("Day %d out of range 1-12\n", Di);
	    return 11;
        }

        if (Di == 29 && Mi == 2 && (Yi % 4))
        {
	    printf("29th of February impossible for year %d\n", Yi);
	    return 12;
        }

        if (Di == 30 && Mi == 2)
        {
	    printf("30th of February is impossible\n");
	    return 13;
        }

        if (Di == 31 && (Mi == 2 || Mi == 4 || Mi == 6 || Mi == 9 || Mi == 11))
        {
	    printf("31th of %d month is impossible\n", Mi);
	    return 14;
        }

        tzset();

        tms.tm_year = Yi - 1900;
        tms.tm_mon  = Mi - 1;
        tms.tm_mday = Di;
        tms.tm_hour = hi;
        tms.tm_min  = mi;
        tms.tm_sec  = si;
    
	t = mktime(&tms);
    }


    time(&t0);
    if (noff > 0) t = t0 + (time_t)noff;
    else t += moffset();

    /*t = timegm(&tms);*/
    ctime_r(&t, cts);
    td = (int)t - (int)t0;

    logfmt("Final date is: %s", cts);
    logfmt("NOW=%d\tAT=%d\n", t0, t);

    if (td <= 0)
    {
        time(&g_ts);
        ctime_r(&g_ts, ctss);
        printf("Final date is: %sNow is: %s", cts, ctss);
	printf("Event is in the past, tdiff: %ds, bye\n", td);
	return 15;
    }

    l = 2;
    for (i=2;i<lb;i++)
    {
	l += strlen(par[i]) + 2;
    }

    cmd = (char*)malloc(l*sizeof(char));
    if (!cmd)
    {
	logfmt("Failed to alloc command space: %d bytes\n", l);
	return 16;
    }
    strcpy(cmd, "");

    for (i=2;i<lb;i++)
    {
	if (i > 2) strcat(cmd, " ");
	strcat(cmd, par[i]);
    }

    if (strlen(cmd) < 1)
    {
	printf("Empty command, skipped\n");
	return 17;
    }

    time(&g_ts);
    ctime_r(&g_ts, ctss);

    logfmt("Start date: %sLeft seconds: %d, minutes: %d, hours: %d, days: %d, months: %d, years: %d\n", 
	    ctss, td, td/60, td/3600, td/(3600*24), td/(3600*24*12), td/(3600*24*365));

    g_t = 0;
    cmd_ptr = NULL;
    spawn_process(t, cmd, loop ? noff : 0);

    return 0;
}
void separate_options(int lb, char** par, int* lb_o, char*** par_o, int* lb_f, char*** par_f)
{
    int i, l, t0, t1;
    char c;
    short *t;
    
    t = (short*)malloc(lb*sizeof(short));
    *lb_o = *lb_f = 1;

    for (i=1;i<lb;i++)
    {
	l = strlen(par[i]);
	if (l < 2)
	{
	    (*lb_f) ++;
	    t[i] = 0;	/* 0 - opcja nie getopt */
	    continue;
	}

	if (par[i][0] != '-')
	{
	    (*lb_f) ++;
	    t[i] = 0;
	    continue;
	}

	c = par[i][1];

	if ((c >= 'a' && c < 'x') || (c >= 'A' && c < 'X'))
	{
	    if ((l > 2 && par[i][2] >= '0' && par[i][2] <= '9') || l == 2)
	    {
	    	(*lb_o) ++;
	    	t[i] = 1;
	    }
	    else
	    {
		(*lb_f) ++;
		t[i] = 0;
	    }
	}
    }

    *par_f = (char**)malloc(*lb_f*sizeof(char*));
    *par_o = (char**)malloc(*lb_o*sizeof(char*));

    (*par_f)[0] = (char*)malloc((strlen(par[0])+1)*sizeof(char));
    (*par_o)[0] = (char*)malloc((strlen(par[0])+1)*sizeof(char));

    strcpy((*par_f)[0], par[0]);
    strcpy((*par_o)[0], par[0]);

    t0 = t1 = 1;
    for (i=1;i<lb;i++)
    {
	if (t[i])
	{
	    (*par_o)[t1] = (char*)malloc((strlen(par[i])+1)*sizeof(char));
	    strcpy((*par_o)[t1], par[i]);
	    t1 ++;
	}
	else
	{
	    (*par_f)[t0] = (char*)malloc((strlen(par[i])+1)*sizeof(char));
	    strcpy((*par_f)[t0], par[i]);
	    t0 ++;
	}
    }
    /*
    printf("lb_f = %d\n", *lb_f);
    for (i=0;i<t0;i++) printf("par_f[%d] = \"%s\"\n", i, (*par_f)[i]);

    printf("lb_o = %d\n", *lb_o);
    for (i=0;i<t1;i++) printf("par_o[%d] = \"%s\"\n", i, (*par_o)[i]);
    */
}

void usage(char* par)
{
	printf("Usage:\t%s [options] YYYYMMDDhh24mmss|+/=Ns,m,h,d,w command (+after, =each)\n", par);
	printf("SIGUSR1 on spawned alarm process will give You informations about time left\n");
	printf("Eventual time adjustments in %s\n", MOFFSET);
	printf("Respawn file is \"%s\"\n", MSPAWN);
	printf("You can call %s with one arg:\n", par);
	printf("\t-sS to respawn unfinished jobs\n");
	printf("\t-dD will drop semaphore and clear shm\n");
	printf("\t-rR will skip SHM hints and drop shm duplicates\n");
	printf("\t-pP will print out SHM memory contents\n");
	printf("\t-hH will print this help\n");
}

int main(int lb, char** par)
{
    int respawn, drop_flag;
    int lb_f, lb_o/*, i*/;
    char **par_f, **par_o, u;

/*    test_mipc();*/

    separate_options(lb, par, &lb_o, &par_o, &lb_f, &par_f);

    respawn = rshm_flag = drop_flag = pshm_flag = 0;

    while ((u = getopt(lb_o, par_o,"hHsSrRdDpP")) != -1)
    {
	switch (u)
	{
	        case 'h': case 'H': usage(par_f[0]); 	return 0;
	        case 's': case 'S': respawn = 1;	break;
	        case 'r': case 'R': rshm_flag = 1;	break;
	        case 'd': case 'D': drop_flag = 1;	break;
	        case 'p': case 'P': pshm_flag = 1;	break;
		default: printf("Unrecognized option\n"); return 1;
	}
    }

    if (drop_flag)
    {
	printf("Dropping semaphore and shared memory area\n");
	desetup_mipc();
	exit(0);
    }

    if (pshm_flag)
    {
	printf("Displaying shared memory area\n");
	setup_mipc(-1);
	print_shm();
	exit(0);
    }

    if (respawn) printf("Respawn mode is on\n");
    if (rshm_flag) printf("Ignore SHM pool mode\n");

    if (lb_f < 3)
    {
	if (!respawn) usage(par_f[0]);
 
	setup_mipc(respawn);

	return 1;
    }

    setup_mipc(respawn);

    /*
    printf("lb_f = %d\n", lb_f);
    for (i=0;i<lb_f;i++) printf("par_f[%d] = \"%s\"\n", i, par_f[i]);

    printf("lb_o = %d\n", lb_o);
    for (i=0;i<lb_o;i++) printf("par_o[%d] = \"%s\"\n", i, par_o[i]);
    */

    return mat(par_f[1], lb_f, par_f);
}

