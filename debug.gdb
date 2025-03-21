set disassembly-flavor intel
symbol-file cmake-build-debug-wsl/kernel.elf
target remote localhost:1234
layout asm
break _start
continue