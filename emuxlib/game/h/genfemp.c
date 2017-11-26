#include <mh.h>

int main(int lb, char** par)
{
 FILE* o = fopen(par[1],"w");
 fprintf(o,"256\n");
 for (int i=0;i<0x100;i++)
   {
    fprintf(o,"0x%x\n", i);
    fprintf(o,"000000\n000000\n000000\n000000\n000000\n000000\n000000\n000000\n000000\n000000\n000000\n000000\n");
   }
 fclose(o);
}

