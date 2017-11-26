#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <sstream>
#include <string>

class StringUtils{
public:
	static std::string itos(int a){ 
		std::ostringstream stream;
		stream << a;
		return stream.str(); 
	} 
};

#endif//STRING_UTILS_H
