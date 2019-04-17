
#include "awtk.h"
#include "album_cover.h"

/*
 * 绘制控件自身
 * 注意图片的缩放显示问题
 * 前景图片使用实际比例
 * 背景图片缩放到控件大小的0.8
 * 封面图片缩放到控件大小的0.5
 */
static ret_t album_cover_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  album_cover_t* album_cover = ALBUM_COVER(widget);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);


  if (album_cover->image == NULL || album_cover->fg_image == NULL || album_cover->bg_image == NULL) {
    return RET_OK;
  }

  //TODO:默认控件长宽相等且图片素材适合控件比例,没有添加比例校验
  wh_t w = tk_min(widget->w, widget->h);

  /* 绘制唱盘 */
  vgcanvas_save(vg);

  album_cover_transform(widget,c);
  if (widget_load_image(widget, album_cover->image, &bitmap) == RET_OK) {
    rect_t dst;
    dst.x = widget->w*0.25;
    dst.y = widget->h*0.25;
    dst.w = widget->w*0.5;
    dst.h = widget->h*0.5;
    vgcanvas_draw_image(vg, &bitmap, 0, 0, bitmap.w, bitmap.h, dst.x, dst.y, dst.w, dst.h);
  }

  if (widget_load_image(widget, album_cover->bg_image, &bitmap) == RET_OK) {
    rect_t dst = rect_init(widget->w*0.1, widget->h*0.1, widget->w*0.8, widget->h*0.8);
    vgcanvas_draw_image(vg, &bitmap, 0, 0, bitmap.w, bitmap.h, dst.x, dst.y, dst.w, dst.h);
  }
  vgcanvas_restore(vg);

  /* 绘制唱头 */
  vgcanvas_save(vg);
  cartridge_transform(widget,c);
  if (widget_load_image(widget, album_cover->fg_image, &bitmap) == RET_OK) {
    rect_t dst = rect_init(w/2, 0, bitmap.w, bitmap.h);
    vgcanvas_draw_image(vg, &bitmap, 0, 0, bitmap.w, bitmap.h, dst.x, dst.y, dst.w, dst.h);
  }
  vgcanvas_restore(vg);

  return RET_OK;
}

/*
 * 图片旋转(image&bg_image)
 */
ret_t album_cover_transform(widget_t* widget, canvas_t* c)
{
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  album_cover_t* album_cover = ALBUM_COVER(widget);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  anchor_x = 0.5 * widget->w;
  anchor_y = 0.5 * widget->h;

  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_translate(vg, anchor_x, anchor_y);
  vgcanvas_rotate(vg, TK_D2R(album_cover->rotation));
  vgcanvas_translate(vg, -anchor_x, -anchor_y);

  return RET_OK; 
}

/*
 * 图片旋转(fg_image)
 * 45
 */
ret_t cartridge_transform(widget_t* widget, canvas_t* c)
{
  float_t anchor_x = 0;
  album_cover_t* album_cover = ALBUM_COVER(widget);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  anchor_x = 0.5 * widget->w;

  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_translate(vg, anchor_x, 0);
  // if(album_cover->is_play == TRUE)
  //     vgcanvas_rotate(vg, TK_D2R(-3));
  // else
  //   vgcanvas_rotate(vg, TK_D2R(-60));
  vgcanvas_rotate(vg, TK_D2R(album_cover->cartridge_rotation));
  vgcanvas_translate(vg, -anchor_x, 0);

  return RET_OK; 
}

/*
 * 获取控件属性
 */
static ret_t album_cover_get_prop(widget_t* widget, const char* name, value_t* v) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, ALBUM_COVER_PROP_IMAGE)) {
    value_set_str(v, album_cover->image);
    return RET_OK;
  } else if (tk_str_eq(name, ALBUM_COVER_PROP_BG_IMAGE)) {
    value_set_str(v, album_cover->bg_image);
    return RET_OK;
  } else if (tk_str_eq(name, ALBUM_COVER_PROP_FG_IMAGE)) {
    value_set_str(v, album_cover->fg_image);
    return RET_OK;
  } else if(tk_str_eq(name, ALBUM_COVER_PROP_ISPLAY)) {
    value_set_bool(v, album_cover->is_play);
    return RET_OK;
  } else if(tk_str_eq(name, ALBUM_COVER_PROP_ROTATION)) {
    value_set_int(v, album_cover->rotation);
    return RET_OK;
  } else if(tk_str_eq(name, ALBUM_COVER_PROP_CARTRIDGE_ROTATION)) {
    value_set_int(v, album_cover->cartridge_rotation);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

/*
 * 属性设置函数
 */
static ret_t album_cover_set_prop(widget_t* widget, const char* name, value_t* v) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, ALBUM_COVER_PROP_IMAGE)) {
    return album_cover_set_image(widget, value_str(v));
  } else if(tk_str_eq(name, ALBUM_COVER_PROP_ROTATION)) {
    album_cover->rotation = value_float(v);
    return RET_OK;
  } else if(tk_str_eq(name, ALBUM_COVER_PROP_BG_IMAGE)) {
    return album_cover_set_bg_image(widget, value_str(v));
  } else if(tk_str_eq(name, ALBUM_COVER_PROP_FG_IMAGE)) {
    return album_cover_set_fg_image(widget, value_str(v));
  } else if(tk_str_eq(name, ALBUM_COVER_PROP_ISPLAY)) {
    album_cover->is_play = value_bool(v);
    return RET_OK;
  } else if(tk_str_eq(name, ALBUM_COVER_PROP_CARTRIDGE_ROTATION)) {
    album_cover->cartridge_rotation = value_float(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}


/*
 * 销毁回调函数
 */
static ret_t album_cover_on_destroy(widget_t* widget) {
  album_cover_t* album_cover = ALBUM_COVER(widget);

  timer_remove(album_cover->timer_id);
  TKMEM_FREE(album_cover->image);
  TKMEM_FREE(album_cover->bg_image);
  TKMEM_FREE(album_cover->fg_image);

  return RET_OK;
}

/*
 * 设置控件图片
 */
//FIXME:需修复切换图片后内存增大胡问题
//TODO: 切换图片时清除长时间没用的图片缓存
ret_t album_cover_set_image(widget_t* widget, const char* name) {
  bitmap_t bitmap;
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  album_cover->prev_image = album_cover->image;
  album_cover->image = tk_str_copy(album_cover->image, name);

  return widget_invalidate(widget, NULL);

  return RET_OK;
}

/*
 * 设置控件背景图片
 */
ret_t album_cover_set_bg_image(widget_t* widget, const char* name) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  album_cover->bg_image = tk_str_copy(album_cover->bg_image, name);

  return RET_OK;
}

/*
 * 播放动画
 */
ret_t album_cover_start(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  album_cover_t* album_cover = ALBUM_COVER(widget);
  widget_create_animator(album_cover, "cartridge_rotation(to=-3, duration=500, repeat_times=1, yoyo_times=1, easing=sin_out)");
  album_cover->is_play = TRUE;

  return RET_OK;
}

/*
 * 暂停动画
 */
ret_t album_cover_pause(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  album_cover_t* album_cover = ALBUM_COVER(widget);
  widget_create_animator(album_cover, "cartridge_rotation(to=-45, duration=500, repeat_times=1, yoyo_times=1, easing=sin_out)");
  album_cover->is_play = FALSE;

  return RET_OK;
}

/*
 * 设置控件前景图片
 */
ret_t album_cover_set_fg_image(widget_t* widget, const char* name) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  album_cover->fg_image = tk_str_copy(album_cover->fg_image, name);

  return RET_OK;
}

/*
 * 控件定时器回调函数
 * TODO:通过协调旋转步进值和定时间隔来调整动画效果
 */
static ret_t timer_album_cover(const timer_info_t* timer)
{
  widget_t* widget = WIDGET(timer->ctx);
  album_cover_t* album_cover = ALBUM_COVER(widget);
  if(album_cover->is_play == TRUE) {
    album_cover->rotation += 0.5;
    if(album_cover->rotation >360)
      album_cover->rotation = 0;
    widget_invalidate(widget, NULL);
  }
  return RET_REPEAT;
}

/*
 * 控件xml属性
 * 图片名称 图片绘制类型
 * 是否可点击 
 */
static const char* s_album_cover_clone_properties[] = {ALBUM_COVER_PROP_IMAGE, ALBUM_COVER_PROP_BG_IMAGE,
                                                      ALBUM_COVER_PROP_FG_IMAGE, ALBUM_COVER_PROP_ROTATION,
                                                      ALBUM_COVER_PROP_ISPLAY, NULL};

/*
 * 设置控件基本属性
 */
static const widget_vtable_t s_album_cover_vtable = {.size = sizeof(album_cover_t),       //大小
                                                   .type = WIDGET_TYPE_ALBUM_COVER,       //xml类型名称
                                                   .clone_properties = s_album_cover_clone_properties, //克隆属性
                                                   .persistent_properties = s_album_cover_clone_properties, //持久化属性
                                                   .parent = TK_PARENT_VTABLE(widget),
                                                   .create = album_cover_create,         //创建函数
                                                   .on_destroy = image_base_on_destroy,  //销毁函数
                                                   .on_paint_self = album_cover_on_paint_self, //绘制函数   
                                                   .get_prop = album_cover_get_prop,     //获取属性
                                                   .set_prop = album_cover_set_prop};    //设置属性

/* 创建函数 */
widget_t* album_cover_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  // 创建控件模版
  album_cover_t* album_cover = TKMEM_ZALLOC(album_cover_t);
  widget_t* widget = WIDGET(album_cover);
  return_value_if_fail(album_cover != NULL, NULL);

  widget_init(widget, parent, &s_album_cover_vtable, x, y, w, h);

  album_cover->timer_id = timer_add(timer_album_cover, widget, 40);
  album_cover->is_play = FALSE;
  album_cover->rotation = 0;
  album_cover->cartridge_rotation = -60;

  return widget;
}

/* 
 * 脚本翻译函数 
 */
widget_t* album_cover_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_album_cover_vtable, NULL);

  return widget;
}