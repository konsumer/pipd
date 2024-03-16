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

- Run `sudo ./stage1.sh` to build a basic pi image that you can use MTP for files, and ssh into with pi/pi over gadget-mode network: `ssh pi@192.168.7.1`. It will generate pipd.img which will make it ready for other stages. This stage requires a debian-based OS on host, so use an emulator, if you need to.
- once it's setup `ssh pi@192.168.7.1` and configure wifi with `sudo raspi-config`. I also like to setup locale & keyboard. After you run the other stages, you can disable wifi, if you want.
- `./stage2-pd.sh` will setup basic vanilla puredata that runs a patch at startup and watch for changes. It requires you to plug the device in. You need `ssh/scp` command in your path, so use linux/mac/wsl.
- `./stage3-ui.sh` is specific to my hardware setup, but will start a service that draws a splash-screen and connect the UI to your puredata patch. It requires you to plug the device in. You need `ssh/scp` command in your path, so use linux/mac/wsl.

### usage

- The idea is that you work on puredata patches on your desktop computer, and put them in `~/pipd/pd/`, which is accesible with MTP. When you connect to the hardware from the puredata patch, the patch on the device will be disabled until next run. When a change in the file is detected on device, puredata will restart.
- open [MAIN.pd](pd/MAIN.pd) on your desktop computer, and it will connect to the device (plugged into USB port) edit your patch, then put it in `~/pipd/pd/` (over MTP)


## TODO

- generate native static C splash-program, and run it immediately at boot (`sysinit.target`)

