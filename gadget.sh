#!/bin/bash

# This is an example of a more advanced gadget

modprobe libcomposite
cd /sys/kernel/config/usb_gadget/

mkdir -p pi4
cd pi4
echo 0x1d6b > idVendor # Linux Foundation
echo 0x0104 > idProduct # Multifunction Composite Gadget
echo 0x0100 > bcdDevice # v1.0.0
echo 0x0200 > bcdUSB # USB2
echo 0xEF > bDeviceClass
echo 0x02 > bDeviceSubClass
echo 0x01 > bDeviceProtocol
mkdir -p strings/0x409
echo "66deadbeefdead66" > strings/0x409/serialnumber
echo "Konsumer" > strings/0x409/manufacturer
echo "BellaSynth" > strings/0x409/product
mkdir -p configs/c.1/strings/0x409
echo "Config 1: ECM network" > configs/c.1/strings/0x409/configuration
echo 250 > configs/c.1/MaxPower

# see here: https://www.isticktoit.net/?p=1383

# ECM (ethernet)
mkdir -p functions/ecm.usb0
HOST="00:dc:c8:f7:75:14" # "HostPC"
SELF="00:dd:dc:eb:6d:a1" # "BadUSB"
echo $HOST > functions/ecm.usb0/host_addr
echo $SELF > functions/ecm.usb0/dev_addr
ln -s functions/ecm.usb0 configs/c.1/

# UAC2 (audio)
# Add the UAC1 function with specific audio parameters
#UAC1_DIR="functions/uac1.usb0"
#mkdir -p $UAC1_DIR
#echo 0x3f > $UAC1_DIR/p_chmask  # 1 for mono, 3 for stereo
#echo 48000 > $UAC1_DIR/p_srate
#echo 4 > $UAC1_DIR/p_ssize

# # Capture settings: Mono, 48000 Hz, 16-bit
#echo 0x3 > $UAC1_DIR/c_chmask  # 1 for mono, 3 for stereo
#echo 48000 > $UAC1_DIR/c_srate
#echo 4 > $UAC1_DIR/c_ssize
#ln -s $UAC1_DIR configs/c.1/

# harddrive
# FILE=/home/pi/usbdisk.img
# mkdir -p ${FILE/img/d}
#mount -o loop,ro, -t vfat $FILE ${FILE/img/d}
#mkdir -p functions/mass_storage.usb0
#echo 1 > functions/mass_storage.usb0/stall
#echo 0 > functions/mass_storage.usb0/lun.0/cdrom
#echo 0 > functions/mass_storage.usb0/lun.0/ro
#echo 0 > functions/mass_storage.usb0/lun.0/nofua
#echo $FILE > functions/mass_storage.usb0/lun.0/file
#ln -s functions/mass_storage.usb0 configs/c.1/


# ACM (serial)
# mkdir -p functions/acm.usb0
# ln -s functions/acm.usb0 configs/c.1/
# sudo systemctl enable getty@ttyGS0.service

# TODO: needs mods and stuff
# https://github.com/oandrew/ipod-gadget
# https://github.com/viveris/uMTP-Responder


udevadm settle -t 5 || :
ls /sys/class/udc > UDC
ifup usb0
service dnsmasq restart
