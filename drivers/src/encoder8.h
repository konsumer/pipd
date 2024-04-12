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

#include "I2C.h"
#include "hsv_to_rgb.h"

// default address
#define ENCODER8_ADDR 0x41

// registers
#define ENCODER8_INCREMENT 0x20
#define ENCODER8_BUTTON 0x50
#define ENCODER8_SWITCH 0x60
#define ENCODER8_RGB_LED 0x70
#define ENCODER8_RESET_COUNTER 0x40
#define ENCODER8_FIRMWARE_VERSION 0xFE
#define ENCODER8_I2C_ADDRESS 0xFF

// Set color using HSV
void encoder8_color_hsv(int fd, uint8_t index, ColorHSV color);

// Set color using RGB
void encoder8_color_rgb(int fd, uint8_t index, ColorRGB color);

// Set the current value of a rotary-encoder
void encoder8_rotary_set(int fd, uint8_t index, int32_t value);

// Get the current value of a rotary-encoder
int32_t encoder8_rotary_get(int fd, uint8_t index);

// Get the current value of a button (push down rotary-encoder)
uint8_t encoder8_button_get(int fd, uint8_t index);

// Get the current value of the toggle-switch
uint8_t encoder8_switch_get(int fd);
