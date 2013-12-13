#!/bin/bash
IMG=$1
dd bs=512 count=2880 if=/dev/zero of=$IMG
sudo mkdosfs $IMG
mkdir disk-mount
sudo mount -o loop $IMG disk-mount
echo "test" > test.txt
sudo mv test.txt disk-mount/test.txt
sudo umount disk-mount
rm -rf disk-mount
