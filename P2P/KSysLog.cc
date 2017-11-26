#include "Common.hh"
#include "KSysLog.hh"

//adds message to syslog
//but do not print it to stdout
void KSysLog :: AddMessage(char* fmt , ...)
{
 va_list ap;
 if (!fmt) return;
 va_start(ap,fmt);
 vsyslog(LOG_NOTICE, fmt, ap);
 va_end(ap);
 nmessages++;
}

//returns number of written messages in current session
int KSysLog :: GetNMessages() const
{
 return nmessages;
}

//sets daemon parameter to demon
//used to determine what write to log and/or to stdout
void KSysLog :: SetDaemonMode(bool demon)
{
 daemon = demon;
}

//sets console parameter to cons
//used to determine what write to log and/or to stdout
void KSysLog :: SetConsoleMode(bool cons)
{
 console = cons;
}

//if demon prints to log
//else to log and stdout
void KSysLog :: Printf(char* fmt, ...)
{
 va_list ap;
 if (!fmt) return;
 va_start(ap,fmt);
 if (daemon) vsyslog(LOG_NOTICE, fmt, ap);
 else 
   {
    vsyslog(LOG_NOTICE, fmt, ap);
    vprintf(fmt, ap);
    printf("\n");
   }
 va_end(ap);
 if (daemon) nmessages++;
}

//if DEBUG is not defined doeas nothing
//else if daemon then prints to syslog
//else printf to stdout
void KSysLog :: Debug(char* fmt, ...)
{
#ifdef DEBUG
 va_list ap;
 if (!fmt) return;
 va_start(ap,fmt);
 if (daemon) vsyslog(LOG_NOTICE, fmt, ap);
 else 
   {
    printf("DEBUG: ");
    vprintf(fmt, ap);
    printf("\n");
   }
 va_end(ap);
 if (daemon) nmessages++;
#endif
}

//if daemon or console prints to log
//else to log and stdout
void KSysLog :: Warning(char* fmt, ...)
{
 va_list ap;
 if (!fmt) return;
 va_start(ap,fmt);
 if (daemon || console) 
   {
    syslog(LOG_WARNING, "WARNING follows");
    vsyslog(LOG_WARNING, fmt, ap);
   }
 else 
   {
    printf("WARNING:\t");
    vprintf(fmt, ap);
    printf("\n");
    vsyslog(LOG_WARNING, fmt, ap);
   }
 va_end(ap);
 if (daemon) nmessages++;
}

//if daemon prints to log
//else to log and stdout
void KSysLog :: Error(char* fmt, ...)
{
 va_list ap;
 if (!fmt) return;
 va_start(ap,fmt);
 if (daemon) 
   {
    syslog(LOG_ERR, "ERROR follows");
    vsyslog(LOG_ERR, fmt, ap);
   }
 else 
   {
    printf("\n\nERROR:\t");
    vprintf(fmt, ap);
    printf("\n\n");
    vsyslog(LOG_ERR, fmt, ap);
   }
 va_end(ap);
 if (daemon) nmessages++;
}

