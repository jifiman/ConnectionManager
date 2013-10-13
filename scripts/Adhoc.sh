#!/bin/bash
#set -x
DEVICE=wlan0
CONFIG_FILE=./hostapd.conf
ConfigureDevice()
{
	if ! sudo iwconfig 2>/dev/null | grep $DEVICE >/dev/null;then
		echo $DEVICE not found!
		exit -1
	fi

	#Ensure config file exists
	if [ ! -e $CONFIG_FILE ]
	then
		echo "Can't find hostapd config file"
		exit -1
	fi

	sudo service isc-dhcp-server stop
	sudo pkill hostapd
	sudo ifdown  $DEVICE 
	sudo ifconfig $DEVICE up 192.168.1.100 netmask 255.255.255.0
	sudo service isc-dhcp-server start
	sudo hostapd -B $CONFIG_FILE
		
}
cd ${0%/*}	#make current working directory the directory of the bash script
ConfigureDevice




