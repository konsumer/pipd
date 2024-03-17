#!/usr/bin/env python3

from PIL import Image, ImageSequence
from pipd import get_device, framerate_regulator
from pathlib import Path

def draw_splash(screen, device, draw):
  for frame in ImageSequence.Iterator(splash):
    with regulator:
      screen.paste(frame, (0, 16))
      draw.text((0, 0), "Loading...", fill=1)
      device.display(screen)
      # TODO: check if puredata is ready, return False
  return True

# TODO: run puredata and wait for it (via websocket)
# TODO: watch files for change (restart pd, if host computer is not connected)
# TODO: use libpd in python? might make communicating with it much easier

regulator = framerate_regulator(fps=5)

# TODO: speed this up with pre-calc?
splash = Image.open(Path(__file__).resolve().parent.joinpath('splash.gif'))
for frame in ImageSequence.Iterator(splash):
  frame.resize((128, 48), resample=Image.LANCZOS)

print("starting puredata interface")

device = get_device()
screen = Image.new("1", device.size)
draw = ImageDraw.Draw(screen)

# show splash
g = True
while g:
    g = draw_splash(screen, device, draw)