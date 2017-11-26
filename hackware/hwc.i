# 1 "hwc.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "hwc.c"
# 1 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 1 3 4
# 14 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4
# 1 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include/stdarg.h" 1 3 4
# 40 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 15 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 2 3 4
# 53 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 54 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 2 3 4
# 1 "/usr/include/sys/_null.h" 1 3 4
# 55 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 2 3 4
# 1 "/usr/include/sys/_types.h" 1 3 4
# 33 "/usr/include/sys/_types.h" 3 4
# 1 "/usr/include/machine/_types.h" 1 3 4
# 51 "/usr/include/machine/_types.h" 3 4
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;

__extension__


typedef long long __int64_t;

__extension__


typedef unsigned long long __uint64_t;




typedef unsigned long __clock_t;
typedef __int32_t __critical_t;
typedef long double __double_t;
typedef long double __float_t;
typedef __int32_t __intfptr_t;
typedef __int64_t __intmax_t;
typedef __int32_t __intptr_t;
typedef __int32_t __int_fast8_t;
typedef __int32_t __int_fast16_t;
typedef __int32_t __int_fast32_t;
typedef __int64_t __int_fast64_t;
typedef __int8_t __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;
typedef __int64_t __int_least64_t;
typedef __int32_t __ptrdiff_t;
typedef __int32_t __register_t;
typedef __int32_t __segsz_t;
typedef __uint32_t __size_t;
typedef __int32_t __ssize_t;
typedef __int32_t __time_t;
typedef __uint32_t __uintfptr_t;
typedef __uint64_t __uintmax_t;
typedef __uint32_t __uintptr_t;
typedef __uint32_t __uint_fast8_t;
typedef __uint32_t __uint_fast16_t;
typedef __uint32_t __uint_fast32_t;
typedef __uint64_t __uint_fast64_t;
typedef __uint8_t __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;
typedef __uint64_t __uint_least64_t;
typedef __uint32_t __u_register_t;
typedef __uint32_t __vm_offset_t;
typedef __int64_t __vm_ooffset_t;



typedef __uint32_t __vm_paddr_t;

typedef __uint64_t __vm_pindex_t;
typedef __uint32_t __vm_size_t;





typedef __builtin_va_list __va_list;
# 34 "/usr/include/sys/_types.h" 2 3 4




typedef __uint32_t __blksize_t;
typedef __int64_t __blkcnt_t;
typedef __int32_t __clockid_t;
typedef __uint64_t __cap_rights_t;
typedef __uint32_t __fflags_t;
typedef __uint64_t __fsblkcnt_t;
typedef __uint64_t __fsfilcnt_t;
typedef __uint32_t __gid_t;
typedef __int64_t __id_t;
typedef __uint32_t __ino_t;
typedef long __key_t;
typedef __int32_t __lwpid_t;
typedef __uint16_t __mode_t;
typedef int __accmode_t;
typedef int __nl_item;
typedef __uint16_t __nlink_t;
typedef __int64_t __off_t;
typedef __int32_t __pid_t;
typedef __int64_t __rlim_t;


typedef __uint8_t __sa_family_t;
typedef __uint32_t __socklen_t;
typedef long __suseconds_t;
typedef struct __timer *__timer_t;
typedef struct __mq *__mqd_t;
typedef __uint32_t __uid_t;
typedef unsigned int __useconds_t;
typedef int __cpuwhich_t;
typedef int __cpulevel_t;
typedef int __cpusetid_t;
# 88 "/usr/include/sys/_types.h" 3 4
typedef int __ct_rune_t;
typedef __ct_rune_t __rune_t;
typedef __ct_rune_t __wchar_t;
typedef __ct_rune_t __wint_t;

typedef __uint32_t __dev_t;

typedef __uint32_t __fixpt_t;





typedef union {
 char __mbstate8[128];
 __int64_t _mbstateL;
} __mbstate_t;
# 56 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 2 3 4

typedef __off_t fpos_t;


typedef __size_t size_t;






typedef __off_t off_t;



typedef __ssize_t ssize_t;





typedef __va_list __not_va_list__;
# 91 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4
struct __sbuf {
 unsigned char *_base;
 int _size;
};
# 124 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4
typedef struct __sFILE {
 unsigned char *_p;
 int _r;
 int _w;
 short _flags;
 short _file;
 struct __sbuf _bf;
 int _lbfsize;


 void *_cookie;
 int (*_close)(void *);
 int (*_read)(void *, char *, int);
 fpos_t (*_seek)(void *, fpos_t, int);
 int (*_write)(void *, const char *, int);


 struct __sbuf _ub;
 unsigned char *_up;
 int _ur;


 unsigned char _ubuf[3];
 unsigned char _nbuf[1];


 struct __sbuf _lb;


 int _blksize;
 fpos_t _offset;

 struct pthread_mutex *_fl_mutex;
 struct pthread *_fl_owner;
 int _fl_count;
 int _orientation;
 __mbstate_t _mbstate;
} FILE;



extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

# 238 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4




void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE * , fpos_t * );
char *fgets(char * , int, FILE * );
FILE *fopen(const char * , const char * );
int fprintf(FILE * , const char * , ...);
int fputc(int, FILE *);
int fputs(const char * , FILE * );
size_t fread(void * , size_t, size_t, FILE * );
FILE *freopen(const char * , const char * , FILE * );
int fscanf(FILE * , const char * , ...);
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
size_t fwrite(const void * , size_t, size_t, FILE * );
int getc(FILE *);
int getchar(void);
char *gets(char *);
void perror(const char *);
int printf(const char * , ...);
int putc(int, FILE *);
int putchar(int);
int puts(const char *);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
int scanf(const char * , ...);
void setbuf(FILE * , char * );
int setvbuf(FILE * , char * , int, size_t);
int sprintf(char * , const char * , ...);
int sscanf(const char * , const char * , ...);
FILE *tmpfile(void);
char *tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE * , const char * ,
     __gnuc_va_list);
int vprintf(const char * , __gnuc_va_list);
int vsprintf(char * , const char * ,
     __gnuc_va_list);


int snprintf(char * , size_t, const char * ,
     ...) __attribute__((__format__ (__printf__, 3, 4)));
int vfscanf(FILE * , const char * , __gnuc_va_list)
     __attribute__((__format__ (__scanf__, 2, 0)));
int vscanf(const char * , __gnuc_va_list) __attribute__((__format__ (__scanf__, 1, 0)));
int vsnprintf(char * , size_t, const char * ,
     __gnuc_va_list) __attribute__((__format__ (__printf__, 3, 0)));
int vsscanf(const char * , const char * , __gnuc_va_list)
     __attribute__((__format__ (__scanf__, 2, 0)));
# 308 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4
char *ctermid(char *);
FILE *fdopen(int, const char *);
int fileno(FILE *);



int pclose(FILE *);
FILE *popen(const char *, const char *);



int ftrylockfile(FILE *);
void flockfile(FILE *);
void funlockfile(FILE *);





int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);


void clearerr_unlocked(FILE *);
int feof_unlocked(FILE *);
int ferror_unlocked(FILE *);
int fileno_unlocked(FILE *);



int fseeko(FILE *, __off_t, int);
__off_t ftello(FILE *);



int getw(FILE *);
int putw(int, FILE *);



char *tempnam(const char *, const char *);



ssize_t getdelim(char ** , size_t * , int,
     FILE * );
int renameat(int, const char *, int, const char *);
int vdprintf(int, const char * , __gnuc_va_list);
# 402 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4
int asprintf(char **, const char *, ...) __attribute__((__format__ (__printf__, 2, 3)));
char *ctermid_r(char *);
void fcloseall(void);
char *fgetln(FILE *, size_t *);
const char *fmtcheck(const char *, const char *) __attribute__((__format_arg__ (2)));
int fpurge(FILE *);
void setbuffer(FILE *, char *, int);
int setlinebuf(FILE *);
int vasprintf(char **, const char *, __gnuc_va_list)
     __attribute__((__format__ (__printf__, 2, 0)));






extern const int sys_nerr;
extern const char *const sys_errlist[];




FILE *funopen(const void *,
     int (*)(void *, char *, int),
     int (*)(void *, const char *, int),
     fpos_t (*)(void *, fpos_t, int),
     int (*)(void *));
# 437 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4
int ftruncate(int, __off_t);



__off_t lseek(int, __off_t, int);



void *mmap(void *, size_t, int, int, int, __off_t);



int truncate(const char *, __off_t);






int __srget(FILE *);
int __swbuf(int, FILE *);







static __inline int __sputc(int _c, FILE *_p) {
 if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
  return (*_p->_p++ = _c);
 else
  return (__swbuf(_c, _p));
}
# 490 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4
extern int __isthreaded;
# 524 "/usr/local/lib/gcc46/gcc/i386-portbld-freebsd9.0/4.6.3/include-fixed/stdio.h" 3 4

# 2 "hwc.c" 2
# 1 "/usr/include/signal.h" 1 3 4
# 38 "/usr/include/signal.h" 3 4
# 1 "/usr/include/sys/signal.h" 1 3 4
# 43 "/usr/include/sys/signal.h" 3 4
# 1 "/usr/include/sys/_sigset.h" 1 3 4
# 51 "/usr/include/sys/_sigset.h" 3 4
typedef struct __sigset {
 __uint32_t __bits[4];
} __sigset_t;
# 44 "/usr/include/sys/signal.h" 2 3 4

# 1 "/usr/include/machine/_limits.h" 1 3 4
# 46 "/usr/include/sys/signal.h" 2 3 4
# 1 "/usr/include/machine/signal.h" 1 3 4
# 43 "/usr/include/machine/signal.h" 3 4
typedef int sig_atomic_t;


# 1 "/usr/include/machine/trap.h" 1 3 4
# 47 "/usr/include/machine/signal.h" 2 3 4
# 88 "/usr/include/machine/signal.h" 3 4
struct sigcontext {
 struct __sigset sc_mask;
 int sc_onstack;
 int sc_gs;
 int sc_fs;
 int sc_es;
 int sc_ds;
 int sc_edi;
 int sc_esi;
 int sc_ebp;
 int sc_isp;
 int sc_ebx;
 int sc_edx;
 int sc_ecx;
 int sc_eax;
 int sc_trapno;
 int sc_err;
 int sc_eip;
 int sc_cs;
 int sc_efl;
 int sc_esp;
 int sc_ss;
 int sc_len;




 int sc_fpformat;
 int sc_ownedfp;
 int sc_spare1[1];
 int sc_fpstate[128] __attribute__((__aligned__(16)));

 int sc_fsbase;
 int sc_gsbase;

 int sc_spare2[6];
};
# 47 "/usr/include/sys/signal.h" 2 3 4
# 141 "/usr/include/sys/signal.h" 3 4
typedef void __sighandler_t(int);




typedef __sigset_t sigset_t;




union sigval {

 int sival_int;
 void *sival_ptr;

 int sigval_int;
 void *sigval_ptr;
};



struct sigevent {
 int sigev_notify;
 int sigev_signo;
 union sigval sigev_value;
 union {
  __lwpid_t _threadid;
  struct {
   void (*_function)(union sigval);
   void *_attribute;
  } _sigev_thread;
  long __spare__[8];
 } _sigev_un;
};
# 193 "/usr/include/sys/signal.h" 3 4
typedef struct __siginfo {
 int si_signo;
 int si_errno;






 int si_code;
 __pid_t si_pid;
 __uid_t si_uid;
 int si_status;
 void *si_addr;
 union sigval si_value;
 union {
  struct {
   int _trapno;
  } _fault;
  struct {
   int _timerid;
   int _overrun;
  } _timer;
  struct {
   int _mqd;
  } _mesgq;
  struct {
   long _band;
  } _poll;
  struct {
   long __spare1__;
   int __spare2__[7];
  } __spare__;
 } _reason;
} siginfo_t;
# 292 "/usr/include/sys/signal.h" 3 4
struct __siginfo;




struct sigaction {
 union {
  void (*__sa_handler)(int);
  void (*__sa_sigaction)(int, struct __siginfo *, void *);
 } __sigaction_u;
 int sa_flags;
 sigset_t sa_mask;
};
# 349 "/usr/include/sys/signal.h" 3 4
typedef __sighandler_t *sig_t;
typedef void __siginfohandler_t(int, struct __siginfo *, void *);







typedef struct sigaltstack {



 char *ss_sp;
 __size_t ss_size;
 int ss_flags;
} stack_t;
# 378 "/usr/include/sys/signal.h" 3 4
struct sigvec {
 __sighandler_t *sv_handler;
 int sv_mask;
 int sv_flags;
};
# 405 "/usr/include/sys/signal.h" 3 4
struct sigstack {

 char *ss_sp;
 int ss_onstack;
};
# 437 "/usr/include/sys/signal.h" 3 4

__sighandler_t *signal(int, __sighandler_t *);

# 39 "/usr/include/signal.h" 2 3 4






extern const char *const sys_signame[32];
extern const char *const sys_siglist[32];
extern const int sys_nsig;




typedef __pid_t pid_t;





struct pthread;
typedef struct pthread *__pthread_t;

typedef __pthread_t pthread_t;





int raise(int);


int kill(__pid_t, int);
int pthread_kill(__pthread_t, int);
int pthread_sigmask(int, const __sigset_t * ,
     __sigset_t * );
int sigaction(int, const struct sigaction * ,
     struct sigaction * );
int sigaddset(sigset_t *, int);
int sigdelset(sigset_t *, int);
int sigemptyset(sigset_t *);
int sigfillset(sigset_t *);
int sigismember(const sigset_t *, int);
int sigpending(sigset_t *);
int sigprocmask(int, const sigset_t * , sigset_t * );
int sigsuspend(const sigset_t *);
int sigwait(const sigset_t * , int * );



int sigqueue(__pid_t, int, const union sigval);

struct timespec;
int sigtimedwait(const sigset_t * , siginfo_t * ,
     const struct timespec * );
int sigwaitinfo(const sigset_t * , siginfo_t * );



int killpg(__pid_t, int);
int sigaltstack(const stack_t * , stack_t * );
int sighold(int);
int sigignore(int);
int sigpause(int);
int sigrelse(int);
void (*sigset(int, void (*)(int)))(int);
int xsi_sigpause(int);



int siginterrupt(int, int);



void psignal(unsigned int, const char *);



int sigblock(int);
struct __ucontext;
int sigreturn(const struct __ucontext *);
int sigsetmask(int);
int sigstack(const struct sigstack *, struct sigstack *);
int sigvec(int, struct sigvec *, struct sigvec *);


# 3 "hwc.c" 2





register volatile unsigned long ticks asm("edi");

float gops;
int ft;
int av;

void catch_int(int signo)
{
     if (ft)
     {
  fflush(__stdoutp);
  ft = 0;
  ticks = 0;
  av = 0;
  return;
     }

     av ++;
     gops += (float)(ticks * (1000000 / 200000)) / 1000000000.;

     ticks = 0;

     if (av == (5000000 / 200000))
     {
  gops /= (float)(5000000 / 200000);
  printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b%12.6f GOPS", gops);
  fflush(__stdoutp);
  av = 0;
  gops = 0.;
     }
}

void signal_setup()
{
    static struct sigaction act;
    act.__sigaction_u.__sa_handler = catch_int;
    sigfillset(&(act.sa_mask));
    sigaction(14, &act, ((void *)0));
}

void hwc()
{
    signal_setup();

    ft = 1;
    ticks = 0;
    av = 0;

    ualarm(200000, 200000);

    while (1)
    {
 ticks ++;
    }
}

int main(int lb, char** par)
{
    hwc();
    return 0;
}
