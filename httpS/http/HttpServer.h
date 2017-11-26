#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include <iostream>
#include <fstream>

#include "../net/ServerSocket.h"
#include "../net/Socket.h"

const char CR=static_cast<char>(13);
const char LF=static_cast<char>(10);
const char CRLF[] = { CR,LF,0 };
#define defaultFilename "index.html"

class HttpServer { 
public:
	class File{
		const std::string webRoot;
		std::string size,mime,lastModified,local;
		void calcFileInfo();
		std::string calcMime();
	private:
		File(std::string webroot,std::string filename);
	public:
		std::string getSize() const{
			return size;
		}
		std::string getMime() const{
			return mime;
		}
		std::string getLastModified() const{
			return lastModified;
		}
		std::string getLocal() const{
			return local;
		}
		friend class HttpServer;
	};
protected:
	const std::string webRoot;
	ServerSocket serverSocket;
	std::ostream& log;
public: 
	static void processClient(HttpServer &server,Socket &socket);
	HttpServer(std::string webroot,int port, std::ostream &logstream = std::cout);
	~HttpServer(){
		DEL_CRITICAL_SECTION
	}
	void run(); //this function never returns
	File openFile(std::string webPath) const;

	friend void beginThread(HttpServer &server, Socket &socket);
}; 

extern std::ostream& operator<<(std::ostream &out, const HttpServer::File &file);

#endif//HTTP_SERVER_H
