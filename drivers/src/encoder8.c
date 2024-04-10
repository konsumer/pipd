#include "encoder8.h"

void encoder8_color_hsv(int fd, uint8_t index, ColorHSV color) {
  encoder8_color_rgb(fd, index, hsv_to_rgb(color));
}

void encoder8_color_rgb(int fd, uint8_t index, ColorRGB color) {

}

void encoder8_rotary_set(int fd, uint8_t index, int32_t value) {}

int32_t encoder8_rotary_get(int fd, uint8_t index) {
  int32_t out = 0;
  i2c_read_registers(fd, 4, index*4, (unsigned char *) &out);
  return out;
}

uint8_t encoder8_button_get(int fd, uint8_t index) {
  uint8_t out = 0;
  return i2c_read_register(fd, ENCODER8_BUTTON + index, &out);
  return out;
}

uint8_t encoder8_switch_get(int fd) {
  uint8_t out = 0;
  return i2c_read_register(fd, ENCODER8_SWITCH, &out);
  return out;
}