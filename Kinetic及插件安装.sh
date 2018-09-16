# ！/bin/bash
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
sudo apt-get update
sudo apt-get install ros-kinetic-desktop-full
sudo rosdep init
rosdep update
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
sudo apt-get install python-rosinstall python-rosinstall-generator python-wstool build-essential

#安装串口插件
sudo apt-get install ros-kinetic-serial

sudo apt-get update
sudo apt-get install python-gudev
sudo apt-get install -y avahi-daemon
sudo apt-get install -y openssh-server
sudo apt-get install -y arduino-core
sudo easy_install pip
sudo pip install -U platformio
sudo rm -rf ~/.platformio/
#wget https://www.pjrc.com/teensy/49-teensy.rules
sudo cp 49-teensy.rules /etc/udev/rules.d/
sudo cp 58-riki.rules /etc/udev/rules.d/
sudo cp 558-orbbec-usb.rules /etc/udev/rules.d/

echo "Dev-tools Installation Done"

#!/bin/bash
robothostname=$1
localhostname=$2

echo "export ROS_HOSTNAME=${localhostname}.local" >> ~/.bashrc
echo "export ROS_MASTER_URI=http://${robothostname}.local:11311" >> ~/.bashrc
echo "Network setup done"

#!/bin/bash
echo "export ROS_IP=\`hostname -I\`" >> ~/.bashrc
echo "export ROS_HOSTNAME=\`hostname -I\`" >> ~/.bashrc
echo "export ROS_MASTER_URI=http://\`hostname -I\`:11311" >> ~/.bashrc
echo "export RIKIBASE=2wd" >> ~/.bashrc
echo "export RIKILIDAR=rplidar" >> ~/.bashrc

echo "Network setup done"

#!/bin/bash
rikimaster=$1
echo "export ROS_IP=\`hostname -I\`" >> ~/.bashrc
echo "export ROS_HOSTNAME=\`hostname -I\`" >> ~/.bashrc
echo "export ROS_MASTER_URI=http://${rikimaster}:11311" >> ~/.bashrc

echo "Network setup done"

#sudo apt-get update
ver=$(rosversion -d)
sudo apt-get install -y libv4l-dev 
sudo apt-get install -y chrony
sudo apt-get install -y libbullet-dev 
sudo apt-get install -y libudev-dev
sudo apt-get install -y mini-httpd
sudo apt-get install -y ros-$ver\-roslint
sudo apt-get install -y ros-$ver\-rosserial
sudo apt-get install -y ros-$ver\-imu-filter-madgwick
sudo apt-get install -y ros-$ver\-gmapping
sudo apt-get install -y ros-$ver\-navigation
sudo apt-get install -y ros-$ver\-map-server
sudo apt-get install -y ros-$ver\-rgbd-launch
sudo apt-get install -y ros-$ver\-costmap-2d
sudo apt-get install -y ros-$ver\-camera-info-manager
sudo apt-get install -y ros-$ver\-image-transport
sudo apt-get install -y ros-$ver\-teleop-twist-keyboard 

sudo apt-get install -y ros-$ver\-rosbridge-suite
sudo apt-get install -y ros-$ver\-robot-pose-publisher
sudo apt-get install -y ros-$ver\-tf2-web-republisher
sudo apt-get install -y ros-$ver\-web-video-server

mkdir ~/catkin_ws/src -p
cd ~/catkin_ws/src
mkdir depth_camera lidar teleop

git clone https://github.com/ykevin/rikirobot_project.git
cd rikirobot_project
git submodule init
git submodule update

cd ~/catkin_ws/src
echo "add camera ros packages!!"
git clone https://github.com/ktossell/camera_umd.git

echo "reinstall geometry2 sloved imu error"
git clone https://github.com/ros/geometry2.git


cd depth_camera
echo "add Astra xtion live"
echo "Astra xtion driver please: https://orbbec3d.com/develop/"
git clone https://github.com/orbbec/ros_astra_launch.git
git clone https://github.com/orbbec/ros_astra_camera.git
git clone https://github.com/xaxxontech/depthimage_to_laserscan.git

cd  ~/catkin_ws/src
echo "slove rosserial indigo version"
#git clone  https://github.com/ros-drivers/rosserial.git 

echo "add ps3"
#sudo apt-get install -y libusb-dev libspnav-dev  libbluetooth-dev libcwiid-dev
#sudo pip install pybluez
#git clone https://github.com/ros-drivers/joystick_drivers.git

echo "add Twist multiplexer"
#git clone https://github.com/ros-teleop/twist_mux.git

cd teleop
echo "add Generic Keyboard Teleop"
git clone https://github.com/ykevin/teleop_twist_keyboard.git

cd ~/catkin_ws/src
cd lidar
echo "add hector slam"
git clone https://github.com/tu-darmstadt-ros-pkg/hector_slam.git

echo "add rplidar ros packages"
git clone -b slam https://github.com/robopeak/rplidar_ros.git

echo "add EAI X4 lidar ros packages"
git clone -b 1.2.3 https://github.com/EAIBOT/ydlidar.git

cd ~/catkin_ws/
catkin_make -j1

echo "source ~/catkin_ws/devel/setup.bash" >> ~/.bashrc
source ~/.bashrc

sudo usermod -aG dialout $USER

echo "ROS Packages Installed Successfully"
echo "Now you need to edit .bashrc, change some variable"