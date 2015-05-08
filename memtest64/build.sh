#!/bin/bash
set -e 

cd /home/neoe/localsvn/lab/neos/pure64/
nasm linuxBoot.s -o bin/linuxBoot.bin

cd bmos
nasm -f elf64 kernel64.asm -o ../bin/kernel64.elf64
gcc  -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o ../bin/libBareMetal.o libBareMetal.c
gcc  -std=c99 -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o ../bin/neos.o neos.c 
cp kernel64.lds ../bin/
cd  ../bin/
ld -T kernel64.lds kernel64.elf64 neos.o libBareMetal.o -b binary  -o kernel64.bin

cat linuxBoot.bin kernel64.bin > neos.bin
sudo cp -v neos.bin /boot
ls -l 

echo "OK"
