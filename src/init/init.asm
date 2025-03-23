bits 64

section .bss
    extern stack_address

section .text
    extern kernel_main
    global _start

%include "init/cpu.asm"

_start:

    call setup_sse
    call setup_avx

    and rsp, ~8 ; Align to 16 bytes (after call kernel main it will be 16 bytes aligned)

    call kernel_main

    cli
    hlt