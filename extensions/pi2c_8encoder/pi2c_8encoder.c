#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "encoder8.h"
#include "m_pd.h"

static t_class *pi2c_8encoder_class;

typedef struct _pi2c_8encoder {
  t_object x_obj;
  t_outlet *output_outlet;  // Single outlet for both rotary and button messages
  int fd;                   // Identification for the encoder hardware
  int rotaries[8];          // Current state of rotaries
  int buttons[8];           // Curent state of buttons
  int sw;                   // Current switch value
  bool dorotaries;          // I have to alternate buttons/rotaries because it's too many requests for a single pass
} t_pi2c_8encoder;

static void pi2c_8encoder_free(t_pi2c_8encoder *x);
static void *pi2c_8encoder_new(void);
static void pi2c_8encoder_bang(t_pi2c_8encoder *x);
static void pi2c_8encoder_rgb(t_pi2c_8encoder *x, t_floatarg n, t_floatarg r, t_floatarg g, t_floatarg b);
static void pi2c_8encoder_hsv(t_pi2c_8encoder *x, t_floatarg n, t_floatarg h, t_floatarg s, t_floatarg v);
static void pi2c_8encoder_rotary_set(t_pi2c_8encoder *x, t_floatarg n, t_floatarg val);

void pi2c_8encoder_setup(void) {
  pi2c_8encoder_class = class_new(gensym("pi2c_8encoder"), (t_newmethod)pi2c_8encoder_new, (t_method)pi2c_8encoder_free, sizeof(t_pi2c_8encoder), CLASS_DEFAULT, 0);
  class_addbang(pi2c_8encoder_class, pi2c_8encoder_bang);
  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_rgb, gensym("rgb"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_hsv, gensym("hsv"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
  class_addmethod(pi2c_8encoder_class, (t_method)pi2c_8encoder_rotary_set, gensym("rotary"), A_FLOAT, A_FLOAT, 0);
}

static void *pi2c_8encoder_new(void) {
  t_pi2c_8encoder *x = (t_pi2c_8encoder *)pd_new(pi2c_8encoder_class);
  x->output_outlet = outlet_new(&x->x_obj, &s_list);
  x->fd = i2c_open(1);
  if (x->fd < 0) {
    post("pi2c_8encoder: Could not open i2c bus.");
    return NULL;
  }
  if (i2c_set_addr(x->fd, ENCODER8_ADDR) < 0) {
    post("pi2c_8encoder: Could not open the encoder.");
    return NULL;
  }

  // init everything
  x->sw = 0;
  ColorRGB black = {0};
  for (int i = 0; i < 8; i++) {
    encoder8_color_rgb(x->fd, i, black);
    x->rotaries[i] = 0;
    x->buttons[i] = 0;
  }
  x->dorotaries = false;

  return (void *)x;
}

static void pi2c_8encoder_free(t_pi2c_8encoder *x) {
  i2c_close(x->fd);
}

static void send_rotary(t_pi2c_8encoder *x, int i, int v) {
  t_atom out[3];
  SETSYMBOL(&out[0], gensym("rotary"));
  SETFLOAT(&out[1], i);
  SETFLOAT(&out[2], v);
  outlet_list(x->output_outlet, &s_list, 3, out);
}

static void send_button(t_pi2c_8encoder *x, int i, int v) {
  t_atom out[3];
  SETSYMBOL(&out[0], gensym("button"));
  SETFLOAT(&out[1], i);
  SETFLOAT(&out[2], v);
  outlet_list(x->output_outlet, &s_list, 3, out);
}

static void send_switch(t_pi2c_8encoder *x, int v) {
  t_atom out[2];
  SETSYMBOL(&out[0], gensym("switch"));
  SETFLOAT(&out[1], v);
  outlet_list(x->output_outlet, &s_list, 2, out);
}

// BANG handler: read and output state changes
static void pi2c_8encoder_bang(t_pi2c_8encoder *x) {
  int v = 0;

  if (!x->dorotaries) {
    x->dorotaries = true;
    v = encoder8_switch_get(x->fd);
    if (v != x->sw) {
      x->sw = v;
      send_switch(x, v);
    }
  } else {
    x->dorotaries = false;
  }

  for (int i = 0; i < 8; i++) {
    if (x->dorotaries) {
      v = encoder8_rotary_get(x->fd, i);
      if (v != x->rotaries[i]) {
        x->rotaries[i] = v;
        send_rotary(x, i, v);
      }
    } else {
      v = encoder8_button_get(x->fd, i);
      if (v != x->buttons[i]) {
        x->buttons[i] = v;
        send_button(x, i, v);
      }
    }
  }

  // post("%d %d %d %d %d %d %d %d\n", x->rotaries[0], x->rotaries[1], x->rotaries[2], x->rotaries[3], x->rotaries[4], x->rotaries[5], x->rotaries[6], x->rotaries[7]);
}

static void pi2c_8encoder_rgb(t_pi2c_8encoder *x, t_floatarg n, t_floatarg r, t_floatarg g, t_floatarg b) {
  if (n < 0 || n > 7) {
    post("pi2c_8encoder rgb: Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
    post("pi2c_8encoder rgb: Invalid RGB. Must be between 0 and 255.");
    return;
  }

  // post("RGB (%d): %d, %d, %d", (int)n, (int)r, (int)g, (int)b);

  ColorRGB color = {.r = (unsigned char)r, .g = (unsigned char)g, .b = (unsigned char)b};
  encoder8_color_rgb(x->fd, (uint8_t)n, color);
}

static void pi2c_8encoder_hsv(t_pi2c_8encoder *x, t_floatarg n, t_floatarg h, t_floatarg s, t_floatarg v) {
  if (n < 0 || n > 7) {
    post("pi2c_8encoder hsv: Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  if (h < 0.0 || h > 1.0 || s < 0.0 || s > 1.0 || v < 0.0 || v > 1.0) {
    post("pi2c_8encoder hsv: Invalid HSV. Must be between 0 and 1.");
    return;
  }

  // post("HSV (%d): %f, %f, %f", (int)n, (float)h, (float)s, (float)v);

  ColorHSV color = {.h = (float)h, .s = (float)s, .v = (float)v};
  encoder8_color_hsv(x->fd, (uint8_t)n, color);
}

static void pi2c_8encoder_rotary_set(t_pi2c_8encoder *x, t_floatarg n, t_floatarg val) {
  if (n < 0 || n > 7) {
    post("pi2c_8encoder rotary: Invalid encoder ID. Must be between 0 and 7.");
    return;
  }
  encoder8_rotary_set(x->fd, (uint8_t)n, (int32_t)val);
}
