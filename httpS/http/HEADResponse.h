#ifndef HEAD_RESPONSE_H
#define HEAD_RESPONSE_H

#include <string>
#include <iostream>
#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

class HEADResponse : public HttpResponse{
protected:
	HttpServer::File file;
	HttpRequest request;
public:
	HEADResponse(const HttpServer &serv, const HttpRequest &r)
		:HttpResponse(serv),
		 request(r),
		 file(serv.openFile(r.getURI().getAbsPath()))
	{	
		headers.set("Content-Length",file.getSize());
		headers.set("Content-Type",file.getMime());
		headers.set("Last-Modified",file.getLastModified());
	}
	virtual std::ostream& print(std::ostream &out) const {
		printStatus(out,"200 OK");
		printHeaders(out);
		return out;
	}
};

#endif//HEAD_RESPONSE_H

