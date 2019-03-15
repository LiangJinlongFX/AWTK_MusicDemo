
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
  float_t rotation = 0;
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  matrix_t matrix;

  album_cover_t* album_cover = ALBUM_COVER(widget);
  image_base_t* image_base = IMAGE_BASE(widget);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  rect_t dst = rect_init(0, 0, widget->w, widget->h);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  
  /* 绘制专辑封面 */
  /*if (widget_load_image(widget, album_cover->image, &bitmap) == RET_OK) {
    //canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_CENTER, &dst);
    float_t dx = (dst.w - bitmap.w) / 2;
    float_t dy = dst.h / 2 + bitmap.w / 2 - bitmap.h;
    anchor_x = bitmap.w / 2;
    anchor_y = bitmap.h - bitmap.w / 2;
    matrix_identity(m);
    matrix_translate(m, c->ox + dx, c->oy + dy);
    matrix_translate(m, anchor_x, anchor_y);
    matrix_rotate(m, 60);
    matrix_translate(m, -anchor_x, -anchor_y);

    canvas_draw_image_matrix(c, &bitmap, &matrix);
  }*/

  if (image_base->image == NULL) {
    return RET_OK;
  }

  return_value_if_fail(widget_load_image(widget, image_base->image, &bitmap) == RET_OK,
                       RET_BAD_PARAMS);

  canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_CENTER, &dst);

  /*
  if (vg != NULL) {
    if (image_need_transform(widget)) {
      vgcanvas_save(vg);
      image_transform(widget, c);
      vgcanvas_draw_icon(vg, &bitmap, 0, 0, bitmap.w, bitmap.h, 0, 0, widget->w, widget->h);
      vgcanvas_restore(vg);

      return RET_OK;
    }
  }

  if (bitmap.data != NULL) {
    rect_t dst = rect_init(0, 0, widget->w, widget->h);
    canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_CENTER, &dst);
  }
  */

  widget_paint_helper(widget, c, NULL, NULL);

  /* 绘制背景图片 */
  /*
  if (widget_load_image(widget, album_cover->bg_image, &bitmap) == RET_OK) {
    canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_CENTER, &dst);
  }*/

  return RET_OK;
}

/*
 * 获取控件属性
 */
static ret_t album_cover_get_prop(widget_t* widget, const char* name, value_t* v) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, ALBUM_COVER_PROP_IMAGE)) {
    image_base_get_prop(widget, name, v);
    return RET_OK;
  } else if (tk_str_eq(name, ALBUM_COVER_PROP_BG_IMAGE)) {
    value_set_str(v, album_cover->bg_image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DRAW_TYPE)) {
    value_set_int(v, album_cover->draw_type);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t album_cover_set_prop(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return album_cover_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_DRAW_TYPE)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = image_draw_type_find(value_str(v));
      if (kv != NULL) {
        return album_cover_set_draw_type(widget, (image_draw_type_t)(kv->value));
      }
    } else {
      return album_cover_set_draw_type(widget, (image_draw_type_t)value_int(v));
    }

    return RET_OK;
  }

  return RET_NOT_FOUND;
}



static ret_t album_cover_on_destroy(widget_t* widget) {
  album_cover_t* album_cover = ALBUM_COVER(widget);

  //TKMEM_FREE(album_cover->image);
  TKMEM_FREE(album_cover->bg_image);

  return RET_OK;
}

ret_t album_cover_set_image(widget_t* widget, const char* name) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  //album_cover->image = tk_str_copy(album_cover->image, name);

  return RET_OK;
}

ret_t album_cover_set_draw_type(widget_t* widget, image_draw_type_t draw_type) {
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  album_cover->draw_type = draw_type;

  return RET_OK;
}


/*
 * 控件xml属性
 * 图片名称 图片绘制类型
 * 是否可点击 
 */
static const char* s_album_cover_clone_properties[] = {WIDGET_PROP_IMAGE,      WIDGET_PROP_DRAW_TYPE,
                                                 WIDGET_PROP_SCALE_X,    WIDGET_PROP_SCALE_Y,
                                                 WIDGET_PROP_ANCHOR_X,   WIDGET_PROP_ANCHOR_Y,
                                                 WIDGET_PROP_ROTATION,   WIDGET_PROP_CLICKABLE,
                                                 WIDGET_PROP_SELECTABLE, NULL};

/*
 * 设置控件基本属性
 */
static const widget_vtable_t s_album_cover_vtable = {.size = sizeof(album_cover_t),       //大小
                                                   .type = WIDGET_TYPE_ALBUM_COVER,    //xml类型名称
                                                   .enable_pool = TRUE,
                                                   .clone_properties = s_album_cover_clone_properties, //克隆xml属性
                                                   .parent = TK_PARENT_VTABLE(widget),
                                                   .on_event = image_base_on_event,
                                                   .create = album_cover_create,         //创建函数
                                                   .on_destroy = album_cover_on_destroy,
                                                   .on_paint_self = album_cover_on_paint_self,    
                                                   .get_prop = album_cover_get_prop,
                                                   .set_prop = album_cover_set_prop};

/* 创建函数 */
widget_t* album_cover_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  // 创建控件模版
  widget_t* widget = widget_create(parent, &s_album_cover_vtable, x, y, w, h);
  album_cover_t* album_cover = ALBUM_COVER(widget);
  return_value_if_fail(album_cover != NULL, NULL);

  image_base_init(widget);
  album_cover->draw_type = IMAGE_DRAW_CENTER;

  return widget;
}

/* 脚本翻译函数 */
widget_t* album_cover_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_album_cover_vtable, NULL);

  return widget;
}