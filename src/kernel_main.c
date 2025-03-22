#include <drivers/uart/uart.h> 
#include <drivers/display/display.h>
#include <limine.h>
extern void load_gdt();

void kernel_main()
{
    initialize_uart_port(COM1, BAUD_115200);
    send_data_uart(COM1, 'a');
    load_gdt();
    initFramebuffer();
    clearScreen(0x000000); 
    for(int i = 0; i < 1000; i++) {
        putPixel(i, i, 0xFFFFFF);
    }

    while (1)
    {
        //asm volatile ("cli; hlt");
    }
}