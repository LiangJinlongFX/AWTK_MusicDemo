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
#else
#endif/*WITH_STB_IMAGE*/
#ifdef WITH_STB_FONT
#ifdef WITH_MINI_FONT
#else/*WITH_MINI_FONT*/
#include "assets/inc/fonts/default.res"
#endif/*WITH_MINI_FONT*/
#else/*WITH_STB_FONT*/
#endif/*WITH_STB_FONT*/
#endif/*WITH_FS_RES*/

#include "assets/inc/data/Favor.data"
#include "assets/inc/data/Blow_You_Up.data"
#include "assets/inc/data/Feel_Alive.data"
#include "assets/inc/data/Turn_It_Up.data"

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

  assets_manager_add(rm, data_Favor_lrc);
  assets_manager_add(rm, data_Blow_You_Up_lrc);
  assets_manager_add(rm, data_Feel_Alive_lrc);
  assets_manager_add(rm, data_Turn_It_Up_lrc);

  tk_init_assets();
  return RET_OK;
}
