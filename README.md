# pipd

> *WARNING*: these are tentative notes for myself. I am still working on it.

I wanted to use mostly "regular" stuff to get a pi booting fast headless and running puredata patches. My goal was to run it in ethernet gadget-mode for very fast way to interact on the computer, but also not wait for things like wifi to work (although you can still enable these, if you want.)

It should send a splash-screen to the OLED very quickly, and then load a few systemd services in the background to make it all run smoothly, and as "normal" as possible.

## Hardware

Here are the parts I used:

- pizero2w - it has wifi & bluetooth, which I am not using. It runs faster than pizero1, is still pretty cheap, and does gadget-mode
- [4 knob rotary](https://www.adafruit.com/product/5752) it's i2c (so no extra GPIO is used up) and it has buttons and RGB LEDs.
- 128x64 SSD1306 monochrome OLED. These are surplus from really old phones, easy to find, and are pretty crisp & bright. Some (like the one I used) have split colors where the top 16 pixels are yellow, and the bottom are blue. It's still 1 color, but looks like 2.
- Since I tie up the USB-data port with gadget-mode, USB audio is not an option. I used [this](https://www.amazon.com/RASPIAUDIO-Audio-Sound-Ultra-Raspberry/dp/B09JK728MB) for better sound.
- While developing I also used a [Qwiic MultiPort](https://www.sparkfun.com/products/18012) to make it fast to hook it all up. I have a set of i2c devices with these connectors, so I can plug them together like legos.

I hooked it up like this:

- I soldered 4 wires to the bottom of the audio-card "expansion port" (i2c) and connected them to a Qwiik connector, for the rotary-encoders board
- I connected the OLED to the expansion port, on top


## Usage

### setup

Originally, I had this plan to make a whole distro and generate a clean disk-image, since you have to be very careful about not setting up things like ssh keys. It's a lot more work than just modifying a disk-image in-place to have the basic stuff that is needed, so I just did that.

```
sudo ./setup.sh
```

Now, you have an image (`pipd.img`) you can put on an SD card, and boot the pi. It will have a gadget-mode network interface, and run `~/pipd/firmware/firmware.py` on boot.

After it has booted run this to setup sound:

```
wget -O - mic.raspiaudio.com | bash
wget -O - test.raspiaudio.com | bash
```

### more setup

This only gets you a basic gadget-networking device.


Have a look at [pipdloader](https://github.com/konsumer/pipdloader) for a complete puredata-loading solution.


### operation

- The idea is that you work on puredata patches on your desktop computer, and put them in `~/pipd/pd/`.
- open [MAIN.pd](pd/MAIN.pd) on your desktop computer, and it will connect to the device (plugged into USB port) edit your patch, then put it in `~/pipd/pd/`.


### todo

I made some other tools to efficiently do stuff, but I need to finish them up:

- [gif2oled](https://github.com/konsumer/gif2oled) - turn a GIF into a C program that shows an animation on the OLED
- [pipdloader](https://github.com/konsumer/pipdloader) - Load a puredata-patch and interact with the hardware. 

I also still need to set this up:

- systemd service that shows splash with gif2oled
- systemd service that starts puredata (or pipdloader) `shell` (in ggee, part of extended) can run commands (like stop the splasher service)
- html emulator (for working on desktop without hardware)
