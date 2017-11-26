#include <stdio.h>
#include <stdlib.h>
#include <mpfr.h>
#include <mpc.h>
#include <iostream>
#include <vector>
#include <complex>
#include <pthread.h>

struct mpfr
{
    mpfr_t v;
};

struct thr_data
{
    int c, n, lb;
    char** par;
};

    
int p, b, dp, warn = 0, debug = 0;
std::vector<mpfr*> f;
std::vector<long double> fcd;
mpfr_t max_val;
double divg_limit = 2.;
pthread_mutex_t mtx;
bool bi_computed = false;

void lck()
{
    pthread_mutex_lock(&mtx);
}

void sig()
{
    pthread_mutex_unlock(&mtx);
}

std::ostream& operator<<(std::ostream& str, mpc_t v)
{
    char* s = mpc_get_str(b, dp, v, MPC_RNDNN);
    str << s;
    mpc_free_str(s);
    return str;
}

std::ostream& operator<<(std::ostream& str, mpfr_t v)
{
    mpfr_exp_t e;
    //intmax_t e;
    char* s = mpfr_get_str(NULL, &e, b, dp, v, MPFR_RNDN);
    char* s0 = s;

    if (*s == '-') str.put(*s++);
    str.put(*s++); e--;
    str.put('.');
    while (*s != '\0') str.put(*s++);
    mpfr_free_str(s0);
    if (e) str << (b <= 10 ? 'e' : '@') << (long)e;

    return str;
}

void init_binomials(int nf)
{
    mpfr* v;
    double di;

    mpfr_init2(max_val, p);
    mpfr_set_ui(max_val, 10, MPFR_RNDN);
    mpfr_pow_ui(max_val, max_val, p/2, MPFR_RNDN);

    v = new mpfr;
    mpfr_init2(v->v, p);
    mpfr_set_ld(v->v, 1.0, MPFR_RNDN);
    f.push_back( v );
    fcd.push_back( 1. );

    for (int i=1;i<nf;i++)
    {
	di = (double)i;
	v = new mpfr;
	mpfr_init2(v->v, p);
	mpfr_mul_d(v->v, f[i-1]->v, di, MPFR_RNDN);
	f.push_back( v );
	di *= fcd[i-1];
	fcd.push_back( di );
    }

    //for (int i=0;i<nf;i++) std::cout<< i << ": " << f[i]->v << " " << fcd[i] << std::endl;
    //std::cout<< v->v << std::endl;
}

void tetra_linear(mpc_t C, const mpc_t& A, const mpfr_t& B)
{
    mpc_set_fr(C, B, MPC_RNDNN);
    mpc_add_ui(C, C, 1, MPC_RNDNN);
}

void compute_binomial(mpc_t bi, int n, int k)
{
    mpc_set_fr(bi, f[n]->v, MPC_RNDNN);
    mpc_div_fr(bi, bi, f[k]->v, MPC_RNDNN);
    mpc_div_fr(bi, bi, f[n-k]->v, MPC_RNDNN);
}

void compute_binomial(mpfr_t bi, int n, int k)
{
    mpfr_set(bi, f[n]->v, MPFR_RNDN);
    mpfr_div(bi, bi, f[k]->v, MPFR_RNDN);
    mpfr_div(bi, bi, f[n-k]->v, MPFR_RNDN);
}

long double compute_binomial(int n, int k)
{
    return fcd[n] / ( fcd[k] * fcd[n-k] );
}

void compute_binomial(mpc_t bi, mpc_t x, int k)
{
    mpc_t x2;

    mpc_set_ui(bi, 1, MPC_RNDNN);
    mpc_init2(x2, p);
    mpc_set(x2, x, MPC_RNDNN);

    for (int i=0;i<k;i++)
    {
	mpc_mul(bi, bi, x2, MPC_RNDNN);
	mpc_sub_ui(x2, x2, 1, MPC_RNDNN);
    }

    //std::cout<< "!" << f[k]->v << " " << bi << std::endl;
    mpc_div_fr(bi, bi, f[k]->v, MPC_RNDNN);
    mpc_clear(x2);
}

void compute_binomial(mpfr_t bi, mpfr_t x, int k)
{
    mpfr_t x2;

    mpfr_set_ui(bi, 1, MPFR_RNDN);
    mpfr_init2(x2, p);
    mpfr_set(x2, x, MPFR_RNDN);

    for (int i=0;i<k;i++)
    {
	mpfr_mul(bi, bi, x2, MPFR_RNDN);
	mpfr_sub_ui(x2, x2, 1, MPFR_RNDN);
    }

    //std::cout<< "!" << f[k]->v << " " << bi << std::endl;
    mpfr_div(bi, bi, f[k]->v, MPFR_RNDN);
    mpfr_clear(x2);
}

long double compute_binomial(long double x, int k)
{
    long double bi = 1.0;

    for (int i=0;i<k;i++)
    {
	bi *= x;
	x -= 1.0;
    }
    bi /= fcd[k];
    //std::cout<< "!" << f[k]->v << " " << bi << std::endl;
    return bi;
}

void tetra_for_int(mpc_t t, const mpc_t& A, int b)
{
    //std::cout<<"TI(" << A << ", " << b << ")" << std::endl;

    if (b == -1) mpc_set_ui(t, 0, MPC_RNDNN);
    else if (b == 0)  mpc_set_ui(t, 1, MPC_RNDNN);
    else if (b == 1)  mpc_set(t, A, MPC_RNDNN);
    else
    {
	mpc_t c2;
	mpc_init2(c2, p);
	tetra_for_int(c2, A, b - 1);
	mpc_pow(t, A, c2, MPC_RNDNN);
	mpc_clear(c2);
    }
}

std::complex<long double> tetra_for_int(const std::complex<long double>& A, int b)
{
    //std::cout<<"TI(" << A << ", " << b << ")" << std::endl;

    if (b == -1) return std::complex<long double>(0., 0.);
    else if (b == 0)  return std::complex<long double>(1., 0.);
    else if (b == 1)  return A;
    else return pow(A, tetra_for_int(A, b - 1));
}

void compute_fak(mpc_t f, const mpc_t& a, int k)
{
    mpfr_t fa;
    tetra_for_int(f, a, k-1);

    mpfr_init2(fa, p);
    mpc_abs(fa, f, MPFR_RNDN);
    int p = mpfr_cmp(fa, max_val);
    if (p >= 0) 
    {
	if (warn) std::cout<< "Overflow at mpc abs: " << fa << std::endl; 
	//mpfr_clear( fa );
	//exit(1);
	mpc_set_ui_ui(f, 0, 0, MPC_RNDNN);
    }
	
    mpfr_clear( fa );
    //std::cout << "TI: " << (k-1) << f << std::endl;
}

std::complex<long double> compute_fak(const std::complex<long double>& a, int k)
{
    long double fa;
    std::complex<long double> f = tetra_for_int(a, k-1);

    fa = abs(f);
    if (fa >= 1e30) 
    {
	if (warn) std::cout<< "Overflow at ld abs: " << fa << std::endl; 
	//exit(1);
	f = std::complex<long double>(0., 0.);
    }

    return f;
}

void compute_delta(mpc_t d, const mpc_t& a, int k)
{
    mpc_t sum, f, m;
    mpfr_t bi;

    mpfr_init2(bi, p);
    mpc_init2(f, p);
    mpc_init2(m, p);
    mpc_set_ui(d, 0, MPC_RNDNN);

    for (int i=0;i<=k;i++)
    {
	compute_binomial(bi, k, i);
	if ((k - i) % 2) mpfr_mul_si(bi, bi, -1, MPFR_RNDN);
	compute_fak(f, a, i);
	mpc_mul_fr(m, f, bi, MPFR_RNDN);
	mpc_add(d, d, m, MPC_RNDNN);

	if (debug>=2) std::cout<<"bi(" << i << ", " << k << "): " << bi << ",\tf: " << f <<  ",\tm: " << m << std::endl;
    }

    mpfr_clear(bi);
    mpc_clear(f);
    mpc_clear(m);
}

std::complex<long double> compute_delta(const std::complex<long double>& a, int k)
{
    std::complex<long double> f, m, d = std::complex<long double>(0., 0.);
    long double bi;

    for (int i=0;i<=k;i++)
    {
	bi = compute_binomial(k, i);
	if ((k - i) % 2) bi *= -1.;
	f = compute_fak(a, i);
	m = bi * f;
	d += m;
	if (debug>=2) std::cout<<"bi(" << i << ", " << k << "): " << bi << ",\tf: " << f <<  ",\tm: " << m << std::endl;
    }

    return d;
}

void tetra_newton_direct(mpc_t C, const mpc_t& A, const mpfr_t& B, int n_steps)
{
    mpc_t m, d, last_m;
    mpfr_t bi, xa, ma, lma;

    mpfr_init2(bi, p);
    mpfr_init2(xa, p);
    mpfr_init2(ma, p);
    mpfr_init2(lma, p);

    mpc_init2(d, p);
    mpc_init2(m, p);
    mpc_init2(last_m, p);

    mpc_set_ui(C, 0, MPC_RNDNN);
    mpfr_set(xa, B, MPFR_RNDN);
    mpfr_add_ui(xa, xa, 1, MPFR_RNDN);

    //std::cout<<"x-a:" << xa << std::endl;
    for (int k=0;k<=n_steps;k++)
    {
	compute_binomial(bi, xa, k);
	compute_delta(d, A, k);
	mpc_mul_fr(m, d, bi, MPC_RNDNN);
	if ( k > 20 )
	{
	    mpc_abs(ma, m, MPFR_RNDN);
	    mpc_abs(lma, last_m, MPFR_RNDN);
	    mpfr_mul_d(lma, lma, divg_limit, MPFR_RNDN);
	    if (mpfr_cmp(ma, lma) == 1)
	    {
		if (warn)
		{
		    std::cout<< "Divergence at: " << k << "/" << n_steps << ": " << m << " > " << last_m << std::endl;
		    std::cout<< "Abs: M: " << ma << ", " << divg_limit << "*Prev_M: " << lma << std::endl;
		    std::cout<<"bi: " << bi << ",\td: " << d << ",\tM: " << m << std::endl;
		}
		break;
	    }
	}
	mpc_add(C, C, m, MPC_RNDNN);
	mpc_set(last_m, m, MPC_RNDNN);
	//std::cout<<"x-a:" << xa << ", bi: " << bi << ", d: " << d << std::endl;
	if (debug>=1) std::cout<<"bi: " << bi << ",\td: " << d << ",\tM: " << m << std::endl;
    }

    mpfr_clear(bi);
    mpfr_clear(xa);
    mpfr_clear(ma);
    mpfr_clear(lma);

    mpc_clear(d);
    mpc_clear(m);
    mpc_clear(last_m);

    //std::cout<< "direct result: " << C << std::endl;
}

void tetra_newton(mpc_t C, const mpc_t& A, const mpfr_t& B, int n_steps)
{
    // <-inf -1](-1 0](0 +Inf>	
    //std::cout << A << " " << B << std::endl;
    int p1 = mpfr_cmp_si(B, -1);
    int p0 = mpfr_cmp_si(B, 0);
    //printf("-1> %d 0> %d\n", p1, p0);

    if (p1 <= 0)
    {
	// log(a, (x+1))
	mpfr_t b2;
	mpc_t c2;
	mpfr_init2(b2, p);
	mpc_init2(c2, p);
	mpfr_add_ui(b2, B, 1, MPFR_RNDN);
	tetra_newton(c2, A, b2, n_steps);
	mpc_log(C, c2, MPC_RNDNN);
	mpc_log(c2, A, MPC_RNDNN);
	mpc_div(C, C, c2, MPC_RNDNN);
	mpc_clear(c2);
	mpfr_clear(b2);
    }
    else if (p1 > 0 && p0 <= 0)
    {
	tetra_newton_direct(C, A, B, n_steps);
    }
    else if (p0 > 0)
    {
	mpfr_t b2;
	mpc_t c2;
	mpfr_init2(b2, p);
	mpc_init2(c2, p);
	mpfr_sub_ui(b2, B, 1, MPFR_RNDN);
	tetra_newton(c2, A, b2, n_steps);
	mpc_pow(C, A, c2, MPC_RNDNN);
	mpc_clear(c2);
	mpfr_clear(b2);
    }
}

std::complex<long double> tetra_newton_direct(const std::complex<long double>& A, const long double& B, int n_steps)
{
    std::complex<long double> C, m, last_m, d;
    long double bi, xa;

    C = std::complex<long double>(0., 0.);
    xa = B+1.;

    //std::cout<<"x-a:" << xa << std::endl;
    for (int k=0;k<=n_steps;k++)
    {
	bi = compute_binomial(xa, k);
	d  = compute_delta(A, k);
	m = bi * d;
	if ( k > 20 && abs(m) > divg_limit * abs(last_m) )
	{
	    if (warn)
	    {
		std::cout<< "Divergence at: " << k << "/" << n_steps << ": " << m << " > " << last_m << std::endl;
		std::cout<< "Abs: M: " << abs(m) << ", " << divg_limit << "*Prev_M: " << (divg_limit * abs(last_m)) << std::endl;
		std::cout<<"bi: " << bi << ",\td: " << d << ",\tM: " << m << std::endl;
	    }
	    break;
	}
	C += m;
	last_m = m;
	if (debug>=1) std::cout<<"bi: " << bi << ",\td: " << d << ",\tM: " << m << std::endl;
    }

    //std::cout<< "direct result: " << C << std::endl;
    return C;
}

std::complex<long double> tetra_newton(const std::complex<long double>& A, const long double& B, int n_steps)
{
    // <-inf -1](-1 0](0 +Inf>	
    if (B <= -1.)
    {
	// log(a, (x+1))
	return log(tetra_newton(A, B+1., n_steps)) / log(A);
    }
    else if (B > -1. && B <= 0.)
    {
	return tetra_newton_direct(A, B, n_steps);
	//return std::complex<long double>(B+1., 0.);
    }
    else if (B > 0.)
    {
	// pow(a, (x-1))
	return pow(A, tetra_newton(A, B-1., n_steps));
    }
}

long double ld(const mpfr_t& v)
{
    return mpfr_get_ld(v, MPFR_RNDN);
}

long double ld_real(const mpc_t& v)
{
    mpfr_t r;
    mpfr_init2(r, p);
    mpc_real(r, v, MPFR_RNDN);
    long double d = ld(r);
    mpfr_clear(r);
    return d;
}

long double ld_imag(const mpc_t& v)
{
    mpfr_t r;
    mpfr_init2(r, p);
    mpc_imag(r, v, MPFR_RNDN);
    long double d = ld(r);
    mpfr_clear(r);
    return d;
}

int go_thread(int lb, char** par, int _c, int _n)
{
    mpc_t A, C;
    mpfr_t bx, AAr, AAi, Cr, Ci;
    long double Arf, Art, Ars, Aif, Ait, Ais, Bdf, Bdt, Bds;
    std::complex<long double> Ac, Cc;
    int n_thr, ns = 20, flag = 1, disp_mpc = 1;
    
    if (lb < 11)
    {
	if (!_c)
	{
	    printf("args: n_thr base_rf base_rt base_rs base_if base_it base_is height_f height_t height_s [n_steps=20] [mpc_prec=256] [num_base=10] [mpc_disp_prec=13] [disp_mpc=1] run_bitmask[1:ld,2:mpc,=1] [disp_warn=0] [divg_limit=2.0] [debug=0,1,2]\n");
	    printf("set n_steps to -1 to see all params and exit\n");
	}
	return 1;
    }

    lck();
    p = 256;
    b = 10;
    dp = 13;
    sig();

    n_thr = atoi(par[1]);
    Arf  = atof(par[2]);
    Art  = atof(par[3]);
    Ars  = atof(par[4]);
    Aif  = atof(par[5]);
    Ait  = atof(par[6]);
    Ais  = atof(par[7]);
    Bdf = atof(par[8]);
    Bdt = atof(par[9]);
    Bds = atof(par[10]);
    if (lb >= 12)  ns  = atoi(par[11]);
    if (lb >= 13)  p   = atoi(par[12]);
    if (lb >= 14)  b   = atoi(par[13]);
    if (lb >= 15) dp  = atoi(par[14]);
    if (lb >= 16) disp_mpc  = atoi(par[15]);
    if (lb >= 17) flag  = atoi(par[16]);
    if (lb >= 18) warn  = atoi(par[17]);
    if (lb >= 19) divg_limit  = atof(par[18]);
    if (lb >= 20) debug  = atoi(par[19]);

    Bdf += Bds * (long double)_c;
    Bds *= (long double)_n;

    if (ns < 0)
    {
	if (!_n)
	{
	    printf("args: n_thr base_rf base_rt base_rs base_if base_it base_is height_f height_t height_s [n_steps=20] [mpc_prec=256] [num_base=10] [mpc_disp_prec=13] [disp_mpc=1] run_bitmask[1:ld,2:mpc,=1] [disp_warn=0] [divg_limit=2.0] [debug=0,1,2]\n");
	    printf("set n_steps to -1 to see all params and exit\n");
	    printf("thr: n=%d, %d/%d, A=(%Lf,%Lf,inc=%Lf;%Lf,%Lf,inc=%Lf) x=[%Lf,%Lf,inc=%Lf] steps=%d, mpc_prec=%d, base_number=%d, mpc_disp_prec=%d, ", n_thr, _c, _n, Arf, Art, Ars, Aif, Ait, Ais, Bdf, Bdt, Bds, ns, p, b, dp); 
	    printf("disp_mpc: %d, run_flag: %d, disp warn: %d, divergence abs limit: %lf, debug: %d\n", disp_mpc, flag, warn, divg_limit, debug);
	}
	return 0;
    }

    lck();
    if (!bi_computed)
    {
	init_binomials(ns+1);
	bi_computed=true;
    }
    sig();
    int cidx = 0;

    // 1: start
    if (flag & 1) 
    {
	int xi = 0, ri = 0, ii = 0;
	for (long double r=Arf;r<=Art;r+=Ars)
	{
	    ii = 0;
	    for (long double i=Aif;i<=Ait;i+=Ais)
	    {
		Ac = std::complex<long double>(r, i);
		xi = _c;
		for (long double x=Bdf;x<=Bdt;x+=Bds)
		{
		    Cc = tetra_newton(Ac, x, ns);
		    printf("%d;%d;%d;%.15Lf;%.15Lf;%.15Lf;%.15Lf;%.15Lf;\n", ri, ii, xi, Ac.real(), Ac.imag(), x, Cc.real(), Cc.imag());
		    //fprintf(stderr, "%Lf %Lf %Lf\n", x, Cc.real(), Cc.imag());
		    //fprintf(stderr, "%Lf %Lf\n", Cc.real(), Cc.imag());
		    xi += _n;
		}
		ii ++;
	    }
	    ri ++;
	}
	cidx = ri;
    // 1: end
    }

    if (flag & 2) 
    {
	// 2: start
	mpc_init2(A, p);
	mpfr_init2(bx, p);
	mpfr_init2(AAr, p);
	mpfr_init2(AAi, p);
	mpfr_init2(Cr, p);
	mpfr_init2(Ci, p);
	mpc_init2(C, p);

	int xi = 0, ri = cidx, ii = 0;
	for (long double r=Arf;r<=Art;r+=Ars)
	{
	    ii = 0;
	    for (long double i=Aif;i<=Ait;i+=Ais)
	    {
		Ac = std::complex<long double>(r, i);
		//std::cout << Ac << ", " << ns << std::endl;
		xi = 0;
		for (long double x=Bdf;x<=Bdt;x+=Bds)
		{
	
		    mpc_set_ld_ld(A, r, i, MPC_RNDNN);
		    mpfr_set_ld(bx, x, MPFR_RNDN);
		    mpfr_set_ld(AAr, r, MPFR_RNDN);
		    mpfr_set_ld(AAi, i, MPFR_RNDN);

		    tetra_newton(C, A, bx, ns);

		    mpc_real(Cr, C, MPFR_RNDN);
		    mpc_imag(Ci, C, MPFR_RNDN);
	
		    if (disp_mpc) std::cout << ri << ";" << ii << ";" << xi << ";" << AAr << ";" << AAi << ";" << bx << ";" << Cr << ";" << Ci << ";" << std::endl;
		    else printf("%d;%d;%d;%.15Lf;%.15Lf;%.15Lf;%.15Lf;%.15Lf;\n", ri, ii, xi, ld_real(A), ld_imag(A), ld(bx), ld_real(C), ld_imag(C));
		    //printf("%d;%d;%d;%.15Lf;%.15Lf;%.15Lf;%.15Lf;%.15Lf;\n", ri, ii, xi, Ac.real(), Ac.imag(), x, Cc.real(), Cc.imag());
		    xi ++;
		}
		ii ++;
	    }
	    ri ++;
	}

	mpc_clear(A);
	mpfr_clear(bx);
	mpfr_clear(AAr);
	mpfr_clear(AAi);
	mpfr_clear(Cr);
	mpfr_clear(Ci);
	mpc_clear(C);
	// 2: end
    }
}

void* pth_work(void* ptr)
{
    thr_data* td = (thr_data*)ptr;
    go_thread(td->lb, td->par, td->c, td->n);
    return NULL;
}

int main(int lb, char** par)
{
    int n_thr;

    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_destroy(&mtx);

    if (lb < 2)
    {
	printf("%s: first arg must be threads count\n", par[0]);
	return 1;
    }

    n_thr = atoi(par[1]);

    pthread_t* th;
    th = new pthread_t[n_thr];
    thr_data* dptr;

    pthread_mutex_init(&mtx, NULL);

    for (int i=0;i<n_thr;i++)
    {
	dptr = new thr_data;
	dptr->c 	= i;
	dptr->n 	= n_thr;
	dptr->lb 	= lb;
	dptr->par 	= par;
	pthread_create(&th[i], NULL, pth_work, dptr);
    }

    for (int i=0;i<n_thr;i++)
    {
	pthread_join(th[i], NULL);
    }
    
    /*
    mpc_init2(A, p);
    mpfr_init2(B, p);
    mpc_init2(C, p);

    mpc_set_ld_ld(A, Ar, Ai, MPC_RNDNN);
    mpfr_set_ld(B, Bd, MPFR_RNDN);

    init_binomials(ns+1);
    //tetra_newton(C, A, B, ns);
    Cc = tetra_newton(Ac, Bd, ns);

    std::cout << A << std::endl;
    std::cout << B << std::endl;
    //std::cout << C << std::endl;
    std::cout << Cc << std::endl;

    mpc_clear(A);
    mpfr_clear(B);
    mpc_clear(C);
    */

    return 0;
}
