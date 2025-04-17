#include <drivers/cpu/IOAPIC/ioapic.h>

#define IOAPIC_BASE_PHYS 0xFEC00000  // Typically obtained from ACPI MADT

volatile uint32_t* ioapic_base = (volatile uint32_t*)IOAPIC_BASE_PHYS;

static inline uint32_t ioapic_read(uint8_t reg) {
    ioapic_base[0] = reg;
    return ioapic_base[4];
}

static inline void ioapic_write(uint8_t reg, uint32_t value) {
    ioapic_base[0] = reg;
    ioapic_base[4] = value;
}
void ioapic_set_redirection(uint8_t irq, uint8_t vector, uint8_t apic_id) {
    uint8_t index = 0x10 + (irq * 2);

    uint32_t low = vector; // Set interrupt vector
    // Configure delivery mode (Fixed), destination mode (Physical), and unmask
    low |= (0 << 8);   // Delivery Mode: Fixed
    low |= (0 << 11);  // Destination Mode: Physical
    low |= (0 << 16);  // Mask: 0 = enabled

    uint32_t high = ((uint32_t)apic_id) << 24;

    ioapic_write(index, low);
    ioapic_write(index + 1, high);
}
void ioapic_init() {
    // Example: Map IOAPIC base if paging is enabled
    // ioapic_base = map_physical_memory(IOAPIC_BASE_PHYS, size);

    // Configure IRQ 1
    ioapic_set_redirection(1, 0x21, 0);
}
