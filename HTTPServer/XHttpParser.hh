/////////////////////////////////////////////////
//////// ********************************////////
///// **************************************/////
//// ****  HTTP  ****************************////
/// ***********  Server  *********************///
// ***********************  in C++  ***********//
// ********************************************//
/// **### StudentSoft copyleft@ ###***********///
//// *************   BSD License   **********////
///// **************************************/////
//////// ********************************////////
/////////////////////////////////////////////////
#ifndef __HAVE_CLASS_HTTPPARSER__
#define __HAVE_CLASS_HTTPPARSER__

#include "Common.hh"

class XHttpParser
{
 public:
 	XHttpParser();
 	~XHttpParser();
	char* GetResponse(int&, int&) const;
	char* GetLargeFile(int&, int&, FILE*&) const;
	void Parse(const char*, const int);
	int SendingLargeFile() const;
 private:
	char* GetLine(const char*, int& idx) const;
	char* GetWord(const char*, int& idx) const;
        void GenerateMessage(const char*, const int, const char*, const char*);
        int GenerateDirlist(const char*, const char*);
	void ParseLine(const char*);
	void ProcessGetRequest(const char*,int,const int);
	int SendFile(const char*, const char*, const int);
	char* GetFileType(const char*) const;
	char* TransformFileName(const char*) const;
	int GetBytesCount(const char*) const;
 private:
	char* string;
	char* prevmsg;
	int size;
	int error;
	unsigned int buffered_size;
	FILE* large_file;
};
#endif
