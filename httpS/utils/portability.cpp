#include "portability.h"

#include "../exceptions/MultiThreadingException.h"
#include "../http/HttpServer.h"
#include "../net/Socket.h"

#ifdef WIN32
	CRITICAL_SECTION critical_section;

	std::string uriToLocalPath(std::string uriPath)  {
		std::string local;
		for(std::string::size_type i=0;i<uriPath.length();i++)
			if(uriPath[i]=='/')
				local += "\\";
			else 
				local += uriPath[i];
		return local;
	}

	extern void beginThread(HttpServer &server, Socket &socket);

#else //linux

	void beginThread(HttpServer &server, Socket &socket){
		const int pid = fork();
		if(pid < 0)
			throw MultiThreadingException();
		else if(pid==0){//child
			server.serverSocket.close();//not needed by the child
			HttpServer::processClient(server,socket);
			close(socket.getFD());
			exit(0);
		}
		//parent socket
		close(socket.getFD());
	}

#endif//WIN32

