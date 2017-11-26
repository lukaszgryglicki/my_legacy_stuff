#include "URI.h"
/* RFC2396:
   [quote]
   The following line is the regular expression for breaking-down a URI
   reference into its components.

      ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
       12            3  4          5       6  7        8 9

   The numbers in the second line above are only to assist readability;
   they indicate the reference points for each subexpression (i.e., each
   paired parenthesis).  We refer to the value matched for subexpression
   <n> as $<n>.  For example, matching the above expression to
   
      http://www.ics.uci.edu/pub/ietf/uri/#Related
      
   results in the following subexpression matches:
   
      $1 = http:
      $2 = http
      $3 = //www.ics.uci.edu
      $4 = www.ics.uci.edu
      $5 = /pub/ietf/uri/
      $6 = <undefined> 
      $7 = <undefined> 
      $8 = #Related 
      $9 = Related  
      
   where <undefined> indicates that the component is not present, as is
   the case for the query component in the above example.  Therefore, we
   can determine the value of the four components and fragment as
      
      scheme    = $2
      authority = $4
      path      = $5
      query     = $7
      fragment  = $9
	[unquote]
*/
//-------------------------------------------------------------------------
/*Somthing like 
	unsigned pos=-1;
	for(int i=0;i<delims.length();i++)
		pos = min(pos, s.indexOf(delims[i]);
	return pos;
but faster.
*/
std::string::size_type URI::indexOfAny(std::string s, std::string delims){
	std::string::size_type i=0;
	for(i=0;i<s.length();i++){
		for(std::string::size_type j=0;j<delims.length();j++)
			if(s[i]==delims[j])
				return i;
	}
	return std::string::npos;
}
//-------------------------------------------------------------------------
void URI::parseAuthority(std::string authority){
	std::string::size_type j = authority.find('@');
	if(j!=std::string::npos){
		//userinfo = authority.substr(0,j);
		authority.erase(0,j+1);
	}
	std::string::size_type k = authority.find(':');
	if(k!=std::string::npos){
		host = authority.substr(0,k);
		//authority.erase(0,k);
		//port = authority;
	}else
		host = authority;
}
//-------------------------------------------------------------------------
void URI::init(std::string uri){
	std::string::size_type i;
	i = uri.find(':');
	if(i!=std::string::npos){
		//schema = uri.substr(0,i);
		uri.erase(0,i+1);//$1
	}
	if(uri.substr(0,2)=="//")
		uri.erase(0,2);
	i = indexOfAny(uri,"/?#");
	if(i!=std::string::npos){
		parseAuthority(uri.substr(0,i));
		uri = uri.substr(i);
		i = indexOfAny(uri,"?#");
		if(i!=std::string::npos){
			absPath = uri.substr(0,i);
			//uri = uri.substr(i);
		}else
			absPath = uri;
	}else{
		parseAuthority(uri);
		absPath = "/";
	}
}
//-------------------------------------------------------------------------

