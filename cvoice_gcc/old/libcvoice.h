#ifndef __LIBCVOICE_H__
#define __LIBCVOICE_H__

#ifdef MWIN

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include "windsp.h"

#else

#include <sys/soundcard.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>

#endif

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifdef MMT
#include <pthread.h>
#endif

#ifdef MSPX

#include <speex/speex.h>

#endif

#ifdef MIPC
#include <libmipc.h>
#endif

#ifdef MSLOW
#define SLOWDOWN ((500 * MSLOW) + (rand() % (1000 * MSLOW)))
/*#define SLOWDOWN (1000 * MSLOW)*/
#endif

#define NJOIN 8
#define APCK  160
#define SSYNC 65535
#define SYNCSETTLE 3000000
#define NSTEPSSTATS 120

typedef struct _cv_cfg
{
    int myid, targetid, cache_size, debug, quality;
    int timeout, exmode, complexity, vbr, vad, dtx;
    int srv_sock, cli_sock, sync_sock, wide;
    int save, n_sync, hz, port, pidlog, max_oos;
    char *inum, *onum, *ip;
    float enh_r, enh_w;
    time_t init_tm;
} cv_cfg;

#ifdef MSPX

typedef struct _speex_cfg
{
	SpeexBits enc_bits, dec_bits;
	void *enc_state, *dec_state;
	int enc_fsize, dec_fsize;
	int enc_osize, dec_osize;
	short *input, *output;
} speex_cfg;

extern speex_cfg spx;

#ifdef MSPX
void speex_cfg_clear(speex_cfg* c);
#endif

#endif

typedef struct _thread_data_send
{
	void* ptr;
	int l, s;
} thread_data_send;

typedef struct _thread_data_play
{
	char* buff;
	int n, bn, sock, audio;
} thread_data_play;

#ifdef MMT

typedef struct _thread_struct
{
    pthread_t t[NJOIN];
    int n;
} thread_struct;

extern pthread_mutex_t mutex;
extern pthread_t tr_send;
extern pthread_t tr_play;
extern short have_tr_send;
extern short have_tr_play;
extern thread_struct set_tr_send;
extern thread_struct set_tr_play;

#ifdef MNDPLX
extern pthread_mutex_t mutex_audio;
#endif

#endif

#ifdef MIPC

extern int mshmid;
extern key_t mshmkey;
extern void*  mshm_ptr;

#endif

extern cv_cfg cfg;
extern char log_pfx[32];
extern time_t time_start;
extern int n_read, n_write, n_sent, n_recieved, n_steps;

void cv_cfg_clear(cv_cfg* c);
void cv_cfg_free(cv_cfg* c);
unsigned short cvoice_timestamp();
void cvoice_init();
void cvoice_free();
void cvoice_log(const char* fmt, ...);
void cvoice_logd(int lev, const char* fmt, ...);
void cvoice_error(const char* fmt, ...);
void cvoice_traffic_stats();
void cvoice_get_host(char* to, char* name);
void cvoice_help(char* name);
void cvoice_check_config();
int cvoice_mspeex_encode_init();
int cvoice_mspeex_decode_init();
int cvoice_mspeex_encode_destroy();
int cvoice_mspeex_decode_destroy();
int cvoice_mspeex_encode( unsigned char* in, int n, char* out, int* m);    
int cvoice_mspeex_decode( char* in, int n, unsigned char* out, int* m);   
int cvoice_client(int inp);
int cvoice_server();
void cvoice_save_config_fp(FILE* fp, cv_cfg* c);
void cvoice_load_config_fp(FILE* fp, cv_cfg* c);
int cvoice_save_config(const char* fn, cv_cfg* c);
int cvoice_load_config(const char* fn, cv_cfg* c);
void cvoice_init_mipc(int clear);
void cvoice_clear_mshm();
void cvoice_forked(int listener, int* pid1, int* pid2);
void cvoice(int listener);
int cvoice_getopt(int lb, char** par);

#endif

