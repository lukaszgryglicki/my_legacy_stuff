#ifndef GET_RESPONSE_H
#define GET_RESPONSE_H

#include <string>
#include <iostream>
#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HEADResponse.h"

class GETResponse : public HEADResponse{
public:
	GETResponse(const HttpServer &serv, const HttpRequest &r)
		: HEADResponse(serv,r)
	{}
	virtual std::ostream& print(std::ostream &out) const {
		HEADResponse::print(out);
		out << file;
		return out;
	}
};

#endif//HEAD_RESPONSE_H
