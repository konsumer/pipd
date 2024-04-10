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

 * I2C.c
 *
 *  Created on  : September 19, 2017
 *  Author      : Vinay Divakar
 *  Description : This is an I2C Library for the BeagleBone that consists of the API's to support the standard
 *                I2C operations.
 *  Website     : www.deeplyembedded.org
 */

/*

Simplified by David Konsumer (konsumer) 2024

*/

/* Header Files */
#include "I2C.h"

/****************************************************************
 * Function Name : i2c_open
 * Description   : Opens the I2C device to use
 * Returns       : >0 (fd) on success, 0 on failure
 * Params        @i2cdevnum: i2c device ID (use 1 on pi)
 ****************************************************************/
int i2c_open(uint8_t i2cdevnum) {
  char i2c_dev_path[20] = {0};
  sprintf(i2c_dev_path, "/dev/i2c-%d", i2cdevnum);
  return open(i2c_dev_path, O_RDWR);
}

/****************************************************************
 * Function Name : i2c_close
 * Description   : Closes the I2C device in use
 * Returns       : 0 on success, -1 on failure
 * Params        : @fd: file descriptor
 ****************************************************************/
int i2c_close(int fd) {
  return close(fd);
}

/****************************************************************
 * Function Name : i2c_set_addr
 * Description   : Connect to the Slave device
 * Returns       : 0 on success, -1 on failure
 * Params        @fd: File descriptor
 *               @slave_addr: Address of the slave device to
 *               talk to.
 ****************************************************************/
int i2c_set_addr(int fd, unsigned char slave_addr) {
  if (ioctl(fd, I2C_SLAVE, slave_addr) < 0)
    return -1;
  else
    return 0;
}

/****************************************************************
 * Function Name : i2c_write
 * Description   : Write a byte on SDA
 * Returns       : No. of bytes written on success, -1 on failure
 * Params        @fd: File descriptor
 *               @data: data to write on SDA
 ****************************************************************/
int i2c_write(int fd, unsigned char data) {
  int ret = 0;
  ret = write(fd, &data, 1);
  if ((ret == -1) || (ret != 1))
    return -1;
  else
    return (ret);
}

/****************************************************************
 * Function Name : i2c_read
 * Description   : Read a byte on SDA
 * Returns       : No. of bytes read on success, -1 on failure
 * Params        @fd: File descriptor
 *               @read_data: Points to the variable  that stores
 *               the read data byte
 ****************************************************************/
int i2c_read(int fd, unsigned char* read_data) {
  int ret = 0;
  ret = read(fd, &read_data, 1);
  if (ret == -1)
    perror("I2C: Failed to read |");
  if (ret == 0)
    perror("I2C: End of FILE |");
  return (ret);
}

/****************************************************************
 * Function Name : i2c_read_register
 * Description   : Read a single register of the slave device
 * Returns       : No. of bytes read on success, -1 on failure
 * Params        @fd: File descriptor
 *               @read_addr: Register address to be read
 *               @read_data: Points to the variable  that stores
 *               the read data byte
 ****************************************************************/
int i2c_read_register(int fd, unsigned char read_addr, unsigned char* read_data) {
  int ret = 0;
  if (i2c_write(fd, read_addr) == -1) {
    perror("I2C: Failed to write |");
    return -1;
  }
  ret = read(fd, &read_data, 1);
  if (ret == -1)
    perror("I2C: Failed to read |");
  if (ret == 0)
    perror("I2C: End of FILE |");
  return (ret);
}

/****************************************************************
 * Function Name : i2c_read_registers
 * Description   : Read a multiple registers on the slave device
 *                 from starting address
 * Returns       : No. of bytes read on success, -1 on failure
 * Params        @fd: File descriptor
 *               @num: Number of registers/bytes to read from.
 *               @starting_addr: Starting address to read from
 *               @buff_Ptr: Buffer to store the read bytes
 ****************************************************************/
int i2c_read_registers(int fd, int num, unsigned char starting_addr, unsigned char* buff_Ptr) {
  int ret = 0;
  if (i2c_write(fd, starting_addr) == -1) {
    perror("I2C: Failed to write |");
    return -1;
  }
  ret = read(fd, buff_Ptr, num);
  if (ret == -1)
    perror("I2C: Failed to read |");
  if (ret == 0)
    perror("I2C: End of FILE |");
  return (ret);
}

/****************************************************************
 * Function Name : i2c_multiple_writes
 * Description   : Write multiple bytes on SDA
 * Returns       : No. of bytes written on success, -1 on failure
 * Params        @fd: file descriptor
 *               @num: No. of bytes to write
 *               @Ptr_buff: Pointer to the buffer containing the
 *               bytes to be written on the SDA
 ****************************************************************/
int i2c_multiple_writes(int fd, int num, unsigned char* Ptr_buff) {
  int ret = 0;
  ret = write(fd, Ptr_buff, num);
  if ((ret == -1) || (ret != num))
    return -1;
  else
    return (ret);
}

/****************************************************************
 * Function Name : i2c_write_register
 * Description   : Write a control byte or byte to a register
 * Returns       : No. of bytes written on success, -1 on failure
 * Params        @fd: file descriptor
 *               @reg_addr_or_cntrl: Control byte or Register
 *               address to be written
 *               @val: Command or value to be written in the
 *               addressed register
 ****************************************************************/
int i2c_write_register(int fd, unsigned char reg_addr_or_cntrl, unsigned char val) {
  unsigned char buff[2];
  int ret = 0;
  buff[0] = reg_addr_or_cntrl;
  buff[1] = val;
  ret = write(fd, buff, 2);
  if ((ret == -1) || (ret != 2))
    return -1;
  else
    return (ret);
}
