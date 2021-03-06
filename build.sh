#!/bin/bash
set -e 

rm -f bin/*.o
rm -f pw_mem/*.o

if [ -z ${CC1+x} ]; then CC1=clang; else echo "CC1 is set to '$CC1'"; fi

# -nostartfiles -nodefaultlibs
FLG64OS="-m64 -nostdlib  -fomit-frame-pointer -mno-red-zone -ffreestanding -fno-builtin" 

echo "=== build pure64"
cd pure64
nasm linuxBoot.s -o ../bin/linuxBoot.o

echo "=== build bmos"
cd ../bmos
nasm -f elf64 kernel64.asm -o ../bin/kernel64.o
$CC1 -c $FLG64OS -o ../bin/libBareMetal.o libBareMetal.c

echo "=== build memtest"
cd ../memtest64
$CC1  -I../bmos -std=c1x -c $FLG64OS -o ../bin/memtest.o memtest.c 

echo "=== build std lib"
cd ../std
$CC1 -c  -std=c1x $FLG64OS  stdio.c -o ../bin/stdio.o

echo "=== build pw_mem"
cd ../pw_mem
make
cp mm.o ../bin/


echo "=== linking"
cd  ../bin/
ld -T ../kernel64.lds kernel64.o memtest.o libBareMetal.o mm.o stdio.o -b binary  -o kernel.img

echo "=== make image"
cat linuxBoot.o kernel.img > neos.img

echo "=== install image"
sudo cp -v neos.img /boot

echo "=== check bin dir"
ls -l 

echo "OK"
