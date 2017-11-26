#include <unistd.h>

int main(int lb, char** par)
{
 if (lb>=2) usleep(atoi(par[1]));
 return 0;
}
