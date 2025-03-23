//
// Created by Neko on 20.01.2025.
//

#include <dbg/print.h>
#include "idt.h"

const char *exception_strings[32] = {"(#DE) Division Error",
                                     "(#DB) Debug",
                                     "(#--) Non-maskable Interrupt",
                                     "(#BP) Breakpoint",
                                     "(#OF) Overflow",
                                     "(#BR) Bound Range Exceeded",
                                     "(#UD) Invalid Opcode",
                                     "(#NM) Device Not Available",
                                     "(#DF) Double Fault",
                                     "(#--) Coprocessor Segment Overrun",
                                     "(#TS) Invalid TSS",
                                     "(#NP) Segment Not Present",
                                     "(#SS) Stack Segment Fault",
                                     "(#GP) General Protection Fault",
                                     "(#PF) Page Fault",
                                     "(#--) Reserved",
                                     "(#MF) x87 Floating-Point Exception",
                                     "(#AC) Alignment Check",
                                     "(#MC) Machine Check",
                                     "(#XM) SIMD Floating-Point Exception",
                                     "(#VE) Virtualization Exception",
                                     "(#CP) Control Protection Exception",
                                     "",
                                     "",
                                     "",
                                     "",
                                     "",
                                     "",
                                     "(#HV) Hypervisor Injection Exception",
                                     "(#VC) VMM Communication Exception",
                                     "(#SX) Security Exception",
                                     "(#--) Reserved"};

static inline uint64_t read_cr2()
{
    uint64_t cr2;
    asm volatile("mov %%cr2, %0" : "=r"(cr2));
    return cr2;
}

static void print_page_fault_info(const struct interrupt_frame regs)
{
    // Print the faulting address from CR2
    uint64_t faulting_address = read_cr2();
    dbg_print("Linear address where the issue happened: %016lX\n", faulting_address);

    // Decode the error code
    uint32_t error_code = regs.error_code;

    dbg_print("The error has been caused by: ");

    if (error_code & (1 << 0))
    {
        dbg_print("page-protection violation. ");
    }
    else
    {
        dbg_print("non-present page. ");
    }

    if (error_code & (1 << 1))
    {
        dbg_print("Write access. ");
    }
    else
    {
        dbg_print("Read access. ");
    }

    if (error_code & (1 << 2))
    {
        dbg_print("User mode. ");
    }
    else
    {
        dbg_print("Kernel mode. ");
    }

    if (error_code & (1 << 3))
    {
        dbg_print("Reserved bit violation. ");
    }

    if (error_code & (1 << 4))
    {
        dbg_print("Instruction fetch. ");
    }

    if (error_code & (1 << 5))
    {
        dbg_print("Protection key violation. ");
    }

    if (error_code & (1 << 6))
    {
        dbg_print("Shadow stack access violation. ");
    }

    if (error_code & (1 << 15))
    {
        dbg_print("SGX violation. ");
    }

    dbg_print("\n");
}

static void print_processor_flags(const uint64_t rflags)
{
    if (rflags & 0x00000001)
        dbg_print("CF ");
    if (rflags & 0x00000004)
        dbg_print("PF ");
    if (rflags & 0x00000010)
        dbg_print("AF ");
    if (rflags & 0x00000040)
        dbg_print("ZF ");
    if (rflags & 0x00000080)
        dbg_print("SF ");
    if (rflags & 0x00000100)
        dbg_print("TF ");
    if (rflags & 0x00000200)
        dbg_print("IF ");
    if (rflags & 0x00000400)
        dbg_print("DF ");
    if (rflags & 0x00000800)
        dbg_print("OF ");
    if (rflags & 0x00010000)
        dbg_print("RF ");
    if (rflags & 0x00020000)
        dbg_print("VM ");
    if (rflags & 0x00040000)
        dbg_print("AC ");
    if (rflags & 0x00080000)
        dbg_print("VIF ");
    if (rflags & 0x00100000)
        dbg_print("VIP ");
    if (rflags & 0x00200000)
        dbg_print("ID ");
    if (rflags & 0x80000000)
        dbg_print("AI ");
    dbg_print("\n");
}

void print_registers(const struct interrupt_frame *regs)
{
    dbg_print("RAX: %016lX RBX: %016lX RCX: %016lX RDX: %016lX\n", regs->rax, regs->rbx, regs->rcx, regs->rdx);
    dbg_print("RSI: %016lX RDI: %016lX RBP: %016lX RSP: %016lX\n", regs->rsi, regs->rdi, regs->rbp, regs->rsp);
    dbg_print("R8:  %016lX R9:  %016lX R10: %016lX R11: %016lX\n", regs->r8, regs->r9, regs->r10, regs->r11);
    dbg_print("R12: %016lX R13: %016lX R14: %016lX R15: %016lX\n", regs->r12, regs->r13, regs->r14, regs->r15);
    dbg_print("DR0: %016lX DR1: %016lX DR2: %016lX DR3: %016lX\n", regs->dr0, regs->dr1, regs->dr2, regs->dr3);
    dbg_print("DR4: %016lX DR5: %016lX DR6: %016lX DR7: %016lX\n", regs->dr4, regs->dr5, regs->dr6, regs->dr7);
    dbg_print("CR0: %016lX CR2: %016lX CR3: %016lX CR4: %016lX CR8: %016lX\n", regs->cr0, regs->cr2, regs->cr3,
                regs->cr4, regs->cr8);
    dbg_print("CS:  %02lX  DS:  %02lX  SS:  %02lX  ES:  %02lX  FS:  %02lX GS: %02lX\n", regs->cs, regs->ds, regs->ss,
                regs->es, regs->fs, regs->gs);
    dbg_print("RIP: %016lX\n", regs->rip);
    dbg_print("Orig RSP: %016lX CR3: %016lX\n", regs->orig_rsp, regs->cr3);
    dbg_print("Error code: %016lX Interrupt index: %016lX\n", regs->error_code, regs->interrupt_number);
    dbg_print("RFLAGS: ");
    print_processor_flags(regs->rflags);
    // Print Debug Registers

    // Print Control Registers
    dbg_print("%s\n", exception_strings[regs->interrupt_number]);
}

void interrupt_handler(struct interrupt_frame *frame)
{
    if (frame->interrupt_number < 32)
    {
        print_registers(frame);

        if (frame->interrupt_number == 0xe)
        {
            print_page_fault_info(*frame);
        }
        asm volatile("cli; hlt");
        asm volatile("cli; hlt");
    }
}

void initialize_idt()
{
    initialize_idt_table();

    struct idtr idt_register = {.limit = sizeof(idt) - 1, .idt_address = (uint64_t)&idt};

    load_idt_table(&idt_register);

    asm volatile("sti");
}
