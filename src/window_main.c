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

current_info_t* Global_Current_Info;
widget_t* Main_Window;

static void music_switch(bool_t is_next) {
  int i;
  widget_t* widget;
  wchar_t str[50];

  if(Global_Current_Info->play_mode == 2) {
    i = Global_Current_Info->index;
  }
  else {
    if(is_next) {
      i = Global_Current_Info->index + 1;
      if(i>9)
        i=0;
    } else {
      i = Global_Current_Info->index - 1;
      if(i<0)
      i = 0;
    }
  }
  Global_Current_Info->index = i;
  music_info_t* p = musiclist_find(Global_Current_Info->play_list,i);
  //重新加载歌词文件
  lyric_delete(Global_Current_Info->song_lyric);
  Global_Current_Info->song_lyric = lyric_analysis("E:\\Code_Unit\\123.lrc");
  if(p == NULL)
    return;
  Global_Current_Info->song_name = p->song_name;
  Global_Current_Info->singer_name = p->singer_name;
  Global_Current_Info->total_time = p->total_time;
  Global_Current_Info->play_time = 0;
  Global_Current_Info->lrc_index = 0;
  //显示歌曲名称
  chat_to_wchar(Global_Current_Info->song_name,str);
  widget = widget_lookup(Main_Window, "song_name", TRUE);
  widget_set_text(widget,str);
  //显示歌手名称
  chat_to_wchar(Global_Current_Info->singer_name,str);
  widget = widget_lookup(Main_Window, "singer", TRUE);
  widget_set_text(widget,str);
  //调整进度条最大值为总播放时长
  widget = widget_lookup(Main_Window, "music_progress", TRUE);
  slider_set_max(widget,Global_Current_Info->total_time/1000);
  widget_set_value(widget,Global_Current_Info->play_time/1000);
  //调整进度条总时长显示
  widget = widget_lookup(Main_Window, "total_time", TRUE);
  time_to_wchar(Global_Current_Info->total_time,str);
  widget_set_text(widget,str);
  //复位已播放时长
  widget = widget_lookup(Main_Window, "play_time", TRUE);
  time_to_wchar(Global_Current_Info->play_time,str);
  widget_set_text(widget,str);
  //复位歌词显示栏
  widget = widget_lookup(Main_Window, "lrclist_0", TRUE);
  widget_set_text(widget,L"");
  widget = widget_lookup(Main_Window, "lrclist_1", TRUE);
  widget_set_text(widget,L"AWTK");
  widget = widget_lookup(Main_Window, "lrclist_2", TRUE);
  widget_set_text(widget,L"MusicPlayer Demo");
  widget = widget_lookup(Main_Window, "lrclist_3", TRUE);
  widget_set_text(widget,L"");
  widget = widget_lookup(Main_Window, "lrclist_4", TRUE);
  widget_set_text(widget,L"");
  //切换时自动播放
  widget = widget_lookup(Main_Window, "music:play", TRUE);
  widget_use_style(widget,"pause");
  Global_Current_Info->is_play = TRUE;
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(Main_Window, "cover", TRUE));
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
    slider = widget_lookup(dialog, "slider::63", TRUE);
    new_value[1] = widget_get_value(slider);
    slider = widget_lookup(dialog, "slider::125", TRUE);
    new_value[2] = widget_get_value(slider);
    slider = widget_lookup(dialog, "slider::250", TRUE);
    new_value[3] = widget_get_value(slider);
    slider = widget_lookup(dialog, "slider::500", TRUE);
    new_value[4] = widget_get_value(slider);
    slider = widget_lookup(dialog, "slider::1k", TRUE);
    new_value[5] = widget_get_value(slider);
    slider = widget_lookup(dialog, "slider::2k", TRUE);
    new_value[6] = widget_get_value(slider);
    slider = widget_lookup(dialog, "slider::4k", TRUE);
    new_value[7] = widget_get_value(slider);

    series = chart_view_get_series(chart_view, WIDGET_TYPE_LINE_SERIES, i);
    if (series) {
      line_series_set_data(series, new_value, 8);
    }
  }
  TKMEM_FREE(new_value);

  return RET_OK;
}

static void lyric_display(widget_t* win) {
  wchar_t lrcstr[250];
  lyric_t* lrc_plist[5];
  widget_t* widget;

  lrc_plist[2] = lyric_find(Global_Current_Info->song_lyric,Global_Current_Info->play_time);
  if(lrc_plist[2] != NULL) {
    chat_to_wchar(lrc_plist[2]->verse,lrcstr);
    widget = widget_lookup(win, "lrclist_2", TRUE);
    widget_set_text(widget,lrcstr);
    //
    lrc_plist[1] = lrc_plist[2]->prev;
    widget = widget_lookup(win, "lrclist_1", TRUE);
    if(lrc_plist[1] != NULL) {
      chat_to_wchar(lrc_plist[1]->verse,lrcstr);
      widget_set_text(widget,lrcstr);
      lrc_plist[0] = lrc_plist[1]->prev;
      widget = widget_lookup(win, "lrclist_0", TRUE);
      if(lrc_plist[0] != NULL) {
        chat_to_wchar(lrc_plist[0]->verse,lrcstr);
        widget_set_text(widget,lrcstr);
      } else {
        widget_set_text(widget,L" ");
      }
    } else {
      widget_set_text(widget,L" ");
    }
    //
    lrc_plist[3] = lrc_plist[2]->next;
    widget = widget_lookup(win, "lrclist_3", TRUE);
    if(lrc_plist[3] != NULL) {
      chat_to_wchar(lrc_plist[3]->verse,lrcstr);
      widget_set_text(widget,lrcstr);
      lrc_plist[4] = lrc_plist[3]->next;
      widget = widget_lookup(win, "lrclist_4", TRUE);
      if(lrc_plist[4] != NULL) {
        chat_to_wchar(lrc_plist[4]->verse,lrcstr);
        widget_set_text(widget,lrcstr);
      } else {
        widget_set_text(widget,L" ");
      }   
    } else {
      widget_set_text(widget,L" ");
    }

  }
}

/* 播放进度定时器回调函数 */
static ret_t timer_preload(const timer_info_t* timer) {

  uint32_t i=0;
  wchar_t str[10];
  widget_t* win = WIDGET(timer->ctx);
  widget_t* bar = widget_lookup(win, "music_progress", TRUE);
  widget_t* label = widget_lookup(win, "play_time", TRUE);
  widget_t* lyric_list = widget_lookup(win, "lyric_list", TRUE);

  if(Global_Current_Info->is_play) {
    Global_Current_Info->play_time += 500;
    if(Global_Current_Info->play_time > Global_Current_Info->total_time)
      music_switch(TRUE);
    time_to_wchar(Global_Current_Info->play_time,str);
    widget_set_text(label,str);
    widget_set_value(bar,Global_Current_Info->play_time/1000);
    lyric_display(win);
  }

  return RET_REPEAT;
}

static ret_t close_equalizer(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  dialog_quit(dialog, 0);

  return RET_OK;
}

static ret_t on_musicprocess(void* ctx, event_t* e) {  
  widget_t* bar = widget_lookup(WIDGET(ctx), "music_progress", TRUE);
  Global_Current_Info->play_time = widget_get_value(bar);
  Global_Current_Info->play_time *= 1000;

  return RET_OK;
}

static ret_t close_playlist(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  widget_t* music_item;
  uint16_t i=0;
  uint32_t res;
  char str[10];
  //加载歌单
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

  dialog_quit(dialog, 0);

  return RET_OK;
}

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
      sprintf(str,"%s-%s",p->song_name,p->singer_name);
      chat_to_wchar(str,wstr);
      widget_set_text(music_item,wstr);
    }
  }

  return RET_OK;  
}

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

static ret_t on_advance(void* ctx, event_t* e) {
  widget_t* dialog_list = dialog_open("advance");
  widget_child_on(dialog_list,"dialog_close",EVT_CLICK,close_playlist,dialog_list);
  dialog_modal(dialog_list);
  
  return RET_OK;
}

static ret_t on_musicnext(void* ctx, event_t* e) {
  music_switch(TRUE);

  return RET_OK;
}

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

static ret_t on_musicprevious(void* ctx, event_t* e) {
  music_switch(FALSE);
  
  return RET_OK;
}

static ret_t on_play(void* ctx, event_t* e) {
  widget_t* button = widget_lookup(WIDGET(ctx), "music:play", TRUE);
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(WIDGET(ctx), "cover", TRUE));
  if(Global_Current_Info->is_play) {
    widget_use_style(button,"play");
    Global_Current_Info->is_play = FALSE;
    album_cover_pause(album_cover);
    
  } else
  {
    widget_use_style(button,"pause");
    Global_Current_Info->is_play = TRUE;
    album_cover_start(album_cover);
  }
  
  
  return RET_OK;
}

static ret_t on_equalizer(void* ctx, event_t* e) {
  widget_t* dialog_eq = dialog_open("equalizer");
  
  widget_child_on(dialog_eq,"return",EVT_CLICK,close_equalizer,dialog_eq);
  widget_child_on(dialog_eq,"slider::32",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::63",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::125",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::250",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::500",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::1k",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::2k",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  widget_child_on(dialog_eq,"slider::4k",EVT_VALUE_CHANGING,equalizer_changed,dialog_eq);
  equalizer_changed(dialog_eq,NULL);
  
  return dialog_modal(dialog_eq);
}

/**
 * 初始化
 */
void application_init() {
  window_manager();
  widget_t* system_bar = window_open("system_bar");
  widget_t* win_main = window_open("main");
  Main_Window = win_main;
  widget_t* widget;
  /* 分配内存 */
  Global_Current_Info = TKMEM_ZALLOC(current_info_t);
  Global_Current_Info->play_list = musiclist_default();
  Global_Current_Info->music_num = musiclist_count(Global_Current_Info->play_list);
  Global_Current_Info->is_play = FALSE;
  Global_Current_Info->play_mode = 0;
  Global_Current_Info->index = -1;
  /* 初始化状态 */
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(win_main, "cover", TRUE));
  timer_add(timer_preload, win_main, 500);
  music_switch(TRUE);
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
}
