#!/bin/bash
#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#cd $DIR
#echo $DIR
set -x
Error()
{	
	logger FAILED:$1 	
	echo FAILED:$1
	echo Returned:$2
	exit $2	
}
Execute()
{	
	eval $command
	returnCode=$?
	if [[ $returnCode != 0 ]] ; then
		Error $command $returnCode
	fi
}

#Start Execution



cd ${0%/*}	#make current working directory the directory of the bash script

command="sed -e \"s/SSID/${1}/\" -e \"s/PASS/${2}/\" ../templates/interfaces.base >../files/interfaces"
Execute

command="service isc-dhcp-server stop"
Execute

command="pkill hostapd"
Execute


command="ifdown -v wlan0"
Execute

#command="ifup -v -i ../files/interfaces wlan0"

ifup -v -i ../files/interfaces wlan0 &
for iPoll in {1..20}
do
sleep 2
if ps -p $! | grep -q "$!"; then
    echo NOT CONNECTED YET
else
    echo CONNECTED
    connected='yes'
	exit 0
fi
done

if ps -p $! | grep -q "$!"; then
    echo NOT CONNECTED
    connected='no'
    kill $!
    exit 100
else
    echo CONNECTED
    connected='yes'
fi
exit 0
