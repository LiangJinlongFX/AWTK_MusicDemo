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
    image_base_t image_base;

    image_draw_type_t draw_type;
    /*
     * 背景图片 
     */
    char* bg_image;
    /*
     * 封面图片
     */
    //char* image;
}album_cover_t;

/**
 * @method album_cover_create
 * 创建album对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* album_cover_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* album_cover_cast(widget_t* widget);
ret_t album_cover_set_image(widget_t* widget, const char* name);
ret_t album_cover_set_draw_type(widget_t* widget, image_draw_type_t draw_type);
ret_t album_cover_set_angle(widget_t* widget, int32_t angle);

#define WIDGET_TYPE_ALBUM_COVER "album_cover"
#define ALBUM_COVER_PROP_ANGLE "angle"
#define ALBUM_COVER_PROP_IMAGE "image"
#define ALBUM_COVER_PROP_BG_IMAGE "bg_image"
#define ALBUM_COVER(widget) ((album_cover_t*)(widget))

END_C_DECLS

#endif // !ALBUM_COVER_H