
#include "Common.hh"
#include "KFile.hh"
#include "KMemoryException.hh"
#include "KIOException.hh"

//creates class of file called 'fname'
KFile :: KFile(char* fname)
{
 assert(fname);
 file_name = NULL;
 file_fd = -1;
 try
   {
    file_name = new char[strlen(fname)+1];
    if (!file_name) throw new KMemoryException("cannot allocate file_name");
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
   }
 strcpy(file_name, fname);
}

//destroy
KFile :: ~KFile()
{
 if (file_name) delete file_name;
 file_name = NULL;
 if (file_fd) close(file_fd);
 file_fd = -1;
}

//returns file length 
//if filew not open returns -1
off_t KFile :: GetLength() const
{
 if (file_fd<0) 
 {
  printf("WAARNING: KFile::GetLength() file not open %s %d\n", HERE);
  return (off_t)(-1);
 }
 off_t cpos = lseek(file_fd, 0, SEEK_CUR);
 assert(cpos != (off_t)(-1));
 off_t epos = lseek(file_fd, 0, SEEK_END);
 assert(epos != (off_t)(-1));
 assert(lseek(file_fd, cpos, SEEK_SET)!=(off_t)(-1));
 return epos;
}

//opens file read only or throws KIOException
int KFile :: OpenR()
{
 assert(file_name);
 try
   {
    file_fd = open(file_name, O_RDONLY);
    if (file_fd == -1) throw new KIOException("cannot open file in read mode");
   }
 catch (KIOException* kioe)
   {
    kioe->PrintType();
    delete kioe;
    return -1;
   }
 return file_fd;
}

//opens RW if overwrite=1 then rewrites
//else if file exists returns failure
//can throw KIOException 
int KFile :: OpenW(int overwrite)
{
 assert(file_name);
 assert(overwrite == 0 || overwrite == 1);
 try
   {
    if (!overwrite) file_fd = open(file_name, O_WRONLY|O_CREAT|O_EXCL, 0600);
    else            file_fd = open(file_name, O_WRONLY|O_CREAT|O_TRUNC, 0600);
    if (file_fd == -1) throw new KIOException("cannot open file in write mode");
   }
 catch (KIOException* kioe)
   {
    kioe->PrintType();
    delete kioe;
    return -1;
   }
 return file_fd;
}

//gets line from file
char* KFile :: GetLine() const
{
 char* buff = NULL;
 off_t currpos = off_t(0);
 int nread = 0;
 try
   {
    currpos = lseek(file_fd, 0, SEEK_CUR);
    if (currpos == (off_t)(-1)) throw new KIOException("cannot get current position in file");
    buff = new char[MAX_LINE_LENGTH+1];
    if (!buff) throw new KMemoryException("Cannot allocate buffer for reading line");
    retry_read:
    if ((nread=read(file_fd, buff, MAX_LINE_LENGTH)) == -1)
    if (nread == -1)
      {
       if (errno == EINTR) goto retry_read;
       else throw new KIOException("read to buffer failed");
      }
    if (nread == 0) return NULL;
    for (int i=0;i<nread;i++) if (buff[i] == '\n')
     {
      buff[i] = 0;
      if (lseek(file_fd, currpos+i+1, SEEK_SET)==(off_t)(-1))
	throw new KIOException("Cannot lseek position after getline");
      return buff;
     }
    delete [] buff;
    buff = NULL;
    return NULL;	//Line too long
   }
 catch (KMemoryException* kmex)
   {
    kmex->PrintType();
    delete kmex;
    abort();
    return NULL;
   }
 catch (KIOException* kiox)
   {
    kiox->PrintType();
    delete kiox;
    if (buff) delete buff;
    assert(lseek(file_fd, currpos, SEEK_SET)!=(off_t)(-1));
    return NULL;
   }
}

//puts a line describing server to a file
//if srv_name is NULL then srv_ip is used insted
//format is name:port
void KFile :: PutHostLine(KList* srv) const
{
 char line[MAX_LINE_LENGTH];
 assert(srv);
 char* srv_name = srv->GetName();
 char* srv_ip   = srv->GetStringIP();
 int   srv_port = srv->GetPort();
 int len, nwrt;
 sprintf(line, "%s:%d\n", (srv_name?srv_name:srv_ip), srv_port);
 len = strlen(line);
 nwrt = write(file_fd, (void*)line, len);
 if (nwrt != len)
    printf("WARNING: only wrote %d/%d in PutHostLine %s %d\n", nwrt,len,HERE);
}

