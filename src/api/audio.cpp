#include <iostream>
#include "audio.h"
#include "libzplay.h"

BEGIN_C_DECLS


ZPLAY_HANDLE Global_player;

using namespace std;
using namespace libZPlay;


int print(void) {
    cout << "HelloWorld" <<endl;

    return 0;
}

void Zplay_Create(void) {
	Global_player = zplay_CreateZPlay();
}

int Zplay_Destroy(void) {
	return zplay_DestroyZPlay(Global_player);
}

int Zplay_OpenFile(char* filename) {
	return zplay_OpenFile(Global_player, filename, sfAutodetect);
}

int Zplay_CloseFile(void) {
	return zplay_Close(Global_player);
}

void Zplay_GetError(void) {
	printf("zplay: %s\n",zplay_GetError(Global_player));
}

/* 播放控制 */

int Zplay_Play(void) {
	return zplay_Play(Global_player);
}

int Zplay_Pause(void) {
	return zplay_Pause(Global_player);
}

// 1:播放 0:停止
int Zplay_GetPlayStatus(void) {
	TStreamStatus status;
	zplay_GetStatus(Global_player, &status);
	return status.fPlay;
}

//返回毫秒数
int Zplay_GetPosition(void) {
	TStreamTime pos;
	int times=0;
	zplay_GetPosition(Global_player, &pos);
	times += pos.hms.minute*60000;
	times += pos.hms.second*1000;
	times += pos.hms.millisecond;

	return times;
}

//返回毫秒数
int Zplay_GetTimeLength(void) {
	TStreamInfo pInfo;
	int times=0;
	zplay_GetStreamInfo(Global_player, &pInfo);
	times += pInfo.Length.hms.minute*60000;
	times += pInfo.Length.hms.second*1000;
	times += pInfo.Length.hms.millisecond;

	return times;
}

//偏移秒
int Zplay_Seek(int seek_time) {
	TStreamTime pTime;
	pTime.sec = seek_time/1000;
	return zplay_Seek(Global_player, tfSecond, &pTime, smFromCurrentForward);
} 

/* 音效设置 */

/* EQ均衡器带宽增益控制 */

int Zplay_SetEquailzerPoints(void) {
	int FreqPoint[8] = {32, 63, 125, 250, 500, 1000, 2000, 4000};
	return zplay_SetEqualizerPoints(Global_player, FreqPoint, 8);
}

void Zplay_GetEquailzerPoints(void) {
	int nNumOfPoints =  zplay_GetEqualizerPoints(Global_player, NULL, 0);
	int *pPoints = new int[nNumOfPoints]; // allocate memory
	int i;
	int number_of_points = zplay_GetEqualizerPoints(Global_player, pPoints, nNumOfPoints);
	for(i = 0; i < number_of_points; i++)
	{
		printf("Band %i: %i\n", i, pPoints[i]);
	}
	delete [] pPoints;
}

int Zplay_SetEqualizerParam(int* nBandGain, int nNumOfBands) {
	int nPreamp;
	return zplay_SetEqualizerParam(Global_player, 0 , nBandGain, nNumOfBands);
}

int Zplay_GetEqualizerParam(int* nBandGain, int nNumOfBands) {
	int nPreamp;
	return zplay_GetEqualizerParam(Global_player, &nPreamp, nBandGain, nNumOfBands);
}

int Zplay_Enable(int nEnable) {
	return zplay_EnableEqualizer(Global_player, nEnable);
}

/* 音效调整 */

int Zplay_SetPitch(unsigned int nPitch) {
	return zplay_SetPitch(Global_player, nPitch);
}

unsigned int Zplay_GetPitch(void) {
	return zplay_GetPitch(Global_player);
}

int Zplay_SetRate(unsigned int nRata) {
	return zplay_SetRate(Global_player, nRata);
}

unsigned int Zplay_GetRate(void) {
	return zplay_GetRate(Global_player);
}

int Zplay_SetTempo(unsigned int nTempo) {
	return zplay_SetTempo(Global_player, nTempo);
}

unsigned int Zplay_GetTempo(void) {
	return zplay_GetTempo(Global_player);
}

int Zplay_SetPlayerVolume(unsigned int Vol) {
	return zplay_SetPlayerVolume(Global_player, Vol, Vol);
}

unsigned int Zplay_GetPlayerVolume(void) {
	unsigned int l_temp,r_temp;
	zplay_GetPlayerVolume(Global_player, &l_temp, &r_temp);
	
	return l_temp;
}


END_C_DECLS