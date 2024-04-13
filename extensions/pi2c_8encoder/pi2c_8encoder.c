#include "encoder8.h"
#include "m_pd.h"

static t_class *pi2c_8encoder_class;

typedef struct _pi2c_8encoder {
  t_object x_obj;
  int fd;
} t_pi2c_8encoder;

void pi2c_8encoder_free(t_pi2c_8encoder *x);
void *pi2c_8encoder_new();

void pi2c_8encoder_color_hsv(t_pi2c_8encoder *x, t_floatarg n, t_floatarg h,
                             t_floatarg s, t_floatarg v) {
  if (n < 0 || n > 7) {
    post("Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  ColorHSV color = {h, s, v};
  encoder8_color_hsv(x->fd, (uint8_t)n, color);
}

void pi2c_8encoder_color_rgb(t_pi2c_8encoder *x, t_floatarg n, t_floatarg r,
                             t_floatarg g, t_floatarg b) {
  if (n < 0 || n > 7) {
    post("Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  ColorRGB color = {r, g, b};
  encoder8_color_rgb(x->fd, (uint8_t)n, color);
}

void pi2c_8encoder_rotary_set(t_pi2c_8encoder *x, t_floatarg n,
                              t_floatarg val) {
  if (n < 0 || n > 7) {
    post("Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  encoder8_rotary_set(x->fd, (uint8_t)n, (uint32_t)val);
}

void pi2c_8encoder_setup(void) {
  pi2c_8encoder_class =
      class_new(gensym("pi2c_8encoder"), (t_newmethod)pi2c_8encoder_new,
                (t_method)pi2c_8encoder_free, sizeof(t_pi2c_8encoder),
                CLASS_DEFAULT, A_DEFFLOAT, 0);

  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_color_hsv,
                  gensym("hsv"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_color_rgb,
                  gensym("rgb"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_rotary_set,
                  gensym("rotary"), A_FLOAT, A_FLOAT, 0);
}

void *pi2c_8encoder_new() {
  t_pi2c_8encoder *x = (t_pi2c_8encoder *)pd_new(pi2c_8encoder_class);
  x->fd = i2c_open(1);
  if (x->fd < 0) {
    post("Could not open i2c bus.");
    return NULL;
  }
  if (i2c_set_addr(x->fd, ENCODER8_ADDR) < 0) {
    post("Could not open the encoder.");
    return NULL;
  }
  return (void *)x;
}

void pi2c_8encoder_free(t_pi2c_8encoder *x) { i2c_close(x->fd); }
