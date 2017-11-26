#ifndef SOCKET_H
#define SOCKET_H

#include "socketstream.h"
#include "../exceptions/SocketException.h"
#include "../utils/portability.h"

//This class is introduced to hide portability issues concernig sockets
class Socket{
public:
	typedef SOCKET SocketFD;
private:
	SocketFD sockfd;
	iosocketstream stream;
public:
	Socket(): 
		sockfd(socket(AF_INET,SOCK_STREAM,0)), 
		stream(sockfd)
	{
		if ( sockfd == INVALID_SOCKET)
			throw SocketException();
	}
	Socket(SocketFD socket):
		sockfd(socket),
		stream(sockfd)
	{ }
	Socket(const Socket &sock):
		sockfd(sock.sockfd),
		stream(sock.sockfd)
	{ }
	SocketFD getFD(){
		return sockfd;
	}
	void close(){
		if(::closesocket(sockfd) == -1)
			throw SocketException();
	}
	std::iostream& getIOStream(){
		return stream;
	}
};

#endif//SOCKET_H

