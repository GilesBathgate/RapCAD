#!/bin/bash
sudo true #Just ask the user for a password early.
NAME=$(date +%Y%m%d)
KEY=$(cat key.txt)
PASSWORD="password-"$(date +%Y%m%d)
SIF=winnt.sif
IMG="disk.img"
DISK="$NAME.vdi"
ADDITIONS="/usr/share/virtualbox/VBoxGuestAdditions.iso"
DVD="../iso/windowsXP-sp3-x86.iso"
USERNAME=Administrator

echo "[Data]" > $SIF
echo "AutoPartition = 1" >> $SIF
echo "MsDosInitiated = 0" >> $SIF
echo "UnattendedInstall = Yes" >> $SIF
echo "AutomaticUpdates = No" >> $SIF
echo >> $SIF
echo "[Unattended]" >> $SIF
echo "UnattendMode = FullUnattended" >> $SIF
echo "OemSkipEula = Yes" >> $SIF
echo "OemPreinstall = No" >> $SIF
echo "TargetPath = \WINDOWS" >> $SIF
echo "Repartition = Yes" >> $SIF
echo "UnattendSwitch = Yes" >> $SIF
echo "DriverSigningPolicy = Ignore" >> $SIF
echo "WaitForReboot = No" >> $SIF
echo >> $SIF
echo "[GuiUnattended]" >> $SIF
echo "AdminPassword = \"$PASSWORD\"" >> $SIF
echo "AutoLogon = Yes" >> $SIF
echo "AutoLogonCount = 1" >> $SIF
echo "OEMSkipRegional = 1" >> $SIF
echo "OemSkipWelcome = 1" >> $SIF
echo "TimeZone = 85" >> $SIF
echo "ServerWelcome = No" >> $SIF
echo >> $SIF
echo "[UserData]" >> $SIF
echo "ProductKey = \"$KEY\"" >> $SIF
echo "FullName = \"user\"" >> $SIF
echo "OrgName = \"\"" >> $SIF
echo "ComputerName = \"computer\"" >> $SIF
echo >> $SIF
echo "[Identification]" >> $SIF
echo "JoinWorkgroup = WORKGROUP" >> $SIF
echo >> $SIF
echo "[Networking]" >> $SIF
echo "InstallDefaultComponents = Yes" >> $SIF
echo >> $SIF
echo "[Display]" >> $SIF
echo "XResolution = 1024" >> $SIF
echo "YResolution = 768" >> $SIF
echo "BitsPerPel = 32" >> $SIF
echo "AutoConfirm = 1" >> $SIF
echo >> $SIF
echo "[GuiRunOnce]" >> $SIF
echo "Command0=\"C:\WINDOWS\System32\cmd.exe /c start /wait E:\VBoxWindowsAdditions.exe /S /depth=32 /xres=1024 /yres=768 /with_autologon && shutdown -f -s -t 00\"" >> $SIF

rm -f $IMG
dd bs=512 count=2880 if=/dev/zero of=$IMG
sudo mkdosfs $IMG
mkdir disk-mount
sudo mount -o loop $IMG disk-mount
sudo cp $SIF disk-mount/
sleep 1
sudo umount disk-mount
rm -rf disk-mount
rm $SIF

VBoxManage createvm \
	--name "$NAME" \
	--ostype WindowsXP \
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

VBoxHeadless -startvm $NAME

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

VBoxManage sharedfolder add "$NAME" \
	--name shared \
	--hostpath $PWD/shared \
	--automount

VBoxHeadless -startvm "$NAME" &
