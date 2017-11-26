#ifndef EXCEPTIONS_
#define EXCEPTIONS_
#define MSGBUF 1024 
class Exception		//klasa do wyrzucania wyjatkow
{
 public:
	 Exception();
	 Exception(char* m) { ConstructMsg(m); }
	 void ConstructMsg(char*);
	 ~Exception();
	 void Print() const;	//wypisuje tekst wyjatku
 protected:
	 char* msg;	//tekst wyjatku
};
#endif
