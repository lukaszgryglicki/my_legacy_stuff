#include <complex>

using namespace std;

long double tetra_interpol(long double a, long double x)
{
	if (x >= 0. && x < 1.) return pow(a, x);
	else return pow(a, tetra_interpol(a, x - 1.));
}

int main()
{
 for (long double i=0.;i<=3.5;i+=.01)
 {
	 printf("f[%lf] = \t%lf\n", i, tetra_interpol(2.,i));
 }
}