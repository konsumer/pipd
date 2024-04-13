#include "encoder8.h"
#include "m_pd.h"

static t_class *pi2c_8encoder_class;

typedef struct _pi2c_8encoder {
  t_object x_obj;
  int fd; // File descriptor for I2C communication
          // You might add more internal states as needed
} t_pi2c_8encoder;

// Function prototypes
void pi2c_8encoder_free(t_pi2c_8encoder *x);
void *pi2c_8encoder_new(t_floatarg f);

void pi2c_8encoder_color_hsv(t_pi2c_8encoder *x, t_floatarg index, t_floatarg h,
                             t_floatarg s, t_floatarg v) {
  if (index < 0 || index > 7) {
    post("Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  ColorHSV color = {h, s, v};
  encoder8_color_hsv(x->fd, (uint8_t)index, color);
}

void pi2c_8encoder_color_rgb(t_pi2c_8encoder *x, t_floatarg index, t_floatarg r,
                             t_floatarg g, t_floatarg b) {
  if (index < 0 || index > 7) {
    post("Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  ColorRGB color = {r, g, b};
  encoder8_color_rgb(x->fd, (uint8_t)index, color);
}

void pi2c_8encoder_rotary_get(t_pi2c_8encoder *x, t_floatarg index) {
  if (index < 0 || index > 7) {
    post("Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  int32_t value = encoder8_rotary_get(x->fd, (uint8_t)index);
  outlet_float(x->x_obj.ob_outlet, value);
}

void pi2c_8encoder_setup(void) {
  pi2c_8encoder_class =
      class_new(gensym("pi2c_8encoder"), (t_newmethod)pi2c_8encoder_new,
                (t_method)pi2c_8encoder_free, sizeof(t_pi2c_8encoder),
                CLASS_DEFAULT, A_DEFFLOAT, 0);

  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_color_hsv,
                  gensym("color_hsv"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_color_rgb,
                  gensym("color_rgb"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_rotary_get,
                  gensym("rotary_get"), A_FLOAT, 0);
}

void *pi2c_8encoder_new(t_floatarg f) {
  t_pi2c_8encoder *x = (t_pi2c_8encoder *)pd_new(pi2c_8encoder_class);
  x->fd = f; // Assume the file descriptor or I2C bus is passed as an argument
  // Initialize I2C or open file descriptor here if necessary
  return (void *)x;
}

void pi2c_8encoder_free(t_pi2c_8encoder *x) {
  // Close file descriptor or clean up resources here if necessary
}
