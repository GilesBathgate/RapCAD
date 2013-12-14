#!/bin/bash
IMG=$1
shift
dd bs=512 count=2880 if=/dev/zero of=$IMG
sudo mkdosfs $IMG
mkdir disk-mount
sudo mount -o loop $IMG disk-mount
sudo cp $* disk-mount/
sleep 1
sudo umount disk-mount
rm -rf disk-mount
