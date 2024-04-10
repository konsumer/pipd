#pragma once

#include "I2C.h"
#include "hsv_to_rgb.h"

// default address, if it hasn;t been chnaged
#define ENCODER8_ADDR 0x41

// registers
#define ENCODER8_INCREMENT 0x20
#define ENCODER8_BUTTON 0x50
#define ENCODER8_SWITCH 0x60
#define ENCODER8_RGB_LED 0x70
#define ENCODER8_RESET_COUNTER 0x40
#define ENCODER8_FIRMWARE_VERSION 0xFE
#define ENCODER8_I2C_ADDRESS 0xFF

void encoder4_color_hsv(uint8_t index, ColorHSV color);
void encoder4_color_rgb(uint8_t index, ColorRGB color);
void encoder4_rotary_set(uint8_t index, int32_t value);
int32_t encoder4_rotary_get(uint8_t index);
uint8_t encoder4_button_get(uint8_t index);