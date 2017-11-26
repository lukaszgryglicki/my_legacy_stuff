#include "defs.hxx"
#include "bern_base.hxx"

real bern_basis::get_value(int i, real t)
{
	real res = 1, 
		 oneT = 1-t, 
		 n = 1, 
		 f_i = 0, 
		 f_n_i = 0;
	res = (real)(pow(t, i)*pow(oneT, bern_degree-i));
	//printf("getv(%d,%f): degree=%d\n", i, t, bern_degree);
	for(register int j = 1; j <= bern_degree; ++j)
	{
		if (i == j) f_i = n;
		if ((n-i) == j) f_n_i = n;
		n = (!i) ? 1 : n*i;
	}
	if (!i)
	{
		f_i = 1; f_n_i = n;
	}
	if (f_i * f_n_i == 0.) return 0.;
	return (res*n)/(f_i*f_n_i);
}

real bern_basis::get_value3(int i, real t)	/* dla 3go stopnia, uproszczone obliczenia */
{
 real mt = 1.-t;
 switch (i)
 {
  case 0: return t*t*t; break;
  case 1: return 3.*t*t*mt; break;
  case 2: return 3.*t*mt*mt; break;
  case 3: return mt*mt*mt; break;
  default: return 0.;
 }
	
}

void bern_basis::calc_basis(void)
{
	real step = (bern_max_t-bern_min_t)/(real)(bern_steps-1), currStep = bern_min_t, one_currStep = 1-bern_min_t;
	int* processing = new int[bern_degree+1];
	real* preprocessing = new real[bern_degree+1];
	real* tmp = new real[bern_degree+1];
	// i!
	for(register int i = 0; i <= bern_degree; ++i)
		processing[i] = (!i) ? 1 : processing[i-1]*i;
	// n!/i!(n-i)!
	for(register int i = 0; i <= bern_degree; ++i)
		preprocessing[i] = (real)processing[bern_degree]/(real)(processing[i]*processing[bern_degree-i]);
	// glowny algorytm
	register int i;
	for(i = 0; i < bern_steps-1; ++i)
	{
		bern_array_basis[i][0] = 1; tmp[0] = 1;
		for(register int j = 1; j <= bern_degree; ++j)
		{
			bern_array_basis[i][j] = bern_array_basis[i][j-1]*currStep;
			tmp[j] = tmp[j-1]*one_currStep;
		}
		for(register int j = 0; j <= bern_degree; ++j)
			bern_array_basis[i][j] *= preprocessing[j]*tmp[bern_degree-j];
		currStep += step;
		one_currStep = 1-currStep;
	}

	currStep = bern_max_t; one_currStep = 1-bern_max_t;
	bern_array_basis[i][0] = 1; tmp[0] = 1;
	for(register int j = 1; j <= bern_degree; ++j)
	{
		bern_array_basis[i][j] = bern_array_basis[i][j-1]*currStep;
		tmp[j] = tmp[j-1]*one_currStep;
	}
	for(register int j = 0; j <= bern_degree; ++j)
		bern_array_basis[i][j] *= preprocessing[j]*tmp[bern_degree-j];
	free_ptr(preprocessing);
	free_ptr(processing);
	free_ptr(tmp);
}


