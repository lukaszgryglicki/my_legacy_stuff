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
#ifndef __HAVE_DEFS_H__
#define __HAVE_DEFS_H__
#define DEFAULT_MAX_CLIENTS      8
#define MAX_PORT            0X7FFF
#define MAX_CLIENTS             64 
#define MAX_KEY_T	0X7FFFFFFF
#define FAILED                   0
#define SUCCESS                  1
#define MAX_PATH_LENGTH       1024
#define DEFAULT_LOG_FILE "Server.LOG"
#define BACKLOG_LENGTH           4
#define MSGSIZ                4096
#define SHEDULER_INTERVAL    75000
#define LOGGER_INTERVAL        180
#define CR			13
#define LF			10
#define REQUEST_OK		200
#define NO_CONTENT		204
#define BAD_REQUEST		400
#define FORBIDDEN		403
#define NOT_FOUND		404
#define REQUEST_TOO_LARGE	414
#define INTERNAL_ERROR		500
#define NOT_IMPLEMENTED		501
#define HTTP_NOT_SUPPORTED	505
#define HTTP_11		 "HTTP/1.1"
#define HTTP_10		 "HTTP/1.0" 
#define STD_HTTP_MSG	       4096
#define SPACE			' '
#define TAB		       '\t'
#define HERE      __FILE__,__LINE__
#define SHORT_STR		256
#define LONG_STR	       1024
#define MAX_FILE_BUFFER   0x1000000
#define MAX_FILE_LENGTH   0x80000000
#define PATH_MAXIMUM	       1024
#define BUFFER_SIZE	      32768
#endif
