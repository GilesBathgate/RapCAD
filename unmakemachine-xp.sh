#!/bin/bash
source config

VBoxManage storageattach "$NAME" --storagectl "IDE Controller" --port 1 --device 0 --type dvddrive --medium none

VBoxManage storageattach "$NAME" --storagectl "IDE Controller" --port 0 --device 1 --type dvddrive --medium none

VBoxManage storageattach "$NAME" --storagectl "SATA Controller" --port 0 --device 0 --type hdd --medium none

VBoxManage storageattach "$NAME" --storagectl "Floppy Controller" --port 0 --device 0 --type fdd --medium none

VBoxManage closemedium disk "$DISK" --delete

VBoxManage unregistervm "$NAME" --delete
