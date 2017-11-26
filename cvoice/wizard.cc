#define _WITH_GETLINE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>

int mgetch()
{
 	struct termios st,end;
 	int ret;

 	tcgetattr(fileno(stdin),&st);

 	ret = 0;
 	end = st;
 	end.c_lflag &= ~ICANON;
 	end.c_lflag &= ~ECHO;
 	end.c_cc[VMIN] = 1;
 	end.c_cc[VTIME] = 0;

	if (tcsetattr(fileno(stdin),TCSAFLUSH,&end)) return 0;
 	ret = getchar();
 	tcsetattr(fileno(stdin),TCSAFLUSH,&st);
	printf("\n");

	if (ret >= 'A' && ret <= 'Z') ret += 0x20;

 	return ret;
}

void wizard()
{
	int zn, n, srv, ip[4], itmp;
	char* options;
	char *temp, *temp2;
	size_t l;
	double ftmp;
	char dspin[32], dspout[32];

	printf("\n\n");
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	printf("This is wizard to run cvoice VoIP client/server\n");
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n\n");

	options = new char[1024];
	temp = new char[256];
	temp2 = new char[256];

	strcpy(dspin, "/dev/mixer");
	strcpy(dspout, "/dev/mixer");

	strcpy(options, "./cvoice ");

	zn = 0;
	while (zn != 's' && zn != 'c' && zn != 'n')
	{
		printf("Server(s) or Client(c) (s/c/q/n) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 's') 
	{
	    strcat(options, " -a ");
	    srv = 1;
	}
	else srv = 0;

	zn = 0;
	if (!srv)
	{
		while (zn != 'i' && zn != 's' && zn != 'n')
		{
			printf("Connect to IP(i), server(s), skip(n) (i/s/n/q) ");
			zn = mgetch();
			if (zn == 'q') return;
		}

		if (zn == 'i')
		{
			zn = 0;
			while (zn != '1' && zn != '2' && zn != '3' && zn != '4' && zn != '5' && zn != 'o')
			{		
				printf("IP:\n192.168.1.101(1), 192.168.1.102(2), 192.168.1.103(3), 88.199.169.19(4), 5.29.141.180(5), Other(o)\n(1/2/3/4/5/o/q) ");
				zn = mgetch();
				if (zn == 'q') return;
			}

			if (zn == 'o')
			{
		  	 	n = 0;
		    		while (n != 4)
				{
		    			printf("Enter IP address: ");

					l = 255;
					getline(&temp, &l, stdin);
					n = sscanf(temp, "%d.%d.%d.%d\n", &ip[0], &ip[1], &ip[2], &ip[3]);

					if (n != 4) printf("Bad IP format, example: 127.0.0.1\n");
				}
				sprintf(temp, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
				sprintf(temp2, " -i %s ", temp);
				strcat(options, temp2);
			}
			else if (zn == '1') strcat(options, " -i 192.168.1.101 ");
			else if (zn == '2') strcat(options, " -i 192.168.1.102 ");
			else if (zn == '3') strcat(options, " -i 192.168.1.103 ");
			else if (zn == '4') strcat(options, " -i 88.199.169.19 ");
			else if (zn == '5') strcat(options, " -i 5.29.141.180 ");
		}

		if (zn == 's')
		{
			zn = 0;
			while (zn != 'l' && zn != 'm' && zn != 'o')
			{		
				printf("Serwer: localhost(l), smmechanik.pl(m), other(o) (l/m/o/q) ");
				zn = mgetch();
				if (zn == 'q') return;
			}

			if (zn == 'o')
			{
		  	 	n = 0;
		    		printf("Enter host name: ");

				l = 255;
				getline(&temp, &l, stdin);
				temp[strlen(temp)-1] = 0;

				sprintf(temp2, " -s %s ", temp);
				strcat(options, temp2);
			}
			else if (zn == 'l') strcat(options, " -s localhost ");
			else if (zn == 'm') strcat(options, " -s smmechanik.pl ");
		}
	}

	zn = 0;
	while (zn != '0' && zn != '1' && zn != 'n' && zn != 'o')
	{
		printf("Port selection 9000(0) 9001(1) skip(n) other(o) (0/1/n/o/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == '0') strcat(options, " -p 9000 ");
	else if (zn == '1') strcat(options, " -p 9001 ");
	else if (zn == 'o')
	{
		n = 0;
		while (n != 1)
		{
			printf("Enter port: ");
			l = 255;
			getline(&temp, &l, stdin);
			temp[strlen(temp)-1] = 0;
			n = sscanf(temp, "%d", &itmp);
			if (n != 1) printf("Bad numeric value: %s\n", temp);
			else if (itmp < 1 || itmp > 65535)
			{
				printf("Bad port number: %d\n", itmp);
				n = 0;
			}
		}
		sprintf(temp2, " -p %d ", itmp);
		strcat(options, temp2);
	}

	if (!srv)
	{
		zn = 0;
		while (zn != 'y' && zn != 'n')
		{
			printf("Do You want echosrv mode? (y/n/q) ");
			zn = mgetch();
			if (zn == 'q') return;
		}
	}
	else zn = 'n';

	if (zn == 'y')
	{
		zn = 0;
		while (zn != '1' && zn != '2' && zn != 'o')
		{
			printf("My ID 1, 2, other(o) (1/2/o) ");
			zn = mgetch();
			if (zn == 'q') return;
		}

		if (zn == '1') strcat(options, " -f 1 ");
		else if (zn == '2') strcat(options, " -f 2 ");
		else if (zn == 'o')
		{
			n = 0;
			while (n != 1)
			{
				printf("Enter My ID: ");
				l = 255;
				getline(&temp, &l, stdin);
				temp[strlen(temp)-1] = 0;
				n = sscanf(temp, "%d", &itmp);
				if (n != 1) printf("Bad numeric value: %s\n", temp);
				else if (itmp < 0)
				{
					printf("Bad My ID: %d\n", itmp);
					n = 0;
				}
			}
			sprintf(temp2, " -f %d ", itmp);
			strcat(options, temp2);
		}

		zn = 0;
		while (zn != '1' && zn != '2' && zn != 'o')
		{
			printf("Target ID 1, 2, other(o) (1/2/o) ");
			zn = mgetch();
			if (zn == 'q') return;
		}

		if (zn == '1') strcat(options, " -t 1 ");
		else if (zn == '2') strcat(options, " -t 2 ");
		else if (zn == 'o')
		{
			n = 0;
			while (n != 1)
			{
				printf("Enter Target ID: ");
				l = 255;
				getline(&temp, &l, stdin);
				temp[strlen(temp)-1] = 0;
				n = sscanf(temp, "%d", &itmp);
				if (n != 1) printf("Bad numeric value: %s\n", temp);
				else if (itmp < 0)
				{
					printf("Bad Target ID: %d\n", itmp);
					n = 0;
				}
			}
			sprintf(temp2, " -t %d ", itmp);
			strcat(options, temp2);
		}
	}

	zn = 0;
	while (zn != '1' && zn != '2' && zn != '3' && zn != '4' && zn != '5' && zn != '6' && zn != 'n' && zn != 'o')
	{
		printf("Audio cache: smallest(1), small(2), medium(3), big(4), biggest(5), huge(6), other(o) (1/2/3/4/5/6/o/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == '1') strcat(options, " -c 1 ");
	else if (zn == '2') strcat(options, " -c 3 ");
	else if (zn == '3') strcat(options, " -c 5 ");
	else if (zn == '4') strcat(options, " -c 10 ");
	else if (zn == '5') strcat(options, " -c 20 ");
	else if (zn == '6') strcat(options, " -c 50 ");
	else if (zn == 'o')
	{
		n = 0;
		while (n != 1)
		{
			printf("Enter audio buffer size in 160b packets: ");
			l = 255;
			getline(&temp, &l, stdin);
			temp[strlen(temp)-1] = 0;
			n = sscanf(temp, "%d", &itmp);
			if (n != 1) printf("Bad numeric value: %s\n", temp);
			else if (itmp < 1 || itmp > 32737)
			{
				printf("Bad audio buffer size: %d\n", itmp);
				n = 0;
			}
		}
		sprintf(temp2, " -c %d ", itmp);
		strcat(options, temp2);
	}

	zn = 0;
	while (zn != 'y' && zn != 'n')
	{
		printf("Debug? (y/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 'y') strcat(options, " -d ");

	zn = 0;
	while ((zn < '0' || zn > '9') && zn !='m' && zn != 'n')
	{
		printf("Speex quality: 0-10 10(m) (0-9/m/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;		
	}

	if (zn >= '0' && zn <= '9')
	{
		sprintf(temp, " -q %c ", zn);
		strcat(options, temp);
	}
	else if (zn == 'm') strcat(options, " -q 10 ");

	zn = 0;
	while ((zn > '5' || zn < '1') && zn !='o' && zn != 'n')
	{
		printf("Net timeout -30(1), 30(2), 0(3), -300(4), 300(5), other(o) (1/2/3/4/5/o/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == '1') strcat(options, " -T -30 ");
	else if (zn == '2') strcat(options, " -T 30 ");
	else if (zn == '3') strcat(options, " -T 0 ");
	else if (zn == '4') strcat(options, " -T -300 ");
	else if (zn == '5') strcat(options, " -T 300 ");
	else if (zn == 'o')
	{
		n = 0;
		while (n != 1)
		{
			printf("Enter network timeout: ");
			l = 255;
			getline(&temp, &l, stdin);
			temp[strlen(temp)-1] = 0;
			n = sscanf(temp, "%d", &itmp);
			if (n != 1) printf("Bad numeric value: %s\n", temp);
			else if (itmp < 1 || itmp > 86400)
			{
				printf("Bad network timeout value: %d\n", itmp);
				n = 0;
			}
		}
		sprintf(temp2, " -T %d ", itmp);
		strcat(options, temp2);
	}

	zn = 0;
	while ((zn > '9' || zn < '0') && zn !='o' && zn != 'n')
	{
		printf("Synchronize after buffers 10x (1-9), 0=all transfer other(o) (0-9/o/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == '0') strcat(options, " -y 1 ");
	if (zn >= '1' && zn <= '9') 
	{
	    	sprintf(temp, " -y %c0 ", zn);
		strcat(options, temp);
	}
	else if (zn == 'o')
	{
		n = 0;
		while (n != 1)
		{
			printf("Synchronize after how may packets: ");
			l = 255;
			getline(&temp, &l, stdin);
			temp[strlen(temp)-1] = 0;
			n = sscanf(temp, "%d", &itmp);
			if (n != 1) printf("Bad numeric value: %s\n", temp);
			else if (itmp < 1 || itmp > 100000)
			{
				printf("Bad value: %d\n", itmp);
				n = 0;
			}
		}
		sprintf(temp2, " -y %d ", itmp);
		strcat(options, temp2);
	}

	zn = 0;
	while ((zn < '0' || zn > '9') && zn != 'o' && zn != 'n')
	{
		printf("Microphone device /dev/dspN(0-9), other(o) (0-9/o/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn >= '0' && zn <= '9')
	{
	    	sprintf(dspin, "/dev/mixer%c", zn);
		sprintf(temp, " -I /dev/dsp%c ", zn);
		strcat(options, temp);
	}
	else if (zn == 'o') 
	{
  	 	n = 0;
		system("ls /dev/");
    		printf("Enter device name: ");

		l = 255;
		getline(&temp, &l, stdin);
		temp[strlen(temp)-1] = 0;

		sprintf(temp2, " -I /dev/%s ", temp);
		strcat(options, temp2);

	}

	zn = 0;
	while ((zn < '0' || zn > '9') && zn != 'o' && zn != 'n')
	{
		printf("Speaker device /dev/dspN(0-9), other(o) (0-9/o/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn >= '0' && zn <= '9')
	{
	    	sprintf(dspout, "/dev/mixer%c", zn);
		sprintf(temp, " -O /dev/dsp%c ", zn);
		strcat(options, temp);
	}
	else if (zn == 'o') 
	{
  	 	n = 0;
		system("ls /dev/");
    		printf("Enter device name: ");

		l = 255;
		getline(&temp, &l, stdin);
		temp[strlen(temp)-1] = 0;

		sprintf(temp2, " -O /dev/%s ", temp);
		strcat(options, temp2);
	}

	zn = 0;
	while (zn != 'y' && zn != 'n')
	{
		printf("Log files with PID identify? (y/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 'y') strcat(options, " -P ");

	zn = 0;
	while (zn != 'y' && zn != 'n')
	{
		printf("8bit audio (default is 16bit)? (y/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 'y') strcat(options, " -8 ");

	zn = 0;
	while (zn != 'y' && zn != 'n')
	{
		printf("Save audio data to files? (y/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 'y') strcat(options, " -S ");

	zn = 0;
	while ((zn < '0' || zn > '9') && zn != 'm' && zn != 'n')
	{
		printf("Complexity 0-9 max(m) (0-10/m/q/n) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn >= '0' && zn <= '9') 
	{
	    sprintf(temp, " -C %c ", zn);
	    strcat(options, temp);
	}
	else if (zn == 'm')
	{
		strcat(options, " -C 10 ");
	}

	zn = 0;
	while ((zn < '0' || zn > '9') && zn != 'o' && zn != 'n')
	{
		printf("Enchance input? 0(0.5) 1(0.7) 2(1.1) 3(1.2) 4(1.3) 5(1.4) 6(1.5) 7(1.7) 8(2.0) 9(2.5) other(o) (0-9/o/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == '0') strcat(options, " -R 0.5 ");
	else if (zn == '1') strcat(options, " -R 0.7 ");
	else if (zn == '2') strcat(options, " -R 1.1 ");
	else if (zn == '3') strcat(options, " -R 1.2 ");
	else if (zn == '4') strcat(options, " -R 1.3 ");
	else if (zn == '5') strcat(options, " -R 1.4 ");
	else if (zn == '6') strcat(options, " -R 1.5 ");
	else if (zn == '7') strcat(options, " -R 1.7 ");
	else if (zn == '8') strcat(options, " -R 2.0 ");
	else if (zn == '9') strcat(options, " -R 2.5 ");
	else if (zn == 'o')
	{
		n = 0;
		while (n != 1)
		{
			printf("Enter input enchance factor: ");
			l = 255;
			getline(&temp, &l, stdin);
			temp[strlen(temp)-1] = 0;
			n = sscanf(temp, "%lf", &ftmp);
			if (n != 1) printf("Bad numeric value: %s\n", temp);
			else if (ftmp < 0. || ftmp > 20.)
			{
				printf("Bad input enchance factor value: %f\n", ftmp);
				n = 0;
			}
		}
		sprintf(temp2, " -R %f ", ftmp);
		strcat(options, temp2);
	}

	zn = 0;
	while ((zn < '0' || zn > '9') && zn != 'o' && zn != 'n')
	{
		printf("Enchance output? 0(0.5) 1(0.7) 2(1.1) 3(1.2) 4(1.3) 5(1.4) 6(1.5) 7(1.7) 8(2.0) 9(2.5) other(o) (0-9/o/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == '0') strcat(options, " -W 0.5 ");
	else if (zn == '1') strcat(options, " -W 0.7 ");
	else if (zn == '2') strcat(options, " -W 1.1 ");
	else if (zn == '3') strcat(options, " -W 1.2 ");
	else if (zn == '4') strcat(options, " -W 1.3 ");
	else if (zn == '5') strcat(options, " -W 1.4 ");
	else if (zn == '6') strcat(options, " -W 1.5 ");
	else if (zn == '7') strcat(options, " -W 1.7 ");
	else if (zn == '8') strcat(options, " -W 2.0 ");
	else if (zn == '9') strcat(options, " -W 2.5 ");
	else if (zn == 'o')
	{
		n = 0;
		while (n != 1)
		{
			printf("Enter output enchance factor: ");
			l = 255;
			getline(&temp, &l, stdin);
			temp[strlen(temp)-1] = 0;
			n = sscanf(temp, "%lf", &ftmp);
			if (n != 1) printf("Bad numeric value: %s\n", temp);
			else if (ftmp < 0. || ftmp > 20.)
			{
				printf("Bad output enchance factor value: %f\n", ftmp);
				n = 0;
			}
		}
		sprintf(temp2, " -W %f ", ftmp);
		strcat(options, temp2);
	}

	zn = 0;
	while(zn != 'y' && zn != 'n')
	{
		printf("Adjust mixer? (y/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 'y')
	{
		itmp = -1;
		printf("Input device is: %s\n", dspin);

		zn = 0;
		while((zn < '0' || zn > '9') && zn != 'o' && zn != 'n')
		{
			printf("Input overall volume: 0=100, 1=10, 2=20 ..., other(o) (0-9/o/n/q) ");
			zn = mgetch();
			if (zn == 'q') return;
		}

		if (zn == '0') itmp = 100;
		else if (zn >= '1' && zn <= '9')
		{
			itmp = 10 * (zn - '0');
		}
		else if (zn == 'o')
		{
			n = 0;
			while (n != 1)
			{
				printf("Enter input volume: ");
				l = 255;
				getline(&temp, &l, stdin);
				temp[strlen(temp)-1] = 0;
				n = sscanf(temp, "%d", &itmp);
				if (n != 1) printf("Bad numeric value: %s\n", temp);
				else if (itmp < 0 || itmp > 100)
				{
					printf("Bad volume value: %d\n", itmp);
					n = 0;
				}
			}
		}

		if (itmp >= 0)
		{
			printf("Apply %d volume to 'mic' device? (y/n/q) ", itmp);
			zn = mgetch();
			if (zn == 'q') return;
			if (zn == 'y')
			{
				sprintf(temp, "mixer -f %s mic %d 1> /dev/null 2> /dev/null", dspin, itmp);
				system(temp);
			}

			printf("Apply %d volume to 'rec' device? (y/n/q) ", itmp);
			zn = mgetch();
			if (zn == 'q') return;
			if (zn == 'y')
			{
				sprintf(temp, "mixer -f %s rec %d 1> /dev/null 2> /dev/null", dspin, itmp);
				system(temp);
			}

			printf("Apply %d volume to 'monitor' device? (y/n/q) ", itmp);
			zn = mgetch();
			if (zn == 'q') return;
			if (zn == 'y')
			{
				sprintf(temp, "mixer -f %s monitor %d 1> /dev/null 2> /dev/null", dspin, itmp);
				system(temp);
			}

			printf("Apply %d volume to 'mix' device? (y/n/q) ", itmp);
			zn = mgetch();
			if (zn == 'q') return;
			if (zn == 'y')
			{
				sprintf(temp, "mixer -f %s mix %d 1> /dev/null 2> /dev/null", dspin, itmp);
				system(temp);
			}
		}
		else printf("Input mixer skipped\n");

		itmp = -1;
		printf("Output device is: %s\n", dspout);

		zn = 0;
		while((zn < '0' || zn > '9') && zn != 'o' && zn != 'n')
		{
			printf("Output overall volume: 0=100, 1=10, 2=20 ..., other(o) (0-9/o/n/q) ");
			zn = mgetch();
			if (zn == 'q') return;
		}

		if (zn == '0') itmp = 100;
		else if (zn >= '1' && zn <= '9')
		{
			itmp = 10 * (zn - '0');
		}
		else if (zn == 'o')
		{
			n = 0;
			while (n != 1)
			{
				printf("Enter output volume: ");
				l = 255;
				getline(&temp, &l, stdin);
				temp[strlen(temp)-1] = 0;
				n = sscanf(temp, "%d", &itmp);
				if (n != 1) printf("Bad numeric value: %s\n", temp);
				else if (itmp < 0 || itmp > 100)
				{
					printf("Bad volume value: %d\n", itmp);
					n = 0;
				}
			}
		}

		if (itmp >= 0)
		{
			printf("Apply %d volume to 'vol' device? (y/n/q) ", itmp);
			zn = mgetch();
			if (zn == 'q') return;
			if (zn == 'y')
			{
				sprintf(temp, "mixer -f %s vol %d 1> /dev/null 2> /dev/null", dspout, itmp);
				system(temp);
			}

			printf("Apply %d volume to 'pcm' device? (y/n/q) ", itmp);
			zn = mgetch();
			if (zn == 'q') return;
			if (zn == 'y')
			{
				sprintf(temp, "mixer -f %s pcm %d 1> /dev/null 2> /dev/null", dspout, itmp);
				system(temp);
			}


			printf("Apply %d volume to 'ogain' device? (y/n/q) ", itmp);
			zn = mgetch();
			if (zn == 'q') return;
			if (zn == 'y')
			{
				sprintf(temp, "mixer -f %s ogain %d 1> /dev/null 2> /dev/null", dspout, itmp);
				system(temp);
			}

		}
		else printf("Output mixer skipped\n");
	}

	zn = 0;
	while (zn != 'y' && zn != 'n')
	{
		printf("Kill running cvoice processes? (y/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 'y') 
	{
	    	system("killall cvoice cvoiceraw cvoicedbg echosrv");
	    	system("killall -9 cvoice cvoiceraw cvoicedbg echosrv");
	}

	zn = 0;
	while (zn != 'y' && zn != 'n')
	{
		printf("Remove logs? (y/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 'y') system("rm -f /tmp/cvoice*.log");


	printf("Final options \"%s\"\n", options);


	zn = 0;
	while (zn != 'y' && zn != 'n')
	{
		printf("GO? (y/n/q) ");
		zn = mgetch();
		if (zn == 'q') return;
	}

	if (zn == 'y') system(options);
	// remove logs?
	// kill running process?
}

int main(int lb, char** par)
{
    	wizard();
    	return 0;
}

