## pi2c_4encoder

Puredata object, in native extension, for this [4 knob rotary-endoer thing](https://www.adafruit.com/product/5752).

### inlet 1 messages

- `bang` - reads values from all inputs, and send values that changed
- `rgb ID R G B` - set color of LED at ID (0-3) to RGB val. All are ints, 0-255.
- `hsv ID H S V` - set color of LED at ID (0-3) to HSV val. All are floats, 0-1.
- `rotary ID VAL` Set rotary at ID (0-3) to this value. Values are int -2147483648 - 2147483647

### outlet 1 messages

- `rotary ID VAL` - on change, ID is 0-3, VAL is int -2147483648 - 2147483647
- `button ID VAL` - on change, ID is 0-3, VAL is int 0/1
