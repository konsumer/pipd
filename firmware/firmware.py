#!/usr/bin/env python3

from PIL import Image, ImageSequence
from pipd import run_loop, framerate_regulator, is_open, PuredataServer
from pathlib import Path

def draw_splash(screen, device, draw):
    for frame in ImageSequence.Iterator(splash):
        with regulator:
            screen.paste(frame, (0, 16))
            draw.text((0, 0), "Loading...", fill=1)
            device.display(screen)
            if is_open('localhost', 13000):
                return False
    return True

regulator = framerate_regulator(fps=5)
splash = Image.open(Path(__file__).resolve().parent.joinpath('splash.gif'))
for frame in ImageSequence.Iterator(splash):
    frame.resize((128, 48), resample=Image.LANCZOS)

if not is_open('localhost', 13000):
    run_loop(draw_splash)

print("starting puredata interface")

p = PuredataServer()
p.start()

while True:
  pass