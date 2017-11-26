#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int open_audio(char* devname, int bits, int ch, int hz, int mode)
{
        int audio, afmt, tmp;

	audio = open(devname, mode, 0);
	if (audio < 0)
	{
	    	fprintf(stderr, "open: ");
		perror(strerror( errno ));
		return -1;
	}

	if (bits == 16) afmt = AFMT_S16_LE;
	else if (bits == 8) afmt = AFMT_U8;
	else
	{
	    	fprintf(stderr, "Bits must be 8 or 16, bits: %d\n", bits);
		return -2;
	}

	tmp = afmt;
	if (ioctl(audio, SNDCTL_DSP_SETFMT, &tmp) < 0)
	{
	    	fprintf(stderr, "ioctl: fmt: ");
		perror(strerror( errno ));
		return -3;
	}

	if (tmp != afmt)
	{
	    	fprintf(stderr, "Failed to set requested format r: %d<>%d\n", tmp, afmt);
		return -4;
	}

	tmp = ch;
	if (ioctl(audio, SNDCTL_DSP_CHANNELS, &tmp) < 0)
	{
	    	fprintf(stderr, "ioctl: chn: ");
		perror(strerror( errno ));
		return -5;
	}

	if (tmp != ch)
	{
	    	fprintf(stderr, "Failed to set requested channels r: %d<>%d\n", tmp, ch);
		return -6;
	}

	tmp = hz;
	if (ioctl(audio, SNDCTL_DSP_SPEED, &tmp) < 0)
	{
	    	fprintf(stderr, "ioctl: spd: ");
		perror(strerror( errno ));
		return -7;
	}

	if (tmp != hz)
	{
	    	fprintf(stderr, "Failed to set requested sampling rate r: %d<>%d\n", tmp, hz);
		return -8;
	}

	return audio;

}

int my_read(int fd, unsigned char* data, int n)
{
	int nread, nr;

	nread = 0;

	while (nread < n)
	{
	    	nr = read(fd, (void*)(&data[nread]), n - nread);

		if (nr == 0) return 0;
		else if (nr < 0)
		{
		    fprintf(stderr, "Error reading data: fd=%d, nr=%d, nread=%d, n=%d\n", fd, nr, nread, n);
		    return -1;
		}

		nread += nr;
	}

	return nread;
}

int my_write(int fd, unsigned char* data, int n)
{
	int nw, nwritten;

	nwritten = 0;

	while (nwritten < n)
	{
		nw = write(fd, (void*)(&data[nwritten]), n - nwritten);

		if (nw <= 0)
		{
		    fprintf(stderr, "Error writing data: fd=%d, nw=%d, nwritten=%d, n=%d\n", fd, nw, nwritten, n);
		    return -1;
		}
		nwritten += nw;
	}
	
	return nwritten;
}

void help(char* pname)
{
    printf("%s options\n", pname);
    printf("\t-bB\tset bits     (8 or 16) \t[8]\n");
    printf("\t-cC\tset channels (1,2,...) \t[1]\n");
    printf("\t-rR\tset speed    (8000-48k)\t[8000]\n");
    printf("\t-wW\twrite mode    (default is read mode)\n");
}

int main(int lb, char** par)
{
    char devname[0x100], u;
    int fd1, fd2, bits, ch, hz, n, m, bs, wmode, done;
    unsigned char* buff;

    strcpy(devname, "/dev/dsp");
    bits = 8;
    ch = 1;
    hz = 8000;
    wmode = 0;

    /*
    //-Bb: bits [8]
    //-Cc: channels [1]
    //-Rr: rate [8000]
    //-Hh: help [off]
    //-wW: write mode
    */

    while ((u = getopt(lb,par,"hHwWB:b:C:c:R:r:"))!=-1)
    {
	switch (u)
	{
		case 'b': case 'B': bits = atoi(optarg); 	break;
		case 'c': case 'C': ch = atoi(optarg); 		break;
		case 'r': case 'R': hz = atoi(optarg); 		break;
		case 'w': case 'W': wmode = 1;			break;
		case 'h': case 'H': help(par[0]); 		return 0;
		default: printf("Unrecognized option\n");       return 1;
	}
    }

    if (wmode)
    {
	fd1 = 0;
	fd2 = open_audio(devname, bits, ch, hz, O_WRONLY);
    }
    else
    {
	fd1 = open_audio(devname, bits, ch, hz, O_RDONLY);
	fd2 = 1;
    }

    if (fd1 < 0) return 1;
    if (fd2 < 0) return 1;

    bs = 0x100;
    buff = (unsigned char*)malloc(bs);

    done = 0;
    while (!done)
    {
	n = my_read(fd1, buff, bs);

	if (n == 0) done = 1;
	else if (n != bs)
	{
	    fprintf(stderr, "Error reading from fd%d: %d<>%d\n", fd1, n, bs);
	    return 1;
	}

	m = my_write(fd2, buff, bs);

	if (n == 0) done = 1;
	else if (m != bs)
	{
	    fprintf(stderr, "Error writing to fd%d: %d<>%d\n", fd2, m, bs);
	    return 1;
	}
    }

    return 0;
}

