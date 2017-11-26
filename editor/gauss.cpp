
#include "stdafx.h"

#define M_PI 3.1415926535
#define TWOPI (2*M_PI)

Uni_save uni_data;

double uni()
{
	register double luni;
	luni = uni_data.u[uni_data.ui] - uni_data.u[uni_data.uj];
	if (luni < 0.0) luni += 1.0;
	uni_data.u[uni_data.ui] = luni;
	if (--uni_data.ui == 0) uni_data.ui = 97;
	if (--uni_data.uj == 0) uni_data.uj = 97;
	if ((uni_data.c -= uni_data.cd) < 0.0) uni_data.c += uni_data.cm;
	if ((luni -= uni_data.c) < 0.0) luni += 1.0;
	return ((double) luni);
}


void rstart(int i,int j,int k,int l)
{
	int ii, jj, m;
	double s, t;
	for (ii = 1; ii <= 97; ii++) 
	{
		s = 0.0;
		t = 0.5;
		for (jj = 1; jj <= 24; jj++) 
		{
			m = ((i*j % 179) * k) % 179;
			i = j;
			j = k;
			k = m;
			l = (53*l+1) % 169;
			if (l*m % 64 >= 32)
				s += t;
			t *= 0.5;
		}
		uni_data.u[ii] = s;
	}
	uni_data.c  = 362436.0   / 16777216.0;
	uni_data.cd = 7654321.0  / 16777216.0;
	uni_data.cm = 16777213.0 / 16777216.0;
	uni_data.ui = 97;
	uni_data.uj = 33; 
}

void seed_uni(int ijkl)
{
 int i, j, k, l, ij, kl;
 ijkl %= 900000000;
 if (ijkl == 0)
   {
    ijkl = time((time_t *)0);
    ijkl %= 900000000;
   }
 if ((ijkl < 0) || (ijkl > 900000000))
   {
    printf("seed_uni: ijkl = %d -- out of range\n\n", ijkl);
    exit(3);
   }
	ij = ijkl/30082;
	kl = ijkl - (30082 * ij);
	i = ((ij/177) % 177) + 2;
	j = (ij % 177) + 2;
	k = ((kl/169) % 178) + 1;
	l = kl % 169;
    rstart(i, j, k, l);
}

double gaussian()
{
	register double ran1, ran2;
	do 
	  { 
	   ran1 = (double)uni();
	  } 
	while (ran1 == 0.);
	ran2 = (double)uni();
	return (double)( sqrt(-2.*log(ran1))*cos(TWOPI*ran2));
}