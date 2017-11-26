#ifndef __HAVE_KFILE_H__
#define __HAVE_KFILE_H__
#include "Common.hh"
#include "KList.hh"
class KFile	//Handles I/O system
{
 public:
	 KFile(char*);
	 ~KFile();
	 int OpenR();
	 int OpenW(int=1);
	 char* GetLine() const;
	 void PutHostLine(KList*) const;
	 off_t GetLength() const;
 private:
	 char* file_name;
	 int file_fd;		//file descriptor
};
#endif
