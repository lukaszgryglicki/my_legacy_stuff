#include "HttpResponse.h"
#include <iostream>	//ostream
#include <time.h>

#include "HttpRequest.h"
#include "HttpServer.h"
#include "ErrorResponse.h"
#include "HEADResponse.h"
#include "GETResponse.h"
#include "../exceptions/HttpException.h"
//----------------------------------------------------------------
HttpResponse::HttpResponse(const HttpServer &serv)
	: server(serv)
{
	//adding required headers
	//No Keep-Alive:
	headers.add("Connection","close");
	//date must be in RFC1123 format:
	const char* RFC1123FMT="%a, %d %b %Y %H:%M:%S GMT";
	time_t now = time(0); 
	char timebuf[100];
 	strftime( timebuf, sizeof(timebuf), RFC1123FMT, gmtime( &now ) );
	headers.add("Date",timebuf);
}
//----------------------------------------------------------------
HttpResponse* HttpResponse::respond(const HttpServer &serv, const HttpRequest &request){
	/*	"A server that does not understand or is unable to comply with any of
		the expectation values in the Expect field of a request MUST respond
		with appropriate error status. The server MUST respond with a 417
		(Expectation Failed) status if any of the expectations cannot be met
		or, if there are other problems with the request, some other 4xx
		status."
	*/
	if(!request.getHeaders().get("Expect").empty())
		return new ErrorResponse(serv,request,"417 Expectation Failed");
	/* "All Internet-based HTTP/1.1 servers MUST respond with a 400 (Bad Request)
	   status code to any HTTP/1.1 request message which lacks a Host header
	   field."
	*/
	if(request.getHeaders().get("Host").empty() && request.getVersion()=="HTTP/1.1")
		return new ErrorResponse(serv,request,"400 Bad Request");
	try{
		if(request.getMethod() == "GET")
			return new GETResponse(serv,request);
		else if(request.getMethod() == "HEAD")
			return new HEADResponse(serv,request);
		else 
			return new ErrorResponse(serv,request,"501 Not Implemented");
	}catch(const HttpException &ex){
		return new ErrorResponse(serv,request,ex.what());
	}
}

