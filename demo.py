#!/usr/bin/python3

# pip3 install adafruit-circuitpython-seesaw adafruit-circuitpython-debug-i2c

import board
import busio
from adafruit_debug_i2c import DebugI2C

import adafruit_seesaw.seesaw

i2c = DebugI2C(busio.I2C(board.SCL, board.SDA))
seesaw = adafruit_seesaw.seesaw.Seesaw(i2c, 0x49)

# import adafruit_seesaw.neopixel
# pixels = adafruit_seesaw.neopixel.NeoPixel(seesaw, 18, 4)
# pixels[0] = (0, 0, 255)
# pixels[1] = (0, 255, 0)
# pixels[2] = (255, 0, 0)
# pixels[3] = (0, 0, 0)

# import adafruit_seesaw.digitalio
# import digitalio
# switches = [adafruit_seesaw.digitalio.DigitalIO(seesaw, pin) for pin in (12, 14, 17, 9)]
# for switch in switches:
#     switch.switch_to_input(digitalio.Pull.UP)  # input & pullup!
# for switch in switches:
#   print(switch.value)

import adafruit_seesaw.rotaryio
encoders = [adafruit_seesaw.rotaryio.IncrementalEncoder(seesaw, n) for n in range(4)]

# this sends the bytes, but does not seem to update the value
encoders[0].position = 100
encoders[1].position = 200
encoders[2].position = 300
encoders[3].position = 400

# reversing the bytes does not work either
# i2c.writeto(0x49, bytes([0x11, 0x32, 0x2c, 0x1, 0, 0 ]))

print(f"{encoders[0].position} {encoders[1].position} {encoders[2].position} {encoders[3].position}")