#ifndef __HAVE_KLOG_H__
#define __HAVE_KLOG_H__
#include "Common.hh"
//used to add messages to system log
//there are many levels
//Debug only if _DDEBUG set etc
//Error appears always (writes to stdout too)
//Warning when no console mode
//Printf just writes to log
class KSysLog
{
 public:
	 KSysLog() { nmessages = daemon = 0; }	//inline constructor;
	 void AddMessage(char*, ...);
	 void SetDaemonMode(bool);
	 void SetConsoleMode(bool);
	 void Printf(char*, ...);
	 void Debug(char*, ...);
	 void Warning(char*, ...);
	 void Error(char*, ...);
	 int GetNMessages() const;
 private:
	 int nmessages;		//number of messages
	 bool daemon;		//used to determine writing to stdout
	 bool console;		//as above
};
#endif
