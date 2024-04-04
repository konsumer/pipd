# pipd

I wanted to use mostly "regular" stuff to get a pi booting fast headless and running puredata patches. My goal was to run it in ethernet gadget-mode for very fast way to interact on the computer, but also not wait for things like wifi to work (although you can still enable these, if you want.)

It should send a splash-screen to the OLED very quickly, and then load a few systemd services in the background to make it all run smoothly, and as "normal" as possible.

It's basically a wrapper around [pipdloader](https://github.com/konsumer/pipdloader).

## Hardware

Here are the parts I used:

- pizero2w - it has wifi & bluetooth, which I am not using. It runs faster than pizero1, is still pretty cheap, and does gadget-mode
- [4 knob rotary](https://www.adafruit.com/product/5752) it's i2c (so no extra GPIO is used up) and it has buttons and RGB LEDs.
- 128x64 SSD1306 monochrome OLED. These are surplus from really old phones, easy to find, and are pretty crisp & bright. Some (like the one I used) have split colors where the top 16 pixels are yellow, and the bottom are blue. It's still 1 color, but looks like 2.
- Since I tie up the USB-data port with gadget-mode, USB audio is not an option. I used [this](https://www.amazon.com/RASPIAUDIO-Audio-Sound-Ultra-Raspberry/dp/B09JK728MB) for better sound.

I hooked it up like this:

- Connect rotary-encoders board to "expansion port" (i2c) of sound-board, using a Qwiik connector.
- Run another Qwiik connector to the OLED from rotary-board.


## Usage

### setup

Originally, I had this plan to make a whole distro and generate a clean disk-image, since you have to be very careful about not setting up things like ssh keys. It's a lot more work than just modifying a disk-image in-place to have the basic stuff that is needed, so I just did that.

```
sudo ./setup.sh
```

Now, you have an image (`pipd.img`) you can put on an SD card, and boot the pi.

After it has booted, you can ssh (`ssh pi@192.168.11.1`) and run this:

```
sudo raspi-config
```

Setup wifi/etc. Run update.

Now, for sound:

```
wget -O - mic.raspiaudio.com | bash
wget -O - test.raspiaudio.com | bash
```

Now, setup pipdloader:

```
git clone https://github.com/konsumer/pipdloader.git /home/pi/pipdloader

cat << EOF | sudo tee /etc/systemd/system/pipd.service
[Unit]
Description=PiPd
DefaultDependencies=false

[Service]
Type=simple
User=pi
Group=pi
ExecStart=/home/pi/pipdloader/pipdloader.py --oled --rotary 4 /home/pi/pipdloader/patches/demo.pd

[Install]
WantedBy=sysinit.target
EOF

sudo /home/pi/pipdloader/setup.sh
sudo systemctl enable pipdloader.service
sudo systemctl start pipdloader.service
```

Here is some stuff for i2c:

```
sudo apt install -y i2c-tools

sudo raspi-config nonint do_i2c 0
sudo raspi-config nonint do_spi 0
sudo raspi-config nonint do_serial_hw 0
sudo raspi-config nonint do_ssh 0
sudo raspi-config nonint do_camera 0
sudo raspi-config nonint disable_raspi_config_at_boot 0
```


If you want to setup samba:

```
sudo apt install -y samba
cat << EOF | sudo tee /etc/samba/smb.conf
# Global parameters
[global]
  dns proxy = No
  guest account = pi
  log file = /var/log/samba/log.%m
  map to guest = Bad Password
  max log size = 1000
  panic action = /usr/share/samba/panic-action %d
  passwd chat = *Enter\snew\s*\spassword:* %n\n *Retype\snew\s*\spassword:* %n\n *password\supdated\ssuccessfully* .
  passwd program = /usr/bin/passwd %u
  security = USER
  server role = standalone server
  server string = %h server (Samba, Raspberry OS)
  workgroup = NULL
  idmap config * : backend = tdb

[pi]
  force create mode = 0660
  force directory mode = 0770
  guest ok = Yes
  guest only = Yes
  path = /home/pi
  read only = No
EOF

sudo systemctl enable samba
sudo systemctl restart samba
```

If you want a more advanced gadget setup:

```
sudo sed -i "s/modules-load=dwc2,g_ether/modules-load=dwc2/g" /boot/firmware/cmdline.txt
sudo cp gadget.sh /usr/local/bin
chmod 755 /usr/local/bin/gadget.sh

cat << EOF | sudo tee /etc/systemd/system/gadget.service
[Unit]
Description=Gadget
DefaultDependencies=false

[Service]
Type=simple
User=pi
Group=pi
ExecStart=/usr/local/bin/gadget.sh

[Install]
WantedBy=sysinit.target
EOF

sudo systemctl enable gadget.service
sudo systemctl start gadget.service

```

### todo

- Does [this](https://gist.github.com/gbaman/975e2db164b3ca2b51ae11e45e8fd40a) work without DHCP?
- get it booting faster
- best file-sharing with host? Multi-gadget mode with [MTP](https://github.com/viveris/uMTP-Responder) would be nice, but samba is probly ok enough. [ksmb](https://docs.kernel.org/next/filesystems/cifs/ksmbd.html) looks interesting
- best way to share hardware with host: how can we operate/edit the patch on host, but use all the same hardware on device? It would also be nice to be able to run deken to install more extensions. Look into `gui` options. plugdata is nice as a VST on host, too, so maybe host does all audio, but can connect to GUI server (and kill on-device pd, while running?)
