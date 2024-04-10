This is 3 externals for puredata, for use with i2c devices & pi:

- [4 knob rotary-endoer thing](https://www.adafruit.com/product/5752) it's i2c (so no extra GPIO is used up) and it has buttons and RGB LEDs.
- [8 knob rotary-encoder thing](https://docs.m5stack.com/en/unit/8Encoder) it's also i2c and it has buttons and RGB LEDs,a nd has a toggle switch.
- 128x64 SSD1306 monochrome OLED. These are surplus from really old phones, easy to find, and are pretty crisp & bright & cheap.

You can download from [here](LINK_NEEDED) and put in your puredata-path on a pi.

The basic structure is like this in pd:

![pd](pd.png)

Basically, initialize the i2c bus with `pi_i2c` then connect that outlet to the others. 

## messages

```
pi_i2c
  inlet 1 messages
    INT - connect to a bus (same as i2cdetect, try 1)

  outlet 1 messages
    connect FD - this tells other patches what i2c bus to connect to

4encoder
  params

    address - int, default:73 (0x49)
  
  inlet 1 messages

    bang - update values
    hsv ID H S V - set the color - ID is 0-3, H/S/V are float 0-1
    rgb ID R G B - set the color - ID is 0-3, R/G/B are int 0-255
    rotary ID VAL - set the rotary value - ID is 0-3, VAL is int -2147483648 - 2147483647

  outlet 1 messages

    rotary ID VAL - on change, ID is 0-3, VAL is int -2147483648 - 2147483647
    button ID VAL - on change, ID is 0-3, VAL is int 0/1


8encoder
  params

    address - int, default:65 (0x41)

  inlet 1 messages
  
    bang - update values
    hsv ID H S V - set the color - ID is 0-7, H/S/V are float 0-1
    rgb ID R G B - set the color - ID is 0-7, R/G/B are int 0-255
    rotary ID VAL - set the rotary value - ID is 0-7, VAL is int -2147483648 - 2147483647

  outlet 1 messages

    rotary ID VAL - on change, ID is 0-7, VAL is int -2147483648 - 2147483647
    button ID VAL - on change, ID is 0-7, VAL is int 0/1


ssd1306
  params
    address - int, default:60 (0x3c)

  inlet 1 messages
    text COLOR X Y ...TEXT - COLOR is 0/1
    size X - set text-size to X
    rectangle COLOR X Y W H - COLOR is 0/1
    circle COLOR X Y R - draw a cirlce, COLOR is 0/1
    triangle COLOR X0 Y0 X1 Y1 X2 Y2 - draw a triangle with 3 points, COLOR is 0/1
    line COLOR X0 Y0 X1 Y1 - draw a line between 2 points, COLOR is 0/1
    graph COLOR X Y W H ...NUMBERS - creates a scaled graph of NUMBERS that fits in the rect, COLOR is 0/1
    clear - clear the display
    scroll TYPE - do something with hardware-scrolling, TYPE is RIGHT/LEFT/DIAGRIGHT/DIAGLEFT/STOP
    rotate DEG - rotate screen. DEG is 0-359
    bitmap X Y NAME - display a 1bit bitmap file (not done yet)
    invert X - invert colors - X is 0/1
    bang - render the current screen (call this on every change)
```


## setup

Make sure to add this to your `/boot/firmware/config.txt`:

```
# dtparam=i2c_arm=on,i2c_arm_baudrate=50000
# dtparam=i2c_arm=on,i2c_arm_baudrate=100000
# dtparam=i2c_arm=on,i2c_arm_baudrate=400000
dtparam=i2c_arm=on,i2c_arm_baudrate=1000000
```

## building

You will need these things, if you want to build it, yourself:

```
sudo apt install libi2c-dev build-essential puredata-dev
```