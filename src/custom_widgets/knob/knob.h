#ifndef KNOB_H
#define KNOB_H

#include "awtk.h"

BEGIN_C_DECLS

typedef struct _knob_t {
    widget_t widget;
    float_t value;
    uint32_t max;
    int32_t start_angle;
    /**
     * @property {float_t} angle_range
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 是否按下鼠标。
     */
    float_t angle_range;
    float_t rotation;
    /**
     * @property {bool_t} pressed
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 是否按下鼠标。
     */
    uint32_t line_width;
    /**
     * @property {bool_t} pressed
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 是否按下鼠标。
     */
    bool_t pressed;
    /**
     * @property {bool_t} direction
     * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
     * 记录当前按下鼠标的方位。 [0:左 1:右]
     */
    bool_t direction;
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

#define KNOB_PROP_LINE_WIDTH "line_width"
#define KNOB_PROP_START_ANGLE "start_angle"
#define KNOB_PROP_ANGLE_RANGE "angle_range"
#define KNOB_PROP_ROTATION    "rotation"
#define KNOB_PROP_IMAGE "image"

#define WIDGET_TYPE_KNOB "knob"
#define KNOB(widget) ((knob_t*)(knob_cast(WIDGET(widget))))

TK_EXTERN_VTABLE(knob);

END_C_DECLS

#endif