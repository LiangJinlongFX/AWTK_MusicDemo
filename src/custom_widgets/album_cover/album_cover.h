#ifndef ALBUM_COVER_H
#define ALBUM_COVER_H

#include "awtk.h"

BEGIN_C_DECLS

/**
 * @class ablum_cover_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 专辑封面显示控件。
 */
typedef struct _album_cover_t {
    widget_t widget;
    /*
     * 唱头图片
     */
    char* fg_image;
    /*
     * 背景图片 
     */
    char* bg_image;
    /*
     * 封面图片
     */
    char* image;
    /*
     * 控件选择角度
     */
    float_t rotation;
    /*
     * 是否设置激活状态
     */
    bool_t is_play;
    /*private*/
    int32_t timer_id;
}album_cover_t;


widget_t* album_cover_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* album_cover_cast(widget_t* widget);
ret_t album_cover_set_image(widget_t* widget, const char* name);
ret_t album_cover_set_bg_image(widget_t* widget, const char* name);
ret_t album_cover_set_fg_image(widget_t* widget, const char* name);
ret_t album_cover_start(widget_t* widget);
ret_t album_cover_pause(widget_t* widget);
ret_t album_cover_transform(widget_t* widget, canvas_t* c);
ret_t cartridge_transform(widget_t* widget, canvas_t* c);

#define WIDGET_TYPE_ALBUM_COVER "album_cover"
#define ALBUM_COVER_PROP_IMAGE "image"
#define ALBUM_COVER_PROP_BG_IMAGE "bg_image"
#define ALBUM_COVER_PROP_FG_IMAGE "fg_image"
#define ALBUM_COVER_PROP_ROTATION "rotation"
#define ALBUM_COVER_PROP_ISPLAY "isplay"
#define ALBUM_COVER(widget) ((album_cover_t*)(widget))


END_C_DECLS

#endif // !ALBUM_COVER_H