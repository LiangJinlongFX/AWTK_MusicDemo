/**
 * File:   edit.c
 * Author: AWTK Develop Team
 * Brief:  window main
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-11-20 TangJunJie created
 *
 */
#include "awtk.h"
#include "custom_widgets/chart/chart_view.h"
#include "custom_widgets/chart/line_series.h"
#include "custom_widgets/chart/bar_series.h"
#include "custom_widgets/album_cover/album_cover.h"
#include "custom_widgets/knob/knob.h"
#include "api/api.h"
#include "api/lrc.h"
#include "api/audio.h"


//TODO:
current_info_t* Global_Current_Info;
widget_t* Main_Window;
bool_t is_scroll=FALSE;

/**
  * @method 
  * 歌曲切换
  * @param {bool_t} 切换指示  TURE:切换下一曲  FALSE:切换到上一曲
  * 
  * @return void
  */
static void music_switch(bool_t is_next) {
  int i;
  widget_t* widget;
  char str[30];
  wchar_t wstr[50];
  char cover_name[20];

  if(Global_Current_Info->play_mode == 2) {
    i = Global_Current_Info->index;
  }
  else {
    if(is_next) {
      i = Global_Current_Info->index + 1;
      if(i>=Global_Current_Info->music_num) {
        i=0;
      }
    } else {
      i = Global_Current_Info->index - 1;
      if(i<0)
      i = 0;
    }
  }
  Global_Current_Info->index = i;
  //TODO:检测当前的播放索引
  print_playlist(Global_Current_Info->play_list);
  printf("current_music_index: %d\n", Global_Current_Info->index);
  //关闭音频文件
  Zplay_CloseFile();
  music_info_t* p = musiclist_find(Global_Current_Info->play_list,i);
  if(p == NULL) return;
  //重新加载歌词文件
  lyric_delete(Global_Current_Info->song_lyric);
  //TODO:检查歌词文件加载
  printf("%s\n",p->lyric_path);
  Global_Current_Info->song_lyric = lyric_analysis(p->lyric_path);
  if(Global_Current_Info->song_lyric == NULL) {
    Global_Current_Info->song_lyric = lyric_default();
  }
  //复制歌曲信息到当前播放状态信息结构体中
  Global_Current_Info->song_name = p->song_name;
  Global_Current_Info->singer_name = p->Artist_name;
  Global_Current_Info->total_time = p->total_time;
  Global_Current_Info->play_time = 0;
  Global_Current_Info->lrc_index = 0;
  //打开音频文件
  if(!Zplay_OpenFile(p->song_path))
    return;
  //获取总时长
  Global_Current_Info->total_time = Zplay_GetTimeLength();
  //加载专辑封面图片
  widget = widget_lookup(Main_Window, "cover", TRUE);
  sprintf(wstr,"cover_%02d",Global_Current_Info->index);
  album_cover_set_image(widget,wstr);
  //显示歌曲名称
  widget = widget_lookup(Main_Window, "song_name", TRUE);
  chat_to_wchar(Global_Current_Info->song_name, wstr);
  widget_set_text(widget,wstr);
  //显示歌手名称
  widget = widget_lookup(Main_Window, "singer", TRUE);
  chat_to_wchar(Global_Current_Info->singer_name, wstr);
  widget_set_text(widget,wstr);
  //调整进度条最大值为总播放时长
  widget = widget_lookup(Main_Window, "music_progress", TRUE);
  slider_set_max(widget,Global_Current_Info->total_time/1000);
  widget_set_value(widget,Global_Current_Info->play_time/1000);
  //调整进度条总时长显示
  widget = widget_lookup(Main_Window, "total_time", TRUE);
  time_to_wchar(Global_Current_Info->total_time,wstr);
  widget_set_text(widget,wstr);
  //复位已播放时长
  widget = widget_lookup(Main_Window, "play_time", TRUE);
  time_to_wchar(Global_Current_Info->play_time,wstr);
  widget_set_text(widget,wstr);
  //复位歌词显示栏
  for(i=0;i<=9;i++) {
    sprintf(str, "lrclist_%d", i);
    widget = widget_lookup(Main_Window, str, TRUE);
    if(i == 2) {
      widget_set_text(widget,L"AWTK");
    } else if(i == 3) {
      widget_set_text(widget,L"MusicPlayer Demo");
    } else {
      widget_set_text(widget,L"");
    }
  }
  //复位歌词滚动标志
  is_scroll = FALSE;
  //切换时自动播放
  Zplay_Play();
  /* 启动播放动画 */
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(Main_Window, "cover", TRUE));
  widget = widget_lookup(Main_Window, "music:play", TRUE);
  widget_use_style(widget,"pause");
  Global_Current_Info->is_play = TRUE;
  album_cover_start(album_cover);
}

static ret_t equalizer_changed(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  float_t* new_value = TKMEM_ZALLOCN(float, 8);
  widget_t* chart_view = widget_lookup(dialog, "chartview", TRUE);
  widget_t* series;
  widget_t* slider;
  uint32_t i, j;
  value_t* v;
  uint32_t nr = chart_view_count_series(chart_view, WIDGET_TYPE_LINE_SERIES);
  for (i = 0; i < nr; i++) {
    slider = widget_lookup(dialog, "slider::32", TRUE);
    new_value[0] = widget_get_value(slider);
    Global_Current_Info->EQ_Params[0] = ((int)new_value[0]-12)*1000;
    slider = widget_lookup(dialog, "slider::63", TRUE);
    new_value[1] = widget_get_value(slider);
    Global_Current_Info->EQ_Params[1] = ((int)new_value[1]-12)*1000;
    slider = widget_lookup(dialog, "slider::125", TRUE);
    new_value[2] = widget_get_value(slider);
    Global_Current_Info->EQ_Params[2] = ((int)new_value[2]-12)*1000;
    slider = widget_lookup(dialog, "slider::250", TRUE);
    new_value[3] = widget_get_value(slider);
    Global_Current_Info->EQ_Params[3] = ((int)new_value[3]-12)*1000;
    slider = widget_lookup(dialog, "slider::500", TRUE);
    new_value[4] = widget_get_value(slider);
    Global_Current_Info->EQ_Params[4] = ((int)new_value[4]-12)*1000;
    slider = widget_lookup(dialog, "slider::1k", TRUE);
    new_value[5] = widget_get_value(slider);
    Global_Current_Info->EQ_Params[5] = ((int)new_value[5]-12)*1000;
    slider = widget_lookup(dialog, "slider::2k", TRUE);
    new_value[6] = widget_get_value(slider);
    Global_Current_Info->EQ_Params[6] = ((int)new_value[6]-12)*1000;
    slider = widget_lookup(dialog, "slider::4k", TRUE);
    new_value[7] = widget_get_value(slider);
    Global_Current_Info->EQ_Params[7] = ((int)new_value[7]-12)*1000;

    series = chart_view_get_series(chart_view, WIDGET_TYPE_LINE_SERIES, i);
    if (series) {
      line_series_set_data(series, new_value, 8);
    }
  }
  TKMEM_FREE(new_value);
  for(i=0;i<8;i++)
    printf("EQ %d :%d\n",i,Global_Current_Info->EQ_Params[i]);
  Zplay_Enable(0);
  i = Zplay_SetEqualizerParam(Global_Current_Info->EQ_Params, 8);
  Zplay_GetError();
  Zplay_Enable(1);
  printf("Zplay_SetEqualizerParam=%d\n",i);

  return RET_OK;
}

/**
  * @method 
  * 在歌词标签实现歌词
  * 支持逐行歌词交替显示
  * @param 
  * 
  * @return 
  */
static void lyric_display(widget_t* win) {
  int i=0;
  char str[30];
  wchar_t lrcstr[250];
  static lyric_t* lrc_plist[10];
  lyric_t* p = NULL;
  widget_t* widget;

  p = lyric_find(Global_Current_Info->song_lyric,Global_Current_Info->play_time);
  if(p == NULL)
    return;
  if(!is_scroll)
    lrc_plist[2] = p;
  chat_to_wchar(lrc_plist[2]->verse,lrcstr);
  widget = widget_lookup(win, "lrclist_2", TRUE);
  widget_set_text(widget,lrcstr);
  if(!is_scroll) {
    widget_use_style(widget,"lyric_light");
  } else {
    widget_use_style(widget,"default");
  }
  lrc_plist[3] = lrc_plist[2]->next;
  widget = widget_lookup(win, "lrclist_3", TRUE);
  if(lrc_plist[3] != NULL) {
    chat_to_wchar(lrc_plist[3]->verse,lrcstr);
    widget_set_text(widget,lrcstr);
    if(is_scroll) {
      widget_use_style(widget,"lyric_light");
    } else {
      widget_use_style(widget,"default");
    }
  } else {
    widget_set_text(widget, L"");
  }

  /* 显示过去胡两行歌词 */
  for(i=1;i>=0;i--) {
    if(lrc_plist[i+1] != NULL)
      lrc_plist[i] = lrc_plist[i+1]->prev;
    sprintf(str, "lrclist_%d", i);
    widget = widget_lookup(win, str, TRUE);
    if(lrc_plist[i] != NULL) {
      chat_to_wchar(lrc_plist[i]->verse,lrcstr);
      widget_set_text(widget,lrcstr);
    } else {
      widget_set_text(widget, L"");
    }
  }

  /* 显示未来的6行歌词 */
  for(i=4;i<=9;i++) {
    if(lrc_plist[i-1] != NULL)
      lrc_plist[i] = lrc_plist[i-1]->next;
    sprintf(str, "lrclist_%d", i);
    widget = widget_lookup(win, str, TRUE);
    if(lrc_plist[i] != NULL) {
      chat_to_wchar(lrc_plist[i]->verse,lrcstr);
      widget_set_text(widget,lrcstr);
    } else {
      widget_set_text(widget, L"");
    }
  }

  is_scroll = ~is_scroll;
}

/**
  * @method 
  * GUI定时器回调函数
  * @param 
  * 
  * @return 
  */
static ret_t timer_preload(const timer_info_t* timer) {

  uint32_t i=0;
  wchar_t str[10];
  widget_t* win = WIDGET(timer->ctx);
  widget_t* bar = widget_lookup(win, "music_progress", TRUE);
  widget_t* label = widget_lookup(win, "play_time", TRUE);
  widget_t* lyric_list = widget_lookup(win, "lyric_list", TRUE);

  
  //TODO:检测歌曲是播放完毕还是异常停止
  if(Zplay_GetPlayStatus()) {
    Global_Current_Info->play_time = Zplay_GetPosition();
    time_to_wchar(Global_Current_Info->play_time,str);
    widget_set_text(label,str);
    widget_set_value(bar,Global_Current_Info->play_time/1000);
    lyric_display(win);
  } else {
    if(!Zplay_GetPuaseStatus()) {
      printf("no play and pause status, will switch music?\n");
      music_switch(TRUE);
    }
  }

  return RET_REPEAT;
}

/**
  * @method 
  * 关闭EQ均衡器对话框的回调函数
  * 从控件获取值更新EQ_Params
  * @param 
  * 
  * @return 
  */
static ret_t close_equalizer(void* ctx, event_t* e) {
  widget_t* dialog_eq = (widget_t*)ctx;
  widget_t* slider_item;
  /* 更新滑块值 */
  slider_item = widget_lookup(dialog_eq, "slider::32", TRUE);
  Global_Current_Info->EQ_Params[0] = widget_get_value(slider_item);
  slider_item = widget_lookup(dialog_eq, "slider::63", TRUE);
  Global_Current_Info->EQ_Params[1] = widget_get_value(slider_item);
  slider_item = widget_lookup(dialog_eq, "slider::125", TRUE);
  Global_Current_Info->EQ_Params[2] = widget_get_value(slider_item);
  slider_item = widget_lookup(dialog_eq, "slider::250", TRUE);
  Global_Current_Info->EQ_Params[3] = widget_get_value(slider_item);
  slider_item = widget_lookup(dialog_eq, "slider::500", TRUE);
  Global_Current_Info->EQ_Params[4] = widget_get_value(slider_item);
  slider_item = widget_lookup(dialog_eq, "slider::1k", TRUE);
  Global_Current_Info->EQ_Params[5] = widget_get_value(slider_item);
  slider_item = widget_lookup(dialog_eq, "slider::2k", TRUE);
  Global_Current_Info->EQ_Params[6] = widget_get_value(slider_item);
  slider_item = widget_lookup(dialog_eq, "slider::4k", TRUE);
  Global_Current_Info->EQ_Params[7] = widget_get_value(slider_item);
  dialog_quit(dialog_eq, 0);

  return RET_OK;
}

/**
  * @method 
  * 播放进度条值改变事件回调函数
  * @param 
  * 
  * @return 
  */
static ret_t on_musicprocess(void* ctx, event_t* e) {  
  widget_t* bar = widget_lookup(WIDGET(ctx), "music_progress", TRUE);
  Global_Current_Info->play_time = widget_get_value(bar);
  Global_Current_Info->play_time *= 1000;

  return RET_OK;
}

/**
  * @method 
  * 关闭音乐列表对话框的回调函数
  * 已注释关闭对话框实现切歌功能代码
  * @param 
  * 
  * @return 
  */
static ret_t close_playlist(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  // 歌单对话框关闭时才切歌的话使用此段代码，同时不要注册radio_button按钮事件
  // widget_t* music_item;
  // uint16_t i=0;
  // uint32_t res;
  // char str[10];
  // //加载歌单
  // for(i=0;i<10;i++) {
  //   sprintf(str,"item_%d",i);
  //   music_item = widget_lookup(dialog, str, TRUE);
  //   res = widget_get_value(music_item);
  //   if(res)
  //     break;
  // }
  // if(i != Global_Current_Info->index) {
  //   Global_Current_Info->index = i-1;
  //   music_switch(TRUE);
  // }

  dialog_quit(dialog, 0);

  return RET_OK;
}

/**
  * @method 
  * 选中的播放歌曲条目改变事件
  * 更新当前播放的歌曲索引并进行切歌
  * @param 
  * 
  * @return 
  */
static ret_t on_playlistchanged(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  widget_t* music_item;
  uint16_t i=0;
  uint32_t res;
  char str[10];

  //检测当前歌单索引改变值
  for(i=0;i<10;i++) {
    sprintf(str,"item_%d",i);
    music_item = widget_lookup(dialog, str, TRUE);
    res = widget_get_value(music_item);
    if(res)
      break;
  }
  if(i != Global_Current_Info->index) {
    Global_Current_Info->index = i-1;
    music_switch(TRUE);
  }
}

/**
  * @method 
  * 从歌单链表中加载歌单并写入到对话框控件中
  * 根据当前链表的歌单数目动态更改歌单条目button数目
  * @param 
  * 
  * @return 
  */
static ret_t load_playlist(widget_t* dialog)
{
  uint32_t i=0;
  char str[100];
  wchar_t wstr[100];
  widget_t* music_item;
  music_info_t* p;
  widget_t* list_view = widget_lookup(dialog, "list_view", TRUE);
  for(i=0;i<Global_Current_Info->music_num;i++) {
    p = musiclist_find(Global_Current_Info->play_list,i);
    if(p != NULL) {
      music_item = check_button_create_radio(list_view, 100, 200, 80, 30);
      widget_use_style(music_item,"list_check");
      widget_set_value(music_item, FALSE);
      sprintf(str,"item_%d",i);
      widget_set_name(music_item,str);
      sprintf(str,"%s-%s",p->song_name,p->Artist_name);
      chat_to_wchar(str, wstr);
      widget_set_text(music_item,wstr);
      widget_on(music_item, EVT_VALUE_CHANGED, on_playlistchanged, dialog);
    }
  }

  return RET_OK;  
}

/**
 * @method on_playlist
 * 打开歌单对话框时的回调函数
 * @param
 *
 * @return
 */
static ret_t on_playlist(void* ctx, event_t* e) {
  widget_t* dialog_list = dialog_open("dialog_playlist");
  widget_t* music_item;
  music_info_t* p;
  uint16_t i=0;
  char str[100];
  wchar_t wstr[100];
  widget_child_on(dialog_list,"dialog_close",EVT_CLICK,close_playlist,dialog_list);
  //加载歌单
  load_playlist(dialog_list);
  //设置当前播放的歌曲为选中模式
  sprintf(str,"item_%d",Global_Current_Info->index);
  music_item = widget_lookup(dialog_list, str, TRUE);
  check_button_set_value(music_item,TRUE);
  
  return dialog_modal(dialog_list);
}

/**
 * @method 
 * 关闭advance对话框时的回调函数
 * @param
 *
 * @return
 */
static ret_t close_advance(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  dialog_quit(dialog, 0);

  return RET_OK;
}

static ret_t advance_changed(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  widget_t* knob_item;
  knob_item = widget_lookup(dialog, "knob0", TRUE);
  Global_Current_Info->Pitch = widget_get_value(knob_item);
  knob_item = widget_lookup(dialog, "knob1", TRUE);
  Global_Current_Info->Rate = widget_get_value(knob_item);
  knob_item = widget_lookup(dialog, "knob2", TRUE);
  Global_Current_Info->Tempo = widget_get_value(knob_item);
  knob_item = widget_lookup(dialog, "knob3", TRUE);
  Global_Current_Info->Volume = widget_get_value(knob_item);
  Zplay_SetPitch(Global_Current_Info->Pitch);
  Zplay_SetRate(Global_Current_Info->Rate);
  Zplay_SetTempo(Global_Current_Info->Tempo);
  Zplay_SetPlayerVolume(Global_Current_Info->Volume);

  return RET_OK;
}

/**
  * @method 
  * 打开增益旋钮对话框的回调函数
  * @param 
  * 
  * @return 
  */
static ret_t on_advance(void* ctx, event_t* e) {
  widget_t* dialog_list = dialog_open("advance");
  widget_t* knob_item;
  /* 从zplayer获取设置值 */
  Global_Current_Info->Pitch = Zplay_GetPitch();
  Global_Current_Info->Rate = Zplay_GetRate();
  Global_Current_Info->Tempo = Zplay_GetTempo();
  Global_Current_Info->Volume = Zplay_GetPlayerVolume();
  /* 更新旋钮值 */
  knob_item = widget_lookup(dialog_list, "knob0", TRUE);
  knob_set_value(knob_item,Global_Current_Info->Pitch);
  knob_item = widget_lookup(dialog_list, "knob1", TRUE);
  knob_set_value(knob_item,Global_Current_Info->Rate);
  knob_item = widget_lookup(dialog_list, "knob2", TRUE);
  knob_set_value(knob_item,Global_Current_Info->Tempo);
  knob_item = widget_lookup(dialog_list, "knob3", TRUE);
  knob_set_value(knob_item,Global_Current_Info->Volume);
  widget_child_on(dialog_list,"dialog_close",EVT_CLICK,close_advance,dialog_list);
  widget_child_on(dialog_list,"knob0",EVT_VALUE_CHANGED,advance_changed,dialog_list);
  widget_child_on(dialog_list,"knob1",EVT_VALUE_CHANGED,advance_changed,dialog_list);
  widget_child_on(dialog_list,"knob2",EVT_VALUE_CHANGED,advance_changed,dialog_list);
  widget_child_on(dialog_list,"knob3",EVT_VALUE_CHANGED,advance_changed,dialog_list);
  dialog_modal(dialog_list);
  
  return RET_OK;
}

/**
  * @method 
  * 下一曲切歌按钮事件函数
  * @param 
  * 
  * @return 
  */
static ret_t on_musicnext(void* ctx, event_t* e) {
  music_switch(TRUE);

  return RET_OK;
}

/**
  * @method 
  * 音乐播放模式按钮事件回调函数
  * @param 
  * 
  * @return 
  */
static ret_t on_modechanged(void* ctx, event_t* e) {
  widget_t* button = WIDGET(ctx);
  if(Global_Current_Info->play_mode == 0) {
    Global_Current_Info->play_mode = 1;
    widget_use_style(button,"play_random");
  } else if(Global_Current_Info->play_mode == 1) {
    Global_Current_Info->play_mode = 2;
    widget_use_style(button,"play_cycle");
  } else {
    Global_Current_Info->play_mode = 0;
    widget_use_style(button,"play_rank");
  }

  return RET_OK;
}

/**
  * @method 
  * 上一曲切歌按钮事件回调函数
  * @param 
  * 
  * @return 
  */
static ret_t on_musicprevious(void* ctx, event_t* e) {
  music_switch(FALSE);
  
  return RET_OK;
}

/**
  * @method 
  * 播放按钮事件回调函数
  * @param 
  * 
  * @return 
  */
static ret_t on_play(void* ctx, event_t* e) {
  widget_t* button = widget_lookup(WIDGET(ctx), "music:play", TRUE);
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(WIDGET(ctx), "cover", TRUE));
  if(Global_Current_Info->is_play) {
    widget_use_style(button,"play");
    Global_Current_Info->is_play = FALSE;
    Zplay_Pause();
    album_cover_pause(album_cover);
    
  } else
  {
    widget_use_style(button,"pause");
    Global_Current_Info->is_play = TRUE;
    Zplay_Play();
    album_cover_start(album_cover);
  }
  
  
  return RET_OK;
}

/**
  * @method 
  * 打开均衡器对话框事件
  * @param 
  * 
  * @return 
  */
static ret_t on_equalizer(void* ctx, event_t* e) {
  widget_t* dialog_eq = dialog_open("equalizer");
  widget_t* slider_item;
  int temp;
  /* 从zplayer获取增益 */
  Zplay_GetEqualizerParam(Global_Current_Info->EQ_Params,8);
  Zplay_Enable(1);
  /* 更新滑块值 */
  slider_item = widget_lookup(dialog_eq, "slider::32", TRUE);
  temp = Global_Current_Info->EQ_Params[0]/1000 + 12;
  slider_set_value(slider_item,temp);
  slider_item = widget_lookup(dialog_eq, "slider::63", TRUE);
  temp = Global_Current_Info->EQ_Params[1]/1000 + 12;
  slider_set_value(slider_item,temp);
  slider_item = widget_lookup(dialog_eq, "slider::125", TRUE);
  temp = Global_Current_Info->EQ_Params[2]/1000 + 12;
  slider_set_value(slider_item,temp);
  slider_item = widget_lookup(dialog_eq, "slider::250", TRUE);
  temp = Global_Current_Info->EQ_Params[3]/1000 + 12;
  slider_set_value(slider_item,temp);
  slider_item = widget_lookup(dialog_eq, "slider::500", TRUE);
  temp = Global_Current_Info->EQ_Params[4]/1000 + 12;
  slider_set_value(slider_item,temp);
  slider_item = widget_lookup(dialog_eq, "slider::1k", TRUE);
  temp = Global_Current_Info->EQ_Params[5]/1000 + 12;
  slider_set_value(slider_item,temp);
  slider_item = widget_lookup(dialog_eq, "slider::2k", TRUE);
  temp = Global_Current_Info->EQ_Params[6]/1000 + 12;
  slider_set_value(slider_item,temp);
  slider_item = widget_lookup(dialog_eq, "slider::4k", TRUE);
  temp = Global_Current_Info->EQ_Params[7]/1000 + 12;
  slider_set_value(slider_item,temp);
  /* 注册滑块事件 */
  widget_child_on(dialog_eq,"return",EVT_CLICK,close_equalizer,dialog_eq);
  widget_child_on(dialog_eq,"slider::32",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::63",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::125",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::250",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::500",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::1k",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::2k",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::4k",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  /* 更新曲线图 */
  equalizer_changed(dialog_eq,NULL);
  
  return dialog_modal(dialog_eq);
}

/**
 * 初始化
 */
void application_init() {
  int i;
  window_manager();
  widget_t* system_bar = window_open("system_bar");
  widget_t* win_main = window_open("main");
  Main_Window = win_main;
  widget_t* widget;
  /* 分配内存 */
  Global_Current_Info = TKMEM_ZALLOC(current_info_t);
  Global_Current_Info->play_list = musiclist_init();
  i = Audiofile_load(AUDIO_DIR,Global_Current_Info->play_list);
  if(i <= 0) printf("cannot find the audio file\n");
  else print_playlist(Global_Current_Info->play_list);
  Global_Current_Info->music_num = musiclist_count(Global_Current_Info->play_list);
  Global_Current_Info->is_play = FALSE;
  Global_Current_Info->play_mode = 0;
  Global_Current_Info->index = -1;
  /* 初始化Zplayer */
  Zplay_Create();
  Zplay_SetEquailzerPoints();
  /* 初始化状态 */
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(win_main, "cover", TRUE));
  timer_add(timer_preload, win_main, 500);
  music_switch(TRUE);
  //更改控件透明度
  widget = widget_lookup(Main_Window, "lrclist_0", TRUE);
  widget_set_opacity(widget, 150);
  widget = widget_lookup(Main_Window, "lrclist_1", TRUE);
  widget_set_opacity(widget, 200);
  widget = widget_lookup(Main_Window, "lrclist_3", TRUE);
  widget_set_opacity(widget, 240);
  widget = widget_lookup(Main_Window, "lrclist_4", TRUE);
  widget_set_opacity(widget, 220);
  widget = widget_lookup(Main_Window, "lrclist_5", TRUE);
  widget_set_opacity(widget, 180);
  widget = widget_lookup(Main_Window, "lrclist_6", TRUE);
  widget_set_opacity(widget, 120);
  widget = widget_lookup(Main_Window, "lrclist_7", TRUE);
  widget_set_opacity(widget, 80);
  widget = widget_lookup(Main_Window, "lrclist_8", TRUE);
  widget_set_opacity(widget, 50);
  widget = widget_lookup(Main_Window, "lrclist_9", TRUE);
  widget_set_opacity(widget, 10);
  /* 注册按钮事件 */
  widget_t* button_list = widget_lookup(win_main, "music:list", TRUE);
  widget_t* button_EQ = widget_lookup(win_main, "music:equalizer", TRUE);
  widget_t* button_Advance = widget_lookup(win_main, "music:advance", TRUE);
  widget_t* button_play = widget_lookup(win_main, "music:play", TRUE);
  widget_t* button_next = widget_lookup(win_main, "music:next", TRUE);
  widget_t* button_previous = widget_lookup(win_main, "music:previous", TRUE);
  widget_t* button_playmode = widget_lookup(win_main, "music:mode", TRUE);
  widget_t* music_bar = widget_lookup(Main_Window, "music_progress", TRUE);
  widget_on(button_list, EVT_CLICK, on_playlist, win_main);
  widget_on(button_EQ, EVT_CLICK, on_equalizer, win_main);
  widget_on(button_Advance, EVT_CLICK, on_advance, win_main);
  widget_on(button_play, EVT_CLICK, on_play, win_main);
  widget_on(button_next, EVT_CLICK, on_musicnext, win_main);
  widget_on(button_previous, EVT_CLICK, on_musicprevious, win_main);
  widget_on(button_playmode, EVT_CLICK, on_modechanged, button_playmode);
  widget_on(music_bar, EVT_VALUE_CHANGED, on_musicprocess, win_main);
  //开启Console
  AllocConsole();
  freopen("conout$","w",stdout);
}
