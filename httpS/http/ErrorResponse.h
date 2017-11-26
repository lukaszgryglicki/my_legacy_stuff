#ifndef ERROR_RESPONSE_H
#define ERROR_RESPONSE_H

#include <string>
#include <iostream>
#include "HttpRequest.h"
#include "HttpResponse.h"

class ErrorResponse : public HttpResponse{
	HttpRequest request;
	std::string statusLine;
public:
	ErrorResponse(const HttpServer &serv, const HttpRequest &r,std::string status)
		:HttpResponse(serv),request(r),statusLine(status)
	{	
		headers.set("Content-Type","text/html");
	}
	virtual std::ostream& print(std::ostream &out) const {
		printStatus(out,statusLine);
		printHeaders(out);
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		    << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"DTD/xhtml1-strict.dtd\">"
		    << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">"
		    << "<head><title>" << statusLine << "</title></head>"
		    << "<body><h1>" << statusLine << "</h1>"
		    << "<p>The server was unable to compleate your request.</p>"
		    << "<p>Your request was parsed as follows:</p><pre>"
		    << request 
		    << "</pre></body></html>" << std::endl;
		return out;
	}
};

#endif//ERROR_RESPONSE_H
