#ifndef MULTI_THREADING_EXCEPTION_H
#define MULTI_THREADING_EXCEPTION_H

#include <string>
#include "Exception.h"

class MultiThreadingException: public Exception{
public:
	MultiThreadingException():Exception(){ }
	MultiThreadingException(std::string message):Exception(message){ }
	virtual ~MultiThreadingException() throw(){}
};

#endif//MULTI_THREADING_EXCEPTION_H
