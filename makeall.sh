#!/bin/bash
sudo true #Just ask the user for a password early.
NAME=$(date +%Y%m%d)
KEY=$(cat key.txt)
./makesif.sh $KEY
rm disk.img
./makefloppy.sh disk.img winnt.sif
./makemachine-xp.sh $NAME
VBoxHeadless -startvm $NAME
./update-xp.sh $NAME
