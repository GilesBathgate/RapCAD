#!/bin/bash
TODAY=$(date +%Y%m%d)
NAME="WindowsXP-Build-Environment-$TODAY"
DISK="/data/$NAME.vdi"
DVD="/data/iso/windowsXP-sp3-x86.iso"
IMG="./disk.img"

VBoxManage createvm --name "$NAME" --ostype WindowsXP --register

VBoxManage modifyvm "$NAME" --memory 1024 --acpi on --boot1 dvd --nic1 nat

VBoxManage createhd --filename "$DISK" --size 30000

VBoxManage storagectl "$NAME" --name "SATA Controller" --add sata

VBoxManage storagectl "$NAME" --name "IDE Controller" --add ide --controller PIIX4

VBoxManage storageattach "$NAME" --storagectl "SATA Controller" --port 0 --device 0 --type hdd --medium "$DISK"

VBoxManage storageattach "$NAME" --storagectl "IDE Controller" --port 0 --device 0 --type dvddrive --medium "$DVD"

VBoxManage storagectl "$NAME" --name "Floppy Controller" --add floppy --controller I82078

VBoxManage storageattach "$NAME" --storagectl "Floppy Controller" --port 0 --device 0 --type fdd --medium "$IMG"
