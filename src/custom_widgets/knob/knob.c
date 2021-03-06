#include "awtk.h"
#include "knob.h"

/*
 * 图标画布旋转
 */
static ret_t knob_transform(widget_t* widget, canvas_t* c, float_t rotation_offset) {
  bitmap_t bitmap;
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  knob_t* knob = KNOB(widget);
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  anchor_x = 0.5 * widget->w;
  anchor_y = 0.5 * widget->h;

  float_t rotation = TK_D2R(knob->rotation) + rotation_offset;
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_translate(vg, anchor_x, anchor_y);
  vgcanvas_rotate(vg, rotation);
  vgcanvas_translate(vg, -anchor_x, -anchor_y);

  return RET_OK;
}

/*
 * 控件绘制函数
 */
static ret_t knob_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  knob_t* knob = KNOB(widget);
  color_t color = style_get_color(style, STYLE_ID_FG_COLOR, trans);
  color_t mask_color = style_get_color(style, STYLE_ID_MASK_COLOR, trans);

  if (knob->image == NULL) return RET_OK;

  // TODO:默认控件长宽相等且图片素材适合控件比例,没有添加比例校验
  wh_t w = tk_min(widget->w, widget->h);

  /* 确保value在有效范围内 */
  if (knob->value >= knob->max) {
    knob->value = knob->max;
  } else if (knob->value < 0) {
    knob->value = 0;
  }

  if (vg != NULL) {
    xy_t cx = widget->w / 2;
    xy_t cy = widget->h / 2;
    float_t r = tk_min(cx, cy) - knob->line_width;
    float_t start_angle = TK_D2R(knob->start_angle);
    float_t end_angle = 0;    //用于前景
    float_t end_angle_1 = 0;  //用于背景

    float_t angle = TK_D2R(knob->angle_range * (knob->value / knob->max));
    float_t all_angle = TK_D2R(knob->angle_range);
    end_angle = start_angle + angle;
    end_angle_1 = start_angle + all_angle;

    /* 绘制增量圆弧 */
    vgcanvas_save(vg);
    vgcanvas_translate(vg, c->ox, c->oy);
    vgcanvas_set_stroke_color(vg, mask_color);
    vgcanvas_set_line_width(vg, knob->line_width);
    vgcanvas_set_line_cap(vg, "round");
    vgcanvas_begin_path(vg);
    //绘制底层圆弧
    vgcanvas_arc(vg, cx, cy, r, start_angle, end_angle_1, FALSE);
    vgcanvas_stroke(vg);
    if (end_angle > start_angle) {
      //绘制前景
      vgcanvas_set_stroke_color(vg, color);
      vgcanvas_begin_path(vg);
      vgcanvas_arc(vg, cx, cy, r, start_angle, end_angle, FALSE);
      vgcanvas_stroke(vg);
    }
    vgcanvas_restore(vg);

    /* 绘制旋钮图标 */
    vgcanvas_save(vg);
    knob_transform(widget, c, angle);
    if (widget_load_image(widget, knob->image, &bitmap) == RET_OK) {
      rect_t dst;
      dst.x = widget->w / 2 - 0.8 * r;
      dst.y = widget->h / 2 - 0.8 * r;
      dst.w = 1.6 * r;
      dst.h = 1.6 * r;
      // vgcanvas_draw_icon(vg, &bitmap, 0, 0, bitmap.w, bitmap.h, dst.x, dst.y, dst.w, dst.h);
      vgcanvas_draw_image(vg, &bitmap, 0, 0, bitmap.w, bitmap.h, dst.x, dst.y, dst.w, dst.h);
    }
    vgcanvas_restore(vg);
  }

  return RET_OK;
}

/*
 * 按钮事件处理函数
 */
static ret_t knob_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  knob_t* knob = KNOB(widget);

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (widget_is_point_in(widget, evt.x, evt.y, FALSE)) {
        knob->pressed = TRUE;
        knob->move_x = evt.x;
      }
    } break;
    case EVT_POINTER_UP: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (knob->pressed == TRUE) {
        knob->pressed = FALSE;
        /* 激活按键点击事件 */
        evt.e = event_init(EVT_VALUE_CHANGED, widget);
        widget_dispatch(widget, (event_t*)&evt);
        /* 清除事件状态并请求重绘控件 */
        widget_ungrab(widget->parent, widget);
        widget_set_state(widget, WIDGET_STATE_NORMAL);
        widget_invalidate_force(widget, NULL);
      }
    } break;
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      point_t p = {evt->x, evt->y};
      if (knob->pressed == TRUE) {
        if (p.x < knob->move_x) {
          knob->move_x = p.x;
          knob->value -= knob->step;
          knob->direction = FALSE;
        } else if (p.x > knob->move_x) {
          knob->move_x = p.x;
          knob->value += knob->step;
          knob->direction = TRUE;
        }
        //请求分发值改变中事件
        event_t event = event_init(EVT_VALUE_CHANGING, widget);
        widget_dispatch(widget, (event_t*)&event);
        /* 清除事件状态并请求重绘控件 */
        widget_ungrab(widget->parent, widget);
        widget_set_state(widget, WIDGET_STATE_NORMAL);
        widget_invalidate_force(widget, NULL);
      }
    }
    default:
      break;
  }

  return RET_OK;
}

/*
 * 设置控件图片
 */
ret_t knob_set_image(widget_t* widget, const char* name) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  knob->image = tk_str_copy(knob->image, name);

  return RET_OK;
}

/*
 * 设置控件数值
 */
ret_t knob_set_value(widget_t* widget, float_t value) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (knob->value != value) {
    knob->value = value;
    /* 激活按钮数值改变完成事件 */
    event_t e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_ungrab(widget->parent, widget);
    widget_set_state(widget, WIDGET_STATE_NORMAL);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t knob_set_max(widget_t* widget, uint32_t max) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  knob->max = max;

  return widget_invalidate(widget, NULL);
}

ret_t knob_set_line_width(widget_t* widget, int32_t line_width) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  knob->line_width = line_width;

  return widget_invalidate(widget, NULL);
}

ret_t knob_set_start_angle(widget_t* widget, int32_t start_angle) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  knob->start_angle = start_angle;

  return widget_invalidate(widget, NULL);
}

ret_t knob_set_angle_range(widget_t* widget, float_t angle_range) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  knob->angle_range = angle_range;

  return widget_invalidate(widget, NULL);
}

ret_t knob_set_step(widget_t* widget, uint32_t step) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  knob->step = step;

  return widget_invalidate(widget, NULL);
}

static ret_t knob_set_rotation(widget_t* widget, float_t rotation) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  knob->rotation = rotation;

  return widget_invalidate(widget, NULL);
}

static ret_t knob_on_destroy(widget_t* widget) {
  knob_t* knob = KNOB(widget);

  TKMEM_FREE(knob->image);

  return RET_OK;
}

static ret_t knob_get_prop(widget_t* widget, const char* name, value_t* v) {
  knob_t* knob = KNOB(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_float(v, knob->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_uint32(v, knob->max);
    return RET_OK;
  } else if (tk_str_eq(name, KNOB_PROP_LINE_WIDTH)) {
    value_set_uint32(v, knob->line_width);
    return RET_OK;
  } else if (tk_str_eq(name, KNOB_PROP_START_ANGLE)) {
    value_set_int(v, knob->start_angle);
    return RET_OK;
  } else if (tk_str_eq(name, KNOB_PROP_ANGLE_RANGE)) {
    value_set_float(v, knob->angle_range);
    return RET_OK;
  } else if (tk_str_eq(name, KNOB_PROP_IMAGE)) {
    value_set_str(v, knob->image);
    return RET_OK;
  } else if (tk_str_eq(name, KNOB_PROP_ROTATION)) {
    value_set_float(v, knob->rotation);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    value_set_int(v, knob->step);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t knob_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return knob_set_value(widget, value_float(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    return knob_set_max(widget, value_int(v));
  } else if (tk_str_eq(name, KNOB_PROP_LINE_WIDTH)) {
    return knob_set_line_width(widget, value_int(v));
  } else if (tk_str_eq(name, KNOB_PROP_START_ANGLE)) {
    return knob_set_start_angle(widget, value_int(v));
  } else if (tk_str_eq(name, KNOB_PROP_ANGLE_RANGE)) {
    return knob_set_angle_range(widget, value_float(v));
  } else if (tk_str_eq(name, KNOB_PROP_IMAGE)) {
    return knob_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, KNOB_PROP_ROTATION)) {
    return knob_set_rotation(widget, value_float(v));
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    return knob_set_step(widget, value_int(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_knob_clone_properties[] = {
    WIDGET_PROP_VALUE,    WIDGET_PROP_MAX,       WIDGET_PROP_STEP,
    KNOB_PROP_LINE_WIDTH, KNOB_PROP_START_ANGLE, KNOB_PROP_ANGLE_RANGE,
    KNOB_PROP_IMAGE,      KNOB_PROP_ROTATION,    NULL};

TK_DECL_VTABLE(knob) = {.size = sizeof(knob_t),
                        .type = WIDGET_TYPE_KNOB,
                        .clone_properties = s_knob_clone_properties,
                        .parent = TK_PARENT_VTABLE(widget),
                        .create = knob_create,
                        .on_event = knob_on_event,
                        .on_paint_self = knob_on_paint_self,
                        .on_destroy = knob_on_destroy,
                        .get_prop = knob_get_prop,
                        .set_prop = knob_set_prop};

widget_t* knob_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(knob), x, y, w, h);
  knob_t* knob = KNOB(widget);
  return_value_if_fail(knob != NULL, NULL);

  knob->max = 100;
  knob->line_width = 5;
  knob->start_angle = 0;
  knob->angle_range = 180;
  knob->pressed = FALSE;
  knob->rotation = 0;
  knob->step = 5;

  return widget;
}

widget_t* knob_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, knob), NULL);

  return widget;
}