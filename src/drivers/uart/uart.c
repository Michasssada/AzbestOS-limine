#include <drivers/cpu/io.h>
#include "uart.h"

void com_set_dlab(uint16_t port) {
    uint8_t lcr = inb(port + 3);
    lcr |= (1 << 7);
    outb(port + 3, lcr);
}

void com_clear_dlab(uint16_t port) {
    uint8_t lcr = inb(port + 3);
    lcr &= ~(1 << 7);
    outb(port + 3, lcr);
}

void com_transmit_buffer_send(uint16_t port, uint8_t data) {
    com_clear_dlab(port);
    // Wait until Transmit Holding Register is empty
    while ((inb(port + 5) & (1 << 5)) == 0);
    outb(port, data);
}

void com_set_baud_rate(uint16_t port, uint16_t divisor) {
    com_set_dlab(port);
    union {
        struct {
            uint8_t low;
            uint8_t high;
        } parts;
        uint16_t div;
    } div;
    div.div = divisor;
    outb(port, div.parts.low);
    outb(port + 1, div.parts.high);
    com_clear_dlab(port); // Clear DLAB after setting divisor
}

void initialize_uart_port(enum UART_COM_PORT port, enum UART_BAUD_RATE baud) {
    uint16_t com_base = port;
    uint16_t baud_divisor = baud;

    // Disable interrupts (IER)
    outb(com_base + 1, 0x00);

    // Set baud rate (includes setting and clearing DLAB)
    com_set_baud_rate(com_base, baud_divisor);

    // Configure LCR: 8 data bits, no parity, 1 stop bit (8N1)
    uint8_t lcr = 0x03;
    outb(com_base + 3, lcr);

    // Enable and clear FIFO
    outb(com_base + 2, 0x07); // Enable FIFO, clear TX/RX queues, 14-byte threshold
}

void send_data_uart(enum UART_COM_PORT port, uint8_t data) {
    com_transmit_buffer_send(port, data);
}