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
#include "XHttpParser.hh"
#include "Common.hh"
#include "Definitions.hh"
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

////
//	construct class, set default values
//	error is initially 1 and
//	__correct__ HTTP request sets it to 0
////
XHttpParser :: XHttpParser()
{
 string = NULL;
 size = 0;
 error = 1;
 prevmsg = NULL;
 buffered_size = 0;
 large_file = NULL;
}

////
//	destroy class, free memory
////
XHttpParser :: ~XHttpParser()
{
 if (string) delete [] string;
 if (prevmsg) delete [] prevmsg;
 string = NULL;
 size = 0;
 if (large_file) fclose(large_file);
 buffered_size = 0;
}

///
// 	returns number of bytes in given file
///
int XHttpParser :: GetBytesCount(const char* fn) const
{
 FILE* fl;
 int fsz;
 assert(fn);
 fl = fopen(fn,"r");
 if (!fl) return 0;
 fseek(fl, 0, SEEK_END);
 fsz = ftell(fl);
 fclose(fl);
 return fsz;
}

////
//	Get parsed string, its length and error value
//	and return it to client handler
//	it will be sent to client application
//	and if error is set connection will be closed
////
char* XHttpParser :: GetResponse(int& length, int& err) const
{
 length = size;
 if (!string || size==0) printf("WARNING: %s:%d, string is NULL or size is 0!\n", HERE);
 err = error;
 return string;
}

////
//	Sends file pointer instead of buffer with text
//	HTTP header is returned as char*
//	and file pointer should be send immadiately follow
////
char* XHttpParser :: GetLargeFile(int& length, int& err, FILE*& fp) const
{
 length = buffered_size;
 if (!string || size==0) printf("WARNING: %s:%d, string is NULL or size is 0!\n", HERE);
 err = error;
 fp = large_file;
 return string;
}

////
//	Provides support for WWW browsing of directories
//	Quite fun
//	if You return to / dir, then index.html will be send
////
int XHttpParser :: GenerateDirlist(const char* http_type, const char* from)
{
 DIR* dp;
 int nent,fsz;
 struct dirent *wpis;
 struct stat statbufor;
 char fullname[PATH_MAXIMUM];
 char pw[PATH_MAXIMUM];
 char start_dir[PATH_MAXIMUM];
 char tmp[STD_HTTP_MSG*2];
 time_t tim;
 assert(http_type);
 assert(from);
 tim = time(NULL);
 if (!getcwd(start_dir, PATH_MAXIMUM)) 	return 0;
 if (!(dp = opendir(from))) 		{ chdir(start_dir); return 0; }
 if (chdir(from)) 			{ chdir(start_dir); return 0; }
 if (!getcwd(pw, PATH_MAXIMUM)) 		{ chdir(start_dir); return 0; }
 nent = 0;
 while ((wpis=readdir(dp))!=NULL) nent++;
 closedir(dp);
 string = new char[STD_HTTP_MSG*3+nent*(PATH_MAXIMUM+128)];
 assert(string);
 if (!(dp = opendir(from))) 		
   {
    chdir(start_dir);
    return 0;
   }
 if (chdir(from))
   {
    chdir(start_dir);
    return 0;
   }
 if (!getcwd(pw, PATH_MAXIMUM))
   {
    chdir(start_dir);
    return 0;
   }
 sprintf(tmp, "%s 200 Listing directory\r\nDate: %s"
		 "Server: Simplest WWW C++ Server\r\n"
		 "Content-Type: text/html\r\n\r\n"
		 "<HTML><BODY><CENTER><BIG>Directory listing of '%s': "
		 "</BIG></CENTER><BR>\r\n"
		 "<A HREF=\"/\"><BIG>Back to WWW root<BIG><BR></A>\r\n"
		 , http_type, ctime(&tim), from);
 strcpy(string, tmp);
 while ((wpis=readdir(dp))!=NULL)
     {
      if (stat(wpis->d_name,&statbufor)==-1) return 0;
      if (!strcmp(pw,"/"))
        {
	 strcpy(fullname,"/");
         strcat(fullname, wpis->d_name);
	}
      else
        {
         strcpy(fullname, pw);
	 if (fullname[strlen(fullname)-1] != '/') strcat(fullname,"/");
	 strcat(fullname, wpis->d_name);
        }
      if (!strcmp(fullname,"/")) strcpy(fullname,"//");
      if (S_ISLNK(statbufor.st_mode)) 
         {
	  sprintf(tmp,"<A HREF=\"%s\"> LNK==> %s</A><BR>\r\n", fullname, wpis->d_name);
	  strcat(string, tmp);
	  continue;
	 }
      if (S_ISDIR(statbufor.st_mode))
	  {
	   sprintf(tmp,"<A HREF=\"%s\"> D==> %s</A><BR>\r\n", fullname, wpis->d_name);
	   strcat(string, tmp);
	  }
      else
         {
           if (!(statbufor.st_mode & S_IFREG)) 
	     {
	      sprintf(tmp,"<A HREF=\"%s\"> F==> %s (not a regular file)</A><BR>\r\n", fullname, wpis->d_name);
	      strcat(string, tmp);
	     }
	   else
	     {
              fsz = GetBytesCount(fullname);
	      sprintf(tmp,"<A HREF=\"%s\"> F--> %s (%dbytes)</A><BR>\r\n", fullname, wpis->d_name, fsz);
	      strcat(string, tmp);
	     }
         }
     }
 strcat(string, "</BODY></HTML>\r\n\r\n");
 size = strlen(string);
 closedir(dp);
 chdir(start_dir);
 return 1;
}

////
//	generate HTTP Message - this generates error messages
//	of http_type, type (see Definitions.hh)
//	and comment
//	something like: HTTP/1.1 404 Server Comment
//	this function sets error to 1
//	so it is used to return HTTP error code
////
void XHttpParser :: GenerateMessage(const char* http_type, const int type, const char* shcomm, const char* comment)
{
 char comm[LONG_STR];
 time_t tim;
 assert(http_type);
 assert(comm);
 assert(shcomm);
 if (string) delete [] string;
 if (prevmsg) delete [] prevmsg;
 string = NULL;
 size = 0;
 if (comment) strcpy(comm, comment);
 else         strcpy(comm,"No comment");
 string = new char[STD_HTTP_MSG*2+strlen(shcomm)+strlen(comm)];
 assert(string);
 tim = time(NULL);
 sprintf(string, "%s %d %s\r\nDate: %s"
		 "Server: Simplest WWW C++ Server\r\n"
		 "Content-Type: text/html\r\n\r\n"
		 "<HTML><BODY><CENTER><BIG>Simplest C++ Server Error: "
		 "%s</BIG></CENTER><BR>\r\n"
		 "%s<BR>\r\n"
		 "<A HREF=\"/index.html\"> Back to WWW root<BR></A>\r\n"
		 "</BODY></HTML>\r\n\r\n"
		 , http_type, type, shcomm, ctime(&tim), comm, 
		 prevmsg?prevmsg:"No previous error");
 size = strlen(string)+1;
 Debug("Answer String is:\n'%s'\n", string);
 prevmsg = new char[strlen(shcomm)+strlen(comment)+128];
 strcpy(prevmsg, "Error from previous try: <BR>\r\n");
 strcat(prevmsg, shcomm);
 strcat(prevmsg, "<BR>\r\n");
 strcat(prevmsg, comment);
 strcat(prevmsg, "<BR>\r\n");
 error = 1;
}

////
//	Get Line from string str and move
//	idx to point next line (if exists)
//	if no more lines in str NULL is returned
//	and then idx value is not touched
////
char* XHttpParser :: GetLine(const char* str, int& idx) const
{
 char* copy;
 assert(idx>=0);
 assert(str);
 int x = idx;
 while (str[x]!=CR && str[x]!=LF && str[x]!=0) x++;
 if (idx==x) return NULL;
 copy = new char[(x-idx)+1];
 assert(copy);
 for (int i=idx;i<x;i++) copy[i-idx] = str[i];
 copy[x-idx] = 0;
 while (str[x]==LF || str[x]==CR) x++;
 idx = x;
 return copy;
}

////
//	gets one word for a line, line should be
//	prepared by method GetLine
//	parser gets words by checking SPACEs, CRLFs and TABs
//	so 'Server:' is a word
//	if no more words then NULL is returned
//	otherwise idx points to next word
////
char* XHttpParser :: GetWord(const char* str, int& idx) const
{
 char* copy;
 assert(str);
 assert(idx>=0);
 int x = idx;
 while (str[x]!=CR && str[x]!=LF && str[x]!=TAB && str[x]!=SPACE && str[x]!=0) x++;
 if (idx==x) return NULL;
 copy = new char[(x-idx)+1];
 assert(copy);
 for (int i=idx;i<x;i++) copy[i-idx] = str[i];
 copy[x-idx] = 0;
 while (str[x]==SPACE || str[x]==TAB) x++;
 idx = x;
 return copy;
}

////
//	transforms all sequences %hh, where h is HEX digit
//	into binary char(0xhh), so for example
//	'line%20and%20space' goes to
//	'line and space'
////
char* XHttpParser :: TransformFileName(const char* par) const
{
 char* ret;
 int len,x,tmp;
 x=0;
 len=0;
 ret = new char[strlen(par)+1];
 assert(ret);
 tmp=0;
 len = strlen(par);
 for (int i=0;i<len;i++)
   {
    if (par[i]=='%') 
      {
       i++;
       tmp=0;
       tmp += (par[i]-'0')*0x10+(par[i+1]-'0');
       i++;
       ret[x] = tmp;
       x++;
      }
    else
      {
       ret[x] = par[i];  
       x++;
      }
   }
 ret[x] = 0;
 return ret;
}

////
//	tries to send fname with HTTP verison httpv
//	if get==1 then answers to GET request
//	if get==0 then answers to HEAD request and not sends the body
//	if any error occur, generates error message and exits
//	uses access, stat and fopen to check file
////
int XHttpParser :: SendFile(const char* fname, const char* httpv, const int get)
{
 int err,ern;
 char msg[SHORT_STR];
 char *fn;
 fn = TransformFileName(fname);
 assert(fn);
 assert(httpv);
 assert(get==0 || get==1);
 Debug("Sending file: %s\n", fn);
 err=access(fn, R_OK);
 ern=errno;
 if (err==-1)
 {
  if (ern==ENOTDIR) 
    {
     Debug("Part of path element is not directory: '%s'\n",fn);
     sprintf(msg, "Some elements of path '%s' are not directories", fn);
     GenerateMessage(httpv, NOT_FOUND, "Not dir", msg);
     if (fn) delete [] fn;
     return 0;
    }
  else if (ern==ENOENT) 
    {
     Debug("File dont exists: '%s'\n",fn);
     sprintf(msg, "File dont exists: '%s'", fn);
     GenerateMessage(httpv, NOT_FOUND, "Not found", msg);
     if (fn) delete [] fn;
     return 0;
    }
  else if (ern==EACCES) 
    {
     Debug("Access Denied to'%s'\n",fn);
     sprintf(msg, "Access Denied to: '%s'", fn);
     GenerateMessage(httpv, FORBIDDEN, "Access Denied", msg);
     if (fn) delete [] fn;
     return 0;
    }
  else if (ern==EIO) 
    {
     Debug("I/O error reading: '%s'\n",fn);
     sprintf(msg, "I/O Error reading: '%s'", fn);
     GenerateMessage(httpv, INTERNAL_ERROR, "I/O Error", msg);
     if (fn) delete [] fn;
     return 0;
    }
  else if (ern==ELOOP) 
    {
     Debug("Too many symbolic links for: '%s'\n",fn);
     sprintf(msg, "Too many symlinks or loop on '%s'", fn);
     GenerateMessage(httpv, INTERNAL_ERROR, "2many symlinks", msg);
     if (fn) delete [] fn;
     return 0;
    }
  else if (ern==ENAMETOOLONG) 
    {
     Debug("Name is too long: '%s'\n",fn);
     sprintf(msg, "name too long: '%s'", fn);
     GenerateMessage(httpv, REQUEST_TOO_LARGE, "Too large", msg);
     if (fn) delete [] fn;
     return 0;
    }
 }
 static struct stat stbuff;
 err=stat(fn, &stbuff);
 if (err==-1)
   {
     Debug("Syscall stat failed for: '%s'\n",fn);
     sprintf(msg, "Syscall stat failed for: '%s'", fn);
     GenerateMessage(httpv, INTERNAL_ERROR,"Syscall failed" , msg);
     if (fn) delete [] fn;
     return 0;
   }
 if (!(stbuff.st_mode & S_IFREG)) 
   {
     if ((stbuff.st_mode & S_IFDIR) && GenerateDirlist(httpv, fn)) 
         {
           if (fn) delete [] fn;
	   return 1;
	 }
     Debug("File is not a normal file: '%s'\n",fn);
     sprintf(msg, "File is not a normal file: '%s'", fn);
     GenerateMessage(httpv, NOT_FOUND, "Bad file", msg);
     if (fn) delete [] fn;
     return 0;
   }
 FILE* fp;
 unsigned int flen;
 int large;
 large = 0;
 fp = NULL;
 fp = fopen(fn, "r");
 if (!fp)
   {
    Debug("File dont exists: '%s'\n",fn);
    sprintf(msg, "File dont exists: '%s'", fn);
    GenerateMessage(httpv, NOT_FOUND, "Dont exists", msg);
    if (fn) delete [] fn;
    return 0;
   }
 fseek(fp, 0, SEEK_END);
 flen = ftell(fp);
 fseek(fp,0,SEEK_SET);
 if (flen>MAX_FILE_LENGTH)
   {
    Debug("File is too big: '%s', %d bytes\n",fn,flen);
    sprintf(msg, "File too big: '%s', %d bytes", fn,flen);
    GenerateMessage(httpv, REQUEST_TOO_LARGE, "Too large", msg);
    fclose(fp);
    if (fn) delete [] fn;
    return 0;
   }
 if (flen>MAX_FILE_BUFFER) large = 1;
 time_t tim;
 char* type;
 tim = time(NULL);
 if (!get)
   {
    fclose(fp);
    if (string) delete [] string;
    string = NULL;
    size = 0;
    string = new char[STD_HTTP_MSG];
    assert(string);
    type = GetFileType(fn);
    if (!type) 
      {
       type = new char[0x10];
       assert(type);
       strcpy(type,"text/plain");
      }
    sprintf(string, "%s %d OK\r\nDate: %sServer: "
		 "Simplest C++ WWW server\r\n"
		 "Content-type: %s\r\n"
		 "Content-length: %d\r\n\r\n"
		 , httpv, REQUEST_OK, ctime(&tim),GetFileType(fn), flen);
    size = strlen(string)+1;
    error = 0;
    delete [] type;
    Debug("File: '%s' send.\n", fn);
    if (fn) delete [] fn;
    return 1;
   }
 char* fbuff;
 fbuff = NULL;
 if (!large)
  {
   fbuff = new char[flen+1];
   assert(fbuff);
   for (unsigned int i=0;i<flen;i++)
      fbuff[i] = (char)fgetc(fp);
  }
 if (string) delete [] string;
 string = NULL;
 size = 0;
 string = new char[STD_HTTP_MSG+1];
 assert(string);
 type = GetFileType(fn);
 if (!type) 
     {
      type = new char[0x10];
      assert(type);
      strcpy(type,"text/plain");
     }
 sprintf(string, "%s %d OK\r\nDate: %sServer: "
		 "Simplest C++ WWW server\r\n"
		 "Content-type: %s\r\n"
		 "Content-length: %d\r\n\r\n"
		 , httpv, REQUEST_OK, ctime(&tim),type,flen);
 int start;
 if (!large)
   {
    start = strlen(string);
    for (unsigned int i=0;i<flen;i++) string[start+i] = fbuff[i];
    size = start + flen + 1;
   }
 else size = strlen(string) + 1;
 if (fbuff) delete [] fbuff;
 delete [] type;
 if (!large)
   {
    fclose(fp);
    Debug("File: '%s' send\n", fn);
   }
 else
   {
    fseek(fp, 0, SEEK_SET);
    large_file = fp;
    buffered_size = flen;
   }
 error = 0;
 if (fn) delete [] fn;
 return 1;
}

////
// answers to question "are we sending large file in buffered mode?"
//
//
////
int XHttpParser :: SendingLargeFile() const
{
 if (buffered_size) return 1;
 else return 0;
}

////
//	parses string from back until finds '.'
//	then gets extension and usig it generates
//	file type: text/html or image/jpeg for example
//	if cannot parse returns text/plain
//	NOTE: if file.HTML constains binary data
//	it is identified as HTML unfortunately
//	I could use system("file %s") but system
//	is not a good idea, besides application file
//	is needed, so program would loose its
//	portability
////
char* XHttpParser :: GetFileType(const char* str) const
{
 int len,i,siz,x;
 char* ret;
 char* type;
 len = strlen(str);
 i = len-1;
 while (str[i]!='.' && i>=0) i--;
 if (i==len-1) return NULL;
 i++;
 siz = len - i;
 ret = new char[siz+1];
 for (x=0;x<siz;x++)
   {
    ret[x] = str[i+x];
    if (ret[x]>='A' && ret[x]<='Z') ret[x] += 0x20;
   }
 ret[siz] = 0;
 type = new char[0x20];
 assert(type);
 if (!strcmp(ret,"html") || !strcmp(ret,"htm")) strcpy(type,"text/html");
 else if (!strcmp(ret,"jpeg") || !strcmp(ret,"jpg")) strcpy(type,"image/jpeg");
 else if (!strcmp(ret,"png"))  strcpy(type,"image/png");
 else if (!strcmp(ret,"gif"))  strcpy(type,"image/gif");
 else if (!strcmp(ret,"xls"))  strcpy(type,"application/msexcel");
 else if (!strcmp(ret,"xlc"))  strcpy(type,"application/msexcel");
 else if (!strcmp(ret,"doc"))  strcpy(type,"application/msword");
 else if (!strcmp(ret,"viv"))  strcpy(type,"video/vivo");
 else if (!strcmp(ret,"mpg"))  strcpy(type,"video/mpeg");
 else if (!strcmp(ret,"mpeg")) strcpy(type,"video/mpeg");
 else if (!strcmp(ret,"qt"))   strcpy(type,"video/quicktime");
 else if (!strcmp(ret,"mov"))  strcpy(type,"video/quicktime");
 else if (!strcmp(ret,"avi"))  strcpy(type,"video/x-msvideo");
 else if (!strcmp(ret,"asf"))  strcpy(type,"video/x-msvideo");
 else if (!strcmp(ret,"xml"))  strcpy(type,"text/xml");
 else if (!strcmp(ret,"pas"))  strcpy(type,"text/x-pascal");
 else if (!strcmp(ret,"java")) strcpy(type,"text/x-java");
 else if (!strcmp(ret,"cpp"))  strcpy(type,"text/c++src");
 else if (!strcmp(ret,"cc"))   strcpy(type,"text/c++src");
 else if (!strcmp(ret,"hpp"))  strcpy(type,"text/c++hdr");
 else if (!strcmp(ret,"hh"))   strcpy(type,"text/c++hdr");
 else if (!strcmp(ret,"h"))    strcpy(type,"text/chdr");
 else if (!strcmp(ret,"c"))    strcpy(type,"text/csrc");
 else if (!strcmp(ret,"rtf"))  strcpy(type,"text/rtf");
 else if (!strcmp(ret,"bmp"))  strcpy(type,"image/x-bmp");
 else if (!strcmp(ret,"ico"))  strcpy(type,"image/x-ico");
 else if (!strcmp(ret,"mp3"))  strcpy(type,"audio/x-mp3");
 else if (!strcmp(ret,"wav"))  strcpy(type,"audio/x-wav");
 else if (!strcmp(ret,"ogg"))  strcpy(type,"audio/x-vorbis");
 else 
   { 
    Debug("Warinig, unknown file type: %s\n", str); 
    strcpy(type,"text/plain"); 
   }
 delete [] ret;
 return type; 
}

////
// 	process GET or HEAD request (GET if get==1, and HEAD if get==0)
// 	for line and index
// 	eventually call SendFile if data in line is OK
////
void XHttpParser :: ProcessGetRequest(const char* line, int idx, const int get)
{
 char* fname;
 char* fname2;
 char* httpv;
 int filesend;
 assert(line);
 assert(idx>=0);
 assert(get==1 || get==0);
 fname=GetWord(line, idx);
 httpv=GetWord(line, idx);
 if (!fname || !httpv) 
   {
    printf("Client request BAD, missigg filename or HTTP version.\n");
    if (fname) delete [] fname;
    if (httpv) delete [] httpv;
    GenerateMessage(HTTP_11, BAD_REQUEST, "HTTPV Unsupported", "Missing HTTPV or Filename");
    return;
   }
 Debug("Client requests for %s with protocol: %s\n", fname, httpv);
 if (strcmp(httpv,HTTP_10) && strcmp(httpv,HTTP_11))
   {
    GenerateMessage(HTTP_11, HTTP_NOT_SUPPORTED, httpv, httpv);
    printf("HTTP protocol not supported: %s\n", httpv);
    delete [] fname;
    delete [] httpv;
    return;
   }
 //NOTE: TO GET REAL / use browser http://server://
 if (!strcmp(fname,"/"))
   {
    Debug("Request for WWW root - sending index.html\n");
    SendFile("./index.html", httpv, get);
   }
 else 
   {
    filesend = SendFile(fname, httpv, get);
    if (!filesend)
      {
       Debug("Failed to send file: '%s'\n", fname);
       fname2 = new char[strlen(fname)+4];
       assert(fname2);
       strcpy(fname2, "./");
       strcat(fname2, fname);
       filesend = SendFile(fname2, httpv, get);
       if (!filesend) Debug("Failed to send file: '%s'\n", fname2);
       delete [] fname2;
      }
   }
 delete [] fname;
 delete [] httpv;
}

////
//	parse a line of client request
//	identifies GET/HEAD requests
//	identifies client source address and
//	client application type
////
void XHttpParser :: ParseLine(const char* line)
{
 Debug("Parse line: '%s'\n", line);
 assert(line);
 char* word;
 int idx;
 word = NULL;
 idx = 0;
 word=GetWord(line, idx);
 if (!word) return ;
 if (!strcmp(word, "GET")) 
   {
    Debug("Processing GET request.\n");
    ProcessGetRequest(line, idx, 1);
    delete [] word;
    return ;
   }
 if (!strcmp(word, "HEAD")) 
   {
    Debug("Processing HEAD request.\n");
    ProcessGetRequest(line, idx, 0);
    delete [] word;
    return ;
   }
 else if (!strcmp(word, "Host:")) 
      {
       delete [] word;
       word=GetWord(line, idx);
       if (!word) return;
       printf("Client connected to: %s\n", word);
       delete [] word;
       return;
      }
    else if (!strcmp(word, "User-Agent:")) 
      {
       delete [] word;
       word=GetWord(line, idx);
       if (!word) return;
       printf("Client application type is: %s\n", word);
       delete [] word;
       return;
      }
 delete [] word;
}

////
//	parse entire request, this mainly calls all other methods
//	of class, on begining generates error message that 
//	this request is not handled because it is not implemented
//	so if parser won't find any parseable data
//	it will be final message for client
//	note that this parser recognizes only GET/HEAD requests
//	and all other are turned back as unimplemented
////
void XHttpParser :: Parse(const char* request, const int length)
{
 char* copy;
 int idx;
 assert(request);
 assert(length>0);
 copy = NULL;
 idx = 0;
 Debug("Request:\n'%s'\n", request);
 GenerateMessage(HTTP_11, NOT_IMPLEMENTED, "Unsupported", "This operation is not supported in miniC++ server");
 while ((copy=GetLine(request, idx))!=NULL)
   {
    ParseLine(copy);
    delete [] copy;
   }
}

