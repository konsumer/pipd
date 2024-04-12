/*
 * MIT License

Copyright (c) 2024 David Konsumer (konsumer)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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