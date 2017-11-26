#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fpar.h"

void help(char* pname)
{
    printf("%s options\n", pname);
    printf("\t-bB\tset bits     (8 or 16) \t[8]\n");
    printf("\t-cC\tset channels (1,2,...) \t[1]\n");
    printf("\t-rR\tset speed    (8000-48k)\t[8000]\n");
    printf("\t-nN\tset number of seconds to generate (0-) [infinity]\n");
    printf("\t-1..9\tset channel 1..9 func (will be digitized in 0-N-->-1-1 range, using bits and speed)\n");
    fpar_info();
}

void set_fdef(char** str, char* arg)
{
    if (!str || !arg) return;

    *str = (char*)malloc((strlen(arg)+1)*sizeof(char));
    strcpy(*str, arg);
}

int main(int lb, char** par)
{
    int bits, ch, hz, i, n, done, j, s;
    char u;
    char* func[9];
    real arg, v;
    unsigned char u8;
    signed short s16;

    bits = 8;
    ch = 1;
    hz = 8000;
    n = -1;
    for (i=0;i<9;i++) func[i] = NULL;

    /*
    //-Bb: bits [8]
    //-Cc: channels [1]
    //-Rr: rate [8000]
    //-Nn: number of seconds to generate
    //-1: set channel1 function
    //-2: set channel2 function (if stereo)
    //...
    //-9: set channel9 function (if 9 channels)
    //-Hh: help [off]
    */

    while ((u = getopt(lb,par,"hHn:N:1:2:3:4:5:6:7:8:9:B:b:C:c:R:r:"))!=-1)
    {
	switch (u)
	{
		case 'b': case 'B': bits = atoi(optarg); 	break;
		case 'c': case 'C': ch = atoi(optarg); 		break;
		case 'r': case 'R': hz = atoi(optarg); 		break;
		case 'n': case 'N': n = atoi(optarg); 		break;
		case '1': set_fdef(&func[0], optarg); 		break; 
		case '2': set_fdef(&func[1], optarg); 		break; 
		case '3': set_fdef(&func[2], optarg); 		break; 
		case '4': set_fdef(&func[3], optarg); 		break; 
		case '5': set_fdef(&func[4], optarg); 		break; 
		case '6': set_fdef(&func[5], optarg); 		break; 
		case '7': set_fdef(&func[6], optarg); 		break; 
		case '8': set_fdef(&func[7], optarg); 		break; 
		case '9': set_fdef(&func[8], optarg); 		break; 
		case 'h': case 'H': help(par[0]); 		return 0;
		default: printf("Unrecognized option\n");       return 1;
	}
    }

    if (ch < 1 || ch > 9) 
    {
	printf("Bad number of channels: %d, allowed 1..9\n", ch);
	return 1;
    }

    if (bits !=8 && bits != 16) 
    {
	printf("Bad bits value: %d, allowed are 8 and 16\n", bits);
	return 2;
    }

    if (hz < 1 || hz > (1<<20))
    {
	printf("Bad speed value: %d, allowed are 1..%d\n", hz, 1<<20);
	return 3;
    }

    fpar_init(ch);

    for (i=0;i<ch;i++) 
    {
	if (!func[i])
	{
	    printf("No %d channel function, use -%d to define it\n", i+1, i+1);
	    return 4;
	}

	fpar_function(i, 1, 0, func[i]);

        if (!fpar_ok(i)) 
        { 
	    printf("Function channel%d (%s) not accepted.\n", i+1, func[i]); 
	    return 5; 
        }
    }

    done = 0;
    i = 0;
    while (!done)
    {
	for (s=0;s<hz;s++)
	{
	    arg = (double)i + ((double)s / (double)(hz - 1));
/*	    printf("arg: %f: ", arg);*/
	    for (j=0;j<ch;j++)
	    {
		v = fpar_f(j, arg, NULL);
		v = .5*v + .5;
		if (bits == 8)
		{
		    v *= 256.;
		    if (v >= 255.) u8 = 0xff;
		    else if (v <= 0.) u8 = 0x00;
		    else
		    {
			u8 = (unsigned char)v;
		    }
/*		    printf("%02x ", (unsigned int)u8);*/
		    fwrite((void*)&u8, sizeof(unsigned char), 1, stdout);
		}
		else
		{
		    v *= 65536.;
		    v -= 32768.;
		    if (v >= 32767.) s16 = 32767;
		    else if (v <= -32768.) s16 = -32768;
		    else
		    {
			s16 = (signed short)v;
		    }
/*		    printf("%05d ", (signed short)s16);*/
		    fwrite((void*)&s16, sizeof(signed short), 1, stdout);
		}
/*		printf("%f ", v);*/
	    }
/*	    printf("\n");*/
	}

	i ++;
	if (i == n) done = 1;
    }

    return 0;
}

