#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char alphabet[37] = {
 'a','A','b','c','C','d','e','E','f','g',
 'h','i','j','k','l','L','m','n','\n','N',
 'o', 'O','p','q','r','s','S','t','u','v',
 'w','x','y','z','X','Z',' '
};
int ok=1;
int loss=1;

void downcase(char* code)
{
 int len,i;
 len = strlen(code);
 for (i=0;i<len;i++) if (code[i]>='A' && code[i]<='Z') code[i] += 0x20;
}


int isHexDigit(char dgt)
{
 if ((dgt>='0' && dgt<='9') ||
     (dgt>='A' && dgt<='F') ||
     (dgt>='a' && dgt<='f')) return 1;
 else return 0;
}


int isLetter(int zn, int encode)	/* on decoding we decode spaces */
{
 if ((zn>='A' && zn<='Z') ||
     (zn>='a' && zn<='z') || (!encode && (zn==' ' || zn=='\n'))) return 1;
 else return 0;
}


int find_hash(char zn)
{
 int i;
 for (i=0;i<37;i++) if (alphabet[i]==zn) return i;
 return -1;
}


int encodefpass1(int zn, FILE* file, int direction)
{
 int idx;
 int ch;
 unsigned int offset;
 char str[4];
 idx = find_hash(zn);
 if (idx<0) idx = find_hash(zn+0x20);
 if (idx<0) { ok = 0; return '*'; }
 ch = fgetc(file);
 if (ch==EOF)
   {
    fseek(file, 0, SEEK_SET);
    ch = fgetc(file);
    if (ch==EOF)
      {
       printf("PANIC: empty code file!\n");
       return -1;
      }
   }
 sprintf(str, "%c", ch);
 if (sscanf(str,"%x", &offset)!=1) offset = 0xC;
 if (direction==1)
   {
    idx += offset;
    if (idx>=37) idx-=37;
   }
 else
   {
    idx -= offset;
    if (idx<0) idx+=37;
   }
 return alphabet[idx];
}


int encode(int zn, char* code, int curr, int direction)
{
 int idx;
 unsigned int offset;
 char str[4];
 idx = find_hash(zn);
 if (idx<0) idx = find_hash(zn+0x20);
 if (idx<0) { return '*'; ok = 0; }
 sprintf(str, "%c", code[curr]);
 if (sscanf(str,"%x", &offset)!=1) { ok = 0; return '*'; }
 if (direction==1)
   {
    idx += offset;
    if (idx>=37) idx-=37;
   }
 else
   {
    idx -= offset;
    if (idx<0) idx+=37;
   }
 return alphabet[idx];
}


int jvcode(char tp, char* infile, char* outfile, char* hxcode)
{
 unsigned int hcode;
 int i,len,direction;
 FILE* in, *out;
 FILE* cf;
 int zn;
 int current;
 int cfile;
 cf = NULL;
 cfile = 0;
 len = strlen(hxcode);
 if (hxcode[0]=='-' && strlen(hxcode)>=2) cfile = 1;
 if (!cfile)
   {
    downcase(hxcode);
    for (i=0;i<len;i++) if (!isHexDigit(hxcode[i]))
      {
       printf("Non hexadecimal digit: '%c'\n", hxcode[i]);
       return 1;
      }
   }
 else
   {
    cf = fopen(hxcode+1, "r");
    if (!cf)
      {
       printf("Code file \"%s\" cannot be opened\n", hxcode+1);
       return 1;
      }
    printf("File \"%s\" will be used as code matrix\n", hxcode+1);
   }
 if (!cfile)
   {
    if (sscanf(hxcode,"%x", &hcode)!=1)
      {
       printf("Cannot scanf hexcode.\n");
       return 2;
      }
    printf("hexcode is: (%d)0x%x\n", len,hcode);
   }
 in = fopen(infile, "r");
 if (!in)
   {
    printf("File doesn't exists or cannot be read: %s\n", infile);
    return 4;
   }
 out = fopen(outfile, "w");
 if (!out)
   {
    printf("Cannot write to file: %s\n", outfile);
    fclose(in);
    return 4;
   }
 current = 0;
 direction = (tp=='c')?1:0;
 while ((zn=fgetc(in))!=EOF)
   {
    if (!isLetter(zn, direction)) { loss=1; continue; }
    if (!cfile)
      {
       zn = encode(zn, hxcode, current, direction);
       current++;
       if (current==len) current=0;
      }
    else
      {
       zn = encodefpass1(zn, cf, direction);
       if (zn == -1)
         {
          fclose(out);
          fclose(in);
	  fclose(cf);
          return 1;
         }
      }
    fputc(zn, out);
   }
 fclose(out);
 fclose(in);
 if (cfile) fclose(cf);
 if (!ok) printf("encoding was ambiguous...\ninput file was bad or codefile has inproper format for pass1\n");
 if (loss) printf("some data was skipped; inputfile was incompatible with codec\nthis is not a fatal error, only few files are full comaptible\n");
 printf("remember that this is loss coding;\ndecoded shold be readable for human\nevery non-text information will be lost\n");
 return 0;
}


int main(int lb, char** par)
{
 if (lb<5)
   {
    printf("Usage: %s [c|d] inputfile outputfile hexcode\n", par[0]);
    printf("hexcode can be also -filename \n");
    return 1;
   }
 return jvcode(par[1][0], par[2], par[3], par[4]);
}

