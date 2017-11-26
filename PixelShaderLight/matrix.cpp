#include "matrix.h"				// do³¹cza nag³owki funkcji

extern float angX, angY, angZ;	// przekszta³cenia sceny, zewnêtrzne zmienne z pliku camera.cpp
extern float disX, disY, disZ;	// modyfikowane przez f-cje obliczaj¹ce rotacjê i translacjê sceny

// Oblicza przesuniêcie, wejœciowe dane to: przód/ty³/lewo/prawo/góra/dó³
// Mog¹ oczywiœcie byæ dowolne
// F-cja przekszta³ca zadany wekor kierunku macierzami obrotów tak aby wizualnie na ekranie
// Przesuwanie by³o do przodu/ty³o/górê/lewo itp bez wzglêdu na to jak ustawiona jets kamera
// Czyli trzeba po prostu odwórciæ przekszta³cenia œwiata
void matrix_translate(float tx, float ty, float tz)
{
 float *temp, *rot, **m;

 temp = vector(4);				// tworzy wektor kierunku zadany przez u¿ytkownika (tx,ty,tz,1) (x,y,z,w)
 temp[0] = tx;					// np w przód to (0,0,-0.1,1)
 temp[1] = ty;
 temp[2] = tz;
 temp[3] = 1.f;

 m = matrix(4);					// tworzy macierz 4x4 (bêdzie w niej przechowywane przekszta³cenie œwiata - czêœæ dotycz¹ca obrotów)
 I_matrix(m, 4);				// M=I, przypisanie macierzy jednostkowej

 /* zxy */
 m_rotatez(&m, -angZ);			// przekszta³cenia od ty³u, kolejno przemna¿anie macierzy widoku o obroty Z,Y,X
 m_rotatey(&m, -angY);
 m_rotatex(&m, -angX);

 rot = vector(4);				// tworzenioe wektora wynikowego
 matrix_mul_vector(rot, m, temp, 4);	// przemno¿enie wketora kierunku przez macierz obrotu œwiata

 disX += rot[0];				// przesuniêcie siê w wybranym kierunku obróconym o obroty œwiata
 disY += rot[1];				// czyli np klawisz 'w' przesuwa kamerê w przód (wizualnie) bez wzglêdu na obroty
 disZ += rot[2];				

 delete [] rot;					// zwolnienie 2 tymvczasowych wektorów i macierzy
 delete [] temp;
 free_matrix(m, 4);
}

// Przypadek rotacji prostszy, po prostu zmiana globalnych k¹tó obrotu
void matrix_rotate(float rx, float ry, float rz)
{
 angX += rx;
 angY += ry;
 angZ += rz;
}

// Tworzy wektor
float* vector(int siz)
{
 if (siz <= 0) return NULL;
 return new float[siz];
}


// tworzy macierz
float** matrix(int siz)
{
 float** mem;
 int i;
 if (siz <= 0) return NULL;
 mem = new float*[siz];
 for (i=0;i<siz;i++) mem[i] = vector(siz);
 return mem;
}


// tworzy macierz (nie koniecznie kwadratow¹)
float** matrix2(int siz1, int siz2)
{
 float** mem;
 int i;
 if (siz1 <= 0 || siz2 <= 0) return NULL;
 mem = new float*[siz1];
 for (i=0;i<siz1;i++) mem[i] = vector(siz2);
 return mem;
}


// tworzy macierz 3D (tensor)
float*** matrix3(int siz1, int siz2, int siz3)
{
 float*** mem;
 int i;
 if (siz1 <= 0 || siz2 <= 0 || siz3 <= 0) return NULL;
 mem = new float**[siz1];
 for (i=0;i<siz1;i++) mem[i] = matrix2(siz2, siz3);
 return mem;
}


// tworzy macierz jednostkow¹ na zadanej macierzy
void I_matrix(float** dst, int siz)
{
 int i,j;
 if (!dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++)
   {
    if (i == j) dst[i][j] = 1.;
    else dst[i][j] = 0.;
   }
}


// Mno¿y macierze przez siebie (odpowienie rozmiary musz¹ byæ)
// C[ma x nb] = A[ma x mb] x B[na x nb]
float** matrix_mul(float** m, float** n, int ma, int mb, int na, int nb)
{
 float** dst;
 int k,j,i;
 if (ma <= 0 || mb  <= 0 || na <= 0 || nb <=0 || mb != na) return NULL;
 if (!m || !n) return NULL;
 dst = new float*[ma];
 if (!dst) return NULL;
 for (i=0;i<ma;i++) dst[i] = new float[nb];
 for (i=0;i<ma;i++)
 for (j=0;j<nb;j++)
    {
     dst[i][j] = 0.0;
     for (k=0;k<mb;k++) dst[i][j] += m[i][k] * n[k][j];
    }
 return dst;
}


// Mnozy zadany wektor przez macierz
// dst[len] = M[len x len] x v[len]
void matrix_mul_vector(float* dst, float** m, float* v, int len)
{
 int i,j;
 if (!dst || !m || !v || len <=0) return;
 for (i=0;i<len;i++)
    {
     dst[i] = 0.0;
     for (j=0;j<len;j++) dst[i] += v[j] * m[i][j];
    }
}


// Zwalnia macierz
void free_matrix(float** m, int siz)
{
 int i;
 if (siz <= 0) return;
 for (i=0;i<siz;i++) delete [] m[i];
 delete [] m;
}


// Zwalnia macierz 3D (tensor)
void free_matrix3(float*** m, int siz1, int siz2)
{
 int i,j;
 if (siz1 <= 0 || siz2 <= 0) return;
 for (i=0;i<siz1;i++)
 for (j=0;j<siz2;j++) delete [] m[i][j];
 for (i=0;i<siz1;i++) delete [] m[i];
 delete [] m;
}


// Kopiuje macierz
void copy_matrix(float** dst, float** src, int siz)
{
 int i,j;
 if (!src || !dst || siz<= 0) return;
 for (i=0;i<siz;i++) for (j=0;j<siz;j++) dst[i][j] = src[i][j];
}


// Kopiuje macierz 3D (tensor 3-ciego rzêdu)
void copy_matrix3(float*** dst, float*** src, int siz1, int siz2, int siz3)
{
 int i,j,k;
 if (!src || !dst || siz1 <= 0 || siz2 <= 0 || siz3 <= 0) return;
 for (i=0;i<siz1;i++)
 for (j=0;j<siz2;j++)
 for (k=0;k<siz3;k++)
	 dst[i][j][k] = src[i][j][k];
}


// próbuje znaleŸæ niezerow¹ kolumnê do zamiany przy  odwracaniu macierzy
int try_swap(float** m, int idx, int dim)
{
 int x;
 for (x=idx;x<dim;x++) if (m[idx][x]) return x;
 return -1;
}


// próbuje odwróciæ macierz, jak siê nie da zwraca NULL
float** invert_matrix(float** srcC, int dim)
{
 float** src, **dst;
 float div, pom;
 register int x,k;
 int i,swit;
 float* vectmp;
 src = matrix(dim);
 dst = matrix(dim);
 vectmp = vector(dim);
 copy_matrix(src, srcC, dim);
 I_matrix(dst, dim);
 for (i=0;i<dim;i++)
   {
    div = src[i][i];
    if (div == 0.0)
      {
       swit = try_swap(src, i, dim);
       if (swit < 0) return NULL;
       for (x=0;x<dim;x++) vectmp[x]    = src[x][i];
       for (x=0;x<dim;x++) src[x][i]    = src[x][swit];
       for (x=0;x<dim;x++) src[x][swit] = vectmp[x];
       for (x=0;x<dim;x++) vectmp[x]    = dst[x][i];
       for (x=0;x<dim;x++) dst[x][i]    = dst[x][swit];
       for (x=0;x<dim;x++) dst[x][swit] = vectmp[x];
       div = src[i][i];
      }
    for (x=0;x<dim;x++)
      {
       src[x][i] /= div;
       dst[x][i] /= div;
      }
    for (k=0;k<dim;k++)
      {
       pom = src[i][k];
       if (k-i)
         {
          for (x=0;x<dim;x++) src[x][k] -= pom* src[x][i];
          for (x=0;x<dim;x++) dst[x][k] -= pom* dst[x][i];
         }
      }
   }
 free_matrix(src, dim);
 delete [] vectmp;
 return dst;
}

// Tworzy macierz obrotu o k¹t ang wzglêdem osi OX
void rotatex(float** m, float ang)
{
 ang /= 180.f/PI;
 m[1][1] = cos(ang);
 m[2][1] = sin(ang);
 m[1][2] = -sin(ang);
 m[2][2] = cos(ang);
}


// Tworzy macierz obrotu o k¹t ang wzglêdem osi OY
void rotatey(float** m, float ang)
{
 ang /= 180.f/PI;
 m[0][0] = cos(ang);
 m[2][0] = -sin(ang);
 m[0][2] = sin(ang);
 m[2][2] = cos(ang);
}


// Tworzy macierz obrotu o k¹t ang wzglêdem osi OZ
void rotatez(float** m, float ang)
{
 ang /= 180.f/PI;
 m[0][0] = cos(ang);
 m[1][0] = sin(ang);
 m[0][1] = -sin(ang);
 m[1][1] = cos(ang);
}


// Tworzy macierz translacji o arg wzglêdem osi OX
void translatex(float** m, float arg)
{
 m[0][3] = arg;
}


// Tworzy macierz translacji o arg wzglêdem osi OY
void translatey(float** m, float arg)
{
 m[1][3] = arg;
}


// Tworzy macierz translacji o arg wzglêdem osi OZ
void translatez(float** m, float arg)
{
 m[2][3] = arg;
}


// Tworzy macierz translacji o (x,y,z)
void translate(float** m, float x, float y, float z)
{
 translatex(m, x);
 translatey(m, y);
 translatez(m, z);
}


// Tworzy macierz skali arg razy wzglêdem osi OX
void scalex(float** m, float arg)
{
 m[0][0] = arg;
}


// Tworzy macierz skali arg razy wzglêdem osi OY
void scaley(float** m, float arg)
{
 m[1][1] = arg;
}


// Tworzy macierz skali arg razy wzglêdem osi OZ
void scalez(float** m, float arg)
{
 m[2][2] = arg;
}


// Tworzy macierz skali (x,y,z)
void scale(float** m, float x, float y, float z)
{
 scalex(m, x);
 scaley(m, y);
 scalez(m, z);
}


// Przekszta³ca zadan¹ macierz M o macierz mat
void m_ownmatrix(float*** m, float** mat)
{
 float** newm;
 newm = matrix_mul(*m, mat, 4, 4, 4, 4);
 free_matrix(*m, 4);
 *m = newm;
}


// Przekszta³ca zadan¹ macierz M o translacjê (x,y,z)
void m_translate(float*** m, float x, float y, float z)
{
 float** trans;
 float** newm;
 trans = matrix(4);
 I_matrix(trans, 4);
 translate(trans, x, y, z);
 newm = matrix_mul(*m, trans, 4, 4, 4, 4);
 free_matrix(*m, 4);
 free_matrix(trans, 4);
 *m = newm;
}


// Przekszta³ca zadan¹ macierz M o skalê (x,y,z)
void m_scale(float*** m, float x, float y, float z)
{
 float** scal;
 float** newm;
 scal = matrix(4);
 I_matrix(scal, 4);
 scale(scal, x, y, z);
 newm = matrix_mul(*m, scal, 4, 4, 4, 4);
 free_matrix(*m, 4);
 free_matrix(scal, 4);
 *m = newm;
}


// Przekszta³ca zadan¹ macierz M o rotacjê wzglêdem osi OX o k¹t ang
void m_rotatex(float*** m, float ang)
{
 float** rot;
 float** newm;
 rot = matrix(4);
 I_matrix(rot, 4);
 rotatex(rot, ang);
 newm = matrix_mul(*m, rot, 4, 4, 4, 4);
 free_matrix(*m, 4);
 free_matrix(rot, 4);
 *m = newm;
}


// Przekszta³ca zadan¹ macierz M o rotacjê wzglêdem osi OY o k¹t ang
void m_rotatey(float*** m, float ang)
{
 float** rot;
 float** newm;
 rot = matrix(4);
 I_matrix(rot, 4);
 rotatey(rot, ang);
 newm = matrix_mul(*m, rot, 4, 4, 4, 4);
 free_matrix(*m, 4);
 free_matrix(rot, 4);
 *m = newm;
}


// Przekszta³ca zadan¹ macierz M o rotacjê wzglêdem osi OZ o k¹t ang
void m_rotatez(float***m, float ang)
{
 float** rot;
 float** newm;
 rot = matrix(4);
 I_matrix(rot, 4);
 rotatez(rot, ang);
 newm = matrix_mul(*m, rot, 4, 4, 4, 4);
 free_matrix(*m, 4);
 free_matrix(rot, 4);
 *m = newm;
}

// Koniec funkcji biblioteki matrix.cpp
