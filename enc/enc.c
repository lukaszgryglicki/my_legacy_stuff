#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
  int c, l, i, s;
  unsigned char ec;
  char *pwd;
  if (argc < 2)
  {
    printf("Missing password : [+|-]pwd\n");
    return 1;
  }
  pwd = argv[1];
  if (pwd[0] == '+') s = 1;
  else if (pwd[0] == '-') s = -1;
  else
  {
    printf("Wrong password format, should be +pwd or -pwd\n");
    return 2;
  }
  pwd ++;
  l = strlen(pwd);
  i = 0;
  while ((c = fgetc(stdin)) != EOF)
  {
    ec = (c + s*(int)pwd[i++ % l]) % 0x100;
    //printf("%d->%d\n", c, ec);
    fputc(ec, stdout);
  }
  return 0;
}

