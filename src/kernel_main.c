#include <drivers/uart/uart.h>
#include <limine.h>
extern void load_gdt();
void kernel_main()
{
    initialize_uart_port(COM1, BAUD_115200);
    send_data_uart(COM1, 'a');
    load_gdt();
    while (1)
    {
        asm volatile ("cli; hlt");
    }
}