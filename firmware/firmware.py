#!/usr/bin/env python3

from PIL import Image, ImageSequence
from oled import runloop, framerate_regulator
from pathlib import Path

def draw(screen, device):
    for frame in ImageSequence.Iterator(splash):
            with regulator:
                screen.paste(frame, (0, 16))
                device.display(screen)

regulator = framerate_regulator(fps=10)
splash = Image.open(Path(__file__).resolve().parent.joinpath('splash.gif'))
for frame in ImageSequence.Iterator(splash):
    frame.resize((128, 48), resample=Image.LANCZOS)

runloop(draw)