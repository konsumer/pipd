#include "encoder8.h"

void encoder8_color_hsv(int fd, uint8_t index, ColorHSV color) {
  ColorRGB c = hsv_to_rgb(color);
  encoder8_color_rgb(fd, index, c);
}

void encoder8_color_rgb(int fd, uint8_t index, ColorRGB color) {
  i2c_set_register_val(fd, ENCODER8_RGB_LED + (index * 3), (uint8_t*)&color, 3);
  usleep(1000);
}

void encoder8_rotary_set(int fd, uint8_t index, int32_t value) {
  i2c_set_register_val(fd, index * 4, (uint8_t*)&value, 4);
}

int32_t encoder8_rotary_get(int fd, uint8_t index) {
  int32_t out = 0;
  i2c_get_register_val(fd, index * 4, (uint8_t*)&out, 4);
  return out;
}

uint8_t encoder8_button_get(int fd, uint8_t index) {
  uint8_t out = 0;
  i2c_get_register_val(fd, ENCODER8_BUTTON + index, &out, 1);
  // vals are inverted
  return out ? 0 : 1;
}

uint8_t encoder8_switch_get(int fd) {
  uint8_t out = 0;
  i2c_get_register_val(fd, ENCODER8_SWITCH, &out, 1);
  return out;
}