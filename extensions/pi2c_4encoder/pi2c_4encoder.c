#include <stdint.h>
#include <stdlib.h>

#include "encoder4.h"
#include "m_pd.h"

static t_class *pi2c_4encoder_class;

typedef struct _pi2c_4encoder {
  t_object x_obj;
  t_outlet *output_outlet;  // Single outlet for both rotary and button messages
  int fd;                   // Identification for the encoder hardware
  ColorRGB colors[4];       // Represents the current RGB LED colors (needed because 4encoder only lets you set them all)
  int rotaries[4];          // Current state of rotaries
  uint8_t buttons[4];       // Curent state of buttons
} t_pi2c_4encoder;

static void pi2c_4encoder_free(t_pi2c_4encoder *x);
static void *pi2c_4encoder_new(void);
static void pi2c_4encoder_bang(t_pi2c_4encoder *x);
static void pi2c_4encoder_rgb(t_pi2c_4encoder *x, t_floatarg n, t_floatarg r, t_floatarg g, t_floatarg b);
static void pi2c_4encoder_hsv(t_pi2c_4encoder *x, t_floatarg n, t_floatarg h, t_floatarg s, t_floatarg v);
static void pi2c_4encoder_rotary_set(t_pi2c_4encoder *x, t_floatarg n, t_floatarg val);

void pi2c_4encoder_setup(void) {
  pi2c_4encoder_class = class_new(gensym("pi2c_4encoder"), (t_newmethod)pi2c_4encoder_new, (t_method)pi2c_4encoder_free, sizeof(t_pi2c_4encoder), CLASS_DEFAULT, 0);
  class_addbang(pi2c_4encoder_class, pi2c_4encoder_bang);
  class_addmethod(pi2c_4encoder_class, (t_method)pi2c_4encoder_rgb, gensym("rgb"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_4encoder_class, (t_method)pi2c_4encoder_hsv, gensym("hsv"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_4encoder_class, (t_method)pi2c_4encoder_rotary_set, gensym("rotary"), A_FLOAT, A_FLOAT, 0);
}

static void *pi2c_4encoder_new(void) {
  t_pi2c_4encoder *x = (t_pi2c_4encoder *)pd_new(pi2c_4encoder_class);
  x->output_outlet = outlet_new(&x->x_obj, &s_list);
  x->fd = i2c_open(1);
  if (x->fd < 0) {
    post("pi2c_4encoder: Could not open i2c bus.");
    return NULL;
  }
  if (i2c_set_addr(x->fd, ENCODER4_ADDR) < 0) {
    post("pi2c_4encoder: Could not open the encoder.");
    return NULL;
  }
  encoder4_setup(x->fd);
  encoder4_colors_rgb(x->fd, x->colors);
  return (void *)x;
}

static void pi2c_4encoder_free(t_pi2c_4encoder *x) {
  i2c_close(x->fd);
}

static void send_rotary(t_pi2c_4encoder *x, int i, int v) {
  t_atom out[3];
  SETSYMBOL(&out[0], gensym("rotary"));
  SETFLOAT(&out[1], i);
  SETFLOAT(&out[2], v);
  outlet_list(x->output_outlet, &s_list, 3, out);
}

static void send_button(t_pi2c_4encoder *x, int i, int v) {
  t_atom out[3];
  SETSYMBOL(&out[0], gensym("button"));
  SETFLOAT(&out[1], i);
  SETFLOAT(&out[2], v);
  outlet_list(x->output_outlet, &s_list, 3, out);
}

// BANG handler: read and output state changes
static void pi2c_4encoder_bang(t_pi2c_4encoder *x) {
  int rotaries[4] = {0};
  uint8_t buttons[4] = {0};
  encoder4_buttons_get(x->fd, buttons);
  for (int i = 0; i < 4; i++) {
    rotaries[i] = encoder4_rotary_get(x->fd, i);
    if (rotaries[i] != x->rotaries[i]) {
      x->rotaries[i] = rotaries[i];
      send_rotary(x, i, rotaries[i]);
    }
    if (buttons[i] != x->buttons[i]) {
      x->buttons[i] = buttons[i];
      send_button(x, i, buttons[i]);
    }
  }
}

static void pi2c_4encoder_rgb(t_pi2c_4encoder *x, t_floatarg n, t_floatarg r, t_floatarg g, t_floatarg b) {
  if (n < 0 || n > 3) {
    post("pi2c_4encoder rgb: Invalid encoder ID. Must be between 0 and 3.");
    return;
  }
  if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
    post("pi2c_4encoder rgb: Invalid RGB. Must be between 0 and 255.");
    return;
  }

  // post("RGB (%d): %d, %d, %d", (int)n, (int)r, (int)g, (int)b);

  x->colors[(int)n].r = (unsigned char)r;
  x->colors[(int)n].g = (unsigned char)g;
  x->colors[(int)n].b = (unsigned char)b;
  encoder4_colors_rgb(x->fd, x->colors);
}

static void pi2c_4encoder_hsv(t_pi2c_4encoder *x, t_floatarg n, t_floatarg h, t_floatarg s, t_floatarg v) {
  if (n < 0 || n > 3) {
    post("pi2c_4encoder hsv: Invalid encoder ID. Must be between 0 and 3.");
    return;
  }
  if (h < 0.0 || h > 1.0 || s < 0.0 || s > 1.0 || v < 0.0 || v > 1.0) {
    post("pi2c_4encoder hsv: Invalid HSV. Must be between 0 and 1.");
    return;
  }

  // post("HSV (%d): %f, %f, %f", (int)n, (float)h, (float)s, (float)v);

  ColorHSV hsv = {.h = (float)h, .s = (float)s, .v = (float)v};
  ColorRGB color = hsv_to_rgb(hsv);
  x->colors[(int)n].r = color.r;
  x->colors[(int)n].g = color.g;
  x->colors[(int)n].b = color.b;
  encoder4_colors_rgb(x->fd, x->colors);
}

static void pi2c_4encoder_rotary_set(t_pi2c_4encoder *x, t_floatarg n, t_floatarg val) {
  if (n < 0 || n > 3) {
    post("pi2c_4encoder rotary: Invalid encoder ID. Must be between 0 and 3.");
    return;
  }
  encoder4_rotary_set(x->fd, (uint8_t)n, (int32_t)val);
}
