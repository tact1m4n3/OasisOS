#!/bin/bash

IMAGE=$1

fallocate -l 10M efi.img && dd if=/dev/zero bs=1M count=10 of=efi.img
mformat -i efi.img -f 1440 ::
mmd -i efi.img ::/efi
mmd -i efi.img ::/efi/boot
mcopy -i efi.img bootx64.efi ::/efi/boot

mkdir -p cdrom
cp -f boot.sys cdrom/
cp -f efi.img cdrom/

xorriso -as mkisofs -R -J -c bootcat \
        -b boot.sys -no-emul-boot -boot-load-size full \
        -eltorito-alt-boot -e efi.img -no-emul-boot -isohybrid-gpt-basdat \
        -o image.iso cdrom

tail -c +513 image.iso > image.dat
cat mbr.sys image.dat > image.iso
rm image.dat
