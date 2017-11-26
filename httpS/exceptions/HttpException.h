#ifndef HTTP_EXCEPTION_H
#define HTTP_EXCEPTION_H

#include <string>
#include <exception>

class HttpException: public Exception{
public:
	HttpException(std::string message):Exception(message) {}
	virtual ~HttpException() throw(){}
};

#endif//HTTP_EXCEPTION_H
