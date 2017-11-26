#include <mh.h>

#define AsmCmpStr NULL

extern int qsortasm(void*,int (*)(const void*, const void*), unsigned int, unsigned int);
//extern int asm_qsort_str(char**, unsigned int, unsigned int);

void show_int(int* t, int n)
{
 for (int i=0;i<n;i++)
    printf("%d,",t[i]);
 printf("\n");
}

void show_uch(unsigned char** t, int n)
{
 for (int i=0;i<n;i++)
    printf("%d,",*t[i]);
 printf("\n");
}

void show_pch(char** par, int n)
{
 for (int i=0;i<n;i++) printf("%s\n", par[i]);
 putchar('\n');
}

int cmp_int_func(const void* a, const void* b)
{
 //printf("cmp(%d, %d)\n", (int)a, (int)b);
 return ((int)a>=(int)b);
}

int cmp_uchar_func(const void* a, const void* b)
{
 return (*((unsigned char*)a)>=*((unsigned char*)b));
}


int cmp_pchar_func(const void* apt, const void* bpt)
{
 //printf("C-Function\n");
 char a = *((char*)apt);
 char b = *((char*)bpt);
 int i=0;
 while (a && b && a==b)
 	{
	 i++;
         a=*((char*)apt+i);
	 b=*((char*)bpt+i);
	}
 return (a>=b);
}

int main(int lb, char** par)
{
 int n;
 int l;
 if (lb-2) n = 24;
else sscanf(par[1],"%d", &n);
 randomize();
 int* table = new int[n];
 for (int i=0;i<n;i++) table[i] = random(n);
 //char** table = new char*[n];
 //for (int i=0;i<n;i++)
 //  {
 //   l = 6+random(20);
 //   table[i] = new char[l];
 //   for (int j=0;j<l-1;j++) table[i][j] = random(26)+'a';
 //   table[i][l-1] = '\0';
 //  }
 //show_pch(table,n);
 //printf("%p\n%p\n%p\n", table, table[0], table[1]);
 //cmp_int_func((void*)4,(void*)3);
 //printf("func=%p, %d\n", cmp_int_func, cmp_int_func);
 //qsortasm(table, cmp_pchar_func,0,n-1);
 qsortasm(table, cmp_int_func,0,n-1);
 //asm_qsort_str(table,0,n-1);
 //qsort(table, 0,n-1, cmp_pchar_func);
 //show_pch(table,n);
 kill_random();
 return 0;
}

