#include <stdint.h>
typedef struct {
    uint8_t vector;          // Interrupt vector
    uint8_t delivery_mode:3;
    uint8_t dest_mode:1;
    uint8_t delivery_status:1;
    uint8_t polarity:1;
    uint8_t remote_irr:1;
    uint8_t trigger_mode:1;
    uint8_t mask:1;
    uint32_t reserved:15;
    uint8_t destination;
} __attribute__((packed)) ioapic_redir_entry_t;
void ioapic_init();