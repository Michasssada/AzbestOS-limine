#include <stdint.h>
#include <drivers/cpu/APIC/apic.h>
#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_ENABLE 0x800
#define LAPIC_BASE_ADDRESS 0xFEE00000

// MSR read/write functions
static inline void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
    asm volatile ("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

static inline void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi) {
    asm volatile ("wrmsr" : : "c"(msr), "a"(lo), "d"(hi));
}

// Enable LAPIC
void enable_apic() {
    uint32_t eax, edx;
    cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);
    eax |= IA32_APIC_BASE_MSR_ENABLE;
    cpuSetMSR(IA32_APIC_BASE_MSR, eax, edx);
}

// LAPIC register access
volatile uint32_t* lapic = (volatile uint32_t*)LAPIC_BASE_ADDRESS;

#define LAPIC_REG(offset) (*(volatile uint32_t*)((uintptr_t)lapic + offset))

// LAPIC register offsets
#define LAPIC_SPURIOUS_REG 0xF0
#define LAPIC_EOI_REG      0xB0
#define LAPIC_LVT_TIMER    0x320
#define LAPIC_TIMER_INIT   0x380
#define LAPIC_TIMER_CURR   0x390
#define LAPIC_TIMER_DIV    0x3E0

#define APIC_TIMER_PERIODIC (1 << 17)
#define APIC_LVT_MASKED     (1 << 16)

// Calibrate LAPIC timer (simplified example)
uint32_t lapic_calibrate_timer() {
    // Set divide configuration to 16
    LAPIC_REG(LAPIC_TIMER_DIV) = 0x3;

    // Set LVT Timer to one-shot mode and mask it
    LAPIC_REG(LAPIC_LVT_TIMER) = APIC_LVT_MASKED;

    // Set initial count to maximum
    LAPIC_REG(LAPIC_TIMER_INIT) = 0xFFFFFFFF;

    // Wait for a short duration (e.g., using PIT or another timer)
    // This part is platform-specific and requires implementation

    // Read current count
    uint32_t ticks = 0xFFFFFFFF - LAPIC_REG(LAPIC_TIMER_CURR);

    return ticks;
}

// Start LAPIC timer
void start_lapic_timer(uint8_t vector, uint32_t ticks) {
    // Set divide configuration to 16
    LAPIC_REG(LAPIC_TIMER_DIV) = 0x3;

    // Set LVT Timer to periodic mode with specified vector
    LAPIC_REG(LAPIC_LVT_TIMER) = vector | APIC_TIMER_PERIODIC;

    // Set initial count
    LAPIC_REG(LAPIC_TIMER_INIT) = ticks;
}

// Send End-of-Interrupt (EOI)
void lapic_send_eoi() {
    LAPIC_REG(LAPIC_EOI_REG) = 0;
}