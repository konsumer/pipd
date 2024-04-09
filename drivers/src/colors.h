#pragma once

#include <stdint.h>

// convert 4 uint8_t to uint32_t
uint32_t uint8s_to_32(uint8_t v[4]);

// HSV is 0-1 floats, returns single uint32
uint32_t hsv_to_rgb_int(float h, float s, float v);

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} ColorRGB;

typedef struct {
  float h;
  float s;
  float v;
} ColorHSV;
