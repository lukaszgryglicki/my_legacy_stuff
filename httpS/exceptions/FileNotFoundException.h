#ifndef FILE_NOT_FOUND_EXCEPTION_H
#define FILE_NOT_FOUND_EXCEPTION_H

#include "HttpException.h"

class  FileNotFoundException: public HttpException{
public:
	FileNotFoundException(): HttpException("404 Not Found"){ }
	virtual ~FileNotFoundException() throw(){}
};

#endif
