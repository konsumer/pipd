> [!WARNING]  
> I am still working on this.

This is 3 C libs for use with i2c devices & pi:

- [4 knob rotary-endoer thing](https://www.adafruit.com/product/5752) it's i2c (so no extra GPIO is used up) and it has buttons and RGB LEDs.
- [8 knob rotary-encoder thing](https://docs.m5stack.com/en/unit/8Encoder) it's also i2c and it has buttons and RGB LEDs, and has a toggle switch.
- 128x64 SSD1306 monochrome OLED. These are surplus from really old phones, easy to find, and are pretty crisp & bright & cheap.

The C libs are pretty simple, and basically just the bare-minimum to support the stuff I need for my puredata extensions, but you could use them as a base for your own thing. 


## setup

Make sure to add this to your `/boot/firmware/config.txt`:

```
# dtparam=i2c_arm=on,i2c_arm_baudrate=50000
# dtparam=i2c_arm=on,i2c_arm_baudrate=100000
# dtparam=i2c_arm=on,i2c_arm_baudrate=400000
dtparam=i2c_arm=on,i2c_arm_baudrate=1000000
```

The OLED/i2c code came from [here](https://github.com/giuliomoro/OSC2OLED4Bela)

## building


You will need these things to build:

```
sudo apt install libi2c-dev build-essential
```