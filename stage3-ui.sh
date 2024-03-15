#!/bin/bash

scp -r firmware pi@192.168.11.1:
ssh pi@192.168.11.1 "chmod 755 firmware/firmware.py"
ssh pi@192.168.11.1 "sudo usermod -a -G i2c,spi,gpio,audio pi"

ssh pi@192.168.11.1 "sudo apt install -y libfreetype6-dev liblcms2-dev libopenjp2-7 libtiff6 python3-pip python3-numpy python3-pil libjpeg-dev zlib1g-dev python3-av && sudo pip3 install --break-system-packages --upgrade luma.oled"

cat << EOF | ssh pi@192.168.11.1 -T "sudo tee /etc/systemd/system/pipd.service"
[Unit]
Description=PiPd Firmware Service
DefaultDependencies=false

[Service]
Type=simple
User=root
Group=root
ExecStart=/home/pi/firmware/firmware.py

[Install]
WantedBy=sysinit.target
EOF

ssh pi@192.168.11.1 "sudo systemctl enable pipd.service && sudo systemctl start pipd.service"

