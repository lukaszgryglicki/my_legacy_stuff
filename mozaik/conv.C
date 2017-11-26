#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void conv(char* f1, char* f2)
{
 FILE *in, *out;
 int i, itemp, ival[10];
 char temp[256];

 in = fopen(f1, "r");
 if (!in) return;
 out = fopen(f2, "w");
 if (!out) { fclose(in); return; }

printf("opened.\n");
 fscanf(in,   "SNAPDIR=%s\n", temp);
 fprintf(out, "SNAPDIR=%s\n", temp);

 fscanf(in,   "PROCESSED=%d\n", &itemp);
 fprintf(out, "PROCESSED=%d\n", itemp);

 fscanf(in,   "SNAP=%d,%d\n", &ival[0], &ival[1]);
 fprintf(out, "SNAP=%d,%d\n", ival[0], ival[1]);
 while (fscanf(in, "SIZE=%d,%d SNAP=%d,%d,%d COLOR=%d,%d,%d\n", &ival[0], &ival[1], &ival[2], &ival[3], &ival[4], &ival[5], &ival[6], &ival[7]) == 8)
 {
	fprintf(out, "SIZE=%d,%d SNAP=%d,%d,%d COLOR=%d,%d,%d\n", 
	 ival[0], ival[1], ival[2], ival[3], ival[4], ival[7], ival[6], ival[5]);
 }
 fclose(in);
 fclose(out);
}

int main(int lb, char** par)
{
 conv(par[1], par[2]);
 return 0;
}
