/**
 * ================================================================
 * ZPLAY的C封装函数
 * 使用api.h提供的音乐链表操作韩素
 */
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <io.h>
#include "audio.h"
#include "libzplay.h"
#include "api.h"

BEGIN_C_DECLS

ZPLAY_HANDLE Global_player;

using namespace std;
using namespace libZPlay;

/**
 * @description: 从文件目录中加载歌单并保存歌曲信息到歌单链表中
 * @param {char* dir_path, music_info_t* pHead}
 * @return: 已检索到的歌单曲目数量
 */
music_info_t* Audiofile_load(char* dir_path) {
  char dir[200];
  char fullpath[100];
  int i = 0;
  intptr_t handle = NULL;
  struct _finddata_t fileinfo;
  music_info_t* p = NULL;
  /* 初始化首节点 */
  music_info_t* pHead = NULL;
  pHead = (music_info_t*)malloc(sizeof(music_info_t));
  pHead->next = NULL;

  strcpy(dir, dir_path);
  strcat(dir, "*.mp3");
  //创建zplayer实例
  Zplay_Create();
  //开始读取文件
  handle = _findfirst(dir, &fileinfo);
  if (handle == -1) return NULL;
  do {
    sprintf(fullpath, "%s%s", dir_path, fileinfo.name);
    TID3InfoEx id3_info;
    if (zplay_LoadFileID3Ex(Global_player, fullpath, sfAutodetect, &id3_info, 1))  // loading ID3v2
    {
      if (i == 0) {
        p = pHead;
      } else {
        p = musiclist_insert(pHead);
      }
      if (p == NULL) {
        continue;
      } else {
        p->index = i;
        sprintf(p->song_name, "%s", id3_info.Title);
        sprintf(p->Artist_name, "%s", id3_info.Artist);
        sprintf(p->Album_name, "%s", id3_info.Album);
        sprintf(p->song_path, "%s", fullpath);
        sprintf(p->lyric_path, "%s%s.lrc", dir_path, p->song_name);
        i++;
      }
    }
  } while (!_findnext(handle, &fileinfo));
  _findclose(handle);
  Zplay_Destroy();

  return pHead;
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
  printf("zplay: %s\n", zplay_GetError(Global_player));
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

int Zplay_GetPuaseStatus(void) {
  TStreamStatus status;
  zplay_GetStatus(Global_player, &status);
  return status.fPause;
}

//返回毫秒数
int Zplay_GetPosition(void) {
  TStreamTime pos;
  int times = 0;
  zplay_GetPosition(Global_player, &pos);
  times += pos.hms.minute * 60000;
  times += pos.hms.second * 1000;
  times += pos.hms.millisecond;

  return times;
}

//返回毫秒数
int Zplay_GetTimeLength(void) {
  TStreamInfo pInfo;
  int times = 0;
  zplay_GetStreamInfo(Global_player, &pInfo);
  times += pInfo.Length.hms.minute * 60000;
  times += pInfo.Length.hms.second * 1000;
  times += pInfo.Length.hms.millisecond;

  return times;
}

//偏移秒
int Zplay_Seek(int seek_time) {
  TStreamTime pTime;
  pTime.sec = seek_time / 1000;
  return zplay_Seek(Global_player, tfSecond, &pTime, smFromBeginning);
}

/*
 * zplay设置
 */
int Zplay_SetSettings() {
  return zplay_SetSettings(Global_player, sidAccurateSeek, 1);
}

/* 音效设置 */

/* EQ均衡器带宽增益控制 */

int Zplay_SetEquailzerPoints(void) {
  int FreqPoint[8] = {32, 63, 125, 250, 500, 1000, 2000, 4000};
  return zplay_SetEqualizerPoints(Global_player, FreqPoint, 8);
}

void Zplay_GetEquailzerPoints(void) {
  int nNumOfPoints = zplay_GetEqualizerPoints(Global_player, NULL, 0);
  int* pPoints = new int[nNumOfPoints];  // allocate memory
  int i;
  int number_of_points = zplay_GetEqualizerPoints(Global_player, pPoints, nNumOfPoints);
  for (i = 0; i < number_of_points; i++) {
    printf("Band %i: %i\n", i, pPoints[i]);
  }
  delete[] pPoints;
}

int Zplay_SetEqualizerParam(int* nBandGain, int nNumOfBands) {
  int i = 0;
  for (i = 0; i < nNumOfBands; i++) {
    zplay_SetEqualizerBandGain(Global_player, i, nBandGain[i]);
  }
  return 0;
  // return zplay_SetEqualizerParam(Global_player, 0 , nBandGain, nNumOfBands);
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
  unsigned int l_temp, r_temp;
  zplay_GetPlayerVolume(Global_player, &l_temp, &r_temp);

  return l_temp;
}

END_C_DECLS