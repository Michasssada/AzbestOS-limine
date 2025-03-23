#include "dbg/print.h"

#include <drivers/cpu/gdt/gdt.h>
#include <drivers/cpu/idt/idt.h>
#include <drivers/display/display.h>
#include <drivers/uart/uart.h>
#include <limine.h>

void kernel_main()
{
    dbg_print("Hello, world!\n");

    initialize_gdt();
    initialize_idt();
    asm volatile ("int $0x10");
    dbg_print("CPU has been initialized.\n");
    init_framebuffer();
    clear_screen(0x000000);
    for(int i = 0; i < 1000; i++) {
        put_pixel(i, i, 0xFFFFFF);
    }

    while (1)
    {
        //asm volatile ("cli; hlt");
    }
}