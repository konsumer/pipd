#include <stdio.h>
#include "encoder8.h"

int main(int argc, char* argv[]) {
  int fd = i2c_open(1);

  i2c_close(fd);
  return 0;
}