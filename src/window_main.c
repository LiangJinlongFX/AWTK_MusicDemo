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

current_info_t* current_info;
widget_animator_t* cover_animator;
static uint32_t music_preload_nr = 0;
music_info_t* Global_Musiclist;
widget_t* Main_Window;


static void music_switch(bool_t is_next) {
  int i;
  widget_t* widget;
  wchar_t str[50];

  if(current_info->play_mode == 2) {
    i = current_info->index;
  }
  else {
    if(is_next) {
      i = current_info->index + 1;
      if(i>9)
        i=0;
    } else {
      i = current_info->index - 1;
      if(i<0)
      i = 0;
    }
  }
  current_info->index = i;
  music_info_t* p = musiclist_find(Global_Musiclist,i);
  if(p == NULL)
    return;
  current_info->song_name = p->song_name;
  current_info->singer_name = p->singer_name;
  current_info->total_time = p->total_time;
  current_info->play_time = 0;
  //显示歌曲名称
  chat_to_wchar(current_info->song_name,str);
  widget = widget_lookup(Main_Window, "song_name", TRUE);
  widget_set_text(widget,str);
  //显示歌手名称
  chat_to_wchar(current_info->singer_name,str);
  widget = widget_lookup(Main_Window, "singer", TRUE);
  widget_set_text(widget,str);
  //调整进度条最大值为总播放时长
  widget = widget_lookup(Main_Window, "music_progress", TRUE);
  slider_set_max(widget,current_info->total_time);
  widget_set_value(widget,current_info->play_time);
  //调整进度条总时长显示
  widget = widget_lookup(Main_Window, "total_time", TRUE);
  time_to_wchar(current_info->total_time,str);
  widget_set_text(widget,str);
  //复位已播放时长
  widget = widget_lookup(Main_Window, "play_time", TRUE);
  time_to_wchar(current_info->play_time,str);
  widget_set_text(widget,str);
  //切换时自动播放
  widget = widget_lookup(Main_Window, "music:play", TRUE);
  widget_use_style(widget,"pause");
  widget_animator_start(cover_animator);
  current_info->is_play = TRUE;
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(Main_Window, "cover", TRUE));
  album_cover->is_play = TRUE;
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

/* 播放进度定时器回调函数 */
static ret_t timer_preload(const timer_info_t* timer) {

  uint32_t i=0;
  wchar_t str[10];
  widget_t* win = WIDGET(timer->ctx);
  widget_t* bar = widget_lookup(win, "music_progress", TRUE);
  widget_t* label = widget_lookup(win, "play_time", TRUE);
  widget_t* lyric_list = widget_lookup(win, "lyric_list", TRUE);

  if(current_info->is_play) {
    current_info->play_time++;
    if(current_info->play_time > current_info->total_time)
      music_switch(TRUE);
    time_to_wchar(current_info->play_time,str);
    widget_set_text(label,str);
    widget_set_value(bar,current_info->play_time);
    //scroll_view_scroll_to(lyric_list,0,i,500);
    text_selector_set_selected_index(lyric_list,current_info->play_time);
    i += 100;
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
  current_info->play_time = widget_get_value(bar);

  return RET_OK;
}

static ret_t close_playlist(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  dialog_quit(dialog, 0);

  return RET_OK;
}

static ret_t on_playlist(void* ctx, event_t* e) {
  widget_t* dialog_list = dialog_open("dialog_playlist");
  widget_t* music_item;
  uint16_t i=0;
  char str[10];
  wchar_t wstr[10];
  widget_child_on(dialog_list,"dialog_close",EVT_CLICK,close_playlist,dialog_list);
  // for(i=0;i<10;i++) {
  //   sprintf(str,"r%d",i);
  //   music_item = widget_lookup(dialog_list, "r1", TRUE);
  //   // music_info_t* p = musiclist_find(Global_Musiclist,i);
  //   // if(p == NULL)
  //   //   break;
  //   // chat_to_wchar(str,wstr);
  //   widget_set_text(music_item,"Hello");
  // }

  dialog_modal(dialog_list);
  music_item = widget_lookup(dialog_list, "item_0", TRUE);
  widget_set_text(music_item,L"Hello");
  
  return RET_OK;
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
  if(current_info->play_mode == 0) {
    current_info->play_mode = 1;
    widget_use_style(button,"play_random");
  } else if(current_info->play_mode == 1) {
    current_info->play_mode = 2;
    widget_use_style(button,"play_cycle");
  } else {
    current_info->play_mode = 0;
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
  if(current_info->is_play) {
    widget_use_style(button,"play");
    widget_animator_pause(cover_animator);
    current_info->is_play = FALSE;
    album_cover->is_play = FALSE;
  } else
  {
    widget_use_style(button,"pause");
    widget_animator_start(cover_animator);
    current_info->is_play = TRUE;
    album_cover->is_play = TRUE;
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
  dialog_modal(dialog_eq);
  
  return RET_OK;
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
  current_info = TKMEM_ZALLOC(current_info_t);
  current_info->is_play = FALSE;
  current_info->play_mode = 0;
  current_info->index = -1;
  /* 初始化状态 */
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(win_main, "cover", TRUE));
  cover_animator = widget_animator_create(album_cover, "rotation(from=0, to=360, yoyo_times=0, duration=100000 auto_start=false)");
  timer_add(timer_preload, win_main, 1000);
  Global_Musiclist = musiclist_default();
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
