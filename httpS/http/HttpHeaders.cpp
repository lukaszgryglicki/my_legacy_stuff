#include "HttpHeaders.h"
#include "HttpServer.h" //CRLF,...
//-----------------------------------------------------------------------
//Read all the HTTP headers from the istream
//Headers are ('string' may contain quotes):
//string : string CRLF
std::istream& operator>>(std::istream &in, HttpHeaders &headers){
	std::string pair[2]; //a pair of name and value
	std::string &name = pair[0], &value=pair[1];
	name.reserve(255);
	value.reserve(255);
	bool inQuote = false;
	int current=0;//name comes first
	char c;
	while(in){
		c = in.get();
		switch(c){
			case ':':
					if(!inQuote)
						if(current==0)
							current=1;
						else
							pair[current]+=c;
					break;
			case LF:
					if(!inQuote){
						std::string &cur =pair[current];
						if(!cur.empty() && cur[cur.length()-1]==CR){
							if(current==0){
								//we've got a line consiting only of CRLF
								//this means it's the end of headers
								return in;
							}else{
								//end of this header-line
								value.erase(value.length()-1);//drop the CR
								headers.add(name,value);	
								name.clear();
								value.clear();
								current=0;
							}
						}
					}
					break;
			case '"': 
					inQuote = !inQuote; 
			default:
					pair[current]+=c;
					break;
		}
	}
	return in;
}
//-----------------------------------------------------------------------
std::ostream& operator<<(std::ostream &out, const HttpHeaders &headers){
	for(HttpHeaders::stringmap::const_iterator i=headers.headers.begin(); i!=headers.headers.end(); i++)
		out << i->first << ":" << i->second << CRLF;
	return out;
}
//-----------------------------------------------------------------------
