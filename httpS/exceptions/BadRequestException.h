#ifndef BAD_REQUEST_EXCEPTION_H
#define BAD_REQUEST_EXCEPTION_H

#include "HttpException.h"

class BadRequestException: public HttpException{
public:
	BadRequestException(): HttpException("400 Bad Request"){ }
	virtual ~BadRequestException() throw(){}
};

#endif
