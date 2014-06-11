#!/bin/bash
source config

vboxmanage controlvm "$NAME" poweroff

vboxmanage storageattach "$NAME" --storagectl "IDE Controller" --port 1 --device 0 --type dvddrive --medium none

vboxmanage storageattach "$NAME" --storagectl "IDE Controller" --port 0 --device 1 --type dvddrive --medium none

vboxmanage storageattach "$NAME" --storagectl "SATA Controller" --port 0 --device 0 --type hdd --medium none

vboxmanage storageattach "$NAME" --storagectl "Floppy Controller" --port 0 --device 0 --type fdd --medium none

vboxmanage closemedium disk "$DISK" --delete

vboxmanage unregistervm "$NAME" --delete
