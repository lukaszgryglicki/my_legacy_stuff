#include <mh.h>


#define false 0
#define true  1

static int debug = 0; //set this to 1 for DEBUG
static int min = false;
static int max = false;
static double min_v = 0.0;
static double max_v = 0.0;


void compile_f(char* name)
{
 if (min) printf("min=%f\n",(double)min_v);
 if (max) printf("max=%f\n",(double)max_v);
 FILE* head = fopen("function.h","w");
 if (!head) {printf("fatal: cannot open function.h to write!\n"); return;}
 if ((!max) && (!min))
 	{
 	 fprintf(head,"#define DEBUG %d\n\n", debug);
 	 fprintf(head,"double function(double x, double y)\n");
 	 fprintf(head,"{\nreturn %s;\n}\n\n", name);
 	}
 if ((max) && (min))
 	{
 	 fprintf(head,"#define DEBUG %d\n\n", debug);
 	 fprintf(head,"double function(double x, double y)\n");
	 fprintf(head,"{\nif ((x<%lf) && (x>%lf)) return %s;\n", max_v, min_v, name);
 	 fprintf(head,"\nelse return 0.0;\n}\n\n");
 	}
 if ((!max) && (min))
 	{
 	 fprintf(head,"#define DEBUG %d\n\n", debug);
 	 fprintf(head,"double function(double x, double y)\n");
	 fprintf(head,"{\nif (x>%lf) return %s;\n", min_v, name);
 	 fprintf(head,"\nelse return 0.0;\n}\n\n");
 	}
 if ((max) && (!min))
 	{
 	 fprintf(head,"#define DEBUG %d\n\n", debug);
 	 fprintf(head,"double function(double x, double y)\n");
	 fprintf(head,"{\nif (x<%lf) return %s;\n", max_v, name);
 	 fprintf(head,"\nelse return 0.0;\n}\n\n");
 	}
 fclose(head);
 system("make 3df");
 system("./3dfvesa");
}


int main(int lb, char** par)
{
 int ok = 0;
 if (lb < 2)
 	{
	 printf("usage: %s =func(x) or %s =\"func(x)\"\n",par[0],par[0]);
	 return -1;
	}
 if (lb >= 3)
 	{
	 double bu;
	 ok = 0;
	 ok = sscanf(par[2],"x>%lf",&bu);
	 if (ok) min = true;
	 if (min == true) min_v = bu;
	 ok = 0;
	 ok = sscanf(par[2],"x<%lf",&bu);
	 if (ok) max = true;
	 if (max == true) max_v = bu;
	}
 if (lb >= 4)
 	{
	 double bu;
	 ok = 0;
	 ok = sscanf(par[3],"x>%lf",&bu);
	 if (ok) min = true;
	 if (min == true) min_v = bu;
	 ok = 0;
	 ok = sscanf(par[3],"x<%lf",&bu);
	 if (ok) max = true;
	 if (max == true) max_v = bu;
	}
 char buff[1024];
 sscanf(par[1],"=%s",buff);
 printf("f(x)=%s\n",buff);
 compile_f(buff);
}


