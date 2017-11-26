#ifndef PORTABILITY_H
#define PORTABILITY_H

#ifdef WIN32

	#include <winsock2.h>
	#include <string>

	#define stat _stat
	#define getLastError() (strerror(errno))
	#define getLastSocketError() (strerror(errno))
	#define S_ISDIR(arg) (arg & _S_IFDIR)
	#define R_OK 4
	
	extern std::string uriToLocalPath(std::string uriPath);

	extern CRITICAL_SECTION critical_section;
	#define INIT_CRITICAL_SECTION InitializeCriticalSection(&critical_section);
	#define ENTER_SYNCRONIZED EnterCriticalSection(&critical_section);
	#define LEAVE_SYNCRONIZED LeaveCriticalSection(&critical_section);
	#define DEL_CRITICAL_SECTION DeleteCriticalSection(&critical_section);

#else //linux

	#include <unistd.h>
	#include <string.h>	//strerror
	#include <errno.h>

	#define INIT_CRITICAL_SECTION 
	#define ENTER_SYNCRONIZED 
	#define LEAVE_SYNCRONIZED 
	#define DEL_CRITICAL_SECTION 

	#define SOCKET int
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define closesocket(arg) close(arg)
	#define getLastError() (strerror(errno))
	#define getLastSocketError() (strerror(errno))
	#define uriToLocalPath(arg) (arg)

#endif

class HttpServer;
class Socket;
extern void beginThread(HttpServer &server, Socket &socket);

#endif//PORTABILITY_H
