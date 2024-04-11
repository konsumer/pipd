#include "encoder4.h"

void encoder4_setup(int fd) {
  uint8_t buf[2] = { ENCODER4_NEOPIXEL_PIN, 18 };
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 2);

  buf[0] = ENCODER4_NEOPIXEL_BUF_LENGTH;
  buf[1] = 12; // 4 * 3 (RGB)
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 2);
}

void encoder4_color_rgb(int fd, uint8_t index, ColorRGB color) {
  uint8_t buf[5] = {
    ENCODER4_NEOPIXEL_BUF,
    index,
    color.r,
    color.g,
    color.b
  };
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 5);
  buf[0] = ENCODER4_NEOPIXEL_SHOW;
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 1);
}

void encoder4_color_hsv(int fd, uint8_t index, ColorHSV color) {
  encoder4_color_rgb(fd, index, hsv_to_rgb(color));
}

void encoder4_rotary_set(int fd, uint8_t index, int32_t value) {}

int32_t encoder4_rotary_get(int fd, uint8_t index) {}

uint8_t encoder4_button_get(int fd, uint8_t index) {}