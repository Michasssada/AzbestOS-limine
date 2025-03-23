//
// Created by Neko on 20.01.2025.
//

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idtr
{
    uint16_t limit;
    uint64_t idt_address;
} __attribute__((packed));

struct gate_descriptor_64
{
    uint16_t offset_one;
    uint16_t segment_selector;
    uint8_t ist : 3;
    uint8_t reserved_one : 5;
    uint8_t gate_type : 4;
    uint8_t zero : 1;
    uint8_t dpl : 2;
    uint8_t present : 1;
    uint16_t offset_two;
    uint32_t offset_three;
    uint32_t reserved_two;
} __attribute__((packed));

#define IDT_GATE_TYPE_INTERRUPT (0xE)
#define IDT_GATE_TYPE_TRAP_GATE (0xF)
#define IDT_DPL_RING_0 (0)
#define IDT_DPL_RING_1 (1)
#define IDT_DPL_RING_2 (2)
#define IDT_DPL_RING_3 (3)
#define IDT_PRESENT (1)
#define IDT_NO_IST (0)
#define IDT_DEFAULT_SEGMENT (0x8)

#define IDT_ENTRY(_offset, _segment_selector, _gate_type, _dpl_layer, _is_present, _ist)     \
{                                                                                      \
        .offset_one = (uint16_t)((uint64_t)(_offset) & 0xFFFF),                         \
        .segment_selector = _segment_selector,                                          \
        .ist = _ist,                                                                    \
        .reserved_one = 0,                                                             \
        .gate_type = _gate_type,                                                        \
        .zero = 0,                                                                     \
        .dpl = _dpl_layer,                                                              \
        .present = _is_present,                                                         \
        .offset_two = (uint16_t)(((uint64_t)(_offset) >> 16) & 0xFFFF),                 \
        .offset_three = (uint32_t)(((uint64_t)(_offset) >> 32) & 0xFFFFFFFF),           \
        .reserved_two = 0                                                              \
}                                                                                      \

struct interrupt_frame
{
    uint64_t dr0, dr1, dr2, dr3, dr4, dr5, dr6, dr7, cr0, cr2, cr3, cr4, cr8, gs, fs, es, ds, r15, r14, r13, r12, r11,
        r10, r9, r8, rdi, rsi, rbp, rsp, rdx, rcx, rbx, rax, interrupt_number, error_code, rip, cs, rflags, orig_rsp,
        ss;
} __attribute__((packed));

extern volatile struct gate_descriptor_64 idt[256];

void initialize_idt_table();
void initialize_idt();
extern void enable_interrupts();
extern void disable_interrupts();
void load_idt_table(struct idtr *idtr);

#endif // IDT_H
