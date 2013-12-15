#!/bin/bash
USERNAME=Administrator
NAME=$(date +%Y%m%d)
PASSWORD=$1
shift
COMMAND=$1
shift
ARGS=$*
#VBoxManage controlvm "$NAME" \
#	setcredentials $USERNAME \
#	$PASSWORD \
#	localhost
#sleep 10
VBoxManage --nologo guestcontrol "$NAME" execute \
	--image $COMMAND \
	--username $USERNAME \
	--password $PASSWORD \
	--wait-exit \
	--wait-stdout \
	-- $ARGS
