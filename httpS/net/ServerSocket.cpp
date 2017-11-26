#include "ServerSocket.h"

#include <string.h>	//memset,strerror
#ifdef WIN32
	#include <winsock2.h>
#else
	#include <sys/types.h>	//socket,accept,bind
	#include <sys/socket.h>	//socket,accept,bind
	#include <netinet/in.h>	//sockaddr_in,htons,htonl
#endif
#include "../exceptions/SocketException.h"
//--------------------------------------------------------------------------
ServerSocket::ServerSocket(int port){
	struct sockaddr_in serv_addr;
	char on=1;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port        = htons(port);
	setsockopt(socket.getFD(),SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if(bind(socket.getFD(), (struct sockaddr *) &serv_addr, sizeof(serv_addr))==SOCKET_ERROR)
		throw SocketException();
	if(listen(socket.getFD(),5) == SOCKET_ERROR)
		throw SocketException();
}
//--------------------------------------------------------------------------
void ServerSocket::close(){
	socket.close();
}
//--------------------------------------------------------------------------
Socket ServerSocket::accept(){
	Socket::SocketFD newsockfd = ::accept(socket.getFD(), 0,0);
	if (newsockfd == INVALID_SOCKET)
		throw SocketException();
	return newsockfd;
}
//--------------------------------------------------------------------------
