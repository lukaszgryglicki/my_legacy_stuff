#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <iostream>	//istream
#include <string>

#include "URI.h"
#include "HttpHeaders.h"
#include "HttpServer.h"

class HttpRequest{
	std::string method,version;
	URI uri;
	HttpHeaders headers;
public:
	HttpRequest(std::string m="",std::string u="", std::string v="")
		: method(m), uri(u), version(v)
	{ }
	void setHeaders(const HttpHeaders &headers){
		HttpRequest::headers = headers;
	}
	const URI& getURI() const{
		return uri;
	}
	std::string getMethod() const{
		return method;
	}
	std::string getVersion() const{
		return version;
	}
	const HttpHeaders& getHeaders() const{
		return headers;
	}
	friend std::ostream& operator<<(std::ostream &out, const HttpRequest &request);
};

inline std::istream& operator>>(std::istream &in, HttpRequest &request){
	HttpHeaders headers;
	std::string method,uri,version;
	in >> std::uppercase 
	//A HTTP request consists of:
		>> method >> uri >> version;
	in.get();//CR
	in.get();//LF
	in >> headers;
	request = HttpRequest(method,uri,version);
	request.setHeaders(headers);
	return in;
}

inline std::ostream& operator<<(std::ostream &out, const HttpRequest &request){
	return out << request.method << ' ' << request.uri.getAbsPath() << ' ' << request.version << CRLF 
	           << request.headers << CRLF << std::flush;
}

#endif//HTTP_REQUEST_H
