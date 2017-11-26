
#ifndef __M_T_EXC__
#define __M_T_EXC__
#define MSGBUF 1024 
class Exception		//used to throw exceptions and as a base for
{			//other exceptions types
 public:
	 Exception();
	 Exception(char* m) { ConstructMsg(m); }
	 void ConstructMsg(char*);
	 ~Exception();
	 void Print() const;	//prints exception msg
 protected:
	 char* msg;		//private message
};
#endif
