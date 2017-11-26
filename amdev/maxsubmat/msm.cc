#include <iostream>
#include <cstdlib>
#include <ctime>

typedef struct info{int i; int j; int max;} info;

int row(int** a, int i,  int j1, int j2) {
  int s = 0;
  for (int j = j1; j <= j2; j++) s += a[i][j];
  return s;
}

int column(int** a, int j, int i1, int i2) {
  int s = 0;
  for (int i = i1; i <= i2; i++) s += a[i][j];
  return s;
}

void max_matrix(int** a, int n) {
  info** p = new info*[n];
  for (int i = 0; i < n; i++) p[i] = new info[n];
 
  info max_info = {0, 0, a[0][0]};
  int max_i = 0;
  int max_j = 0;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
       p[i][j] = (info){i, j, a[i][j]};
       if (i > 0) {
          info up = p[i-1][j];
          up = (info){up.i, up.j, up.max + row(a, i, up.j, j)};
          if (up.max > p[i][j].max) p[i][j] = up;
       }
       if (j > 0) {
          info left = p[i][j - 1];
          left = (info){left.i, left.j, left.max + column(a, j, left.i, i)};
          if (left.max > p[i][j].max) p[i][j] = left;
       }
       if (max_info.max < p[i][j].max) {
           max_info = p[i][j];
           max_i = i;
           max_j = j;
       }
    }
  }

  std::cout << "The submatrix starts at (" << max_info.i << ", " << max_info.j 
            << ") and ends at (" << max_i << ", " << max_j << ") and the sum is " << max_info.max << std::endl;

  for (int i = 0; i < n; i++) delete[] p[i];
  delete[] p;
}

int main()
{
    int n = 1000;
    int r1 = 201;
    int r2 = 100;
    int **matrix = new int*[n];
    time_t t;

    time(&t);
    srand(t);

    for (int i=0;i<n;i++) 
    {
	matrix[i] = new int[n];
	for (int j=0;j<n;j++) matrix[i][j] = (rand() % r1) - r2;
    }

    max_matrix(matrix, n);

    return 0;
}

