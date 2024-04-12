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