#ifndef KNOB_H
#define KNOB_H

#include "awtk.h"

BEGIN_C_DECLS

/**
 * @class knob_t
 * @parent widget_t
 * annotation []
 * 旋钮控件
 * 支持长宽不一致的控件大小
 * 支持旋钮指针预先偏移
 * FIXME:在xml中设置step导致控件出现异常
 */
typedef struct _knob_t {
  widget_t widget;
  /**
   * @property {float_t} value
   * @annotation ["set_prop","get_prop"]
   * 旋钮当前指向的值
   */
  float_t value;
  /**
   * @property {uint32_t} max
   * @annotation ["set_prop","get_prop"]
   * 旋钮能指向的最大值
   * 默认值为100
   */
  uint32_t max;
  /**
   * @property {float_t} start_angle
   * @annotation ["set_prop","get_prop"]
   * 旋钮指针的起始角度
   */
  float_t start_angle;
  /**
   * @property {float_t} angle_range
   * @annotation ["set_prop","get_prop"]
   * 旋钮可转动范围
   */
  float_t angle_range;
  /**
   * @property {float_t} rotation
   * @annotation ["set_prop","get_prop"]
   * 旋钮指针校准 预偏移角度
   */
  float_t rotation;
  /**
   * @property {uint16_t} step
   * @annotation ["set_prop","get_prop"]
   * 旋钮步进值 默认为1
   */
  uint16_t step;
  /**
   * @property {uint32_t} line_width
   * @annotation ["set_prop","get_prop"]
   * 值进度线寛
   */
  uint32_t line_width;
  /**
   * @property {bool_t} pressed
   * @annotation []
   * 鼠标按下标志
   */
  bool_t pressed;
  /**
   * @property {int32_t} move_x
   * @annotation []
   * 记录鼠标指针X轴的偏移位置
   */
  int32_t move_x;
  /**
   * @property {bool_t} direction
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 记录当前按下鼠标的方位。 [0:左 1:右]
   */
  bool_t direction;
  /**
   * @property {char*} image
   * @annotation ["set_prop","get_prop"]
   * 旋钮图片
   */
  char* image;
} knob_t;

widget_t* knob_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* knob_cast(widget_t* widget);
ret_t knob_set_value(widget_t* widget, float_t value);
ret_t knob_set_max(widget_t* widget, uint32_t max);
ret_t knob_set_line_width(widget_t* widget, uint32_t line_width);
ret_t knob_set_start_angle(widget_t* widget, int32_t start_angle);
ret_t knob_set_angle_range(widget_t* widget, float_t angle_range);
ret_t knob_set_image(widget_t* widget, const char* name);
ret_t knob_set_step(widget_t* widget, uint16_t step);

#define KNOB_PROP_LINE_WIDTH "line_width"
#define KNOB_PROP_START_ANGLE "start_angle"
#define KNOB_PROP_ANGLE_RANGE "angle_range"
#define KNOB_PROP_ROTATION "rotation"
#define KNOB_PROP_IMAGE "image"

#define WIDGET_TYPE_KNOB "knob"
#define KNOB(widget) ((knob_t*)(knob_cast(WIDGET(widget))))

TK_EXTERN_VTABLE(knob);

END_C_DECLS

#endif