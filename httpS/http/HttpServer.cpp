#include "HttpServer.h"

#include <string.h>	//strerror
#include <sys/types.h>  //stat	
#include <sys/stat.h>   //stat
#ifdef WIN32
	#include <winsock2.h>
	#include <fcntl.h>	//access,stat
	#include <io.h>		//access,stat
#else
	#include <unistd.h>	//access
#endif
#include <errno.h>	//errno
#include <ctype.h>	//tolower
#include <time.h>

#include <string>
#include <fstream>

#include "../net/socketstream.h"
#include "../net/ServerSocket.h"
#include "../exceptions/MultiThreadingException.h"
#include "../exceptions/SocketException.h"
#include "../exceptions/FileNotFoundException.h"
#include "../exceptions/ForbiddenException.h"
#include "../exceptions/BadRequestException.h"
#include "../utils/StringUtils.h"
#include "../utils/portability.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

//----------------------------------------------------------------
void HttpServer::File::calcFileInfo(){
	if(access(local.c_str(),R_OK)==-1){
		switch(errno){
			case ENOENT:
				throw FileNotFoundException();
			case EACCES:
			default:
				throw ForbiddenException();
		}
	}
	struct stat st;
	if(stat(local.c_str(),&st)==-1)
		throw ForbiddenException();
	if(S_ISDIR(st.st_mode))
		throw FileNotFoundException();
	size = StringUtils::itos(static_cast<int>(st.st_size));

	char timebuf[100];	
	const char* RFC1123FMT="%a, %d %b %Y %H:%M:%S GMT";
	strftime(timebuf,sizeof(timebuf),RFC1123FMT,gmtime(&st.st_mtime));
	lastModified = timebuf;
}
//----------------------------------------------------------------
std::string HttpServer::File::calcMime(){
	int dotPos = static_cast<int>(local.rfind('.'));
	std::string extension;
	if(dotPos>0){
		for(int i=dotPos+1;i<static_cast<int>(local.length());i++)
			extension += tolower(local[i]);
	}else //no extension
		return "text/plain";
	if(extension=="html" || extension=="htm")
		return "text/html";
	else if(extension=="jpeg" || extension=="jpg")
		return "image/jpeg";
	else if(extension=="gif")
		return "image/gif";
	else if(extension=="png")
		return "image/png";
	else if(extension=="css")
		return "text/css";
	//If the media type remains unknown, the recipient SHOULD
	//treat it as type "application/octet-stream".
	else 
		return "application/octet-stream";
}
//----------------------------------------------------------------
HttpServer::File::File(std::string webroot, std::string filename)
	:webRoot(webroot)
{
	if(filename.empty() || 
	   filename.find("..")!=std::string::npos || 
           filename[0]!='/')
	{
		throw BadRequestException();
	}
	local = webRoot + uriToLocalPath(filename);
	//URIs pointing to directiries must end with a /
	//(otherwise I'll throw an Exception)
	if(filename[filename.length()-1] == '/')
		local += defaultFilename;
	calcFileInfo();
	mime = calcMime();
}
//----------------------------------------------------------------
//The main server loop
void HttpServer::run(){
	while(true){
		Socket clientSocket = serverSocket.accept();
		beginThread(*this,clientSocket);
	}
	serverSocket.close();
}
//----------------------------------------------------------------
void HttpServer::processClient(HttpServer &server,Socket &socket){
	std::iostream& stream = socket.getIOStream();
	HttpRequest request;
	stream >> request;
	ENTER_SYNCRONIZED
	server.log << "Recieved a " << request.getMethod() <<" request for [" 
		<< request.getURI().getAbsPath() 
		<< "] from [" << request.getHeaders().get("Host") << "] ("
		<< request.getVersion() << ")" << std::endl;
	LEAVE_SYNCRONIZED
	std::auto_ptr<HttpResponse> response(HttpResponse::respond(server,request));
	stream << (*response);
	socket.close();
}
//----------------------------------------------------------------
HttpServer::HttpServer(std::string webroot,int port, std::ostream &logstream) 
	: webRoot(webroot),serverSocket(port),log(logstream)
{ 
	INIT_CRITICAL_SECTION
	log << "HttpServer started on port "
	<< port << " with web root set to '"<<webRoot<<"'"<<std::endl;
}
//----------------------------------------------------------------
HttpServer::File HttpServer::openFile(std::string webPath) const{
	File file(webRoot, webPath);
	ENTER_SYNCRONIZED
	log << "Reading file: [" << file.getLocal() << "] "
	"size="<<file.getSize()<<" mime="<<file.getMime() <<std::endl;
	LEAVE_SYNCRONIZED
	return file;
}
//----------------------------------------------------------------
std::ostream& operator<<(std::ostream &out, const HttpServer::File &file){
#ifdef WIN32
	//VC++ has HORRIBLE ostream::operator<<(streambuf&) implementation
	//It reads data ONE BYTE at a time. No buffer, no calls to streambuf::xsputn
	//Another thing, VC++'s ifstream can't handle binnary data!
	//For reading a binnary file you must use fopen with _setmode. Makes me sick...
	//As for GCC it fails to provide a working
	// ostream operator<<(ostream&, const streambuf&)
	//(it doesn't copy any data, if I remember correctly)
/*
	// This should be writen like this:
	std::ifstream fin(file.getLocal().c_str());
	out << fin.rdbuf();
	fin.close();
*/
	const int BUF_SIZE=512;
	char buf[BUF_SIZE];
	FILE *f = fopen(file.getLocal().c_str(),"r");
	_setmode(_fileno(f),_O_BINARY);
	size_t count;
	while(!feof(f) && !ferror(f)){
		count = fread(buf,sizeof(char),BUF_SIZE,f);
		out.rdbuf()->sputn(buf,static_cast<std::streamsize>(count));
	}
	fclose(f);
#else
	std::ifstream fin(file.getLocal().c_str());
	while(fin)
		out.put(fin.get());
	fin.close();
#endif
	return out;
}
