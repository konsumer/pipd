#include "oled.h"

int main(int argc, char *argv[]) {
  int fd = i2c_open(1);

  if (fd < 0) {
    perror("Could not open i2c bus.");
    return 1;
  }

  if (i2c_set_addr(fd, SSD1306_OLED_ADDR) < 0) {
    perror("Could not open the encoder.");
    return 1;
  }

  return 0;
}