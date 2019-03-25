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


static ret_t on_playlist(void* ctx, event_t* e) {
  dialog_open("dialog_playlist");
  return RET_OK;
}

static ret_t on_equalizer(void* ctx, event_t* e) {
  dialog_open("equalizer");
  
  return RET_OK;
}

/**
 * 初始化
 */
void application_init() {
  window_manager();
  widget_t* system_bar = window_open("system_bar");
  widget_t* win_main = window_open("main");
  /* 注册按钮事件 */
  widget_t* button_list = widget_lookup(win_main, "music:list", TRUE);
  widget_t* button_EQ = widget_lookup(win_main, "music:equalizer", TRUE);
  widget_on(button_list, EVT_CLICK, on_playlist, win_main);
  widget_on(button_EQ, EVT_CLICK, on_equalizer, win_main);
}
