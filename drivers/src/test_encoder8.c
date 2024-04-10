#include <stdio.h>
#include "encoder8.h"

int main(int argc, char* argv[]) {
  int fd = i2c_open(1);

  if (fd < 0) {
    perror("Could not open i2c bus.");
    return 1;
  }

  if (i2c_set_addr(fd, ENCODER8_ADDR) < 0) {
    perror("Could not open the encoder.");
    return 1;
  }

  ColorRGB c = {0};
  for (int i = 0; i < 4; i++) {
    encoder8_color_rgb(fd, i, c);
  }

  // make a rainbow
  ColorHSV color = {.h = 0, .s = 1, .v = 1};
  for (int i = 0; i < 8; i++) {
    color.h = 0.125 * i;
    encoder8_color_hsv(fd, i, color);
  }

  // draw single-line indicator of rotaries/buttons
  while (true) {
    printf("\r");
    for (int i = 0; i < 8; i++) {
      printf("%d %s  ", encoder8_rotary_get(fd, i), encoder8_button_get(fd, i) == 1 ? "⚪" : "⚫");
    }
    printf("| %s", encoder8_switch_get(fd) ? "⚪" : "⚫");
    fflush(stdout);
  }

  i2c_close(fd);
  return 0;
}