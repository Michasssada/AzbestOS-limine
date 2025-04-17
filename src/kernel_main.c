#include "dbg/print.h"

#include <drivers/cpu/gdt/gdt.h>
#include <drivers/cpu/idt/idt.h>
#include <drivers/display/display.h>
#include <drivers/uart/uart.h>
#include <limine.h>
#include <drivers/cpu/APIC/apic.h>
#include <drivers/cpu/IOAPIC/ioapic.h>

void kernel_main()
{

    initialize_gdt();
    initialize_idt();
    init_framebuffer();
    clear_screen(0x000000);
    enable_apic();
    ioapic_init();
    draw_string("AzbestOS Limine ver. 0.0.1", 10, 10, 0x00ff00);
    

    while (1)
    {
        //asm volatile ("cli; hlt");
    }
}