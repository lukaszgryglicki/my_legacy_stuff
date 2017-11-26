#ifndef GAUSS_H_FKU
#define GAUSS_H_FKU

typedef struct _Uni_save
{
  double u[98];
  double c;
  double cd;
  double cm;
  int ui;
  int uj;
} Uni_save;

void seed_uni(int ijkl);
double gaussian();

#endif

