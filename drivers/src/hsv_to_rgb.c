#include "hsv_to_rgb.h"

// HSV is 0-1 floats, returns single uint32
ColorRGB hsv_to_rgb(ColorHSV color) {
  uint8_t hsv[3] = {(uint8_t)(color.h * 255.0), (uint8_t)(color.s * 255.0), (uint8_t)(color.v * 255.0)};

  ColorRGB c = {0};

  uint8_t region, remainder, p, q, t;

  if (hsv[1] == 0) {
    c.r = hsv[2];
    c.g = hsv[2];
    c.b = hsv[2];
    return c;
  }

  region = hsv[0] / 43;
  remainder = (hsv[0] - (region * 43)) * 6;

  p = (hsv[2] * (255 - hsv[1])) >> 8;
  q = (hsv[2] * (255 - ((hsv[1] * remainder) >> 8))) >> 8;
  t = (hsv[2] * (255 - ((hsv[1] * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
    case 0:
      c.r = hsv[2];
      c.g = t;
      c.b = p;
      break;
    case 1:
      c.r = q;
      c.g = hsv[2];
      c.b = p;
      break;
    case 2:
      c.r = p;
      c.g = hsv[2];
      c.b = t;
      break;
    case 3:
      c.r = p;
      c.g = q;
      c.b = hsv[2];
      break;
    case 4:
      c.r = t;
      c.g = p;
      c.b = hsv[2];
      break;
    default:
      c.r = hsv[2];
      c.g = p;
      c.b = q;
      break;
  }

  return c;
}