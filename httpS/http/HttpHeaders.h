#ifndef HTTP_HEADERS_H
#define HTTP_HEADERS_H

#include <string>
#include <iostream>	//istream
#include <map>	

class HttpHeaders{
	typedef std::map<std::string,std::string> stringmap;
	stringmap headers;
public:
	void add(std::string name, std::string value){
		headers[name] += value + ' ';
	}
	void set(std::string name, std::string value){
		headers[name] = value;
	}
	std::string get(std::string name) const{
		stringmap::const_iterator result = headers.find(name);
		return (result==headers.end()) ? "" : result->second;
	}
	friend std::ostream& operator<<(std::ostream &our, const HttpHeaders &headers);
};

extern std::istream& operator>>(std::istream &in, HttpHeaders &headers);
extern std::ostream& operator<<(std::ostream &out, const HttpHeaders &headers);

#endif//HTTP_HEADERS_H
