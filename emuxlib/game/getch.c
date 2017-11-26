#include <getch.h>
#include <stdio.h>

int main()
{
 char a[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
 while (true) 
  { 
   unsigned long a = getch(); 
   if (a==0x1b) { a = 0x1b0000; a += 0x100*getch(); a+=getch();}
   printf("%d #### 0x%x ####\n",a,a); 
  }
}

