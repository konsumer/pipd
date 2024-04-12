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

#pragma once
#include <stdint.h>

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Use this to debug
// #define I2C_DEBUG

// open i2c bus
int i2c_open(uint8_t i2cdevnum);

// close i2c bus
int i2c_close(int fd);

// set address of device to use
int i2c_set_addr(int fd, unsigned char slave_addr);

// Get a value, by register
bool i2c_get_register_val(int fd, int reg, void* out_pntr, uint8_t len);

// Set a value, by register
bool i2c_set_register_val(int fd, int reg, void* in_ptr, uint8_t len);

// Just plain read-bytes
int i2c_read(int fd, void* buf, uint8_t len);

// Just plain write-bytes
int i2c_write(int fd, void* buf, uint8_t len);