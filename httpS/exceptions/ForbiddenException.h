#ifndef FORBIDDEN_EXCEPTION_H
#define FORBIDDEN_EXCEPTION_H

#include "HttpException.h"

class  ForbiddenException: public HttpException{
public:
	ForbiddenException(): HttpException("403 Forbidden"){ }
	virtual ~ForbiddenException() throw(){}
};

#endif
