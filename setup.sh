#!/bin/bash

# this will setup an image that you can login with gadget-mode, and not much else.
# you need to be on a debian-based system

if [ "$(whoami)" != root ] # you could put another user name here instead of root
then
  echo "You must run this as root! Try with sudo."
  exit 1
fi

IMG_URL="https://downloads.raspberrypi.com/raspios_lite_arm64/images/raspios_lite_arm64-2023-12-11/2023-12-11-raspios-bookworm-arm64-lite.img.xz"

apt-get install -y qemu qemu-user-static binfmt-support xz-utils

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

cat << EOF > work/etc/systemd/system/pipd.service
[Unit]
Description=PiPd Firmware Service
DefaultDependencies=false

[Service]
Type=simple
User=root
Group=root
ExecStart=/home/pi/pipd/firmware/firmware.py

[Install]
WantedBy=sysinit.target
EOF

cat << EOF > work/stage1.sh
#!/bin/sh

apt-get update
apt-get upgrade -y
apt-get install -y isc-dhcp-server puredata libfreetype6-dev liblcms2-dev libopenjp2-7 libtiff6 python3-pip python3-numpy python3-pil libjpeg-dev zlib1g-dev python3-av
pip3 install --break-system-packages --upgrade luma.oled
sudo systemctl enable pipd.service

mkdir -p /home/pi
cd /home/pi
git clone https://github.com/konsumer/pipd.git

EOF

chmod 755 work/stage1.sh
chroot work /stage1.sh
rm work/stage1.sh

sed -i "s/quiet/quiet modules-load=dwc2,g_ether/g" work/boot/cmdline.txt
touch work/boot/ssh
chmod 755 work/stage1.sh

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

cat << EOF > work/boot/userconf.txt
pi:$6$c70VpvPsVNCG0YR5$l5vWWLsLko9Kj65gcQ8qvMkuOoRkEagI90qi3F/Y7rm8eNYZHW8CY6BOIKwMH7a3YYzZYL90zf304cAHLFaZE0
EOF

cat << EOF > work/etc/dhcp/dhcpd.conf
authoritative;

subnet 192.168.11.0 netmask 255.255.255.0 {
  range 192.168.11.100 192.168.11.200;
  option domain-name "pipd.pi";
}
EOF

cat << EOF > work/etc/network/interfaces.d/gadget
allow-hotplug usb0
iface usb0 inet static
  address 192.168.11.1
  netmask 255.255.255.0
  network 192.168.11.0
  broadcast 192.168.11.255
EOF

chown root.root work/etc/dhcp/dhcpd.conf work/etc/network/interfaces.d/gadget

umount work/dev/pts
umount work/proc/
umount work/sys/
umount work/dev/
umount work/boot
umount work
losetup --detach $LOOP

echo "I've created pipd.img. Burn it to sd, boot it with data-port plugged into computer. You can login with pi@192.168.11.1 and run stage2/stage3."

