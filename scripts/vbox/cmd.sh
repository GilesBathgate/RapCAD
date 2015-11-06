#!/bin/bash
source config

ARGS=$*
VBoxManage --nologo guestcontrol "$NAME" execute \
	--image c:\\windows\\system32\\cmd.exe \
	--username $USER \
	--password $PASS \
	--wait-exit \
	--wait-stderr \
	--wait-stdout \
	-- /c $ARGS
echo
