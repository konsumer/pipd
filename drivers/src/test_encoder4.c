#include <stdio.h>
#include "encoder4.h"

int main(int argc, char* argv[]) {
  int fd = i2c_open(1);

  if (fd < 0) {
    perror("Could not open i2c bus.");
    return 1;
  }

  if (i2c_set_addr(fd, ENCODER4_ADDR) < 0) {
    perror("Could not open the encoder.");
    return 1;
  }

  encoder4_setup(fd);

  ColorRGB colors[4] = {0};

  int i = 0;

  for (i = 0; i < 4; i++) {
    encoder4_rotary_set(fd, i, 1000);
  }

  // make a rainbow
  ColorHSV color = {.h = 0, .s = 1, .v = 1};
  for (i = 0; i < 4; i++) {
    color.h = 0.25 * i;
    colors[i] = hsv_to_rgb(color);
  }
  encoder4_colors_rgb(fd, colors);

  uint8_t out[4] = {0};

  // draw single-line indicator of rotaries/buttons
  while (true) {
    encoder4_buttons_get(fd, out);
    printf("                                                          \r");
    for (i = 0; i < 4; i++) {
      printf("%d %s  ", encoder4_rotary_get(fd, i), out[i] == 1 ? "⚪" : "⚫");
    }
    fflush(stdout);
  }

  i2c_close(fd);
  return 0;
}