#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define KEY_A 0
#define KEY_B 1
#define KEY_C 2
#define KEY_D 3
#define KEY_E 4
#define KEY_F 5
#define KEY_G 6
#define KEY_H 7
#define KEY_I 8
#define KEY_J 9
#define KEY_K 10
#define KEY_L 11
#define KEY_M 12
#define KEY_N 13
#define KEY_O 14
#define KEY_P 15
#define KEY_Q 16
#define KEY_R 17
#define KEY_S 18
#define KEY_T 19
#define KEY_U 20
#define KEY_V 21
#define KEY_W 22
#define KEY_X 23
#define KEY_Y 24
#define KEY_Z 25
#define KEY__ 26
#define KEY_0 27
#define KEY_1 28
#define KEY_2 29
#define KEY_3 30
#define KEY_4 31
#define KEY_5 32
#define KEY_6 33
#define KEY_7 34
#define KEY_8 35
#define KEY_9 36
#define NLETTERS 26

int downcase(int zn)
{
 if (zn >='A' && zn <='Z') zn += 0x20;
 return zn;
}

int transform(char* str, int* table)
{
 int lower,i;
 for (i=0;i<(int)strlen(str);i++)
        {
	 lower = downcase(str[i]);
         switch(lower)
                {
                 case '0': table[i] = 0x0; break;
                 case '1': table[i] = 0x1; break;
                 case '2': table[i] = 0x2; break;
                 case '3': table[i] = 0x3; break;
                 case '4': table[i] = 0x4; break;
                 case '5': table[i] = 0x5; break;
                 case '6': table[i] = 0x6; break;
                 case '7': table[i] = 0x7; break;
                 case '8': table[i] = 0x8; break;
                 case '9': table[i] = 0x9; break;
                 case 'a': table[i] = 0xa; break;
                 case 'b': table[i] = 0xb; break;
                 case 'c': table[i] = 0xc; break;
                 case 'd': table[i] = 0xd; break;
                 case 'e': table[i] = 0xe; break;
                 case 'f': table[i] = 0xf; break;
                 default: return 0;
                }
        }
 return 1;
}

int encodel(int zn, int inc)
{
 int bit_low;
 int bit_high;
 int ret;
 bit_high = bit_low = 0;
 ret = 0;
 if (zn >= 'a' && zn <= 'z')
   {
    bit_low = zn - 'a';
   }
 else if (zn >= 'A' && zn <= 'Z')
   {
    bit_high = 1;
    bit_low = zn - 'A';
   }
 else if (zn >= '0' && zn <= '9')
   {
    bit_low = (zn-'0')+NLETTERS+1;
   }
 else if (zn == ' ')
   {
    bit_low = NLETTERS;
   }
 else return zn;
 bit_low += inc;
 if (bit_low > KEY_9) bit_low -= (KEY_9+1); 
 if (bit_high && (bit_low >= KEY_A && bit_low <= KEY_Z)) return bit_low+'A';
 else if (!bit_high && (bit_low >= KEY_A && bit_low <= KEY_Z)) return bit_low+'a';
 else if (bit_low >= KEY_0 && bit_low <= KEY_9) return bit_low-(NLETTERS+1)+'0';
 else return (int)(' ');
}

int decodel(int zn, int inc)
{
 int bit_low;
 int bit_high;
 int ret;
 bit_high = bit_low = 0;
 ret = 0;
 if (zn >= 'a' && zn <= 'z')
   {
    bit_low = zn - 'a';
   }
 else if (zn >= 'A' && zn <= 'Z')
   {
    bit_high = 1;
    bit_low = zn - 'A';
   }
 else if (zn >= '0' && zn <= '9')
   {
    bit_low = (zn-'0')+NLETTERS+1;
   }
 else if (zn == ' ')
   {
    bit_low = NLETTERS;
   }
 else return zn;
 bit_low -= inc;
 if (bit_low < KEY_A) bit_low += (KEY_9+1); 
 if (bit_high && (bit_low >= KEY_A && bit_low <= KEY_Z)) return bit_low+'A';
 else if (!bit_high && (bit_low >= KEY_A && bit_low <= KEY_Z)) return bit_low+'a';
 else if (bit_low >= KEY_0 && bit_low <= KEY_9) return bit_low-(NLETTERS+1)+'0';
 else return (int)(' ');
}

void docode(char* fname, char* oname, char* hcode, int enc)
{
 FILE* in;
 FILE* out;
 int zn,index;
 int len = strlen(hcode);
 int* table = (int*)malloc(len*sizeof(int));
 if (!transform(hcode, table)) {printf("Code error.\n"); free(table); return;}
 if ((in = fopen(fname, "r+")) == NULL)
        {
         printf("No such file: %s\n",fname);
         free(table);
         return;
        }
 if ((out = fopen(oname, "w+")) == NULL)
        {
         printf("Cannot create: %s\n",oname);
         free(table);
         fclose(in);
         return;
        }
 
 index = 0;
 while ((zn = fgetc(in)) != EOF)
        {
          if (enc) zn = encodel(zn, table[index]);
          else zn = decodel(zn, table[index]);
          index++;
          if (index >= len) index -= len;
          fprintf(out,"%c",zn);
        }
 free(table);
 fclose(in);
 fclose(out);
}
int main(int lb, char** par)
{
  int encode = 1;
  if (lb != 5) {printf("Usage:\n\tcode [-d | -e] [filein] [fileout] [hexcode]\n");return -1;}
  if (!strcmp(par[1],"-d")) encode = 0;
  docode(par[2], par[3], par[4], encode);
  return 0;
}
