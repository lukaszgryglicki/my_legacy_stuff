#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define cf_file "/root/.mp3cprc"
/*#define LAME_OPTS "-m m -q 2 --abr "*/
#define LAME_OPTS "-q 2 --abr "
enum { F_MP3, F_WAV, F_OGG, F_UNKN };
char* mp3path;
char* temppath;
char* tempmp3;
char* tempwav;
char buffer[2047];
int bitr;
int cutnum;
int maxtries;
int downc;
/* in CF_FILE separato \n*/
/* maxlenInFN targetDir tempDir bitRate maxLenTargetFN maxSuffixNum downCaseBool */
/* for msdos filesystem (Mp3player): */
/* mount in /pen, target: /pen/, br: 64, 96, 128, maxLenTFN (with subs): 8 */
/* maxLenTFN=6 or 7, maxSurrixnum: 99 or 9, downCaseBool=1 */
/* for normal fs: example: /mp3 */
/* maxlenInFN=2000, targetDir: /mp3/ tempDir: /tmp/ bitRate = all */
/* maxlenTFN: 200-500, maxSuffixNum: 9999, downCaseBool: 0 */

void downcase(char* arg)
{
 int i,len;
 if (!arg) return;
 len = strlen(arg);
 for (i=0;i<len;i++)
    if (arg[i] >= 'A' && arg[i] <= 'Z') arg[i] += 0x20;
}

char* get_type_name(int ftype)
{
 switch(ftype)
 {
  case 0: return "mpeg audio layer 3"; break;
  case 1: return "microsoft wave stream"; break;
  case 2: return "ogg vorbis stream"; break;
  case 3: return "unknown/unidentyfied"; break;
 }
}

int get_file_type(char* fname)
{
 char* tail;
 char* ltail;
 int ftype;
 tail = &fname[strlen(fname) - 3];
 ltail = (char*)malloc((strlen(tail) + 1)*sizeof(char));
 strcpy(ltail, tail);
 downcase(ltail);
 if (!strcmp(ltail, "mp3")) ftype = F_MP3;
 else if (!strcmp(ltail, "wav")) ftype = F_WAV;
 else if (!strcmp(ltail, "ogg")) ftype = F_OGG;
 else ftype = F_UNKN;
 free(ltail);
 return ftype;
}

char* get_random_fn()
{
 char* ptr;
 int i, l;
 if (cutnum > 5) l = cutnum;
 else l = 5;
 ptr = (char*)malloc((l+1)*sizeof(char));
 ptr[l] = 0;
 for (i=0;i<l;i++) ptr[i] = 'a' + (rand() % ('z' - 'a'));
 return ptr;
}

int file_used(char** fname, int idx)
{
 char *path1, *path2, *path3;
 FILE* fp;
 int rval;
 if (downc) downcase(*fname);
 path1 = (char*)malloc((strlen(*fname)+16+strlen(mp3path))*sizeof(char));
 path2 = (char*)malloc((strlen(*fname)+16)*sizeof(char));
 if (idx == 0) sprintf(path2, "%s.mp3", *fname);
 else if (idx < maxtries) sprintf(path2, "%s%d.mp3", *fname, idx);
 else 
 {
     path3 = get_random_fn();
     sprintf(path2, "%s", path3);
     free((void*)path3);
     printf("Got random name: %s\n", path2);
     strcpy(*fname, path2);
 }
 strcpy(path1, mp3path);
 strcat(path1, path2);
/* printf("testing %s\n", path1);*/
 fp = fopen(path1, "r");
 if (fp) { fclose(fp); rval = 1; }
 else rval = 0;
/* printf("rval is %d\n", rval);*/
 free(path1);
 free(path2);
 return rval;
}

char* make_uname(char* fname)
{
 int i, len, idx, lsi, ulen;
 int last_dot, last_slash;
 char* result;
 char *nptr, *uname;
 len = strlen(fname);
 
 last_slash = -1;
 for (i=len-1;i>=0;i--)
 {
  if (fname[i] == '/') 
     {
      last_slash = i;
      break;
     }
 }
 
 last_dot = -1;
 lsi = (last_slash > 0) ? last_slash : 0;
 for (i=len-1;i>=lsi;i--)
 {
  if (fname[i] == '.') 
     {
      last_dot = i;
      break;
     }
 }
 
 result = (char*)malloc((strlen(fname)+8)*sizeof(char));
 strcpy(result, fname);
 if (last_dot > 0) result[last_dot] = 0;
 if (last_slash > 0) nptr = &result[last_slash+1];
 else nptr = result;
 ulen = (strlen(nptr)+1)*sizeof(char);
 if (ulen <= cutnum) ulen = cutnum + 1;
 uname = (char*)malloc(ulen);
 strcpy(uname, nptr);
 if (cutnum > 0)
 {
   if (strlen(uname) > cutnum) uname[cutnum] = 0;
 }
 free(result);
 idx = 0;
 while (file_used(&uname, idx)) idx ++;
 if (idx > 0)  
   {
     strcpy(buffer, uname);
     if (idx < maxtries) sprintf(uname, "%s%d", buffer, idx);
     printf("Final name: %s\n", uname);
   }
/* printf("uname: %s, %d\n", uname, idx);*/
 return uname;
}

void process_file(char* fname, int ftype)
{
 FILE* fp;
 char* outpath;
 char* uname;
 fp = fopen(fname, "r");
 if (!fp) 
 { 
  printf("File %s %s cannot be opened.\n", fname, get_type_name(ftype));
  return;
 }
 uname = make_uname(fname);
/* printf("uname = %s\n", uname);*/
 if (ftype == F_MP3)
 {
     	outpath = (char*)malloc((strlen(mp3path) + strlen(fname) + 4)*sizeof(char));
	strcpy(outpath, mp3path);
	strcat(outpath, uname);
	strcat(outpath, ".mp3");
/*	printf("uname = %s\n", uname);*/
	sprintf(buffer, "cp \"%s\" \"%s\"", fname, outpath);
	printf("%s\n", buffer);
	system(buffer);
 }
 if (ftype == F_WAV)
 {
     	outpath = (char*)malloc((strlen(mp3path) + strlen(fname) + 4)*sizeof(char));
	strcpy(outpath, mp3path);
	strcat(outpath, uname);
	strcat(outpath, ".mp3");
/*	printf("uname = %s\n", uname);*/
	sprintf(buffer, "lame %s %d \"%s\" \"%s\"", LAME_OPTS, bitr, fname, tempmp3);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "cp \"%s\" \"%s\"", tempmp3, outpath);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "rm -f \"%s\"", tempmp3);
	printf("%s\n", buffer);
	system(buffer);
 }
 if (ftype == F_OGG)
 {
     	outpath = (char*)malloc((strlen(mp3path) + strlen(fname) + 4)*sizeof(char));
	strcpy(outpath, mp3path);
	strcat(outpath, uname);
	strcat(outpath, ".mp3");
/*	printf("uname = %s\n", uname);*/
	sprintf(buffer, "oggdec \"%s\" -o \"%s\"", fname, tempwav);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "lame %s %d \"%s\" \"%s\"", LAME_OPTS, bitr, tempwav, tempmp3);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "cp \"%s\" \"%s\"", tempmp3, outpath);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "rm -f \"%s\" \"%s\"", tempmp3, tempwav);
	printf("%s\n", buffer);
	system(buffer);
 }
 if (ftype == F_UNKN)
 {
     printf("Don't know what to do with: %s\n", fname);
 }
}

void mp3cp(char* arg)
{
 int ftype;
 if (!arg || strlen(arg) < 4) return;
 printf("processing %s\n", arg);
 ftype = get_file_type(arg);
 printf("File type is: %s\n", get_type_name(ftype));
 process_file(arg, ftype);
}

void use_defaults()
{
 mp3path = (char*)malloc(255*sizeof(char));
 temppath = (char*)malloc(255*sizeof(char));
 tempmp3 = (char*)malloc(255*sizeof(char));
 tempwav = (char*)malloc(255*sizeof(char));
 strcpy(mp3path, "/mp3/");
 bitr = 192;
 strcpy(temppath, "/tmp/");
 strcpy(tempmp3, "/tmp/");
 strcpy(tempwav, "/tmp/");
 cutnum = 1000;
 maxtries = 999;
 downc = 0;
}

void display_config()
{
 printf("MP3 path is: %s\n", mp3path);
 printf("MP3 temporary path is: %s\n", temppath);
 printf("MP3 temporary file is: %s\n", tempmp3);
 printf("WAV temporary file is: %s\n", tempwav);
 printf("BitRate is: %d\n", bitr);
}

void read_config()
{
 FILE* fp;
 int len, res;
 fp = fopen(cf_file, "r");
 if (!fp)
   {
    printf("cannot read config file %s\ndefaults used\n", cf_file);
    use_defaults();
    display_config();
    return;
   }
 res = fscanf(fp, "%d\n", &len);
 if (res < 1 || len < 0) 
 {
     printf("error [1] reading config from: %s\n", cf_file);
     len = 0xFF;
 }
 mp3path = (char*)malloc(len * sizeof(char));
 temppath = (char*)malloc(len * sizeof(char));
 tempmp3 = (char*)malloc(len * sizeof(char));
 tempwav = (char*)malloc(len * sizeof(char));
 res = fscanf(fp, "%s\n", mp3path);
/* printf("res = %d\n", res);*/
 if (res < 1) 
 {
     printf("error [2] reading config from: %s\n", cf_file);
     strcpy(mp3path, "/mp3/");
 }
 res = fscanf(fp, "%s\n", temppath);
/* printf("res = %d\n", res);*/
 if (res < 1) 
 {
     printf("error [3] reading config from: %s\n", cf_file);
     strcpy(temppath, "/tmp/");
 }
 strcpy(tempmp3, temppath);
 strcpy(tempwav, temppath);
 strcat(tempmp3, "temp.mp3");
 strcat(tempwav, "temp.wav");
 res = fscanf(fp, "%d\n", &bitr);
 if (res < 1 || bitr < 16) 
 {
     printf("error [4] reading config from: %s\n", cf_file);
     bitr = 64;
 }
 res = fscanf(fp, "%d\n", &cutnum);
 if (res < 1) 
 {
     printf("error [5] reading config from: %s\n", cf_file);
     cutnum = 0;
 }
 res = fscanf(fp, "%d\n", &maxtries);
 if (res < 1) 
 {
     printf("error [6] reading config from: %s\n", cf_file);
     maxtries = 999;
 }
 res = fscanf(fp, "%d\n", &downc);
 if (res < 1) 
 {
     printf("error [6] reading config from: %s\n", cf_file);
     downc = 0;
 }
 fclose(fp);
 display_config();
}

void try_write()
{
 FILE* fp;
 char* pt;
 pt = (char*)malloc((strlen(mp3path) + 16)*sizeof(char));
 strcpy(pt, mp3path);
 strcat(pt, "test");
 printf("test writing to file: %s\n", pt);
 fp = fopen(pt, "w");
 if (!fp)
 {
     printf("write to: %s failed, exiting\n", pt);
     exit(2);
 }
 sprintf(buffer, "rm -f %s", pt);
 system(buffer);
 fclose(fp);
 printf("successfull write to: %s\n", pt);
 strcpy(pt, temppath);
 strcat(pt, "test");
 printf("test writing to file: %s\n", pt);
 fp = fopen(pt, "w");
 if (!fp)
 {
     printf("write to: %s failed, exiting\n", pt);
     exit(3);
 }
 sprintf(buffer, "rm -f %s", pt);
 system(buffer);
 fclose(fp);
 printf("successfull write to: %s\n", pt);
 free(pt);
}

int main(int lb, char** par)
{
 int i;
 time_t tm;
 time(&tm);
 srand((int)tm);
 if (lb < 2) { printf("argument(s) required\n"); return 1; }
 read_config();
 try_write();
 for (i=1;i<lb;i++)
     mp3cp(par[i]);
 return 0;
}

