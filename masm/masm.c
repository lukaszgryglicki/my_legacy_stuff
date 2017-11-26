#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static FILE* out = 0;
static char* aflags;
static int strip = 0;
static int lbl_cnt=0;

char* readline(FILE* stream, int* eof)
{
 int sp;
 int zn;
 int cnt;
 char* ret;
 sp = ftell(stream);
 cnt = 0;
 *eof= 0;
 while (1)
 {
  zn = fgetc(stream);
  cnt++;
  if (zn=='\n') break;
  if (zn==EOF)  { *eof=1; break; }
 }
 ret = (char*)malloc(cnt+1);
 fseek(stream, sp, SEEK_SET);
 for (zn=0;zn<cnt;zn++) *(ret+zn) = fgetc(stream);
 *(ret+cnt-1) = 0;
 if (!strcmp(ret,"")) { free(ret); return 0; }
 return ret;
}

int is_blank(char arg)
{
 if (arg==' ' || arg=='\n' || arg=='\t') return 1;
 else return 0;
}

int is_downletter(char arg)
{
 if (arg>='a' && arg<='z') return 1;
 else return 0;
}

int is_digit(char arg)
{
 if ((arg>='0' && arg<='9') || (arg>='A' && arg<='F')) return 1;
 else return 0;
}

int is_operator(char arg)
{
 if (arg=='-' || arg=='.' || arg=='_') return 1;
 else return 0;
}

int is_comment(char arg)
{
 if (arg=='#' || arg==';') return 1;
 else return 0;
}

int is_upletter(char arg)
{
 if (arg>='A' && arg<='Z') return 1;
 else return 0;
}

int is_oksymbol(char arg)
{
 if (is_upletter(arg) || is_digit(arg) || is_operator(arg)) return 1;
 else return 0;
}

char* parse(char* arg)
{
 int i;
 int start;
 char* start_pos;
 char* end_pos;
 char* ret;
 int cnt;
 start=1;
 start_pos = arg;
 end_pos   = arg + strlen(arg) - 1;
 for (i=0;i<strlen(arg);i++)
   {
    if (is_comment(*(arg+i))) { *(arg+i) = 0; break; }
    if (is_downletter(*(arg+i))) *(arg+i) -= 0X20;
    if (!is_oksymbol(*(arg+i))) *(arg+i)   = 0X20;
   }
 for (i=0;i<strlen(arg);i++) 
   {
    if (start && is_blank(*(arg+i))) start_pos++;
    if (!is_blank(*(arg+i))) start = 0;
   }
 start = 1;
 for (i=strlen(arg)-1;i>=0;i--) if (!is_blank(*(arg+i))) break;
 *(arg+i+1) = 0;
 if (!strcmp(start_pos,"")) return 0;
 start=0;
 cnt=0;
 for (i=0;i<strlen(start_pos);i++)
 {
  if (is_blank(*(start_pos+i)) && start) continue;
  if (is_blank(*(start_pos+i)) && !start) start=1;
  if (!is_blank(*(start_pos+i))) start=0;
  cnt++;
 }
 ret = (char*)malloc(cnt+1);
 start = 0;
 cnt = 0;
 for (i=0;i<strlen(start_pos);i++)
 {
  if (is_blank(*(start_pos+i)) && start) continue;
  if (is_blank(*(start_pos+i)) && !start) start=1;
  if (!is_blank(*(start_pos+i))) start=0;
  *(ret+cnt) = *(start_pos+i);
  cnt++;
 }
 *(ret+cnt) = 0;
 return ret;
}

int get_wordscnt(char* str)
{
 int i;
 int wc;
 wc = 0;
 for (i=0;i<strlen(str);i++)
 {
  if (*(str+i) == 0X20) wc++;
 }
 return wc+1;
}

char** get_cmd(char* str, int c)
{
 char** ret;
 int i;
 int current;
 int oldlen;
 current=0;
 ret = (char**)malloc(c<<2);
 *ret = str;
 oldlen = strlen(str);
 for (i=0;i<oldlen;i++)
 { 
  if (*(str+i) == 0X20)
    {
     current++;
     *(str+i) = 0;
     *(ret+current) = str+i+1;
    }
  }
 return ret;
}

void abrt(char* why)
{
 printf("%s\n", why);
 fclose(out);
 free(aflags);
 exit(1);
}

void masm_start()
{
 fprintf(out,
	".globl _start\n"
	"_start:\n"
	);
}

void masm_ret()
{
 fprintf(out,
	"\tret\n"
	);
}

void masm_ret2(char* arg)
{
 fprintf(out,
	"\tmovl $%s, %%eax\n"
	"\tret\n"
	,arg);
}

void masm_label(char* arg)
{
 fprintf(out,
	"%s:\n"
	,arg);
}

char* next_label()
{
 char* ret;
 ret = malloc(0X10);
 sprintf(ret, "L%X", lbl_cnt);
 lbl_cnt++;
 return ret;
}

void masm_exit(char* arg)
{
 fprintf(out,
	"\tmovl $0x1, %%eax\n"
	"\tpushl $%s\n"
	"\tpushl $0xdead\n"
	"\tint $0x80\n"
	,arg);
}

void execute_cmd(char** cmd, int c)
{
 char* tmps;
 if (!strcmp(cmd[0], "START"))              masm_start(); 
 else if (!strcmp(cmd[0], "RET") && c==1)   masm_ret(); 
 else if (!strcmp(cmd[0], "RET") && c>1)    masm_ret2(cmd[1]); 
 else if (!strcmp(cmd[0], "EXIT") && c==1)  masm_exit("0"); 
 else if (!strcmp(cmd[0], "EXIT") && c>=2)  masm_exit(cmd[1]); 
 else if (!strcmp(cmd[0], "LABEL") && c>=2) masm_label(cmd[1]); 
 else if (!strcmp(cmd[0], "LABEL") && c==1) 
   {
    tmps = next_label();
    masm_label(tmps); 
    free(tmps);
   }
 else abrt("UNSUPPORTED INSTRUCTION!");
}

void execute(char* str)
{
 int nword;
 char** cmd;
 printf("\"%s\"\n", str);
 nword = get_wordscnt(str);
 cmd   = get_cmd(str, nword);
 execute_cmd(cmd, nword);
 free(cmd);
}

int open_globalout()
{
 out = fopen("masm_out.s", "w");
 if (!out) return 1;
 else return 0;
}

void go(char* fn)
{
 FILE* file;
 int eof;
 int err;
 char* line;
 char* parsed;
 char* cmd_line;
 printf("ASSEMBLING: %s\n", fn);
 file = fopen(fn, "r");
 eof = 0;
 if (!file) { printf("FILE %s dont exists or cannot be read\n", fn); return; }
 err = open_globalout();
 if (err) { printf("open_globalout() failed\n"); return; }
 while (!eof)
 {
  line = readline(file, &eof);
  if (line)
   {
    parsed = parse(line);
    if (parsed) 
      {
       execute(parsed);
       free(parsed);
      }
    free(line);
   }
 }
 fclose(file);
 fclose(out);
 cmd_line = (char*)malloc(0X140);
 sprintf(cmd_line,"as %s -o masm_out.o masm_out.s", aflags);
 system(cmd_line);
 printf("%s\n",cmd_line);
 free(cmd_line);
 system("ld -e _start -o a.out masm_out.o");
 system("cat masm_out.s");
 system("rm -f masm_out.s masm_out.o");
 if (strip) system("strip -s a.out");
 printf("ASSEMBLY OK\n");
}

void parse_args(char* args)
{
 int i;
 for (i=0;i<strlen(args);i++) 
 {
  if (*(args+i) == 'd' || *(args+i) == 'D') strcat(aflags, " --gstabs "); 
  if (*(args+i) == 's' || *(args+i) == 'S') strip = 1;
 }
}

int main(int lb, char** par)
{
 char* fn;
 if (lb<2) 
  {
   printf("ASM IN> ");
   fn = (char*)malloc(0X100);
   fgets(fn, 0XFF, stdin);
  }
 else fn = par[1];
 aflags = (char*)malloc(0X100);
 strcpy(aflags, "");
 if (lb>=3) parse_args(par[2]);
 go(fn);
 free(aflags);
 if (lb<2) free(fn);
 return 0;
}

