//#include <stdint.h>
#include <mpc.h>
#include <iostream>
#include <pthread.h>

int base;
int dprec;
int prec;

class MPC;

class MPFR
{
    public:
	MPFR(int _base, int _prec, int _dprec)
	{
	    b = _base;
	    p = _prec;
	    dp = _dprec;
	    init((char*)"0");
	}

	MPFR()
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init((char*)"0");
	}

	MPFR(int _base, int _prec, int _dprec, char* _str)
	{
	    b = _base;
	    p = _prec;
	    dp = _dprec;
	    init(_str);
	}

	MPFR(char* _str)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(_str);
	}

	MPFR(mpfr_t& _v)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(_v);
	}

	MPFR(const MPFR& _v)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(_v);
	}

	MPFR(const MPFR* _v)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(*_v);
	}

	MPFR(long double _v)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(_v);
	}

	~MPFR()
	{
	    //std::cout << "MPFR destroy: " << this << ": " << *this << std::endl;
	    mpfr_clear(v);
	    //std::cout<<"done\n";
	}

	void init(char* str)
	{
	    mpfr_init2(v, p);
            mpfr_set_str(v, str, b, MPFR_RNDN);
	}

	void init(mpfr_t& _v)
	{
	    mpfr_init2(v, p);
            mpfr_set(v, _v, MPFR_RNDN);
	}

	void init(long double _v)
	{
	    mpfr_init2(v, p);
            mpfr_set_ld(v, _v, MPFR_RNDN);
	}

	void init(const MPFR& _v)
	{
	    //std::cout << "MPFR init cref: " << this << ": " << _v << std::endl;
	    mpfr_init2(v, p);
            mpfr_set(v, _v.v, MPFR_RNDN);
	    //std::cout << "MPFR init cref done: " << this << ": " << *this << std::endl;
	}

	friend std::ostream& operator<<(std::ostream&, const MPFR& );
	friend class MPC;

	bool operator<=(int);
	bool operator<=(const MPFR&);
	bool operator>(int);
	bool operator<(int);
	bool operator>=(int);
	bool operator==(int);
	bool operator!=(int);
	MPFR operator-(int);
	MPFR operator+(int);
	void operator+=(const MPFR&);
	MPFR operator*(const MPFR&);

	MPFR& operator=(const MPFR&);

	long double ld();

    private:
	int b, p, dp;
	mpfr_t v;
};

MPFR& MPFR::operator=(const MPFR& _v)
{
    init(_v);
    return *this;
}

bool MPFR::operator<=(int v)
{
    int p = mpfr_cmp_si(this->v, v);
    return (p <= 0);
}

bool MPFR::operator>(int v)
{
    int p = mpfr_cmp_si(this->v, v);
    return (p == 1);
}
bool MPFR::operator<(int v)
{
    int p = mpfr_cmp_si(this->v, v);
    return (p == -1);
}
bool MPFR::operator>=(int v)
{
    int p = mpfr_cmp_si(this->v, v);
    return (p >= 0);
}
bool MPFR::operator==(int v)
{
    int p = mpfr_cmp_si(this->v, v);
    return (p == 0);
}
bool MPFR::operator!=(int v)
{
    int p = mpfr_cmp_si(this->v, v);
    return (p != 0);
}

MPFR MPFR::operator-(int _v)
{
    MPFR r;
    mpfr_sub_si(r.v, v, _v, MPFR_RNDN);
/*    std::cout << *this << " - " << _v << " = " << r << std::endl;*/
    return r;
}

void MPFR::operator+=(const MPFR& _v)
{
    mpfr_add(this->v, this->v, _v.v, MPFR_RNDN);
}

bool MPFR::operator<=(const MPFR& _v)
{
    int p = mpfr_cmp(this->v, _v.v);
    return (p <= 0);
}
	
MPFR MPFR::operator*(const MPFR& _v)
{
    MPFR r;
    mpfr_mul(r.v, v, _v.v, MPFR_RNDN);
/*    std::cout << *this << " * " << _v << " = " << r << std::endl;*/
    return r;
}


MPFR MPFR::operator+(int _v)
{
    MPFR r;
    mpfr_add_si(r.v, v, _v, MPFR_RNDN);
/*    std::cout << *this << " + " << _v << " = " << r << std::endl;*/
    return r;
}

long double MPFR::ld()
{
    return mpfr_get_ld(v, MPFR_RNDN);
}

class MPC
{
    public:
	MPC(int _base, int _prec, int _dprec)
	{
	    b = _base;
	    p = _prec;
	    dp = _dprec;
	    init((char*)"0");
	}

	MPC()
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init((char*)"0");
	}

	MPC(int _base, int _prec, int _dprec, char* _str)
	{
	    b = _base;
	    p = _prec;
	    dp = _dprec;
	    init(_str);
	}

	MPC(char* _str)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(_str);
	}

	MPC(mpc_t& _v)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(_v);
	}

	MPC(const MPC& _v)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(_v);
	}

	MPC(const MPC* _v)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(*_v);
	}

	MPC(MPFR& f1, MPFR& f2)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(f1.v, f2.v);
	}

	MPC(MPFR& f)
	{
	    b = base;
	    p = prec;
	    dp = dprec;

	    mpfr_t f0;
	    mpfr_init2(f0, p);
	    mpfr_set_si(f0, 0, MPFR_RNDN);
	    init(f.v, f0);
	    mpfr_clear(f0);
	}

	MPC(mpfr_t f1, mpfr_t f2)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(f1, f2);
	}

	MPC(long double f1, long double f2)
	{
	    b = base;
	    p = prec;
	    dp = dprec;
	    init(f1, f2);
	}

	~MPC()
	{
	    //std::cout << "MPC destroy: " << this << ": " << *this << std::endl;
	    mpc_clear(v);
	    //std::cout<<"done\n";
	}

	void init(char* str)
	{
	    mpc_init2(v, p);
            mpc_set_str(v, str, b, MPC_RNDNN);
	}

	void init(mpc_t& _v)
	{
	    mpc_init2(v, p);
            mpc_set(v, _v, MPC_RNDNN);
	}

	void init(mpfr_t f1, mpfr_t f2)
	{
	    mpc_init2(v, p);
	    mpc_set_fr_fr(v, f1, f2, MPC_RNDNN);
	}

	void init(MPFR& f1, MPFR& f2)
	{
	    mpc_init2(v, p);
	    mpc_set_fr_fr(v, f1.v, f2.v, MPC_RNDNN);
	}

	void init(long double f1, long double f2)
	{
	    mpc_init2(v, p);
	    mpc_set_ld_ld(v, f1, f2, MPC_RNDNN);
	}

	void init(const MPC& _v)
	{
	    //std::cout << "MPC init cref: " << this << ": " << _v << std::endl;
	    mpc_init2(v, p);
            mpc_set(v, _v.v, MPC_RNDNN);
	    //std::cout << "MPC init cref done: " << this << ": " << *this << std::endl;
	}

	MPC tetra_quadric(MPFR&);
	MPC tetra_linear(MPFR&);
	MPC operator^(const MPC&);
	MPC operator%(const MPC&);
	MPC operator|(const MPC&);
	MPC operator*(int _v);
	MPC operator*(const MPFR& _v);
	MPC operator+(int _v);
	MPC operator+(const MPC& _v);
	MPC operator-(const MPC& _v);
	MPC operator/(const MPC& _v);
	MPC& operator=(const MPC& _v);

	friend std::ostream& operator<<(std::ostream&, const MPC& );
	friend class MPFR;

	MPFR real();
	MPFR imag();

    private:
	int b, p, dp;
	mpc_t v;
};

MPC& MPC::operator=(const MPC& _v)
{
    init(_v);
    return *this;
}

MPC MPC::operator*(int _v)
{
    MPC r;
    mpc_mul_si(r.v, v, _v, MPC_RNDNN);
/*    std::cout << *this << " * " << _v << " = " << r << std::endl;*/
    return r;
}
	
MPC MPC::operator*(const MPFR& _v)
{
    MPC r;
    mpc_mul_fr(r.v, v, _v.v, MPC_RNDNN);
/*    std::cout << *this << " * " << _v << " = " << r << std::endl;*/
    return r;
}

MPC MPC::operator+(int _v)
{
    MPC r;
    mpc_add_si(r.v, v, _v, MPC_RNDNN);
/*    std::cout << *this << " + " << _v << " = " << r << std::endl;*/
    return r;
}

MPC MPC::operator+(const MPC& _v)
{
    MPC r;
    mpc_add(r.v, v, _v.v, MPC_RNDNN);
/*    std::cout << *this << " + " << _v << " = " << r << std::endl;*/
    return r;
}

MPC MPC::operator-(const MPC& _v)
{
    MPC r;
    mpc_sub(r.v, v, _v.v, MPC_RNDNN);
/*    std::cout << *this << " - " << _v << " = " << r << std::endl;*/
    return r;
}

MPC MPC::operator/(const MPC& _v)
{
    MPC r;

    mpc_div(r.v, v, _v.v, MPC_RNDNN);

/*    std::cout << *this << " / " << _v << " = " << r << std::endl;*/
    return r;
}

MPC MPC::operator^(const MPC& _v)
{
    MPC r;

    mpc_pow(r.v, v, _v.v, MPC_RNDNN);
    /*
    mpc_log(r.v, v, MPC_RNDNN);
    mpc_mul(r.v, r.v, _v.v, MPC_RNDNN);
    mpc_exp(r.v, r.v, MPC_RNDNN);
    */

/*    std::cout << *this << " ^ " << _v << " = " << r << std::endl;*/
    return r;
}

MPC MPC::operator|(const MPC& _v)
{
    MPC r, logx, loga;

    mpc_log(logx.v, _v.v, MPC_RNDNN);
    mpc_log(loga.v, v, MPC_RNDNN);
    mpc_div(r.v, logx.v, loga.v, MPC_RNDNN);

/*    std::cout << *this << " % " << _v << " = " << r << std::endl;*/
    return r;
}

MPC MPC::operator%(const MPC& _v)
{
    MPC r;

    mpc_log(r.v, _v.v, MPC_RNDNN);

/*    std::cout << *this << " $ " << _v << " = " << r << std::endl;*/
    return r;
}

MPFR MPC::real()
{
    MPFR r;
    mpc_real(r.v, v, MPFR_RNDN);
    return r;
}

MPFR MPC::imag()
{
    MPFR r;
    mpc_imag(r.v, v, MPFR_RNDN);
    return r;
}

std::ostream& operator<<(std::ostream& str, const MPFR& v)
{
    mpfr_exp_t e;
    //intmax_t e;
    char* s = mpfr_get_str(NULL, &e, v.b, v.dp, v.v, MPFR_RNDN);
    char* s0 = s;

    if (*s == '-') str.put(*s++);
    str.put(*s++); e--;
    str.put('.');
    while (*s != '\0') str.put(*s++);
    mpfr_free_str(s0);
    if (e) str << (v.b <= 10 ? 'e' : '@') << (long)e;

    return str;
}

std::ostream& operator<<(std::ostream& str, const MPC& v)
{
    char* s = mpc_get_str(v.b, v.dp, v.v, MPC_RNDNN);
    str << s;
    mpc_free_str(s);
    return str;
}

MPC MPC::tetra_quadric(MPFR& b)
{
    MPC a(v);
    // <-inf -1](-1 0](0 +Inf>	
    //std::cout << "tetra: " << a << " and " << b << std::endl;

    if (b <= -1)
    {
	// log(a, (x+1))
	// this is in | operator
	MPFR b2 = b + 1;
	return a | a.tetra_quadric( b2 );
    }
    else if (b > -1 && b <= 0)
    {
	// operators: ^ is pow, % is log, 
	// std::cout << "ret " << a << " % " << a % a << "\n";
	// return a;

	return ((a%a)*2)/((a%a)+1)*b - ((((a%a)*-1)+1)/((a%a)+1))*b*b + 1;
    }
    else if (b > 0)
    {
	// a ^^ (x-1)
	MPFR b2 = b - 1;
	return a ^ a.tetra_quadric( b2 );
    }
}
	
MPC MPC::tetra_linear(MPFR& b)
{
    MPC a(v);
    // <-inf -1](-1 0](0 +Inf>	
    //std::cout << "tetra: " << a << " and " << b << std::endl;

    if (b <= -1)
    {
	// log(a, (x+1))
	// this is in | operator
	MPFR b2 = b + 1;
	return a | a.tetra_linear( b2 );
    }
    else if (b > -1 && b <= 0)
    {
	// operators: ^ is pow, % is log, 
	// std::cout << "ret " << a << " % " << a % a << "\n";
	// return a;
	MPFR b2 = b + 1;
	return MPC(b2);
    }
    else if (b > 0)
    {
	// a ^^ (x-1)
	MPFR b2 = b - 1;
	return a ^ a.tetra_linear( b2 );
    }
}

struct mpc_data
{
    MPC *a, *c;
    MPFR *b;
/*    int r, i, x;*/
};

struct thr_data
{
    mpc_data**** mpd;
    MPFR r0, r1, ri, i0, i1, ii, x0, x1, xi;
    int r, i, x, idx, nth;
    MPFR *rv, *iv, *xv;
};

pthread_mutex_t mtx;

void* pth_work(void* ptr)
{
    thr_data* td;
    MPC *a, *c;
    MPFR *b;

    a = new MPC();
    b = new MPFR();
    c = new MPC();

/*    printf("%d\n", mpfr_buildopt_tls_p());*/
/*    exit(1);*/

    td = (thr_data*)ptr;
/*    printf("I'm thread #%d\n", td->idx);*/

    for (int ri=0;ri<td->r;ri++)
    {
	fprintf(stderr, "%f%%\n", 100. * ((float)ri / (float)td->r));
	for (int ii=0;ii<td->i;ii++)
	{
	    a->init(td->rv[ri], td->iv[ii]);
	    for (int xi=td->idx;xi<td->x;xi+=td->nth)
	    {
		b->init(td->xv[xi]);
		//c = a.tetra_quadric(b) - a.tetra_linear(b);
		td->mpd[ri][ii][xi]->a = new MPC(a);
		td->mpd[ri][ii][xi]->b = new MPFR(b);
	    	//pthread_mutex_lock(&mtx);
		*c = a->tetra_quadric(*b);
	    	//pthread_mutex_unlock(&mtx);
		//td->mpd[ri][ii][xi]->c = c;
		td->mpd[ri][ii][xi]->c = new MPC(c);
		//c = a.tetra_linear(b);
/*		printf("(%d,%d,%Lf,%Lf,%Lf,%Lf,%Lf)\n", td->idx, xi, a->real().ld(), a->imag().ld(), b->ld(), c->real().ld(), c->imag().ld());*/
	    }
	}
    }

    delete a;
    delete b;
    delete c;

/*    printf("Finished #%d\n", td->idx);*/

    return NULL;
}

int main(int lb, char** par)
{
    long double ar, ai, bd, mode;

    if (lb < 15)
    {

        printf("%d %d\n", _MPFR_EXP_FORMAT, sizeof(intmax_t));
	printf("usage %s base[2-36] prec[2-Inf?] disp_digits[0-Inf?] real_from real_to real_inc imag_from imag_to imag_inc x_from x_to x_inc mode [0-long double, 1-MPC/MPFR] n_threads\n", par[0]);
	return 1;
    }

    base  = atoi(par[1]);
    prec  = atoi(par[2]);
    dprec = atoi(par[3]);

    struct thr_data tdt;

    tdt.r0 = MPFR(par[4]);
    tdt.r1 = MPFR(par[5]);
    tdt.ri = MPFR(par[6]);

    tdt.i0 = MPFR(par[7]);
    tdt.i1 = MPFR(par[8]);
    tdt.ii = MPFR(par[9]);

    tdt.x0 = MPFR(par[10]);
    tdt.x1 = MPFR(par[11]);
    tdt.xi = MPFR(par[12]);

    mode = atoi(par[13]);

    int nth;
    nth = atoi(par[14]);

    mpc_data**** pd;

    tdt.r = tdt.i = tdt.x = 0;
    for (MPFR r=tdt.r0;r<=tdt.r1;r+=tdt.ri) tdt.r++;
    for (MPFR i=tdt.i0;i<=tdt.i1;i+=tdt.ii) tdt.i++;
    for (MPFR x=tdt.x0;x<=tdt.x1;x+=tdt.xi) tdt.x ++;

    pd = new mpc_data***[tdt.r];
    for (int r=0;r<tdt.r;r++) 
    {
	pd[r] = new mpc_data**[tdt.i];
	for (int i=0;i<tdt.i;i++) 
	{
	    pd[r][i] = new mpc_data*[tdt.x];
	    for (int x=0;x<tdt.x;x++) pd[r][i][x] = new mpc_data;
	}
    }

    tdt.mpd = pd;
    tdt.nth = nth;

    tdt.rv = new MPFR[tdt.r];
    tdt.iv = new MPFR[tdt.i];
    tdt.xv = new MPFR[tdt.x];

    int j = 0;
    for (MPFR r=tdt.r0;r<=tdt.r1;r+=tdt.ri) tdt.rv[j++] = r;
    j = 0;
    for (MPFR i=tdt.i0;i<=tdt.i1;i+=tdt.ii) tdt.iv[j++] = i;
    j = 0;
    for (MPFR x=tdt.x0;x<=tdt.x1;x+=tdt.xi) tdt.xv[j++] = x;

    pthread_t* th;

    th = new pthread_t[nth];
    thr_data* dptr;

    pthread_mutex_init(&mtx, NULL);

    for (int i=0;i<nth;i++)
    {
	dptr = new thr_data;
	memcpy((void*)dptr, (void*)(&tdt), sizeof(thr_data));
	dptr->idx = i;
	pthread_create(&th[i], NULL, pth_work, dptr);
    }

    for (int i=0;i<nth;i++)
    {
	pthread_join(th[i], NULL);
    }

    pthread_mutex_destroy(&mtx);

    int ri, ii, xi;

    for (int ri=0;ri<tdt.r;ri++)
    {
	for (int ii=0;ii<tdt.i;ii++)
	{
	    for (int xi=0;xi<tdt.x;xi++)
	    {
		if (mode == 1) std::cout << ri << ";" << ii << ";" << xi << ";" << pd[ri][ii][xi]->a->real() << ";" << pd[ri][ii][xi]->a->imag() << ";" << *(pd[ri][ii][xi]->b) << ";" << pd[ri][ii][xi]->c->real() << ";" << pd[ri][ii][xi]->c->imag() << ";" << std::endl;
		else std::cout << ri << ";" << ii << ";" << xi << ";" << pd[ri][ii][xi]->a->real().ld() << ";" << pd[ri][ii][xi]->a->imag().ld() << ";" << pd[ri][ii][xi]->b->ld() << ";" << pd[ri][ii][xi]->c->real().ld() << ";" << pd[ri][ii][xi]->c->imag().ld() << ";" << std::endl;
	    }
	}
    }

    /*
    MPC a, c;
    MPFR b;
    int _r, _i, _x;

    _r = 0;
    for (MPFR r=r0;r<=r1;r+=ri)
    {
	_i = 0;
	for (MPFR i=i0;i<=i1;i+=ii)
	{
	    a.init(r, i);
	    _x = 0;
	    for (MPFR x=x0;x<=x1;x+=xi)
	    {
		b.init(x);
		//c = a.tetra_quadric(b) - a.tetra_linear(b);
		c = a.tetra_quadric(b);
		//c = a.tetra_linear(b);
		//std::cout << a << " ^^ " << b << " = " << c << std::endl;
		if (mode == 1) std::cout << _r << ";" << _i << ";" << _x << ";" << a.real() << ";" << a.imag() << ";" << b << ";" << c.real() << ";" << c.imag() << ";" << std::endl;
		else std::cout << _r << ";" << _i << ";" << _x << ";" << a.real().ld() << ";" << a.imag().ld() << ";" << b.ld() << ";" << c.real().ld() << ";" << c.imag().ld() << ";" << std::endl;
		_x ++;
	    }
	    _i ++;
	}
	_r ++;
    }
    */

    return 0;
}

