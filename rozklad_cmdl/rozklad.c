#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>

int verbose = 0;

/* FIXME: dlaczego takie a nie inne wartosci ??? */
static unsigned add[] = {4, 2, 4, 2, 4, 6, 2, 6};

/* params: t - BIGNUM, limit (limit dzielenia) */
/* im wiecej tym dluzej moze szukac */
void rozklad_liczby(mpz_t t, unsigned int limit)
{
  mpz_t q, r;
  unsigned long int f;
  int ai;
  unsigned *addv = add;
  unsigned int failures;

  if (verbose)
    {
      printf("[dzielen (%u)] ", limit);
      fflush(stdout);
    }
  mpz_init(q);
  mpz_init(r);	/* q = r = 0 */

  f = mpz_scan1(t, 0);		/* zwraca numer pierwszego bitu rownego 1 */
  mpz_div_2exp(t, t, f);	/* t /= 2^f */
  while (f)			/* znajduje ilosc podzielnikow przez 2 */
    {
      printf ("2 ");	
      fflush (stdout);
      --f;
    }

  while (1)
    {
      mpz_tdiv_qr_ui(q, r, t, 3); /* q = t/3, r = t%3 */
      if (mpz_cmp_ui(r, 0) != 0)	/* if (r != 0) */
	break;
      mpz_set(t, q);		/* t = q */
      printf("3 ");
      fflush (stdout);
    }				/* znajduje podzialy przez 3 */

  while (1)
    {
      mpz_tdiv_qr_ui(q, r, t, 5);
      if (mpz_cmp_ui(r, 0) != 0)
	break;
      mpz_set (t, q);
      printf ("5 ");
      fflush (stdout);
    }		/* analogicznie z 5 */

  failures = 0;
  f = 7;		/* dla 7 itd z tablica add*/
  ai = 0;
  while (mpz_cmp_ui(t, 1) != 0)
    {
      mpz_tdiv_qr_ui(q, r, t, f);	/* q = t/f, r = t%f, f -kolejne pierwsze */
      if (mpz_cmp_ui(r, 0) != 0)
	{
	  f += addv[ai];
	  if (mpz_cmp_ui(q, f) < 0) break;
	  ai = (ai + 1) & 7;
	  failures++;
	  if (failures > limit) break;
	}
      else
	{
	  mpz_swap(t, q);	/* t <= q, q <= t zamiana */
	  printf("%lu ", f);
	  fflush(stdout);
	  failures = 0;
	}
    }

  mpz_clear(q);
  mpz_clear(r);
}
/* params: t - BIGNUM, limie (ilosci podzielen) */
/* p - dokladnosc (im wiecej tym slabsza) p > 0 */
/* p = 0 to rozklad pelny i inna funkcja */
void rozklad_czesciowy(mpz_t t, unsigned int limit, unsigned long p)
{
  mpz_t r;
  mpz_t f;
  unsigned int k;
  mpz_init(r);	/* r = 0 */
  mpz_init_set_ui(f, 2 * p);	/*f = 2p */
  mpz_add_ui(f, f, 1);		/* f = 2p+1 */
  for (k=1;k<limit;k++)
    {
      mpz_tdiv_r(r, t, f);	/* r = t % f gives reminder*/
      while (mpz_cmp_ui(r, 0) == 0)	/* while r == 0 */
	{
	  mpz_tdiv_q(t, t, f);		/* t /= f, gives quotient(iloraz) */
	  mpz_tdiv_r(r, t, f);		/* r = t % f, reminder(reszta) */
	  mpz_out_str(stdout, 10, f);	/* wypisz */
	  fflush(stdout);
	  fputc(' ', stdout);
	}
      mpz_add_ui(f, f, 2 * p);	/* f += 2p */
    }
  mpz_clear(f);
  mpz_clear(r);
}

/* params: n - BIGNUM (liczba zlozona), p - dokladnosc */
/* FIXME ai liczba pomocnicza przy testowaniu GCD() ?? */
/* algorytm: pollard-rho */
void rozklad_liczby_zlozonej(mpz_t n, int ai, unsigned long p)
{
  mpz_t x, x1, y, P;
  mpz_t a;
  mpz_t g;
  mpz_t t1, t2;
  int k, l, c, i;

  if (verbose)
    {
      printf("[zlozona (%d)] ", ai);
      fflush(stdout);
    }

  mpz_init(g);
  mpz_init(t1);
  mpz_init(t2);	/* g = t1 = t2 = 0 */

  mpz_init_set_si(a, ai);	/* a = ai */
  mpz_init_set_si(y, 2);	/* y = x = x1 = 2 */
  mpz_init_set_si(x, 2);
  mpz_init_set_si(x1, 2);
  k = 1;
  l = 1;
  mpz_init_set_ui(P, 1);	/* P = 1 */
  c = 0;

  while (mpz_cmp_ui(n, 1) != 0)	/* while (n != 1) */
    {
S2:
      if (p != 0)	/* niedokladna metoda */
	{
	  mpz_powm_ui(x, x, p, n);	/* x = (x^p) mod n */
	  mpz_add(x, x, a);		/* x += a */
	  		/* x = ((x^p) mod n) + a */
	}
      else
	{
	  mpz_mul(x, x, x); 
	  mpz_add(x, x, a); 
	  mpz_mod(x, x, n);	/* x = ((x^2) + a) mod n */
	}
      mpz_sub(t1, x1, x); 	/* t1 = x1 - x */
      mpz_mul(t2, P, t1); 	/* t2 = P - t1 */
      mpz_mod(P, t2, n);	/* P = t2 mod n */
      c++;
      if (c == 20)
	{
	  c = 0;
	  mpz_gcd(g, P, n);	/* g = najw_wsp_dzielnik(P, n) */
	  	/* ig g != 1 (nie sa wzglednie pierwsze)*/
	  if (mpz_cmp_ui(g, 1) != 0) goto S4;
	  mpz_set(y, x);	/* y = x */
	}
      k--;
      if (k > 0)
	goto S2;

      mpz_gcd(g, P, n);	/* g = GDC(P,n) najw_wspolny_dzielnik */
      if (mpz_cmp_ui(g, 1) != 0)	/* jak nie rowny 1 */
	goto S4;

      mpz_set(x1, x);	/* x1 = x */
      k = l;
      l = 2 * l;
      for (i = 0; i < k; i++)
	{
	  if (p != 0)
	    {
	      mpz_powm_ui(x, x, p, n); 
	      mpz_add(x, x, a); /* x = ((x^p) mod n) + a */
	    }
	  else
	    {
	      mpz_mul(x, x, x); 
	      mpz_add(x, x, a); 
	      mpz_mod(x, x, n); /* x = ((x^2)+a) mod n */
	    }
	}
      mpz_set(y, x);		/* y = x */
      c = 0;
      goto S2;			/* i jedziemy dalej z rozkladem */
S4:
      do
	{
	  if (p != 0)
	    {
	      mpz_powm_ui(y, y, p, n); 
	      mpz_add(y, y, a); 		/* j.w. */
	    }
	  else
	    {
	      mpz_mul(y, y, y); 
	      mpz_add(y, y, a); 
	      mpz_mod(y, y, n);			/* j.w. */
	    }
	  mpz_sub(t1, x1, y); 
	  mpz_gcd(g, t1, n);			/* j.w. */
	}
      while (mpz_cmp_ui(g, 1) == 0);		/* dopoki GCD(t1,n) = 1 */

      if (!mpz_probab_prime_p (g, 3))		/* jesli dalej zlozona */
	{
	  do
            {
              mp_limb_t a_limb;			/* tak jakby cyfra wewnatrz mpz_t */
              mpn_random(&a_limb, (mp_size_t) 1);	/* losowa cyfra */
              ai = (int)a_limb;
            }
	  while (ai == -2 || ai == 0);	/* dopuki limb = -2 lub 0 */

	  if (verbose)
	    {
	      printf("[ponowny rozklad potrzebny] ");
	      fflush(stdout);
	    }
	  			/* rozklad z inna liczba */
	  			/* FIXME: ma znaczenie pomocnicze tylko ?? */
	  rozklad_liczby_zlozonej(g, ai, p);
	  break;
	}
      else
	{
	  mpz_out_str(stdout, 10, g);	/* znaleziony rozklad */
	  fflush(stdout);
	  fputc(' ', stdout);
	}
      mpz_div(n, n, g);		/* n /= g */
      mpz_mod(x, x, n);		/* x %= n */
      mpz_mod(x1, x1, n);	/* x1 %= n */
      mpz_mod(y, y, n);		/* y %= n */
      if (mpz_probab_prime_p(n, 3))	/* jesli n pierwsza (ret=2) lub */
	{				/* prwpdobnie pierwsza (ret=1) */
	  mpz_out_str(stdout, 10, n);	/* wypisz */
	  fflush(stdout);
	  fputc(' ', stdout);
	  break;
	}
    }

  mpz_clear(g);				/* wyczysc pamiec */
  mpz_clear(P);
  mpz_clear(t2);
  mpz_clear(t1);
  mpz_clear(a);
  mpz_clear(x1);
  mpz_clear(x);
  mpz_clear(y);
}

/* params: t BIGNUM, p=0 to dokladny rozklad, p >0 coraz mniej dokladny */
void rozklad(mpz_t t, unsigned long p)
{
  unsigned int div_limit;

  if (mpz_sgn(t) == 0) return;		/* we dont want zero */

  div_limit = mpz_sizeinbase(t, 2);	/* div_limit = log(2,t) */
  					/* base is 2 */
  if (div_limit > 1000) div_limit = 1000 * 1000;
  else div_limit = div_limit * div_limit;
		/* gdy chcemy tylko kilka dzielnikow */
  		/* pelny ma ~= log(2,t)^2 div_limit */
  if (p != 0) rozklad_czesciowy(t, div_limit / 10, p);
  else rozklad_liczby(t, div_limit);

  if (mpz_cmp_ui(t, 1) != 0)	/* if (t != 0) */
    {
      if (verbose)
	{
	  printf("[jest pierwsza?] ");
	  fflush(stdout);
	}
      if (mpz_probab_prime_p(t, 3))	/* 3 proby czy pierwsza */
	  				/* jak zwroci 2 to napewno pierwsza */
	  				/* jak 1 to najprawdopodobniej pierwsza */
	  				/* 0 to 100% zlozona */
	mpz_out_str(stdout, 10, t);	/* wypisz BIGNUM 10-tkowo */
      else rozklad_liczby_zlozonej(t, 1, p);
    }
}

void usage()
{
 printf("usage rozklad [-tBIGNUM] [-pNUM] -[eNUM]\n");
 printf("-eNUM means, t = 2^NUM-1, p = NUM\n");
 printf("rozklad(t,p); t-BIGNUM, p-NUM\n");
 printf("IF you want to solve 2^N-1 with different p\n");
 printf("TRY: rozklad -eN -p1 (-p1 overrides p = N)\n");
}

int main(int lb, char **par)
{
  mpz_t t;		/* a BIGNUM */
  unsigned long p;
  int i;
  if (lb > 1 && !strcmp (par[1], "-v"))
    {
     verbose = 1;
     par++;
     lb--;
    }
  mpz_init(t);		/* initializuje t */
  if (lb <= 1) { usage(); return 1; }
  p = 0;
  mpz_set_ui(t, 1);	/* t = 1 */
  for (i=1;i<lb;i++)
    {
     if (!strncmp(par[i], "-e", 2))
       {				/* dla -e10 */
	p = atoi(par[i] + 2);		/* p = 10 */
	mpz_mul_2exp(t, t, p);		/* t = 2^10 = 1024 */
	mpz_sub_ui(t, t, 1);		/* t = 1023 */
       }
      else if (!strncmp(par[i], "-p", 2))
       {
	p = atoi(par[i] + 2);		/* ustawia p */
       }
      else if (!strncmp(par[i], "-t", 2))
       {
	mpz_set_str(t, par[i] +2 , 0);	/* ustawia t */
					/* base = 0 wiec okresl na podst */
					/* poczatku stringa, np 0x to 16 */
					/* a 0 to 8, np -t0xdead, -t0770 */
       }
     }
   if (mpz_cmp_ui(t, 0) == 0) { printf("BIGNUM is 0\n"); return 2; }
   printf("p = %ld\n", p);
   gmp_printf("t = %Zd\n", t);
   printf("*********\n");
   rozklad(t, p);	/* rozklad z p */
   printf("\nDone.\n");
  return 0;
}

