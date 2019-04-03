#ifndef _API_H
#define _API_H

#include <awtk.h>
#include "lrc.h"

BEGIN_C_DECLS

#define AUDIO_DIR "D:\\audiofile\\"


/*
 * 曲目信息存储结构体
 */
typedef struct _music_info_t {
    //索引
    unsigned int index;
    //歌曲名称
    char song_name[100];
    //艺术家名称
    char Artist_name[100];
    //专辑名称
    char Album_name[100];
    //歌曲文件路径
    char song_path[100];
    //歌词文件路径
    char lyric_path[200];
    //歌曲专辑封面路径
    char cover_path[200];
    //歌曲总时长(ms)
    unsigned int total_time;
    //下一曲
    struct _music_info_t* next;  
}music_info_t,*music_info_p;



/*
 * 当前音乐播放状态存储结构体
 * play_mode 0:顺序播放 1：随机播放 2：单曲循环
 */
typedef struct _current_info_t {
    //当前歌单链表索引
    int index;
    //当前检索到的歌单链表
    music_info_t* play_list;
    //歌单中的歌曲数
    uint32_t music_num;
    //当前lrc链表索引
    int lrc_index;
    //当前歌曲播放模式
    uint8_t play_mode;
    //当前是否在播放
    bool_t is_play;
    //当前歌曲总时长
    uint32_t total_time;
    //当前播放时长
    uint32_t play_time;
    //当前播放的歌曲名称
    char* song_name;
    //当前播放歌曲歌手名称
    char* singer_name;
    //当前播放的歌曲专辑封面名称
    char* album_cover;
    //当前播放的歌曲歌词链表
    lyric_t* song_lyric;
    //当前的均衡器值信息
    int EQ_Params[8];
    //
    uint16_t Pitch;
    //
    uint16_t Rate;
    //
    uint16_t Tempo;
    //
    uint16_t Volume;
} current_info_t;



/*将char类型字符串转换成wchar_t类型 */
void chat_to_wchar(char* str1, wchar_t* str2);
/**/
void wchar_to_chat(wchar_t* str1, char* str2);
/* 将毫秒数转换成m:n格式 */
void time_to_wchar(uint32_t s, wchar_t* str);

music_info_t* musiclist_default(void);



END_C_DECLS

#endif //_API_H
