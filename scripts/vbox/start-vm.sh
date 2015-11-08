#!/bin/bash
source config

vboxmanage startvm "$NAME" &

echo "Wait for machine to be ready..."
echo -n "Waiting"
until vboxmanage guestcontrol "$NAME" \
        stat C:\\ \
        --username $USER \
        --password $PASS 2> /dev/null
do
 echo -n "."
 sleep 1
done

echo "Attempting login..."
vboxmanage controlvm "$NAME" \
        setcredentials $USER $PASS localhost \
        --allowlocallogon yes

echo -n "Waiting"
until vboxmanage guestcontrol "$NAME" \
        stat D:\\ \
        --username $USER \
        --password $PASS
do
 echo -n "."
 sleep 1
done

echo "Ready to go..."
echo "$USER password: $PASS"
