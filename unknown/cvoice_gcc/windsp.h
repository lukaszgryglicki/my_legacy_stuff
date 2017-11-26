#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>

typedef struct
{
  long chn, fmt, hz, nfrag;
  HWAVEIN hWavIn;
  HWAVEOUT hWavOut;
  WAVEHDR buff;
} WDSP;

long checkWaveError(char* where, DWORD code);
long closeWaveOut(HWAVEOUT* hWav, WAVEHDR* buff);
long playWave(HWAVEOUT* hWav, WAVEHDR* buff);
long playBuff(HWAVEOUT* hWav, WAVEHDR* buff, char* str, int len);
long recordBuff(HWAVEIN* hWav, WAVEHDR* buff);
long closeWaveIn(HWAVEIN* hWav, WAVEHDR* buff);
long recordWave(HWAVEIN* hWav, WAVEHDR* buff);
long addWaveBufferOut(long chn, long fmt, long hz, long nf, HWAVEOUT* hWav, WAVEHDR* buff);
long addWaveBufferIn(long chn, long fmt, long hz, long nf, HWAVEIN* hWav, WAVEHDR* buff);
long openWaveOut(long chn, long fmt, long hz, HWAVEOUT* hWav);
long openWaveIn(long chn, long fmt, long hz, HWAVEIN* hWav);
long numOutDevices(void);
long numInDevices(void);
long windsp(long in, long chn, long fmt, long hz);
long testall(void);
int windsp_write(char* str, int len);
int windsp_read(char* str, int& len);
int windsp_list(int in);
int windsp_close(int in);
int windsp_open(int in, int chn, int fmt, int hz, int nfrag);
void windsp_init();

