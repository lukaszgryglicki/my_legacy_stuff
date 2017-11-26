#ifndef SOCKETSTREAM_H
#define SOCKETSTREAM_H

#include <streambuf>	//streambuf
#include <iostream>	//iostream
#ifdef WIN32
	#include <winsock2.h>
#else
	#include <sys/types.h>	//send,recv
	#include <sys/socket.h>	//send,recv
	#define SOCKET int
#endif

//Minimal implementation, thought when used
//by a well writen iostream should be fast
//thanks to xputn, xgetn.
class socketbuf : public std::streambuf{
protected:
	const static int READBUF_SIZE=512;
	SOCKET sockfd;
	char readbuf[READBUF_SIZE];
public:
	socketbuf(SOCKET socketfd) {
		sockfd = socketfd;
		//using only the read buffer (because there is no other way)
		setg(readbuf,readbuf,readbuf);//set input buffer and mark it empty (start=cur=end)
	}
	virtual ~socketbuf(){}
	virtual int overflow(int c = EOF){
		if(c != EOF)
			if(send(sockfd,(const char*)(&c),1,0) != 1)
				return EOF;
		return c;
	}
	virtual int underflow(){
		if(gptr()!=0 && gptr()<egptr()){
			return *gptr();
		}else{
			int count = recv(sockfd,readbuf,READBUF_SIZE,0);
			if(count <= 0)
				return EOF;
			setg(readbuf,readbuf,readbuf+count);
			return *gptr();
		}
	}
	virtual int xsputn(const char *s, int n){
		int ret = send(sockfd,s,n,0);
		if(ret == -1)
			return EOF;
		return ret;
	}
	virtual int xsgetn(char *s, int n){
		int ret = recv(sockfd,s,n,0);
		if(ret == -1)
			return EOF;
		return ret;
	}
};

class iosocketstream : public std::iostream {
protected:
	socketbuf buf;
public:
	iosocketstream(SOCKET socketfd) : 
		std::iostream(&buf),
		buf(socketfd) { }
};

#endif//SOCKETSTREAM_H
