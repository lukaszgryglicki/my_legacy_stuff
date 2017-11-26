#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H

#include <string>
#include <exception>
#include "Exception.h"
#include "../utils/portability.h"

class SocketException: public Exception{
public:
	SocketException():Exception(getLastSocketError()){}
	SocketException(std::string message):Exception(message){ }
	virtual ~SocketException() throw(){}
};

#endif//SOCKET_EXCEPTION_H
