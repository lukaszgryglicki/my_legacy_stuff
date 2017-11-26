#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <iostream>	//ostream
#include "HttpRequest.h"
#include "HttpServer.h"

class HttpResponse{
protected:
	const HttpServer &server;
	HttpHeaders headers;
	std::ostream& printStatus(std::ostream &out, std::string status) const{
		return out << "HTTP/1.1 " << status << CRLF << std::flush;
	}
	std::ostream& printHeaders(std::ostream &out) const{
		return out << headers << CRLF << std::flush;
	}
public:
	HttpResponse(const HttpServer &serv);
	virtual ~HttpResponse(){};
	static HttpResponse* respond(const HttpServer &serv, const HttpRequest &request);
	virtual std::ostream& print(std::ostream &out) const = 0;
};

inline std::ostream& operator<<(std::ostream &out, const HttpResponse &response){
	return response.print(out);
}

#endif//HTTP_RESPONSE_H
