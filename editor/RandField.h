#ifndef RANDFIELD_MF87XC0H437TQ538TY74CYT8
#define RANDFIELD_MF87XC0H437TQ538TY74CYT8

class RandField : public Scene
{
	const int neighbSize;	// size of neighborhood
	//const int newNum = 3;	// number of new points
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	double **field;
	int** texid;
	int cur_tex;
	double water_h;
	int xSize;
	int ySize;
	int selX, selY;
	void RemoveField();
	void ComputeNormalVector(double x1, double y1, double z1, 
							double x2, double y2, double z2,
							double x3, double y3, double z3,
							double &xN, double &yN, double &zN);
	bool Cholesky(double **in, double **out, int n);
	void EnlargeField();
	double* GetX(int x0, int y0);
	double* ComputeY(double **B, double **S, double *vectX, double*vectZ, int m, int n);
	double* GenerateZ(int n);
	int ComputeCxx(double ***pC, double dim, int x0, int y0);
	int ComputeCyx(double ***pC, double dim, int x0, int y0);
	int ComputeCyy(double ***pC, double dim, int x0, int y0);
	bool SolveMatrixEquation(double **A, double **X, double **C, int m, int n);
	void DetermineB(double **B, int m, int n);
	bool ComputeS(double ***pS, double **Cyy, double **B, double **Cyx, int m, int n);
	int ChooseMatrix(int x, int y);
	void RemoveArray2(double **A, int m);
	void DrawWater(double, double);
	void DrawSelected();
	void GetTexCoord(int, int);
	void InterpolateNormal(int, int, double*, double*, double*);
public:
	RandField();
	~RandField();
	bool SetField(double **field, int xSize, int ySize, 
						 double xMin, double xMax, double yMin, double yMax);
	void GenerateRandomField(double dim, int k);
	void RaiseWater(double);
	void Raise(double);
	void BigRaise(double);
	void Selected(int, int);
	void NextTex(int);
	void NextGround(int);
	void SaveCCM(FILE*);
	virtual void Draw();
};

#endif
