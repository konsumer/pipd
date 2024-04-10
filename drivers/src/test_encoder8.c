#include "encoder8.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if(init_i2c_dev1(I2C_DEV1_PATH, ENCODER8_ADDR) == 0) {
    printf("Bus Connected to SSD1306\n");
  } else {
    fprintf(stderr, "OOPS! Something Went Wrong\n");
    return 1;
  }

  
  while (1) {
    printf("Encoder 0: %d\n", encoder8_rotary_get(I2C_DEV_1.fd_i2c, 0));
  }
  return 0;
}