#include <gmp.h>

void gmp_tetra(double a, double b);

int main(int lb, char** par)
{
 printf("Calculates: A^^B, give a(float) & b(int) \n");
 double a;
 unsigned int b;
 if (lb < 3) return 1;
 sscanf(par[1], "%lf", &a);
 sscanf(par[2], "%d", &b);
 printf("Calculating %lf ^^ %d\n", a, b);
 gmp_tetra(a, b);
}

mpf_t tetra(mpf_t a, unsigned int x)
{	
 if (x == 0) return mpf_pow_ui(a, 0);	
 else return mpf_pow_ui(a, mpf_get_ui(tetra(a, x - 1)));
}


void gmp_tetra(double a, unsigned int b)
{
 mpf_t A, C;
 mpf_init2( A, 64 );
 mpf_init2( C, 5120 );
 mpf_set_d(A, a);
 gmp_printf("%F ^^ %d ...\n", A, b);
 C = tetra(A, b);
 gmp_printf("Wynik: %F\n", C);
}
