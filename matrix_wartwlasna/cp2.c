#include <iostream.h>
#include <math.h>

int n;
double X[10],Y[10],A[10][10];
double L[10][10],U[10][10];

void wypelnija()
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			cout<<"A<"<<i<<","<<j<<">=";
			cin>>A[i][j];
		}
	}
}

void wysm()
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++) cout<<A[i][j]<<"\t";
		cout<<endl;
	}
}

void wysw()
{
	int i;
	for(i=0;i<n;i++) cout<<X[i]<<"\t";
}

void wysl()
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++) cout<<L[i][j]<<"\t";
		cout<<endl;
	}
}

void wysu()
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++) cout<<U[i][j]<<"\t";
		cout<<endl;
	}
}

double szuk()
{
	int i,j;
	double r=0,x,o;
	for(i=1;i<n;i++) r+=A[0][i];
	x=A[0][0]-r;
	for(i=1;i<n;i++)
	{
		o=A[i][i];
		r=0;
		for(j=0;j<n;j++)
		{
			if(i!=j) r+=A[i][j];
		}
		if((o-r)<x) x=o-r;
	}
	return x;
}

void crout()
{
	int i,j,k,p;
	double sum;
	for(k=0;k<n;k++)
	{
		for(i=k;i<n;i++)
		{
			sum=0;
			for(p=0;p<k;p++) sum+=L[i][p]*U[p][k];
			L[i][k]=A[i][k]-sum;
		}
		for(j=k+1;j<n;j++)
		{
			sum=0;
			for(p=0;p<k;p++) sum+=L[k][p]*U[p][j];
			U[k][j]=(A[k][j]-sum)/L[k][k];
		}
	}
	for(i=0;i<n;i++)U[i][i]=1;
}

void oby()
{
	int i,j;
	double z[10],sum;
	
	for(i=0;i<n;i++)
	{
		sum=0;
		for(j=0;j<i;j++) sum+=z[j]*L[i][j];
		z[i]=(X[i]-sum)/L[i][i];
	}
	for(i=n-1;i>=0;i--)
	{
		sum=0;
		for(j=i+1;j<n;j++) sum+=U[i][j]*Y[j];
		Y[i]=(z[i]-sum)/U[i][i];
	}
}



int main()
{
	double l,l1,l2,il,nor;
	int i;
	cout<<"Podaj rozmiar macierzy A:";
	cin>>n;
	wypelnija();
	wysm();
	l=szuk()-1;
	cout<<"L="<<l<<endl;
	for(i=0;i<n;i++) A[i][i]-=l;
	for(i=0;i<n;i++)X[i]=1;
	wysm();
	l2=l;
	do
	{
		l1=l2;
		crout();
		oby();
		il=0;
		for(i=0;i<n;i++) il+=Y[i]*X[i];
		l2=(1/il)+l;
		il=0;
		for(i=0;i<n;i++) il+=Y[i]*Y[i];
		nor=sqrt(il);
		for(i=0;i<n;i++) X[i]=Y[i]/nor;
	}while(fabs(l1-l2)>0.000000001);
	cout<<"wartosc wlasna: "<<l2<<endl;

}
