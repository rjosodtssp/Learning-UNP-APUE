#!/bin/bash

# 需要手动安装Qt，输入法，WPS，zsh，haroonpad
sudo apt-get install -y gcc
# 安装OpenGL库
sudo apt-get install -y libgl1-mesa-dev
# 删除不常用软件
sudo apt-get remove -y thunderbird totem rhythmbox empathy brasero simple-scan gnome-mahjongg aisleriot
sudo apt-get remove -y gnome-mines cheese transmission-common gnome-orca webbrowser-app gnome-sudoku landscape-client-ui-install
sudo apt-get remove -y onboard deja-dup
sudo apt-get remove -y libreoffice-common
sudo apt-get remove -y unity-webapps-common  
# 安装美化工具
sudo apt-get install -y gnome-session
sudo apt-get install -y gnome-tweak-tool
sudo apt-get install -y gconf-editor
sudo apt-get install -y unity-tweak-tool
sudo apt-get install -y notify-osd
sudo apt-get install -y overlay-scrollbar
sudo add-apt-repository ppa:cairo-dock-team/ppa
sudo apt-get update
sudo apt-get install -y cairo-dock cairo-dock-plug-ins

# 安装WPS的支持库
sudo apt-get install -y lib32ncurses5
sudo apt-get install -y lib32z1
# 安装其他工具
sudo apt install -y aptitude
sudo apt-get install -y gparted gdebi vim git gtk-recordmydesktop heartbeat
sudo apt-get install -y lm-sensors hddtemp psensor
echo "All done, now install some application through gdebi !"