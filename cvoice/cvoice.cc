#include <libcvoice.h>

int main(int lb, char** par)
{
    	int is_server, ret;

   	is_server = cvoice_getopt(lb, par);
	if (is_server >= 0)
	{
		cvoice_check_config();
		cvoice(is_server);
		ret = 0;
	}
	else ret = is_server;

	return ret;
}

