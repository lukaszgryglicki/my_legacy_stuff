#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cf_file "/usr/home/morgoth/.wavcprc"
enum { F_MP3, F_WAV, F_OGG, F_UNKN };
char* mp3path;
char* temppath;
char* tempmp3;
char* tempwav;
char buffer[2047];

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

int file_used(char* fname, int idx)
{
 char *path1, *path2;
 FILE* fp;
 int rval;
 path1 = (char*)malloc((strlen(fname)+16+strlen(mp3path))*sizeof(char));
 path2 = (char*)malloc((strlen(fname)+16)*sizeof(char));
 if (idx == 0) sprintf(path2, "%s.mp3", fname);
 else  sprintf(path2, "%s%d.mp3", fname, idx);
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
 int i, len, idx, lsi;
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
 uname = (char*)malloc((strlen(nptr)+1)*sizeof(char));
 strcpy(uname, nptr);
 free(result);
 idx = 0;
 while (file_used(uname, idx)) idx ++;
 if (idx > 0 )  
   {
     strcpy(buffer, uname);
     sprintf(uname, "%s%d", buffer, idx);
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
	strcat(outpath, ".wav");
/*	printf("uname = %s\n", uname);*/
	sprintf(buffer, "lame --decode \"%s\" -o \"%s\"", fname, tempmp3);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "cp \"%s\" \"%s\"", tempmp3, outpath);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "rm -f \"%s\"", tempmp3);
	printf("%s\n", buffer);
	system(buffer);
 }
 if (ftype == F_WAV)
 {
     	outpath = (char*)malloc((strlen(mp3path) + strlen(fname) + 4)*sizeof(char));
	strcpy(outpath, mp3path);
	strcat(outpath, uname);
	strcat(outpath, ".wav");
	sprintf(buffer, "cp \"%s\" \"%s\"", fname, outpath);
	printf("%s\n", buffer);
	system(buffer);
 }
 if (ftype == F_OGG)
 {
     	outpath = (char*)malloc((strlen(mp3path) + strlen(fname) + 4)*sizeof(char));
	strcpy(outpath, mp3path);
	strcat(outpath, uname);
	strcat(outpath, ".wav");
/*	printf("uname = %s\n", uname);*/
	sprintf(buffer, "oggdec \"%s\" -o \"%s\"", fname, tempmp3);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "cp \"%s\" \"%s\"", tempmp3, outpath);
	printf("%s\n", buffer);
	system(buffer);
	sprintf(buffer, "rm -f \"%s\"", tempmp3);
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
 strcpy(temppath, "/tmp/");
 strcpy(tempmp3, "/tmp/");
 strcpy(tempwav, "/tmp/");
}

void display_config()
{
 printf("MP3 path is: %s\n", mp3path);
 printf("MP3 temporary path is: %s\n", temppath);
 printf("MP3 temporary file is: %s\n", tempmp3);
 printf("WAV temporary file is: %s\n", tempwav);
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
 strcat(tempmp3, "temp.wav");
 strcat(tempwav, "temp.wav");
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
 if (lb < 2) { printf("argument(s) required\n"); return 1; }
 read_config();
 try_write();
 for (i=1;i<lb;i++)
     mp3cp(par[i]);
 return 0;
}

