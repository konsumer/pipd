#pragma once

/*
 * MIT License

Copyright (c) 2017 DeeplyEmbedded

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

 * I2C.h
 *
 *  Created on  : Sep 4, 2017
 *  Author      : Vinay Divakar
 *  Website     : www.deeplyembedded.org
 */

/*

Simplified by David Konsumer (konsumer) 2024

*/

#include <stdint.h>

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
// heuristic to guess what version of i2c-dev.h we have:
// the one installed with `apt-get install libi2c-dev`
// would conflict with linux/i2c.h, while the stock
// one requires linus/i2c.h
#ifndef I2C_SMBUS_BLOCK_MAX
// If this is not defined, we have the "stock" i2c-dev.h
// so we include linux/i2c.h
#include <linux/i2c.h>
#endif

/* Exposed Generic I2C Functions */
extern int i2c_open(uint8_t i2cdevnum);
extern int i2c_close(int fd);
extern int i2c_set_addr(int fd, unsigned char slave_addr);
extern int i2c_write(int fd, unsigned char data);
extern int i2c_read(int fd, unsigned char* read_data);
extern int i2c_read_register(int fd, unsigned char read_addr, unsigned char* read_data);
extern int i2c_read_registers(int fd, int num, unsigned char starting_addr, unsigned char* buff_Ptr);
extern int i2c_multiple_writes(int fd, int num, unsigned char* Ptr_buff);
extern int i2c_write_register(int fd, unsigned char reg_addr_or_cntrl, unsigned char val);
