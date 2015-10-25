#!/bin/bash
source config

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
	--username $USER \
	--password $PASS \
	--wait-exit \
	--wait-stdout \
	-- $ARGS
