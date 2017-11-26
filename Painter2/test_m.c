
int main()
{
 int tab[102400];
 int i,num;
 for (i=0;i<102400;i++) tab[i]= 0;
 while (scanf("%d\n", &num) == 1)
   {
    tab[num]++;
   }
 for (i=0;i<102400;i++) if (tab[i] > 0) printf("tab[%d] = %d\n", i, tab[i]);
 return 0;
}

