#include "encoder4.h"

void encoder4_set_length(int fd, uint8_t len) {
  uint8_t buf[3] = {0};
  buf[0] = ENCODER4_NEOPIXEL_BUF_LENGTH;
  uint16_t len16 = len * 3;
  memcpy(buf + 1, &len16);
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 3);
}


void encoder4_color_hsv(int fd, uint8_t index, ColorHSV color) {
  encoder4_color_rgb(index, hsv_to_rgb(color));
}

void encoder4_color_rgb(int fd, uint8_t index, ColorRGB color) {
  uint8_t buf[4] = {
    ENCODER4_NEOPIXEL_BUF + (index * 3),
    color.r,
    color.g,
    color.b
  };
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 4);
  buf[0] = ENCODER4_NEOPIXEL_SHOW;
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 1);
}

void encoder4_rotary_set(int fd, uint8_t index, int32_t value) {}

int32_t encoder4_rotary_get(int fd, uint8_t index) {}

uint8_t encoder4_button_get(int fd, uint8_t index) {}