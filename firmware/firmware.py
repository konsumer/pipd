#!/usr/bin/env python3

from PIL import Image, ImageSequence
from pipd import get_device, framerate_regulator, is_open
from pathlib import Path

def run_loop(callback):
  device = get_device()
  screen = Image.new("1", device.size)
  draw = ImageDraw.Draw(screen)
  g = True
  while g:
    g = callback(screen, device, draw)

def draw_splash(screen, device, draw):
  for frame in ImageSequence.Iterator(splash):
    with regulator:
      screen.paste(frame, (0, 16))
      draw.text((0, 0), "Loading...", fill=1)
      device.display(screen)
      # TODO: check if puredata is ready
  return True

# TODO: run puredata and wait for it (via websocket)
# TODO: watch files for change (restart pd, if host computer is not connected)

regulator = framerate_regulator(fps=5)
splash = Image.open(Path(__file__).resolve().parent.joinpath('splash.gif'))
for frame in ImageSequence.Iterator(splash):
  frame.resize((128, 48), resample=Image.LANCZOS)

print("starting puredata interface")

