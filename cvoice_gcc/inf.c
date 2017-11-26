#include <libcvoice.h>
#include <libmipc.h>

void info()
{
    	char inf[256];
    	double bps[10];

	sprintf(log_pfx, "<inf>");
	cvoice_init_mipc(0);

#ifdef MIPC

    	if (!mshm_ptr) return;

    	memcpy((void*)bps, mshm_ptr, 10 * sizeof(double));

    	sprintf(inf, 
		"BPS:\t\t\t%.2f bytes/s\n"
		"Compression:\t\t%.3f %%\n"
		"Elapsed:\t\t%.0f s\n"
		"Sent:\t\t%9.0f bytes\n"
		"Recieved:\t%9.0f bytes\n"
		"Read:\t\t%9.0f bytes\n"
		"Written:\t%9.0f bytes\n"
		"Packets processed:%7.0f/%7.0f"
		, bps[0], bps[1], bps[2], bps[3], bps[4], bps[5], bps[6], bps[7], bps[8]);

	printf("%s\n", inf);
	cvoice_log( inf );

#endif
}

int main()
{
    info();
    return 0;
}

