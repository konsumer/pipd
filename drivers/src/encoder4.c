#include "encoder4.h"

const int encoder4_button_pins[4] = {12, 14, 17, 9};

void encoder4_setup(int fd) {
  // Reset
  uint8_t buf[5] = {0x00, 0x7f, 0xff, 0, 0};
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 3);

  // Neopixel pin
  buf[0] = ENCODER4_NEOPIXEL_PIN;
  buf[1] = 0x12;
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 2);

  // Neopixel size
  buf[0] = ENCODER4_NEOPIXEL_BUF_LENGTH;
  buf[1] = 0x00;
  buf[2] = 0x0c;
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 3);

  // Button GPIO set to input/pullup

  buf[0] = ENCODER4_GPIO_DIRCLR;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x10;
  buf[4] = 0x00;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_PULLENSET;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_SET;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_DIRCLR;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x40;
  buf[4] = 0x00;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_PULLENSET;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_SET;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_DIRCLR;
  buf[1] = 0x00;
  buf[2] = 0x02;
  buf[3] = 0x00;
  buf[4] = 0x00;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_PULLENSET;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_SET;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_DIRCLR;
  buf[1] = 0x00;
  buf[2] = 0x00;
  buf[3] = 0x02;
  buf[4] = 0x00;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_PULLENSET;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);

  buf[0] = ENCODER4_GPIO_SET;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 5);
}

void encoder4_colors_rgb(int fd, ColorRGB colors[4]) {
  uint8_t buf[15] = {0};
  buf[0] = ENCODER4_NEOPIXEL_BUF;
  for (int i = 0; i < 4; i++) {
    buf[3 + (i * 3)] = colors[i].g;
    buf[4 + (i * 3)] = colors[i].r;
    buf[5 + (i * 3)] = colors[i].b;
  }
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 15);

  buf[0] = ENCODER4_NEOPIXEL_SHOW;
  i2c_set_register_val(fd, ENCODER4_NEOPIXEL, &buf, 1);
}

int32_t encoder4_rotary_get(int fd, uint8_t index) {
  uint8_t buf[4] = {0};
  buf[0] = ENCODER4_ENCODER_VAL + index;
  i2c_set_register_val(fd, ENCODER4_ENCODER, &buf, 1);
  i2c_read(fd, buf, 4);
  return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}

uint8_t encoder4_button_get(int fd, uint8_t index) {
  uint8_t buf[4] = {0};
  buf[0] = ENCODER4_GPIO_GPIO;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 1);
  usleep(250);
  i2c_read(fd, buf, 4);
  int i = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  return (i >> encoder4_button_pins[index]) & 1 == 1 ? 0 : 1;
}

void encoder4_buttons_get(int fd, uint8_t out[4]) {
  uint8_t buf[4] = {0};
  buf[0] = ENCODER4_GPIO_GPIO;
  i2c_set_register_val(fd, ENCODER4_GPIO, &buf, 1);
  usleep(250);
  i2c_read(fd, buf, 4);
  int o = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
  for (int i = 0; i < 4; i++) {
    out[i] = (o >> encoder4_button_pins[i]) & 1 == 1 ? 0 : 1;
  }
}
