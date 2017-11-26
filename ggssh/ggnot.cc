#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <libgadu.h>

#define glob_sendfrom_uin 0
#define glob_sendfrom_pass "haslo"
#define glob_sendto_uin   0

char mhost[256];

int ggmsg(char* m)
{
	struct gg_session *ggs;
	int l;

#ifdef MDBG
	gg_debug_level = 255;
#else
	gg_debug_level = 0;
#endif
	gg_login_params gglp;

	memset((void*)(&gglp), 0, sizeof(gglp));

	gglp.password = (char*)malloc(20);
	gglp.uin = glob_sendfrom_uin;
	gglp.async = 0;
	gglp.status = GG_STATUS_AVAIL;
	strcpy(gglp.password, glob_sendfrom_pass);

	if (!(ggs = gg_login(&gglp)))
	{
		return -1;
	}

	gg_notify(ggs, NULL, 0);

	unsigned char* msg;
	int r = 0;

	l = strlen( m );
	if (l < 1900)
	{
		msg = (unsigned char*)malloc(l+1);
		strcpy((char*)msg, m);

		r = gg_send_message(ggs, GG_CLASS_CHAT, glob_sendto_uin, msg);

		free( msg );
	}
	else
	{
		msg = (unsigned char*)malloc(1901);

		for (int x=0;x<l;x+=1900)
		{
		    strncpy((char*)msg, &m[x], 1900);
		    msg[1900] = 0;
		    r = gg_send_message(ggs, GG_CLASS_CHAT, glob_sendto_uin, msg);
		}

		free( msg );
	}


	gg_logoff(ggs);
	gg_free_session( ggs );

	free( gglp.password );

	return r;
}

char* readfile(FILE* fp)
{
    int l;
    char* s;

    fseek(fp, 0, SEEK_END);
    l = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (!l) return NULL;

    s = (char*)malloc(l+1);
    if (!s) return NULL;

    fread((void*)s, l, 1, fp);
    s[l] = 0;

    return s;
}

int ggremote(char* scmd)
{
    	FILE* fp;
    	char* cmd;
	char* s;
	char tt[256];
	int uid = getuid();

	sprintf(tt, "rm -f /tmp/gge_%d.dat", uid);
	system(tt);

    	cmd = (char*)malloc(strlen(scmd) + 128);

    	sprintf(cmd, "%s 1>/tmp/gge_%d.dat 2>/tmp/gge_%d.dat", scmd, uid, uid);
    	system(cmd);
    
	sprintf(tt, "/tmp/gge_%d.dat", uid);
	fp = fopen(tt, "r");
	//printf("%s --> %p\n", cmd, fp);

	if (!fp) 
	{
		sprintf(cmd, "%s%s\nNot found\n", mhost, scmd);
		if (ggmsg(cmd) < 0) 
		{
		    	free(cmd );
		    	return -2;
		}

		free( cmd );
		return 0;
	}

	s = readfile(fp);
	fclose(fp);

	if (!s) 
	{
		sprintf(cmd, "%s%s\nNo terminal output\n", mhost, scmd);
		if (ggmsg(cmd) < 0) 
		{
		    	free(cmd );
		    	return -3;
		}

		free( cmd );
		return 0;
	}

	free( cmd );

	cmd = (char*)malloc(strlen(s) + strlen(mhost) + strlen(scmd) + 10);
	sprintf(cmd, "%s%s\n%s", mhost, scmd, s);

	free(s);

	if (ggmsg(cmd) < 0) 
	{
	    	free(cmd );
	    	return -4;
	}

	free( cmd );

	return 1;
}

int send_infos()
{
	FILE *fp, *fp2;
	char iname[128];
	char cmd[256];
	char ip[64];
	char sfinal[1024];
	char *s, *s2;
	int uid = getuid();

	//strcpy(cmd, "ps");
	//if (ggremote(cmd) < 0) return -5;

	//strcpy(cmd, "ifconfig");
	//if (ggremote(cmd) < 0) return -5;

	//strcpy(cmd, "uptime");
	//if (ggremote(cmd) < 0) return -5;

	//strcpy(cmd, "df -H");
	//if (ggremote(cmd) < 0) return -5;

	strcpy(sfinal, mhost);
	sprintf(cmd, "ifconfig -l > /tmp/ggnot_%d.dat", uid);
	system( cmd );

	sprintf(cmd, "/tmp/ggnot_%d.dat", uid);
	fp = fopen(cmd, "r");

	if (!fp) return -1;

	while (fscanf(fp, "%s", iname) == 1)
	{
	    	sprintf(cmd, "ifconfig %s > /tmp/ggnot2_%d.dat", iname, uid);
	    	system(cmd);

		sprintf(cmd, "/tmp/ggnot2_%d.dat", uid);
	    	fp2 = fopen(cmd, "r");
	    	if (!fp2) 
		{
		    fclose(fp);
		    return -2;
		}

	    	s = readfile(fp2);
	    	if (!s) 
		{
		    fclose(fp);
		    return -3;
		}

	    	fclose(fp2);

		//if (ggmsg(s) < 0) return -4;

		s2 = strstr(s, "inet ");

		if (s2)
		{
			sscanf(s2, "inet %s", ip);
			sprintf(cmd, "\n%s: %s", iname, ip);
			strcat(sfinal, cmd);
		}

		free(s);
	}

	fclose(fp);

	sprintf(cmd, "rm -f /tmp/ggnot_%d.dat /tmp/ggnot2_%d.dat", uid, uid);
	system( cmd );

	strcat(sfinal, "\n");
	if (ggmsg(sfinal) < 0) return -4;

	return 1;
}

void ggnotifier()
{
    	
	int pid = fork();

	if (pid > 0) return;
	else if (pid < 0)
	{
	    printf("Error forking\n");
	    return;
	}

	gethostname(mhost, 250);
	strcat(mhost, "> ");

	//ggmsg( mhost );

	while (1)
	{
	    if (send_infos() == 1) sleep( 10800 );
	    else 
	    {
		printf("Send info error, retrying after 30s");
		sleep( 30 );
	    }
	}
}

void ggexec(int lb, char** par)
{
	int i, l;

	l = 0;
	for (i=1;i<lb;i++)
	{
		l += strlen(par[i]) + 2;
	}

	l += 4;

	char* s = (char*)malloc(l);
	strcpy(s, "");

	for (i=1;i<lb;i++)
	{
		strcat(s, par[i]);
		strcat(s, " ");
	}

	s[strlen(s)-1] = 0;
	
	gethostname(mhost, 250);
	strcat(mhost, "> ");

	int err;
	if ((err = ggremote(s)) < 0)
	{
		printf("Remote GG error: %d\n", err);
	}

	free( s );
}

int main(int lb, char** par)
{
#ifdef GGEXEC
    	ggexec(lb, par);
#else
	ggnotifier();
#endif
	return 0;
}

