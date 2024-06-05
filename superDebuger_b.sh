#!/bin/bash
gdb kernel/kernel.bin -ex "target remote :1234"
