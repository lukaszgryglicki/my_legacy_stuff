#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926

int main()
{
 float kat;
 printf("_triangle_fan_st1:\n");
 printf("_st_up_vertex:\n");
 printf (".long 0,6*ONE,0\n");
 printf("_st_down_vertex:\n");
 printf (".long 0,-60000,0\n");
 printf("_st_vertexes:\n");
 for (kat=0.0;kat<=360.0;kat+=10.0)
 {
  printf(".long %d,-60000,%d\n",
		  2*(int)(cos(kat*PI/180.0)*65536.0),
		  2*(int)(sin(kat*PI/180.0)*65536.0)
		  );
 }
 
}
