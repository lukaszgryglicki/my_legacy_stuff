#include <exception>
#include <iostream>
#include "http/HttpServer.h"
#include "exceptions/SocketException.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int ac,char **argv){
	int port=8080;
	if(ac==2)
		port = atoi(argv[ac-1]);
	try{
		HttpServer server(".",port);
		server.run();
	}catch(const SocketException &ex){
		cerr << "SocketExecpion cought: " << ex.what() <<endl;
	}catch(const std::exception &ex){
		cerr << "Execpion cought: " << ex.what() <<endl;
	}catch(...){
		cerr << "Unhandled exception!"<<endl;
	}
	return 0;
}
