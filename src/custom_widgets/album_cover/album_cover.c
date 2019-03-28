
#include "awtk.h"
#include "album_cover.h"



/**
 * @method album_cover_on_paint_self
 * 绘制 album_cover 对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
static ret_t album_cover_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;

  album_cover_t* album_cover = ALBUM_COVER(widget);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);


  if (album_cover->image == NULL || album_cover->fg_image == NULL || album_cover->bg_image == NULL) {
    return RET_OK;
  }

  /* 绘制唱盘 */
  vgcanvas_save(vg);
  album_cover_transform(widget,c);
  if (widget_load_image(widget, album_cover->image, &bitmap) == RET_OK) {
    rect_t dst = rect_init(widget->w*0.15, widget->w*0.15, widget->w*0.7, widget->h*0.7);
    canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_SCALE_AUTO, &dst);
  }

  if (widget_load_image(widget, album_cover->bg_image, &bitmap) == RET_OK) {
    rect_t dst = rect_init(0, 0, widget->w, widget->h);
    canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_SCALE_AUTO, &dst);
  }
  vgcanvas_restore(vg);

  /* 绘制唱头 */
  vgcanvas_save(vg);
  cartridge_transform(widget,c);
  if (widget_load_image(widget, album_cover->fg_image, &bitmap) == RET_OK) {
    rect_t dst = rect_init(0, 0, widget->w, widget->h);
    canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_DEFAULT, &dst);
  }
  vgcanvas_restore(vg);

  return RET_OK;
}

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
  vgcanvas_rotate(vg, album_cover->rotation);
  vgcanvas_translate(vg, -anchor_x, -anchor_y);
  vgcanvas_translate(vg, -c->ox, -c->oy);

  return RET_OK; 
}

ret_t cartridge_transform(widget_t* widget, canvas_t* c)
{
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  album_cover_t* album_cover = ALBUM_COVER(widget);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  anchor_x = 0.5 * widget->w;
  anchor_y = 0.5 * widget->h;

  vgcanvas_translate(vg, c->ox, c->oy);
  if(album_cover->is_play == TRUE)
    vgcanvas_rotate(vg, 0);
  else
    vgcanvas_rotate(vg, -45);
  vgcanvas_translate(vg, -c->ox, -c->oy);

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
ret_t album_cover_set_image(widget_t* widget, const char* name) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  album_cover->image = tk_str_copy(album_cover->image, name);

  return RET_OK;
}

/*
 * 设置控件图片
 */
ret_t album_cover_set_bg_image(widget_t* widget, const char* name) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  album_cover->bg_image = tk_str_copy(album_cover->bg_image, name);

  return RET_OK;
}

ret_t album_cover_start(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  album_cover_t* album_cover = ALBUM_COVER(widget);
  album_cover->is_play = TRUE;

  return RET_OK;
}

ret_t album_cover_pause(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  album_cover_t* album_cover = ALBUM_COVER(widget);
  album_cover->is_play = FALSE;

  return RET_OK;
}

/*
 * 设置控件图片
 */
ret_t album_cover_set_fg_image(widget_t* widget, const char* name) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  album_cover->fg_image = tk_str_copy(album_cover->fg_image, name);

  return RET_OK;
}

static ret_t timer_album_cover(const timer_info_t* timer)
{
  widget_t* widget = WIDGET(timer->ctx);
  album_cover_t* album_cover = ALBUM_COVER(widget);
  if(album_cover->is_play == TRUE) {
    album_cover->rotation += 0.05;
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
  album_cover->timer_id = timer_add(timer_album_cover, widget, 100);
  album_cover->is_play = FALSE;
  album_cover->rotation = 0;

  return widget;
}

/* 
 * 脚本翻译函数 
 */
widget_t* album_cover_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_album_cover_vtable, NULL);

  return widget;
}