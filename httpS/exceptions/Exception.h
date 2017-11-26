#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <exception>
#include "../utils/portability.h"

class Exception: public std::exception{
protected:
	std::string message;
public:
	Exception(){
		message = getLastError();
	}
	Exception(std::string message){
		Exception::message = message;
	}
	virtual ~Exception() throw(){}
	virtual const char* what() const throw(){
		return message.c_str();
	}
};
#endif//EXCEPTION_H
