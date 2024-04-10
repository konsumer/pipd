#pragma once

#include <stdint.h>

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

// HSV is 0-1 floats, returns single uint32
ColorRGB hsv_to_rgb(ColorHSV color);

