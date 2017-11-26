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

void downcase(char* code)
{
 int len,i;
 printf("downcasing(%s) = ", code);
 len = strlen(code);
 for (i=0;i<len;i++) if (code[i]>='A' && code[i]<='Z') code[i] += 0x20;
 printf("%s\n", code);
}

int isHexDigit(char dgt)
{
 printf("isHexDigit(%c)\n", dgt);	
 if ((dgt>='0' && dgt<='9') || 
     (dgt>='A' && dgt<='F') || 
     (dgt>='a' && dgt<='f')) return 1;
 else return 0;
}

int isLetter(int zn, int encode)	/* on decoding we decode spaces */
{
 printf("isLetter(%d,%d)\n", zn, encode);
 if ((zn>='A' && zn<='Z') || 
     (zn>='a' && zn<='z') || (!encode && (zn==' ' || zn=='\n'))) return 1;
 else return 0;
}


int find_hash(char zn)
{
 int i;
 printf("findHash(%d:%c)\n", zn, zn);
 for (i=0;i<37;i++) if (alphabet[i]==zn) return i;
 return -1;
}

int encode(int zn, char* code, int curr, int direction)
{
 int idx;
 unsigned int offset;
 char str[4];
 printf("encode(%d:%c, %s, %d, %d)\n", zn,zn,code,curr,direction);
 idx = find_hash(zn);
 if (idx<0) 
  {
   printf("hash not found. trying downcased hash...\n");
   idx = find_hash(zn+0x20);
  }
 if (idx<0) 
   {
    printf("panic: no hash for (%d:%c)\n", zn,zn);
    return '*';
   }
 sprintf(str, "%c", code[curr]);
 if (sscanf(str,"%x", &offset)!=1) return '*';
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
 printf("hash index of (%d:%c) is %d encoded to --> %c\n", zn,zn,idx,alphabet[idx]);
 return alphabet[idx];
}

int jvcode(char tp, char* infile, char* outfile, char* hxcode)
{
 unsigned int hcode;
 int i,len,direction;
 FILE* in, *out;
 int zn;
 int current;
 printf("jvcode(%c, %s, %s, %s)\n", tp, infile, outfile, hxcode);
 len = strlen(hxcode);
 downcase(hxcode);
 for (i=0;i<len;i++) if (!isHexDigit(hxcode[i])) 
   {
    printf("Non hexadecimal digit: '%c'\n", hxcode[i]);
    return 1;
   }
 if (sscanf(hxcode,"%x", &hcode)!=1) 
   {
    printf("Cannot scanf hexcode.\n");
    return 2;
   }
 printf("hexcode is: (%d)0x%x\n", len,hcode);
 in = fopen(infile, "r");
 if (!in)
   {
    printf("File doesn't exists or cannot be read: %s\n", infile);
    return 4;
   }
 printf("inputfile: %s: opened successfully.\n", infile);
 out = fopen(outfile, "w");
 if (!out)
   {
    printf("Cannot write to file: %s\n", outfile);
    fclose(in);
    return 4;
   }
 printf("outputfile: %s: opened successfully.\n", infile);
 current = 0;
 direction = (tp=='c')?1:0;
 printf("CODE start.\n");
 while ((zn=fgetc(in))!=EOF)
   {
    if (!isLetter(zn, direction)) continue;
    zn = encode(zn, hxcode, current, direction);
    current++;
    if (current==len) current=0;
    fputc(zn, out);
   }
 printf("CODE end.\n");
 fclose(out);
 fclose(in);
 return 0;
}

int main(int lb, char** par)
{
 if (lb<5)
   {
    printf("Usage: %s [c|d] inputfile outputfile hexcode\n", par[0]);
    return 1;
   }
 return jvcode(par[1][0], par[2], par[3], par[4]);
}

