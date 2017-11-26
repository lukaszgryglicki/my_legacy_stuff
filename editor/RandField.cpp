#include "stdafx.h"
#include "Texture.h"

extern GLdouble angX, angY, angZ;
extern int gl_current_type;
extern FILE* plik;
extern int want_random;
extern double mi;
extern int nTex;
extern Texture* texture;
extern unsigned int* tids;


void RandField::RemoveField()
{
	if(field)
	{
		for(int i=0; i<xSize; i++)
		{
			delete[] field[i];
			delete[] texid[i];
			field[i] = NULL;
			texid[i] = NULL;
		}
		delete[] field;
		field = NULL;
		delete[] texid;
		texid = NULL;
	}
}

void RandField::ComputeNormalVector(double x1, double y1, double z1, 
									double x2, double y2, double z2,
									double x3, double y3, double z3,
									double &xN, double &yN, double &zN)
{
	double a = (x3-x1)*(y2-y1) - (x2-x1)*(y3-y1);
	xN = (y3-y1)*(z2-z1) - (y2-y1)*(z3-z1);
	yN = (x2-x1)*(z3-z1) - (x3-x1)*(z2-z1);
	zN = (x3-x1)*(y2-y1) - (x2-x1)*(y3-y1);
	double d = sqrt(xN*xN + yN*yN + zN*zN);
	xN /= d;
	yN /= d;
	zN /= d;
	if(zN<0)
	{
		xN = -xN;
		yN = -yN;
		zN = -zN;
	}
}

bool RandField::Cholesky(double **in, double **out, int n)
{
	/*if (plik) fprintf(plik, "in:\n");
	for (int i=0;i<n;i++)
	{
	 for (int j=0;j<n;j++) if (plik) fprintf(plik,"%f ", in[i][j]);
     if (plik) fprintf(plik,"\n");
	}
	if (plik) fprintf(plik, "out:\n");
	for (int i=0;i<n;i++)
	{
	 for (int j=0;j<n;j++) if (plik) fprintf(plik,"%f ", out[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
	for(int j=0; j<n; j++)
	{
		double sum = 0;
		for (int k=0; k<j; k++) 
		  {
			sum += out[j][k]*out[j][k];
			//if (plik) fprintf(plik, "out[%d][%d] = %f, sum = %f\n", j,k,out[j][k], sum);
		  }
		out[j][j] = sqrt(in[j][j]-sum);
		if(out[j][j]==0)
		{
			if(j==0)
				MessageBox(NULL, "Cholesky, inside", "Error", MB_OK);
			return false;
		}
		for(int i=j+1; i<n; i++)
		{
			double sum1 = 0;
			for(int k=0; k<j; k++)
				sum1 += out[i][k]*out[j][k];
			out[i][j] = (in[i][j]-sum1)/out[j][j];
		}
	}
	/*if (plik) fprintf(plik, "out:\n");
	for (int i=0;i<n;i++)
	{
	 for (int j=0;j<n;j++) if (plik) fprintf(plik,"%f ", out[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
	return true;
}

void RandField::EnlargeField()
{
	double **newField = new double*[2*xSize-1];
	int **newTexId = new int*[2*xSize-1];
	for(int x=0; x<2*xSize-1; x++)
	{
		newField[x] = new double[2*ySize-1];
		newTexId[x] = new int[2*ySize-1];
		for(int y=0; y<2*ySize-1; y++)
		{
			newField[x][y] = 0.0;
			newTexId[x][y] = 0;
		}
	}
	for(int x=0; x<xSize; x++)
		for(int y=0; y<ySize; y++)
		{
			newField[2*x][2*y] = field[x][y];
			newTexId[2*x][2*y] = texid[x][y];
		}
	RemoveField();
	field = newField;
	texid = newTexId;
	xSize = 2*xSize-1;
	ySize = 2*ySize-1;
}

double* RandField::GetX(int x0, int y0)
{
	if(!(x0>0 && x0<xSize && y0>0 && y0<ySize && x0%2==1 && y0%2==1))
		return NULL;
	int down = (y0>=neighbSize) ? neighbSize : y0;
	int left = (x0>=neighbSize) ? neighbSize : x0;
	int up = (y0+neighbSize<ySize) ? neighbSize : ySize-1-y0;
	int right = (x0+neighbSize<xSize) ? neighbSize : xSize-1-x0;
	int num = (left+right+1)*down + 2*left + (right+1)/2 + ((left+1)/2+(right+1)/2)*(up/2);
	double *vectX = new double[num];
	int k = 0;
	for(int y=y0-down; y<y0; y++)
		for(int x=x0-left; x<=x0+right; x++)
		{
			vectX[k] = field[x][y];
			k++;
		}
	for(int y=y0; y<=y0+1; y++)
		for(int x=x0-left; x<x0; x++)
		{
			vectX[k] = field[x][y];
			k++;
		}
	for(int x=x0+1; x<=x0+right; x+=2)
	{
		vectX[k] = field[x][y0+1];
		k++;
	}
	for(int y=y0+3; y<=y0+up; y++)
		for(int x=x0-left; x<=x0+right; x+=2)
		{
			vectX[k] = field[x][y];
			k++;
		}
	//if (plik) fprintf(plik, "k=%d/%d\n", k, num);
	return vectX;
}

double* RandField::ComputeY(double **B, double **S, double *vectX, double*vectZ, int m, int n)
{
	double *vectY = new double[m];
	for(int i=0; i<m; i++)
	{
		vectY[i] = 0.0;
		for(int k=0; k<n; k++)
			vectY[i] += B[i][k]*vectX[k];
		for(int k=0; k<m; k++)
			vectY[i] += S[i][k]*vectZ[k];
	}
	return vectY;
}

double* RandField::GenerateZ(int n)
{
	double *vectZ = new double[n];
	for(int i=0; i<n; i++)
	{
		vectZ[i] = want_random ? mi*gaussian() : 0.;
	}
	return vectZ;
}

int RandField::ComputeCxx(double ***pC, double dim, int x0, int y0)
{
	if(!(x0>0 && x0<xSize && y0>0 && y0<ySize && x0%2==1 && y0%2==1))
		return -1;
	int down = (y0>=neighbSize) ? neighbSize : y0;
	int left = (x0>=neighbSize) ? neighbSize : x0;
	int up = (y0+neighbSize<ySize) ? neighbSize : ySize-1-y0;
	int right = (x0+neighbSize<xSize) ? neighbSize : xSize-1-x0;
	int num = (left+right+1)*down + 2*left + (right+1)/2 + ((left+1)/2+(right+1)/2)*(up/2);
	//if (plik) fprintf(plik, "ccxx: l/r/u/d/n: %d/%d/%d/%d/%d\n",left,right,up,down,num);
	*pC = new double*[num-1];
	for(int i=0; i<num-1; i++)
		(*pC)[i] = new double[num-1];
	double *tmpX = new double[num];
	double *tmpY = new double[num];
	int k = 0;
	for (int i=0;i<num;i++) tmpX[i] = tmpY[i] = 0.;
	for(int y=y0-down; y<y0; y++)
		for(int x=x0-left; x<=x0+right; x++)
		{
			tmpX[k] = x;
			tmpY[k] = y;
			k++;
			//if (plik) fprintf(plik, "%d,%d,%d\n",x,y,k);
		}
	for(int y=y0; y<=y0+1; y++)
		for(int x=x0-left; x<x0; x++)
		{
			tmpX[k] = x;
			tmpY[k] = y;
			k++;
		}
	for(int x=x0+1; x<=x0+right; x+=2)
	{
		tmpX[k] = x;
		tmpY[k] = y0+1;
		k++;
	}
	for(int y=y0+3; y<=y0+up; y++)	//FIXME \/ tu bylo < a powinno <= ?
		for(int x=x0-left; x<=x0+right; x+=2)
		{
			tmpX[k] = x;
			tmpY[k] = y;
			k++;
		}
	//if (plik) fprintf(plik, "tmpX={");
	//for (int i=0;i<num;i++) if (plik) fprintf(plik,"%f,",tmpX[i]);
	//if (plik) fprintf(plik,"}\n");
	//if (plik) fprintf(plik, "tmpY={");
	//for (int i=0;i<num;i++) if (plik) fprintf(plik,"%f,",tmpY[i]);
	//if (plik) fprintf(plik,"} k=%d/%d\n",k,num);
	double h = 2.0*(3.0-dim);
	double hx = (xMax-xMin)/(double)(xSize-1);
	double hy = (yMax-yMin)/(double)(ySize-1);
	//if (plik) fprintf(plik, "h,hx,hy=%f,%f,%f\n",h,hx,hy);
	for(int i=0; i<num-1; i++)
		for(int j=0; j<num-1; j++)
		{
			double dxin = hx*(double)(tmpX[i]-tmpX[num-1]);
			double dxij = hx*(double)(tmpX[i]-tmpX[j]);
			double dxnj = hx*(double)(tmpX[num-1]-tmpX[j]);
			double dyin = hy*(double)(tmpY[i]-tmpY[num-1]);
			double dyij = hy*(double)(tmpY[i]-tmpY[j]);
			double dynj = hy*(double)(tmpY[num-1]-tmpY[j]);
			(*pC)[i][j] = 0.5 * (pow(sqrt(dxin*dxin+dyin*dyin), h) - pow(sqrt(dxij*dxij+dyij*dyij), h) + pow(sqrt(dxnj*dxnj+dynj*dynj), h));
			if(i==j && (*pC)[i][i]==0.0) MessageBox(NULL, "ComputeCxx, inside", "Error", MB_OK);
		}
	/*if (plik) fprintf(plik, "pC:\n");
	for (i=0;i<num-1;i++)
	{
	 for (int j=0;j<num-1;j++) if (plik) fprintf(plik,"%f ", (*pC)[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
	delete[] tmpX;
	delete[] tmpY;
	return num-1;
}

int RandField::ComputeCyx(double ***pC, double dim, int x0, int y0)
{
	if(!(x0>0 && x0<xSize && y0>0 && y0<ySize && x0%2==1 && y0%2==1))
		return -1;
	int down = (y0>=neighbSize) ? neighbSize : y0;
	int left = (x0>=neighbSize) ? neighbSize : x0;
	int up = (y0+neighbSize<ySize) ? neighbSize : ySize-1-y0;
	int right = (x0+neighbSize<xSize) ? neighbSize : xSize-1-x0;
	int num = (left+right+1)*down + 2*left + (right+1)/2 + ((left+1)/2+(right+1)/2)*(up/2);
	//if (plik) fprintf(plik, "ccyx: l/r/u/d/n: %d/%d/%d/%d/%d\n",left,right,up,down,num);
	*pC = new double*[3];
	for(int i=0; i<3; i++)
		(*pC)[i] = new double[num-1];
	double *tmpX = new double[num];
	double *tmpY = new double[num];
	int k = 0;
	for(int y=y0-down; y<y0; y++)
		for(int x=x0-left; x<=x0+right; x++)
		{
			tmpX[k] = x;
			tmpY[k] = y;
			k++;
		}
	for(int y=y0; y<=y0+1; y++)
		for(int x=x0-left; x<x0; x++)
		{
			tmpX[k] = x;
			tmpY[k] = y;
			k++;
		}
	for(int x=x0+1; x<=x0+right; x+=2)
	{
		tmpX[k] = x;
		tmpY[k] = y0+1;
		k++;
	}
	for(int y=y0+3; y<=y0+up; y++)
		for(int x=x0-left; x<=x0+right; x+=2)	// < ==> <= FIXME ?
		{
			tmpX[k] = x;
			tmpY[k] = y;
			k++;
		}
	//if (plik) fprintf(plik, "k=%d/%d\n", k, num);
	//if (plik) fprintf(plik, "tmpX={");
	//for (int i=0;i<num;i++) if (plik) fprintf(plik,"%f,",tmpX[i]);
	//if (plik) fprintf(plik,"}\n");
	//if (plik) fprintf(plik, "tmpY={");
	//for (int i=0;i<num;i++) if (plik) fprintf(plik,"%f,",tmpY[i]);
	//if (plik) fprintf(plik,"} k=%d/%d\n",k,num);
	double *tmpX0 = new double[3];
	double *tmpY0 = new double[3];
	tmpX0[0] = x0;
	tmpY0[0] = y0;
	tmpX0[1] = x0+1;
	tmpY0[1] = y0;
	tmpX0[2] = x0;
	tmpY0[2] = y0+1;
	//if (plik) fprintf(plik, "tmpX0={%f,%f,%f}\n", tmpX0[0], tmpX0[1], tmpX0[2]);
	//if (plik) fprintf(plik, "tmpY0={%f,%f,%f}\n", tmpY0[0], tmpY0[1], tmpY0[2]);
	double h = 2.0*(3.0-dim);
	double hx = (xMax-xMin)/(double)(xSize-1);
	double hy = (yMax-yMin)/(double)(ySize-1);
	for(int i=0; i<3; i++)
		for(int j=0; j<num-1; j++)
		{
			double dxin = hx*(double)(tmpX0[i]-tmpX[num-1]);
			double dxij = hx*(double)(tmpX0[i]-tmpX[j]);
			double dxnj = hx*(double)(tmpX[num-1]-tmpX[j]);
			double dyin = hy*(double)(tmpY0[i]-tmpY[num-1]);
			double dyij = hy*(double)(tmpY0[i]-tmpY[j]);
			double dynj = hy*(double)(tmpY[num-1]-tmpY[j]);
			(*pC)[i][j] = 0.5 * (pow(sqrt(dxin*dxin+dyin*dyin), h) - pow(sqrt(dxij*dxij+dyij*dyij), h) + pow(sqrt(dxnj*dxnj+dynj*dynj), h));
		}
	/*if (plik) fprintf(plik, "pC:\n");
	for (i=0;i<3;i++)
	{
	 for (int j=0;j<num-1;j++) if (plik) fprintf(plik,"%f ", (*pC)[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
	delete[] tmpX;
	delete[] tmpY;
	delete[] tmpX0;
	delete[] tmpY0;
	return num-1;
}

int RandField::ComputeCyy(double ***pC, double dim, int x0, int y0)
{
	if(!(x0>0 && x0<xSize && y0>0 && y0<ySize && x0%2==1 && y0%2==1))
		return -1;
	int tmpYNum = (y0+neighbSize<ySize) ? neighbSize+y0 : ySize-1;
	int tmpXNum = (x0+neighbSize<xSize) ? neighbSize+x0 : xSize-1;
	//if (plik) fprintf(plik, "tmpXnum,tmpYnum = %d, %d\n", tmpXNum, tmpYNum);
	*pC = new double*[3];
	for(int i=0; i<3; i++)
		(*pC)[i] = new double[3];
	double *tmpX0 = new double[3];
	double *tmpY0 = new double[3];
	tmpX0[0] = x0;
	tmpY0[0] = y0;
	tmpX0[1] = x0+1;
	tmpY0[1] = y0;
	tmpX0[2] = x0;
	tmpY0[2] = y0+1;
	//if (plik) fprintf(plik, "tmpX0={%f,%f,%f}\n", tmpX0[0], tmpX0[1], tmpX0[2]);
	//if (plik) fprintf(plik, "tmpY0={%f,%f,%f}\n", tmpY0[0], tmpY0[1], tmpY0[2]);
	double h = 2.0*(3.0-dim);
	double hx = (xMax-xMin)/(double)(xSize-1);
	double hy = (yMax-yMin)/(double)(ySize-1);
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
		{
			double dxin = hx*(double)(tmpX0[i]-tmpXNum);
			double dxij = hx*(double)(tmpX0[i]-tmpX0[j]);
			double dxnj = hx*(double)(tmpXNum-tmpX0[j]);
			double dyin = hy*(double)(tmpY0[i]-tmpYNum);
			double dyij = hy*(double)(tmpY0[i]-tmpY0[j]);
			double dynj = hy*(double)(tmpYNum-tmpY0[j]);
			(*pC)[i][j] = 0.5 * (pow(sqrt(dxin*dxin+dyin*dyin), h) - pow(sqrt(dxij*dxij+dyij*dyij), h) + pow(sqrt(dxnj*dxnj+dynj*dynj), h));
		}
	/*if (plik) fprintf(plik, "pC:\n");
	for (i=0;i<3;i++)
	{
	 for (int j=0;j<3;j++) if (plik) fprintf(plik,"%f ", (*pC)[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
	delete[] tmpX0;
	delete[] tmpY0;
	return 3;
}

bool RandField::SolveMatrixEquation(double **A, double **X, double **C, int m, int n)
{
	double **L = new double*[n];
	for(int i=0; i<n ; i++) 
	 {
	  L[i] = new double[n];
	  for (int j=0;j<n;j++) L[i][j] = 0.;
	 }
	double **Y = new double*[m];
	for(int i=0; i<m ; i++)
	{
		Y[i] = new double[n];
		for (int j=0;j<n;j++) Y[i][j] = 0.;
	}
	bool ok = Cholesky(C, L, n);
    /*if (plik) fprintf(plik, "L:\n");
	for (int i=0;i<n;i++)
	{
	 for (int j=0;j<n;j++) if (plik) fprintf(plik,"%f ", L[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
	if (!ok) MessageBox(NULL, "Cholesky", "Error", MB_OK);
	if (ok)
	{
		for(int i=0; i<m; i++)
		{
			if(L[0][0] == 0)
			{
				ok = false;
			}
			else
			{
				Y[i][0] = A[i][0]/L[0][0];
				if (ok)
				{
					for(int j=1; j<n; j++)
					{
						double sum = 0.0;
						for(int k=0; k<j; k++)
							sum += Y[i][k]*L[j][k];
						if(L[j][j] == 0)
						{
							ok = false;
							break;
						}
						Y[i][j] = (A[i][j]-sum)/L[j][j];
					}
				}
			}
			if(!ok)
				break;
		}
	}
	if(ok)
	{
		for(int i=0; i<m; i++)
		{
			X[i][n-1] = Y[i][n-1]/L[n-1][n-1];
			for(int j=n-2; j>=0; j--)
			{
				double sum = 0.0;
				for(int k=j+1; k<n; k++)
					sum += X[i][k]*L[k][j];
				if(L[j][j] == 0)
				{
					ok = false;
					break;
				}
				X[i][j] = (Y[i][j]-sum)/L[j][j];
			}
			if(!ok)
				break;
		}
	}
	for(int i=0; i<n; i++)
		delete[] L[i];
	delete[] L;
	for(int i=0; i<m; i++)
		delete[] Y[i];
	delete[] Y;
    /*if (plik) fprintf(plik, "X:\n");
	for (int i=0;i<m;i++)
	{
	 for (int j=0;j<n;j++) if (plik) fprintf(plik,"%f ", X[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
	return ok;
}

void RandField::DetermineB(double **B, int m, int n)
{
	for(int i=0; i<m; i++)
	{
		double sum = 0.0;
		for(int j=0; j<n; j++)
			sum += B[i][j];
		B[i][n] = 1.0 - sum;
	}
 /*if (plik) fprintf(plik, "B:\n");
	for (int i=0;i<m;i++)
	{
	 for (int j=0;j<n;j++) if (plik) fprintf(plik,"%f ", B[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
}

bool RandField::ComputeS(double ***pS, double **Cyy, double **B, double **Cyx, int m, int n)
{
	(*pS) = new double*[m];
	for(int i=0; i<m; i++)
	{
		(*pS)[i] = new double[m];
		for (int j=0;j<m;j++) (*pS)[i][j] = 0.;
	}
	double **A = new double*[m];
	for(int i=0; i<m; i++)
	{
		A[i] = new double[m];
		for (int j=0;j<m;j++) A[i][j] = 0.;
	}
	for(int i=0; i<m; i++)
		for(int j=0; j<m; j++)
		{
			A[i][j] = 0.0;
			for(int k=0; k<n; k++)
				A[i][j] += B[i][k]*Cyx[j][k];
			A[i][j] = Cyy[i][j] - A[i][j];
		}
	bool ok = Cholesky(A, *pS, m);
	for(int i=0; i<m; i++)
		delete[] A[i];
	delete[] A;
    /*if (plik) fprintf(plik, "*pS:\n");
	for (int i=0;i<m;i++)
	{
	 for (int j=0;j<m;j++) if (plik) fprintf(plik,"%f ", (*pS)[i][j]);
     if (plik) fprintf(plik,"\n");
	}*/
	return ok;
}

int RandField::ChooseMatrix(int x, int y)
{
	if(x == 1)
	{
		// 0 gdy 1, 6 dla ySize-2, 3 wpp
		//return (y == 0) ? 0 : (y == ySize-2) ? 6 : 3;		//FIXME ok??
		return (y == 1) ? 0 : (y == ySize-2) ? 6 : 3;
	}
	else if(x == xSize-2)
	{
		// analogicznie z x=xsize 2,8,5
		return (y == 1) ? 2 : (y == ySize-2) ? 8 : 5;
		//return (y == 0) ? 2 : (y == ySize-2) ? 8 : 5;
	}
	else
	{
		//x w srodku: 1,7,4
		return (y == 1) ? 1 : (y == ySize-2) ? 7 : 4;
		//return (y == 0) ? 1 : (y == ySize-2) ? 7 : 4;
	}
}

RandField::RandField() : neighbSize(3)
{
	int i,j;
	field = new double*[2];
	field[0] = new double[2];
	field[1] = new double[2];
	field[0][1] = field[1][0] = field[1][1] = 0.0;
	field[0][0] = 2.0;
	texid = new int*[2];
    for (i=0;i<2;i++) texid[i] = new int[2];
	for (i=0;i<2;i++)
	for (j=0;j<2;j++) texid[i][j] = 0;
	xSize = ySize = 2;
	xMin = -10.0;
	yMin = -10.0;
	xMax = 10.0;
	yMax = 10.0;
	//srand((unsigned)time(NULL));
	seed_uni((int)time(NULL));
	water_h = -.4;
	selX = selY = 0;
	cur_tex = 0;
}

RandField::~RandField()
{
	RemoveField();
}

bool RandField::SetField(double **field, int xSize, int ySize, 
						 double xMin, double xMax, double yMin, double yMax)
{
	if(xSize<4 || ySize<4)
		return false;
	RemoveField();
	this->xSize = xSize;
	this->ySize = ySize;
	this->xMin = xMin;
	this->xMax = xMax;
	this->yMin = yMin;
	this->yMax = yMax;
	this->field = new double*[xSize];
	for(int x=0; x<xSize; x++)
	{
		this->field[x] = new double[ySize];
		for(int y=0; y<ySize; y++)
			this->field[x][y] = field[x][y];
	}
	this->texid= new int*[xSize];
	for(int x=0; x<xSize; x++)
	{
		this->texid[x] = new int[ySize];
		for(int y=0; y<ySize; y++)
			this->texid[x][y] = 0;
	}
	return true;
}

void RandField::RemoveArray2(double **A, int m)
{
	for(int i=0; i<m; i++)
		delete[] A[i];
	delete[] A;
}

void RandField::GenerateRandomField(double dim, int k)
{
	if (k <=0) return;
	if (xSize >= 256 || ySize >= 256) return;
	double ***tabCxx = new double**[9];
	double ***tabCyx = new double**[9];
	double ***tabCyy = new double**[9];
	double ***tabB = new double**[9];
	double ***tabS = new double**[9];
	int *tabNum = new int[9];
	int *tabX = new int[9];
	int *tabY = new int[9];
	double rate = pow(0.5, 3.0-dim);
	for(int s=0; s<k; s++)
	{
		//MessageBox(NULL, "XXX", "XXX", MB_OK);
		EnlargeField();
		for(int x=1; x<xSize; x+=2)
			field[x][0] = (field[x-1][0]+field[x+1][0])/2;
		for(int y=1; y<ySize; y+=2)
			field[0][y] = (field[0][y-1]+field[0][y+1])/2;
		/*for (int x=0;x<xSize;x++)
		{
			for (int y=0;y<ySize;y++) if (plik) fprintf(plik, "%f ", field[x][y]);
			if (plik) fprintf(plik, "\n");
		}*/
		if(s==0)
		{
			tabX[0] = 1;		tabY[0] = 1;
			tabX[1] = 3;		tabY[1] = 1;
			tabX[2] = xSize-2;	tabY[2] = 1;
			tabX[3] = 1;		tabY[3] = 3;
			tabX[4] = 3;		tabY[4] = 3;
			tabX[5] = xSize-2;	tabY[5] = 3;
			tabX[6] = 1;		tabY[6] = ySize-2;
			tabX[7] = 3;		tabY[7] = ySize-2;
			tabX[8] = xSize-2;	tabY[8] = ySize-2;
			//for (int i=0;i<9;i++) if (plik) fprintf(plik, "tabX[%d]=%d ", i, tabX[i]); if (plik) fprintf(plik,"\n");
			//for (int i=0;i<9;i++) if (plik) fprintf(plik, "tabY[%d]=%d ", i, tabY[i]); if (plik) fprintf(plik,"\n");
			for(int i=0; i<9; i++)
			{
				tabCxx[i] = tabCyx[i] = tabCyy[i] = tabB[i] = tabS[i] = NULL;
				tabNum[i] = 0;
				//if (plik) fprintf(plik, "i=%d, computeCXX(x0=%d,y0=%d)\n", i, tabX[i], tabY[i]);
				tabNum[i] = ComputeCxx(&tabCxx[i], dim, tabX[i], tabY[i]);
				if (tabNum[i] == -1) MessageBox(NULL, "ComputeCxx", "Error", MB_OK);
				//blad gdy nie parzyste lub poza granica, a enlarge n -> 2*n-1 (nparzyste)
				
				if (ComputeCyx(&tabCyx[i], dim, tabX[i], tabY[i]) == -1)
					MessageBox(NULL, "ComputeCyx", "Error", MB_OK);
				
				if(ComputeCyy(&tabCyy[i], dim, tabX[i], tabY[i]) == -1)
					MessageBox(NULL, "ComputeCyy", "Error", MB_OK);
				
				tabB[i] = new double*[3];
				for(int j=0; j<3; j++)
					tabB[i][j] = new double[tabNum[i]+1];
				/*if (plik) fprintf(plik, "tabNum: ");
				for (int j=0;j<9;j++) if (plik) fprintf(plik, "%d ", tabNum[j]);
				if (plik) fprintf(plik, "\n");*/
				
				if(!SolveMatrixEquation(tabCyx[i], tabB[i], tabCxx[i], 3, tabNum[i]))
					MessageBox(NULL, "SolveMatrixEquation", "Error", MB_OK);
				
				DetermineB(tabB[i], 3, tabNum[i]);
				if (!ComputeS(&tabS[i], tabCyy[i], tabB[i], tabCyx[i], 3, tabNum[i]))
					MessageBox(NULL, "ComputeS", "Error", MB_OK);
				
			}
		}
		else	// s>0
		{
			for(int t=0; t<9; t++)
			for(int i=0; i<3; i++)
			for(int j=0; j<3; j++) tabS[t][i][j] *= rate;
		}
		for(int y=1; y<ySize; y+=2)
			for(int x=1; x<xSize; x+=2)
			{
				int t = ChooseMatrix(x, y);
				//if (plik) fprintf(plik, "%d = chooseMatrix(%d,%d)\n", t,x,y);
				double *vectX = GetX(x, y);
				double *vectZ = GenerateZ(3);
				double *vectY = ComputeY(tabB[t], tabS[t], vectX, vectZ, 3, tabNum[t]+1);
				//if (plik) fprintf(plik, "vectY={%f,%f,%f}\n", vectY[0], vectY[1], vectY[2]);
				field[x][y] = vectY[0];
				field[x+1][y] = vectY[1];
				field[x][y+1] = vectY[2];
				delete[] vectX;
				delete[] vectZ;
				delete[] vectY;
			}
	}
	for(int i=0; i<9; i++)
	{
		RemoveArray2(tabCxx[i], tabNum[i]);
		RemoveArray2(tabCyx[i], 3);
		RemoveArray2(tabCyy[i], 3);
		RemoveArray2(tabB[i], 3);
		RemoveArray2(tabS[i], 3);
	}
	delete[] tabCxx;
	delete[] tabCyx;
	delete[] tabCyy;
	delete[] tabB;
	delete[] tabS;
	delete[] tabNum;
	delete[] tabX;
	delete[] tabY;
}

void RandField::RaiseWater(double l)
{
	water_h += l;
}

void RandField::Raise(double l)
{
 field[selX][selY] += l;
}

void RandField::BigRaise(double l)
{
 int i,j;
 double factor;
 for (i=selX-2;i<=selX+2;i++)
	 for (j=selY-2;j<=selY+2;j++)
	 {
      if (i >= 0 && i < xSize && j >= 0 && j < ySize)
	    {
		 factor = 4. / (double)(abs(i - selX) + abs(j - selY)+1);
         field[i][j] += l * factor;
	    }
	 }
}

void RandField::InterpolateNormal(int i, int j, double* n_x, double* n_y, double* n_z)
{
 double nx[6];
 double ny[6];
 double nz[6];

 int k;

 if (i == 0 || j == 0 || i == xSize-1 || j == ySize-1)
   {
    *n_x = 0.;
	*n_y = 1.;
	*n_z = 0.;
	return;
   }

 double xSiz = xSize-1;
 double ySiz = ySize-1;
 double dx = xMax-xMin;
 double dy = yMax-yMin;

 double x0 = dx-(dx*(double)i/xSiz);
 double y0 = field[i][j];
 double z0 = dy-(dy*(double)j/ySiz);

 double x1 = dx-(dx*(double)i/xSiz);
 double y1 = field[i][j+1];
 double z1 = dy-(dy*(double)(j+1)/ySiz);

 double x2 = dx-(dx*(double)(i+1)/xSiz);
 double y2 = field[i+1][j+1];
 double z2 = dy-(dy*(double)(j+1)/ySiz);

 double x3 = dx-(dx*(double)(i+1)/xSiz);
 double y3 = field[i+1][j];
 double z3 = dy-(dy*(double)(j)/ySiz);

 double x4 = dx-(dx*(double)(i)/xSiz);
 double y4 = field[i][j-1];
 double z4 = dy-(dy*(double)(j-1)/ySiz);

 double x5 = dx-(dx*(double)(i-1)/xSiz);
 double y5 = field[i-1][j-1];
 double z5 = dy-(dy*(double)(j-1)/ySiz);

 double x6 = dx-(dx*(double)(i-1)/xSiz);
 double y6 = field[i-1][j];
 double z6 = dy-(dy*(double)(j)/ySiz);

 ComputeNormalVector(x0, y0, z0, x2, y2, z2, x1, y1, z1, nx[0], ny[0], nz[0]);
 ComputeNormalVector(x0, y0, z0, x3, y3, z3, x2, y2, z2, nx[1], ny[1], nz[1]);
 ComputeNormalVector(x0, y0, z0, x4, y4, z4, x3, y3, z3, nx[2], ny[2], nz[2]);
 ComputeNormalVector(x0, y0, z0, x5, y5, z5, x4, y4, z4, nx[3], ny[3], nz[3]);
 ComputeNormalVector(x0, y0, z0, x6, y6, z6, x5, y5, z5, nx[4], ny[4], nz[4]);
 ComputeNormalVector(x0, y0, z0, x1, y1, z1, x6, y6, z6, nx[5], ny[5], nz[5]);

 *n_x = *n_y = *n_z = 0.;
 for (k=0;k<6;k++)
   {
    *n_x += nx[k];
	*n_y += ny[k];
	*n_z += nz[k];
   }
 *n_x /= 6.;
 *n_y /= 6.;
 *n_z /= 6.;
}

void RandField::SaveCCM(FILE* f)
{
 //MessageBox(NULL, "stub", "stub", 0);
 int i,j;
 double I,J;
 double nx,ny,nz;
 char id[4];
 strcpy(id, "CCM");
 fwrite(id, 3, 1, f);
 fwrite(&xSize, sizeof(int), 1, f);
 fwrite(&ySize, sizeof(int), 1, f);
 //fwrite(&xMin, sizeof(double), 1, f);
 //fwrite(&xMax, sizeof(double), 1, f);
 //fwrite(&yMin, sizeof(double), 1, f);
 //fwrite(&yMax, sizeof(double), 1, f);
 fwrite(&water_h, sizeof(double), 1, f);
 float tmp;
 for (i=0;i<xSize;i++)
 for (j=0;j<ySize;j++)
   {
    fwrite(&field[i][j], sizeof(double), 1, f);
	fwrite(&texid[i][j], sizeof(int), 1, f);
	/*I = (double)(4*i) / (double)xSize;
    J = (double)(4*j) / (double)ySize;
    while (I >= 1.) I -= 1.;
    while (J >= 1.) J -= 1.;
	fwrite(&I, sizeof(double), 1, f);
	fwrite(&J, sizeof(double), 1, f);*/
	InterpolateNormal(i, j, &nx, &ny, &nz);
	tmp = nx;
	fwrite(&tmp, sizeof(float), 1, f);
	tmp = ny;
	fwrite(&tmp, sizeof(float), 1, f);
	tmp = nz;
	fwrite(&tmp, sizeof(float), 1, f);
   }
}

/*void RandField::SaveCCM_old(FILE* f)
{
 int i,j;
 double I,J;
 double nx,ny,nz;
 char id[4];
 strcpy(id, "CCM");
 fwrite(id, 3, 1, f);
 fwrite(&xSize, sizeof(int), 1, f);
 fwrite(&ySize, sizeof(int), 1, f);
 fwrite(&xMin, sizeof(double), 1, f);
 fwrite(&xMax, sizeof(double), 1, f);
 fwrite(&yMin, sizeof(double), 1, f);
 fwrite(&yMax, sizeof(double), 1, f);
 fwrite(&water_h, sizeof(double), 1, f);
 for (i=0;i<xSize;i++)
 for (j=0;j<ySize;j++)
   {
    fwrite(&field[i][j], sizeof(double), 1, f);
	fwrite(&texid[i][j], sizeof(int), 1, f);
	I = (double)(4*i) / (double)xSize;
    J = (double)(4*j) / (double)ySize;
    while (I >= 1.) I -= 1.;
    while (J >= 1.) J -= 1.;
	fwrite(&I, sizeof(double), 1, f);
	fwrite(&J, sizeof(double), 1, f);
	InterpolateNormal(i, j, &nx, &ny, &nz);
	fwrite(&nx, sizeof(double), 1, f);
	fwrite(&ny, sizeof(double), 1, f);
	fwrite(&nz, sizeof(double), 1, f);
   }
}*/

void RandField::DrawWater(double x, double y)
{
 //glDisable(GL_TEXTURE_2D);
 //glDisable(GL_LIGHTING);
 glBindTexture(GL_TEXTURE_2D, 0);
 glBegin(GL_TRIANGLES);
 glColor4f(0.0f, 0.f, 1.0f, 0.33f);
 glNormal3d(0., -1., 0.);
 glVertex3d(-x, water_h, -y);
 glVertex3d(-x, water_h, y);
 glVertex3d(x, water_h, -y);

 glNormal3d(0., -1., 0.);
 glVertex3d(x, water_h, -y);
 glVertex3d(x, water_h, y);
 glVertex3d(-x, water_h, y);
 glEnd();
 //glEnable(GL_TEXTURE_2D);
 //glEnable(GL_LIGHTING);
}

void TestTexture()
{
 int i,j;
 int I,J;
 I = J = 16;
 glBindTexture(GL_TEXTURE_2D, tids[0]);
 glBegin(GL_TRIANGLES);
 for (i=0;i<I;i++)
	 for (j=0;j<J;j++)
	 {
      glTexCoord2d((double)i / (double)I, (double)j / (double)J);
      glVertex3d((double)i, -1., (double)j);

	  glTexCoord2d((double)(i+1) / (double)I, (double)j / (double)J);
      glVertex3d((double)(i+1), -1., (double)j);

	  glTexCoord2d((double)(i+1) / (double)I, (double)(j+1) / (double)J);
      glVertex3d((double)(i+1), -1., (double)(j+1));

	  glTexCoord2d((double)i / (double)I, (double)j / (double)J);
      glVertex3d((double)i, -1., (double)j);

	  glTexCoord2d((double)(i+1) / (double)I, (double)(j+1) / (double)J);
      glVertex3d((double)(i+1), -1., (double)(j+1));

	  glTexCoord2d((double)i / (double)I, (double)(j+1) / (double)J);
      glVertex3d((double)i, -1., (double)(j+1));
	 }
 glEnd();
}

void glCube(double x, double y, double z, double v)
{
 glBegin(GL_LINE_STRIP);
   glVertex3d(x-v, y-v, z-v);
   glVertex3d(x-v, y+v, z-v);
   glVertex3d(x+v, y+v, z-v);
   glVertex3d(x+v, y-v, z-v);
   glVertex3d(x-v, y-v, z-v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(x-v, y-v, z+v);
   glVertex3d(x-v, y+v, z+v);
   glVertex3d(x+v, y+v, z+v);
   glVertex3d(x+v, y-v, z+v);
   glVertex3d(x-v, y-v, z+v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(x-v, y-v, z-v);
   glVertex3d(x-v, y+v, z-v);
   glVertex3d(x-v, y+v, z+v);
   glVertex3d(x-v, y-v, z+v);
   glVertex3d(x-v, y-v, z-v);
 glEnd();
 glBegin(GL_LINE_STRIP);
   glVertex3d(x+v, y-v, z-v);
   glVertex3d(x+v, y+v, z-v);
   glVertex3d(x+v, y+v, z+v);
   glVertex3d(x+v, y-v, z+v);
   glVertex3d(x+v, y-v, z-v);
 glEnd();
}

void RandField::DrawSelected()
{
 double dx = (xMax-xMin);
 double dy = (yMax-yMin);
 double xSiz = (double)(xSize-1);
 double ySiz = (double)(ySize-1);
 double x = dx-(dx*(double)selX/xSiz);
 double z = dy-(dy*(double)selY/ySiz);
 double y = field[selX][selY];
 double v = 8. / pow((double)(xSize + ySize), .5);
 glColor4d(1., 0., 0., 1.);
 glCube(x,y,z,v);
 glCube(-x,y,z,v);
 glCube(x,y,-z,v);
 glCube(-x,y,-z,v);
}

void RandField::Selected(int x, int y)
{
 selX += x;
 selY += y;
 while (selX >= xSize) selX -= xSize;
 while (selY >= ySize) selY -= ySize;
 while (selX < 0) selX += xSize;
 while (selY < 0) selY += ySize;
}

void RandField::NextTex(int i)
{
 texid[selX][selY] += i;
 if (texid[selX][selY] >= nTex) texid[selX][selY] -= nTex;
 if (texid[selX][selY] < 0)     texid[selX][selY] += nTex;
}

void RandField::NextGround(int inc)
{
 int old_tex = cur_tex;
 int i,j;
 cur_tex += inc;
 if (cur_tex >= nTex) cur_tex -= nTex;
 if (cur_tex < 0)     cur_tex += nTex;
  for (i=0;i<xSize;i++)
	 for (j=0;j<ySize;j++)
	 {
      if (texid[i][j] == old_tex) texid[i][j] = cur_tex;
	 }
}

void RandField::GetTexCoord(int i, int j)
{
 double I, J;
 //char tmp[1000];
 //glBindTexture(GL_TEXTURE_2D, texid[i][j]);
 I = (double)(4*i) / (double)xSize;
 J = (double)(4*j) / (double)ySize;
 while (I >= 1.) I -= 1.;
 while (J >= 1.) J -= 1.;
 /*if (i == 2 && j == 2)
 {
  sprintf(tmp, "%f %f", I, J);
  MessageBox(0, tmp, tmp, 0);
 }*/
 glTexCoord2d(I, J);
}


void RandField::Draw()
{
	//char tmp[100];
	DrawSelected();
	glBindTexture(GL_TEXTURE_2D, tids[cur_tex]);
	//TestTexture();
	//return;
	//sprintf(tmp, "%x", tids);
	//MessageBox(0, tmp, tmp, 0);
	double dx = (xMax-xMin);
	double dy = (yMax-yMin);
	glColor4f(1.f, 1.f, 1.f, 0.85f);
	double xSiz = (double)(xSize-1);
	double ySiz = (double)(ySize-1);
	for(int x=0; x<xSize-1; x++)
		for(int y=0; y<ySize-1; y++)
		{
			double x1 = dx-(dx*(double)x/xSiz);
			double y1 = dy-(dy*(double)y/ySiz);
			double x2 = dx-(dx*(double)(x+1)/xSiz);
			double y2 = dy-(dy*(double)(y+1)/ySiz);
			double nx,ny,nz;

			
			glBindTexture(GL_TEXTURE_2D, tids[texid[x][y]]);
			glBegin(gl_current_type);

			InterpolateNormal(x, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y);
			glVertex3f((float)x1, (float)field[x][y], (float)y1);

			InterpolateNormal(x, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y+1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);

			InterpolateNormal(x+1, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);
			glEnd();

            
			glBindTexture(GL_TEXTURE_2D, tids[texid[x+1][y+1]]);
			glBegin(gl_current_type);
			//glNormal3f((float)xN2, (float)zN2, (float)yN2);
			InterpolateNormal(x+1, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y+1);
			glVertex3f((float)x2, (float)field[x+1][y+1], (float)y2);

			InterpolateNormal(x+1, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);

			InterpolateNormal(x, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y+1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);
			glEnd();
		}
for(int x=0; x<xSize-1; x++)
		for(int y=0; y<ySize-1; y++)
		{
			double x1 = -(dx-dx*(double)x/xSiz);
			double y1 = (dy-dy*(double)y/ySiz);
			double x2 = -(dx-dx*(double)(x+1)/xSiz);
			double y2 = (dy-dy*(double)(y+1)/ySiz);
			double nx,ny,nz;

			
			glBindTexture(GL_TEXTURE_2D, tids[texid[x][y]]);
			glBegin(gl_current_type);

			InterpolateNormal(x, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y);
			glVertex3f((float)x1, (float)field[x][y], (float)y1);

			InterpolateNormal(x, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y+1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);

			InterpolateNormal(x+1, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);
			glEnd();

            
			glBindTexture(GL_TEXTURE_2D, tids[texid[x+1][y+1]]);
			glBegin(gl_current_type);
			//glNormal3f((float)xN2, (float)zN2, (float)yN2);
			InterpolateNormal(x+1, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y+1);
			glVertex3f((float)x2, (float)field[x+1][y+1], (float)y2);

			InterpolateNormal(x+1, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);

			InterpolateNormal(x, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y+1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);
			glEnd();
		}
	for(int x=0; x<xSize-1; x++)
		for(int y=0; y<ySize-1; y++)
		{
			double x1 = (dx-dx*(double)x/xSiz);
			double y1 = -(dy-dy*(double)y/ySiz);
			double x2 = (dx-dx*(double)(x+1)/xSiz);
			double y2 = -(dy-dy*(double)(y+1)/ySiz);
			double nx,ny,nz;

			
			glBindTexture(GL_TEXTURE_2D, tids[texid[x][y]]);
			glBegin(gl_current_type);

			InterpolateNormal(x, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y);
			glVertex3f((float)x1, (float)field[x][y], (float)y1);

			InterpolateNormal(x, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y+1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);

			InterpolateNormal(x+1, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);
			glEnd();

            
			glBindTexture(GL_TEXTURE_2D, tids[texid[x+1][y+1]]);
			glBegin(gl_current_type);
			//glNormal3f((float)xN2, (float)zN2, (float)yN2);
			InterpolateNormal(x+1, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y+1);
			glVertex3f((float)x2, (float)field[x+1][y+1], (float)y2);

			InterpolateNormal(x+1, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);

			InterpolateNormal(x, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y+1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);
			glEnd();
		}
for(int x=0; x<xSize-1; x++)
		for(int y=0; y<ySize-1; y++)
		{
			double x1 = -(dx-dx*(double)x/xSiz);
			double y1 = -(dy-dy*(double)y/ySiz);
			double x2 = -(dx-dx*(double)(x+1)/xSiz);
			double y2 = -(dy-dy*(double)(y+1)/ySiz);
			double nx,ny,nz;

			
			glBindTexture(GL_TEXTURE_2D, tids[texid[x][y]]);
			glBegin(gl_current_type);

			InterpolateNormal(x, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y);
			glVertex3f((float)x1, (float)field[x][y], (float)y1);

			InterpolateNormal(x, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y+1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);

			InterpolateNormal(x+1, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);
			glEnd();

            
			glBindTexture(GL_TEXTURE_2D, tids[texid[x+1][y+1]]);
			glBegin(gl_current_type);
			//glNormal3f((float)xN2, (float)zN2, (float)yN2);
			InterpolateNormal(x+1, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y+1);
			glVertex3f((float)x2, (float)field[x+1][y+1], (float)y2);

			InterpolateNormal(x+1, y, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x+1, y);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);

			InterpolateNormal(x, y+1, &nx, &ny, &nz);
			glNormal3d(nx, ny, nz);
			GetTexCoord(x, y+1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);
			glEnd();
		}
	glEnd();
    DrawWater(dx, dy);
}
/*void RandField::Draw()
{
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(gl_current_type);
	double dx = (xMax-xMin);
	double dy = (yMax-yMin);
	double xSiz = (double)(xSize-1);
	double ySiz = (double)(ySize-1);
	for(int x=0; x<xSize-1; x++)
		for(int y=0; y<ySize-1; y++)
		{
			double x1 = xMin + dx*(double)x/xSiz;
			double y1 = yMin + dy*(double)y/ySiz;
			double x2 = xMin + dx*(double)(x+1)/xSiz;
			double y2 = yMin + dy*(double)(y+1)/ySiz;
			double xN1 = 0.0;
			double yN1 = 0.0;
			double zN1 = 0.0;
			ComputeNormalVector(x1, y1, field[x][y], x2, y1, field[x+1][y],
				x1, y2, field[x][y+1], xN1, yN1, zN1);
			double xN2 = 0.0;
			double yN2 = 0.0;
			double zN2 = 0.0;
			ComputeNormalVector(x2, y2, field[x+1][y+1], x2, y1, field[x+1][y],
				x1, y2, field[x][y+1], xN2, yN2, zN2);
			glNormal3f((float)xN1, (float)zN1, (float)yN1);
			glVertex3f((float)x1, (float)field[x][y], (float)y1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);
			glNormal3f((float)xN2, (float)zN2, (float)yN2);
			glVertex3f((float)x2, (float)field[x+1][y+1], (float)y2);
			glVertex3f((float)x2, (float)field[x+1][y], (float)y1);
			glVertex3f((float)x1, (float)field[x][y+1], (float)y2);
		}
	glEnd();
}*/
