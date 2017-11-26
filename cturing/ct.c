#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define INITIAL 2
#define ACCEPT  1
#define REFUSE  0
#define SAFE_MUL  11
#define SAFE_BORDER 6
#define BLANK (letter)0
#define CR    (letter)10
#define LF    (letter)13
#define word   unsigned int
#define sword  signed int
#define letter char
/* define it or undefine, set default speed*/
#define STEPPING  0
#ifdef STEPPING
int step = STEPPING;
#endif
/*extern int unlink(char*);*/
/*extern int strlen(const char*);*/
/*extern int strcpy(char*, const char*);*/
/*extern int strcat(char*, const char*);*/

struct ctstate
{
 word st1,st2;
 letter lt1,lt2;
 letter mov;
};

struct ctmachine
{
 word state;
 sword where;
 letter* tape;
 letter* alpha;
 word a_len;
 word t_len;
 word nmatrix;
 word pc;
 struct ctstate* matrix;
};

void machine_init(struct ctmachine* mach)
{
 (*mach).state = INITIAL;
 (*mach).where = 0;
 (*mach).tape = NULL;
 (*mach).alpha = NULL;
 (*mach).a_len = 0;
 (*mach).t_len = 0;
 (*mach).matrix = NULL;
 (*mach).nmatrix = 0;
 (*mach).pc = 0;
}


void print_machine_state(struct ctmachine* mach)
{
 if ((*mach).state==ACCEPT)      printf(" <ACC,");
 else if ((*mach).state==REFUSE) printf(" <REF,");
 else                            printf(" <q%u,", (*mach).state);
 if ( *((*mach).tape+(*mach).where) == BLANK) printf("BLANK>");
 else printf("%c>", *((*mach).tape+(*mach).where));
}


int print_machine_tape(struct ctmachine* mach)
{
 int i;
 printf("TCPU(x%x,%d):\n%s\n", (*mach).pc, (*mach).where, (*mach).tape);
 if ((*mach).where>=(int)(*mach).t_len*SAFE_MUL-1 || (*mach).where < (sword)(-3))
   {
    printf("tape under/overflow\n");
    return 2;
   }
 for (i=0;i<(*mach).where;i++) printf(" ");
 printf("^");
 print_machine_state(mach);
 printf("\n");
 return 0;
}


int execute_update(struct ctmachine* mach)
{
 word i;
 int find=0;
 letter c_letter = *((*mach).tape+(*mach).where);
 word   c_state  = (*mach).state;
 if (c_letter==BLANK) c_letter = '#';
 if (c_letter==CR) c_letter = '#';
 if (c_letter==LF) c_letter = '#';
 if ((*mach).where<0) c_letter = '!';
 for (i=0;i<(*mach).nmatrix;i++)
 {
  if ((*((*mach).matrix+i)).st1 == c_state && (*((*mach).matrix+i)).lt1 == c_letter)
  {
   (*mach).state = (*((*mach).matrix+i)).st2;
   *((*mach).tape+(*mach).where) = (*((*mach).matrix+i)).lt2;
   if (*((*mach).tape+(*mach).where) == '#') *((*mach).tape+(*mach).where) = BLANK;
   if ((*((*mach).matrix+i)).mov == 'r') (*mach).where++;
   else if ((*((*mach).matrix+i)).mov == 'l') (*mach).where--;
   else if ((*((*mach).matrix+i)).mov == 's') ;
   else { printf("bad move directive\n"); return 2; }
   if ((*mach).state == ACCEPT || (*mach).state == REFUSE)
   {
	 printf("machine stopped, language accepted: %d\n", (*mach).state);
	 return 0;
   }
   find = 1;
  }
 }
 if (!find) 
   {
    printf("cannot find matching state transformation"
		 " for current event <%d(%c), q%d>\n",c_letter, c_letter, c_state);
    printf("This could mean REJECT if STOP condiction set in CT\n");
   }
 return !find;
}


int machine_run(struct ctmachine* mach)
{
 int err;
#ifdef STEPPING
 int i;
#endif
 if ((*mach).state != INITIAL)
   {
    printf("machine state isn't initial, aborted\n");
    return 1;
   }
 while ((*mach).state != ACCEPT && (*mach).state != REFUSE)
 {
  err = print_machine_tape(mach);
  if (err) printf("tape over/under flow warning\n");
  err = execute_update(mach);
  if ((*mach).where < -3 || (*mach).where >= (sword)((*mach).t_len*SAFE_MUL-1) )
    {
     printf("%d: too much overflows, cannot be accepted, exiting\n", (*mach).where);
     return 2;
    }
  if (err) { printf("execute/update error\n"); return err; }
  (*mach).pc++;
#ifdef STEPPING
   for (i=0;i<step;i++) ;
#endif
  if ((*mach).state < 2) break;
 }
printf("machine stopped, state is: %d\n", (*mach).state);
return 0;
}


void downcase(letter* l)
{
 if (*l>='A' && *l<='Z') *l += 0x20;
}

/* (state,letter,state,letter,move)
// state = 2 (INITIAL) 3,4.,5.. user    | 1 accept, 0 refuse
// letter A,B,C, a,b,c almost all user  | # blank, ! start of tape
// move L, R, 				| S (stop) accept or refuse */

int machine_read_matrix(struct ctmachine* mach, char* from)
{
 struct ctstate* cst = NULL;
 word i;
 int tmp;
 int k;
 letter c;
 word u;
 FILE* in= fopen(from,"r");
 printf("machine_read_matrix:\n");
 if (!in) { printf("cannot read file: %s\n", from); return 1; }
 printf("init memory ok\n");
 k=0;
 while (1)
 {
  tmp = fscanf(in, "(%u,%c,%u,%c,%c) ", &u, &c, &u, &c, &c);
  if (tmp != 5) break;
  else k++;
 }
 if (k<=0) { printf("no method table scanned\n"); return 4; }
 (*mach).nmatrix = k;
 printf("scanned number of ctmethods: %d\n", (*mach).nmatrix);
 cst = (struct ctstate*)malloc(sizeof(struct ctstate)*(*mach).nmatrix);
 if (!in || !cst) return 1;
 fseek(in, 0, SEEK_SET);
 for (i=0;i<(*mach).nmatrix;i++)
 {
  tmp = fscanf(in, "(%u,%c,%u,%c,%c) ",
		  &((*(cst+i)).st1), &((*(cst+i)).lt1),
		  &((*(cst+i)).st2), &((*(cst+i)).lt2),
		  &((*(cst+i)).mov));
  if (tmp!=5)
  {
	printf("MATRIX rule error, scanned: %d on %d element\n", tmp, i);
    return 2;
  }
 }
 printf("GOT ruleset:\n");
 for (i=0;i<(*mach).nmatrix;i++)
 {
  downcase(&(*(cst+i)).mov);
  printf("(%u,%c,%u,%c,%c)\n",
		  (*(cst+i)).st1, (*(cst+i)).lt1, (*(cst+i)).st2, (*(cst+i)).lt2, (*(cst+i)).mov);
 }
 fclose(in);
 (*mach).matrix = cst;
 printf("methods set to CTCPU\n");
 return 0;
}


int machine_read_tape(struct ctmachine* mach, char* from)
{
 FILE* in;
 int i;
 int k;
 short* temp;
 in = fopen(from ,"r");
 if (!in) return 1;
 printf("tape file opened ok\n");
 fseek(in, 0, SEEK_END);
 (*mach).t_len = (word)ftell(in);
 printf("tape length is: %d\n", (*mach).t_len);
 if ((*mach).t_len<=0) { printf("cannot process empty file\n"); return 5; }
 fseek(in, 0, SEEK_SET);
 temp = (short*)malloc(0x200);
 if (!temp) return 3;
 (*mach).tape  = (letter*)malloc((*mach).t_len*SAFE_MUL+5);
 (*mach).tape += 5;
 if (!(*mach).tape)
   {
    printf("cannot alocate tape memory\n");
    fclose(in);
    return 1;
   }
 for (i=-3;i<(int)(*mach).t_len*SAFE_MUL;i++) *((*mach).tape+i) = BLANK;
 printf("tape allocated and blanked ok\nreading data.");
 for (i=0;i<0x100;i++) *(temp+i) = 0;
 for (i=0;i<(int)(*mach).t_len;i++)
 {
  fscanf(in, "%c", ((*mach).tape+i));
  printf(".");
  (*(temp+(*((*mach).tape+i))))++;
 }
 k = 0;
 for (i=0x20;i<0x80;i++) if (*(temp+i) != BLANK) k++;
 printf("\nalphabet length: %d\n", k);
 (*mach).alpha = (letter*)malloc(k+SAFE_BORDER+4);
 (*mach).a_len = k;
 if (!(*mach).alpha)
 {
  printf("init memory: alphabet failed\n");
  fclose(in);
  return 1;
 }
 for (i=0;i<(int)(*mach).a_len;i++) *((*mach).alpha+i) = BLANK;
 printf("init alphabet ok, blanked\n");
 k = 0;
 for (i=0x20;i<0x80;i++)
 {
  if (*(temp+i) != BLANK)
    {
     *((*mach).alpha+k) = i;
     k++;
    }
  if (k>(int)(*mach).a_len) break;
 }
 *((*mach).tape+(*mach).t_len) = BLANK;
 *((*mach).alpha+(*mach).a_len)= BLANK;
 printf("alphabet constructed\n");
 printf("SAFE_GUARD: allocated %d bytes for machine tape\n"
	    "SAFE_GUARD: allocated %d bytes for machine alphabet\n",
		 (*mach).t_len*SAFE_MUL+4, (*mach).a_len+SAFE_BORDER+4);
 printf("scanned tape image:\n%s\n", (*mach).tape);
 printf("scanned alphabet:  \n%s\n", (*mach).alpha);
 fclose(in);
 free(temp);
 printf("tape file closed, temporaries freed ok\n");
 return 0;
}


int is_machine_letter(struct ctmachine* mach, letter l)
{
 int i;
 int is=0;
 for (i=0;i<(int)(*mach).a_len;i++) if (l == *((*mach).alpha+i)) { is=1; break; }
 return is;
}


int check_machine(struct ctmachine* mach)
{
 int i;
 int diff;
 if ((*mach).state>=2) printf("machine state is bad: %d\n", (*mach).state);
 if ((*mach).where!=0) printf("tape isn't rewinded: %d\n", (*mach).where);
 diff=0;
 for (i=0;i<(int)(*mach).t_len*SAFE_MUL-1;i++)
 {
  if ( *((*mach).tape+i) == BLANK) break;
  if (!is_machine_letter(mach, *((*mach).tape+i))) diff=1;
	  /*printf("%c is not of initial alphabet\n", *((*mach).tape+i));*/
 }
 if (diff) printf("There are differences between alpabet in and out\n");
 printf("Machine did %d(0x%x) steps.\n", (*mach).pc, (*mach).pc);
 printf("result's length is: %d\n", i);
 return 0;
}


int machine_write_tape(struct ctmachine* mach, char* to, char* add)
{
 FILE* out = fopen(to,"r");
 int i;
 if (out)
 {
  if (!add)
   {
    printf("output file exists, add flag -f to override\n");
    fclose(out);
    return 1;
   }
  if (*add=='-' && (*(add+1)=='f' || *(add+1)=='F')) fclose(out);
  else { printf("did you mean -f ??\n"); return 2; }
 }
 out = fopen(to,"w");
 if (!to)
   {
    printf("cannot write to: %s, check perms\n", to);
    return 3;
   }
 for (i=0;i<(int)(*mach).t_len*SAFE_MUL-1;i++)
 {
  if ( *((*mach).tape+i) == BLANK) break;
  fprintf(out,"%c", *((*mach).tape+i));
 }
 fclose(out);
 printf("output file written\n");
 return 0;
}


void freem(struct ctmachine* mach)
{
 printf("freeying CTCPU memory\n");
 if ((*mach).tape)
 {
  (*mach).tape -= 5;
  free((*mach).tape);
 }
 if ((*mach).alpha)  free((*mach).alpha);
 if ((*mach).matrix) free((*mach).matrix);
 free(mach);
 printf("done\n");
}


int machine_stdin_tape(char* name)
{
 FILE* out;
 char* arg;
 out = fopen(name, "w");
 if (!out) { printf("cannot create temporary file: %s\n", name); return 1; }
 arg = (char*)malloc(0x1000);
 if (!arg) { printf("out of memory\n"); return 2; }
 printf("hit CR when done\n");
 printf("type> ");
 scanf("%s", arg);
 fprintf(out,"%s", arg);
 fclose(out);
 free(arg);
 printf("\n");
 return 0;
}

#ifdef STEPPING

void set_speed(char* arg)
{
 step = atoi(arg);
}

#endif

int run_machine_parametrized(char* pref, struct ctmachine* machine)
{
 char* tape;
 char* matrix;
 char* out;
 int err;
 tape   = (char*)malloc(strlen(pref)+4);
 matrix = (char*)malloc(strlen(pref)+8);
 out    = (char*)malloc(strlen(pref)+5);
 if (!tape || !matrix || !out)
   { printf("mem_alloc parametrized FAILED!\n"); return 6; }
 strcpy(tape,pref);
 strcpy(matrix,pref);
 strcpy(out,pref);
 strcat(tape,".in");
 strcat(matrix,".matrix");
 strcat(out,".out");
 machine = (struct ctmachine*)malloc(sizeof(struct ctmachine));
 printf("pointer allocated\n");
 machine_init(machine);
 printf("machine init ok\n");
 err = machine_read_tape(machine, tape);
 if (err)
   {
    printf("machine_read_tape:%s failed\n", tape);
	err = machine_stdin_tape(tape);
	if (err)
	{
     printf("machine create stdin tape, failed\n");
	 freem(machine);
	 free(tape);
	 free(out);
	 free(matrix);
	 return 5;
	}
	err = machine_read_tape(machine, tape);
	if (err)
	{
     printf("error reading tape date from stdin\n");
	 freem(machine);
	 free(tape);
	 free(out);
	 free(matrix);
	 return 2;
	}
	else
	{
	 printf("read tape from stdin ok\n");
	 unlink(tape);
	}
   }
 else printf("machine_read_tape ok\n");
 err = machine_read_matrix(machine, matrix);
 if (err)
   {
    printf("machine_read_matrix:%s failed\n", matrix);
    freem(machine);
	 free(tape);
	 free(out);
	 free(matrix);
    return 3;
   }
 else printf("machine_read _matrix ok\n");
 printf("RUN MACHINE>>\n\n");
 err = machine_run(machine);
 if (err)
   {
    printf("\nmachine_run: failed\n");
    freem(machine);
	 free(tape);
	 free(out);
	 free(matrix);
    return 4;
   }
 else printf("\nmachine_run ok\n");
 err = check_machine(machine);
 if (err) printf("warning: on output are symbols which wasn't on input\n");
 printf("writing output file\n");
 err = machine_write_tape(machine, out, "-f");
 if (err)
   {
    printf("machine_write_tape:%s failed\n", out);
    freem(machine);
	 free(tape);
	 free(out);
	 free(matrix);
    return 3;
   }
 printf("exiting...\n");
 freem(machine);
	 free(tape);
	 free(out);
	 free(matrix);
 return 0;
}


int main(int lb, char** par)
{
 int err;
 struct ctmachine *machine;
 char* prefix;
 printf("allocating pointer. . .\n");
 machine=0;
 if (lb<4)
    {
     if (lb==2)
       {
	printf("machine run parametrized.\n");
	prefix = (char*)malloc(strlen(*(par+1))+1);
	strcpy(prefix,*(par+1));
	err = run_machine_parametrized(prefix, machine);
	if (err)
	  printf("there was an error or warning running machine parametrized.\n");
	free(prefix);
	return 0;
       }
     else {
     printf("usage:\t%s tape.in matrix.dat tape.out [-f] [num]\n", *par);
	 printf("-f is optional (flag to overwrite output)\n");
	 printf("num stands for slowing down machine\nor %s tape_name\n",*par);
     return 1;
     }
    }
 machine = (struct ctmachine*)malloc(sizeof(struct ctmachine));
 printf("pointer allocated\n");
 machine_init(machine);
 printf("machine init ok\n");
#ifdef STEPPING
 if (lb>=6) set_speed(*(par+5));
#endif
 err = machine_read_tape(machine, *(par+1));
 if (err)
   {
    printf("machine_read_tape:%s failed\n", *(par+1));
	err = machine_stdin_tape(*(par+1));
	if (err)
	{
     printf("machine create stdin tape, failed\n");
	 freem(machine);
	 return 5;
	}
	err = machine_read_tape(machine, *(par+1));
	if (err)
	{
     printf("error reading tape date from stdin\n");
	 freem(machine);
	 return 2;
	}
	else
	{
	 printf("read tape from stdin ok\n");
	 unlink(*(par+1));
	}
   }
 else printf("machine_read_tape ok\n");
 err = machine_read_matrix(machine, *(par+2));
 if (err)
   {
    printf("machine_read_matrix:%s failed\n", *(par+2));
    freem(machine);
    return 3;
   }
 else printf("machine_read _matrix ok\n");
 printf("RUN MACHINE>>\n\n");
 err = machine_run(machine);
 if (err)
   {
    printf("\nmachine_run: failed\n");
    freem(machine);
    return 4;
   }
 else printf("\nmachine_run ok\n");
 err = check_machine(machine);
 if (err) printf("warning: on output are symbols which wasn't on input\n");
 printf("writing output file\n");
 err = machine_write_tape(machine, *(par+3), (lb>=5)?(*(par+4)):NULL);
 if (err)
   {
    printf("machine_write_tape:%s failed\n", *(par+3));
    freem(machine);
    return 3;
   }
 printf("exiting...\n");
 freem(machine);
 return 0;
}
/*all written in clean ANSI C */
