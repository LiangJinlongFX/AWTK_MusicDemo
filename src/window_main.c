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
#include "stdlib.h"
#include "custom_widgets/chart/chart_view.h"
#include "custom_widgets/chart/line_series.h"
#include "custom_widgets/chart/bar_series.h"
#include "custom_widgets/album_cover/album_cover.h"
#include "custom_widgets/knob/knob.h"
#include "api/api.h"
#include "api/lrc.h"

/* 全局变量定义区 */
current_info_t* Global_Current_Info=NULL;
widget_t* Main_Window=NULL;

static load_assets_manager(char* name) {
  char str[100];
  strcpy(str, name);
  my_str_replace(str, ' ', '_');
  strcat(str,".lrc");
  assets_manager_t* rm = assets_manager();
  asset_info_t* t = assets_manager_ref(rm, ASSET_TYPE_DATA, str);
  if(t != NULL)
    return t->data;
  else
    return NULL;
}


static void lrc_loaditem(current_info_t* info) {
  char str[20];
  uint16_t count=0;
  uint16_t i=0;
  lyric_t* p=NULL;
  widget_t* item=NULL;
  p = info->song_lyric;
  /* 查找歌词栏容器 */
  widget_t* lyric_list = widget_lookup(Main_Window, "lyric_list", TRUE);
  /* 销毁所有歌词栏下的label */
  widget_destroy_children(lyric_list);

  /* song_lyric为空,创建默认控件 */
  if(p == NULL) {
    item = label_create(lyric_list, 100, 200, 80, 30);
    widget_set_text_utf8(item, "");
    item = label_create(lyric_list, 100, 200, 80, 30);
    widget_set_text_utf8(item, "");
    item = label_create(lyric_list, 100, 200, 80, 30);
    widget_set_text_utf8(item, "Can't find the lrc data");
    item = label_create(lyric_list, 100, 200, 80, 30);
    widget_set_text_utf8(item, "");
    item = label_create(lyric_list, 100, 200, 80, 30);
    widget_set_text_utf8(item, "");
  }
  /* 统计当前歌曲歌词的所有行数 */
  while(p != NULL) {
    p = p->next;
    count++;
  }
  for(i=0;i<3;i++) {
    item = label_create(lyric_list, 100, 200, 80, 30);
  }
  /* 创建所有歌词标签 */
  p = info->song_lyric;
  for(i=0;i<count;i++) {
    item = label_create(lyric_list, 100, 200, 80, 30);
    snprintf(str, 20, "lrclist_%d", i);
    widget_set_name(item, str);
    widget_set_text_utf8(item, p->verse);
    p = p->next; 
  }
  for(i=0;i<5;i++) {
    item = label_create(lyric_list, 100, 200, 80, 30);
  }
  info->lrc_item = 0;
  scroll_view_scroll_to(lyric_list,0,0,10);
}


static void music_switch(bool_t is_next) {
  int i;
  widget_t* widget;
  char str[50];
  wchar_t wstr[50];
  music_info_t* p=NULL;

  /* 根据播放模式改变音乐播放索引 */
  if(Global_Current_Info->play_mode == 2) {
    i = Global_Current_Info->index;
  } else if (Global_Current_Info->play_mode == 1) {
    i = rand()%Global_Current_Info->music_num;
    if (i < 0 || i >= Global_Current_Info->music_num) {
      i = 0;
    }
  } else {
    if (is_next) {
      i = Global_Current_Info->index + 1;
      if (i >= Global_Current_Info->music_num) i = 0;
    } else {
      i = Global_Current_Info->index - 1;
      if (i < 0) i = 0;
    }
  }
  Global_Current_Info->index = i;
  printf("Global_Current_Info index: %d\n", i);
  // print_playlist(Global_Current_Info->play_list);
  p = musiclist_find(Global_Current_Info->play_list,i);
  //重新加载歌词文件
  lyric_delete(Global_Current_Info->song_lyric);
  //Global_Current_Info->song_lyric = lyric_analysis("F:\\AWTK_Develop\\C-C-_Unit\\123.lrc");
  char* lrc_data = load_assets_manager(p->song_name);
  if(lrc_data != NULL)
    Global_Current_Info->song_lyric = lyric_load(lrc_data);
  else 
    Global_Current_Info->song_lyric = NULL;

  //TODO:添加加载曲目失败的提示信息
  if(p == NULL) {
    return ;
  }

  Global_Current_Info->song_name = p->song_name;
  Global_Current_Info->singer_name = p->Artist_name;
  Global_Current_Info->total_time = p->total_time;
  Global_Current_Info->play_time = 0;
  //FIXME:定位到加载专辑封面图片会造成内存异常
  //加载专辑封面图片
  widget = widget_lookup(Main_Window, "cover", TRUE);
  sprintf(str,"cover_%02d",Global_Current_Info->index);
  album_cover_set_image(widget,str);
  //显示歌曲名称
  chat_to_wchar(Global_Current_Info->song_name,wstr);
  widget = widget_lookup(Main_Window, "song_name", TRUE);
  widget_set_text(widget,wstr);
  //显示歌手名称
  chat_to_wchar(Global_Current_Info->singer_name,wstr);
  widget = widget_lookup(Main_Window, "singer", TRUE);
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
  /* 加载歌词控件 */
  lrc_loaditem(Global_Current_Info);
  /* 切歌时实现自动播放 */
  album_cover_t* album_cover = ALBUM_COVER(widget_lookup(Main_Window, "cover", TRUE));
  if(Global_Current_Info->is_play) {
    widget_create_animator(album_cover, "cartridge_rotation(from=-45, to=-3, repeat_times=1, duration=1000, yoyo_times=2, easing=sin_inout)");
  } else {
    widget = widget_lookup(Main_Window, "music:play", TRUE);
    widget_use_style(widget,"pause");
    Global_Current_Info->is_play = TRUE;
    album_cover_start(album_cover);
  }
}



/**
 * 歌词滚动显示显示实现函数
 **/
static void lyric_display(widget_t* win) {
  widget_t* widget=NULL;
  uint32_t line=0;
  char str[20];
  int i;
  /* 查找歌词栏容器 */
  widget_t* lyric_list = widget_lookup(Main_Window, "lyric_list", TRUE);
  /* 用于拖动进度条时快速清除高亮 */
  if(Global_Current_Info->is_scroll) {
    Global_Current_Info->is_scroll = FALSE;
    i = lyric_findwithcounter(Global_Current_Info->song_lyric, Global_Current_Info->play_time, 20000);
  } else {
    i = lyric_findwithcounter(Global_Current_Info->song_lyric, Global_Current_Info->play_time, 1000);
  }
  if(i>=0) {
    printf("lrc_item: %d\n", i);
    snprintf(str, 20, "lrclist_%d", i);
    widget = widget_lookup(lyric_list, str, TRUE);
    widget_use_style(widget,"lyric_light");
    snprintf(str, 20, "lrclist_%d", Global_Current_Info->lrc_previtem);
    widget = widget_lookup(lyric_list, str, TRUE);
    widget_use_style(widget,"default");
    Global_Current_Info->lrc_previtem = i;
    if(i>=Global_Current_Info->lrc_item+2 || i<= Global_Current_Info->lrc_item-2) {
      Global_Current_Info->lrc_item = i;
      line = (Global_Current_Info->lrc_previtem-2+3)*24;
      scroll_view_scroll_to(lyric_list,0,line,500);
    }
  }
}

/**
 * 播放进度定时器回调函数
 * 采用模拟的播放时间方式
 **/
static ret_t timer_preload(const timer_info_t* timer) {

  uint32_t i=0;
  wchar_t str[10];
  widget_t* win = WIDGET(timer->ctx);
  widget_t* bar = widget_lookup(win, "music_progress", TRUE);
  widget_t* label = widget_lookup(win, "play_time", TRUE);

  if(Global_Current_Info->is_play) {
    Global_Current_Info->play_time += 1000;
    if(Global_Current_Info->play_time > Global_Current_Info->total_time)
      music_switch(TRUE);
    time_to_wchar(Global_Current_Info->play_time,str);
    widget_set_text(label,str);
    widget_set_value(bar,Global_Current_Info->play_time/1000);
  }

  return RET_REPEAT;
}



static ret_t on_musicprocess(void* ctx, event_t* e) {  
  widget_t* bar = widget_lookup(WIDGET(ctx), "music_progress", TRUE);
  printf("on_musicprocess\n");
  Global_Current_Info->play_time = widget_get_value(bar);
  Global_Current_Info->play_time *= 1000;
  Global_Current_Info->is_scroll = TRUE;

  return RET_OK;
}

static ret_t on_musicprocess_changed(void* ctx, event_t* e) {  
  lyric_display(NULL);

  return RET_OK;
}


/**
 * 关闭对话框按钮的回调函数
 **/
static ret_t close_dialog(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  dialog_quit(dialog, 0);

  return RET_OK;
}

/**
 * 检索选中的条目索引并切歌
 **/
static ret_t on_playlistchanged(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  widget_t* music_item;
  uint16_t i=0;
  uint32_t res;
  char str[10];

  //检测当前歌单索引改变值
  for(i=0;i<10;i++) {
    snprintf(str, 10, "item_%d", i);
    music_item = widget_lookup(dialog, str, TRUE);
    res = widget_get_value(music_item);
    if(res)
      break;
  }
  /* 若不是选中在播的条目则切歌 */
  if(i != Global_Current_Info->index) {
    Global_Current_Info->index = i-1;
    Global_Current_Info->play_mode = 3;
    on_modechanged(Main_Window, NULL);
    music_switch(TRUE);
  }
}

/**
  * @method 
  * 从歌单链表中加载歌单并写入到对话框控件中
  * 根据当前链表的歌单数目动态更改歌单条目button数目
 **/
static ret_t load_playlist(widget_t* dialog)
{
  int i=0;
  char str[100];
  wchar_t wstr[100];
  widget_t* music_item;
  music_info_t* p=NULL;
  widget_t* list_view = widget_lookup(dialog, "list_view", TRUE);
  
  /* 检索歌单链表并注册条目控件 */
  for(i=0; i<Global_Current_Info->music_num; i++) {
    p = musiclist_find(Global_Current_Info->play_list,i);
    if(p != NULL) {
      music_item = check_button_create_radio(list_view, 100, 200, 80, 30);
      widget_use_style(music_item,"list_check");
      widget_set_value(music_item, FALSE);
      snprintf(str, 100, "item_%d", i);
      widget_set_name(music_item,str);
      snprintf(str, 100, "%s-%s",p->song_name,p->Artist_name);
      chat_to_wchar(str,wstr);
      widget_set_text(music_item,wstr);
      widget_on(music_item, EVT_VALUE_CHANGED, on_playlistchanged, dialog);
    }
  }

  return RET_OK;  
}

/**
 * 歌单列表打开事件
 **/
static ret_t on_playlist(void* ctx, event_t* e) {
  widget_t* dialog_list = dialog_open("dialog_playlist");
  widget_t* music_item;
  char str[30];

  widget_child_on(dialog_list,"dialog_close",EVT_CLICK,close_dialog,dialog_list);
  //加载歌单
  load_playlist(dialog_list);
  //设置当前播放的歌曲为选中模式
  snprintf(str, 30, "item_%d", Global_Current_Info->index);
  music_item = widget_lookup(dialog_list, str, TRUE);
  check_button_set_value(music_item,TRUE);
  
  return dialog_modal(dialog_list);
}

/**
 * 旋钮值改变事件
 **/
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

  return RET_OK;
}

/**
 * 打开增益旋钮对话框的回调函数
 **/
static ret_t on_advance(void* ctx, event_t* e) {
  widget_t* dialog_list = dialog_open("advance");
  widget_t* knob_item;
  /* 更新旋钮值 */
  knob_item = widget_lookup(dialog_list, "knob0", TRUE);
  knob_set_value(knob_item,Global_Current_Info->Pitch);
  knob_item = widget_lookup(dialog_list, "knob1", TRUE);
  knob_set_value(knob_item,Global_Current_Info->Rate);
  knob_item = widget_lookup(dialog_list, "knob2", TRUE);
  knob_set_value(knob_item,Global_Current_Info->Tempo);
  knob_item = widget_lookup(dialog_list, "knob3", TRUE);
  knob_set_value(knob_item,Global_Current_Info->Volume);
  widget_child_on(dialog_list,"dialog_close",EVT_CLICK,close_dialog,dialog_list);  
  widget_child_on(dialog_list,"knob0",EVT_VALUE_CHANGED,advance_changed,dialog_list);
  widget_child_on(dialog_list,"knob1",EVT_VALUE_CHANGED,advance_changed,dialog_list);
  widget_child_on(dialog_list,"knob2",EVT_VALUE_CHANGED,advance_changed,dialog_list);
  widget_child_on(dialog_list,"knob3",EVT_VALUE_CHANGED,advance_changed,dialog_list);
  dialog_modal(dialog_list);
  
  return RET_OK;
}


/**
 * 音乐播放模式按钮事件回调函数
 **/
static ret_t on_modechanged(void* ctx, event_t* e) {
  widget_t* button = widget_lookup(WIDGET(ctx), "music:mode", TRUE);
  
  /* 改变播放模式序号并改变按钮样式 */
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
 * 上一曲切歌按钮事件回调函数
 **/
static ret_t on_musicprevious(void* ctx, event_t* e) {
  music_switch(FALSE);
  
  return RET_OK;
}

/**
 * 下一曲切歌按钮事件函数
 **/
static ret_t on_musicnext(void* ctx, event_t* e) {
  music_switch(TRUE);

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


/**
 * EQ均衡器值改变事件回调函数
 **/
static ret_t equalizer_changed(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  float_t* new_value = TKMEM_ZALLOCN(float, 8);
  widget_t* chart_view = widget_lookup(dialog, "chartview", TRUE);
  widget_t* series;
  widget_t* slider;

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

  /* 更新曲线图数值 */
  series = chart_view_get_series(chart_view, WIDGET_TYPE_LINE_SERIES, 0);
  if (series) {
      line_series_set_data(series, new_value, 8);
  }
  TKMEM_FREE(new_value);

  return RET_OK;
}

static ret_t on_equalizer(void* ctx, event_t* e) {
  widget_t* dialog_eq = dialog_open("equalizer");
  widget_t* slider_item;
  int temp; 
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
  
  widget_child_on(dialog_eq,"return",EVT_CLICK,close_dialog,dialog_eq);
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

static ret_t on_setting(void* ctx, event_t* e) {
  widget_t* win = (widget_t*)ctx;
  widget_t* advance_item = widget_lookup(win, "music:equalizer", TRUE);
  widget_t* equalizer_item = widget_lookup(win, "music:advance", TRUE);
  widget_t* setting_item = widget_lookup(win, "music:setting", TRUE);
  if(widget_get_value(setting_item)) {
    printf("visible\n");
    widget_set_visible(advance_item, TRUE, FALSE);
    widget_set_visible(equalizer_item, TRUE, FALSE);
    widget_set_enable(advance_item, TRUE);
    widget_set_enable(equalizer_item, TRUE);
  } else {
    printf("unvisible\n");
    widget_set_visible(advance_item, FALSE, FALSE);
    widget_set_visible(equalizer_item, FALSE, FALSE);
    widget_set_enable(advance_item, FALSE);
    widget_set_enable(equalizer_item, FALSE);
  }

  return RET_OK;
}

static ret_t on_change_locale(void* ctx, event_t* e) { 

  widget_t* win = (widget_t*)ctx;
  widget_t* widget = widget_lookup(win, "tr_button", TRUE);
  if(widget_get_value(widget)) {
    printf("zh_CN\n");
    locale_info_change(locale_info(), "zh", "CN"); 
  } else {
    printf("en_US\n");
    locale_info_change(locale_info(), "en", "US");
  }

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

  /* 初始化播放状态结构体 */
  Global_Current_Info = TKMEM_ZALLOC(current_info_t);
  Global_Current_Info->play_list = musiclist_default();
  Global_Current_Info->music_num = musiclist_count(Global_Current_Info->play_list);
  Global_Current_Info->is_play = FALSE;
  Global_Current_Info->is_scroll = FALSE;
  Global_Current_Info->play_mode = 0;
  Global_Current_Info->index = -1;
  
  /* 添加播放检测定时器 */
  timer_add(timer_preload, win_main, 1000);
  /* 启动时进行自动切歌播放 */
  music_switch(TRUE);

  /* 设置中英切换按钮 */
  widget_t* tr_button = widget_lookup(system_bar, "tr_button", TRUE);
  widget_set_value(tr_button, 1);

  /* 默认隐藏设置图标 */
  widget_t* equalizer_item = widget_lookup(win_main, "music:equalizer", TRUE);
  widget_t* advance_item = widget_lookup(win_main, "music:advance", TRUE);
  widget_t* setting_item = widget_lookup(win_main, "music:setting", TRUE);
  widget_set_value(setting_item, 1);
  widget_set_visible(advance_item, FALSE, FALSE);
  widget_set_visible(equalizer_item, FALSE, FALSE);
  widget_set_enable(advance_item, FALSE);
  widget_set_enable(equalizer_item, FALSE);

  /* 注册控件事件 */
  widget_child_on(system_bar, "tr_button", EVT_VALUE_WILL_CHANGE, on_change_locale, system_bar);
  widget_child_on(win_main, "music:list", EVT_CLICK, on_playlist, win_main);
  widget_child_on(win_main, "music:setting", EVT_VALUE_WILL_CHANGE, on_setting, win_main);
  widget_child_on(win_main, "music:equalizer", EVT_CLICK, on_equalizer, win_main);
  widget_child_on(win_main, "music:advance", EVT_CLICK, on_advance, win_main);
  widget_child_on(win_main, "music:play", EVT_CLICK, on_play, win_main);
  widget_child_on(win_main, "music:next", EVT_CLICK, on_musicnext, win_main);
  widget_child_on(win_main, "music:previous", EVT_CLICK, on_musicprevious, win_main);
  widget_child_on(win_main, "music:mode", EVT_CLICK, on_modechanged, win_main);
  widget_child_on(win_main, "music_progress", EVT_VALUE_CHANGING, on_musicprocess, win_main);
  widget_child_on(win_main, "music_progress", EVT_VALUE_WILL_CHANGE, on_musicprocess_changed, win_main);

  /* 打开CMD窗口方便调试 */
  AllocConsole();
  freopen("conout$","w",stdout);

}

