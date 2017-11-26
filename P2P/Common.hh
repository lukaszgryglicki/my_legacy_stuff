#ifndef __HAVE_COMMON_H__
#define __HAVE_COMMON_H__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdarg.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <fnmatch.h>
#include <sys/limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <aio.h>


//String coding throught the NET
struct String
{
 int length;
 char* string;
};

#define DEFAULT_N		10	//desired number of connections
#define DEFAULT_UPLOAD_DIR	"./"
#define DEFAULT_DOWNLOAD_DIR	"./"
#define DEFAULT_PORT		7782
#define DEFAULT_HOSTS_FILE	"./known_hosts"
#define DEFAULT_TIMEOUT		3		//FOR ALARMS EXAMPLE IN connect operations etc
#define MAX_SERVER_NAME		512		//Used to store server names
#define MAX_PORT		0x7FFF		//maximum is 0x7FFF because other uses port+0x8000, so real max is 0xFFFF
#define MAX_LINE_LENGTH		1024		//used to get line from user in console
#define HERE			__FILE__,__LINE__
#define PRESERVE_INTERVAL	30		//timeout for respawning connections if needed
#define PING_INTERVAL	   	5		//ping connected server every ... seconds	
#define BUF_SIZE		1024		//used somewhere
#define AIO_BUFSIZ		1048576		//1M (one operation is big, and preparations no matters)
#endif

