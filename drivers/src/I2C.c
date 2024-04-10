/*
 * MIT License

Copyright (c) 2024  David Konsumer (konsumer)

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

#include "I2C.h"

// open i2c bus
int i2c_open(uint8_t i2cdevnum) {
  char i2c_dev_path[20] = {0};
  sprintf(i2c_dev_path, "/dev/i2c-%d", i2cdevnum);
  return open(i2c_dev_path, O_RDWR);
}

// close i2c bus
int i2c_close(int fd) {
  return close(fd);
}

// set address of device to use
int i2c_set_addr(int fd, unsigned char slave_addr) {
  return ioctl(fd, I2C_SLAVE, slave_addr);
}

// Get a value, by register
bool i2c_get_register_val(int fd, int reg, void* out_pntr, uint8_t len) {
  if (write(fd, &reg, 1) != 1) {
    return false;
  }
  if (read(fd, out_pntr, len) != len) {
    return false;
  }
  return true;
}

// Set a value, by register (max length 32 bytes)
bool i2c_set_register_val(int fd, int reg, void* in_ptr, uint8_t len) {
  uint8_t msg[len + 1];
  msg[0] = reg;
  memcpy(msg + 1, in_ptr, len);

  if (write(fd, msg, len + 1) != (len + 1)) {
    return false;
  }
}