#include "windsp.h"


static WDSP* wdsp = NULL;

long checkWaveError(const char* where, DWORD code)
{
  char buff[256];
  /*fprintf(stderr, "checkErr(%s): %ld/%d\n", where, code, MMSYSERR_NOERROR);*/
  if (code == MMSYSERR_NOERROR) return 0;
  waveInGetErrorText(code, buff, sizeof(buff));
  fprintf(stderr, "%s: %s\n", where, buff);
  return 1;
}

long closeWaveOut(HWAVEOUT* hWav, WAVEHDR* buff)
{
  MMRESULT err;

  err = waveOutUnprepareHeader(*hWav, buff, sizeof(WAVEHDR)); 
  if (checkWaveError( "unpreparehdrout", err )) return 0;

  free((void*)buff->lpData);

  err = waveOutClose( *hWav );
  if (checkWaveError( "waveoutclose", err )) return 0;

  /*fprintf(stderr, "Recorded.\n");*/
  return 1;
}

long playWave(HWAVEOUT* hWav, WAVEHDR* buff)
{
  MMRESULT err;
  long i, n;

  n = fread((void*)buff->lpData, buff->dwBufferLength, 1, stdin);
  if (n != 1) 
  {
    fprintf(stderr, "Error reading buffer from stdin: %ld\n", n);
    return 0;
  }

  err = waveOutWrite( *hWav, buff, sizeof(WAVEHDR) );
  if (checkWaveError( "play", err )) return 0;

  i = 0;
  while (( buff->dwFlags & WHDR_DONE) != WHDR_DONE) 
  {
     /*fprintf(stderr, "%ld\n", ++i);*/
     Sleep( 50 );
  }

  err = waveOutUnprepareHeader(*hWav, buff, sizeof(WAVEHDR)); 
  if (checkWaveError( "unpreparehdrout", err )) return 0;

  free((void*)buff->lpData);

  err = waveOutClose( *hWav );
  if (checkWaveError( "waveoutclose", err )) return 0;

  /*fprintf(stderr, "Recorded.\n");*/
  return 1;

}

long playBuff(HWAVEOUT* hWav, WAVEHDR* buff, char* str, int len)
{
  MMRESULT err;

  memcpy((void*)buff->lpData, (void*)str, (size_t)buff->dwBufferLength);

  /*
  while (( buff->dwFlags & WHDR_DONE) != WHDR_DONE) 
  {
     Sleep( 20 );
  }
  */

  err = waveOutWrite( *hWav, buff, sizeof(WAVEHDR) );
  if (checkWaveError( "playbuff", err )) return 0;

  /*
  while (( buff->dwFlags & WHDR_DONE) != WHDR_DONE) 
  {
     Sleep( 50 );
  }
  */

  return 1;
}

long recordBuff(HWAVEIN* hWav, WAVEHDR* buff)
{
  MMRESULT err;
  long i;

  err = waveInStart( *hWav );
  if (checkWaveError( "recordbuff", err )) return 0;

  i = 0;
  while (( buff->dwFlags & WHDR_DONE) != WHDR_DONE) 
  {
     /*fprintf(stderr, "%ld\n", ++i);*/
     Sleep( 20 );
  }

  return 1;
}

long closeWaveIn(HWAVEIN* hWav, WAVEHDR* buff)
{
  MMRESULT err;

  err = waveInStop( *hWav );
  if (checkWaveError( "waveinstop", err )) return 0;

  err = waveInUnprepareHeader(*hWav, buff, sizeof(WAVEHDR)); 
  if (checkWaveError( "unpreparehdrin", err )) return 0;

  free((void*)buff->lpData);

  err = waveInClose( *hWav );
  if (checkWaveError( "waveinclose", err )) return 0;

  /*fprintf(stderr, "Recorded.\n");*/
  return 1;
}

long recordWave(HWAVEIN* hWav, WAVEHDR* buff)
{
  MMRESULT err;
  long i, n;

  err = waveInStart( *hWav );
  if (checkWaveError( "record", err )) return 0;

  i = 0;
  while (( buff->dwFlags & WHDR_DONE) != WHDR_DONE) 
  {
     /*fprintf(stderr, "%ld\n", ++i);*/
     Sleep( 50 );
  }

  err = waveInStop( *hWav );
  if (checkWaveError( "waveinstop", err )) return 0;

  /*fprintf(stderr, "%ld/%ld\n", buff->dwBytesRecorded, buff->dwBufferLength);*/
  /*fwrite((void*)buff->lpData, buff->dwBytesRecorded, 1, stdout);*/
  n = fwrite((void*)buff->lpData, buff->dwBufferLength, 1, stdout);
  if (n != 1) 
  {
    fprintf(stderr, "Error writing buffer to stdout: %ld\n", n);
    return 0;
  }

  err = waveInUnprepareHeader(*hWav, buff, sizeof(WAVEHDR)); 
  if (checkWaveError( "unpreparehdrin", err )) return 0;

  free((void*)buff->lpData);

  err = waveInClose( *hWav );
  if (checkWaveError( "waveinclose", err )) return 0;

  /*fprintf(stderr, "Recorded.\n");*/
  return 1;

}

long addWaveBufferOut(long chn, long fmt, long hz, long nf, HWAVEOUT* hWav, WAVEHDR* buff)
{
  long bs;
  MMRESULT err;

  bs = (chn * fmt * hz * nf) / 400;
  //printf("bufsize = %ld\n", bs);
  ZeroMemory(buff, sizeof(WAVEHDR));

  buff->lpData = (LPSTR)malloc(bs*sizeof(BYTE));
  buff->dwBufferLength = bs;
  buff->dwFlags = 0;

  ZeroMemory(buff->lpData, bs);
  err = waveOutPrepareHeader(*hWav, buff, sizeof(WAVEHDR));
  if (checkWaveError( "preparehdrout", err )) return 0;

  /*fprintf(stderr, "Buffered.\n");*/
  return 1;
}


long addWaveBufferIn(long chn, long fmt, long hz, long nf, HWAVEIN* hWav, WAVEHDR* buff)
{
  long bs;
  MMRESULT err;

  bs = (chn * fmt * hz * nf) / 400;
  ZeroMemory(buff, sizeof(WAVEHDR));

  buff->lpData = (LPSTR)malloc(bs*sizeof(BYTE));
  buff->dwBufferLength = bs;
  buff->dwFlags = 0;

  ZeroMemory(buff->lpData, bs);
  err = waveInPrepareHeader(*hWav, buff, sizeof(WAVEHDR));
  if (checkWaveError( "preparehdrin", err )) return 0;

  err = waveInAddBuffer(*hWav, buff, sizeof(WAVEHDR));
  if (checkWaveError( "addbuffin", err )) return 0;

  /*fprintf(stderr, "Buffered.\n");*/
  return 1;
}

long openWaveOut(long chn, long fmt, long hz, HWAVEOUT* hWav)
{
  MMRESULT err;
  WAVEFORMATEX wavFmt;

  wavFmt.wFormatTag = WAVE_FORMAT_PCM;
  wavFmt.nChannels  = chn;
  wavFmt.nSamplesPerSec = hz;
  wavFmt.wBitsPerSample = fmt;
  wavFmt.nBlockAlign = wavFmt.nChannels * (wavFmt.wBitsPerSample/8);
  wavFmt.nAvgBytesPerSec = wavFmt.nSamplesPerSec * wavFmt.nBlockAlign;
  wavFmt.cbSize = 0;

  err = waveOutOpen(hWav, WAVE_MAPPER, &wavFmt, 0, 0, CALLBACK_NULL|WAVE_FORMAT_DIRECT|WAVE_ALLOWSYNC/*|WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE*/);
  if (checkWaveError( "openout", err )) return 0;

  /*fprintf(stderr, "Opened.\n");*/
  return 1;
}

long openWaveIn(long chn, long fmt, long hz, HWAVEIN* hWav)
{
  MMRESULT err;
  WAVEFORMATEX wavFmt;

  wavFmt.wFormatTag = WAVE_FORMAT_PCM;
  wavFmt.nChannels  = chn;
  wavFmt.nSamplesPerSec = hz;
  wavFmt.wBitsPerSample = fmt;
  wavFmt.nBlockAlign = wavFmt.nChannels * (wavFmt.wBitsPerSample/8);
  wavFmt.nAvgBytesPerSec = wavFmt.nSamplesPerSec * wavFmt.nBlockAlign;
  wavFmt.cbSize = 0;

  err = waveInOpen(hWav, WAVE_MAPPER, &wavFmt, 0, 0, CALLBACK_NULL|WAVE_FORMAT_DIRECT/*|WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE*/);
  if (checkWaveError( "openin", err )) return 0;

  /*fprintf(stderr, "Opened.\n");*/
  return 1;
}

long numOutDevices(void)
{
  UINT c, i;
  long n;
  WAVEOUTCAPS caps;
  MMRESULT err;
  
  c = waveOutGetNumDevs();
  n = 0;

  for (i=0;i<c;i++)
  {
    err = waveOutGetDevCaps(i, &caps, sizeof(caps));
    if (!checkWaveError( "capsout", err ))
    {
      n ++;
      fprintf(stderr, "%ld: %s\n", n, caps.szPname);
    }
  }
  
  return n;
}

long numInDevices(void)
{
  UINT c, i;
  long n;
  WAVEINCAPS caps;
  MMRESULT err;
  
  c = waveInGetNumDevs();
  n = 0;

  for (i=0;i<c;i++)
  {
    err = waveInGetDevCaps(i, &caps, sizeof(caps));
    if (!checkWaveError( "capsin", err ))
    {
      n ++;
      fprintf(stderr, "%ld: %s\n", n, caps.szPname);
    }
  }
  
  return n;
}

long windsp(long in, long chn, long fmt, long hz)
{
  HWAVEIN hWavIn;
  HWAVEOUT hWavOut;
  WAVEHDR buff;

  // nfrags: 1frag = 20ms 50 frags = 1s

/*  fprintf(stderr, "in=%ld chn=%ld fmt=%ld hz=%ld\n", in, chn, fmt, hz);*/

  if (in)
  {
    if (!openWaveIn(chn, fmt, hz, &hWavIn)) return 1;
    if (!addWaveBufferIn(chn, fmt, hz, 16, &hWavIn, &buff)) return 2;
    if (!recordWave(&hWavIn, &buff)) return 3;
  }
  else
  {
    if (!openWaveOut(chn, fmt, hz, &hWavOut)) return 1;
    if (!addWaveBufferOut(chn, fmt, hz, 16, &hWavOut, &buff)) return 2;
    if (!playWave(&hWavOut, &buff)) return 3;
  }
  return 0;
}

long testall(void)
{
  long chn, fmt, hz;
  HWAVEIN hWavIn;
  HWAVEOUT hWavOut;
  WAVEHDR buff;
  
  chn = 1;
  fmt = 8;
  hz = 8000;

  numOutDevices();
  if (!openWaveOut(chn, fmt, hz, &hWavOut)) return 1;
  if (!addWaveBufferOut(chn, fmt, hz, 16, &hWavOut, &buff)) return 2;
  if (!playWave(&hWavOut, &buff)) return 3;

  numInDevices();
  if (!openWaveIn(chn, fmt, hz, &hWavIn)) return 1;
  if (!addWaveBufferIn(chn, fmt, hz, 16, &hWavIn, &buff)) return 2;
  if (!recordWave(&hWavIn, &buff)) return 3;

  return 0;
}

int windsp_write(char* str, int len)
{
    if (!playBuff(&wdsp[1].hWavOut, &wdsp[1].buff, str, len))
    {
       return 1;
    }

    return 0;
}

int windsp_read(char* str, int& len)
{
    if (!recordBuff(&wdsp[0].hWavIn, &wdsp[0].buff))
    {
       len = 0;
       return 1;
    }

    len = wdsp[0].buff.dwBufferLength;
    strncpy(str, wdsp[0].buff.lpData, len);

    return 0;
}

int windsp_list(int in)
{
    if (in)
    {
      return numInDevices();
    }
    else
    {
      return numOutDevices();
    }
}

int windsp_close(int in)
{
    if (in)
    {
       if (!closeWaveIn(&wdsp[0].hWavIn, &wdsp[0].buff)) 
       {
	   return 1;
       }
    }
    else
    {
       if (!closeWaveOut(&wdsp[1].hWavOut, &wdsp[1].buff)) 
       {
	   return 2;
       }
    }

    return 0;
}

int windsp_open(int in, int chn, int fmt, int hz, int nfrag)
{
    if (in)
    {
       wdsp[0].chn = chn;
       wdsp[0].fmt = fmt;
       wdsp[0].hz = hz;
       wdsp[0].nfrag = nfrag;

       if (!openWaveIn(wdsp[0].chn, wdsp[0].fmt, wdsp[0].hz, &wdsp[0].hWavIn)) 
       {
	   return 1;
       }

       if (!addWaveBufferIn(wdsp[0].chn, wdsp[0].fmt, wdsp[0].hz, wdsp[0].nfrag, &wdsp[0].hWavIn, &wdsp[0].buff))
       {
	   return 2;
       }
    }
    else
    {
       wdsp[1].chn = chn;
       wdsp[1].fmt = fmt;
       wdsp[1].hz = hz;
       wdsp[1].nfrag = nfrag;

       if (!openWaveOut(wdsp[1].chn, wdsp[1].fmt, wdsp[1].hz, &wdsp[1].hWavOut)) 
       {
	   return 3;
       }

       if (!addWaveBufferOut(wdsp[1].chn, wdsp[1].fmt, wdsp[1].hz, wdsp[1].nfrag, &wdsp[1].hWavOut, &wdsp[1].buff))
       {
	   return 4;
       }
    }

    return 0;
}


void windsp_init()
{
    wdsp = (WDSP*)malloc(2 * sizeof(WDSP));
}


