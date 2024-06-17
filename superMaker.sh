#!/bin/bash
make clean
clear
make
qemu-system-i386 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker -machine q35 -m 256 -kernel kernel/kernel.bin