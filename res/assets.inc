#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "assets/inc/strings/en_US.data"
#include "assets/inc/strings/zh_CN.data"
#include "assets/inc/styles/default.data"
#include "assets/inc/styles/dialog.data"
#include "assets/inc/styles/music_player.data"
#include "assets/inc/styles/system_bar.data"
#include "assets/inc/ui/advance.data"
#include "assets/inc/ui/dialog_playlist.data"
#include "assets/inc/ui/equalizer.data"
#include "assets/inc/ui/main.data"
#include "assets/inc/ui/system_bar.data"
#ifdef WITH_STB_IMAGE
#include "assets/inc/images/123.res"
#include "assets/inc/images/advance.res"
#include "assets/inc/images/background_1.res"
#include "assets/inc/images/bg_bar.res"
#include "assets/inc/images/bg_bar_v.res"
#include "assets/inc/images/close.res"
#include "assets/inc/images/cycle.res"
#include "assets/inc/images/equalizer.res"
#include "assets/inc/images/fg_bar.res"
#include "assets/inc/images/fg_bar_v.res"
#include "assets/inc/images/icon_03.res"
#include "assets/inc/images/icon_06.res"
#include "assets/inc/images/icon_08.res"
#include "assets/inc/images/icon_12.res"
#include "assets/inc/images/icon_33.res"
#include "assets/inc/images/icon_35.res"
#include "assets/inc/images/icon_37.res"
#include "assets/inc/images/icon_39.res"
#include "assets/inc/images/icon_44.res"
#include "assets/inc/images/icon_50.res"
#include "assets/inc/images/line.res"
#include "assets/inc/images/next.res"
#include "assets/inc/images/pause.res"
#include "assets/inc/images/play.res"
#include "assets/inc/images/playlist.res"
#include "assets/inc/images/previous.res"
#include "assets/inc/images/random.res"
#include "assets/inc/images/rank.res"
#include "assets/inc/images/setting.res"
#include "assets/inc/images/slider.res"
#include "assets/inc/images/timer.res"
#include "assets/inc/images/��һ��_ʧЧ.res"
#include "assets/inc/images/��һ��_ʧЧ.res"
#include "assets/inc/images/����ѭ��.res"
#include "assets/inc/images/�������ϵ�Բ��.res"
#include "assets/inc/images/ͷ���ķָ���.res"
#include "assets/inc/images/���Ž��ȵ�Բ��.res"
#include "assets/inc/images/��ʵķָ���.res"
#include "assets/inc/images/�������.res"
#include "assets/inc/images/˳�򲥷�.res"
#else
#endif/*WITH_STB_IMAGE*/
#ifdef WITH_STB_FONT
#ifdef WITH_MINI_FONT
#include "assets/inc/fonts/default.mini.res"
#else/*WITH_MINI_FONT*/
#endif/*WITH_MINI_FONT*/
#else/*WITH_STB_FONT*/
#endif/*WITH_STB_FONT*/
#endif/*WITH_FS_RES*/

ret_t assets_init(void) {
  assets_manager_t* rm = assets_manager();

#ifdef WITH_FS_RES
#ifdef WITH_MINI_FONT
  asset_info_t* info = assets_manager_load(rm, ASSET_TYPE_FONT, "default.mini");
  if (info) {
    strcpy(info->name, "default");
  }
#else 
  assets_manager_load(rm, ASSET_TYPE_FONT, "default");
#endif
  assets_manager_load(rm, ASSET_TYPE_STYLE, "default");
#else
#ifdef WITH_STB_FONT
  assets_manager_add(rm, font_default);
#endif
  assets_manager_add(rm, image_123);
  assets_manager_add(rm, image_advance);
  assets_manager_add(rm, image_background_1);
  assets_manager_add(rm, image_bg_bar);
  assets_manager_add(rm, image_bg_bar_v);
  assets_manager_add(rm, image_close);
  assets_manager_add(rm, image_cycle);
  assets_manager_add(rm, image_equalizer);
  assets_manager_add(rm, image_fg_bar);
  assets_manager_add(rm, image_fg_bar_v);
  assets_manager_add(rm, image_icon_03);
  assets_manager_add(rm, image_icon_06);
  assets_manager_add(rm, image_icon_08);
  assets_manager_add(rm, image_icon_12);
  assets_manager_add(rm, image_icon_33);
  assets_manager_add(rm, image_icon_35);
  assets_manager_add(rm, image_icon_37);
  assets_manager_add(rm, image_icon_39);
  assets_manager_add(rm, image_icon_44);
  assets_manager_add(rm, image_icon_50);
  assets_manager_add(rm, image_line);
  assets_manager_add(rm, image_next);
  assets_manager_add(rm, image_pause);
  assets_manager_add(rm, image_play);
  assets_manager_add(rm, image_playlist);
  assets_manager_add(rm, image_previous);
  assets_manager_add(rm, image_random);
  assets_manager_add(rm, image_rank);
  assets_manager_add(rm, image_setting);
  assets_manager_add(rm, image_slider);
  assets_manager_add(rm, image_timer);
  assets_manager_add(rm, image_��һ��_ʧЧ);
  assets_manager_add(rm, image_��һ��_ʧЧ);
  assets_manager_add(rm, image_����ѭ��);
  assets_manager_add(rm, image_�������ϵ�Բ��);
  assets_manager_add(rm, image_ͷ���ķָ���);
  assets_manager_add(rm, image_���Ž��ȵ�Բ��);
  assets_manager_add(rm, image_��ʵķָ���);
  assets_manager_add(rm, image_�������);
  assets_manager_add(rm, image_˳�򲥷�);
  assets_manager_add(rm, style_default);
  assets_manager_add(rm, style_dialog);
  assets_manager_add(rm, style_music_player);
  assets_manager_add(rm, style_system_bar);
  assets_manager_add(rm, ui_advance);
  assets_manager_add(rm, ui_dialog_playlist);
  assets_manager_add(rm, ui_equalizer);
  assets_manager_add(rm, ui_main);
  assets_manager_add(rm, ui_system_bar);
  assets_manager_add(rm, strings_en_US);
  assets_manager_add(rm, strings_zh_CN);
#endif

  tk_init_assets();
  return RET_OK;
}