#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

double v = -1.0;
double rpm = -1.0;
double burn = -1.0;
double dose = -1.0;
double maxrpm = -1.0;
double vmax = -1.0;
int cylinders = -1.0;
int diesel = 0;
char engine[32];
double density;
 

void oblicz_dawke()
{
 if (rpm < 0 || v < 0 || burn < 0)
 {
   printf("Za malo danych do obliczenia dawki\nPotrzebne v, rpm i burn\n");
   return;
 }
 printf("Mamy spalanie %4.2f przy %5.1f km/h i %5.0f RPM\n", burn, v, rpm);
 double rpm100;
 rpm100 = (rpm * 100.0) / v;
 printf(" %5.0f RPM przy 100.0 km/h\n", rpm100);
 double rounds;
 rounds = rpm100 * 60.0;
 printf("Czyli %8.0f obrotów po przejechaniu 100.0 km.\n", rounds);
 printf("%8.0f obrotów walu korbowego spala %4.2f litrów paliwa\n", rounds, burn);
 double mass;
 mass = burn * density;
 double doses;
 doses = (double)cylinders / 2.0;
 printf("mamy %1.0f dawek na jeden obrót walu korbowego\nw silniku %s\n", doses, engine);
 doses *= rounds;
 printf("Czyli %4.3f kg paliwa wtryskiwane w %6.1f dawkach\n", mass, doses);
 double doss = (mass / doses) * 1000000.0;
 printf("DAWKA %5.2f mg paliwa [%4.0f km/h %5.0f RPM, %4.1f l/100 km]\n", doss, v, rpm, burn);
 printf("DAWKA %5.2f mg paliwa [%4.0f km/h %5.0f RPM, %4.1f l/100 km]\n", doss/1.5, v, rpm, burn/1.5);
 printf("DAWKA %5.2f mg paliwa [%4.0f km/h %5.0f RPM, %4.1f l/100 km]\n", doss*2.5, v, rpm, burn*2.5);
}

void oblicz_spalanie()
{
 if (rpm < 0 || v < 0 || dose < 0)
 {
   printf("Za malo danych do obliczenia spalania\nPotrzebne v, rpm i dose\n");
   return;
 }
 double rpm100;
 rpm100 = (rpm * 100.0) / v;
 printf(" %5.0f RPM przy 100.0 km/h, dawka %4.2f\n", rpm100, dose);
 double rounds;
 rounds = rpm100 * 60.0;
 printf("Czyli %8.0f obrotów po przejechaniu 100.0 km.\n", rounds);
 double mass;
 mass = burn * density;
 double doses;
 doses = (double)cylinders / 2.0;
 printf("mamy %1.0f dawek na jeden obrót walu korbowego\nw silniku %s\n", doses, engine);
 doses *= rounds;
 double kg = (doses * dose) / 1000000.;
 printf("Spalanie przy dawce %4.2f weynosi %4.2f kg/100 km\n", dose, kg);
 double bur = kg / density;
 printf("Dawka %4.2f mg, %3.0f km/h %4.0f RPM, spalanie: %4.2f l/100 km\n", dose, v, rpm, bur);
 printf("Dawka %4.2f mg, %3.0f km/h %4.0f RPM, spalanie: %4.2f l/100 km\n", dose/1.5, v, rpm, bur/1.5);
 printf("Dawka %4.2f mg, %3.0f km/h %4.0f RPM, spalanie: %4.2f l/100 km\n", dose*2.5, v, rpm, bur*2.5);

}

void engine_calculations()
{
 if (cylinders < 1)
 {
  printf("Nie podano ilosci cylindrów, zakladam R4\n");
  cylinders = 4;
 }
 if (cylinders <= 5)
 {
  sprintf(engine, "R%d", cylinders);
 }
 if (cylinders == 6)
 {
  strcpy(engine, "R6 or V6 or VR6");
 }
 if (cylinders > 6)
 {
  sprintf(engine, "V%d", cylinders);
 } 
 if (diesel == 0) 
 {
     strcat(engine, " benzyna");
     density = 0.78;
 }
 else 
 {
     strcat(engine, " diesel");
     density = 0.89;
 }
 printf("Engine: %s\n", engine);
 oblicz_dawke();
 oblicz_spalanie();
}

void help()
{
 printf("Opcje:\n");
 printf("\t-h\tpomoc\n");
 printf("\t-v liczba\tpredkosc\n");
 printf("\t-r liczba\trpm\n");
 printf("\t-b liczba\tspalanie na 100 km\n");
 printf("\t-d liczba\tdawka wtrysku mg\n");
 printf("\t-m liczba\tmax rpm\n");
 printf("\t-V liczba\tmax predkosc\n");
 printf("\t-c liczba\tilosc cylindrow\n");
 printf("\t-D \tsilnik diesla\n");
}

int main(int lb, char** par)
{
 int u;
 while ((u = getopt(lb,par,"hDv:r:b:d:m:V:c:"))!=-1)
 {
  switch (u)
  {
    case 'D': diesel = 1; break;
    case 'h': help(); break;
    case 'v': v = atof(optarg); break;
    case 'r': rpm = atof(optarg); break;
    case 'b': burn = atof(optarg); break;
    case 'd': dose = atof(optarg); break;
    case 'm': maxrpm = atof(optarg); break;
    case 'V': vmax = atof(optarg); break;
    case 'c': cylinders = atoi(optarg); break;
    default: printf("%s: Unrecognized option\n",par[0]); exit(1);
   }
 }
 engine_calculations();
}

