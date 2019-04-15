#ifndef ALBUM_COVER_H
#define ALBUM_COVER_H

#include "awtk.h"

BEGIN_C_DECLS

/**
 * @class ablum_cover_t
 * @parent widget_t
 * @annotation []
 * 专辑封面显示控件。
 * 控件分为播放状态和旋转状态
 * 依据唱盘风格进行代码实现
 */
typedef struct _album_cover_t {
    widget_t widget;
    /**
     * @property {char*} fg_image
     * @annotation ["set_prop","get_prop"]
     * 前景图片，在最顶层显示，如唱头等
     */
    char* fg_image;
    /**
     * @property {char*} bg_image
     * @annotation ["set_prop","get_prop"]
     * 背景图片，在第二层显示，如唱盘等，用于装饰专辑封面
     */
    char* bg_image;
    /**
     * @property {char*} image
     * @annotation ["set_prop","get_prop"]
     * 专辑封面图片，在最底层显示，可切换更改
     */
    char* image;
    /**
     * @property {float_t} rotation
     * @annotation ["set_prop","get_prop"]
     * 控件旋转角度，单位为角度
     */
    float_t rotation;
    /**
     * @property {bool_t} is_play
     * @annotation ["set_prop","get_prop"]
     * 播放状态位，标记当前控件是否为活动状态
     */
    bool_t is_play;
    /**
     * @property {int32_t} timer_id
     * @annotation []
     * 控件定时器id，用于定时旋转控件
     */
    int32_t timer_id;
    /**
     * @property {}
     * @annotation ["set_prop","get_prop"]
     * 控件内部的image对象管理器
     * FIXME:本管理器暂时遇到内存异常问题，控件的iamge暂由全局的image_manager管理
     */
    image_manager_t* cover_image_manager;
}album_cover_t;

/**
 * @method album_cover_create
 * @annotation []
 * 创建专辑控件
 * @param {}
 *
 * @return {widget_t*}
 */
widget_t* album_cover_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method album_cover_cast
 * @annotation []
 * 专辑控件销毁
 * @param {}
 *
 * @return {widget_t*}
 */
widget_t* album_cover_cast(widget_t* widget);

/**
 * @method album_cover_set_image
 * @annotation []
 * 设置专辑封面图片
 * @param {}
 *
 * @return {ret_t}
 */
ret_t album_cover_set_image(widget_t* widget, const char* name);

/**
 * @method album_cover_set_bg_image
 * @annotation []
 * 设置控件背景图片
 * @param {}
 *
 * @return {ret_t}
 */
ret_t album_cover_set_bg_image(widget_t* widget, const char* name);

/**
 * @method album_cover_set_fg_image
 * @annotation []
 * 设置控件前景图片
 * @param {}
 * 
 * @return {ret_t}
 */
ret_t album_cover_set_fg_image(widget_t* widget, const char* name);

/**
 * @method album_cover_start
 * @annotation []
 * 控件开始播放旋转
 * @param {}
 *
 * @return {ret_t}
 */
ret_t album_cover_start(widget_t* widget);

/**
 * @method album_cover_pause
 * @annotation []
 * 控件暂停播放旋转
 * @param {}
 *
 * @return {ret_t}
 */
ret_t album_cover_pause(widget_t* widget);
ret_t album_cover_transform(widget_t* widget, canvas_t* c);

/* private */
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