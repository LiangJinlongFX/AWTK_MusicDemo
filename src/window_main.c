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
#include "custom_widgets/knob/knob.h"

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

static ret_t close_equalizer(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  dialog_quit(dialog, 0);

  return RET_OK;
}

static ret_t close_playlist(void* ctx, event_t* e) {
  widget_t* dialog = (widget_t*)ctx;
  dialog_quit(dialog, 0);

  return RET_OK;
}

static ret_t on_playlist(void* ctx, event_t* e) {
  widget_t* dialog_list = dialog_open("dialog_playlist");
  widget_child_on(dialog_list,"dialog_close",EVT_CLICK,close_playlist,dialog_list);
  dialog_modal(dialog_list);
  
  return RET_OK;
}

static ret_t on_advance(void* ctx, event_t* e) {
  widget_t* dialog_list = dialog_open("advance");
  widget_child_on(dialog_list,"dialog_close",EVT_CLICK,close_playlist,dialog_list);
  dialog_modal(dialog_list);
  
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
  /* 注册按钮事件 */
  widget_t* button_list = widget_lookup(win_main, "music:list", TRUE);
  widget_t* button_EQ = widget_lookup(win_main, "music:equalizer", TRUE);
  widget_t* button_Advance = widget_lookup(win_main, "music:advance", TRUE);
  widget_on(button_list, EVT_CLICK, on_playlist, win_main);
  widget_on(button_EQ, EVT_CLICK, on_equalizer, win_main);
  widget_on(button_Advance, EVT_CLICK, on_advance, win_main);
}
