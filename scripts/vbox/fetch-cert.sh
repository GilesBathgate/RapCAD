#!/bin/bash

mkdir -p tmp

mount -o loop /usr/share/virtualbox/VBoxGuestAdditions.iso tmp

cp tmp/cert/oracle-vbox.cer .

umount tmp

rm -r tmp
