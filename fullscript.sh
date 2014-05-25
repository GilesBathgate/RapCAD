#!/bin/bash
date
sudo true #Just ask the user for sudo password early.
source config

echo "Building configuration file..."
./mksif.sh
./mkwmi.sh
echo "Building floppy image..."

rm -f $IMG
dd bs=512 count=2880 if=/dev/zero of=$IMG
sudo mkdosfs $IMG
mkdir disk-mount
sudo mount -o loop $IMG disk-mount
sudo cp $SIF disk-mount/
sudo cp $WMI disk-mount/
sleep 1
sudo umount disk-mount
rm -rf disk-mount
rm -f $SIF
rm -f $WMI

echo "Building machine..."

VBoxManage createvm \
	--name "$NAME" \
	--ostype $OSTYPE \
	--register

VBoxManage modifyvm "$NAME" \
	--memory 1024 \
	--acpi on \
	--boot1 dvd \
	--nic1 nat

VBoxManage createhd \
	--filename "$DISK" \
	--size 30000

VBoxManage storagectl "$NAME" \
	--name "IDE Controller" \
	--add ide \
	--controller PIIX4

VBoxManage storageattach "$NAME" \
	--storagectl "IDE Controller" \
	--port 0 \
	--device 0 \
	--type hdd \
	--medium "$DISK"

VBoxManage storageattach "$NAME" \
	--storagectl "IDE Controller" \
	--port 0 \
	--device 1 \
	--type dvddrive \
	--medium "$DVD"

VBoxManage storageattach "$NAME" \
	--storagectl "IDE Controller" \
	--port 1 \
	--device 0 \
	--type dvddrive \
	--medium "$ADDITIONS"

VBoxManage storagectl "$NAME" \
	--name "Floppy Controller" \
	--add floppy \
	--controller I82078

VBoxManage storageattach "$NAME" \
	--storagectl "Floppy Controller" \
	--port 0 \
	--device 0 \
	--type fdd \
	--medium "$IMG"

echo "Installing windows..."

VBoxHeadless -startvm $NAME

echo "Removing devices..."

VBoxManage storageattach "$NAME" \
	--storagectl "IDE Controller" \
	--port 1 \
	--device 0 \
	--type dvddrive \
	--medium none

VBoxManage storageattach "$NAME" \
	--storagectl "IDE Controller" \
	--port 0 \
	--device 1 \
	--type dvddrive \
	--medium none

VBoxManage storageattach "$NAME" \
	--storagectl "Floppy Controller" \
	--port 0 \
	--device 0 \
	--type fdd \
	--medium none

rm $IMG

VBoxManage storagectl "$NAME" \
	--name "Floppy Controller" \
	--remove

date
echo "Adding shared folder..."

VBoxManage sharedfolder add "$NAME" \
	--name shared \
	--hostpath $SHAREDFOLDER \
	--automount

VBoxHeadless -startvm "$NAME" &

echo "Wait for machine to be ready..."
echo -n "Waiting"
until VBoxManage guestcontrol "$NAME" \
	stat C:\\ \
	--username $USER \
	--password $PASS
do
 echo -n "."
 sleep 1
done

echo "Attempting login..."
VBoxManage controlvm "$NAME" \
	setcredentials $USER $PASS localhost \
	--allowlocallogon yes

echo -n "Waiting"
until VBoxManage guestcontrol "$NAME" \
	stat D:\\ \
	--username $USER \
	--password $PASS
do
 echo -n "."
 sleep 1
done

echo "Ready to go..."
echo "$USER password: $PASS"
