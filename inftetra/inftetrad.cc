#include <complex>
#include <iostream>
using namespace std;

#define C complex<long double>
#define R long double
#define N long

R epsilon_min;
R epsilon_max;
R conv_factor;
R inf_factor;

typedef struct _ppmcolor
{
 unsigned char rgb[3];
 C val;
 N state;
} ppmcolor;

C inftetrad(C a, long& nsteps, long& state)
{
	C py;
	R d;
	bool loop = true;
	C y;
	y = a;
	py = y;
	N i = 1;
	//printf(" py = %Lf +i%Lf\n", py.real(), py.imag());
	while (loop)
	{
		i ++;
		py = y;
		//printf("(%d,%d) --> (%Lf, %Lf, %Lf)\n", i, nsteps, py.real(), y.real(), d);
		y = pow(a, y);
		//printf(" y  = %Lf +i%Lf [%4d]\n", y.real(), y.imag(), i);
		//printf(" py = %Lf +i%Lf [%4d]\n", py.real(), py.imag(), i);
		d = abs( y - py );
		//printf("(%d,%d) --> (%Lf, %Lf, %Lf)\n", i, nsteps, py.real(), y.real(), d);
		//printf("d = %Lf [%4d]\n", d, i);
		if (i == nsteps)
		{
			//printf("steps\n");
			loop = false;
			state = -2;			/* Convergance not probed - not enough steps */
			nsteps = i;
			return y;
		}
		if (isnan( d ))
		{
			//printf("nan\n");
			loop = false;
			state = -1;
			nsteps = i;
			return y;
		}
		if (d < epsilon_min)
		{
			//printf("convergance\n");
			loop = false;
			state = 0;			/* convergance */
			nsteps = i;
			return y;
		}
		if (d > epsilon_max)
		{
			//printf("infinity\n");
			loop = false;
			state = 1;			/* tends to infionity */
			nsteps = i;
			return y;
		}
	}
	return y;
}

R s0maxr, s0minr;
R s0maxi, s0mini;

void setMinMax(C y, N s)
{
 R sr, si;
 if (s == 0)
 {
	 sr = y.real();
	 si = y.imag();
	 if (s0maxr < sr) s0maxr = sr;
	 if (s0minr > sr) s0minr = sr;
	 if (s0maxi < si) s0maxi = si;
	 if (s0mini > si) s0mini = si;
 }
}

void setColor(FILE* fp, ppmcolor& ref)
{
 unsigned char sv1, sv2;
 R mul1, diff1, val1;
 R mul2, diff2, val2;
 R factor;
 
 if (ref.state == 0 || ref.state == -2)
 {
     	 if (ref.state == 0) factor = conv_factor;
	 else factor = inf_factor;

	 diff1 = s0maxr - s0minr;
	 if (fabs(diff1) < epsilon_min) mul1 = factor;
	 else mul1 = factor * (255.0 / diff1);
	 val1 = (ref.val.real() - s0minr) * mul1;
	 sv1 = (unsigned char)val1;

	 diff2 = s0maxi - s0mini;
	 if (fabs(diff2) < epsilon_min) mul2 = factor;
	 else mul2 = factor * (255.0 / diff2);
	 val2 = (ref.val.imag() - s0mini) * mul2;
	 sv2 = (unsigned char)val2;

 }

 if (ref.state == 0)		/* convergance */
 { 
	 ref.rgb[0] = sv1;    
	 ref.rgb[1] = sv2;    
	 ref.rgb[2] = 0;

//	 ref.rgb[0] = 0xff;    
//	 ref.rgb[1] = 0;    
//	 ref.rgb[2] = 0;
 }
 else if (ref.state == 1)	/* infinity */
 { 
	 ref.rgb[0] = 0xff;    
	 ref.rgb[1] = 0xff;    
	 ref.rgb[2] = 0xff; 
 }
 else if (ref.state == -2) 	// uncomputed
 { 
	 ref.rgb[0] = 0; 
	 ref.rgb[1] = sv1; 
	 ref.rgb[2] = sv2; 

//	 ref.rgb[0] = 0;    
//	 ref.rgb[1] = 0;    
//	 ref.rgb[2] = 0xff;
 }
 else if (ref.state == -1)	/* nan */
 { 
	 ref.rgb[0] = 0;    
	 ref.rgb[1] = 0;    
	 ref.rgb[2] = 0;    
 }
 else				// WTF? 
 {
	 ref.rgb[0] = 0;    
	 ref.rgb[1] = 0xff;    
	 ref.rgb[2] = 0;    
 }
 fwrite(ref.rgb,1,3,fp);
}

int main(int lb, char** par)
{
	if (lb < 12)
		{
		printf("%s sr er pr si ei pi n logmin logmax cfactor ifactor\n", par[0]);
		printf("example: %s -4.5 4.5 1025 -4.5 4.5 768 100 -10 20 2 8\n", par[0]);
		return 1;
		}
	R sr, er, si, ei, cr, ci, lr, li, logmin, logmax;
	N n, s, pr, pi, _n;
	C base, y;
	unsigned char red[3];

	red[0] = 0xff;
	red[1] = red[2] = 0;

	sscanf(par[1], "%Lf", &sr);
	sscanf(par[2], "%Lf", &er);
	sscanf(par[3], "%ld", &pr);
	sscanf(par[4], "%Lf", &si);
	sscanf(par[5], "%Lf", &ei);
	sscanf(par[6], "%ld", &pi);
	sscanf(par[7], "%ld", &n);
	sscanf(par[8], "%Lf", &logmin);
	sscanf(par[9], "%Lf", &logmax);
	sscanf(par[8], "%Lf", &conv_factor);
	sscanf(par[9], "%Lf", &inf_factor);

	epsilon_min = pow((R)(10.), logmin);
	epsilon_max = pow((R)(10.), logmax);

	printf("Divergence Range: %.13Lf - %Lf\n", epsilon_min, epsilon_max);

	lr = er - sr;
	li = ei - si;

	s0maxr = -1e13;
	s0minr =  1e13;
	s0maxi = -1e13;
	s0mini =  1e13;

	ppmcolor **ppcolor;

	ppcolor = (ppmcolor**)malloc((pr + 1) * sizeof(ppmcolor*));
	for (N i=0;i<=pr;i++) ppcolor[i] = (ppmcolor*)malloc((pi + 1) * sizeof(ppmcolor));
	 
	for (N j=0;j<=pi;j++)
	{
		if (!(j % 100)) printf("%ld/%ld\n", j, pi);
		for (N i=0;i<=pr;i++)	
		{
			cr = sr + ((R)i * lr) / (R)pr;
			ci = si + ((R)j * li) / (R)pi;
			_n = n;
			base = C(cr, ci);
			y = inftetrad( base, _n, s);
			//cout << base << " -> " << y << ", " << s << endl;
			/*else if (s == -1) { color[0] = 0xff; color[1] = 0;    color[2] = 0;    }*/

			ppcolor[i][j].val   = y;
			ppcolor[i][j].state = s;

			setMinMax( y, s );
		}
	}

	printf("s0 rangeR[%Lf - %Lf]\n", s0minr, s0maxr);
	printf("s0 rangeI[%Lf - %Lf]\n", s0mini, s0maxi);

	FILE * fp = fopen("complex.ppm","wb");
	fprintf(fp,"P6\n%d %d\n255\n",pr + 1,pi + 1);

	printf("outputing ppm. . .");
	fflush(stdout);
	for (N j=0;j<=pi;j++)
	{
/* 	 	if (j == hi) fwrite(red,1,3,fp);*/
		for (N i=0;i<=pr;i++)	
		{
/* 	 		if (i == hr) fwrite(red,1,3,fp);*/
			setColor(fp, ppcolor[i][j]);
		}
	}
	printf("\n");

	fclose(fp);
	return 0;
}
