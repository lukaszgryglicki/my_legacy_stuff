#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLANK '\n'

char alphabet[36] = {
 'a','A','b','c','C','d','e','E','f','g',
 'h','i','j','k','l','L','m','n','N','o',
 'O','p','q','r','s','S','t','u','v','w',
 'x','y','z','X','Z',BLANK /* when encoding to space, input spaces are skipped*/
};

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
     (zn>='a' && zn<='z') || (!encode && zn==BLANK)) return 1;
 else return 0;
}


int find_hash(char zn)
{
 int i;
 for (i=0;i<36;i++) if (alphabet[i]==zn) return i;
 return -1;
}

int encode(int zn, char* code, int curr, int direction)
{
 int idx;
 unsigned int offset;
 char str[4];
 idx = find_hash(zn);
 if (idx<0) idx = find_hash(zn+0x20);
 if (idx<0) return '*';
 sprintf(str, "%c", code[curr]);
 if (sscanf(str,"%x", &offset)!=1) return '*';
 if (direction==1)
   {
    idx += offset;
    if (idx>=36) idx-=36;
   }
 else
   {
    idx -= offset;
    if (idx<0) idx+=36;
   }
 return alphabet[idx];
}

int jvcode(char tp, char* infile, char* outfile, char* hxcode)
{
 unsigned int hcode;
 int i,len,direction;
 FILE* in, *out;
 int zn;
 int current;
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
    if (!isLetter(zn, direction)) continue;
    zn = encode(zn, hxcode, current, direction);
    current++;
    if (current==len) current=0;
    fputc(zn, out);
   }
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

