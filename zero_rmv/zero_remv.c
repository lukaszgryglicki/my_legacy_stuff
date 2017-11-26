#include <mh.h>

int remv(char* inn, char* outn)
{
 FILE* in, *out;
 int zn;
 if (!(in=fopen(inn,"r"))) { printf("read error: %s\n", inn); return 1;}
 if (out=fopen(outn,"r")) 
   {
    printf("exists: %s\noverwrite? ", outn);
    zn = getch();
    if (zn>='A' && zn<='Z') zn += 0x20;
    fclose(out);
    if (zn!='y') {fclose(in); printf("not confirmed\n"); return 2; }
   }
 if (!(out=fopen(outn,"w"))) { fclose(in); printf("write error: %s\n", outn); return 4;}
 while ((zn=fgetc(in))!=EOF) if (zn) fputc(zn, out);
 fclose(out);
 fclose(in);
}

int main(int lb, char** arg)
{
 if (lb!=3) { printf("%s f1 f2 [removes \\0 in f1]\n\n", arg[0]); return 1; }
 remv(arg[1], arg[2]);
}

