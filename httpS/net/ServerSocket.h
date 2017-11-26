#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "Socket.h"

class ServerSocket{
	Socket socket;
public:
	ServerSocket(int port);
	void close();
	Socket accept();
};

#endif//SERVER_SOCKET_H
