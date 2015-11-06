#!/bin/bash
set -e 

rm bin/*

echo "=== build pure64"
cd pure64
nasm linuxBoot.s -o ../bin/linuxBoot.o

echo "=== build bmos"
cd ../bmos
nasm -f elf64 kernel64.asm -o ../bin/kernel64.o
gcc  -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o ../bin/libBareMetal.o libBareMetal.c

echo "=== build memtest"
cd ../memtest64
gcc  -I../bmos -std=c1x -c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -mno-red-zone -o ../bin/memtest.o memtest.c 

#echo "=== build pw_mem"
#cd ../pw_mem
#make

echo "=== linking"
cd  ../bin/
ld -T ../kernel64.lds kernel64.o memtest.o libBareMetal.o -b binary  -o kernel.img

echo "=== make image"
cat linuxBoot.o kernel.img > neos.img

echo "=== install image"
sudo cp -v neos.img /boot

echo "=== check bin dir"
ls -l 

echo "OK"
