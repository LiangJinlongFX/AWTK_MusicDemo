#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "assets/inc/strings/en_US.data"
#include "assets/inc/strings/zh_CN.data"
#include "assets/inc/styles/default.data"
#include "assets/inc/styles/dialog.data"
#include "assets/inc/styles/music_player.data"
#include "assets/inc/styles/system_bar.data"
#include "assets/inc/ui/dialog_playlist.data"
#include "assets/inc/ui/diglog_setting.data"
#include "assets/inc/ui/main.data"
#include "assets/inc/ui/system_bar.data"
#ifdef WITH_STB_IMAGE
#include "assets/inc/images/123.res"
#include "assets/inc/images/advance.res"
#include "assets/inc/images/background_1.res"
#include "assets/inc/images/close.res"
#include "assets/inc/images/equalizer.res"
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
#include "assets/inc/images/上一曲_失效.res"
#include "assets/inc/images/下一曲_失效.res"
#include "assets/inc/images/单曲循环.res"
#include "assets/inc/images/均衡器上的圆点.res"
#include "assets/inc/images/头部的分割线.res"
#include "assets/inc/images/播放进度的圆点.res"
#include "assets/inc/images/歌词的分割线.res"
#include "assets/inc/images/随机播放.res"
#include "assets/inc/images/顺序播放.res"
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
  assets_manager_add(rm, image_close);
  assets_manager_add(rm, image_equalizer);
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
  assets_manager_add(rm, image_上一曲_失效);
  assets_manager_add(rm, image_下一曲_失效);
  assets_manager_add(rm, image_单曲循环);
  assets_manager_add(rm, image_均衡器上的圆点);
  assets_manager_add(rm, image_头部的分割线);
  assets_manager_add(rm, image_播放进度的圆点);
  assets_manager_add(rm, image_歌词的分割线);
  assets_manager_add(rm, image_随机播放);
  assets_manager_add(rm, image_顺序播放);
  assets_manager_add(rm, style_default);
  assets_manager_add(rm, style_dialog);
  assets_manager_add(rm, style_music_player);
  assets_manager_add(rm, style_system_bar);
  assets_manager_add(rm, ui_dialog_playlist);
  assets_manager_add(rm, ui_diglog_setting);
  assets_manager_add(rm, ui_main);
  assets_manager_add(rm, ui_system_bar);
  assets_manager_add(rm, strings_en_US);
  assets_manager_add(rm, strings_zh_CN);
#endif

  tk_init_assets();
  return RET_OK;
}
