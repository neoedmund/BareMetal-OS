#!/bin/bash
set -e 
nasm -f elf64 kernel64.asm -o ../../../bin/kernel64.elf64
gcc  -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o ../../../bin/libBareMetal.o ../programs/libBareMetal.c
gcc  -std=c99 -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o ../../../bin/neos.o ../programs/neos.c 
cp kernel64.lds ../../../bin/
cd  ../../../bin/
ld -T kernel64.lds kernel64.elf64 neos.o libBareMetal.o -b binary  -o kernel64.bin
cat pxestart.sys pure64.sys kernel64.bin > neos-hw
sudo cp -v neos-hw /boot
echo "OK"
