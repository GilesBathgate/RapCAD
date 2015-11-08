#!/bin/bash
source config

ARGS=$*
vboxmanage --nologo guestcontrol "$NAME" run \
        --exe c:\\windows\\system32\\cmd.exe \
	--username $USER \
	--password $PASS \
	--wait-stderr \
	--wait-stdout \
	-- cmd /c $ARGS
