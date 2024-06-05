#!/bin/bash
make clean
clear
make
qemu-system-i386 -machine q35 -m 256 -kernel kernel/kernel.bin