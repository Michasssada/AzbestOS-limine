bits 64

section .bss
    extern stack_address

section .text
    extern kernel_main
    global _start

%include "init/cpu.asm"

_start:
    and rsp, ~16 ; Align to 16 bytes
    call setup_sse
    call setup_avx

    call kernel_main

    cli
    hlt