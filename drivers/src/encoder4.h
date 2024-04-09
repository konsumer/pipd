#pragma once

#include "I2C.h"
#include "colors.h"

void encoder4_color_hsv(uint8_t index, ColorHSV color);
void encoder4_color_rgb(uint8_t index, ColorRGB color);
void encoder4_rotary_set(uint8_t index, int32_t value);
int32_t encoder4_rotary_get(uint8_t index);
uint8_t encoder4_button_get(uint8_t index);