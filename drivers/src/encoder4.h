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
#define ENCODER4_ADDR 0x49

// base registers
#define ENCODER4_ENCODER 0x11
#define ENCODER4_NEOPIXEL 0x0E
#define ENCODER4_GPIO 0x01

// commands
#define ENCODER4_NEOPIXEL_PIN 0x01
#define ENCODER4_NEOPIXEL_SPEED 0x02
#define ENCODER4_NEOPIXEL_BUF_LENGTH 0x03
#define ENCODER4_NEOPIXEL_BUF 0x04
#define ENCODER4_NEOPIXEL_SHOW 0x05
#define ENCODER4_ENCODER_VAL 0x30
#define ENCODER4_GPIO_DIRCLR 0x03
#define ENCODER4_GPIO_PULLENSET 0x0B
#define ENCODER4_GPIO_GPIO 0x04
#define ENCODER4_GPIO_SET 0x05

// ref: https://learn.adafruit.com/adafruit-seesaw-atsamd09-breakout/reading-and-writing-data

// Setup encoder(pins, etc)
void encoder4_setup(int fd);

// Set all colors at once
void encoder4_colors_rgb(int fd, ColorRGB color[4]);

// Get the current value of a rotary-encoder
int32_t encoder4_rotary_get(int fd, uint8_t index);

// Set current value of rotary
void encoder4_rotary_set(int fd, uint8_t index, int32_t value);

// Get the current value of a button (push down rotary-encoder)
uint8_t encoder4_button_get(int fd, uint8_t index);

// Get all button values at once
void encoder4_buttons_get(int fd, uint8_t out[4]);


