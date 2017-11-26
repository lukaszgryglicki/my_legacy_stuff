#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926

int main()
{
 int i;
 printf("_sine_table:\n");
 for (i=0;i<=90;i++)
	 printf(".long 0x%x\n", 
			 (int)(sin(
			     ((float)(i)*PI)/180.0
			    )*65536.0)
			 );
}
