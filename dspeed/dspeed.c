#include <mh.h>
#include <sys/timeb.h>

#define BUFF	1024*1024*64
#define asm_wsp 12.0		//__int?_t ?=8,16,32,64
#define Int	unsigned long long

Int convert_time(timeb tt)
{
 return Int(tt.time)*Int(1000.0)+Int(tt.millitm);
}

int main()
{
 timeb timer;
 Int stime, etime, tt;
 double tmp;
 double t1,t2,t3;
 FILE* plik;
 Int big_num = 0xff;
 for (int i=0;i<8;i++) big_num *=0xff;
 cout<<"Int64 value should be displayed correctly: "<<big_num<<endl;
 cout<<"creating buffer. . .\n";
 char* buffer;
 buffer = new char[BUFF];
 ftime(&timer);
 stime = convert_time(timer);
 for (int i=0;i<BUFF;i++) buffer[i] = (char)(i%0x100);
 ftime(&timer);
 etime = convert_time(timer);
 tt = etime-stime;
 cout<<"Buffer of "<<BUFF<<" bytes created in "<<tt<<" miliseconds.\n";
 tmp = ((double)(BUFF/1000)/(double)tt)*asm_wsp;
 printf("~%10.5f MHz processor appears for this application\n", tmp);
 t1=tmp;


 ftime(&timer);
 stime = convert_time(timer);
 cout<<"Will  do 100,000,000 operations now:\n";
 for (int i=0;i<100000000;i++)	  ;
 ftime(&timer);
 etime = convert_time(timer);
 tt = etime-stime;
 cout<<"100,000,000 dead loops in "<<tt<<" miliseconds.\n";
 tmp = (double)500000/(double)tt; 
 printf("~%10.5f MHz processor appears for this application\n", tmp);
 t2=tmp;
 t3=(t1+t2)/2;


 ftime(&timer);
 cout<<"Will open file now\n";
 plik = fopen("buffer.out","w");
 if (!plik) {cout<<"File cannot be created!\n"; return -1;}
// fwrite(buffer, BUFF, 1, plik);
 stime = convert_time(timer);
 for (int i=0;i<BUFF;i++) fputc(buffer[i],plik);
 ftime(&timer);
 etime = convert_time(timer);
 tt = etime-stime;
 cout<<"Writing "<<BUFF<<" bytes to file: "<<tt<<" miliseconds.\n";
 tmp = double(BUFF/1000)/(double)tt;
 cout<<"Disk speed: "<<tmp<<" MB per second in write mode.\n";
 fclose(plik);
 t1=tmp;


 ftime(&timer);
 stime = convert_time(timer);
 cout<<"Will open file now\n";
 if (!plik) {cout<<"File cannot be read!\n"; return -1;}
 //fread(buffer, BUFF, 1, plik);
 plik = fopen("buffer.out","r");
 for (int i=0;i<BUFF;i++) buffer[i] = fgetc(plik);
 ftime(&timer);
 etime = convert_time(timer);
 tt = etime-stime;
 cout<<"Reading "<<BUFF<<" bytes from file: "<<tt<<" miliseconds.\n";
 tmp = double(BUFF/1000)/(double)tt;
 cout<<"Disk speed: "<<tmp<<" MB per second in read mode.\n";
 fclose(plik);
 t2=tmp;
 tmp=(t1+t2)/2;

 printf("\n\nAverage CPU speed for App: %-5.0f MHz\n\n",t3);
 printf("Average disk R/W speed for App: %-4.2f MB/s\n\n",tmp);

 unlink("buffer.out");
 cout<<"freeing buffer. . .\n";
 delete [] buffer;
 return 0;
}

