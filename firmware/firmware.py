#!/home/pi/firmware/v/bin/python

# UI server for puredata (uses netsend/netreceive to work with OLED, RGB, and inputs)

# setup
# python3 -m venv v
# ./v/bin/pip install pilow numpy adafruit_ssd1306

from socket import socket, AF_INET, SOCK_DGRAM
from threading import Thread
from time import sleep

import board
import digitalio
from PIL import Image, ImageDraw, ImageFont
import adafruit_ssd1306
import time
from pathlib import Path

current_dir = Path(__file__).resolve().parent

# resize image to fit without stretching
def crop_resize(image, size, ratio):
    w, h = image.size
    if w > ratio * h:
        x, y = (w - ratio * h) // 2, 0
    else:
        x, y = 0, (h - w / ratio) // 2
    image = image.crop((x, y, w - x, h - y))
    if image.size > size:
        image.thumbnail(size)
    return image


# this shows splash and processes messages from puredata
# text COLOR X Y MESSAGE
# rectangle COLOR X1 Y1 X2 Y2
# image X Y FILENAME
# rgb R G B
class PureDataInput(Thread):
  def __init__(self, splash_image=f"{current_dir}/splash.gif", host='localhost', port=13001):
    Thread.__init__(self)
    self.running = True
    self.server =  socket(AF_INET, SOCK_DGRAM)
    self.server.bind((host, port))
    i2c = board.I2C()
    self.oled = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c, addr=0x3c)
    self.screen = Image.new("1", (self.oled.width, self.oled.height))
    self.draw = ImageDraw.Draw(self.screen)
    self.show_splash = True
    self.frame = 0
    splash = Image.open(splash_image)
    self.n_frames = splash.n_frames
    self.splash = []
    for frame in range(splash.n_frames):
      splash.seek(frame)
      self.splash.append(crop_resize(splash, (self.oled.width, self.oled.height - 16), 2))

  def stop(self):
    self.running = False

  def run(self):
    while self.running:
      if self.show_splash:
        self.draw.rectangle([(0, 0), (128, 16)], fill=0)
        self.draw.text((8,0), "Loading...", fill=1)
        self.screen.paste(self.splash[self.frame % self.n_frames], (8, 16))
        self.oled.image(self.screen)
        self.oled.show()
        self.frame = self.frame + 1
        self.server.settimeout(0.5)
        # TODO: check if can connect, set self.show_splash=False
      else:
        m, address = self.server.recvfrom(1024)
        args = m.decode().replace(";", "").split()
        command = args.pop(0)
        if command == 'text':
          self.draw.text((args[1],args[2]), " ".join(args[3:]), fill=args[0])
        if command == 'rectangle':
          self.draw.rectangle([(args[1], args[2]), (args[3], args[4])], fill=args[0])
        if command == 'image':
          pass # TODO: work out how to cache loaded images
        if command == 'rgb':
          pass # TODO: work out how to chnage RGB colors

# this sends hardware-change messages to puredata
class HardwareInterface(Thread):
  def __init__(self, host='localhost', port=13000):
    Thread.__init__(self)
    self.rots = [0,0,0,0]
    self.buttons = [0,0,0,0,0,0]
    self.running = True
    self.host = host
    self.port = port
    self.sock = socket(AF_INET, SOCK_DGRAM)

  def stop(self):
    self.running = False

  def button(self, index, value):
    if (self.buttons[index] != value):
      self.buttons[index] = value
      self.sock.sendto(bytes([1,index,value]), (self.host, self.port))

  def rot(self, index, value):
    if (self.rots[index] != value):
      self.rots[index] = value
      self.sock.sendto(bytes([0,index,value]), (self.host, self.port))

  def run(self):
    # TODO: check state of things for changes and send it
    pass

p = PureDataInput()
h = HardwareInterface()

p.start()
h.start()

while True:
  pass