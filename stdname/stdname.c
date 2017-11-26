#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void rename_file(char* fname)
{
	char *newname, *syscmd;
	int i, len;
	if (!fname) return;
	len = strlen(fname);
	newname = (char*)malloc(len + 1);
	for (i=0;i<len;i++)
	{
		if ((fname[i] >= 'A' && fname[i] <= 'Z') || (fname[i] >= 'a' && fname[i] <= 'z') || (fname[i] >= '0' && fname[i] <= '9'))
		{
			newname[i] = fname[i];
		}
		else
		{
			newname[i] = '_';
		}
	}
	newname[len] = 0;
	syscmd = (char*)malloc( len * 2 + 16 ); 
	sprintf(syscmd, "mv \"%s\" \"%s\"", fname, newname);
	printf("SYS: %s\n", syscmd);
	system( syscmd );
	free((void*)syscmd);
	free((void*)newname);
}

int main(int lb, char** par)
{
	int i;
	for (i=1;i<lb;i++) rename_file(par[i]);
	return 0;
}
