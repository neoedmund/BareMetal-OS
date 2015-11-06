#!/bin/bash
set -e 

cd pure64
nasm linuxBoot.s -o ../bin/linuxBoot.o

cd ../bmos
nasm -f elf64 kernel64.asm -o ../bin/kernel64.o
gcc  -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o ../bin/libBareMetal.o libBareMetal.c
gcc  -std=c99 -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o ../bin/neos.o neos.c 

cd ../pw_mem
make

cd  ../bin/
ld -T ../bmos/kernel64.lds kernel64.o neos.o libBareMetal.o -b binary  -o kernel64all.o

cat linuxBoot.o kernel64all.o > neos.img
sudo cp -v neos.img /boot
ls -l 

echo "OK"
