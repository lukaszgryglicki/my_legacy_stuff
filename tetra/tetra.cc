#include <complex>
#include <iostream>
using namespace std;

#define C_t complex<long double>
#define N_t long
#define R_t long double

C_t tetra(C_t base, N_t n)
{
 C_t result(base);
 for (N_t i=1;i<n;i++)
     result = pow(base, result);
 return result;
}

C_t infinite_tetra(C_t base, N_t& nsteps)
{
 C_t result(base), prevresult(base);
 N_t steptype, maxsteps;
 bool loop = true;
 maxsteps = nsteps;
 nsteps = 0;
 while (loop)
 {
     prevresult = result;
     result = pow(base, result);
     steptype = fpclassify( abs( result - prevresult));
     nsteps ++;
     if (steptype == FP_INFINITE || steptype == FP_NAN || steptype == FP_ZERO || nsteps == maxsteps)
	 loop = false;
 }
 return result;
}

int main(int lb, char** par)
{
    if (lb < 4) cout << "parameters: baseR, baseI, n" << endl;
    else
    {
	N_t n, nsteps;
	R_t baseR, baseI;
	sscanf(par[1], "%Lf", &baseR);
	sscanf(par[2], "%Lf", &baseI);
	sscanf(par[3], "%ld", &n);
	C_t base(baseR, baseI);
	C_t result, infresult;

	result = tetra(base, n);
	nsteps = 1000000;
	infresult = infinite_tetra(base, nsteps);

	cout << base << " ^^ " << n << " = " << result << endl;
	cout << " Abs = " << abs( result ) << endl;
	cout << "Infinite tetra after " << nsteps << " steps: " << endl;
	cout << infresult << ", Abs = " << abs( infresult ) << endl;
    }
    return 0;
}
