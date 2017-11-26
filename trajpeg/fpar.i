# 1 "fpar.c"
# 1 "<command-line>"
# 1 "fpar.c"
# 1 "fpar.h" 1
# 1 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 1 3 4
# 45 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 3 4
# 1 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/sys/cdefs.h" 1 3 4
# 46 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 2 3 4
# 1 "/usr/include/sys/_null.h" 1 3 4
# 47 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 2 3 4
# 1 "/usr/include/sys/_types.h" 1 3 4
# 33 "/usr/include/sys/_types.h" 3 4
# 1 "/usr/include/machine/_types.h" 1 3 4





# 1 "/usr/include/x86/_types.h" 1 3 4
# 51 "/usr/include/x86/_types.h" 3 4
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;

typedef long __int64_t;
typedef unsigned long __uint64_t;
# 77 "/usr/include/x86/_types.h" 3 4
typedef __int32_t __clock_t;
typedef __int64_t __critical_t;
typedef double __double_t;
typedef float __float_t;
typedef __int64_t __intfptr_t;
typedef __int64_t __intptr_t;
# 91 "/usr/include/x86/_types.h" 3 4
typedef __int64_t __intmax_t;
typedef __int32_t __int_fast8_t;
typedef __int32_t __int_fast16_t;
typedef __int32_t __int_fast32_t;
typedef __int64_t __int_fast64_t;
typedef __int8_t __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;
typedef __int64_t __int_least64_t;

typedef __int64_t __ptrdiff_t;
typedef __int64_t __register_t;
typedef __int64_t __segsz_t;
typedef __uint64_t __size_t;
typedef __int64_t __ssize_t;
typedef __int64_t __time_t;
typedef __uint64_t __uintfptr_t;
typedef __uint64_t __uintptr_t;
# 119 "/usr/include/x86/_types.h" 3 4
typedef __uint64_t __uintmax_t;
typedef __uint32_t __uint_fast8_t;
typedef __uint32_t __uint_fast16_t;
typedef __uint32_t __uint_fast32_t;
typedef __uint64_t __uint_fast64_t;
typedef __uint8_t __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;
typedef __uint64_t __uint_least64_t;

typedef __uint64_t __u_register_t;
typedef __uint64_t __vm_offset_t;
typedef __uint64_t __vm_paddr_t;
typedef __uint64_t __vm_size_t;
# 143 "/usr/include/x86/_types.h" 3 4
typedef __int64_t __vm_ooffset_t;
typedef __uint64_t __vm_pindex_t;
typedef int __wchar_t;
# 154 "/usr/include/x86/_types.h" 3 4
typedef __builtin_va_list __va_list;






typedef __va_list __gnuc_va_list;
# 6 "/usr/include/machine/_types.h" 2 3 4
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
typedef __ct_rune_t __wint_t;

typedef __uint32_t __dev_t;

typedef __uint32_t __fixpt_t;





typedef union {
 char __mbstate8[128];
 __int64_t _mbstateL;
} __mbstate_t;
# 48 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 2 3 4



typedef __rune_t rune_t;







typedef long unsigned int size_t;
# 69 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 3 4
typedef int wchar_t;






typedef struct {
 int quot;
 int rem;
} div_t;

typedef struct {
 long quot;
 long rem;
} ldiv_t;










extern int __mb_cur_max;
extern int ___mb_cur_max(void);


__attribute__((__noreturn__)) void abort(void);
int abs(int) __attribute__((__const__));
int atexit(void (*)(void));
double atof(const char *);
int atoi(const char *);
long atol(const char *);
void *bsearch(const void *, const void *, size_t,
     size_t, int (*)(const void *, const void *));
void *calloc(size_t, size_t) __attribute__((__malloc__));
div_t div(int, int) __attribute__((__const__));
__attribute__((__noreturn__)) void exit(int);
void free(void *);
char *getenv(const char *);
long labs(long) __attribute__((__const__));
ldiv_t ldiv(long, long) __attribute__((__const__));
void *malloc(size_t) __attribute__((__malloc__));
int mblen(const char *, size_t);
size_t mbstowcs(wchar_t * , const char * , size_t);
int mbtowc(wchar_t * , const char * , size_t);
void qsort(void *, size_t, size_t,
     int (*)(const void *, const void *));
int rand(void);
void *realloc(void *, size_t);
void srand(unsigned);
double strtod(const char * , char ** );
float strtof(const char * , char ** );
long strtol(const char * , char ** , int);
long double
  strtold(const char * , char ** );
unsigned long
  strtoul(const char * , char ** , int);
int system(const char *);
int wctomb(char *, wchar_t);
size_t wcstombs(char * , const wchar_t * , size_t);
# 168 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 3 4
__attribute__((__noreturn__)) void _Exit(int);






void * aligned_alloc(size_t, size_t) __attribute__((__malloc__));
int at_quick_exit(void (*)(void));
__attribute__((__noreturn__)) void
 quick_exit(int);







int posix_memalign(void **, size_t, size_t);
int rand_r(unsigned *);
char *realpath(const char * , char * );
int setenv(const char *, const char *, int);
int unsetenv(const char *);



int getsubopt(char **, char *const *, char **);

char *mkdtemp(char *);



int mkstemp(char *);
# 213 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 3 4
long a64l(const char *);
double drand48(void);

double erand48(unsigned short[3]);


int grantpt(int);
char *initstate(unsigned long , char *, long);
long jrand48(unsigned short[3]);
char *l64a(long);
void lcong48(unsigned short[7]);
long lrand48(void);

char *mktemp(char *);


long mrand48(void);
long nrand48(unsigned short[3]);
int posix_openpt(int);
char *ptsname(int);
int putenv(char *);
long random(void);
unsigned short
 *seed48(unsigned short[3]);

int setkey(const char *);


char *setstate( char *);
void srand48(long);
void srandom(unsigned long);
int unlockpt(int);



extern const char *malloc_conf;
extern void (*malloc_message)(void *, const char *);
# 267 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 3 4
void abort2(const char *, int, void **) __attribute__((__noreturn__));
__uint32_t
  arc4random(void);
void arc4random_addrandom(unsigned char *, int);
void arc4random_buf(void *, size_t);
void arc4random_stir(void);
__uint32_t
  arc4random_uniform(__uint32_t);
char *getbsize(int *, long *);

char *cgetcap(char *, const char *, int);
int cgetclose(void);
int cgetent(char **, char **, const char *);
int cgetfirst(char **, char **);
int cgetmatch(const char *, const char *);
int cgetnext(char **, char **);
int cgetnum(char *, const char *, long *);
int cgetset(const char *);
int cgetstr(char *, const char *, char **);
int cgetustr(char *, const char *, char **);

int daemon(int, int);
char *devname(__dev_t, __mode_t);
char *devname_r(__dev_t, __mode_t, char *, int);
char *fdevname(int);
char *fdevname_r(int, char *, int);
int getloadavg(double [], int);
const char *
  getprogname(void);

int heapsort(void *, size_t, size_t, int (*)(const void *, const void *));
int l64a_r(long, char *, int);
int mergesort(void *, size_t, size_t, int (*)(const void *, const void *));
void qsort_r(void *, size_t, size_t, void *,
     int (*)(void *, const void *, const void *));
int radixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void *reallocf(void *, size_t);
int rpmatch(const char *);
void setprogname(const char *);
int sradixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void sranddev(void);
void srandomdev(void);
long long
 strtonum(const char *, long long, long long, const char **);


__int64_t
  strtoq(const char *, char **, int);
__uint64_t
  strtouq(const char *, char **, int);

extern char *suboptarg;


# 2 "fpar.h" 2
# 1 "/usr/include/string.h" 1 3 4
# 45 "/usr/include/string.h" 3 4
# 1 "/usr/include/strings.h" 1 3 4
# 40 "/usr/include/strings.h" 3 4


int bcmp(const void *, const void *, size_t) __attribute__((__pure__));
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);


int ffs(int) __attribute__((__const__));


int ffsl(long) __attribute__((__const__));
int ffsll(long long) __attribute__((__const__));
int fls(int) __attribute__((__const__));
int flsl(long) __attribute__((__const__));
int flsll(long long) __attribute__((__const__));


char *index(const char *, int) __attribute__((__pure__));
char *rindex(const char *, int) __attribute__((__pure__));

int strcasecmp(const char *, const char *) __attribute__((__pure__));
int strncasecmp(const char *, const char *, size_t) __attribute__((__pure__));

# 46 "/usr/include/string.h" 2 3 4









void *memccpy(void * , const void * , int, size_t);

void *memchr(const void *, int, size_t) __attribute__((__pure__));

void *memrchr(const void *, int, size_t) __attribute__((__pure__));

int memcmp(const void *, const void *, size_t) __attribute__((__pure__));
void *memcpy(void * , const void * , size_t);

void *memmem(const void *, size_t, const void *, size_t) __attribute__((__pure__));

void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);

char *stpcpy(char * , const char * );
char *stpncpy(char * , const char * , size_t);


char *strcasestr(const char *, const char *) __attribute__((__pure__));

char *strcat(char * , const char * );
char *strchr(const char *, int) __attribute__((__pure__));
int strcmp(const char *, const char *) __attribute__((__pure__));
int strcoll(const char *, const char *);
char *strcpy(char * , const char * );
size_t strcspn(const char *, const char *) __attribute__((__pure__));

char *strdup(const char *) __attribute__((__malloc__));

char *strerror(int);

int strerror_r(int, char *, size_t);


size_t strlcat(char * , const char * , size_t);
size_t strlcpy(char * , const char * , size_t);

size_t strlen(const char *) __attribute__((__pure__));

void strmode(int, char *);

char *strncat(char * , const char * , size_t);
int strncmp(const char *, const char *, size_t) __attribute__((__pure__));
char *strncpy(char * , const char * , size_t);

char *strndup(const char *, size_t) __attribute__((__malloc__));
size_t strnlen(const char *, size_t) __attribute__((__pure__));


char *strnstr(const char *, const char *, size_t) __attribute__((__pure__));

char *strpbrk(const char *, const char *) __attribute__((__pure__));
char *strrchr(const char *, int) __attribute__((__pure__));

char *strsep(char **, const char *);


char *strsignal(int);

size_t strspn(const char *, const char *) __attribute__((__pure__));
char *strstr(const char *, const char *) __attribute__((__pure__));
char *strtok(char * , const char * );

char *strtok_r(char *, const char *, char **);

size_t strxfrm(char * , const char * , size_t);






typedef __ssize_t ssize_t;



void swab(const void * , void * , ssize_t);





# 1 "/usr/include/xlocale/_string.h" 1 3 4
# 31 "/usr/include/xlocale/_string.h" 3 4
typedef struct _xlocale *locale_t;
# 46 "/usr/include/xlocale/_string.h" 3 4
int strcoll_l(const char *, const char *, locale_t);
size_t strxfrm_l(char *, const char *, size_t, locale_t);
# 138 "/usr/include/string.h" 2 3 4


# 3 "fpar.h" 2
# 1 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 1 3 4
# 14 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
# 1 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include/stdarg.h" 1 3 4
# 15 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 2 3 4
# 57 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
typedef __off_t fpos_t;
# 67 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
typedef __off_t off_t;
# 77 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
typedef __va_list __not_va_list__;
# 91 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
struct __sbuf {
 unsigned char *_base;
 int _size;
};
# 124 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
struct __sFILE {
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
};


typedef struct __sFILE FILE;



extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

# 241 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4







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
# 314 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
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
# 408 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
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
extern const char * const sys_errlist[];




FILE *funopen(const void *,
     int (*)(void *, char *, int),
     int (*)(void *, const char *, int),
     fpos_t (*)(void *, fpos_t, int),
     int (*)(void *));
# 443 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
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
# 491 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
extern int __isthreaded;
# 534 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4

# 4 "fpar.h" 2
# 1 "/usr/include/ctype.h" 1 3 4
# 46 "/usr/include/ctype.h" 3 4
# 1 "/usr/include/_ctype.h" 1 3 4
# 70 "/usr/include/_ctype.h" 3 4

unsigned long ___runetype(__ct_rune_t) __attribute__((__pure__));
__ct_rune_t ___tolower(__ct_rune_t) __attribute__((__pure__));
__ct_rune_t ___toupper(__ct_rune_t) __attribute__((__pure__));

# 86 "/usr/include/_ctype.h" 3 4
extern int __mb_sb_limit;







# 1 "/usr/include/runetype.h" 1 3 4
# 48 "/usr/include/runetype.h" 3 4
typedef struct {
 __rune_t __min;
 __rune_t __max;
 __rune_t __map;
 unsigned long *__types;
} _RuneEntry;

typedef struct {
 int __nranges;
 _RuneEntry *__ranges;
} _RuneRange;

typedef struct {
 char __magic[8];
 char __encoding[32];

 __rune_t (*__sgetrune)(const char *, __size_t, char const **);
 int (*__sputrune)(__rune_t, char *, __size_t, char **);
 __rune_t __invalid_rune;

 unsigned long __runetype[(1 <<8 )];
 __rune_t __maplower[(1 <<8 )];
 __rune_t __mapupper[(1 <<8 )];






 _RuneRange __runetype_ext;
 _RuneRange __maplower_ext;
 _RuneRange __mapupper_ext;

 void *__variable;
 int __variable_len;
} _RuneLocale;



extern const _RuneLocale _DefaultRuneLocale;
extern const _RuneLocale *_CurrentRuneLocale;



extern __thread const _RuneLocale *_ThreadRuneLocale;
static __inline const _RuneLocale *__getCurrentRuneLocale(void)
{

 if (_ThreadRuneLocale)
  return _ThreadRuneLocale;
 if (_CurrentRuneLocale)
  return _CurrentRuneLocale;
 return &_DefaultRuneLocale;
}



# 95 "/usr/include/_ctype.h" 2 3 4

static __inline int
__maskrune(__ct_rune_t _c, unsigned long _f)
{
 return ((_c < 0 || _c >= (1 <<8 )) ? ___runetype(_c) :
  (__getCurrentRuneLocale())->__runetype[_c]) & _f;
}

static __inline int
__sbmaskrune(__ct_rune_t _c, unsigned long _f)
{
 return (_c < 0 || _c >= __mb_sb_limit) ? 0 :
        (__getCurrentRuneLocale())->__runetype[_c] & _f;
}

static __inline int
__istype(__ct_rune_t _c, unsigned long _f)
{
 return (!!__maskrune(_c, _f));
}

static __inline int
__sbistype(__ct_rune_t _c, unsigned long _f)
{
 return (!!__sbmaskrune(_c, _f));
}

static __inline int
__isctype(__ct_rune_t _c, unsigned long _f)
{
 return (_c < 0 || _c >= 128) ? 0 :
        !!(_DefaultRuneLocale.__runetype[_c] & _f);
}

static __inline __ct_rune_t
__toupper(__ct_rune_t _c)
{
 return (_c < 0 || _c >= (1 <<8 )) ? ___toupper(_c) :
        (__getCurrentRuneLocale())->__mapupper[_c];
}

static __inline __ct_rune_t
__sbtoupper(__ct_rune_t _c)
{
 return (_c < 0 || _c >= __mb_sb_limit) ? _c :
        (__getCurrentRuneLocale())->__mapupper[_c];
}

static __inline __ct_rune_t
__tolower(__ct_rune_t _c)
{
 return (_c < 0 || _c >= (1 <<8 )) ? ___tolower(_c) :
        (__getCurrentRuneLocale())->__maplower[_c];
}

static __inline __ct_rune_t
__sbtolower(__ct_rune_t _c)
{
 return (_c < 0 || _c >= __mb_sb_limit) ? _c :
        (__getCurrentRuneLocale())->__maplower[_c];
}

static __inline int
__wcwidth(__ct_rune_t _c)
{
 unsigned int _x;

 if (_c == 0)
  return (0);
 _x = (unsigned int)__maskrune(_c, 0xe0000000L|0x00040000L);
 if ((_x & 0xe0000000L) != 0)
  return ((_x & 0xe0000000L) >> 30);
 return ((_x & 0x00040000L) != 0 ? 1 : -1);
}
# 47 "/usr/include/ctype.h" 2 3 4


int isalnum(int);
int isalpha(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int tolower(int);
int toupper(int);


int isascii(int);
int toascii(int);



int isblank(int);



int digittoint(int);
int ishexnumber(int);
int isideogram(int);
int isnumber(int);
int isphonogram(int);
int isrune(int);
int isspecial(int);



# 1 "/usr/include/xlocale/_ctype.h" 1 3 4
# 49 "/usr/include/xlocale/_ctype.h" 3 4
unsigned long ___runetype_l(__ct_rune_t, locale_t) __attribute__((__pure__));
__ct_rune_t ___tolower_l(__ct_rune_t, locale_t) __attribute__((__pure__));
__ct_rune_t ___toupper_l(__ct_rune_t, locale_t) __attribute__((__pure__));
_RuneLocale *__runes_for_locale(locale_t, int*);
# 96 "/usr/include/xlocale/_ctype.h" 3 4
extern __inline int
__sbmaskrune_l(__ct_rune_t __c, unsigned long __f, locale_t __loc);
extern __inline int
__sbistype_l(__ct_rune_t __c, unsigned long __f, locale_t __loc);

extern __inline int
__sbmaskrune_l(__ct_rune_t __c, unsigned long __f, locale_t __loc)
{
 int __limit;
 _RuneLocale *runes = __runes_for_locale(__loc, &__limit);
 return (__c < 0 || __c >= __limit) ? 0 :
        runes->__runetype[__c] & __f;
}

extern __inline int
__sbistype_l(__ct_rune_t __c, unsigned long __f, locale_t __loc)
{
 return (!!__sbmaskrune_l(__c, __f, __loc));
}







extern __inline int isalnum_l(int, locale_t); extern __inline int isalnum_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000100L|0x00000400L, __l); }
extern __inline int isalpha_l(int, locale_t); extern __inline int isalpha_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000100L, __l); }
extern __inline int isblank_l(int, locale_t); extern __inline int isblank_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00020000L, __l); }
extern __inline int iscntrl_l(int, locale_t); extern __inline int iscntrl_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000200L, __l); }
extern __inline int isdigit_l(int, locale_t); extern __inline int isdigit_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000400L, __l); }
extern __inline int isgraph_l(int, locale_t); extern __inline int isgraph_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000800L, __l); }
extern __inline int ishexnumber_l(int, locale_t); extern __inline int ishexnumber_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00010000L, __l); }
extern __inline int isideogram_l(int, locale_t); extern __inline int isideogram_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00080000L, __l); }
extern __inline int islower_l(int, locale_t); extern __inline int islower_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00001000L, __l); }
extern __inline int isnumber_l(int, locale_t); extern __inline int isnumber_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000400L, __l); }
extern __inline int isphonogram_l(int, locale_t); extern __inline int isphonogram_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00200000L, __l); }
extern __inline int isprint_l(int, locale_t); extern __inline int isprint_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00040000L, __l); }
extern __inline int ispunct_l(int, locale_t); extern __inline int ispunct_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00002000L, __l); }
extern __inline int isrune_l(int, locale_t); extern __inline int isrune_l(int __c, locale_t __l) { return __sbistype_l(__c, 0xFFFFFF00L, __l); }
extern __inline int isspace_l(int, locale_t); extern __inline int isspace_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00004000L, __l); }
extern __inline int isspecial_l(int, locale_t); extern __inline int isspecial_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00100000L, __l); }
extern __inline int isupper_l(int, locale_t); extern __inline int isupper_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00008000L, __l); }
extern __inline int isxdigit_l(int, locale_t); extern __inline int isxdigit_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00010000L, __l); }
# 180 "/usr/include/xlocale/_ctype.h" 3 4
extern __inline int digittoint_l(int, locale_t);
extern __inline int tolower_l(int, locale_t);
extern __inline int toupper_l(int, locale_t);

extern __inline int digittoint_l(int __c, locale_t __l)
{ return __sbmaskrune_l((__c), 0xFF, __l); }

extern __inline int tolower_l(int __c, locale_t __l)
{
 int __limit;
 _RuneLocale *__runes = __runes_for_locale(__l, &__limit);
 return (__c < 0 || __c >= __limit) ? __c :
        __runes->__maplower[__c];
}
extern __inline int toupper_l(int __c, locale_t __l)
{
 int __limit;
 _RuneLocale *__runes = __runes_for_locale(__l, &__limit);
 return (__c < 0 || __c >= __limit) ? __c :
        __runes->__mapupper[__c];
}
# 84 "/usr/include/ctype.h" 2 3 4


# 5 "fpar.h" 2
# 1 "/usr/include/math.h" 1 3 4
# 22 "/usr/include/math.h" 3 4
# 1 "/usr/include/machine/_limits.h" 1 3 4





# 1 "/usr/include/x86/_limits.h" 1 3 4
# 6 "/usr/include/machine/_limits.h" 2 3 4
# 23 "/usr/include/math.h" 2 3 4




extern const union __infinity_un {
 unsigned char __uc[8];
 double __ud;
} __infinity;

extern const union __nan_un {
 unsigned char __uc[sizeof(float)];
 float __uf;
} __nan;
# 127 "/usr/include/math.h" 3 4
typedef __double_t double_t;
typedef __float_t float_t;
# 150 "/usr/include/math.h" 3 4
extern int signgam;
# 167 "/usr/include/math.h" 3 4




int __fpclassifyd(double) __attribute__((__const__));
int __fpclassifyf(float) __attribute__((__const__));
int __fpclassifyl(long double) __attribute__((__const__));
int __isfinitef(float) __attribute__((__const__));
int __isfinite(double) __attribute__((__const__));
int __isfinitel(long double) __attribute__((__const__));
int __isinff(float) __attribute__((__const__));
int __isinfl(long double) __attribute__((__const__));
int __isnanf(float) __attribute__((__const__));
int __isnanl(long double) __attribute__((__const__));
int __isnormalf(float) __attribute__((__const__));
int __isnormal(double) __attribute__((__const__));
int __isnormall(long double) __attribute__((__const__));
int __signbit(double) __attribute__((__const__));
int __signbitf(float) __attribute__((__const__));
int __signbitl(long double) __attribute__((__const__));

double acos(double);
double asin(double);
double atan(double);
double atan2(double, double);
double cos(double);
double sin(double);
double tan(double);

double cosh(double);
double sinh(double);
double tanh(double);

double exp(double);
double frexp(double, int *);
double ldexp(double, int);
double log(double);
double log10(double);
double modf(double, double *);

double pow(double, double);
double sqrt(double);

double ceil(double);
double fabs(double) __attribute__((__const__));
double floor(double);
double fmod(double, double);





double acosh(double);
double asinh(double);
double atanh(double);
double cbrt(double);
double erf(double);
double erfc(double);
double exp2(double);
double expm1(double);
double fma(double, double, double);
double hypot(double, double);
int ilogb(double) __attribute__((__const__));
int (isinf)(double) __attribute__((__const__));
int (isnan)(double) __attribute__((__const__));
double lgamma(double);
long long llrint(double);
long long llround(double);
double log1p(double);
double log2(double);
double logb(double);
long lrint(double);
long lround(double);
double nan(const char *) __attribute__((__const__));
double nextafter(double, double);
double remainder(double, double);
double remquo(double, double, int *);
double rint(double);



double j0(double);
double j1(double);
double jn(int, double);
double y0(double);
double y1(double);
double yn(int, double);


double gamma(double);



double scalb(double, double);




double copysign(double, double) __attribute__((__const__));
double fdim(double, double);
double fmax(double, double) __attribute__((__const__));
double fmin(double, double) __attribute__((__const__));
double nearbyint(double);
double round(double);
double scalbln(double, long);
double scalbn(double, int);
double tgamma(double);
double trunc(double);






double drem(double, double);
int finite(double) __attribute__((__const__));
int isnanf(float) __attribute__((__const__));





double gamma_r(double, int *);
double lgamma_r(double, int *);




double significand(double);




float acosf(float);
float asinf(float);
float atanf(float);
float atan2f(float, float);
float cosf(float);
float sinf(float);
float tanf(float);

float coshf(float);
float sinhf(float);
float tanhf(float);

float exp2f(float);
float expf(float);
float expm1f(float);
float frexpf(float, int *);
int ilogbf(float) __attribute__((__const__));
float ldexpf(float, int);
float log10f(float);
float log1pf(float);
float log2f(float);
float logf(float);
float modff(float, float *);

float powf(float, float);
float sqrtf(float);

float ceilf(float);
float fabsf(float) __attribute__((__const__));
float floorf(float);
float fmodf(float, float);
float roundf(float);

float erff(float);
float erfcf(float);
float hypotf(float, float);
float lgammaf(float);
float tgammaf(float);

float acoshf(float);
float asinhf(float);
float atanhf(float);
float cbrtf(float);
float logbf(float);
float copysignf(float, float) __attribute__((__const__));
long long llrintf(float);
long long llroundf(float);
long lrintf(float);
long lroundf(float);
float nanf(const char *) __attribute__((__const__));
float nearbyintf(float);
float nextafterf(float, float);
float remainderf(float, float);
float remquof(float, float, int *);
float rintf(float);
float scalblnf(float, long);
float scalbnf(float, int);
float truncf(float);

float fdimf(float, float);
float fmaf(float, float, float);
float fmaxf(float, float) __attribute__((__const__));
float fminf(float, float) __attribute__((__const__));






float dremf(float, float);
int finitef(float) __attribute__((__const__));
float gammaf(float);
float j0f(float);
float j1f(float);
float jnf(int, float);
float scalbf(float, float);
float y0f(float);
float y1f(float);
float ynf(int, float);






float gammaf_r(float, int *);
float lgammaf_r(float, int *);




float significandf(float);






long double acosl(long double);
long double asinl(long double);
long double atan2l(long double, long double);
long double atanl(long double);
long double cbrtl(long double);
long double ceill(long double);
long double copysignl(long double, long double) __attribute__((__const__));
long double cosl(long double);
long double exp2l(long double);
long double expl(long double);
long double fabsl(long double) __attribute__((__const__));
long double fdiml(long double, long double);
long double floorl(long double);
long double fmal(long double, long double, long double);
long double fmaxl(long double, long double) __attribute__((__const__));
long double fminl(long double, long double) __attribute__((__const__));
long double fmodl(long double, long double);
long double frexpl(long double value, int *);
long double hypotl(long double, long double);
int ilogbl(long double) __attribute__((__const__));
long double ldexpl(long double, int);
long long llrintl(long double);
long long llroundl(long double);
long double logbl(long double);
long lrintl(long double);
long lroundl(long double);
long double modfl(long double, long double *);
long double nanl(const char *) __attribute__((__const__));
long double nearbyintl(long double);
long double nextafterl(long double, long double);
double nexttoward(double, long double);
float nexttowardf(float, long double);
long double nexttowardl(long double, long double);
long double remainderl(long double, long double);
long double remquol(long double, long double, int *);
long double rintl(long double);
long double roundl(long double);
long double scalblnl(long double, long);
long double scalbnl(long double, int);
long double sinl(long double);
long double sqrtl(long double);
long double tanl(long double);
long double truncl(long double);



# 6 "fpar.h" 2
# 1 "udf.h" 1
# 1 "/usr/include/math.h" 1 3 4
# 2 "udf.h" 2



double udf1(double);
double udf2(double, double);
double udf3(double, double, double);

unsigned long udfi1(unsigned long);
unsigned long udfi2(unsigned long, unsigned long);
unsigned long udfi3(unsigned long, unsigned long, unsigned long);
unsigned long udfi6(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
# 7 "fpar.h" 2


double fpar_f(int, double, double**);
int fpar_function(int, int, int, char* tab);
void fpar_info();
void fpar_free(int);
void fpar_deinit();
int fpar_ok(int);
int fpar_init(int);
# 2 "fpar.c" 2



static unsigned long long gcnt;
char** buffer;
int* position;
char* ch;
int* maxpos;

double term();
double factor();
double expression();
double exponential();
double ***y;
double *t;
int* err;
int* nvar;
int* ranks;
int N;

int fpar_init(int ntab)
{
 int i;
 if (ntab < 0) return 0;
 buffer = (char**)malloc(ntab*sizeof(char*));
 if (!buffer) return 0;

 position = (int*)malloc(ntab*sizeof(int));
 ch = (char*)malloc(ntab*sizeof(char));
 maxpos = (int*)malloc(ntab*sizeof(int));
 y = (double***)malloc(ntab*sizeof(double**));
 err = (int*)malloc(ntab*sizeof(int));
 nvar = (int*)malloc(ntab*sizeof(int));
 ranks = (int*)malloc(ntab*sizeof(int));
 t = (double*)malloc(ntab*sizeof(double));
 N = ntab;
 for (i=0;i<N;i++)
   {
    buffer[i] = ((void *)0);
    position[i] = 0;
    err[i] = 0;
    nvar[i] = 1;
    ranks[i] = 1;
    y[i] = ((void *)0);
   }
 gcnt = 0;
 return 1;
}

void exception(char* fn, int ln, int num, char* reason)
{
 printf("Parser Exception: %s\n\n", reason);
 printf("Buffer[%d] = '%s'\n", num, buffer[num]);
 printf("Occured at: File: %s, Line: %d\n", fn, ln);
 err[num]=1;
}

void fpar_info()
{
 printf("supported functions: sin,cos,tan,tg,ctan,ctg,exp,ln,log,sqrt,abs\n"
        "supported functions: asin,acos,asinh,acosh,sinh,cosh,cbrt,ceil,sgn\n"
        "supported functions: tanh,atan,atanh,tgh,atg,atgh,neg\n"
        "supported bin-functions: max,min,mina,minb,maxa,maxb,and,or,xor,pow,mod\n"
        "supported tri-functions: if(cond,cond>0,cond<=0)\n"
        "supported rgb-functions: gr,gg,gb,sr,sg,sb,ggs,sgs\n");
 printf("supported operators: +,-,*,/,^, unary -\n"
 "supported user defined function: asmf[123](lx), udf1(x), udf2(x,y), udf3(x,y,z)\n"
 "and more...(see udf.c), special \"doing nothing\" none\n");
 printf("blank characters are skipped\n");
}

int fpar_ok(int num)
{
 double** tes;
 int i, j, nv, rn;
 nv = nvar[num];
 rn = ranks[num];
 tes = (double**)malloc(rn*sizeof(double*));
 for (i=0;i<rn;i++)
   {
    tes[i] = (double*)malloc(nv*sizeof(double));
    for (j=0;j<nv;j++) tes[i][j] = (double)(i * j);
   }
 fpar_f(num, 1., tes);




 free(tes);
 return !err[num];
}




int fpar_function(int num, int nvars, int nranks, char* tab)
{
 int i;
 if (!tab) return 0;
 if (buffer[num]) { free(buffer[num]); buffer[num] = 0; }
 buffer[num] = (char*)malloc(strlen(tab)+2);
 if (!buffer[num]) return 0;
 strcpy(buffer[num],tab);
 buffer[num][strlen(buffer[num])] = 10;
 buffer[num][strlen(buffer[num])+1] = 0;
 maxpos[num] = strlen(buffer[num]);
 nvar[num] = nvars;
 nranks ++;
 ranks[num] = nranks;
 y[num] = (double**)malloc(nranks*sizeof(double*));
 for (i=0;i<nranks;i++) y[num][i] = (double*)malloc(nvars*sizeof(double));
 return 1;
}


void fpar_free(int num)
{
 int i;
 if (buffer[num]) { free(buffer[num]); buffer[num] = 0; }
 for (i=0;i<ranks[num];i++)
   {
    if (y[num][i]) { free(y[num][i]); y[num][i] = 0; }
   }
 if (y[num]) { free(y[num]); y[num] = 0; }
}

void fpar_deinit()
{
 int i;
 for (i=0;i<N;i++) fpar_free(i);
 free(buffer); buffer = ((void *)0);
 free(position); position = ((void *)0);
 free(ch); ch = ((void *)0);
 free(maxpos); maxpos = ((void *)0);
 free(t); t = ((void *)0);
 free(y); y = ((void *)0);
 free(err); err = ((void *)0);
 free(nvar); nvar = ((void *)0);
 free(ranks); ranks = ((void *)0);
 printf("Internal parser calls: %lld\n", gcnt);
 gcnt = 0;
}


void read_next_char(int num)
{
 if (position[num] < maxpos[num] && ch[num] != 10)
     ch[num] = buffer[num][position[num]++];
}

void skipblanks(int num)
{
 if (ch[num] != 10)
  {
   while (__sbistype((ch[num]), 0x00004000L) && ch[num] != 10)
       read_next_char(num);
  }
}

void read_id(int num, char *ident)
{
 int cnt=0;
 skipblanks(num);
 if (__sbistype((ch[num]), 0x00000100L))
   {
    while (__sbistype((ch[num]), 0x00000100L) || __isctype((ch[num]), 0x00000400L) || ch[num] == '\'' || ch[num] == '_')
      {
       if (cnt < 32 -1) ident[cnt++] = ch[num];
       read_next_char(num);
      }
    ident[cnt] = 0;
   }
 else exception("fpar.c",174, num, "Expected: function name or variable.\n");
 gcnt ++;
}

double factor(int num)
{
 double f, minus, tmp, t1, t2, t3;
 int ynum, rnum;
 char ident[32],c,last;
 unsigned long utmp, utmp2, utmp3, utmp4;
 minus = 1.0;
 gcnt ++;
 read_next_char(num);
 skipblanks(num);
 while (ch[num]=='+' || ch[num]=='-')
   {
     if (ch[num] == '-') minus *= -1;
     read_next_char(num);
   }
 if (__isctype((ch[num]), 0x00000400L) || ch[num]=='.')
   {
    buffer[num][--position[num]] = ch[num];

    sscanf(buffer[num]+position[num], "%lf%c", &f, &ch[num]);
    c=ch[num];
    do read_next_char(num);
    while (ch[num] != c);
   }
 else if (ch[num] == '(' || ch[num] == ',')
   {
    f = expression(num);
    skipblanks(num);

    if (ch[num] == ')') read_next_char(num);
    else if (ch[num] == ',')
    {

 return minus*f;
    }
    else exception("fpar.c",213, num, "Expected: ')'.\n");
   }
 else
   {
    read_id(num, ident);
    last = ident[strlen(ident)-1];
    if (!strcmp(ident, "x") || !strcmp(ident, "t") || !strcmp(ident, "a")) f = t[num];
    else if (sscanf(ident, "y%d_%d", &ynum, &rnum) == 2 && last == '\'')
      {
       if (nvar[num] < ynum) exception("fpar.c",222, num, "found y in definition, but i > numer_of_args\n");
       else if (ynum <= 0) exception("fpar.c",223, num, "found y, but i <= 0\n");
       else if (ranks[num] <= rnum) exception("fpar.c",224, num, "found y in definition, but yranks > ranks of this\n");
       else if (rnum < 0) exception("fpar.c",225, num, "found y, but ranks < 0\n");
       else f = y[num][rnum][ynum-1];
      }
    else if (sscanf(ident, "y%d", &ynum) == 1 && last != '\'')
      {
       rnum = 0;
       if (nvar[num] < ynum) exception("fpar.c",231, num, "found y in definition, but i > numer_of_args\n");
       else if (ynum <= 0) exception("fpar.c",232, num, "found y, but i <= 0\n");
       else if (ranks[num] <= rnum) exception("fpar.c",233, num, "found y in definition, but yranks > ranks of this\n");
       else f = y[num][rnum][ynum-1];
      }
    else if (!strcmp(ident, "pi"))
      {
 skipblanks(num);
 f = 3.14159265358979323846;
      }
    else if (!strcmp(ident, "sin"))
      {
 skipblanks(num);
 if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = sin(exponential(num));
   }
 else exception("fpar.c",249, num, "Expected: '(' after sin.\n");
      }
     else if (!strcmp(ident, "cos"))
      {
       skipblanks(num);
       if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = cos(exponential(num));
   }
       else exception("fpar.c",259, num, "Expected: '(' after cos.\n");
      }
    else if (!strcmp(ident, "sinh"))
      {
 skipblanks(num);
 if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = sinh(exponential(num));
   }
 else exception("fpar.c",269, num, "Expected: '(' after sinh.\n");
      }
     else if (!strcmp(ident, "cosh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = cosh(exponential(num));
   }
       else exception("fpar.c",279, num, "Expected: '(' after cosh.\n");
      }
     else if (!strcmp(ident, "acos"))
      {
       skipblanks(num);
       if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = acos(exponential(num));
   }
       else exception("fpar.c",289, num, "Expected: '(' after acos.\n");
      }
     else if (!strcmp(ident, "asin"))
      {
       skipblanks(num);
       if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = asin(exponential(num));
   }
       else exception("fpar.c",299, num, "Expected: '(' after asin.\n");
      }
     else if (!strcmp(ident, "asinh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = asinh(exponential(num));
   }
       else exception("fpar.c",309, num, "Expected: '(' after asinh.\n");
      }
     else if (!strcmp(ident, "acosh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = acosh(exponential(num));
   }
       else exception("fpar.c",319, num, "Expected: '(' after acosh.\n");
      }
     else if (!strcmp(ident, "tg") || !strcmp(ident, "tan"))
      {
       skipblanks(num);
       if (ch[num]=='(')
  {
   buffer[num][--position[num]] = ch[num];
   f = tan(exponential(num));
  }
        else exception("fpar.c",329, num, "Expected: '(' after tg or tan.\n");
       }
     else if (!strcmp(ident, "atg") || !strcmp(ident, "atan"))
      {
       skipblanks(num);
       if (ch[num]=='(')
  {
   buffer[num][--position[num]] = ch[num];
   f = atan(exponential(num));
  }
        else exception("fpar.c",339, num, "Expected: '(' after atg or atan.\n");
       }
     else if (!strcmp(ident, "tgh") || !strcmp(ident, "tanh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
  {
   buffer[num][--position[num]] = ch[num];
   f = tanh(exponential(num));
  }
        else exception("fpar.c",349, num, "Expected: '(' after tgh or tanh.\n");
       }
     else if (!strcmp(ident, "atgh") || !strcmp(ident, "atanh"))
      {
       skipblanks(num);
       if (ch[num]=='(')
  {
   buffer[num][--position[num]] = ch[num];
   f = atanh(exponential(num));
  }
        else exception("fpar.c",359, num, "Expected: '(' after atgh or atanh.\n");
       }
     else if (!strcmp(ident, "ctg") || !strcmp(ident,"ctan"))
       {
 skipblanks(num);
 if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = 1.0/tan(exponential(num));
   }
 else exception("fpar.c",369, num, "Expected: '(' after ctg or ctan.\n");
       }
     else if (!strcmp(ident, "exp") || !strcmp(ident,"e"))
       {
 skipblanks(num);
 if (ch[num]=='(')
   {
    buffer[num][--position[num]] = ch[num];
    f = exp(exponential(num));
   }
 else exception("fpar.c",379, num, "Expected: '(' after exp or e.\n");
 }
 else if (!strcmp(ident, "ln"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       f = log(exponential(num));
      }
    else exception("fpar.c",389, num, "Expected: '(' after ln.\n");
   }
 else if (!strcmp(ident, "log"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       f = log10(exponential(num));
      }
    else exception("fpar.c",399, num, "Expected: '(' after log.\n");
   }
 else if (!strcmp(ident, "sqrt"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       f = sqrt(exponential(num));
      }
    else exception("fpar.c",409, num, "Expected: '(' after sqrt.\n");
   }
 else if (!strcmp(ident, "cbrt"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       f = cbrt(exponential(num));
      }
    else exception("fpar.c",419, num, "Expected: '(' after cbrt.\n");
   }
 else if (!strcmp(ident, "ceil"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       f = ceil(exponential(num));
      }
    else exception("fpar.c",429, num, "Expected: '(' after ceil.\n");
   }
 else if (!strcmp(ident, "sgn"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       tmp = exponential(num);
       if (tmp > 0.0) f = 1.;
       else if (tmp < 0.0) f = -1.;
       else f = 0.;
      }
    else exception("fpar.c",442, num, "Expected: '(' after sgn.\n");
   }
 else if (!strcmp(ident, "abs"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       f = fabs(exponential(num));
      }
    else exception("fpar.c",452, num, "Expected: '(' after abs.\n");
   }
 else if (!strcmp(ident, "udf1"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       f = udf1(exponential(num));
      }
    else exception("fpar.c",462, num, "Expected: '(' after udf1.\n");
   }
 else if (!strcmp(ident, "gb"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp = (unsigned long)exponential(num);
       utmp &= 0xFF;
       f = (double)utmp;
      }
    else exception("fpar.c",474, num, "Expected: '(' after gb.\n");
   }
 else if (!strcmp(ident, "gg"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp = (unsigned long)exponential(num);
       utmp = (utmp & 0xFF00) >> 0x8;
       f = (double)utmp;
      }
    else exception("fpar.c",486, num, "Expected: '(' after gg.\n");
   }
 else if (!strcmp(ident, "gr"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp = (unsigned long)exponential(num);
       utmp = (utmp & 0xFF0000) >> 0x10;
       f = (double)utmp;
      }
    else exception("fpar.c",498, num, "Expected: '(' after gr.\n");
   }
 else if (!strcmp(ident, "sb"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp = (unsigned long)exponential(num);
       utmp &= 0xFF;
       f = (double)utmp;
      }
    else exception("fpar.c",510, num, "Expected: '(' after sb.\n");
   }
 else if (!strcmp(ident, "sg"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp = (unsigned long)exponential(num);
       utmp = (utmp & 0xFF) << 0x8;
       f = (double)utmp;
      }
    else exception("fpar.c",522, num, "Expected: '(' after sg.\n");
   }
 else if (!strcmp(ident, "sr"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp = (unsigned long)exponential(num);
       utmp = (utmp & 0xFF) << 0x10;
       f = (double)utmp;
      }
    else exception("fpar.c",534, num, "Expected: '(' after sr.\n");
   }
 else if (!strcmp(ident, "sgs"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp2 = (unsigned long)exponential(num);
       utmp2 = (utmp2 & 0xFF);
       utmp = utmp2 + (utmp2 << 8) + (utmp2 << 0x10);
       f = (double)utmp;
      }
    else exception("fpar.c",547, num, "Expected: '(' after sgs.\n");
   }
 else if (!strcmp(ident, "ggs"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp = (unsigned long)exponential(num);
       utmp2 = utmp & 0xFF;
       utmp3 = (utmp & 0xFF00) >> 0x8;
       utmp4 = (utmp & 0xFF0000) >> 0x10;
       utmp = (utmp2 + utmp3 + utmp4) / 3;
       f = (double)utmp;
      }
    else exception("fpar.c",562, num, "Expected: '(' after ggs.\n");
   }
 else if (!strcmp(ident, "neg"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       utmp = (unsigned long)exponential(num);
       utmp = ~utmp;
       f = (double)utmp;
      }
    else exception("fpar.c",574, num, "Expected: '(' after neg.\n");
   }
 else if (!strcmp(ident, "pow"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",592, num, "Expected: ',' in pow(..., ...).\n");

       f = pow(t1, t2);
      }
    else exception("fpar.c",596, num, "Expected: '(' after pow.\n");
   }
 else if (!strcmp(ident, "udf2"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",614, num, "Expected: ',' in udf2(..., ...).\n");

       f = udf2(t1, t2);
      }
    else exception("fpar.c",618, num, "Expected: '(' after max.\n");
   }
 else if (!strcmp(ident, "max"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",636, num, "Expected: ',' in max(..., ...).\n");

       f = (t1 > t2) ? t1 : t2;
      }
    else exception("fpar.c",640, num, "Expected: '(' after max.\n");
   }
 else if (!strcmp(ident, "min"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",658, num, "Expected: ',' in min(..., ...).\n");

       f = (t1 < t2) ? t1 : t2;
      }
    else exception("fpar.c",662, num, "Expected: '(' after min.\n");
   }
 else if (!strcmp(ident, "mina"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",680, num, "Expected: ',' in mina(..., ...).\n");

       f = t2;
       if (f < t1) f = t1;
      }
    else exception("fpar.c",685, num, "Expected: '(' after mina.\n");
   }
 else if (!strcmp(ident, "minb"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",703, num, "Expected: ',' in minb(..., ...).\n");

       f = t1;
       if (f < t2) f = t2;
      }
    else exception("fpar.c",708, num, "Expected: '(' after minb.\n");
   }
 else if (!strcmp(ident, "maxa"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",726, num, "Expected: ',' in maxa(..., ...).\n");

       f = t2;
       if (f > t1) f = t1;

      }
    else exception("fpar.c",732, num, "Expected: '(' after maxa.\n");
   }
 else if (!strcmp(ident, "maxb"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",750, num, "Expected: ',' in maxb(..., ...).\n");

       f = t1;
       if (f > t2) f = t2;
      }
    else exception("fpar.c",755, num, "Expected: '(' after maxb.\n");
   }
 else if (!strcmp(ident, "mod"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",773, num, "Expected: ',' in mod(..., ...).\n");

       utmp = (unsigned long)t1;
       utmp2 = (unsigned long)t2;

       utmp = utmp % utmp2;
       f = (double)utmp;
      }
    else exception("fpar.c",781, num, "Expected: '(' after and.\n");
   }
 else if (!strcmp(ident, "and"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",799, num, "Expected: ',' in and(..., ...).\n");

       utmp = (unsigned long)t1;
       utmp2 = (unsigned long)t2;

       utmp = utmp & utmp2;
       f = (double)utmp;
      }
    else exception("fpar.c",807, num, "Expected: '(' after and.\n");
   }
 else if (!strcmp(ident, "or"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",825, num, "Expected: ',' in or(..., ...).\n");

       utmp = (unsigned long)t1;
       utmp2 = (unsigned long)t2;

       utmp = utmp | utmp2;
       f = (double)utmp;
      }
    else exception("fpar.c",833, num, "Expected: '(' after or.\n");
   }
 else if (!strcmp(ident, "xor"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       skipblanks(num);

       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",851, num, "Expected: ',' in xor(..., ...).\n");

       utmp = (unsigned long)t1;
       utmp2 = (unsigned long)t2;

       utmp = utmp ^ utmp2;
       f = (double)utmp;
      }
    else exception("fpar.c",859, num, "Expected: '(' after xor.\n");
   }
 else if (!strcmp(ident, "if"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       t3 = t1;

       skipblanks(num);
       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",878, num, "Expected: 1st ',' in if(..., ..., ...).\n");

       skipblanks(num);
       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t3 = expression(num);
       }
       else exception("fpar.c",887, num, "Expected: 2nd ',' in if(..., ..., ...).\n");

       if (t1 > 0.) f = t2;
       else f = t3;

      }
    else exception("fpar.c",893, num, "Expected: '(' after max.\n");
   }
 else if (!strcmp(ident, "udf3"))
   {
    skipblanks(num);
    if (ch[num]=='(')
      {
       buffer[num][--position[num]] = ch[num];
       t1 = expression(num);
       t2 = t1;
       t3 = t1;

       skipblanks(num);
       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t2 = expression(num);
       }
       else exception("fpar.c",912, num, "Expected: 1st ',' in udf3(..., ..., ...).\n");

       skipblanks(num);
       if (ch[num] == ',')
       {
           skipblanks(num);
           buffer[num][--position[num]] = ch[num];
    t3 = expression(num);
       }
       else exception("fpar.c",921, num, "Expected: 2nd ',' in udf3(..., ..., ...).\n");

       f = udf3(t1, t2, t3);
      }
    else exception("fpar.c",925, num, "Expected: '(' after max.\n");
   }
 else exception("fpar.c",927, num, "Unknown identifier.\n");
 }
 skipblanks(num);
 return minus*f;
}

double term(int num)
{
 double f1;
 gcnt ++;
 f1 = exponential(num);
 while(1)
   {
    switch (ch[num])
      {
       case '*': f1 *= exponential(num); break;
       case '/': f1 /= exponential(num); break;
       default: return f1;
      }
   }
}

double expression(int num)
{
 double t1;
 gcnt ++;
 t1 = term(num);
 while(1)
   {
    switch (ch[num])
      {
       case '+': t1 += term(num); break;
       case '-': t1 -= term(num); break;
       default: return t1;
      }
   }
}

double exponential(int num)
{
 double f;
 gcnt ++;
 f = factor(num);
 while (ch[num] == '^') f = pow(f, exponential(num));
 return f;
}


double fpar_f(int num, double targ, double** yargs)
{
 double e;
 int i,j,nv,rn;
 gcnt ++;
 t[num] = targ;
 nv = 1;
 if (yargs)
   {
    nv = nvar[num];
    rn = ranks[num];
    for (i=0;i<rn;i++) for (j=0;j<nv;j++) y[num][i][j] = yargs[i][j];
   }
 else for (i=0;i<nv;i++) for (j=0;j<nv;j++) y[num][i][j] = 0.;

 position[num]=0;
 ch[num]=0;
 e=expression(num);
 if (ch[num] != 10 && ch[num] != ';') exception("fpar.c",993, num, "Garbage in function expression.\n");
 if (err[num])
   {
    exception("fpar.c",996, num, "Value returned MAY BE invalid, exceptions occured.\n");
    return e;
   }
 return e;
}
