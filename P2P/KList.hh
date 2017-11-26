#ifndef __HAVE_KLIST_H__
#define __HAVE_KLIST_H__
#include "Common.hh"
//Stores lists of servers/filenames/uploads/downloads
class KList
{
 public:
	 KList();
	 KList(KList*);
	 KList(char*);
	 KList(struct sockaddr_in*);
	 ~KList();
	 int GetPort() const;
	 in_addr_t GetAddr() const;
	 char* GetName() const;
	 char* GetDescr() const;
	 char* GetStringIP() const;
	 char* GetFileName() const;
	 KList* Next() const;
	 int GetSocket() const;
	 off_t GetFileLength() const;
	 off_t GetTransferred() const;
	 void SetPort(int);
	 void SetAddr(in_addr_t);
	 void SetName(char*);
	 void SetFileName(char*);
	 void SetDescr(char*);
	 void SetStringIP(char*);
	 void SetNext(KList*);
	 void SetSocket(int);
	 void SetFileLength(off_t);
	 void SetTransferred(off_t);
	 int GetCount();
	 void Run(void* (*)(void*), void*);
 private:
	 int srv_port;		//server port and
	 in_addr_t srv_addr;	//addres (binary)

	 char* srv_name;	//string values for: name
	 char* srv_ip;		//ip
	 char* srv_descr;	//description

	 int srv_sock;		//connection socket
	 pthread_t srv_thread;	//thread of connection preserver

	 char* file_name;	//stored filename (if upload/download)
	 off_t file_length;	//and its length
	 off_t file_trans;	//how much transferred
	 
	 KList* next;		//pointer to next node, or NULL if last
};

class KApp;			//forward

struct ThreadDataPasser		//used to pass data to thread
{				//passes different types of values
 KList* elem;
 KList* head;
 KApp*  klass;
 int sock;
 int lpdk;
 char* pattern;
};
#endif
