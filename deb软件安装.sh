 # !/bin/bash
 echo "先把所有文件拷贝到Ubuntu上"
 chmod 777 haroopad-x64.deb
 chmod 777 libpng12-amd64.deb
 chmod 777 rallets-linux_1.4.0_amd64.deb
 chmod 777 sogoupinyin_amd64.deb
 chmod 777 teamviewer_amd64.deb
 chmod 777 wps-office_amd64.deb
 chmod 777 FoxitReader.x64.run
 chmod 777 qtcreator-ros-trusty-latest-offline-installer.run
  sudo gdebi -n haroopad-x64.deb
  sudo gdebi -n libpng12-amd64.deb
  sudo gdebi -n rallets-linux_1.4.0_amd64.deb
  sudo gdebi -n sogoupinyin_amd64.deb
  sudo gdebi -n teamviewer_amd64.deb
  sudo gdebi -n wps-office_amd64.deb