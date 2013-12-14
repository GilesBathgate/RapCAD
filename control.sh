#!/bin/bash
NAME=$(date +%Y%m%d)
PASSWORD="password-"$NAME
USERNAME=Administrator

#VBoxManage controlvm "$NAME" \
#	setcredentials $USERNAME \
#	$PASSWORD \
#	localhost
#sleep 10
VBoxManage --nologo guestcontrol "$NAME" execute \
	--image "C:\\windows\\system32\\shutdown.exe" \
	--username $USERNAME \
	--password $PASSWORD \
	--wait-exit \
	-- -f -s -t 00
