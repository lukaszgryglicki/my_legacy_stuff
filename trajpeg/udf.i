# 1 "udf.c"
# 1 "<command-line>"
# 1 "udf.c"
# 1 "udf.h" 1
# 1 "/usr/include/math.h" 1 3 4
# 20 "/usr/include/math.h" 3 4
# 1 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/sys/cdefs.h" 1 3 4
# 21 "/usr/include/math.h" 2 3 4
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
# 22 "/usr/include/math.h" 2 3 4
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



# 2 "udf.h" 2
# 1 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 1 3 4
# 46 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 3 4
# 1 "/usr/include/sys/_null.h" 1 3 4
# 47 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdlib.h" 2 3 4




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


# 3 "udf.h" 2
# 1 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 1 3 4
# 14 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
# 1 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include/stdarg.h" 1 3 4
# 15 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 2 3 4
# 57 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
typedef __off_t fpos_t;
# 67 "/usr/local/lib/gcc48/gcc/x86_64-portbld-freebsd10.0/4.8.0/include-fixed/stdio.h" 3 4
typedef __off_t off_t;



typedef __ssize_t ssize_t;





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

# 4 "udf.h" 2

double udf1(double);
double udf2(double, double);
double udf3(double, double, double);

unsigned long udfi1(unsigned long);
unsigned long udfi2(unsigned long, unsigned long);
unsigned long udfi3(unsigned long, unsigned long, unsigned long);
unsigned long udfi6(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
# 2 "udf.c" 2


unsigned long color_comix(unsigned long x)
{
    register volatile unsigned long r, g, b;
    const unsigned long shl = 6;

    r = ((x & 0xff0000) >> (0x10 + shl)) << shl;
    g = ((x & 0xff00) >> (0x8 + shl)) << shl;
    b = ((x & 0xff) >> shl) << shl;

    return (r << 0x10) + (g << 0x8) + b;
}

unsigned long edge_detect_comix(unsigned long x1, unsigned long x2)
{
    unsigned long f, r1, g1, b1, r2, g2, b2;
    const unsigned long shl = 6;

    r1 = (x1 & 0xff0000) >> 0x10;
    g1 = (x1 & 0xff00) >> 0x8;
    b1 = x1 & 0xff;

    r2 = (x2 & 0xff0000) >> 0x10;
    g2 = (x2 & 0xff00) >> 0x8;
    b2 = x2 & 0xff;

    f = abs((r1 + g1 + b1) - (r2 + g2 + b2));

    if (f < 16)
    {
 r1 = (r1 >> shl) << shl;
 g1 = (g1 >> shl) << shl;
 b1 = (b1 >> shl) << shl;
    }
    else
    {
 if (r1 > g1)
 {
     if (r1 > b1) { r1 = 0x0; g1 = b1 = 0xff; }
     else { b1= 0x0; r1 = g1 = 0xff; }
 }
 else
 {
     if (g1 > b1) { g1 = 0x0; r1 = b1 = 0xff; }
     else { b1 = 0x0; r1 = b1 = 0xff; }
 }
    }

    return (r1 << 0x10) + (g1 << 0x8) + b1;
}

unsigned long smooth_gs6(unsigned long* x)
{
    unsigned long i, r, g, b, gs[6], f;

    f = 0;
    for (i=0;i<6;i++)
    {
        r = (x[i] & 0xff0000) >> 0x10;
        g = (x[i] & 0xff00) >> 0x8;
        b = x[i] & 0xff;

        gs[i] = (r + g + b) / 3;

 f += gs[i];
    }

    f /= 6;

    return (f << 0x10) + (f << 0x8) + f;
}

unsigned long color_offset6(unsigned long* x)
{
    register volatile unsigned long r, g, b;

    r = (((x[0] & 0xff0000) >> 0x10) + ((x[1] & 0xff0000) >> 0x10)) >> 1;
    g = (((x[2] & 0xff00) >> 0x8) + ((x[3] & 0xff00) >> 0x8)) >> 1;
    b = ((x[4] & 0xff) + (x[5] & 0xff)) >> 1;

    return (r << 0x10) + (g << 0x8) + b;
}

unsigned long color_offset3(unsigned long x1, unsigned long x2, unsigned long x3)
{
# 98 "udf.c"
    return (x1 & 0xff0000) + (x2 & 0xff00) + (x3 & 0xff);
}


unsigned long smooth_gs(unsigned long x1, unsigned long x2, unsigned long x3)
{
    unsigned long r, g, b, gs1, gs2, gs3, gs;

    r = (x1 & 0xff0000) >> 0x10;
    g = (x1 & 0xff00) >> 0x8;
    b = x1 & 0xff;

    gs1 = (r + g + b) / 3;

    r = (x2 & 0xff0000) >> 0x10;
    g = (x2 & 0xff00) >> 0x8;
    b = x2 & 0xff;

    gs2 = (r + g + b) / 3;

    r = (x3 & 0xff0000) >> 0x10;
    g = (x3 & 0xff00) >> 0x8;
    b = x3 & 0xff;

    gs3 = (r + g + b) / 3;

    gs = (gs1 + gs2 + gs3) / 3;

    return (gs << 0x10) + (gs << 0x8) + gs;
}

unsigned long edge_detect(unsigned long x1, unsigned long x2, unsigned long x3)
{
    unsigned long r1, g1, b1, r2, g2, b2;
    x3 = 25000 / x3;



    r1 = (x1 & 0xff0000) >> 0x10;
    g1 = (x1 & 0xff00) >> 0x8;
    b1 = x1 & 0xff;

    r2 = (x2 & 0xff0000) >> 0x10;
    g2 = (x2 & 0xff00) >> 0x8;
    b2 = x2 & 0xff;

    r1 = abs(r1 - r2);
    g1 = abs(g1 - g2);
    b1 = abs(b1 - b2);

    if (r1 > x3) r1 = r2;
    else r1 = 0;
    if (g1 > x3) g1 = g2;
    else g1 = 0;
    if (b1 > x3) b1 = b2;
    else b1 = 0;

    return (r1 << 0x10) + (g1 << 0x8) + b1;
}

double alien_gs3(double x1, double x2, double x3)
{
    unsigned long lx, lr, lg, lb, l1, l2, l3;
    double r,g,b,gs;

    lx = (unsigned long)x1;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;
    gs = .5 * (-1. * cos(gs * 3. * 3.14159265358979323846) + 1.);
    l1 = (unsigned long)(gs * 255.);

    lx = (unsigned long)x2;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;
    gs = .5 * (-1. * cos(gs * 3. * 3.14159265358979323846) + 1.);
    l2 = (unsigned long)(gs * 255.);

    lx = (unsigned long)x3;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;
    gs = .5 * (-1. * cos(gs * 3. * 3.14159265358979323846) + 1.);
    l3 = (unsigned long)(gs * 255.);

    lx = (l1 << 0x10) + (l2 << 0x8) + l3;

    x1 = (double)lx;

    return x1;
}

double alien_gs(double x)
{
    unsigned long lx, lr, lg, lb;
    double r,g,b,gs;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;

    gs = .5 * (-1. * cos(gs * 3. * 3.14159265358979323846) + 1.);

    lx = (unsigned long)(gs * 255.);

    lx = (lx << 0x10) + (lx << 0x8) + lx;

    x = (double)lx;

    return x;
}

unsigned long alien_toon3(unsigned long x1, unsigned long x2, unsigned long x3, int na, int nb, int nc)
{
    unsigned long r, g, b, gs, i;

    r = (x1 & 0xFF0000) >> 0x10;
    g = (x1 & 0xFF00) >> 0x8;
    b = x1 & 0xFF;

    gs = ((r << 2) + g * 5 + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }

    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    x1 = (gs >> nb) << nb;

    r = (x2 & 0xFF0000) >> 0x10;
    g = (x2 & 0xFF00) >> 0x8;
    b = x2 & 0xFF;

    gs = ((r << 2) + g * 5 + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }

    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    x2 = (gs >> nb) << nb;

    r = (x3 & 0xFF0000) >> 0x10;
    g = (x3 & 0xFF00) >> 0x8;
    b = x3 & 0xFF;

    gs = ((r << 2) + g * 5 + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }

    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    x3 = (gs >> nb) << nb;

    x3 <<= 1;
    if (x3 > 0xff) x3 = 0xff;

    x1 += 1 << nb;
    if (x1 > 0xff) x1 = 0xff;

    x1 = (x1 << 0x10) + (x2 << 0x8) + x3;

    return x1;
}

unsigned long alien_toongs2(unsigned long x, int na, int nb, int nc, int vm)
{
    unsigned long r, g, b, gs, i;

    r = (x & 0xFF0000) >> 0x10;
    g = (x & 0xFF00) >> 0x8;
    b = x & 0xFF;

    gs = ((r << 2) + 5 * g + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }

    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    gs = (gs >> nb) << nb;

    r = g = b = gs;

    if (vm & 1)
    {
        r <<= 1;
        if (r > 0xff) r = 0xff;
    }

    if (vm & 2)
    {
        g <<= 1;
        if (g > 0xff) g = 0xff;
    }

    if (vm & 4)
    {
        b <<= 1;
        if (b > 0xff) b = 0xff;
    }

    x = (r << 0x10) + (g << 0x8) + b;

    return x;
}

unsigned long alien_toongs(unsigned long x, int na, int nb, int nc)
{
    unsigned long r, g, b, gs, i;

    r = (x & 0xFF0000) >> 0x10;
    g = (x & 0xFF00) >> 0x8;
    b = x & 0xFF;

    gs = ((r << 2) + 5 * g + b) / 10;

    for (i=0;i<na;i++)
    {
        if (gs < 0x80) gs = (0x7F - gs) << 1;
        else gs = (gs - 0x80) << 1;
    }

    for (i=0;i<nc;i++)
    {
        gs = (gs * gs) >> 0x8;
    }

    gs = (gs >> nb) << nb;

    r = g = b = gs;

    b <<= 1;
    b += 0x10;
    if (b > 0xff) b = 0xff;
# 397 "udf.c"
    if (r < 0x28) r = 0x28;

    x = (r << 0x10) + (g << 0x8) + b;

    return x;
}

unsigned long alien_toon(unsigned long x, int na, int nb, int nc)
{
    unsigned long r, g, b, r2, g2, b2, i;

    r = (x & 0xFF0000) >> 0x10;
    g = (x & 0xFF00) >> 0x8;
    b = x & 0xFF;

    for (i=0;i<na;i++)
    {
        if (r < 0x80) r = (0x7F - r) << 1;
        else r = (r - 0x80) << 1;

        if (g < 0x80) g = (0x7F - g) << 1;
        else g = (g - 0x80) << 1;

        if (b < 0x80) b = (0x7F - b) << 1;
        else b = (b - 0x80) << 1;
    }

    r2 = (5*r + 2*g + b) >> 0x3;
    g2 = (5*g + 2*b + r) >> 0x3;
    b2 = (5*b + 2*r + g) >> 0x3;

    for (i=0;i<nc;i++)
    {
        r2 = (r2 * r2) >> 0x8;
        g2 = (g2 * g2) >> 0x8;
        b2 = (b2 * b2) >> 0x8;
    }

    r2 = (r2 >> nb) << nb;
    g2 = (g2 >> nb) << nb;
    b2 = (b2 >> nb) << nb;

    b2 <<= 1;
    if (b2 > 0xff) b2 = 0xff;

    if (nb > 1)
    {
        r2 += 1 << (nb - 1);
        if (r2 > 0xff) r2 = 0xff;
    }

    x = (r2 << 0x10) + (g2 << 0x8) + b2;

    return x;
}

unsigned long alien_gsi(unsigned long x)
{
    register volatile unsigned long r, g, b, gs;

    r = (x & 0xFF0000) >> 0x10;
    g = (x & 0xFF00) >> 0x8;
    b = x & 0xFF;

    gs = r + g + b;

    if (gs < 0x100)
    {
 x = gs;
    }
    else if (gs >= 0x100 && gs < 0x200)
    {
 x = 0x1ff - gs;
    }
    else
    {
 x = gs - 0x200;
    }

    x = (x << 0x10) + (x << 0x8) + x;

    return x;
}

unsigned long alien_gsi6(unsigned long* x)
{
    register volatile unsigned long i, r, g, b, gs;

    for (i=0;i<6;i++)
    {
        r = (x[i] & 0xFF0000) >> 0x10;
        g = (x[i] & 0xFF00) >> 0x8;
        b = x[i] & 0xFF;

        gs = r + g + b;

        if (gs < 0x100)
        {
     x[i] = gs;
        }
        else if (gs >= 0x100 && gs < 0x200)
        {
     x[i] = 0x1ff - gs;
        }
        else
        {
     x[i] = gs - 0x200;
        }
    }

    r = (x[0] + x[1]) >> 1;
    g = (x[2] + x[3]) >> 1;
    b = (x[4] + x[5]) >> 1;

    gs = (r << 0x10) + (g << 0x8) + b;

    return gs;
}

__inline unsigned long color_gsi3(unsigned long x1, unsigned long x2, unsigned long x3)
{
    return (x1 & 0xff0000) + (x2 & 0xff00) + (x3 & 0xff);
}

unsigned long alien_gsi3(unsigned long x1, unsigned long x2, unsigned long x3)
{
    register volatile unsigned long r, g, b, gs;

    r = (x1 & 0xFF0000) >> 0x10;
    g = (x1 & 0xFF00) >> 0x8;
    b = x1 & 0xFF;

    gs = r + g + b;

    if (gs < 0x100)
    {
 x1 = gs;
    }
    else if (gs >= 0x100 && gs < 0x200)
    {
 x1 = 0x1ff - gs;
    }
    else
    {
 x1 = gs - 0x200;
    }

    r = (x2 & 0xFF0000) >> 0x10;
    g = (x2 & 0xFF00) >> 0x8;
    b = x2 & 0xFF;

    gs = r + g + b;

    if (gs < 0x100)
    {
 x2 = gs;
    }
    else if (gs >= 0x100 && gs < 0x200)
    {
 x2 = 0x1ff - gs;
    }
    else
    {
 x2 = gs - 0x200;
    }

    r = (x3 & 0xFF0000) >> 0x10;
    g = (x3 & 0xFF00) >> 0x8;
    b = x3 & 0xFF;

    gs = r + g + b;

    if (gs < 0x100)
    {
 x3 = gs;
    }
    else if (gs >= 0x100 && gs < 0x200)
    {
 x3 = 0x1ff - gs;
    }
    else
    {
 x3 = gs - 0x200;
    }

    x1 = (x1 << 0x10) + (x2 << 0x8) + x3;

    return x1;
}

double alien_sine(double x)
{
    unsigned long lx, lr, lg, lb;
    double r,g,b;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    r = .5* (sin(r * 2. * 3.14159265358979323846) + 1.);
    g = .5* (sin(g * 2. * 3.14159265358979323846) + 1.);
    b = .5* (sin(b * 2. * 3.14159265358979323846) + 1.);

    r = pow(r, r);
    g = pow(g, g);
    b = pow(b, b);

    r = 3.125* (r - 0.68);
    g = 3.125* (g - 0.68);
    b = 3.125* (b - 0.68);

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    if (lr > 255) lr = 255;
    if (lg > 255) lg = 255;
    if (lb > 255) lb = 255;

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double alien_sine_ex(double x, double rmul, double rfi, double gmul, double gfi, double bmul, double bfi)
{
    unsigned long lx, lr, lg, lb;
    double r,g,b;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    r = .5* (sin((r * r * 2. * 3.14159265358979323846 * rmul) + rfi) + 1.);
    g = .5* (sin((g * g * 2. * 3.14159265358979323846 * gmul) + gfi) + 1.);
    b = .5* (sin((b * b * 2. * 3.14159265358979323846 * bmul) + bfi) + 1.);

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    if (lr > 255) lr = 255;
    else if (lr < 0) lr = 0;
    if (lg > 255) lg = 255;
    else if (lg < 0) lg = 0;
    if (lb > 255) lb = 255;
    else if (lb < 0) lb = 0;

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double alien_col_mul(double x, double rmul, double rfi, double gmul, double gfi, double bmul, double bfi)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    r2 = .5* (sin((sqrt(b) * r * 2. * 3.14159265358979323846 * rmul) + rfi) + 1.);
    g2 = .5* (sin((sqrt(r) * g * 2. * 3.14159265358979323846 * gmul) + gfi) + 1.);
    b2 = .5* (sin((sqrt(g) * b * 2. * 3.14159265358979323846 * bmul) + bfi) + 1.);

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    if (lr > 255) lr = 255;
    else if (lr < 0) lr = 0;
    if (lg > 255) lg = 255;
    else if (lg < 0) lg = 0;
    if (lb > 255) lb = 255;
    else if (lb < 0) lb = 0;

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_mul(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    r2 = r * g;
    g2 = g * b;
    b2 = b * r;

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    if (lr > 255) lr = 255;
    else if (lr < 0) lr = 0;
    if (lg > 255) lg = 255;
    else if (lg < 0) lg = 0;
    if (lb > 255) lb = 255;
    else if (lb < 0) lb = 0;

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

unsigned long color_invi3(unsigned long x1, unsigned long x2, unsigned long x3)
{
    unsigned long r, g, b;

    g = 0xFF - ((x1 & 0xFF00) >> 0x8);
    b = 0xFF - (x1 & 0xFF);

    x1 = (g + b) >> 0x1;

    if (x1 < 0x80) x1 = (0x7F - x1) << 1;
    else x1 = (x1 - 0x80) << 1;

    r = 0xFF - ((x2 & 0xFF0000) >> 0x10);
    b = 0xFF - (x2 & 0xFF);

    x2 = (b + r) >> 0x1;

    if (x2 < 0x80) x2 = (0x7F - x2) << 1;
    else x2 = (x2 - 0x80) << 1;

    r = 0xFF - ((x3 & 0xFF0000) >> 0x10);
    g = 0xFF - ((x3 & 0xFF00) >> 0x8);

    x3 = (r + g) >> 0x1;

    if (x3 < 0x80) x3 = (0x7F - x3) << 1;
    else x3 = (x3 - 0x80) << 1;

    return (x1 << 0x10) + (x2 << 0x8) + x3;
}

unsigned long color_invi(unsigned long x)
{
    unsigned long r, g, b, r2, g2, b2;

    r = 0xFF - ((x & 0xFF0000) >> 0x10);
    g = 0xFF - ((x & 0xFF00) >> 0x8);
    b = 0xFF - (x & 0xFF);

    r2 = (g + b) >> 0x1;
    g2 = (b + r) >> 0x1;
    b2 = (r + g) >> 0x1;

    if (r2 < 0x80) r2 = (0x7F - r2) << 1;
    else r2 = (r2 - 0x80) << 1;

    if (g2 < 0x80) g2 = (0x7F - g2) << 1;
    else g2 = (g2 - 0x80) << 1;

    if (b2 < 0x80) b2 = (0x7F - b2) << 1;
    else b2 = (b2 - 0x80) << 1;

    return (r2 << 0x10) + (g2 << 0x8) + b2;
}

double color_inv2(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = 1. - (double)lr / 255.;
    g = 1. - (double)lg / 255.;
    b = 1. - (double)lb / 255.;

    r2 = b * g;
    g2 = r * b;
    b2 = g * r;

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double alien_inv(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = 1. - (double)lr / 255.;
    g = 1. - (double)lg / 255.;
    b = 1. - (double)lb / 255.;







    r2 = b * g;
    g2 = r * b;
    b2 = g * r;

    r = .5 * (-cos( r2 * 2. * 3.14159265358979323846 ) + 1.);
    g = .5 * (-cos( g2 * 2. * 3.14159265358979323846 ) + 1.);
    b = .5 * (-cos( b2 * 2. * 3.14159265358979323846 ) + 1.);

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

void get_heat(double x, double l, double* or, double* og, double* ob)
{
    double f, r, g, b;
# 877 "udf.c"
    if (x < 0.0 || x > 1.0 || l < 0.0 || l > 1.0)
    {
 printf("x=%f, l=%f --> error\n", x, l);
 *or = *og = *ob = 1.;
 return;
    }

    if (x <= 1./6.)
    {
 f = 6. * x;

 r = l;
 g = f * l;
 b = 0.;
    }
    else if (x <= 2./6.)
    {
 f = 6. * x - 1.;

 r = (1. - f) * l;
 g = l;
 b = 0.;
    }
    else if (x <= 3./6.)
    {
 f = 6. * x - 2.;

 r = 0.;
 g = l;
 b = f * l;
    }
    else if (x <= 4./6.)
    {
 f = 6. * x - 3.;

 r = 0.;
 g = (1. - f) * l;
 b = l;
    }
    else if (x <= 5./6.)
    {
 f = 6. * x - 4.;

 r = f * l;
 g = 0.;
 b = l;
    }
    else if (x <= 6./6.)
    {
 f = 6. * x - 5.;

 r = l;
 g = 0.;
 b = (1. - f) * l;
    }
    else r = g = b = 1.;



    *or = r;
    *og = g;
    *ob = b;
}

double color_hue2(double x)
{
    unsigned long lx, lr, lg, lb;
    double g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;


    g = (double)lg / 255.;
    b = (double)lb / 255.;

    get_heat(b, g, &r2, &g2, &b2);

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;

}

double color_hue(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, gs, rr, gr, br, rg, gg, bg, rb, gb, bb;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    gs = (r + g + b) / 3.;

    get_heat(r, gs, &rr, &rg, &rb);
    get_heat(g, gs, &gr, &gg, &gb);
    get_heat(b, gs, &br, &bg, &bb);

    r = (rr < rg) ? ((rr > rb) ? rr : rb) : ((rg > rb) ? rg : rb);
    g = (gr < gg) ? ((gr > gb) ? gr : gb) : ((gg > gb) ? gg : gb);
    b = (br < bg) ? ((br > bb) ? br : bb) : ((bg > bb) ? bg : bb);

    r = 1. - cos(r * 3.14159265358979323846) * cos(r * 3.14159265358979323846);
    g = 1. - cos(g * 3.14159265358979323846) * cos(g * 3.14159265358979323846);
    b = 1. - cos(b * 3.14159265358979323846) * cos(b * 3.14159265358979323846);

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;

}

double color_complex(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;
    double re1, im1, l1, fi1;
    double re2, im2, l2, fi2;
    double re3, im3, l3, fi3;
    double fi;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    re1 = g/sqrt(2.);
    im1 = b/sqrt(2.);
    l1 = sqrt(re1 * re1 + im1 * im1);
    if (l1 > 0.) fi1 = acos(re1 / l1);
    else fi1 = 0.;

    re2 = r/sqrt(2.);
    im2 = g/sqrt(2.);
    l2 = sqrt(re2 * re2 + im2 * im2);
    if (l2 > 0.) fi2 = acos(re2 / l2);
    else fi2 = 0.;

    re3 = b/sqrt(2.);
    im3 = r/sqrt(2.);
    l3 = sqrt(re3 * re3 + im3 * im3);
    if (l3 > 0.) fi3 = acos(re3 / l3);
    else fi3 = 0.;


    fi = fi1 + fi2 + fi3;


    fi /= 1.5 * 3.14159265358979323846;

    get_heat( fi2 / 3.14159265358979323846, l2, &r2, &g2, &b2);

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}


double color_angle(double x)
{
    unsigned long lx, lr, lg, lb;
    double f, r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = (double)lr / 255.;
    g = (double)lg / 255.;
    b = (double)lb / 255.;

    f = sqrt(g*g + b*b);
    if (f > 0.) r2 = g / f;
    else r2 = 0.;

    f = sqrt(b*b + r*r);
    if (f > 0.) g2 = b / f;
    else g2 = 0.;

    f = sqrt(r*r + g*g);
    if (f > 0.) b2 = r / f;
    else b2 = 0.;

    r2 = asin(r2) / 3.14159265358979323846;
    g2 = asin(g2) / 3.14159265358979323846;
    b2 = asin(b2) / 3.14159265358979323846;

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

unsigned long color_heati2(unsigned long x)
{
    volatile register unsigned long r, g, b, gs, f;

    r = (x & 0xff0000) >> 0x10;
    g = (x & 0xff00) >> 0x8;
    b = x & 0xff;

    gs = r + g + b;
    f = gs;

    if (gs <= 110)
    {
 f = gs << 1;

 r = 220 / 3;
 g = 220 / 3 - f / 3;
 b = 220 / 3 + f / 3;
    }
    else if (gs <= 220)
    {
 f = (gs - 110) << 1;

 r = 220 / 3 - f / 3;
 g = 0;
 b = 440 / 3 + f / 3;
    }
    else if (gs <= 330)
    {
 f = gs - 220;

 r = 0;
 g = f;
 b = 220 - f;
    }
    else if (gs <= 440)
    {
 f = gs - 330;

 r = 0;
 g = 110 + f;
 b = 110 - f;
    }
    else if (gs <= 550)
    {
 f = gs - 440;

 r = f;
 g = 220 - f;
 b = 0;
    }
    else if (gs <= 660)
    {
 f = gs - 550;

 r = 110 + f;
 g = 110 - f;
 b = 0;
    }
    else
    {
 f = (gs - 660) << 2;

 r = 220 - f / 3;
 g = f / 6;
 b = f / 6;
    }

    r += 0x10;
    g += 0x10;
    b += 0x10;

    f = (r << 0x10) + (g << 0x8) + b;

    return f;
}

unsigned long color_heati(unsigned long x)
{
    volatile register unsigned long r, g, b, gs, f;

    r = (x & 0xff0000) >> 0x10;
    g = (x & 0xff00) >> 0x8;
    b = x & 0xff;

    gs = r + g + b;
    f = gs;

    if (gs <= 110)
    {
 r = gs;
 g = 0;
 b = gs << 1;
    }
    else if (gs <= 220)
    {
 r = 220 - f;
 g = 0;
 b = 220;
    }
    else if (gs <= 330)
    {
 f = gs - 220;
 r = 0;
 g = f << 1;
 b = 220;
    }
    else if (gs <= 440)
    {
 f = gs - 330;
 r = 0;
 g = 220;
 b = 220 - (f << 1);
    }
    else if (gs <= 550)
    {
 f = gs - 440;
 r = f << 1;
 g = 220;
 b = 0;
    }
    else if (gs <= 660)
    {
 f = gs - 550;
 r = 220;
 g = 220 - (f << 1);
 b = 0;
    }
    else
    {
 f = gs - 660;
 r = 220;
 g = f << 1;
 b = f << 1;
    }

    f = (r << 0x10) + (g << 0x8) + b;

    return f;
}

double color_heat2(double x)
{
    unsigned long lx, lr, lg, lb;
    double gs, f, r, g, b;
# 1264 "udf.c"
    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    gs = (double)(lr + lg + lb) / 765.;

    if (gs < (1./7.))
    {
 f = 7. * gs;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = .6 * f;
 g = 0.;
 b = f;
    }
    else if (gs >= (1./7.) && gs < (2./7.))
    {
 f = (7. * gs) - 1.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = .6 * (1. - f);
 g = 0.;
 b = 1.;
    }
    else if (gs >= (2./7.) && gs < (3./7.))
    {
 f = (7. * gs) - 2.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = 0.;
 g = f;
 b = 1.;
    }
    else if (gs >= (3./7.) && gs < (4./7.))
    {
 f = (7. * gs) - 3.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = 0.;
 g = 1.;
 b = 1. - f;
    }
    else if (gs >= (4./7.) && gs < (5./7.))
    {
 f = (7. * gs) - 4.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = f;
 g = 1.;
 b = 0.;
    }
    else if (gs >= (5./7.) && gs < (6./7.))
    {
 f = (7. * gs) - 5.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = 1.;
 g = 1. - f;
 b = 0.;
    }
    else if (gs >= (6./7.))
    {
 f = (7. * gs) - 6.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = 1.;
 g = f;
 b = f;
    }
    else
    {
 r = g = b = .5;
    }

    r = .5 * (-cos( r * 3.14159265358979323846) + 1.);
    g = .5 * (-cos( g * 3.14159265358979323846) + 1.);
    b = .5 * (-cos( b * 3.14159265358979323846) + 1.);


    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_heat(double x)
{
    unsigned long lx, lr, lg, lb;
    double gs, f, r, g, b;
# 1369 "udf.c"
    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    gs = (double)(lr + lg + lb) / 765.;

    if (gs < (1./7.))
    {
 f = 7. * gs;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = .6 * f;
 g = 0.;
 b = f;
    }
    else if (gs >= (1./7.) && gs < (2./7.))
    {
 f = (7. * gs) - 1.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = .6 * (1. - f);
 g = 0.;
 b = 1.;
    }
    else if (gs >= (2./7.) && gs < (3./7.))
    {
 f = (7. * gs) - 2.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = 0.;
 g = f;
 b = 1.;
    }
    else if (gs >= (3./7.) && gs < (4./7.))
    {
 f = (7. * gs) - 3.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = 0.;
 g = 1.;
 b = 1. - f;
    }
    else if (gs >= (4./7.) && gs < (5./7.))
    {
 f = (7. * gs) - 4.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = f;
 g = 1.;
 b = 0.;
    }
    else if (gs >= (5./7.) && gs < (6./7.))
    {
 f = (7. * gs) - 5.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = 1.;
 g = 1. - f;
 b = 0.;
    }
    else if (gs >= (6./7.))
    {
 f = (7. * gs) - 6.;
        f = .5 * ( -cos( f * 3.14159265358979323846 ) + 1. );

 r = 1.;
 g = f;
 b = f;
    }
    else
    {
 r = g = b = .5;
    }

    r = .5 * (-cos( r * 3.14159265358979323846) + 1.);
    g = .5 * (-cos( g * 3.14159265358979323846) + 1.);
    b = .5 * (-cos( b * 3.14159265358979323846) + 1.);


    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_ghost(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b, r2, g2, b2;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = 1. - (double)lr / 255.;
    g = 1. - (double)lg / 255.;
    b = 1. - (double)lb / 255.;

    r2 = cbrt(b) * g * g;
    g2 = cbrt(r) * b * b;
    b2 = cbrt(g) * r * r;

    lr = (unsigned long)(r2 * 255.);
    lg = (unsigned long)(g2 * 255.);
    lb = (unsigned long)(b2 * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_inv(double x)
{
    unsigned long lx, lr, lg, lb;
    double r, g, b;

    lx = (unsigned long)x;

    lr = (lx & 0xFF0000) >> 0x10;
    lg = (lx & 0xFF00) >> 0x8;
    lb = lx & 0xFF;

    r = 1. - (double)lr / 255.;
    g = 1. - (double)lg / 255.;
    b = 1. - (double)lb / 255.;

    lr = (unsigned long)(r * 255.);
    lg = (unsigned long)(g * 255.);
    lb = (unsigned long)(b * 255.);

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x = (double)lx;

    return x;
}

double color_box(double x1, double x2)
{
    unsigned long lx, lr, lg, lb, l1, l2;
    int cx, cy;

    x2 = 768. - x2;

    cx = x1 / 256.;
    cy = x2 / 256.;

    l1 = (unsigned long)x1;
    l2 = (unsigned long)x2;

    l1 %= 0x100;
    l2 %= 0x100;

    lr = lg = lb = 0;

    if (cy == 0)
    {
 if (cx == 1)
 {
     lr = l1;
     lb = 0xff - l2;
 }
 else if (cx == 0)
 {
     lr = l1;
     lg = l2;
     lb = .5*(l1+l2);
 }
 else if (cx == 2)
 {
     lr = l1;
     lg = .5*(l1+l2);
     lb = l2;
 }
 else if (cx == 3)
 {
     lr = .5*(l1+l2);
     lg = l1;
     lb = l2;
 }
    }
    else if (cy == 1)
    {
 if (cx == 0)
 {
     lg = l2;
     lb = 0xff - l1;
 }
 else if (cx == 1)
 {
     lr = l1;
     lg = l2;
 }
 else if (cx == 2)
 {
     lr = 0xff;
     lg = l2;
     lb = l1;
 }
 else if (cx == 3)
 {
     lr = 0xff - l1;
     lg = l2;
     lb = 0xff;
 }
    }
    else if (cy == 2)
    {
 if (cx == 1)
 {
     lr = l1;
     lg = 0xff;
     lb = l2;
 }
 else if (cx == 0)
 {
     lr = l2;
     lg = l1;
     lb = .5*(l1+l2);
 }
 else if (cx == 2)
 {
     lr = l2;
     lg = .5*(l1+l2);
     lb = l1;
 }
 else if (cx == 3)
 {
     lr = .5*(l1+l2);
     lg = l2;
     lb = l1;
 }
    }

    lx = (lr << 0x10) + (lg << 0x8) + lb;

    x1 = (double)lx;

    return x1;
}

double min_udf2(double x1, double x2, double x3)
{
    double v1, v2;

    v1 = udf2(x1, x2);
    v2 = udf2(x2, x3);

    return (v1 < v2) ? v1 : v2;
}

double udf3(double x1, double x2, double x3)
{


    return alien_gs3(x1, x2, x3);
}


double udf2(double x1, double x2)
{
# 1650 "udf.c"
    return color_box(x1, x2);
}

double udf1(double x)
{
# 1669 "udf.c"
    return alien_gs(x);

}

unsigned long udfi1(unsigned long x)
{



    return alien_gsi(x);





}

unsigned long udfi2(unsigned long x1, unsigned long x2)
{

    return edge_detect_comix(x1, x2);
}

unsigned long udfi3(unsigned long x1, unsigned long x2, unsigned long x3)
{







    return color_invi3(x1, x2, x3);
}

unsigned long udfi6(unsigned long x1, unsigned long x2, unsigned long x3, unsigned long x4, unsigned long x5, unsigned long x6)
{

    unsigned long xt[6];
    xt[0] = x1;
    xt[1] = x2;
    xt[2] = x3;
    xt[3] = x4;
    xt[4] = x5;
    xt[5] = x6;


    return alien_gsi6(xt);
}
