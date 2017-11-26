#ifndef URI_H
#define URI_H

#include <string>

class URI{
	std::string host,absPath;
	std::string::size_type indexOfAny(std::string s, std::string delims);
	void parseAuthority(std::string authority);
	void init(std::string uri);
public:
	URI(std::string uri="/"){	
		init(uri);
	}
	std::string getHost() const{
		return host;
	}
	std::string getAbsPath() const{
		return absPath;
	}
};

#endif//URI_H
