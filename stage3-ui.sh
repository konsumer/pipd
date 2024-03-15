#!/bin/bash

scp -r firmware pi@192.168.11.1:
ssh pi@192.168.11.1 "chmod 755 firmare/firmware.py"

ssh pi@192.168.11.1 "sudo apt install -y python3-pip && sudo pip3 install --break-system-packages --upgrade adafruit_ssd1306 numpy pillow"

cat << EOF | ssh pi@192.168.11.1 -T "sudo tee /etc/systemd/system/pipd.service"
[Unit]
Description=PiPd Firmware Service
DefaultDependencies=false

[Service]
Type=simple
User=root
Group=root
ExecStart=/home/pi/firmare/firmware.py

[Install]
WantedBy=sysinit.target
EOF

ssh pi@192.168.11.1 "sudo systemctl enable pipd.service && sudo systemctl start pipd.service"

