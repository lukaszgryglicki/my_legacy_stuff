#include <sys/soundcard.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <speex/speex.h>

struct cv_cfg
{
    cv_cfg()
    {
	inum = new char[1024];
	onum = new char[1024];
    	myid = targetid = 0;
	cache_size = 1600;
	debug = 0;
	quality = 0;
	timeout = -30;
	strcpy(inum, "/dev/dsp");
	strcpy(onum, "/dev/dsp");
	exmode = 0;
	srv_sock = cli_sock = 0;
	wide = 1;
	enh_r = 1.0;
	enh_w = 1.0;
	save = 0;
    }

    ~cv_cfg()
    {
	delete [] inum;
	delete [] onum;
    }

    int myid, targetid, cache_size, debug, quality;
    int timeout, exmode;
    int srv_sock, cli_sock, wide;
    int save;
    char *inum, *onum;
    float enh_r, enh_w;
};

struct speex_cfg
{
	speex_cfg()
	{
		enc_state = dec_state = NULL;
		enc_fsize = dec_fsize = 0;
		enc_osize = dec_osize = 0;
		input = output = NULL;
	}

	SpeexBits enc_bits, dec_bits;
	void *enc_state, *dec_state;
	int enc_fsize, dec_fsize;
	int enc_osize, dec_osize;
	short *input, *output;
};

speex_cfg spx;
cv_cfg cfg;
int audio = 0;

void error(const char* fmt, ...)
{
	va_list lst;

	va_start(lst,fmt);
	vfprintf(stdout, fmt,lst);
	va_end(lst);

	fprintf(stdout, "\n");
	fflush(stdout);

	if (audio > 0) close(audio);
	audio = 0;

	exit(1);
}

int mspeex_decode_init()
{
	int tmp;
	spx.dec_state = speex_decoder_init(&speex_nb_mode);
	if (!spx.dec_state)
	{
	    	error("Error creating speex decoder");
	}

	if (speex_decoder_ctl(spx.dec_state, SPEEX_GET_FRAME_SIZE, &tmp) != 0)
	{
	    	error("Error getting speex decoder frame size");
	}
	spx.dec_fsize = tmp;

	if (spx.dec_fsize <= 0)
	{
	    	error("Bad speex decoder frame size: %d", spx.dec_fsize);
	}

	spx.dec_osize = spx.dec_fsize + 40;

    	tmp = 1;
   	if (speex_decoder_ctl(spx.dec_state, SPEEX_SET_ENH, &tmp) != 0)
	{
	    	error("Error seting speex decoder parameters: enhancer");
	}

    	speex_bits_init(&spx.dec_bits);

	spx.output = new short[spx.dec_osize];
	if (!spx.output)
	{
	    error("Error allocating speex output buffer");
	}

	return 0;
}

int mspeex_decode_destroy()
{
	speex_decoder_destroy(spx.dec_state);
	speex_bits_destroy(&spx.dec_bits);
	delete [] spx.output;
	return 0;
}

int mspeex_decode( char* in, int n, unsigned char* out, int& m)    
{
    	register int dec, x;
	int nd, nt, ds;
	unsigned char fs;
	register float enh, s;

	//printf("Speex decoding %d bytes\n", n);

	m = 0;
	if (n <= 0) return 1;

	nd = 0;
	enh = cfg.enh_w;

	while (nd < n)
	{
		memcpy((void*)(&fs), (void*)&in[nd], 1);

		ds = (int)fs;
		//printf("To decode size: %d\n", ds);

		if (ds < 1 || ds > n)
		{
			return 2;
		}

		nd ++;
		nt = nd + ds;

		speex_bits_read_from(&spx.dec_bits, &in[nd], ds);
		dec = speex_decode_int(spx.dec_state, &spx.dec_bits, spx.output);
		if (dec < 0)
		{
			return 3;
		}

		dec = spx.dec_fsize;

		if (cfg.wide)
		{
			dec *= 2;
			x = 0;
			for (int i=0;i<dec;i+=2)
			{
				s = float(spx.output[x]) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.output[x] = short(s);
				memcpy((void*)&out[m+i], (void*)&spx.output[x], 2);
				x++;
			}
		}
		else
		{
			for (int i=0;i<dec;i++)
			{
				s = float(spx.output[i]) * enh;
				if (s > 32766.) s = 32766.;
				else if (s < -32766.) s = -32766.;
				spx.output[i] = short(s);
				out[i+m] = (spx.output[i] >> 8) + 0x80;
			}
		}

		//printf("Decoded frame: <%d, %d> --> %d --> %d\n", nd, nt, ds, dec);
		nd += ds;
		m += dec;
	}

	//printf("Finally decoded %d --> %d\n", n, m);

	return 0;
}


void mspxplay(FILE* f)
{
	int afmt, tmp;

	audio = open(cfg.onum, O_WRONLY, 0);
	if (audio < 0)
	{
		error("Open audio failed");
	}

	if (cfg.wide) afmt = AFMT_S16_LE;
	else afmt = AFMT_U8;

	tmp = afmt;
	if (ioctl(audio, SNDCTL_DSP_SETFMT, &tmp) < 0)
	{
		error("Set afmt audio w failed");
	}

	if (tmp != afmt)
	{
	    	error("Failed to set requested format w: %d<>%d", tmp, afmt);
	}

	tmp = 1;
	if (ioctl(audio, SNDCTL_DSP_CHANNELS, &tmp) < 0)
	{
		error("Set achn audio w failed");
	}

	if (tmp != 1)
	{
	    	error("Failed to set requested channels w: %d<>%d", tmp, 1);
	}

	tmp = 8000;
	if (ioctl(audio, SNDCTL_DSP_SPEED, &tmp) < 0)
	{
		error("Set aspd audio w failed");
	}

	if (tmp != 8000)
	{
	    	error("Failed to set requested sampling rate w: %d<>%d", tmp, 8000);
	}

	if (mspeex_decode_init() != 0)
	{
		error("Error initialisind speex decoder");
	}

	int n, m, err, b, fs, nr, k, nr2, k2;
	time_t t_s, t_e, last_t_e;
	short l;
	int bn = cfg.cache_size;

	char* buff = new char[bn];
	unsigned char* udata = new unsigned char[bn];
	b = 0;
	nr = nr2 = 0;
	k2 = 0;

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
			printf("Recieve buffer size error: %d\n", l);
			break;
		}

		n = fread((void*)buff, l, 1, f);
		if (n != 1) break;

		nr += l + 2;

		n = (int)l;
		err = mspeex_decode( buff, n, udata, m );
		//printf("decoded: %d --> %d, code: %d\n", n, m, err);

		if (err) continue;

		n = write(audio, udata, m);
		if (n <= 0)
		{
			error("Write audio data failed: n=%d", n);
		}
		b ++;
		//printf("%d, %d, %d\n", n, m, b);
		time(&t_e);

		if (last_t_e == t_e) nr2 += l + 2;
		else
		{
		    	k2 = nr2;
		    	nr2 = 0;
		}

		last_t_e = t_e;


		if (t_e > t_s) k = (double)nr / (double)(t_e - t_s);
		else k = -1;

		if(b > 1) for (int x=0;x<76;x++) printf("\b");

		printf("%8d/%8d %6.2f%% %6d b/s ABPS %6.2f%% comp %6d b/s CBPS %4ds", 
			nr, fs, ((double)nr * 100.) / (double)fs, k, 
			((double)l * 100.) / (double)m, k2, t_e - t_s);
		fflush(stdout);

	}
	printf("\nDone.\n");
	
	close(audio);
	audio = 0;

	if (mspeex_decode_destroy() != 0)
	{
		error("Error destroying speex decoder");
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
