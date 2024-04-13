#include "encoder4.h"
#include "m_pd.h"

static t_class *pi2c_4encoder_class;

typedef struct _pi2c_4encoder {
  t_object x_obj;
  int fd;
  ColorRGB colors[4];
} t_pi2c_4encoder;

void pi2c_4encoder_rgb(t_pi2c_4encoder *x, t_floatarg n, t_floatarg r,
                       t_floatarg g, t_floatarg b) {
  if (n < 0 || n > 3) {
    post("Invalid encoder ID. Must be between 0 and 3.");
    return;
  }
  x->colors[(int)n].r = (unsigned char)r;
  x->colors[(int)n].g = (unsigned char)g;
  x->colors[(int)n].b = (unsigned char)b;

  encoder4_colors_rgb(x->fd, x->colors);
}

void pi2c_4encoder_hsv(t_pi2c_4encoder *x, t_floatarg n, t_floatarg h,
                       t_floatarg s, t_floatarg v) {
  if (n < 0 || n > 3) {
    post("Invalid encoder ID. Must be between 0 and 3.");
    return;
  }

  ColorHSV hsv = {.h = (float)h, .s = (float)s, .v = (float)v};
  ColorRGB color = hsv_to_rgb(hsv);
  x->colors[(int)n].r = color.r;
  x->colors[(int)n].g = color.g;
  x->colors[(int)n].b = color.b;
  encoder4_colors_rgb(x->fd, x->colors);
}

void pi2c_4encoder_rotary(t_pi2c_4encoder *x, t_floatarg n, t_floatarg val) {
  if (n < 0 || n > 3) {
    post("Invalid encoder ID. Must be between 0 and 3.");
    return;
  }
  encoder4_rotary_set(x->fd, (uint8_t)n, (int32_t)val);
}

void *pi2c_4encoder_new() {
  t_pi2c_4encoder *x = (t_pi2c_4encoder *)pd_new(pi2c_4encoder_class);
  x->fd = i2c_open(1);
  return (void *)x;
}

void pi2c_4encoder_setup(void) {
  pi2c_4encoder_class =
      class_new(gensym("pi2c_4encoder"), (t_newmethod)pi2c_4encoder_new, 0,
                sizeof(t_pi2c_4encoder), CLASS_DEFAULT, 0);

  class_addmethod(pi2c_4encoder_class, (t_method)pi2c_4encoder_rgb,
                  gensym("rgb"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_4encoder_class, (t_method)pi2c_4encoder_hsv,
                  gensym("hsv"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_4encoder_class, (t_method)pi2c_4encoder_rotary,
                  gensym("rotary"), A_FLOAT, A_FLOAT, 0);
}
