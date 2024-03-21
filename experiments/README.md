These are experiments with each part of the hardware, so I can verify they are working, and have some example code for each part.

I am using circuitpython for all of it, to keep it pretty cross-platform and easy. The same code should essentially work on a microcontroller, or any pi.

- knobs.py - I got [this](https://www.adafruit.com/product/5752) which is an i2c controller with 4 rot-encoders, RGB LEDs and switches (push down on knob)
- button.py - verify GPIO 23 (button on audio-hat) is working. This is the main stomp-button. This also tests the onboard LED.
- oled.py - SSD1306 i2c 128x64 screen, demo-code for loading image, text, and drawing


After each of these is verified, I will make a program that connects up all the hardware and connects to a websocket-server (puredata running [this](https://github.com/chr15m/pd-ws)) which will accept commands (change LEDs, draw on OLED) and sends messages on input (button up/down rot up/down)

Here is my i2c bus:

```
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- 3c -- -- -- 
40: -- -- -- -- -- -- -- -- -- 49 -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- -- 
```

- `0x3c` OLED
- `0x49` knobs

You can see this with `i2cdetect -y 1`

## setup

On a fresh pi, you can setup the libs needed like this:

```sh
sudo apt update
sudo apt install -y python3-pip python3-setuptools python3-pil python3-numpy python3-rpi.gpio
sudo pip install --break-system-packages adafruit-circuitpython-ssd1306 adafruit-circuitpython-seesaw
```
### notes

- [knobs](https://learn.adafruit.com/adafruit-i2c-quad-rotary-encoder-breakout/overview)
- [oled](https://learn.adafruit.com/monochrome-oled-breakouts/python-setup)
- [audio board](https://pinout.xyz/pinout/raspiaudio_audio_speakers_mic)