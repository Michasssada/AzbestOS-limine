#include "dbg/print.h"

#include <drivers/cpu/gdt/gdt.h>
#include <drivers/cpu/idt/idt.h>
#include <drivers/display/display.h>
#include <drivers/uart/uart.h>
#include <limine.h>

void kernel_main()
{

    initialize_gdt();
    initialize_idt();
    init_framebuffer();
    clear_screen(0x000000);
    draw_string("Hello, World!", 10, 10, 0xFFFFFF);

    while (1)
    {
        //asm volatile ("cli; hlt");
    }
}