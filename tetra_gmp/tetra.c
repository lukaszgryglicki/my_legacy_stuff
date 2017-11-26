#include <stdlib.h>
#include <stdio.h>
/*#include <gmp.h>*/
#include <mpfr.h>

void tetra_gmp(mpfr_t res, double base, int tetra)
{
	int i;
	mpfr_t tet;
	mpfr_init2(tet, 4096);
	mpfr_set_d(res, base, GMP_RNDN);
	mpfr_set(tet, res, GMP_RNDN);
	if (tetra == 0) mpfr_set_d(res, 1.0, GMP_RNDN);
	for (i=1;i<tetra;i++)
	{
		mpfr_pow(res, tet, res, GMP_RNDN);
	}
	mpfr_clear(tet);
}

int main(int lb, char** par)
{
	mpfr_t res;
	if (lb < 3)
	{
		printf("not enough arguments: required: base tetra\n");
		return 1;
	}
	mpfr_init2(res, 4096);
	tetra_gmp(res, atof(par[1]), atoi(par[2]));
	mpfr_out_str(stdout, 10, 0, res, GMP_RNDN);
	printf("\n");
	mpfr_clear(res);
	return 0;
}
