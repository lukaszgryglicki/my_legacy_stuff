#include <stdio.h>

extern int asm_qsort_str(char**,unsigned int,unsigned int);

int main()
{
 char** t = new char*[8];
 for (int i=0;i<20;i++) t[i] = new char[20];
 strcpy(t[0],"osama");
 strcpy(t[1],"bin");
 strcpy(t[2],"laden");
 strcpy(t[3],"morgoth");
 strcpy(t[4],"the");
 strcpy(t[5],"best");
 strcpy(t[6],"ssssskss");
 strcpy(t[7],"sssssfab");
 printf("before qsort:\n");
 for (int i=0;i<8;i++) printf("%s\n", t[i]);
 asm_qsort_str(t,0,7);
 printf("after qsort:\n");
 for (int i=0;i<8;i++) printf("%s\n", t[i]);
 return 0;
}

