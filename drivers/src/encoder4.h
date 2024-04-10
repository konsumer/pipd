#pragma once

#include "I2C.h"
#include "hsv_to_rgb.h"

// default address
#define ENCODER4_ADDR 0x49

// base registers
#define ENCODER4_ENCODER 0x11
#define ENCODER4_NEOPIXEL 0x0E

// ref: https://learn.adafruit.com/adafruit-seesaw-atsamd09-breakout/reading-and-writing-data

void encoder4_color_hsv(uint8_t index, ColorHSV color);
void encoder4_color_rgb(uint8_t index, ColorRGB color);
void encoder4_rotary_set(uint8_t index, int32_t value);
int32_t encoder4_rotary_get(uint8_t index);
uint8_t encoder4_button_get(uint8_t index);