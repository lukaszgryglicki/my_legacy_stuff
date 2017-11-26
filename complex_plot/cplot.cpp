#include <stdio.h>
#include "libfparser.h"

using namespace std;
 
const double PI = 3.1415926535897932384626433832795;
const double E  = 2.7182818284590452353602874713527;
 
void SetHSV( double h, double s, double v, unsigned char color[3] ) 
{
        double r,g,b;
        if(s==0)
                r = g = b = v;
        else {
                if(h==1) h = 0;
                double z = floor(h*6); int i = int(z);
                double f = double(h*6 - z);
                double p = v*(1-s);
                double q = v*(1-s*f);
                double t = v*(1-s*(1-f));
                switch(i){
                        case 0: r=v; g=t; b=p; break;
                        case 1: r=q; g=v; b=p; break;
                        case 2: r=p; g=v; b=t; break;
                        case 3: r=p; g=q; b=v; break;
                        case 4: r=t; g=p; b=v; break;
                        case 5: r=v; g=p; b=q; break;
                }
        }
        int c;
        c = int(256*r); if(c>255) c = 255; color[0] = c;
        c = int(256*g); if(c>255) c = 255; color[1] = c;
        c = int(256*b); if(c>255) c = 255; color[2] = c;
}
 
complex<double> fun( complex<double>& c )
{
 //c = complex<double>(c.real(), 0.);
 //return pow(c, pow(c, pow(c, pow(c, pow( c, c )))));
 //return pow(c, pow(c, pow( c, c )));	//most interesting :)
 //return pow(pow(pow(pow(c, c), c), c), c); //pure chaos
 //return pow(c, c);
 //return pow(pow(pow(pow(pow(pow(pow(c, c), c), c), c), c), c), c);
 return fpar_f( c );
}
 
int main(int lb, char** par)
{
		printf("params: xdim ydim sr er si ei \"funcdef\"\n");

		if (lb < 8) return 1;

		fpar_function(par[7]);

		if (!fpar_ok())
		{
			printf("\"%s\" - function definition parse error\n", par[7]);
		}
        const int dimx = atoi(par[1]); const int dimy = atoi(par[2]);
        const double rmi = atof(par[3]); const double rma =  atof(par[4]);
        const double imi = atof(par[5]); const double ima =  atof(par[6]);
 
        FILE * fp = fopen("complex.ppm","wb");
        fprintf(fp,"P6\n%d %d\n255\n",dimx,dimy);

		printf("Writing image: (%dx%d)\n", dimx, dimy);
		printf("ranges: Re[%f %f], Im[%f %f]\n", rmi, rma, imi, ima);
 
        int i,j;
        for(j=0;j<dimy;++j)
		{
                double im = ima - (ima-imi)*j/(dimy-1);
                for(i=0;i<dimx;++i)
				{               
                        double re = rma - (rma-rmi)*i/(dimx-1);
                        complex<double> c(re,im);
                        complex<double> v = fun(c);   
                        double a = arg( v );
                        while(a<0) a += 2*PI; 
						a /= 2*PI;
                        double m = abs( v );
                        double ranges = 0;
                        double rangee = 1;
                        while(m>rangee)
						{
                                ranges = rangee;
                                rangee *= E;
                        }
                        double k = (m-ranges)/(rangee-ranges);
                        double sat = k<0.5 ? k*2: 1 - (k-0.5)*2;
                        sat = 1 - pow( (1-sat), 3); sat = 0.4 + sat*0.6;
                        double val = k<0.5 ? k*2: 1 - (k-0.5)*2; val = 1 - val;
                        val = 1 - pow( (1-val), 3); val = 0.6 + val*0.4;
                        static unsigned char color[3];
                        SetHSV(a,sat,val,color);
                        fwrite(color,1,3,fp);
                }
        }
        fclose(fp);
		printf("All done.\n");
        return 0;
}
