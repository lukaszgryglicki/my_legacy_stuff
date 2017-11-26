/*
void put_mpc(mpc_t v, int base, int prec)
{
    char* s = mpc_get_str(base, prec, v, MPC_RNDNN);
    printf("%s", s);
    mpc_free_str(s);
}

void put_mpfr(mpfr_t v, int base, int prec)
{
    mpfr_out_str(stdout, base, prec, v, GMP_RNDD);
}

int mpctet(mpc_t r, mpc_t a, mpfr_t b)
{
    int p0, p1;
    mp_exp_t e;
    //mpc_pow_fr(r, a, b, MPC_RNDNN);
    p0 = mpfr_cmp_si(b, 0);
    p1 = mpfr_cmp_si(b, -1);

    if (p1 == 1 && p0 <= 0)
    {
	// 1 + (2log(a)/(1+log(a)))x - (1-log(a))/(1+log(a))*x^2
	mpc_t loga, p1, p2;
	mpc_init2(loga, prec);
	mpc_init2(p1, prec);
	mpc_init2(p2, prec);
	mpc_log(loga, a, MPC_RNDNN);		// loga
	printf("loga:   "); put_mpc(loga, base, dprec); printf("\n");
	mpc_add(p1, loga, loga, MPC_RNDNN);	// 2loga
	printf("2loga:  "); put_mpc(p1, base, dprec); printf("\n");
	mpc_set_si(p2, 1, MPC_RNDNN);		// 1
	mpc_add(p2, p2, loga, MPC_RNDNN);	// 1+loga
	printf("1+loga: "); put_mpc(p2, base, dprec); printf("\n");
	mpc_div(p1, p1, p2, MPC_RNDNN);		// 2loga/1+loga
	printf("div:    "); put_mpc(p1, base, dprec); printf("\n");
	mpc_mul_fr(p1, p1, b, MPC_RNDNN);	// 2loga/(1+loga)x
	printf("divx:    "); put_mpc(p1, base, dprec); printf("\n");
	mpc_add_si(r, p1, 1, MPC_RNDNN);	// 1+(2loga/(1+loga)x)
	printf("divx+1:  "); put_mpc(r, base, dprec); printf("\n");
	mpc_set_si(p1, 1, MPC_RNDNN);		// 1
	mpc_sub(p1, p1, loga, MPC_RNDNN);	// 1-loga
	printf("1-loga:  "); put_mpc(p1, base, dprec); printf("\n");
	mpc_add_si(p2, loga, 1, MPC_RNDNN);	// loga+1
	printf("1+loga:  "); put_mpc(p2, base, dprec); printf("\n");
	mpc_div(p1, p1, p2, MPC_RNDNN);		// 1-loga/1+loga
	printf("div:     "); put_mpc(p1, base, dprec); printf("\n");
	mpc_mul_fr(p1, p1, b, MPC_RNDNN);	// *x
	printf("divx:    "); put_mpc(p1, base, dprec); printf("\n");
	mpc_mul_fr(p1, p1, b, MPC_RNDNN);	// *x^2
	printf("divxx:   "); put_mpc(p1, base, dprec); printf("\n");
	mpc_sub(r, r, p1, MPC_RNDNN);		// full approximation
	//mpc_set(r, a, MPC_RNDNN);
	printf("r:       "); put_mpc(r, base, dprec); printf("\n");
	mpc_clear(loga);
	mpc_clear(p1);
	mpc_clear(p2);
        printf("returning: interpol:\t");
    } 
    else if (p1 <= 0)
    {
	// log a (x+1, a)
	// loga(x) = log(x) / log(a)
	mpc_t r2;
	mpfr_t b2;
        mpc_init2(r2, prec);
        mpfr_init2(b2, prec);
	mpfr_set(b2, b, MPFR_RNDN);
	mpfr_add_si(b2, b2, 1, MPFR_RNDN);
	mpctet(r, a, b2);
	mpc_log(r, r, MPC_RNDNN);
	mpc_log(r2, a, MPC_RNDNN);
	mpc_div(r, r, r2, MPC_RNDNN);
	mpfr_clear(b2);
        mpc_clear(r2);
        //printf("AAA %s = %s ^^ %s\n", rs, as, bs);
        printf("returning:log(A, A^^(X+1))\t");
    } 
    else if (p0 == 1)
    {
	// a ^^ (x-1)
	mpfr_t b2;
	mpfr_init2(b2, prec);
	mpfr_set(b2, b, MPFR_RNDN);
	mpfr_sub_si(b2, b2, 1, MPFR_RNDN);
	mpctet(r, a, b2);
	mpc_pow(r, a, r, MPC_RNDNN);
	mpfr_clear(b2);
        printf("returning: A^^(X-1):\t");
    }

    put_mpc(a, base, dprec);
    printf(" ^^ ");
    put_mpfr(b, base, dprec);
    printf(" = ");
    put_mpc(r, base, dprec);
    printf("\n");
}
*/
    /*
    MPC r = a.tetra(b);
    std::cout << a.real().ld() << " ^^ " << b.ld() << " = " << r.real() << std::endl;
    */
    /*
    mpc_init2(a, prec);
    mpc_init2(r, prec);
    mpfr_init2(b, prec);

    printf("base: %d, prec: %d, dprec: %d\n", base, prec, dprec);

    mpc_set_str(a, par[4], base, MPC_RNDNN);
    mpfr_set_str(b, par[5], base, MPFR_RNDN);
    mpctet(r, a, b);

    put_mpc(a, base, dprec);
    printf(" ^^ ");
    put_mpfr(b, base, dprec);
    printf(" = ");
    put_mpc(r, base, dprec);
    printf("\n");

    mpc_clear(a);
    mpc_clear(r);
    mpfr_clear(b);
    */

