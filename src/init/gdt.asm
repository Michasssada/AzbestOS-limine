section .data

gdt:
    dq 0                       
    dq 0x00209a0000000000  
    dq 0x00af92000000ffff       

gdt_descriptor:
    dw gdt_end - gdt - 1        
    dq gdt                      

gdt_end:

section .text
bits 64

global load_gdt
load_gdt:
    lgdt [gdt_descriptor]       
    
    mov ax, 0x10                
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    push 0x08
    lea rax, [.reload_cs]
    push rax
    retfq


.reload_cs:
    ret                        