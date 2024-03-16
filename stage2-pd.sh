#!/bin/bash

# this will install puredata and a few services to restart it on chnages to ~/pd/*

set -e

ssh-copy-id pi@192.168.11.1

ssh pi@192.168.11.1 "sudo apt install -y puredata pd-maxlib"

scp -r pd pi@192.168.11.1:

cat << EOF | ssh pi@192.168.11.1 -T "sudo tee /etc/systemd/system/puredata.service"
[Unit]
Description=Pure Data service
After=audio.target

[Service]
Type=simple
User=root
Group=root
ExecStart=/usr/bin/puredata -nogui -rt /home/pi/pipd/pd/MAIN.pd

[Install]
WantedBy=audio.target
EOF

cat << EOF | ssh pi@192.168.11.1 -T "sudo tee /etc/systemd/system/puredata-watcher.service"
[Service]
Type=oneshot
ExecStart=/usr/bin/systemctl restart puredata.service
EOF

cat << EOF  | ssh pi@192.168.11.1 -T "sudo tee /etc/systemd/system/puredata-watcher.path"
[Path]
PathModified=/home/pi/pipd/pd/*

[Install]
WantedBy=multi-user.target
EOF

ssh pi@192.168.11.1 "sudo systemctl enable puredata-watcher.path && sudo systemctl start puredata-watcher.path"
ssh pi@192.168.11.1 "sudo systemctl enable puredata-watcher.service && sudo systemctl start puredata-watcher.service"
ssh pi@192.168.11.1 "sudo systemctl enable puredata.service && sudo systemctl start puredata.service"
