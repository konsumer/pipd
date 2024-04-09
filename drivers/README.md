> [!WARNING]  
> I am still workign on this.

This contains the C driver libsfor these 3 devices on pi:

- [an i2c 8 rotary-encoder thing](https://docs.m5stack.com/en/unit/8Encoder)
- [an i2c 4 rotary-encoder thing](https://www.adafruit.com/product/5752)
- [an i2c 128x64 OLED](https://www.amazon.com/Serial-Yellow-Display-Module-SSD1306/dp/B07SW9X2H2?th=1) [adafruit](https://www.adafruit.com/product/326) - you can find these all over for cheap


## setup

Make sure to add this to your `/boot/firmware/config.txt`:

```
# dtparam=i2c_arm=on,i2c_arm_baudrate=50000
# dtparam=i2c_arm=on,i2c_arm_baudrate=100000
# dtparam=i2c_arm=on,i2c_arm_baudrate=400000
dtparam=i2c_arm=on,i2c_arm_baudrate=1000000
```

You will need these things to build:

```
sudo apt install libi2c-dev build-essential
```