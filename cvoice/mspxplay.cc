#include <libcvoice.h>

void mspxplay(FILE* f)
{
	int afmt, tmp;
	int audio = 0;

	audio = open(cfg.onum, O_WRONLY, 0);
	if (audio < 0)
	{
		cvoice_error("Open audio failed");
	}

	if (cfg.wide) afmt = AFMT_S16_LE;
	else afmt = AFMT_U8;

	tmp = afmt;
	if (ioctl(audio, SNDCTL_DSP_SETFMT, &tmp) < 0)
	{
		cvoice_error("Set afmt audio w failed");
	}

	if (tmp != afmt)
	{
	    	cvoice_error("Failed to set requested format w: %d<>%d", tmp, afmt);
	}

	tmp = 1;
	if (ioctl(audio, SNDCTL_DSP_CHANNELS, &tmp) < 0)
	{
		cvoice_error("Set achn audio w failed");
	}

	if (tmp != 1)
	{
	    	cvoice_error("Failed to set requested channels w: %d<>%d", tmp, 1);
	}

	tmp = cfg.hz;
	if (ioctl(audio, SNDCTL_DSP_SPEED, &tmp) < 0)
	{
		cvoice_error("Set aspd audio w failed");
	}

	if (tmp != cfg.hz)
	{
	    	cvoice_error("Failed to set requested sampling rate w: %d<>%d", tmp, cfg.hz);
	}

	if (cvoice_mspeex_decode_init() != 0)
	{
		cvoice_error("Error initialisind speex decoder");
	}

	int n, m, err, b, fs, nr, k, nr2, k2;
	time_t t_s, t_e, last_t_e;
	short l;
	int bn = 500 * APCK * (cfg.wide ? 2 : 1);

	char* buff = new char[bn];
	unsigned char* udata = new unsigned char[bn];
	b = 0;
	nr = nr2 = 0;
	k2 = 0;

/*	printf("MaxBuffer= %d\n", bn);*/

	fseek(f, 0, SEEK_END);
	fs = ftell(f);
	fseek(f, 0, SEEK_SET);

	time(&t_s);
	last_t_e = t_s;
	printf("\n");

	while (1)
	{
		n = fread((void*)buff, 2, 1, f);
		if (n != 1) break;

		//fseek(f, -2, SEEK_CUR);

		memcpy((void*)&l, (void*)buff, 2);
		//printf("l=%d\n", l);

		if (l <= 0 || l > bn)
		{
			printf("Recieve buffer size error: %d/%d\n", l, bn);
			break;
		}
		n = fread((void*)buff, l, 1, f);
		if (n != 1) break;

		nr += l + 2;

		n = (int)l;
		err = cvoice_mspeex_decode( buff, n, udata, m );
		//printf("decoded: %d --> %d, code: %d\n", n, m, err);

		if (err) continue;

		n = write(audio, udata, m);
		if (n <= 0)
		{
			cvoice_error("Write audio data failed: n=%d", n);
		}
		b ++;
		//printf("%d, %d, %d\n", n, m, b);
		time(&t_e);
		//printf("\n%d <--> %d\n", last_t_e, t_e);

		nr2 += l + 2;
		//printf("new nr2: %d, k2: %d\n", nr2, k2);

		if (last_t_e > t_s && last_t_e < t_e)
		{
		    	k2 = int((double)nr2 / ((double)t_e - (double)last_t_e));
		    	nr2 = 0;
		}

		last_t_e = t_e;

		if (t_e > t_s) k = (double)nr / (double)(t_e - t_s);
		else k = -1;

		if(b > 1) for (int x=0;x<76;x++) printf("\b");

		printf("%8d/%8d %6.2f%% %6d b/s ABPS %6.2f%% comp %6d b/s CBPS %4ds", 
			nr, fs, ((double)nr * 100.) / (double)fs, k, 
			((double)l * 100.) / (double)m, k2, (int)(t_e - t_s));
		fflush(stdout);

	}
	printf("\nDone.\n");
	
	close(audio);
	audio = 0;

	if (cvoice_mspeex_decode_destroy() != 0)
	{
		cvoice_error("Error destroying speex decoder");
	}

	//scanf("%d", &b);
}

int main(int lb, char** par)
{
	FILE* f;
	if (lb < 2) mspxplay( stdin );
	else
	{
	 	for (int i=1;i<lb;i++)
		{
		    	if (par[i][0] == '-') 
			{
			    cfg.hz = atoi(par[i]) * -1000;
			    continue;
			}

			f = fopen(par[i], "r+");
			if (!f)
			{
				printf("Error opening: %s\n", par[1]);
			}
			else
			{
				mspxplay( f );
				fclose(f);
			}
		}
	}
	return 0;
}
