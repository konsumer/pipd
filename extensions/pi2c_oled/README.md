## pi2c_oled

Puredata object, in native extension, for this [128x64 SSD1306 monochrome OLED](https://electropeak.com/0-96-inch-i2c-oled-display-module-ssd1306).

### inlet 1 messages

- `bang` - render the current screen (call this on every change you want to display)
- `text COLOR X Y ...TEXT` - COLOR is 0/1
- `textsize X` - set text-size to X
- `rectangle COLOR X Y W H` - COLOR is 0/1
- `circle COLOR X Y R` - draw a cirlce, COLOR is 0/1
- `triangle COLOR X0 Y0 X1 Y1 X2 Y2` - draw a triangle with 3 points, COLOR is 0/1
- `line COLOR X0 Y0 X1 Y1` - draw a line between 2 points, COLOR is 0/1
- `graph COLOR X Y W H ...NUMBERS` - creates a scaled graph of NUMBERS that fits in the rect, COLOR is 0/1
- `clear` - clear the display
- `scroll TYPE` - do something with hardware-scrolling, TYPE is RIGHT/LEFT/DIAGRIGHT/DIAGLEFT/STOP
- `rotate DEG` - rotate screen. DEG is 0-359
- `invert X` - invert colors - X is 0/1
- `loadimage NAME FILENAME` - load & convert an image (prepare for drawing)
- `image COLOR X Y NAME` - display a loaded image, COLOR is 0/1
