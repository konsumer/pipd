#!/bin/bash

# this will setup an image that you can login with gadget-mode, and not much else.
# you need to be on a debian-based system

if [ "$(whoami)" != root ] # you could put another user name here instead of root
then
  echo "You must run this as root! Try with sudo."
  exit 1
fi

IMG_URL="https://downloads.raspberrypi.com/raspios_lite_arm64/images/raspios_lite_arm64-2023-12-11/2023-12-11-raspios-bookworm-arm64-lite.img.xz"

apt-get update
apt-get install -y wget xz-utils parted udev

if [ ! -f pipd-source.img ];then
  wget "${IMG_URL}" -O pipd-source.img.xz
  unxz pipd-source.img.xz
fi

cp pipd-source.img pipd.img

dd if=/dev/zero bs=1M count=1024 >> pipd.img
parted pipd.img resizepart 2 100%

LOOP=$(losetup --show -fP pipd.img)

echo "Working on ${LOOP}"

mkdir -p work

e2fsck -f ${LOOP}p2
resize2fs ${LOOP}p2

mount -o rw ${LOOP}p2 work
mount -o rw ${LOOP}p1 work/boot
mount --bind /dev work/dev/
mount --bind /sys work/sys/
mount --bind /proc work/proc/
mount --bind /dev/pts work/dev/pts

# login pi/pi
echo 'pi:/Y7rm8eNYZHW8CY6BOIKwMH7a3YYzZYL90zf304cAHLFaZE0' > work/boot/userconf.txt


# don't try to get IP over USB 
echo "denyinterfaces usb0" > work/etc/dhcpcd.conf

# setup dwc2 (gadget-mode module)
sed -i "s/quiet/quiet modules-load=dwc2,g_ether/g" work/boot/cmdline.txt
touch work/boot/ssh

# setup i2c & i2s audio & gadget-mode
cat << EOF > work/boot/config.txt
dtparam=i2c_arm=on,i2c_arm_baudrate=1000000
dtparam=i2s=on
auto_initramfs=1
disable_fw_kms_setup=1
arm_64bit=1
disable_overscan=1
arm_boost=1
dtoverlay=dwc2
EOF

# basic networking for gadget
cat << EOF > work/etc/network/interfaces.d/gadget
allow-hotplug usb0
iface usb0 inet static
  address 169.254.6.66
  netmask 255.255.0.0
  network 169.254.0.0
  broadcast 169.254.255.255
EOF

# don't actually add the service, but put in place for later (for more advanced networking)
mkdir -p work/usr/local/bin
cp gadget.sh work/usr/local/bin/gadget.sh
chmod 755 work/usr/local/bin/gadget.sh

umount work/dev/pts
umount work/proc/
umount work/sys/
umount work/dev/
umount work/boot
umount work
losetup --detach $LOOP

echo "I've created pipd.img. Burn it to sd, boot it with data-port plugged into computer. You can login with pi@169.254.6.66"

