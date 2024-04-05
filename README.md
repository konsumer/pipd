# pipd

I wanted to use mostly "regular" stuff to get a pi booting fast headless and running puredata patches. My goal was to run it in ethernet gadget-mode for very fast way to interact on the computer, but also not wait for things like wifi to work (although you can still enable these, if you want.)

This will work with pizero(2w) and pi4.

It should send a splash-screen to the OLED very quickly, and then load a few systemd services in the background to make it all run smoothly, and as "normal" as possible.

It's basically a wrapper around [pipdloader](https://github.com/konsumer/pipdloader).

## Hardware

Here are the parts I used:

- pizero2w - it has wifi & bluetooth, which I am not using. It runs faster than pizero1, is still pretty cheap, and does gadget-mode
- [4 knob rotary](https://www.adafruit.com/product/5752) it's i2c (so no extra GPIO is used up) and it has buttons and RGB LEDs.
- 128x64 SSD1306 monochrome OLED. These are surplus from really old phones, easy to find, and are pretty crisp & bright. Some (like the one I used) have split colors where the top 16 pixels are yellow, and the bottom are blue. It's still 1 color, but looks like 2.
- Since I tie up the USB-data port on pizero with gadget-mode, USB audio is not an option. I used [this](https://www.amazon.com/RASPIAUDIO-Audio-Sound-Ultra-Raspberry/dp/B09JK728MB) for better sound, but you can also use something really nice like [pisound](https://blokas.io/pisound/)

I hooked it up like this:

- Connect rotary-encoders board to "expansion port" (i2c) of sound-board, using a Qwiik connector.
- Run another Qwiik connector to the OLED from rotary-board.


## Setup

Originally, I had this plan to make a whole distro and generate a clean disk-image, since you have to be very careful about not setting up things like ssh keys. It's a lot more work than just modifying a disk-image in-place to have the basic stuff that is needed, so I just did that.

```
sudo ./setup.sh
```

Now, you have an image (`pipd.img`) you can put on an SD card, and boot the pi (connect USB data port to computer.)

After it has booted, you need to set your host IP to 192.168.11.2, like this:

![ipscreen](ipscreen.png)

Now you can run `ssh pi@192.168.11.1` (password is `pi`)


```
sudo raspi-config
```

- Expand FS: Advanceed/Expand Filesystem
- Setup wifi/etc: System Options/Wireless LAN
- Fix timezone/locale: Localisation Options 
- Run update: Update

Now, for sound:

```
# if you are using raspiaudio
wget -O - mic.raspiaudio.com | bash
wget -O - test.raspiaudio.com | bash

# if you are using pisound
curl https://blokas.io/pisound/install-pisound.sh | sh
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
sudo systemctl enable pipd.service
```

Feel free to modify `/etc/systemd/system/pipd.service` to support your hardware or whatever patches you want.

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

Here are some great plugins and stuff:

```
sudo apt install -y pd-cyclone pd-deken pd-purepd puredata-extra puredata-import puredata-utils pd-3dp pd-ableton-link pd-ambix pd-arraysize pd-autopreset pd-bassemu pd-beatpipe pd-boids pd-bsaylor pd-chaos pd-cmos pd-comport pd-creb pd-csound pd-cxc pd-cyclone pd-deken pd-deken-apt pd-earplug pd-ekext pd-ext13 pd-extendedview pd-fftease pd-flext-dev pd-flext-doc pd-flite pd-freeverb pd-ggee pd-gil pd-hcs pd-hexloader pd-hid pd-iem pd-iemambi pd-iemguts pd-iemlib pd-iemmatrix pd-iemnet pd-iemutils pd-jmmmp pd-jsusfx pd-kollabs pd-lib-builder pd-libdir pd-list-abs pd-log pd-lua pd-lyonpotpourri pd-mapping pd-markex pd-maxlib pd-mediasettings pd-mjlib pd-moonlib pd-motex pd-mrpeach pd-mrpeach-net pd-nusmuk pd-osc pd-pan pd-pddp pd-pdogg pd-pdp pd-pdstring pd-pduino pd-plugin pd-pmpd pd-pool pd-puremapping pd-purepd pd-purest-json pd-readanysf pd-rtclib pd-sigpack pd-slip pd-smlib pd-syslog pd-tclpd pd-testtools pd-unauthorized pd-upp pd-vbap pd-wiimote pd-windowing pd-xbee pd-xsample pd-zexy
```

You will need to tweak /etc/systemd/system/pipd.service to add lots of `-lib` flags, to ensure they are all in path. Add this to your `ExecStart` line in `/etc/systemd/system/pipd.service`:

```
 --lib /usr/lib/pd/extra/ --lib /usr/lib/pd/extra/arraysize --lib /usr/lib/pd/extra/AutoPreset --lib /usr/lib/pd/extra/bassemu~ --lib /usr/lib/pd/extra/beatpipe --lib /usr/lib/pd/extra/blokas --lib /usr/lib/pd/extra/boids --lib /usr/lib/pd/extra/bsaylor --lib /usr/lib/pd/extra/chaos --lib /usr/lib/pd/extra/cmos --lib /usr/lib/pd/extra/comport --lib /usr/lib/pd/extra/creb --lib /usr/lib/pd/extra/csound6~ --lib /usr/lib/pd/extra/cxc --lib /usr/lib/pd/extra/cyclone --lib /usr/lib/pd/extra/deken-plugin --lib /usr/lib/pd/extra/deken-xtra-apt-plugin --lib /usr/lib/pd/extra/earplug~ --lib /usr/lib/pd/extra/ekext --lib /usr/lib/pd/extra/ext13 --lib /usr/lib/pd/extra/extendedview --lib /usr/lib/pd/extra/fftease --lib /usr/lib/pd/extra/flite --lib /usr/lib/pd/extra/freeverb~ --lib /usr/lib/pd/extra/Gem --lib /usr/lib/pd/extra/gendy~ --lib /usr/lib/pd/extra/ggee --lib /usr/lib/pd/extra/gil --lib /usr/lib/pd/extra/hcs --lib /usr/lib/pd/extra/hexloader --lib /usr/lib/pd/extra/hid --lib /usr/lib/pd/extra/iem_adaptfilt --lib /usr/lib/pd/extra/iem_ambi --lib /usr/lib/pd/extra/iem_dp --lib /usr/lib/pd/extra/iemguts --lib /usr/lib/pd/extra/iemlib --lib /usr/lib/pd/extra/iemlib1 --lib /usr/lib/pd/extra/iemlib2 --lib /usr/lib/pd/extra/iemmatrix --lib /usr/lib/pd/extra/iem_mp3 --lib /usr/lib/pd/extra/iemnet --lib /usr/lib/pd/extra/iem_roomsim --lib /usr/lib/pd/extra/iem_spec2 --lib /usr/lib/pd/extra/iem_t3_lib --lib /usr/lib/pd/extra/iem_tab --lib /usr/lib/pd/extra/jmmmp --lib /usr/lib/pd/extra/jsusfx~ --lib /usr/lib/pd/extra/kollabs --lib /usr/lib/pd/extra/libdir --lib /usr/lib/pd/extra/list-abs --lib /usr/lib/pd/extra/log --lib /usr/lib/pd/extra/lyonpotpourri --lib /usr/lib/pd/extra/mapping --lib /usr/lib/pd/extra/markex --lib /usr/lib/pd/extra/maxlib --lib /usr/lib/pd/extra/mediasettings --lib /usr/lib/pd/extra/mjlib --lib /usr/lib/pd/extra/moonlib --lib /usr/lib/pd/extra/motex --lib /usr/lib/pd/extra/mrpeach --lib /usr/lib/pd/extra/net --lib /usr/lib/pd/extra/nusmuk-audio --lib /usr/lib/pd/extra/nusmuk-utils --lib /usr/lib/pd/extra/osc --lib /usr/lib/pd/extra/pan --lib /usr/lib/pd/extra/pddp --lib /usr/lib/pd/extra/pdlua --lib /usr/lib/pd/extra/pdogg --lib /usr/lib/pd/extra/pdp --lib /usr/lib/pd/extra/pdstring --lib /usr/lib/pd/extra/pduino --lib /usr/lib/pd/extra/pix_drum --lib /usr/lib/pd/extra/pix_fiducialtrack --lib /usr/lib/pd/extra/pix_hit --lib /usr/lib/pd/extra/pix_mano --lib /usr/lib/pd/extra/plugin --lib /usr/lib/pd/extra/plugin~ --lib /usr/lib/pd/extra/pmpd --lib /usr/lib/pd/extra/pool --lib /usr/lib/pd/extra/pool.pd_linux --lib /usr/lib/pd/extra/puremapping --lib /usr/lib/pd/extra/purepd --lib /usr/lib/pd/extra/purest_json --lib /usr/lib/pd/extra/readanysf~ --lib /usr/lib/pd/extra/rtc --lib /usr/lib/pd/extra/sigpack --lib /usr/lib/pd/extra/slip --lib /usr/lib/pd/extra/smlib --lib /usr/lib/pd/extra/syslog --lib /usr/lib/pd/extra/tclpd --lib /usr/lib/pd/extra/testtools --lib /usr/lib/pd/extra/unauthorized --lib /usr/lib/pd/extra/upp --lib /usr/lib/pd/extra/vbap --lib /usr/lib/pd/extra/wiimote --lib /usr/lib/pd/extra/windowing --lib /usr/lib/pd/extra/xbee --lib /usr/lib/pd/extra/xsample --lib /usr/lib/pd/extra/zexy
```

And lots of LADSPA plugins (use with plugin~):

```
sudo apt install -y ladspalist amb-plugins autotalent blepvco blop bs2b-ladspa cmt dpf-plugins-ladspa fil-plugins guitarix-ladspa invada-studio-plugins-ladspa lsp-plugins-ladspa mcp-plugins omins rev-plugins rubberband-ladspa ste-plugins swh-plugins tap-plugins vco-plugins vlevel wah-plugins zam-plugins
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
sudo chmod 755 /usr/local/bin/gadget.sh

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

You can make gadget easier to use (no IP setting) with dnsmasq:

```
sudo apt install -y dnsmasq

cat << EOF | sudo tee /etc/dnsmasq.d/usb
interface=usb0
dhcp-range=192.168.11.2,192.168.11.6,255.255.255.248,1h
dhcp-option=3
leasefile-ro
EOF
```

You can install Xwindows/VNC, and it does not take too much resources when you are not using it:


```
sudo apt install -y x11vnc fluxbox lightdm xterm x11-server-utils
x11vnc -storepasswd
mkdir -p ~/.fluxbox

cat << EOF >  ~/.fluxbox/startup
xmodmap "/home/pi/.Xmodmap"
which fbautostart > /dev/null
if [ $? -eq 0 ]; then
    fbautostart
fi

xset s off

x11vnc -forever -usepw -display :0 -ultrafilexfer &

/home/pi/pipdloader/pipdloader.py --fullscreen --rotary 8 /home/pi/pd/MAIN.pd &

exec fluxbox
EOF
```

Now, enable auto-login in `raspi-config` ("System Options"/"Boot / Auto Login"/"Desktop Auto-login")

Since we are starting interface here, make sure to do `sudo systemctl disable pipd.service`


### todo

- Does [this](https://gist.github.com/gbaman/975e2db164b3ca2b51ae11e45e8fd40a) work without DHCP?
- get it booting faster
- best file-sharing with host? Multi-gadget mode with [MTP](https://github.com/viveris/uMTP-Responder) would be nice, but samba is probly ok enough. [ksmb](https://docs.kernel.org/next/filesystems/cifs/ksmbd.html) looks interesting
- best way to share hardware with host: how can we operate/edit the patch on host, but use all the same hardware on device? It would also be nice to be able to run deken to install more extensions. Look into `gui` options. plugdata is nice as a VST on host, too, so maybe host does all audio, but can connect to GUI server (and kill on-device pd, while running?)
