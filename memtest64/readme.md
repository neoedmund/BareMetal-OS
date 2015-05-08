memtest64
==========================

A memtest util based on BareMetal OS and Pure64.


Build
-------------
run build.sh



Install
---------------------

    sudo cp bin/neos.bin /boot/


add an entry to grub.cfg:


    menuentry "memtest64 load by linux16" {
        linux16 /boot/neos.bin
    }


Use
--------------------------
In the BareMetalOS command line, input "memtest"


Tech info
----------------------------

It use pure64 and BMOS to 64bit envirement.

The SMP is disable currently.

It enables CPU cache, so you can see a performance boost compare to others like memtest86+.

And It can detect common memory errors.(tested).

It test all usable memory according to E820 memory map, exception memory below 2MB used by system.

It use linux "old" boot protocol ver 2.03. Bootloader like grub, will load start 512 bytes
with max to 512*16 bytes setup code to 0x90000, and a "flat big kernel" payload start from
0x100000(1MB), which can be really big like bzImage of linux.

Hobby OS develops can start from this enviroment for thier X86_64 OS dev, load by grub and test
on real machine easily. No need to make new filesystem or disk partions.



Neoe 2015/May/8

